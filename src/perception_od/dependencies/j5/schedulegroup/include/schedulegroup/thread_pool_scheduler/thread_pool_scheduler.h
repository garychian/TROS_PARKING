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

#ifndef SCHEDULEGROUP_THREAD_POOL_SCHEDULER_THREAD_POOL_SCHEDULER_H_
#define SCHEDULEGROUP_THREAD_POOL_SCHEDULER_THREAD_POOL_SCHEDULER_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "schedulegroup/schedule_group.h"
#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/task.h"
#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

class TaskQueue;
class ThreadWorker;

class BOLE_PUBLIC ThreadPoolScheduler : public ScheduleGroup {
  enum class Status { Uninit, Starting, Running, Stopped, Shutdown };

 public:
  /**
   * @brief Create a new fair thread pool
   *
   * @param name the thread pool scheduler's name
   * @param type the fair thread pool scheduler's type - ordered or efficiency
   * @param thread_num the number of the thread pool's thread
   * @return std::shared_ptr<ThreadPoolScheduler> the shared point of
   *  the new create thread pool
   */
  static std::shared_ptr<ThreadPoolScheduler> NewFairThreadPool(
      const std::string &name, FairThreadPoolType type,
      uint32_t thread_num = 0);

  /**
   * @brief Create a new priority thread pool
   *
   * @param name the thread pool scheduler's name
   * @param prio_count the count of task's priority,
   *  Task priority can be set from 0 to prio_count-1
   * @param thread_num the number of thread pool's thread
   * @param wait_time_for_upgrade the duration after which task's priority
   *  will be upgrade
   * @return std::shared_ptr<ThreadPoolScheduler> the shared point of
   *  the new create thread pool
   */
  static std::shared_ptr<ThreadPoolScheduler> NewPriorityThreadPool(
      const std::string &name, uint8_t prio_count, uint32_t thread_num = 0,
      std::chrono::milliseconds wait_time_for_upgrade =
          std::chrono::milliseconds::zero());

  ~ThreadPoolScheduler() noexcept;

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
   * @brief Set thread's name
   *
   * @param thread_index the index of the thread
   * @param name thread's name
   * @return true setting thread name success.
   * @return false setting thread name fail.
   */
  bool SetThreadName(uint32_t thread_index, const std::string &name) override;

  /**
   * @brief Get the Type of the schedulegroup
   *
   * @return SchedulerType the type of the schedulegroup
   */
  SchedulerType GetType() override {
    return SchedulerType::SCHEDULER_TYPE_THREADPOOL;
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
   * @brief add task to schedulegroup,
   * get the thread index running this task.
   * Notice: the task should be persistent, other this interface
   * will return fail.
   *
   * @param spTask the task to be add
   * @param thread_idx storing the thread index running this task.
   * @return int32_t the returned error code
   */
  int32_t AddTask(const std::shared_ptr<Task> &sp_task,
                  uint32_t *thread_idx) override;

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
   * Notice: thread_idx must be the return value from AddTask interface,
   * this Join interface may be block forever if pass the thread_idx other
   * values.
   *
   * @return int32_t the returned error code
   */
  int32_t Join(uint32_t thread_idx) override;

  /**
   * @brief clear tasks which be added to schedulegroup before
   *
   * @param task_producer the tasks' producer
   * @return int32_t the returned error code
   */
  int32_t ClearTasks(uint64_t task_producer) override;

  /**
   * @brief clear all task in scheduler
   *
   * @return int32_t the returned error code
   */
  int32_t ClearTasks() override;

 private:
  ThreadPoolScheduler(const std::string &name, FairThreadPoolType type,
                      uint32_t thread_num = 0);
  ThreadPoolScheduler(const std::string &name, uint8_t prio_count,
                      uint32_t thread_num = 0,
                      std::chrono::milliseconds wait_time_for_upgrade =
                          std::chrono::milliseconds::zero());

  void InitWorkers();

  bool IsValid();

 private:
  uint32_t thread_num_;
  std::vector<std::shared_ptr<ThreadWorker>> workers_;
  std::mutex workers_mutex_;
  std::shared_ptr<TaskQueue> task_queue_{nullptr};
  std::atomic<Status> status_{Status::Uninit};
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_THREAD_POOL_SCHEDULER_THREAD_POOL_SCHEDULER_H_
