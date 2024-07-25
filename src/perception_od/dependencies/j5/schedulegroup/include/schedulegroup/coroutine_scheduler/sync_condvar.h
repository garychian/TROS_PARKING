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

#ifndef SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_CONDVAR_H_
#define SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_CONDVAR_H_

#include <chrono>
#include <functional>
#include <map>
#include <queue>

#include "schedulegroup/coroutine_scheduler/sync_mutex.h"

namespace hobot {
namespace schedulegroup {

enum class CoCvStatus { NoTimeout, Timeout };

class Coroutine;
class CoCondVar {
  using WaitFunc = std::function<bool()>;
  using TimePoint = std::chrono::steady_clock::time_point;

 public:
  CoCondVar() = default;
  ~CoCondVar() = default;
  CoCondVar(const CoCondVar &) = delete;
  CoCondVar &operator=(const CoCondVar &) = delete;

  void notify_one();
  void notify_all();
  void wait(CoMutex &lock, WaitFunc func);
  CoCvStatus wait_for(CoMutex &mtx, uint64_t timeout_us);
  CoCvStatus wait_for(CoMutex &mtx, std::chrono::microseconds timeout_us);
  bool wait_for(CoMutex &mtx, std::chrono::microseconds timeout, WaitFunc func);
  bool wait_for(CoMutex &mtx, uint64_t timeout_us, WaitFunc func);
  void wait(CoMutex &mtx);
  CoCvStatus wait_until(CoMutex &mtx, uint64_t abstract_us);
  CoCvStatus wait_until(CoMutex &mtx, TimePoint abstract_us);
  bool wait_until(CoMutex &mtx, uint64_t abstract_us, WaitFunc func);
  bool wait_until(CoMutex &mtx, TimePoint abstract_us, WaitFunc func);

 private:
  bool LockAcquire() { return !lock_.test_and_set(); }

  void LockRelease() { return lock_.clear(); }
  std::queue<Coroutine *> wait_queues_;
  SpinLock guard_mutex_;
  std::map<Coroutine *, std::chrono::steady_clock::time_point> timer_queues_;
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_CONDVAR_H_
