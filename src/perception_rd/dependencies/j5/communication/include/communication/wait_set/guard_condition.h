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

#ifndef COMMUNICATION_GUARD_CONDITION_H_
#define COMMUNICATION_GUARD_CONDITION_H_

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include "communication/common/comm_log.h"
namespace hobot {
namespace communication {

class DLLEXPORT GuardCondition final {
 public:
  GuardCondition() = default;
  ~GuardCondition() = default;

  void Trigger();

 private:
  bool ExchangeInUseByWaitSet(bool state);
  void AttachCondition(std::mutex *mutex, std::condition_variable *cv);
  void DetachCondition();
  bool HasTriggered();
  void ResetHasTriggered();

 private:
  friend class WaitSet;
  std::mutex internal_mutex_;
  std::mutex *cv_mutex_{nullptr};
  std::condition_variable *cv_{nullptr};
  std::atomic<bool> is_used_by_waitset_{false};
  std::atomic<bool> has_triggered_{false};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_GUARD_CONDITION_H_
