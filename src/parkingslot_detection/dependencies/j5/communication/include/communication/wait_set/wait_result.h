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

#ifndef COMMUNICATION_WAIT_RESULT_H_
#define COMMUNICATION_WAIT_RESULT_H_

#include <cassert>
#include <functional>
#include <stdexcept>
#include <memory>
#include <vector>

#include "communication/common/errors.h"
#include "communication/wait_set/subscriber_base.h"
#include "communication/wait_set/guard_condition.h"

namespace hobot {
namespace communication {
class WaitSet;
struct ReadyObjects {
  std::vector<SubscriberBase *> * subscribers = nullptr;
  std::vector<GuardCondition *> * guard_conditions = nullptr;
};

class DLLEXPORT WaitResult final {
 public:
  explicit WaitResult(ErrorCode code);
  explicit WaitResult(WaitSet * wait_set);
  WaitResult(WaitResult &&);
  ~WaitResult();

  ErrorCode What();
  const ReadyObjects & Result();

 private:
  WaitSet * wait_set_ = nullptr;
  ErrorCode error_code_ = COMM_CODE_WAITSET_SOMETHING_ELSE_WRONG;
  ReadyObjects result_;

  friend class WaitSet;
  // used by WaitSet
  void SetErrorCode(ErrorCode code);
  void SetResult(
              std::vector<SubscriberBase *> * subscribers,
              std::vector<GuardCondition *> * guard_conditions);
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_WAIT_RESULT_H_
