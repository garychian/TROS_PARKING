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

// This is action service implementation.

#ifndef COMMUNICATION_ACTION_PSM_DDS_ACTION_SERVICE_IMPL_H_
#define COMMUNICATION_ACTION_PSM_DDS_ACTION_SERVICE_IMPL_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "communication/participant/writer.h"
#include "communication/service/service_impl.h"
#include "communication/service/service_psm_dds_impl.h"
#include "communication/common/action_types.h"
#include "communication/common/types.h"
namespace hobot {
namespace communication {


template<typename SerializerGoal, typename SerializerFeedback,
    typename SerializerResult>
class ServiceGoalHandle;

// ActionServiceImpl definition
template <typename SerializerGoal, typename SerializerFeedback,
          typename SerializerResult>
class ActionServiceImpl<SerializerGoal, SerializerFeedback, SerializerResult,
                        typename std::enable_if<HasBole2PSMDDSFlag<
                            typename SerializerFeedback::MsgType>::value>::type>
    : public std::enable_shared_from_this<ActionServiceImpl<
          SerializerGoal, SerializerFeedback, SerializerResult>> {
 public:
  using SG = SerializerGoal;
  using SF = SerializerFeedback;
  using SR = SerializerResult;
  friend ServiceGoalHandle<SG, SF, SR>;

  using GoalHandle = ServiceGoalHandle<SG, SF, SR>;
  using GoalMsg = typename SG::MsgType;
  using FeedbackMsg = typename SF::MsgType;

  using GoalCallback = std::function<GoalResponse(
      const GoalUUID &, const std::shared_ptr<GoalMsg> &)>;

  using CancelCallback =
  std::function<CancelResponse(std::shared_ptr<GoalHandle>)>;

  using AcceptedCallback =
  std::function<void(std::shared_ptr<GoalHandle>)>;

  using WrapResultMsg = WrappedActionResult<SR>;
  using WrapResultSer = WrappedActionResultSerializer<SR>;

  using SelfType = ActionServiceImpl<SG, SF, SR>;

 public:
  ActionServiceImpl(const CommAttr &comm_attr, const std::string &action_name,
                    int domain_id, const GoalCallback &handle_goal,
                    const CancelCallback &handle_cancel,
                    const AcceptedCallback &handle_accepted,
                    ProtocolType type) {
    COHLOGD("Create action_psm_dds_service_impl,"
            " topic={}, domain_id={}, type={}", action_name, domain_id, type);
    type_ = type;
    domain_id_ = domain_id;
    action_name_ = action_name;
    goal_service_name_ = action_name + action_goal_service_suffix;
    cancel_service_name_ = action_name + action_cancel_service_suffix;
    result_service_name_ = action_name + action_result_service_suffix;
    feedback_topic_ = action_name + action_feedback_topic_suffix;

    std::vector<std::string> action_client_server{
        goal_service_name_,
        cancel_service_name_,
        result_service_name_};
    tool_server_ = ToolsServer::Instance();
    tools_service_id_ = tool_server_->AddTopic(
        comm_attr,
        action_name, domain_id,
        TOPIC_TYPE::ACTION_SERVICE,
        action_client_server);

    handle_goal_ = handle_goal;
    handle_cancel_ = handle_cancel;
    handle_accepted_ = handle_accepted;

    using std::placeholders::_1;
    using std::placeholders::_2;
    goal_service_.reset(new ServiceImpl<SG, GoalResponseSerializer>(
        comm_attr, goal_service_name_, domain_id,
        std::bind(&SelfType::HandleGoalService, this, _1, _2), type));

    cancel_service_.reset(
        new ServiceImpl<CancelRequestSerializer, CancelResponseSerializer>(
            comm_attr, cancel_service_name_, domain_id,
            std::bind(&SelfType::HandleCancelService, this, _1, _2), type));

    // As HandleResultService will blocked wait for result,
    // we need tell ServiceImpl to handle this type callback
    result_service_.reset(
        new ServiceImpl<ResultRequestSerializer, WrapResultSer>(
            comm_attr, result_service_name_, domain_id,
            std::bind(&SelfType::HandleResultService, this, _1, _2), type,
            true));

    if (type == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
      dds_writer_.reset(CreateDDSWriter<typename SF::DDSCSSupportType>(
        comm_attr, feedback_topic_, domain_id, type));
#else
      dds_writer_.reset();
#endif
    } else {
      writer_.reset(
          new Writer<SF>(comm_attr, feedback_topic_, domain_id, type));
    }
  }

  ~ActionServiceImpl() {
    if (tool_server_ != nullptr) {
      tool_server_->RemoveTopic(tools_service_id_);
    }
    goal_service_.reset();
    cancel_service_.reset();
    result_service_.reset();
    if (type_ == PROTOCOL_DDS) {
      dds_writer_.reset();
    } else {
      writer_.reset();
    }

    if (discovery_manager_ != nullptr && !entity_uuid_.empty()) {
      discovery_manager_->un_regist_entity(entity_uuid_);
    }
  }

  ErrorCode Init() {
    discovery_manager_ = DiscoveryManager::instance();
    entity_uuid_ = discovery_manager_->regist_entity(
      EntityType::ENTITY_ACTION_SERVICE, action_name_, domain_id_, type_);
    ErrorCode ret{COMM_CODE_OK};
    ErrorCode writer_ret{COMM_CODE_OK};
    if (type_ == PROTOCOL_DDS) {
      if (!dds_writer_) {
        COHLOGE("dds_writer_ is nullptr");
        return COMM_CODE_PARAM_IS_NULL;
      }
      writer_ret = dds_writer_->Init();
    } else {
      writer_ret = writer_->Init();
    }

    bool bool_ret = ((ret = goal_service_->Init()) == COMM_CODE_OK) &&
        ((ret = cancel_service_->Init()) == COMM_CODE_OK) &&
        ((ret = result_service_->Init()) == COMM_CODE_OK) &&
        (writer_ret == COMM_CODE_OK);
    return bool_ret ? COMM_CODE_OK : ret;
  }

  void SetEventListener(const ConnEventCallback &cb) {
    auto conn_event_cb = [cb, this](
        const std::shared_ptr<LinkInfo> participants,
        EventType event) {
      ToolsServer::Instance()->AddEvent(
          tools_service_id_, event, TOPIC_TYPE::ACTION_SERVICE);
      if (cb != nullptr) {
        cb(participants, event);
      }
    };

    goal_service_->SetEventListener(conn_event_cb);
    cancel_service_->SetEventListener(conn_event_cb);
    result_service_->SetEventListener(conn_event_cb);
    if (type_ == PROTOCOL_DDS) {
      if (!dds_writer_) {
        COHLOGE("dds_writer_ is nullptr");
        return;
      }
      dds_writer_->SetEventListener(conn_event_cb);
    } else {
      writer_->SetEventListener(conn_event_cb);
    }
  }

 private:
  void HandleGoalService(const std::shared_ptr<GoalMsg> &goal_request,
                         std::shared_ptr<GoalResponse> &goal_response) {
    GoalUUID uuid;
    uuid.val_ = UUIDHelper::GenerateUUID();
    COHLOGD("GoalService create uuid:{}", uuid.val_);
    GoalResponse response{handle_goal_(uuid, goal_request)};
    // rewrite uuid into response msg
    response.uuid_ = uuid;
    #ifdef SUPPORT_DDS
    if (PROTOCOL_DDS == type_) {
      response.dds_data.uuid() = uuid.val_;
      response.dds_data.code() = (response.code_ == GoalResponse::Code::ACCEPT
                                      ? DDSActionCode::ACCEPT
                                      : DDSActionCode::REJECT);
    }
    #endif
    if (response.code_ == GoalResponse::Code::REJECT) {
      // make rejected goal response for this goal
      goal_response.reset(new GoalResponse(response));
    } else if (response.code_ == GoalResponse::Code::ACCEPT) {
      // make goal handle for this goal request
      auto handle =
          std::shared_ptr<GoalHandle>(new GoalHandle(this->shared_from_this(),
                                                     goal_request, uuid));
      goal_handles_[uuid] = handle;
      // invoke accepted callback
      auto handle_accepted_cb = [this](std::shared_ptr<GoalHandle> handle) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        handle_accepted_(handle);
      };
      goal_service_->GetExecutor()->AddTask(handle_accepted_cb, handle);
      goal_response.reset(new GoalResponse(response));
    }
  }

