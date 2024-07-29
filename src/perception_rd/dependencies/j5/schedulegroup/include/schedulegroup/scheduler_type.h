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

#ifndef SCHEDULEGROUP_SCHEDULER_TYPE_H_
#define SCHEDULEGROUP_SCHEDULER_TYPE_H_

#include <cstdint>

namespace hobot {
namespace schedulegroup {
// ScheduleGroup scheduler type
enum class SchedulerType : uint8_t {
  SCHEDULER_TYPE_TIMEAFFI = 0,
  SCHEDULER_TYPE_THREADPOOL,
  SCHEDULER_TYPE_SIMPLE_THREADPOOL,
  SCHEDULER_TYPE_TIMER,
  SCHEDULER_TYPE_DEADLINE,
  SCHEDULER_TYPE_COROUTINE
};

enum SchedulePolicy : int32_t {
  H_SCHED_OTHER = 0,  // Default Linux time-sharing scheduling
  H_SCHED_FIFO = 1,   // First in-first out scheduling
  H_SCHED_RR = 2      // Round-robin scheduling
};

enum class FairThreadPoolType { ORDERED, EFFICIENCY };

}  // namespace schedulegroup
}  // namespace hobot
#endif  // SCHEDULEGROUP_SCHEDULER_TYPE_H_
