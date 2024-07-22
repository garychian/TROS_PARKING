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

#ifndef SCHEDULEGROUP_DEADLINE_SCHEDULER_DEADLINE_SCHEDULER_H_
#define SCHEDULEGROUP_DEADLINE_SCHEDULER_DEADLINE_SCHEDULER_H_

#include <memory>
#include <string>
#include <vector>

#include "schedulegroup/schedule_group.h"
#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/task.h"

namespace hobot {
namespace schedulegroup {
class DeadlineScheduler : public ScheduleGroup {
 public:
  DeadlineScheduler(const std::string &name, uint32_t threads)
      : ScheduleGroup(name) {
    (void)threads;
  }
  ~DeadlineScheduler() = default;

  SchedulerType GetType() override;
  bool Start() override;
  bool Stop() override;
  int32_t AddTask(const std::shared_ptr<Task> &spTask) override;
  int32_t ClearTasks(const std::string &task_producer) override;
  int32_t ClearTasks() override;

  bool Init(const std::string &json_path);
  void SetWcet(const std::string &name, uint64_t wcet);
  bool DebugOn(const std::string &wcet_path);

  bool SetAttr(SchedulePolicy policy, int32_t prio) override;
  bool SetAttr(uint32_t thread_index, SchedulePolicy policy,
               int32_t prio) override;
  bool BindCPU(const std::vector<unsigned int> &cpu_set) override;
  bool BindCPU(uint32_t thread_index,
               const std::vector<unsigned int> &cpu_set) override;
  uint32_t GetThreadNum() const override;
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_DEADLINE_SCHEDULER_DEADLINE_SCHEDULER_H_
