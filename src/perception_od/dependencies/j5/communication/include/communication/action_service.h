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

// This is action service interface definition.

#ifndef COMMUNICATION_ACTION_SERVICE_H_
#define COMMUNICATION_ACTION_SERVICE_H_

#include <memory>
#include <string>

#include "communication/action/action_service_impl.h"
#include "communication/common/types.h"
#include "message/message.h"
#ifdef SUPPORT_DDS
#include "communication/action/action_psm_dds_service_impl.h"
#endif

namespace hobot {
namespace communication {

template<typename SerializerGoal, typename SerializerFeedback,
    typename SerializerResult>
class ActionService {
 public:
  using SG = SerializerGoal;
  using SF = SerializerFeedback;
  using SR = SerializerResult;

  using GoalMsg = typename SG::MsgType;

  using GoalHandle = ServiceGoalHandle<SG, SF, SR>;

  using SelfType = ActionService<SG, SF, SR>;

  using GoalCallback = typename ActionServiceImpl<SG, SF, SR>::GoalCallback;

  using CancelCallback = typename ActionServiceImpl<SG,
                                                    SF,
                                                    SR>::CancelCallback;

  using AcceptedCallback =
  typename ActionServiceImpl<SG, SF, SR>::AcceptedCallback;

 public:
  static auto New(const CommAttr &comm_attr, const std::string &action_name,
                  int domain_id, const GoalCallback &handle_goal,
                  const CancelCallback &handle_cancel,
                  const AcceptedCallback &handle_accepted,
                  ProtocolType type = PROTOCOL_HYBRID,
                  ErrorCode *error_code = nullptr,
                  const ConnEventCallback &event_callback = nullptr)
  -> std::shared_ptr<SelfType> {
    COHLOGI("Create action_service topic:{} domain_id:{} type:{}",
             action_name, domain_id, type);
    bool is_action_cb_null{!handle_goal || !handle_cancel || !handle_accepted};
    auto ret = PreConstructActionServiceCheck(comm_attr, action_name, domain_id,
                                              type, is_action_cb_null);
    if (ret != COMM_CODE_OK) {
      if (error_code) *error_code = ret;
      return nullptr;
    }
    auto
        *raw_ptr = new SelfType(comm_attr, action_name, domain_id, handle_goal,
                                handle_cancel, handle_accepted, type);
    auto ptr = std::shared_ptr<SelfType>(raw_ptr);
    ptr->SetEventListener(event_callback);
    ret = ptr->Init();
    if (error_code != nullptr) {
      *error_code = ret;
    }
    if (ret == COMM_CODE_OK) {
      COHLOGI("Create action_service successfully, topic:{}", action_name);
    }
    return (ret == COMM_CODE_OK) ? ptr : nullptr;
  }

  ~ActionService() { Destroy(); }

 private:
  ActionService(const CommAttr &comm_attr, const std::string &action_name,
                int domain_id, const GoalCallback &handle_goal,
                const CancelCallback &handle_cancel,
                const AcceptedCallback &handle_accepted, ProtocolType type) {
    impl_.reset(new ActionServiceImpl<SG, SF, SR>(
        comm_attr, action_name, domain_id, handle_goal, handle_cancel,
        handle_accepted, type));
  }

  ErrorCode Init() { return impl_->Init(); }

  void SetEventListener(const ConnEventCallback &cb) {
    impl_->SetEventListener(cb);
  }

  ErrorCode Destroy() {
    COHLOGD("Destroy action_service");
    impl_.reset();
    return COMM_CODE_OK;
  }

 private:
  std::shared_ptr<ActionServiceImpl<SG, SF, SR>> impl_;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_ACTION_SERVICE_H_
