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

#ifndef SCHEDULEGROUP_THREAD_UTILS_THREAD_DATA_H_
#define SCHEDULEGROUP_THREAD_UTILS_THREAD_DATA_H_

#include <stack>
#include <string>
#include <thread>
#include <utility>

#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

struct UserSample {
  uint64_t user_id;
  uint64_t id;
  std::chrono::system_clock::time_point start_time;
  std::string desc;
};

struct BOLE_PUBLIC PerThreadData {
  PerThreadData();
  ~PerThreadData() = default;

  /**
   * @brief push a user sample
   *
   * @param sample the user sample to push
   * @return uint64_t the returned schedulegroup's unique id of this sample
   */
  uint64_t Push(UserSample sample);

  /**
   * @brief pop the latest user sample who has given user_id
   *
   * @param user_id user sample's user_id
   * @return true the latest user sample has the given user_id
   * @return false the latest user sample does not has the given user_id
   */
  bool Pop(uint64_t user_id, UserSample &sample);

  /**
   * @brief Get the Current user sample or task id,
   *   return latest user sample's id if there has one, otherwise return task id
   *
   * @return uint64_t the index of task or the latest user sample
   */
  uint64_t GetCurrentId();

  std::string thread_name;
  std::string func_id;
  uint64_t thread_id{0};
  uint64_t task_id{0};
  std::string trigger;

 private:
  std::stack<UserSample> user_samples;
};

BOLE_PUBLIC PerThreadData &GetCurrentThreadData();

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_THREAD_UTILS_THREAD_DATA_H_
