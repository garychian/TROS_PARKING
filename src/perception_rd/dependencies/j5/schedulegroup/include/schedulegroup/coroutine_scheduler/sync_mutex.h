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

#ifndef SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_MUTEX_H_
#define SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_MUTEX_H_

#include <deque>

#include "schedulegroup/coroutine_scheduler/coroutine_scheduler.h"
#include "schedulegroup/utils/spinlock.h"

namespace hobot {
namespace schedulegroup {
class Coroutine;

class CoMutex {
 public:
  CoMutex() = default;
  ~CoMutex() = default;
  CoMutex(const CoMutex &) = delete;
  CoMutex &operator=(const CoMutex &) = delete;

  bool tryLock();
  void lock();
  void unlock();

 private:
  bool CrLockAcquire() { return !lock_.test_and_set(); }
  void CrLockRelease() { return lock_.clear(); }
  SpinLock queue_mutex_;
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
  std::atomic<uint32_t> lock_cnts_{0};
  Coroutine *locked_coroutine_ = nullptr;
  std::deque<Coroutine *> wait_queues_;
};

}  // namespace schedulegroup
}  // namespace hobot
#endif  // SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_MUTEX_H_
