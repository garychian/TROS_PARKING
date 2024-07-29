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

#ifndef SCHEDULEGROUP_SCHEDULE_GROUP_H_
#define SCHEDULEGROUP_SCHEDULE_GROUP_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/task.h"
#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

constexpr uint32_t g_max_thread_num = 512;

class BOLE_PUBLIC ScheduleGroup {
 public:
  explicit ScheduleGroup(const std::string &name) : name_(name) {}

  virtual ~ScheduleGroup() = default;

  ScheduleGroup(const ScheduleGroup &) = delete;
  ScheduleGroup &operator=(const ScheduleGroup &) = delete;

  /**
   * @brief Get the Name of the schedulegroup
   *
   * @return std::string the name of the schedulegroup
   */
  std::string GetName() { return name_; }

  /**
   * @brief Get the number of the thread in this schedulegroup
   *
   * @return uint32_t the number of the thread
   */
  virtual uint32_t GetThreadNum() const = 0;

  /**
   * @brief set thread's cpu affinity
   *        this interface only store the configuration
   *
   * @param thread_index the index of the thread
   * @param cpu_set cpu set on which the thread can run
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  virtual bool BindCPU(uint32_t thread_index,
                       const std::vector<unsigned int> &cpu_set) = 0;

  /**
   * @brief set all threads' cpu affinity
   *        this interface only store the configuration
   *
   * @param cpu_set cpu set on which the thread can run
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  virtual bool BindCPU(const std::vector<unsigned int> &cpu_set) = 0;

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
  virtual bool SetAttr(uint32_t thread_index, SchedulePolicy policy,
                       int32_t prio) = 0;

  /**
   * @brief Set all thread's sched policy and priority
   *
   * @param policy thread's sched policy
   * @param prio thread's sched priority
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  virtual bool SetAttr(SchedulePolicy policy, int32_t prio) = 0;

  /**
   * @brief Set thread's name
   *
   * @param thread_index the index of the thread
   * @param name thread's name
   * @return true setting thread name success.
   * @return false setting thread name fail.
   */
  virtual bool SetThreadName(uint32_t thread_index, const std::string &name);

  /**
   * @brief Get the Type of the schedulegroup
   *
   * @return SchedulerType the type of the schedulegroup
   */
  virtual SchedulerType GetType() = 0;

  /**
   * @brief start the schedulegroup
   *
   * @return true start the schedulegroup success
   * @return false start the schedulegroup failed
   */
  virtual bool Start() = 0;

  /**
   * @brief stop the schedulegroup
   *
   * @return true stop the schedulegroup success
   * @return false stop the schedulegroup failed
   */
  virtual bool Stop() = 0;

  /**
   * @brief add task to schedulegroup
   *
   * @param sp_task the task to be add
   * @return int32_t the returned error code
   */
  virtual int32_t AddTask(const std::shared_ptr<Task> &sp_task) = 0;

  /**
   * @brief add task to schedulegroup,
   * get the thread index running this task.
   *
   * @param spTask the task to be add
   * @param thread_idx storing the thread index running this task.
   * @return int32_t the returned error code
   */
  virtual int32_t AddTask(const std::shared_ptr<Task> &sp_task,
                          uint32_t *thread_idx);

  /**
   * @brief Join resident task thread.
   * Explanation: Used to block and wait for the end of the resident task
   * thread. Short-term tasks are executed in the thread pool, users do not need
   * to Call this Join interface. Defaultly, each resident task adding to the
   * global scheduler, will be joined when the program exits. Sometimes the
   * application needs to add some resident tasks, but the life cycle of these
   * resident tasks is not the whole program life cycle, so the application can
   * actively wait for the thread to exit through this Join interface.
   *
   * @param thread_idx the index of thread running resident task.
   * @return int32_t the returned error code
   */
  virtual int32_t Join(uint32_t thread_idx);

  /**
   * @brief clear tasks which be added to schedulegroup before
   *
   * @param task_producer the tasks' producer
   * @return int32_t the returned error code
   */
  virtual int32_t ClearTasks(uint64_t task_producer) = 0;

  /**
   * @brief clear all tasks in scheduler
   *
   * @return int32_t the returned error code
   */
  virtual int32_t ClearTasks() { return 0; }

 protected:
  std::string ThreadName(uint32_t index);

  std::string ThreadName(const std::string &name);

 private:
  std::string name_;
};

BOLE_PUBLIC int32_t RegistScheduler(std::shared_ptr<ScheduleGroup> scheduler);

BOLE_PUBLIC int32_t UnRegistScheduler(const std::string &name);

BOLE_PUBLIC std::shared_ptr<ScheduleGroup> GetScheduler(
    const std::string &name, int32_t *error_ret = nullptr);

BOLE_PUBLIC const std::map<std::string, std::shared_ptr<ScheduleGroup>>
    &GetSchedulers();

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_SCHEDULE_GROUP_H_
