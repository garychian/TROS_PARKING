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

#ifndef SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_SEMPHORE_H_
#define SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_SEMPHORE_H_

#include "schedulegroup/coroutine_scheduler/sync_condvar.h"
#include "schedulegroup/coroutine_scheduler/sync_mutex.h"

namespace hobot {
namespace schedulegroup {

class CoSemphore {
 public:
  explicit CoSemphore(uint32_t num) { m_num_ = num; }
  ~CoSemphore() = default;
  CoSemphore(const CoSemphore &) = delete;
  CoSemphore &operator=(const CoSemphore &) = delete;

  void wait();
  void signal();

 private:
  int32_t m_num_;
  CoCondVar m_condvar_;
  CoMutex m_mutex_;
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_COROUTINE_SCHEDULER_SYNC_SEMPHORE_H_