  void HandleCancelService(const std::shared_ptr<CancelRequest> &cancel_request,
                           std::shared_ptr<CancelResponse> &cancel_response) {
    #ifdef SUPPORT_DDS
    if (PROTOCOL_DDS == type_) {
      cancel_request->uuid_.val_ = cancel_request->dds_data.uuid();
    }
    #endif
    if (goal_handles_.count(cancel_request->uuid_) == 0) {
      COHLOGW("HandleCancelService can't find uuid:{}",
              cancel_request->uuid_.val_);
      cancel_response.reset(new CancelResponse(CancelResponse::Code::REJECT));
    } else {
      auto handle = goal_handles_[cancel_request->uuid_];
      // invoke cancel callback
      auto response = handle_cancel_(handle);
      if (response.code_ == CancelResponse::Code::REJECT) {
        cancel_response.reset(new CancelResponse(CancelResponse::Code::REJECT));
      } else if (response.code_ == CancelResponse::Code::ACCEPT) {
        handle->RecvCancelGoalReq();
        cancel_response.reset(new CancelResponse(CancelResponse::Code::ACCEPT));
      }
    }
    #ifdef SUPPORT_DDS
    if (PROTOCOL_DDS == type_) {
      cancel_response->dds_data.code() =
          (cancel_response->code_ == CancelResponse::Code::ACCEPT
               ? DDSActionCode::ACCEPT
               : DDSActionCode::REJECT);
    }
    #endif
  }

