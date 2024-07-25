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

#ifndef SCHEDULEGROUP_TIME_AFFI_SCHEDULER_TIME_AFFI_SCHEDULER_H_
#define SCHEDULEGROUP_TIME_AFFI_SCHEDULER_TIME_AFFI_SCHEDULER_H_

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "nlohmann_json/json.hpp"
#include "schedulegroup/schedule_group.h"
#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/task.h"
#include "schedulegroup/time_affi_scheduler/time_affi_config.h"

namespace hobot {
namespace schedulegroup {

class TimeSlot;
class Layout;
class TaskMgr;

/**
 * Futex FUTEX_WAIT use CLOCK_MONOTONIC as default clock
 *  Unit: Nanosecond
 */
using TimePoint = std::chrono::steady_clock::time_point;
#define TP_SEC(TP) (TP.time_since_epoch().count() / 1000000000)
#define TP_MILL(TP) (TP.time_since_epoch().count() / 1000000)
#define TP_MACRO(TP) (TP.time_since_epoch().count() / 1000)
#define TP_NANO(TP) TP.time_since_epoch().count()

/**
 * @brief Event reporting hook function
 * @param spLayout - Layout in schedule table
 */
using TimeAffiHookFunc =
    std::function<void(const struct LayoutConfig &LayoutConf)>;

/**
 * @brief The TimeAffiSchedulerWorkMode enum
 *  DEBUG - Debug mode
 *  TRIGGER - Trigger mode
 *  CYCLIC - Cycle mode
 */
enum class TimeAffiSchedulerWorkMode : uint32_t {
  DEBUG = 0,
  TRIGGER,
  CYCLIC,
};

/**
 * @brief The TimeAffiScheduler class
 */
#pragma pack(8)
class BOLE_PUBLIC TimeAffiScheduler : public ScheduleGroup {
 public:
  /**
   * @brief TimeAffiScheduler initialize constructor
   * @param name - The name of scheduler
   * @param timeslot_num - How many threads in scheduler
   */
  explicit TimeAffiScheduler(const std::string &name,
                             uint32_t timeslot_num = 0);
  /**
   * @brief TimeAffiScheduler initialize constructor
   * @param name - The name of scheduler
   * @param file - Schedule table json file
   */
  explicit TimeAffiScheduler(const std::string &name, const std::string &file);
  TimeAffiScheduler() = delete;
  TimeAffiScheduler(const TimeAffiScheduler &) = delete;
  TimeAffiScheduler &operator=(const TimeAffiScheduler &) = delete;
  ~TimeAffiScheduler();

  //////////////////////////////
  /// configuration function ///
  //////////////////////////////

  /**
   * @brief Set the number of timeslot in scheduler
   * @param timeslot_num - How many timeslot in scheduler
   * @return false - timeslot_num invalid
   *                 Repeat setting
   */
  bool SetTimeSlot(uint32_t timeslot_num);

  /**
   * @brief Bind cpus to one timeslot
   * @param timeslot_index - (0 <= timeslot_index < timeslot_num_)
   * @param cpu_set - CPU cores to bind on
   * @return false - timeslot_index invalid
   *                 cpu_set invalid
   *                 Scheduler busy
   */
  bool BindCPU(uint32_t timeslot_index,
               const std::vector<unsigned int> &cpu_set) override;

  /**
   * @brief Bind cpus to all timeslots
   * @param cpu_set - CPU cores to bind on
   * @return false - cpu_set invalid
   *                 Scheduler busy
   */
  bool BindCPU(const std::vector<unsigned int> &cpu_set) override;

  /**
   * @brief Set schedule policy and priority to one timeslot
   * @param timeslot_index - (0 <= timeslotindex < timeslot_num_)
   * @param policy - Schedule policy
   * @param prio - Schedule priority for RT-thread
   * @return false - timeslot_index invalid
   *                 Scheduler busy
   *                 prio invalid
   */
  bool SetAttr(uint32_t timeslot_index, SchedulePolicy policy,
               int32_t prio) override;

  /**
   * @brief Set schedule policy and priority to all timeslots
   * @param policy - Schedule policy
   * @param prio - Schedule priority for RT-thread
   * @return False - Scheduler busy
   *                 prio invalid
   */
  bool SetAttr(SchedulePolicy policy, int32_t prio) override;

  /**
   * @brief Add a layout to timeslot/thread
   * @param timeslot_index - (0 <= timeslot_index < timeslot_num_)
   * @param layconf - Layout configuration
   * @return false - timeslot_id invalid
   *                 name or wcet invalid
   *                 Scheduler busy
   *                 overlap
   */
  bool AddLayout(uint32_t timeslot_index, const struct LayoutConfig &layconf);

