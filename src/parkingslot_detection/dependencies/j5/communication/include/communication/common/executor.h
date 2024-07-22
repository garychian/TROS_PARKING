
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

//  This is executor definition.


#ifndef COMMUNICATION_COMMON_EXECUTOR_H_
#define COMMUNICATION_COMMON_EXECUTOR_H_

#include <utility>
#include <mutex>
#include <memory>
#include <string>

#include "communication/utils/thread_pool.h"
#include "communication/common/thread_types.h"
#include "communication/common/comm_log.h"

#ifndef DLLEXPORT

#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

class DLLEXPORT ExecutorDelegation {
 public:
  virtual bool AddTask(const std::function<void()> &func) = 0;
  virtual int GetThreadCount() = 0;
  ExecutorDelegation() = default;
  virtual ~ExecutorDelegation() = default;
};

class DLLEXPORT Executor {
 public:
  using MessageListener =
    std::function<void(const spMsg &,
                     const spMessageInfo &, const std::string &topic)>;

  //  using DesCallBack =
  //  std::function<void(const std::string &,
  //                    const spSerData &,
  //                    const spMessageInfo &)>;

  //  using DesCallBackForPrefixMatch =
  //  std::function<void(const std::string &,
  //                    const std::string &,
  //                    const spSerData &,
  //                    const spMessageInfo &)>;

  static std::shared_ptr<Executor> New(int threads_cnt = 1);

  static std::shared_ptr<Executor> New(
    const std::shared_ptr<ExecutorDelegation> &executor);

  static std::shared_ptr<Executor>& GetThreadPoolExecutor();

  static bool SetMaxThreadNum(int thread_num);

  static int GetMaxThreadNum();

  static int GetCurrAllThreadNum();

  ~Executor();

  template<typename FuncType, typename... Args>
  std::future<typename std::result_of<FuncType(Args...)>::type>
  AddTask(FuncType func, Args... args) {
    if (!use_scheduler_group_ && thread_) {
      return thread_->AddTask(func, std::forward<Args>(args)...);
    }
    using ReturnType = typename std::result_of<FuncType(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<ReturnType()> >(
        std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...));
    auto common_func = [task]() { (*task)(); };
    if (use_scheduler_group_) {
      (void)AddScheduleGroupTask(common_func);
    } else {
      if (executor_delegation_) {
        (void)executor_delegation_->AddTask(common_func);
      }
    }
    return task->get_future();
  }

  template<typename FuncType, typename... Args>
  std::future<typename std::result_of<FuncType(Args...)>::type>
  AddLongRunningTask(FuncType func, Args... args) {
    using ReturnType = typename std::result_of<FuncType(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<ReturnType()> >(
        std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...));
    auto common_func = [task]() { (*task)(); };
    try {
      if (use_scheduler_group_) {
        AddScheduleGroupThread(common_func);
      } else {
        std::thread{common_func}.detach();
      }
    } catch (...) {
    }
    return task->get_future();
  }

  int GetThreadNum();

  bool IsSingleThreaded();

  bool AddTask(MessageListener func,
               const spMsg &msg,
               const spMessageInfo &msg_info,
               const std::string &topic);
  //  bool AddTask(DesCallBack func,
  //              const std::string &topic,
  //              const spSerData &ser_data,
  //              const spMessageInfo &msg_info);
  //  bool AddTask(DesCallBackForPrefixMatch func,
  //              const std::string &recv_topic,
  //              const std::string &prefix_topic,
  //              const spSerData &ser_data,
  //              const spMessageInfo &msg_info);

  std::shared_ptr<ExecutorDelegation> &GetExecutorDelegation() {
    return executor_delegation_;
  }

 private:
  explicit Executor(int threads_cnt = 1);
  explicit Executor(const std::shared_ptr<ExecutorDelegation> &executor);

  bool AddScheduleGroupTask(std::function<void()> task);
  void AddScheduleGroupThread(std::function<void()> func);

 private:
  static std::atomic_int max_thread_num_;
  static std::atomic_int all_thread_num_;
  static std::mutex mutex_;
  std::shared_ptr<ThreadPoolAT> thread_;
  std::shared_ptr<ExecutorDelegation> executor_delegation_;
  int thread_num_{0};
  int thread_cnt_{0};
  bool use_scheduler_group_{false};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_EXECUTOR_H_
