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

#ifndef SCHEDULEGROUP_TASK_H_
#define SCHEDULEGROUP_TASK_H_

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "schedulegroup/utils/logging.h"
#include "schedulegroup/utils/thread_data.h"
#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

constexpr uint64_t kTaskIDNone = 0;

constexpr uint32_t kDefaultCrIndex{UINT32_MAX};

using TaskFunc = std::function<void()>;

// record the performance statistics of tasks
struct BOLE_PUBLIC TaskStats {
  uint64_t id;
  std::string func_id;
  std::chrono::system_clock::time_point add_time;
  std::chrono::system_clock::time_point sched_time;
  std::chrono::system_clock::time_point start_time;
  std::chrono::system_clock::time_point end_time;
};

using Functor = std::function<void(uint64_t producer, const TaskStats &stats)>;

struct TimerOption {
  enum class TimerClockType {
    SteadyTime = 0,  // Use a steady clock time
    SimTime,         // Use simulation time, behave as ROS_TIME
    SystemTime       // Use system time
  };
  enum class TriggerType { DefaultTrigger = 0, SelfTrigger = 1 };
  TimerOption() {}
  TimerOption(std::chrono::microseconds interval, int32_t counts = -1,
              TimerClockType type = TimerClockType::SteadyTime)
      : us_interval(interval), repeat_counts(counts), clock_type(type) {}
  // Scheduling period of timer(ms)
  std::chrono::microseconds us_interval{100000};
  // Repeat times of timer
  int32_t repeat_counts{-1};
  // clock type of timer
  TimerClockType clock_type{TimerClockType::SteadyTime};
  // trigger type of timer
  TriggerType trigger_type{TriggerType::DefaultTrigger};
};

using TaskPriority = uint8_t;

class ScheduleGroup;
class CoroutineScheduler;
class TimeAffiScheduler;
class ThreadPoolScheduler;
class TimerScheduler;
class SimpleScheduler;
class PriorityTaskQueue;
class FairTaskQueue;
class SimpleTaskQueue;

class BOLE_PUBLIC Task {
  friend class ScheduleGroup;
  friend class CoroutineScheduler;
  friend class TimeAffiScheduler;
  friend class ThreadPoolScheduler;
  friend class TimerScheduler;
  friend class SimpleScheduler;
  friend class PriorityTaskQueue;
  friend class FairTaskQueue;
  friend class SimpleTaskQueue;

 public:
  /**
   * @brief Construct a new Task object
   *
   * @param producer the task's producer
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param enable_stats whether or not to enable performance statistic
   */
  Task(uint64_t producer, const TaskFunc &func, const std::string &func_id,
       bool enable_stats = false);

  /**
   * @brief Construct a new Task object
   *
   * @param producer the task's producer
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param group_name the group of the task
   * @param priority the priority of the task
   * @param enable_stats whether or not to enable performance statistic
   */
  Task(uint64_t producer, const TaskFunc &func, const std::string &func_id,
       const std::string &group_name, TaskPriority priority = 0,
       bool enable_stats = false);

  /**
   * @brief Construct a new Delay Task object
   *
   * @param producer the task's producer
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param delay_duration the delay time
   * @param group_name the group of the task
   * @param priority the priority of the task
   * @param enable_stats whether or not to enable performance statistic
   */
  Task(uint64_t producer, const TaskFunc &func, const std::string &func_id,
       const std::chrono::microseconds &delay_duration,
       const std::string &group_name, TaskPriority priority = 0,
       bool enable_stats = false);

  /**
   * @brief Construct a new Task object
   *
   * @param producer the task's producer
   * @param func  the function to be executed
   * @param func_id  the id of function which to be executed
   * @param thread_index the thread's index on which this task execute
   * @param persistent whether this task is persistent or not
   * @param enable_stats whether or not to enable performance statistic
   */
  Task(uint64_t producer, const TaskFunc &func, const std::string &func_id,
       uint32_t thread_index, bool persistent = false,
       bool enable_stats = false);

