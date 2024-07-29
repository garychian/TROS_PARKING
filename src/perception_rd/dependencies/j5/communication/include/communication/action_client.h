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

// This is action client interface definition.

#ifndef COMMUNICATION_ACTION_CLIENT_H_
#define COMMUNICATION_ACTION_CLIENT_H_

#include <memory>
#include <string>

#include "communication/action/action_client_impl.h"
#include "communication/common/action_types.h"
#include "communication/common/types.h"
#include "message/message.h"
#ifdef SUPPORT_DDS
#include "communication/action/action_psm_dds_client_impl.h"
#endif

namespace hobot {
namespace communication {

template <typename SerializerGoal, typename SerializerFeedback,
          typename SerializerResult>
class ActionClient {
 public:
  using SG = SerializerGoal;
  using SF = SerializerFeedback;
  using SR = SerializerResult;

  using SelfType = ActionClient<SG, SF, SR>;

  using GoalMsg = typename SG::MsgType;
  using ResultMsg = typename SR::MsgType;
  using FeedbackMsg = typename SF::MsgType;

  using WrappedResult = WrappedActionResult<SR>;

  using GoalHandle = ClientGoalHandle<SG, SF, SR>;

  using GoalResponseCallback =
  typename ActionClientImpl<SG, SF, SR>::GoalResponseCallback;

  using ResultCallback = typename ActionClientImpl<SG, SF, SR>::ResultCallback;

  using FeedbackCallback =
  typename ActionClientImpl<SG, SF, SR>::FeedbackCallback;

  using CancelCallback = typename ActionClientImpl<SG, SF, SR>::CancelCallback;

 public:
  static auto New(const CommAttr &comm_attr, const std::string &action_name,
                  int domain_id, ProtocolType type = PROTOCOL_HYBRID,
                  ErrorCode *error_code = nullptr,
                  const ConnEventCallback &event_callback = nullptr)
  -> std::shared_ptr<SelfType> {
    COHLOGI("Create action_client topic:{}, domain_id:{}, type:{}",
             action_name, domain_id, type);
    auto ret =
        PreConstructActionClientCheck(comm_attr, action_name, domain_id, type);
    if (ret != COMM_CODE_OK) {
      if (error_code) *error_code = ret;
      return nullptr;
    }
    auto *raw_ptr = new SelfType(comm_attr, action_name, domain_id, type);
    auto ptr = std::shared_ptr<SelfType>(raw_ptr);
    ptr->SetEventListener(event_callback);
    ret = ptr->Init();
    if (error_code != nullptr) {
      *error_code = ret;
    }
    if (ret == COMM_CODE_OK) {
      COHLOGI("Create action_client successfully,topic:{}", action_name);
    }
    return (ret == COMM_CODE_OK) ? ptr : nullptr;
  }

  std::shared_future<std::shared_ptr<GoalHandle>> AsyncSendGoal(
      const std::shared_ptr<GoalMsg> &message,
      const GoalResponseCallback &goal_response_callback = nullptr,
      const FeedbackCallback &feedback_callback = nullptr,
      const ResultCallback &result_callback = nullptr) {
    return impl_->AsyncSendGoal(message, goal_response_callback,
                                feedback_callback, result_callback);
  }

  std::shared_future<std::shared_ptr<WrappedResult>> AsyncGetResult(
      const std::shared_ptr<GoalHandle> &handle,
      const ResultCallback &result_callback = nullptr) {
    return impl_->AsyncGetResult(handle, result_callback);
  }

  std::shared_future<std::shared_ptr<CancelResponse>> AsyncCancelGoal(
      const std::shared_ptr<GoalHandle> &handle,
      const CancelCallback &cancel_callback = nullptr) {
    return impl_->AsyncCancelGoal(handle, cancel_callback);
  }

  bool ActionServerIsReady() const { return impl_->ActionServerIsReady(); }

  template<typename RepT = int64_t, typename RatioT = std::milli>
  bool WaitForActionServer(std::chrono::duration<RepT, RatioT> timeout =
  std::chrono::duration<RepT, RatioT>(-1)) {
    return impl_->WaitForActionServerNanoseconds(
        std::chrono::duration_cast<std::chrono::nanoseconds>(timeout));
  }

  ~ActionClient() { Destroy(); }

 private:
  ActionClient(const CommAttr &comm_attr, const std::string &action_name,
               int domain_id, ProtocolType type = PROTOCOL_HYBRID) {
    impl_.reset(new ActionClientImpl<SG, SF, SR>(comm_attr, action_name,
                                                 domain_id, type));
  }

  ErrorCode Init() { return impl_->Init(); }

  void SetEventListener(const ConnEventCallback &cb) {
    impl_->SetEventListener(cb);
  }

  ErrorCode Destroy() {
    COHLOGD("Destroy action_client");
    impl_.reset(nullptr);
    return COMM_CODE_OK;
  }

 private:
  std::unique_ptr<ActionClientImpl<SG, SF, SR>> impl_;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_ACTION_CLIENT_H_
