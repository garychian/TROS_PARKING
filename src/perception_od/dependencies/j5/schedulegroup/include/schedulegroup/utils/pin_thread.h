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
#ifndef SCHEDULEGROUP_UTILS_PIN_THREAD_H_
#define SCHEDULEGROUP_UTILS_PIN_THREAD_H_

#include <string>
#include <thread>
#include <vector>

#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

/**
 * @brief Set thread schedule policy
 * @details For mac, SetThreadAttr not support;
 * For linux, Not support H_SCHED_OTHER set prio, use default 0;
 * For win, policy H_SCHED_FIFO not support;
 * process policy is NORMAL_PRIORITY_CLASS;
 * thread policy is THREAD_PRIORITY_HIGHEST for H_SCHED_RR;
 * thread policy is THREAD_PRIORITY_NORMAL for H_SCHED_OTHER.
 * @param thread thread memory address
 * @param policy the policy of thread scedule policy
 * @param prio the priority of thread priority
 * @return bool value: return true if success, return false otherwise
 */
bool BOLE_PUBLIC SetThreadAttr(std::thread *thread, SchedulePolicy policy,
                               int32_t prio = 0);

/**
 * @brief Set thread cpu affinity
 * @details not support mac plantform,
 * only support linux and win.
 * @param thread thread address
 * @param cpus the cpus of a collection of thread bound cpu
 * @return bool value: return true if success, return false otherwise
 */
bool BOLE_PUBLIC SetThreadBindCPU(std::thread *thread,
                                  const std::vector<unsigned int> &cpus);

/**
 * @brief Set thread name
 * @details not support mac platform,
 * only support linux and win.
 * @param thread thread memory address
 * @param thread_name the thread name of set thread name
 * @return bool value: return true if success, return false otherwise
 */
bool BOLE_PUBLIC SetThreadName(std::thread *thread,
                               const std::string &thread_name);

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_UTILS_PIN_THREAD_H_
