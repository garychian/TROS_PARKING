// Copyright (c) [2021-2023] [Horizon Robotics][Horizon Bole].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     http: //www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

// This is action client implementation.

#ifndef COMMUNICATION_ACTION_ACTION_CLIENT_IMPL_H_
#define COMMUNICATION_ACTION_ACTION_CLIENT_IMPL_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "communication/common/action_types.h"
#include "communication/common/types.h"
#include "communication/participant/reader.h"
#include "communication/service/client_impl.h"
#include "communication/utils/thread_pool.h"

namespace hobot {
namespace communication {

template<typename SerializerGoal, typename SerializerFeedback,
    typename SerializerResult, typename Enable = void>
class ActionClientImpl;

template <typename SerializerGoal, typename SerializerFeedback,
          typename SerializerResult>
class ClientGoalHandle {
 public:
  using SG = SerializerGoal;
  using SF = SerializerFeedback;
  using SR = SerializerResult;
  friend ActionClientImpl<SG, SF, SR>;

  using GoalMsg = typename SG::MsgType;
  using ResultMsg = typename SR::MsgType;
  using FeedbackMsg = typename SF::MsgType;

  using GoalHandle = ClientGoalHandle<SG, SF, SR>;

  using WrapResultMsg = WrappedActionResult<SR>;
  using WrapResultSer = WrappedActionResultSerializer<SR>;

  using GoalResponseCallback = std::function<void(std::shared_ptr<GoalHandle>)>;

  using ResultCallback = std::function<void(std::shared_ptr<WrapResultMsg>)>;

  using FeedbackCallback = std::function<void(std::shared_ptr<GoalHandle>,
                                              std::shared_ptr<FeedbackMsg>)>;

  using CancelCallback = std::function<void(std::shared_ptr<CancelResponse>)>;

 public:
  const GoalUUID &GetGoalId() { return uuid_; }

 private:
  ClientGoalHandle(const FeedbackCallback &fcb, const ResultCallback &rcb)
      : feedback_callback_(fcb), result_callback_(rcb) {
    result_future_ = result_promise_.get_future();
  }

  void SetUUID(const GoalUUID &uuid) { uuid_ = uuid; }

  void SetResultMsg(const std::shared_ptr<WrapResultMsg> &result_msg) {
    result_promise_.set_value(result_msg);
  }

  std::shared_future<std::shared_ptr<WrapResultMsg>> AsyncGetResult() {
    return result_future_;
  }

  void SetResultCallback(const ResultCallback &rcb) { result_callback_ = rcb; }

  bool AtomicSetResultReqFlag() {
    bool expected{false};
    IsAlreadyResultReq.compare_exchange_strong(expected, true);
    return !expected;
  }

  const FeedbackCallback &GetFeedbackCallback() { return feedback_callback_; }
  const ResultCallback &GetResultCallback() { return result_callback_; }

 private:
  GoalUUID uuid_;
  FeedbackCallback feedback_callback_;
  ResultCallback result_callback_;

  std::shared_future<std::shared_ptr<WrapResultMsg>> result_future_;

  std::promise<std::shared_ptr<WrapResultMsg>> result_promise_;

  std::atomic_bool IsAlreadyResultReq{false};
};

template <typename SerializerGoal, typename SerializerFeedback,
          typename SerializerResult, typename Enable>
class ActionClientImpl {
 public:
  using SG = SerializerGoal;
  using SF = SerializerFeedback;
  using SR = SerializerResult;
  using FeedbackMsg = typename SF::MsgType;
  using GoalMsg = typename SG::MsgType;
  using ResultMsg = typename SR::MsgType;

  friend ClientGoalHandle<SG, SF, SR>;

  using GoalHandle = ClientGoalHandle<SG, SF, SR>;

  using WrapResultMsg = WrappedActionResult<SR>;
  using WrapResultSer = WrappedActionResultSerializer<SR>;

  using GoalResponseCallback =
  typename ClientGoalHandle<SG, SF, SR>::GoalResponseCallback;

  using ResultCallback = typename ClientGoalHandle<SG, SF, SR>::ResultCallback;

  using FeedbackCallback =
  typename ClientGoalHandle<SG, SF, SR>::FeedbackCallback;

  using CancelCallback = typename ClientGoalHandle<SG, SF, SR>::CancelCallback;

  using SelfType = ActionClientImpl<SG, SF, SR>;

