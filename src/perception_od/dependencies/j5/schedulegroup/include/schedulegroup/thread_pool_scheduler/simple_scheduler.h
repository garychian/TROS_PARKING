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

#ifndef SCHEDULEGROUP_THREAD_POOL_SCHEDULER_SIMPLE_SCHEDULER_H_
#define SCHEDULEGROUP_THREAD_POOL_SCHEDULER_SIMPLE_SCHEDULER_H_

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "schedulegroup/schedule_group.h"
#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

class TaskQueue;
class ThreadWorker;

class BOLE_PUBLIC SimpleScheduler : public ScheduleGroup {
  enum class Status { Uninit, Starting, Running, Stopped, Shutdown };

 public:
  /**
   * @brief Create a new simple scheduler
   *
   * @param name the name of simple scheduler
   * @param max_thread_count the maximum count of threads this scheduler could
   *   create
   */
  static std::shared_ptr<SimpleScheduler> NewSimpleScheduler(
      const std::string &name, uint32_t max_thread_count);

  SimpleScheduler(const std::string &name, uint32_t max_thread_count);

  ~SimpleScheduler();

  /**
   * @brief Get the number of the thread in this schedulegroup
   *
   * @return uint32_t the number of the thread
   */
  uint32_t GetThreadNum() const override;

  /**
   * @brief set thread's cpu affinity
   *        this interface only store the configuration
   *
   * @param thread_index the index of the thread
   * @param cpu_set cpu set on which the thread can run
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool BindCPU(uint32_t thread_index,
               const std::vector<unsigned int> &cpu_set) override;

  /**
   * @brief set all threads' cpu affinity
   *        this interface only store the configuration
   *
   * @param cpu_set cpu set on which the thread can run
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool BindCPU(const std::vector<unsigned int> &cpu_set) override;

  /**
   * @brief Set thread's sched policy and priority
   *        this interface only store the configuration
   *
   * @param thread_index the index of the thread
   * @param policy thread's sched policy
   * @param prio thread's sched priority
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool SetAttr(uint32_t thread_index, SchedulePolicy policy,
               int32_t prio) override;

  /**
   * @brief Set all thread's sched policy and priority
   *
   * @param policy thread's sched policy
   * @param prio thread's sched priority
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool SetAttr(SchedulePolicy policy, int32_t prio) override;

  /**
   * @brief Get the Type of the schedulegroup
   *
   * @return SchedulerType the type of the schedulegroup
   */
  SchedulerType GetType() override {
    return SchedulerType::SCHEDULER_TYPE_SIMPLE_THREADPOOL;
  }

  /**
   * @brief start the schedulegroup
   *
   * @return true start the schedulegroup success
   * @return false start the schedulegroup failed
   */
  bool Start() override;

  /**
   * @brief stop the schedulegroup
   *
   * @return true stop the schedulegroup success
   * @return false stop the schedulegroup failed
   */
  bool Stop() override;

  /**
   * @brief add task to schedulegroup
   *
   * @param spTask the task to be add
   * @return int32_t the returned error code
   */
  int32_t AddTask(const std::shared_ptr<Task> &sp_task) override;

  /**
   * @brief clear tasks which be added to schedulegroup before
   *
   * @param task_producer the tasks' producer
   * @return int32_t the returned error code
   */
  int32_t ClearTasks(uint64_t task_producer) override;

  /**
   * @brief clear all tasks in scheduler
   *
   * @return int32_t the returned error code
   */
  int32_t ClearTasks() override;

 private:
  struct Group {
    std::atomic<bool> init{false};
    std::shared_ptr<TaskQueue> task_queue{nullptr};
    std::shared_ptr<ThreadWorker> worker{nullptr};
  };

  uint32_t max_thread_count_{0};
  uint32_t current_thread_count_{0};
  mutable std::mutex mutex_;
  std::vector<Group> groups_;
  std::mutex setting_mutex_;
  std::map<uint32_t, std::vector<unsigned int>> cpu_affinity_;
  std::map<uint32_t, std::pair<SchedulePolicy, int32_t>> thread_attribute_;
  std::atomic<Status> status_{Status::Uninit};
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_THREAD_POOL_SCHEDULER_SIMPLE_SCHEDULER_H_