  /**
   * @brief Construct a new Task object
   *
   * @param producer the task's producer
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param thread_index the thread's index on which this task execute
   * @param delay_duration the delay time
   * @param persistent whether this task is persistent or not
   * @param enable_stats whether or not to enable performance statistic
   */
  Task(uint64_t producer, const TaskFunc &func, const std::string &func_id,
       uint32_t thread_index, const std::chrono::microseconds &delay_duration,
       bool persistent = false, bool enable_stats = false);

  /**
   * @brief Construct a Timer Task object
   *
   * @param producer the task's producer
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param time_option the periodic option of timer
   * @param enable_stats whether or not to enable performance statistic
   */
  Task(uint64_t producer, const TaskFunc &func, const std::string &func_id,
       TimerOption &time_option, bool enable_stats = false);

  /**
   * @brief Get the task's Group Name
   *
   * @return const std::string& the task's group name
   */
  const std::string &GetGroupName() const { return group_name_; }

  virtual ~Task() {}

  /**
   * @brief Reset Task object, clear some runtime parameter
   *
   */
  void Reset() {
    execute_flag_ = true;
    stats_enabled_ = false;
    persistent_ = false;
    is_delay_ = false;
    delay_duration_ = std::chrono::microseconds(0);
    priority_ = 0;
    group_name_.clear();
    pre_func_ = nullptr;
    post_func_ = nullptr;
  }

  /**
   * @brief Set a ThreadPool Task object attribute
   *
   * @param producer the task's producer
   * @param func the function to be executed
   * @param func_id the id of function which to be executed
   * @param thread_index the thread's index on which this task execute
   * @param delay_duration the delay time
   * @param group_name the group of the task
   * @param priority the priority of the task
   * @param enable_stats whether or not to enable performance statistic
   */
  void SetTaskAttr(uint64_t producer, const TaskFunc &func,
                   const std::string &func_id, uint32_t thread_index,
                   const std::chrono::microseconds &delay_duration,
                   const std::string &group_name, TaskPriority priority = 0,
                   bool enable_stats = false) {
    producer_ = producer;
    func_ = func;
    func_id_ = func_id;
    thread_index_ = thread_index;
    delay_duration_ = delay_duration;
    group_name_ = group_name;
    priority_ = priority;
    stats_enabled_ = enable_stats;
  }

  /**
   * @brief Get the function id of the Task
   *
   * @return const std::string& the id of function this task execute
   */
  const std::string &GetFuncId() { return func_id_; }

  /**
   * @brief Get the Producer of the Task
   *
   * @return const std::string& the producer of the Task
   */
  uint64_t GetProducer() { return producer_; }

  /**
   * @brief Get the Priority of the task
   *
   * @return TaskPriority the priority of the task
   */
  TaskPriority GetPriority() const { return priority_; }

  /**
   * @brief execute the function of the task
   *
   */
  void DoTask() {
    if (stats_enabled_) {
      auto &per_thread_data = GetCurrentThreadData();
      per_thread_data.task_id = stats_.id;
      per_thread_data.func_id = stats_.func_id;
      stats_.start_time = std::chrono::system_clock::now();
      if (pre_func_) {
        pre_func_(producer_, stats_);
      }
    }

    func_();

    if (stats_enabled_) {
      stats_.end_time = std::chrono::system_clock::now();
      GetCurrentThreadData().task_id = kTaskIDNone;
      if (post_func_) {
        post_func_(producer_, stats_);
      }
    }
  }

  /**
   * @brief enable performance statistic
   *
   */
  void EnableStats() { stats_enabled_ = true; }

  /**
   * @brief Get task's statistic enable or not
   *
   * @return true the performance statistic of the task is enabled
   * @return false the performance statistic of the task is disabled
   */
  bool IsStatsEnabled() { return stats_enabled_; }

  /**
   * @brief Set the Post Function object
   *        this func will be called after task when statistic enabled
   *
   * @param post_func the post function to be set
   */
  void SetPostFunc(Functor post_func) { post_func_ = post_func; }

  /**
   * @brief Set the Pre Func object
   *        this func will be called before task when statistic enabled
   *
   * @param pre_func the pre function to be set
   */
  void SetPreFunc(Functor pre_func) { pre_func_ = pre_func; }

  /**
   * @brief Get the Execute Flag of the task
   *
   * @return true task will be executed
   * @return false task will not be executed
   */
  void SetExecuteFlag(bool execute) { execute_flag_ = execute; }

