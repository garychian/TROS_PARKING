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

#ifndef SCHEDULEGROUP_COROUTINE_SCHEDULER_COROUTINE_SCHEDULER_H_
#define SCHEDULEGROUP_COROUTINE_SCHEDULER_COROUTINE_SCHEDULER_H_

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "schedulegroup/coroutine_scheduler/session.h"
#include "schedulegroup/schedule_group.h"
#include "schedulegroup/scheduler_type.h"
#include "schedulegroup/task.h"

namespace hobot {
namespace schedulegroup {

class Processor;
class ProcessorContext;
class EventHandler;
class Coroutine;
enum IoEventRet { IO_TRIGGER, TIME_OUT, FORCE_STOPPED, ERROR_OCCUR };
class BOLE_PUBLIC CoroutineScheduler : public ScheduleGroup {
  // Starting->Running->Stopped...->Starting->Running->Stopped->Shutdown
  enum class Status { Starting, Running, Stopped, Shutdown };

 public:
  friend class CoMutex;
  friend class ProcessorContext;
  friend class Coroutine;
  friend class CoCondVar;

  /**
   * @brief Construct the coroutine scheduler
   *
   * @param name the timer scheduler's name
   * @param the num of processor threads
   * @param the num of max allowed coroutines num, default 256
   * @param the num of max priority, default 1
   * @return int32_t the Errorcode of create instance
   */

  static int32_t CreateInstance(const std::string &name, uint32_t thread_num,
                                uint32_t max_cr_num = 256,
                                TaskPriority max_prio = 1);

  /**
   * @brief Get the instance of coroutine scheduler
   *
   * @return std::shared_ptr<CoroutineScheduler>& the instance
   *  of coroutine scheduler
   */
  static std::shared_ptr<CoroutineScheduler> Instance();

  /**
   * @brief Remove the instance of coroutine scheduler
   *
   * @return int32_t the Errorcode of remove coroutine scheduler
   */
  static int32_t RemoveInstance();

  ~CoroutineScheduler();

  /**
   * @brief Get the Type of the schedulegroup
   *
   * @return SchedulerType the type of the schedulegroup
   */
  SchedulerType GetType() override {
    return SchedulerType::SCHEDULER_TYPE_COROUTINE;
  };

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
  int32_t AddTask(const std::shared_ptr<Task> &spTask) override;

  /**
   * @brief clear tasks which be added to schedulegroup before
   *
   * @param task_producer the tasks' producer
   * @return int32_t the returned error code
   */
  int32_t ClearTasks(uint64_t task_producer) override;
  int32_t ClearTasks() override;

  /**
   * @brief Set processor threads' sched policy and priority
   *
   * @param policy thread's sched policy
   * @param prio thread's sched priority
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool SetAttr(SchedulePolicy policy, int32_t prio) override;

  /**
   * @brief Set processor threads' sched policy and priority
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
   * @brief set all processor threads' cpu affinity
   *        this interface only store the configuration
   *
   * @param cpu_set cpu set on which the thread can run
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool BindCPU(const std::vector<unsigned int> &cpu_set) override;

  /**
   * @brief set processor thread's cpu affinity
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
   * @brief Get the number of the processor threads in this schedulegroup
   *
   * @return uint32_t the number of processor thread
   */
  uint32_t GetThreadNum() const override;

  /**
   * @brief Get the max coroutine num of scheduler
   *
   * @return uint32_t the number of max coroutine num
   */
  uint32_t GetMaxCrNum() const;

  /**
   * @brief Set poller thread's sched policy and priority
   *
   * @param policy thread's sched policy
   * @param prio thread's sched priority
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool SetPollHandlerThreadAttr(SchedulePolicy policy, int32_t prio);

  /**
   * @brief set poller thread's cpu affinity
   *        this interface only store the configuration
   *
   * @param cpu_set the cpusets of the thread
   * @return true the parameter is valid
   * @return false the parameter is invalid
   */
  bool BindPollHandlerThreadCPU(const std::vector<unsigned int> &cpu_set);

  friend int Socket(int domain, int type, int protocol);
  friend int Listen(int fd, int backlog);
  friend int Bind(int fd, const struct sockaddr *addr, socklen_t addrlen);
  friend int Close(int fd);
  friend int Accept(int fd, struct sockaddr *addr, socklen_t *addrlen);
  friend int Connect(int fd, const struct sockaddr *addr, socklen_t addrlen);
  friend ssize_t Recv(int fd, void *buf, size_t len, int flags,
                      int64_t timeout_us);
  friend ssize_t RecvFrom(int fd, void *buf, size_t len, int flags,
                          struct sockaddr *src_addr, socklen_t *addrlen,
                          int64_t timeout_us);
  friend ssize_t Send(int fd, const void *buf, size_t len, int flags,
                      int64_t timeout_us);
  friend ssize_t SendTo(int fd, const void *buf, size_t len, int flags,
                        const struct sockaddr *dest_addr, socklen_t addrlen,
                        int64_t timeout_us);
  friend ssize_t Read(int fd, void *buf, size_t count, int64_t timeout_us);
  friend ssize_t Write(int fd, const void *buf, size_t count,
                       int64_t timeout_us);

 private:
  CoroutineScheduler(const std::string &name, uint32_t thread_num,
                     uint32_t max_cr_num = 256, TaskPriority max_prio = 1);
  /**
   * @brief Set all threads name
   */
  bool SetAllThreadsName();

  /**
   * @brief Register IO event until after wait timeout
   *
   * @param file descriptor
   * @param is_read type of event
   * @param timeout_us the wait time(us) of coroutine until after timeout
   * @return true wait fd event success, get IO requests
   * @return false wait fd event timeout, not get IO requests
   */
  IoEventRet RegisterIOEvent(int fd, bool is_read, int64_t timeout_us = -1);
  /**
   * @brief Remove IO event
   *
   * @param file descriptor
   */
  void UnRegisterIOEvent(int fd);

  /**
   * @brief Register timer event
   *
   * @param timeout_us the wait time(us) of the coroutine
   */

  std::chrono::steady_clock::time_point RegisterTimerEvent(uint64_t timeout_us,
                                                           uint32_t id);

  bool UnRegisterTimerEvent(std::chrono::steady_clock::time_point &point,
                            uint32_t id);

  /**
   * @brief Remove wait for coroutine
   *
   * @param timeout_us abstract time of coroutine
   * @param id the id of coroutine
   */
  bool NotifyProcessor(uint32_t num = 1);

 private:
  static std::shared_ptr<CoroutineScheduler> instance_;
  std::vector<std::shared_ptr<Processor>> processors_;
  std::shared_ptr<ProcessorContext> global_context_;
  uint32_t thread_num_;
  uint32_t max_cr_num_;
  TaskPriority max_prio_;
  std::atomic<Status> status_{Status::Stopped};
};

}  // namespace schedulegroup
}  // namespace hobot
#endif  // SCHEDULEGROUP_COROUTINE_SCHEDULER_COROUTINE_SCHEDULER_H_