 public:
  ActionClientImpl(const CommAttr &comm_attr, const std::string &action_name,
                   int domain_id, ProtocolType type) : type_(type) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    action_name_ = action_name;
    domain_id_ = domain_id;
    type_ = type;
    goal_service_name_ = action_name + action_goal_service_suffix;
    cancel_service_name_ = action_name + action_cancel_service_suffix;
    result_service_name_ = action_name + action_result_service_suffix;
    feedback_topic_ = action_name + action_feedback_topic_suffix;

    std::vector<std::string> action_client_server{
        goal_service_name_, cancel_service_name_, result_service_name_};
    tool_server_ = ToolsServer::Instance();
    tools_service_id_ = tool_server_->AddTopic(
        comm_attr, action_name, domain_id, TOPIC_TYPE::ACTION_CLIENT,
        action_client_server);

    goal_client_.reset(new ClientImpl<SG, GoalResponseSerializer>(
        comm_attr, goal_service_name_, domain_id, type));

    cancel_client_.reset(
        new ClientImpl<CancelRequestSerializer, CancelResponseSerializer>(
            comm_attr, cancel_service_name_, domain_id, type));

    result_client_.reset(new ClientImpl<ResultRequestSerializer,
                                        WrapResultSer>(
        comm_attr, result_service_name_, domain_id, type));

    if (type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
      reader_.reset(CreateDDSReader<SF>(
          comm_attr, feedback_topic_, domain_id,
          std::bind(&SelfType::HandleFeedbackCallback, this, _1, _2), type));
#endif
    } else {
      reader_.reset(new Reader<SF>(
          comm_attr, feedback_topic_, domain_id,
          std::bind(&SelfType::HandleFeedbackCallback, this, _1, _2), type));
    }
  }

  ~ActionClientImpl() {
    if (tool_server_ != nullptr) {
      tool_server_->RemoveTopic(tools_service_id_);
    }
    goal_client_.reset();
    cancel_client_.reset();
    result_client_.reset();
    reader_.reset();
    if (discovery_manager_ != nullptr && !entity_uuid_.empty()) {
      discovery_manager_->un_regist_entity(entity_uuid_);
    }
  }

  ErrorCode Init() {
    if (reader_ == nullptr) {
      return COMM_CODE_INIT_FAILED;
    }
    discovery_manager_ = DiscoveryManager::instance();
    entity_uuid_ = discovery_manager_->regist_entity(
      EntityType::ENTITY_ACTION_CLIENT, action_name_, domain_id_, type_);
    ErrorCode ret{COMM_CODE_OK};
    bool bool_ret = ((ret = goal_client_->Init()) == COMM_CODE_OK) &&
        ((ret = cancel_client_->Init()) == COMM_CODE_OK) &&
        ((ret = result_client_->Init()) == COMM_CODE_OK) &&
        ((ret = reader_->Init()) == COMM_CODE_OK);
    return bool_ret ? COMM_CODE_OK : ret;
  }
  void SetEventListener(const ConnEventCallback &cb) {
    auto conn_event_cb = [cb, this](
        const std::shared_ptr<LinkInfo> participants,
        EventType event) {
      ToolsServer::Instance()->AddEvent(
          tools_service_id_, event, TOPIC_TYPE::ACTION_CLIENT);
      if (cb != nullptr) {
        cb(participants, event);
      }
    };
    goal_client_->SetEventListener(conn_event_cb);
    cancel_client_->SetEventListener(conn_event_cb);
    result_client_->SetEventListener(conn_event_cb);
    reader_->SetEventListener(conn_event_cb);
  }