  /**
   * @brief Register a cycling task
   * @param spTask - User task
   * @return false - Layout [] not added
   *                 Scheduler busy
   *                 Layout [{}] registered
   */
  bool RegisterTask(const std::shared_ptr<Task> &spTask);

  /**
   * @brief Add a once-only user task to scheduler
   * @param spTask - User task
   * @return false - Scheduler stopped
   *                 Layout [] not added
   * @note Hotspot function, MT-Safe(After layout init)
   */
  int32_t AddTask(const std::shared_ptr<Task> &spTask) override;

  /**
   * @brief Set frame period
   * @param cycle - User expected cycle
   * @return false - Scheduler busy
   */
  bool SetFrameCycle(uint32_t cycle);

  /////////////////////////
  /// control functions ///
  /////////////////////////

  /**
   * @brief Init a scheduler by a json file
   * @param file - Full path of the scheduler's configuration file
   * @param print - Print file parsing result
   * @return false - Scheduler busy
   *                 Open or parse file failed(detailed log)
   */
  bool Init(const std::string &file);

  /**
   * @brief Check all timeslots and create work thread
   * @return always return false
   */
  bool Start() override { return false; }

  /**
   * @brief Check all timeslot and create work thread
   * @param workmode
   *   CYCLIC - Create work threads and start loop
   *   TRIGGER - Create work threads and wait trigger()
   *   DEBUG - Create work threads and wait debug()
   * @return false - no timeslot
   *                 TimeSlot [] null
   *                 cycle invalid
   *                 ThreadSet [] SetThreadBindCPU error
   *                 ThreadSet [] SetThreadAttr error
   *                 futex unexpected error
   */
  bool Start(TimeAffiSchedulerWorkMode workmode);

  /**
   * @brief Trigger once when workmode is TRIGGER
   * @param trigger_time - Trigger time point
   * @param frames - Trigger frames
   * @return false - frames invalid
   *                 scheduler stopped
   *                 work_mode_ unsupport
   *                 CAS failed(no log)
   *
   * @note The caller will be blocked when scheduler is busy
   */
  bool Trigger(uint32_t frames = 1);
  bool Trigger(TimePoint &trigger_time, uint32_t frames = 1);

  /**
   * @brief Wait work threads create and block
   *        Wait scheduler is idle
   * @return false - futex unexpected error
   *                 scheduler stopped(no log)
   */
  bool TriggerWait();

  /**
   * @brief Trigger scheduler work on debug mode
   * @param frames - Trigger frames
   * @param wcet_path - Full path of statistics output file
   * @return false - [] [] invalid(count or wcet_path)
   *                 bad env(Scheduler Stopped or Not DEBUG mode)
   */
  bool Debug(uint32_t frames, const std::string &wcet_path);

  /**
   * @brief Wait for the end of the DebugWaitLoop
   * @return false - Scheduler Stopped(no log)
   *                 Not DEBUG mode(no log)
   */
  bool DebugWait();

  /**
   * @brief Stop all work threads
   * @return false - already stopped
   */
  bool Stop() override;

  ///////////////////////
  /// other functions ///
  ///////////////////////

  /**
   * @brief GetThreadNum
   * @return How many timeslots in scheduler
   */
  inline uint32_t GetThreadNum() const override { return timeslot_num_; }

  /**
   * @brief Get scheduler's type
   * @return Alwayes return SchedulerType::SCHEDULER_TYPE_TIMEAFFI
   */
  inline SchedulerType GetType() override {
    return SchedulerType::SCHEDULER_TYPE_TIMEAFFI;
  }

  /**
   * @brief Unregister a cycling task
   * @param name - The name of layout
   * @return false - Layout [] not added
   *                 Scheduler busy
   *                 Layout [{}] not registered
   */
  bool UnregisterTask(const std::string &name);

  /**
   * @brief Clear all tasks create by task_producer
   * @param task_producer - The name of task producer
   * @return return - ErrorCode::OK
   */
  int32_t ClearTasks(uint64_t task_producer) override;

  /**
   * @brief Clear all unregistered tasks when scheduler stopped
   *
   */
  int32_t ClearTasks() override;

  /**
   * @brief SetTaskNullHook
   * @param hook - Function called when GetTask() return false
   * @return false - Scheduler busy
   */
  bool SetTaskNullHook(const TimeAffiHookFunc &hook);

  /**
   * @brief SetTaskDelayHook
   * @param hook - Function called when user task called delay
   * @return false - Scheduler busy
   */
  bool SetTaskDelayHook(const TimeAffiHookFunc &hook);

