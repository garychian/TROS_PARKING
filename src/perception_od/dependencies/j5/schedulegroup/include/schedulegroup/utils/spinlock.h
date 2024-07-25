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

#ifndef SCHEDULEGROUP_TIME_AFFI_SCHEDULER_SPINLOCK_H_
#define SCHEDULEGROUP_TIME_AFFI_SCHEDULER_SPINLOCK_H_

#include <atomic>
#include <thread>

namespace hobot {
namespace schedulegroup {

constexpr uint32_t default_spinlock_retry = 10;

class SpinLock {
 public:
  SpinLock() {}
  ~SpinLock() {}

  void Lock() {
    bool expected = false;
    uint32_t retry = 0;
    do {
      if (lock_.compare_exchange_weak(expected, true, std::memory_order_acq_rel,
                                      std::memory_order_acquire)) {
        /**
         * Use memory_order_acquire in failure
         * Ensure expected get the actual value
         */
        break;
      } else {
        if (expected) {
          /// Lock occupied by other threads
          expected = false;
          if (default_spinlock_retry <= ++retry) {
            std::this_thread::yield();
            retry = 0;
          }
        } else {
          /**
           * The defect of compare_exchange_weak
           * compare fail even if spTask_queue_lock_==expected
           */
        }
      }
    } while (1);
  }

  void Unlock() { lock_.store(false, std::memory_order_release); }

 private:
  std::atomic<bool> lock_{false};
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_TIME_AFFI_SCHEDULER_SPINLOCK_H_
