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

#ifndef SCHEDULEGROUP_TIME_AFFI_SCHEDULER_TIME_AFFI_CONFIG_H_
#define SCHEDULEGROUP_TIME_AFFI_SCHEDULER_TIME_AFFI_CONFIG_H_

#include <cstdint>
#include <string>

namespace hobot {
namespace schedulegroup {

/**
 * @brief The cache line length of the target CPU
 *  Required value on J5 = 64
 */
constexpr uint32_t cpu_cacheline_size = 64;

/**
 * @brief The time cost from futex() time-out wakeup and preempt CPU
 *  Recommend value on J5 = 60
 *  Unit = us
 */
constexpr uint32_t futex_timeout_wakup = 60;

/**
 * @brief The time cost from sleep() to wakeup and preempt CPU
 *  Recommend value on J5 = 10
 *  Unit = us
 */
constexpr uint32_t sleep_wakeup = 10;

/**
 * The threshold of user task startup delay
 *  Delay algorithm: offset + delay_threshold < now
 *  Recommend value = Maximum of layout.delay_threshold_
 *  Recommend value on J5 = 200
 *  Unit = us
 */
constexpr uint32_t default_delay_threshold = 200;

/**
 * @brief Enable more log in ThreadWaitTime()
 *  To catch futex()/sleep() wakeup cost more then
 *    (futex_timeout_wakup + LOG_THREAD_WAIT_TIMEOUT) or
 *    (sleep_wakeup + LOG_THREAD_WAIT_TIMEOUT)
 *
 *  Default value = 0, disable log
 *  Recommend value = default_delay_threshold in debug mode
 */
#define LOG_THREAD_WAIT_TIMEOUT 200

/**
 * @brief Use futex() to wait in ThreadWaitTime()
 *  Recommend value = true, make the Stop() respond faster
 */
#define THREAD_WAIT_TIME_ON_FUTEX true

/**
 * @brief The node config in user task schedule table
 *  name - Task name
 *  allow_before - Allow execute in advance
 *  offset - The execution time of a function base on trigger (Unit = us)
 *  delay_threshold - Delay threshold of start (Unit = us)
 *  wcet - Worst-case Execution Time (Unit = us)
 *  timeout_threshold - Actual time threshold of the user function
 *    The default value of 0 should be used in most cases  (Unit = us)
 */
struct LayoutConfig {
  std::string task_name;
  bool allow_before{false};
  uint32_t offset{0};
  uint32_t delay_threshold{default_delay_threshold};
  uint32_t wcet{0};
  uint32_t timeout_threshold{0};
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_TIME_AFFI_SCHEDULER_TIME_AFFI_CONFIG_H_