  /**
   * @brief SetTaskTimeoutHook
   * @param hook - Function called when user task execution timeout
   * @return false - Scheduler busy
   */
  bool SetTaskTimeoutHook(const TimeAffiHookFunc &hook);

  /**
   * @brief EnableTaskNullHook
   * @param enable - Enable Hook
   */
  void EnableTaskNullHook(const bool enable) {
    hooks_.task_null_enable = enable;
  }

  /**
   * @brief EnableTaskDelayHook
   * @param enable - Enable Hook
   */
  void EnableTaskDelayHook(const bool enable) {
    hooks_.task_delay_enable = enable;
  }

  /**
   * @brief EnableTaskTimeoutHook
   * @param enable - Enable Hook
   */
  void EnableTaskTimeoutHook(const bool enable) {
    hooks_.task_timeout_enable = enable;
  }

 protected:
  /**
   * @brief GetFrameCycle
   * @return Always return cycle_
   */
  inline uint32_t GetFrameCycle() { return cycle_; }

  ////////////////////////
  /// thread functions ///
  ////////////////////////

  /**
   * @brief Set thread policy\priority\affinity in timeslot
   * @param spTimeSlot - TimeSlot to use
   * @return false - ThreadSet[] SetThreadBindCPU error
   *                 ThreadSet[] SetThreadAttr error
   */
  bool ThreadSet(const std::shared_ptr<TimeSlot> &spTimeSlot);

  /**
   * @brief Work thread wait for next trigger/cycle
   * @param spTimeSlot - TimeSlot to use
   * @return false - Scheduler stopped
   */
  bool ThreadWaitTrigger(const std::shared_ptr<TimeSlot> &spTimeSlot);

  /**
   * @brief Work thread wait for next task
   * @param until - Target time point
   * @param out - Return time point
   * @return false - Scheduler stopped
   */
  bool ThreadWaitTime(const TimePoint &until, TimePoint &now);

  /**
   * @brief ThreadLoop on TRIGGER/CYCLIC mode
   * @param spTimeSlot - TimeSlot to use
   */
  void ThreadLoop(const std::shared_ptr<TimeSlot> &spTimeSlot);

  /**
   * @brief ThreadLoop on DEBUG mode
   * @param spTimeSlot - TimeSlot to use
   */
  void DebugLoop(const std::shared_ptr<TimeSlot> &spTimeSlot);

  /**
   * @brief DebugWaitLoop
   */
  void DebugWaitLoop();

 private:
  std::mutex mutex_;
  uint32_t timeslot_num_{0};
  uint32_t cycle_{0};
  TimeAffiSchedulerWorkMode work_mode_{TimeAffiSchedulerWorkMode::TRIGGER};
  std::unordered_map<std::string, std::shared_ptr<TaskMgr> > spTaskMgr_umap_;
  std::vector<std::shared_ptr<TimeSlot> > timeslot_vector_;

  struct {
    bool task_null_enable{true};
    bool task_delay_enable{true};
    bool task_timeout_enable{true};
    TimeAffiHookFunc task_null;
    TimeAffiHookFunc task_delay;
    TimeAffiHookFunc task_timeout;
  } hooks_;

  struct {
    uint32_t count{0};
    std::string wcet_path;
    std::thread thread;
  } debug_;

  struct {
    /**
     * For calculating the trigger time point of the next frame
     * time: Expected frame start time point
     * trigger_time: Actual frame start time point
     */
    TimePoint time;
    TimePoint trigger_time;

    /**
     * For scheduling thread synchronization
     * sync_count: The number of scheduling thread
     * sync_frame: Use in futex for scheduling thread sleep/wakeup
     *    store() in Trigger()
     *    fetch_sub() in ThreadWaitTrigger()
     */
    alignas(cpu_cacheline_size) std::atomic<uint32_t> sync_count{0};
    alignas(cpu_cacheline_size) std::atomic<uint32_t> sync_frame{0};

    /**
     * Scheduler running state
     * Set 0(termination) in Stop()
     * Set 1(continue) in Start()
     */
    alignas(cpu_cacheline_size) std::atomic<uint32_t> running{0};

    /**
     * Continuous trigger times
     * 0 - schedluer idle (set by last thread in ThreadWaitTrigger())
     * !=0 - schedluer busy (set in Trigger())
     */
    alignas(cpu_cacheline_size) std::atomic<uint32_t> count{0};
  } trigger_;
};
#pragma pack()

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_TIME_AFFI_SCHEDULER_TIME_AFFI_SCHEDULER_H_
