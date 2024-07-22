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

#ifndef COMMUNICATION_WAIT_SET_H_
#define COMMUNICATION_WAIT_SET_H_

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>

#include "communication/common/comm_log.h"
#include "communication/common/errors.h"
#include "communication/wait_set/guard_condition.h"
#include "communication/wait_set/scope_guard.h"
#include "communication/wait_set/subscriber_base.h"
#include "communication/wait_set/wait_result.h"
#include "communication/wait_set/write_preferring_rwlock_for_waitset.h"

namespace hobot {
namespace communication {

class DLLEXPORT WaitSet final {
 public:
  using SubscribersIterable = std::vector<std::shared_ptr<SubscriberBase>>;
  using SubscribersWeakIterable = std::vector<std::weak_ptr<SubscriberBase>>;
  using GuardConditionsIterable = std::vector<std::shared_ptr<GuardCondition>>;
  using GuardConditionsWeakIterable =
      std::vector<std::weak_ptr<GuardCondition>>;

  explicit WaitSet(const SubscribersIterable &subscriptions = {},
                   const GuardConditionsIterable &guard_conditions = {});
  ~WaitSet();

  ErrorCode AddSubscriber(std::shared_ptr<SubscriberBase> subscriber);
  ErrorCode RemoveSubscriber(std::shared_ptr<SubscriberBase> subscriber);
  ErrorCode AddGuardCondition(std::shared_ptr<GuardCondition> guard_condition);
  ErrorCode RemoveGuardCondtion(
      std::shared_ptr<GuardCondition> guard_condition);
  ErrorCode RemoveDeletedEntities();
  template <class Rep = int64_t, class Period = std::milli>
  WaitResult Wait(std::chrono::duration<Rep, Period> time_to_wait =
                      std::chrono::duration<Rep, Period>(-1));

 private:
  friend WaitResult;
  struct NotifySyncronizer {
    std::mutex mutex;
    std::condition_variable cv;
  };

 private:
  WaitResult WaitNano(std::chrono::nanoseconds timeout);
  ErrorCode wait(std::chrono::nanoseconds timeout);
  void AcquireOwnerships();
  void ReleaseOwnerships();
  void InterruptWaitingWaitSet();
  std::chrono::nanoseconds CalculateRemainingTime(
      const std::chrono::time_point<std::chrono::steady_clock,
                                    std::chrono::nanoseconds> &start_time,
      const std::chrono::nanoseconds &total_time);
  // called by WaitResult
  void WaitResultAcquire(WaitResult &wait_result);
  void WaitResultRelease(WaitResult &wait_result);

 private:
  SubscribersWeakIterable subscribers_;
  SubscribersIterable shared_subscribers_;
  GuardConditionsWeakIterable guard_conditions_;
  GuardConditionsIterable shared_guard_conditions_;
  std::shared_ptr<GuardCondition> extra_guard_condition_;
  WritePrefRWLockForWS wsrw_lock_;
  std::vector<SubscriberBase *> sub_ret_;
  std::vector<GuardCondition *> gc_ret_;
  NotifySyncronizer synchronizer_;
  std::mutex check_is_waiting_mutex_;
  bool is_waiting_{false};
  bool wait_result_holding_{false};
  size_t ownership_reference_counter_{0u};
};

template <typename Rep, typename Period>
WaitResult WaitSet::Wait(std::chrono::duration<Rep, Period> time_to_wait) {
  auto time_to_wait_ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(time_to_wait);
  return WaitNano(time_to_wait_ns);
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_WAIT_SET_H_