  std::shared_future<std::shared_ptr<GoalHandle>> AsyncSendGoal(
      const std::shared_ptr<GoalMsg> &message,
      const GoalResponseCallback &goal_response_callback = nullptr,
      const FeedbackCallback &feedback_callback = nullptr,
      const ResultCallback &result_callback = nullptr) {
    auto promise =
        std::make_shared<std::promise<std::shared_ptr<GoalHandle>>>();
    std::shared_future<std::shared_ptr<GoalHandle>> future{
        promise->get_future()};

    auto callback = [this, goal_response_callback, feedback_callback,
        result_callback,
        promise](int error_code,
                 const std::shared_ptr<GoalResponse> &resp) {
      if (error_code != COMM_CODE_OK) {
        COHLOGE("AsyncSendGoal callback err, errcode:{}", error_code);
        promise->set_value(nullptr);
        if (goal_response_callback) {
          // ThreadPoolAT::Instance()->AddTask(
          // goal_response_callback, nullptr);
          goal_response_callback(nullptr);
        }
      } else {
      #ifdef SUPPORT_DDS
        if (PROTOCOL_DDS == type_) {
          resp->uuid_.val_ = resp->dds_data.uuid();
          resp->code_ = (resp->dds_data.code() == DDSActionCode::ACCEPT
                             ? GoalResponse::Code::ACCEPT
                             : GoalResponse::Code::REJECT);
        }
      #endif
        if (resp->code_ == GoalResponse::Code::REJECT) {
          promise->set_value(nullptr);
          if (goal_response_callback) {
            // ThreadPoolAT::Instance()->AddTask(goal_response_callback,
            // nullptr);
            goal_response_callback(nullptr);
          }
        } else {
          COHLOGD("GoalResponse uuid: {}", resp->uuid_.val_);
          auto goal_handle =
              std::shared_ptr<GoalHandle>(new GoalHandle(feedback_callback,
                                                         result_callback));
          goal_handle->SetUUID(resp->uuid_);
          promise->set_value(goal_handle);
          goal_handles_[goal_handle->GetGoalId()] = goal_handle;
          if (goal_response_callback) {
            // ThreadPoolAT::Instance()->AddTask(goal_response_callback,
            // goal_handle);
            goal_response_callback(goal_handle);
          }
          if (result_callback) {
            MakeResultRequest(goal_handle);
          }
        }
      }
    };
    goal_client_->AsyncSendRequest(message, callback);
    return future;
  }

  std::shared_future<std::shared_ptr<WrapResultMsg>> AsyncGetResult(
      const std::shared_ptr<GoalHandle> &goal_handle,
      const ResultCallback &result_callback = nullptr) {
    if (result_callback) {
      goal_handle->SetResultCallback(result_callback);
    }
    MakeResultRequest(goal_handle);
    return goal_handle->AsyncGetResult();
  }

  std::shared_future<std::shared_ptr<CancelResponse>> AsyncCancelGoal(
      const std::shared_ptr<GoalHandle> &handle,
      const CancelCallback &cancel_callback = nullptr) {
    auto promise =
        std::make_shared<std::promise<std::shared_ptr<CancelResponse>>>();
    std::shared_future<std::shared_ptr<CancelResponse>> future{
        promise->get_future()};

    auto callback = [handle, cancel_callback, promise, this](
        int error_code,
        const std::shared_ptr<CancelResponse> &resp) {
      if (error_code != COMM_CODE_OK) {
        COHLOGE("AsyncCancelGoal callback err, err_code: {}", error_code);
        promise->set_value(nullptr);
        if (cancel_callback) {
          // ThreadPoolAT::Instance()->AddTask(cancel_callback, nullptr);
          cancel_callback(nullptr);
        }
      } else {
        #ifdef SUPPORT_DDS
        if (PROTOCOL_DDS == type_) {
          resp->code_ = (resp->dds_data.code() == DDSActionCode::ACCEPT
                             ? CancelResponse::Code::ACCEPT
                             : CancelResponse::Code::REJECT);
        }
        #endif
        promise->set_value(resp);
        if (cancel_callback) {
          // ThreadPoolAT::Instance()->AddTask(cancel_callback, resp);
          cancel_callback(resp);
        }
      }
    };

    auto cancel_req = std::make_shared<CancelRequest>();
    if (handle) {
      cancel_req->uuid_ = handle->GetGoalId();
      #ifdef SUPPORT_DDS
      if (PROTOCOL_DDS == type_) {
        cancel_req->dds_data.uuid() = cancel_req->uuid_.val_;
      }
      #endif
      cancel_client_->AsyncSendRequest(cancel_req, callback);
    } else {
      COHLOGE("handle is nullptr");
    }
    return future;
  }

  bool ActionServerIsReady() const {
    return goal_client_->ServiceIsReady() && cancel_client_->ServiceIsReady()
        && result_client_->ServiceIsReady() && reader_->IsReady();
  }