  /**
   * @brief Set the Execute Flag of the task
   *
   * @param flag the task's execute flag
   */
  bool GetExecuteFlag() const { return execute_flag_; }

  /**
   * @brief Set the execute thread
   *
   * @param thread_index the index of the execute thread
   */
  void SetThreadIndex(uint32_t thread_index) { thread_index_ = thread_index; }

  /**
   * @brief Get the index of thread on which this task execute
   *
   * @return the thread's index
   */
  uint32_t GetThreadIndex() const { return thread_index_; }

  /**
   * @brief Get the Trigger Time of delay task
   *
   * @return const std::chrono::steady_clock::time_point& the time
   *  in which the delay task be actived
   */
  const std::chrono::steady_clock::time_point &GetTriggerTime() const {
    return trigger_time_;
  }

  /**
   * @brief Set the task is persistent or not
   *
   * @param persistent whether the task is persistent or not
   */
  void SetPersistent(bool persistent) { persistent_ = persistent; }

  /**
   * @brief The task is persistent or not
   *
   * @return whether the task is persistent or not
   */
  bool GetPersistent() const { return persistent_; }

  /**
   * @brief Set the thread running this task is detach or not
   *
   * @param detach_flag whether the thread running this task is detach or not
   */
  void SetDetachFlag(bool detach_flag) { detach_flag_ = detach_flag; }

  /**
   * @brief The thread running this task is detach or not.
   *
   * @return whether the thread running this task is detach or not
   */
  bool GetDetachFlag() const { return detach_flag_; }

  /**
   * @brief The task is delay or not
   *
   * @return whether the task is delay or not
   */
  bool IsDelayTask() const { return is_delay_; }

  /**
   * @brief The task timer option
   *
   * @return whether the task timer option
   */
  TimerOption GetTimerOption() { return timer_option_; }

  /**
   * @brief The task delay duration
   *
   * @return the delay duration of task
   */
  std::chrono::microseconds GetDelayDuration() { return delay_duration_; }

 protected:
  // the host of the task. The task may depend on
  // the host object during the running process
  uint64_t producer_;
  // The task function. schedule group call it.
  TaskFunc func_;
  // function id of the task
  std::string func_id_;
  // The hook function before and after task execution
  Functor pre_func_;
  Functor post_func_;

  // task group information. all tasks in one group are
  // cached to one task queue.
  // dataflow set default group_name_ = module/node name + task name.
  // Notice: only ThreadPoolScheduler support this field.
  // tasks are guaranteed to be executed sequentially
  // under ORDERED policy of ThreadPoolScheduler
  std::string group_name_{""};

  // task priority, valid under schedle group
  // supporting task priority
  TaskPriority priority_{0};
  // the time point when adjustment the priority of task
  // Notice: the value is set by schedule group automatically
  std::chrono::steady_clock::time_point priority_adjustment_time_;

  // Periodic Task Properties
  TimerOption timer_option_;

  // the worst execution time of the task, in microseconds
  uint32_t wcet_{0};
  // the deadline of the task.
  // the task must be completed before the deadline.
  // Notice: only DeadlineScheduler support deadline_
  std::chrono::steady_clock::time_point deadline_;

  uint32_t thread_index_{0};

  // the delay time
  std::chrono::microseconds delay_duration_{0};
  // the expected execution time point of the delay task.
  // Notice: the value can be set by constructor automatically
  std::chrono::steady_clock::time_point trigger_time_;
  bool is_delay_{false};

  bool persistent_{false};

  // the flag that the task needs to be executed.
  // the task will not be executed when execute_flag_ equals false.
  bool execute_flag_{true};

  // statistics function switch
  bool stats_enabled_ = false;
  // task time point information for statistics
  // contain add_time, start_time, end_time
  TaskStats stats_;

  // the thread running this task need to be detached?
  bool detach_flag_{false};
};

using spTask = std::shared_ptr<Task>;

template <typename T>
struct TaskTriggerLess {
  bool operator()(const T &lhs, const T &rhs) const {
    return lhs->GetTriggerTime() < rhs->GetTriggerTime();
  }
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_TASK_H_
