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

#pragma once
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "schedulegroup/schedule_group.h"
#include "schedulegroup/task.h"

namespace hobot {
namespace schedulegroup {

constexpr uint8_t TASK_PRIO_COUNT = 5;

constexpr int32_t TASK_PRIORITY_LOW = 0;
constexpr int32_t TASK_PRIORITY_BELOW_NORMAL = 1;
constexpr int32_t TASK_PRIORITY_NORMAL = 2;  // default task priority
constexpr int32_t TASK_PRIORITY_ABOVE_NORMAL = 3;
constexpr int32_t TASK_PRIORITY_HIGH = 4;

struct BOLE_PUBLIC GlobalSchedulerParam {
  // the name of global scheduler
  std::string name{"g_sg"};  // avoid thread name length
  // the size of threads processing short-term and delay task.
  uint32_t thread_pool_num{std::thread::hardware_concurrency()};
  // the thread kernel scheduler policy.
  SchedulePolicy thread_pool_policy{SchedulePolicy::H_SCHED_OTHER};
  // the thread kernel scheduler priority.
  int32_t thread_pool_prio{0};
  // the cpuset of thread pools.
  std::vector<unsigned int> thread_pool_cpu_set;
  // Application configured resident task thread scheduling policy.
  std::unordered_map<std::string, SchedulePolicy> persistent_task_policy;
  // Application configured resident task thread scheduling priority.
  std::unordered_map<std::string, int32_t> persistent_task_priority;
  // Application configured resident task thread scheduling cpuset.
  std::unordered_map<std::string, std::vector<unsigned int> >
      persistent_task_cpuset;
  // Application configured resident task thread name.
  std::unordered_map<std::string, std::string> persistent_task_threadname;
  // The task priority of temporary tasks and delay tasks
  // configured by the application.
  std::unordered_map<std::string, TaskPriority> task_priority;
};

class BOLE_PUBLIC GlobalScheduler {
 public:
  /**
   * @brief Set the environment variable WorkOnScheduleGroup to 1 to
   *        enable global scheduling mode
   *
   * @return int32_t the Errorcode
   */
  static int32_t TurnOnFlag();

  /**
   * @brief Set the environment variable WorkOnScheduleGroup to 0 to
   *        enable global scheduling mode
   *
   * @return int32_t the Errorcode
   */
  static int32_t TurnOffFlag();

  /**
   * @brief Check whether the environment variable WorkOnScheduleGroup is 1,
   *        and judge whether the global scheduling mode is enabled
   *
   * @return int32_t the Errorcode
   */
  static bool IsFlagOn();

  /**
   * @brief Init and construct the global scheduler
   * Notice: Init is not thread-safe.
   * This interface is usually called by deployment tool,
   * eg Mainboard.
   *
   * @param param the parameter of global scheduler.
   * Notice: The Param is usually generate by deployment tool
   * from configure file.
   *
   * @return int32_t the Errorcode
   */
  static int32_t Init(const GlobalSchedulerParam &param);

  /**
   * @brief Get the instance of global scheduler
   *
   * @return std::shared_ptr<ScheduleGroup> the instance
   *  of global scheduler
   */
  static std::shared_ptr<ScheduleGroup> GetScheduler();

  /**
   * @brief DeInit and remove the instance of global scheduler.
   *
   * @return int32_t the error code
   */
  static int32_t DeInit();

  /**
   * @brief Get the Param of global scheduler
   *
   * @return Param of global scheduler
   */
  static GlobalSchedulerParam GetGlobalSchedulerParam();

  /**
   * @brief Construct a new short-term Task object.
   * Notice: the priority of this task can be set
   * in GlobalSchedulerParam
   *
   * @param producer the task's producer
   * @param func  the function to be executed
   * @param func_id  the id of function which to be executed
   *
   * @return shared_ptr<Task> the task object.
   * You can call GlobalScheduler::AddTask() later.
   */
  static std::shared_ptr<Task> GenerateShortTask(uint64_t producer,
                                                 const TaskFunc &func,
                                                 const std::string &func_id);

  /**
   * @brief Construct a new short-term Task object.
   *
   * @param producer the task's producer
   * @param func  the function to be executed
   * @param func_id  the id of function which to be executed
   * @param priority the priority of the task
   *
   * @return shared_ptr<Task> the task object.
   * You can call GlobalScheduler::AddTask() later.
   */
  static std::shared_ptr<Task> GenerateShortTask(uint64_t producer,
                                                 const TaskFunc &func,
                                                 const std::string &func_id,
                                                 TaskPriority priority);

  /**
   * @brief Construct a new delay Task object.
   * Notice: Delay Task will only be executed once.
   * Delay task can not be persistent.
   * The priority of this task can be set
   * in GlobalSchedulerParam.
   *
   * @param producer the task's producer
   * @param func  the function to be executed
   * @param func_id  the id of function which to be executed
   * @param delay_duration the delay time
   *
   * @return shared_ptr<Task> the task object.
   * You can call GlobalScheduler::AddTask() later.
   */
  static std::shared_ptr<Task> GenerateDelayTask(
      uint64_t producer, const TaskFunc &func, const std::string &func_id,
      const std::chrono::microseconds &delay_duration);

  /**
   * @brief Construct a new delay Task object.
   * Notice: Delay Task will only be executed once.
   * Delay task can not be persistent.
   *
   * @param producer the task's producer
   * @param func  the function to be executed
   * @param func_id  the id of function which to be executed
   * @param delay_duration the delay time
   * @param priority the priority of the task
   *
   * @return shared_ptr<Task> the task object.
   * You can call GlobalScheduler::AddTask() later.
   */
  static std::shared_ptr<Task> GenerateDelayTask(
      uint64_t producer, const TaskFunc &func, const std::string &func_id,
      const std::chrono::microseconds &delay_duration, TaskPriority priority);

  /**
   * @brief Construct a new persistent Task object
   *
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param detach_flag whether the thread running this task
   * is detach or not
   *
   * @return shared_ptr<Task> the task object.
   * You can call GlobalScheduler::AddTask() later.
   */
  static std::shared_ptr<Task> GeneratePersistentTask(
      const TaskFunc &func, const std::string &func_id,
      bool detach_flag = false);
  /**
   * @brief add task to global scheduler
   * Explanation: We purposely divide task creation and task addition
   * into two interfaces, because after task creation, users can set
   * some additional attributes on the task, such as whether to enable
   * the statistics function, and the registration of the hook interface
   * of the statistics function.
   *
   * @param sp_task the task to be add
   * @param thread_idx the index of thread running this task.
   * This parameter is only valid for PersistentTask.
   * @return int32_t the returned error code
   */
  static int32_t AddTask(const std::shared_ptr<Task> &sp_task,
                         uint32_t *thread_idx = nullptr);

  /**
   * @brief clear tasks which be added to global scheduler before
   *
   * @param task_producer the tasks' producer
   * @return int32_t the returned error code
   */
  static int32_t ClearTasks(uint64_t task_producer);

  /**
   * @brief clear all tasks which to be added to global scheduler before
   *
   * @return int32_t the returned error code
   */
  static int32_t ClearTasks();

 private:
  static GlobalSchedulerParam init_param_;
  static std::shared_ptr<ScheduleGroup> scheduler_;
};

}  // namespace schedulegroup
}  // namespace hobot