  bool WaitForActionServerNanoseconds(std::chrono::nanoseconds time_out) {
    bool action_server_is_ready{false};
    auto step_duration = std::chrono::nanoseconds(5 * 1000 * 1000);
    while (time_out.count() > 0) {
      action_server_is_ready = ActionServerIsReady();
      if (!action_server_is_ready) {
        std::this_thread::sleep_for(step_duration);
        time_out -= step_duration;
      } else {
        break;
      }
    }
    return action_server_is_ready;
  }

 private:
  void HandleFeedbackCallback(const std::shared_ptr<FeedbackMsg> &feedback_msg,
                              const spMessageInfo &msg_info) {
    GoalUUID uuid;
    uuid.val_ = msg_info->GetSpareId();
#ifdef SUPPORT_DDS
    if (type_ == PROTOCOL_DDS && uuid.val_ == 0) {
      auto dds_ext = std::dynamic_pointer_cast<DDSExtInfo>(msg_info);
      if (dds_ext == nullptr) {
        return;
      }
      uuid.val_ = dds_ext->GetRelativeSeqId().high;
      uuid.val_ = (uuid.val_ << 32) | (dds_ext->GetRelativeSeqId().low);
      COHLOGD("HandleFeedbackCallback uuid.val_ is{}", uuid.val_);
    }
#endif

    if (goal_handles_.count(uuid) == 0) {
      COHLOGD("Received feedback for unknown goal. Ignoring uuid:{}",
               uuid.val_);
      return;
    }
    auto handle = goal_handles_[uuid];
    auto &callback = goal_handles_[uuid]->GetFeedbackCallback();
    if (callback) {
      // ThreadPoolAT::Instance()->AddTask(callback, handle, feedback_msg);
      callback(handle, feedback_msg);
    }
  }

  void MakeResultRequest(const std::shared_ptr<GoalHandle> &handle) {
    if (!handle->AtomicSetResultReqFlag()) {
      return;
    }
    auto &uuid = handle->GetGoalId();
    auto req_msg = std::make_shared<ResultRequest>();
    req_msg->uuid_ = uuid;
    #ifdef SUPPORT_DDS
    if (PROTOCOL_DDS == type_) {
      req_msg->dds_data.uuid() = req_msg->uuid_.val_;
    }
    #endif
    auto callback = [this, handle](
        int error_code,
        const std::shared_ptr<WrapResultMsg> &wrap_result_msg) {
      if (error_code != COMM_CODE_OK) {
        COHLOGE("AsyncSendRequest callback err, errcode:{}", error_code);
        handle->SetResultMsg(nullptr);
        auto result_cb = handle->GetResultCallback();
        if (result_cb) {
          // ThreadPoolAT::Instance()->AddTask(result_cb, nullptr);
          result_cb(nullptr);
        }
        return;
      }
      if (PROTOCOL_DDS != type_) {
        wrap_result_msg->result->SetData(wrap_result_msg->GetData());
      } else {
     #ifdef SUPPORT_DDS
        wrap_result_msg->ConvertFromDDSUnderlayResult();
      #endif
      }

      handle->SetResultMsg(wrap_result_msg);
      auto result_cb = handle->GetResultCallback();
      if (result_cb) {
        // ThreadPoolAT::Instance()->AddTask(result_cb, wrap_result_msg);
        result_cb(wrap_result_msg);
      }
      goal_handles_.erase(handle->GetGoalId());
    };
    result_client_->AsyncSendRequest(req_msg, callback);
  }

 private:
  ProtocolType type_;

  std::string action_name_;
  std::string goal_service_name_;
  std::string cancel_service_name_;
  std::string result_service_name_;

  GoalResponseCallback goal_response_callback_;
  FeedbackCallback feedback_callback_;
  ResultCallback result_callback_;

  std::shared_ptr<ClientImpl<SG, GoalResponseSerializer>> goal_client_;
  std::shared_ptr<ClientImpl<CancelRequestSerializer,
                             CancelResponseSerializer>>
      cancel_client_;
  std::shared_ptr<ClientImpl<ResultRequestSerializer, WrapResultSer>>
      result_client_;
  std::string feedback_topic_;
  std::shared_ptr<Reader<SF>> reader_;

  std::map<GoalUUID, std::shared_ptr<GoalHandle>> goal_handles_;

  uint64_t tools_service_id_ = 0;
  int domain_id_;
  std::string entity_uuid_;
  DiscoveryManager* discovery_manager_{nullptr};
  ToolsServer* tool_server_{nullptr};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_ACTION_ACTION_CLIENT_IMPL_H_