  void HandleResultService(
      const std::shared_ptr<ResultRequest> &result_request,
      std::shared_ptr<WrapResultMsg> &wrap_result_response) {
    COHLOGD("HandleResultService received result request, uuid: {}",
            result_request->uuid_.val_);
    #ifdef SUPPORT_DDS
    if (PROTOCOL_DDS == type_) {
      result_request->uuid_.val_ = result_request->dds_data.uuid();
    }
    #endif
    if (goal_handles_.count(result_request->uuid_) == 0) {
      COHLOGW("HandleResultService can't find uuid: {}",
              result_request->uuid_.val_);
      wrap_result_response.reset();
    } else {
      auto handle = goal_handles_[result_request->uuid_];
      handle->WaitResult(wrap_result_response);
      // after result finished, delete handle
      goal_handles_.erase(result_request->uuid_);
    }
  }

  void PublishFeedback(GoalHandle *handle,
                       const std::shared_ptr<FeedbackMsg> &feedback_msg) {
    // (maosong): As we did not use spmsg in pub/sub, we can put uuid into it
    // Maybe there is a better way, but I have not find it yet.
    if (type_ == PROTOCOL_DDS) {
      spMessageInfo sp_msg =
          std::make_shared<DDSExtInfo>(0, 0, handle->GetGoalId().val_);
      if (!dds_writer_) {
        COHLOGE("dds_writer_ is nullptr");
        return;
      }
      auto dds_ext_info =
        std::make_shared<typename SF::MsgType::DDSCSMsgType>();
      // TODO(heyao.cai): The current version does not use
      // the move constructor.
      // Add PublishFeedbackMove interface in version 0330,
      // which uses move constructor to improve performance
      auto feedback_send_msg =
        std::make_shared<FeedbackMsg>(*(feedback_msg.get()));
      dds_ext_info->set_comm_user_define_msg(feedback_send_msg.get(), false);
      if (ErrorCode::COMM_CODE_OK != dds_writer_->Write(dds_ext_info, sp_msg)) {
        COHLOGE("writer feedback_msg  failed");
      }
    } else {
      auto sp_msg =
          std::make_shared<MessageInfo>(0, 0, handle->GetGoalId().val_);
      if (ErrorCode::COMM_CODE_OK != writer_->Write(feedback_msg, sp_msg)) {
        COHLOGE("writer feedback_msg  failed");
      }
    }
  }

 private:
  int domain_id_{0};
  int channel_id_{0};
  ProtocolType type_;
  std::string action_name_;
  std::string goal_service_name_;
  std::string cancel_service_name_;
  std::string result_service_name_;

  GoalCallback handle_goal_;
  CancelCallback handle_cancel_;
  AcceptedCallback handle_accepted_;

  std::shared_ptr<ServiceImpl<SG, GoalResponseSerializer>> goal_service_;
  std::shared_ptr<
      ServiceImpl<CancelRequestSerializer, CancelResponseSerializer>>
      cancel_service_;
  std::shared_ptr<ServiceImpl<ResultRequestSerializer, WrapResultSer>>
      result_service_;
  std::string feedback_topic_;
  std::shared_ptr<Writer<typename SF::DDSCSSupportType>> dds_writer_;
  std::shared_ptr<Writer<SF>> writer_;

  std::map<GoalUUID, std::shared_ptr<GoalHandle>> goal_handles_;

  uint64_t tools_service_id_ = 0;
  std::string entity_uuid_;
  DiscoveryManager* discovery_manager_{nullptr};
  ToolsServer* tool_server_{nullptr};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_ACTION_PSM_DDS_ACTION_SERVICE_IMPL_H_

