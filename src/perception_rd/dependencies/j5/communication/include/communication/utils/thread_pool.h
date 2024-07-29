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

// This is the implementation of a general thread pool

#ifndef COMMUNICATION_UTILS_THREAD_POOL_H_
#define COMMUNICATION_UTILS_THREAD_POOL_H_

#include <algorithm>
#include <queue>
#include <future>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <functional>
#include "communication/common/message_wrapper.h"
#include "communication/service/message_info.h"

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

void DLLEXPORT SetThreadName(const std::string &th_name);

class ThreadPoolAT {
 public:
//  static ThreadPoolAT *Instance(int threads_cnt = 0) {
//    static ThreadPoolAT thread_pool_obj(threads_cnt);
//    return &thread_pool_obj;
//  }
  using MessageListener =
    std::function<void(const spMsg &,
                     const spMessageInfo &, const std::string &topic)>;
  using DesCallBack =
    std::function<void(const std::string &,
                     const spSerData &,
                     const spMessageInfo &)>;

  using DesCallBackForPrefixMatch =
    std::function<void(const std::string &,
                     const std::string &,
                     const spSerData &,
                     const spMessageInfo &)>;

  static std::shared_ptr<ThreadPoolAT> NewThreadPool(int threads_cnt = 1);

  int GetThreadNum();

  ~ThreadPoolAT();

  template<typename FuncType, typename... Args>
  std::future<typename std::result_of<FuncType(Args...)>::type> AddTask(
      FuncType func, Args... args);

  // if you have a long running task(such as some task which will wait for
  // condition_variable, and run this task will take long time), prefer to use
  // AddLongRunningTask(..) which put blocked task in a detached thread,
  // in oreder avoid exhaust thread pool
  template<typename FuncType, typename... Args>
  std::future<typename std::result_of<FuncType(Args...)>::type>
  AddLongRunningTask(
      FuncType func, Args... args);

  ThreadPoolAT(const ThreadPoolAT &) = delete;
  ThreadPoolAT(ThreadPoolAT &&) = delete;
  ThreadPoolAT &operator=(const ThreadPoolAT &) = delete;
  ThreadPoolAT &operator=(ThreadPoolAT &&) = delete;

  bool AddTask(MessageListener func,
               const spMsg &msg,
               const spMessageInfo &msg_info,
               const std::string &topic) {
    std::lock_guard<std::mutex> lck(cond_mutex_);
    if (stop_flag_) {
      return false;
    }
    task_queue_.emplace(
      [func, msg, msg_info, topic]() {func(msg, msg_info, topic);});
    sync_cond_.notify_one();
    return true;
  }

  // bool AddTask(DesCallBack func,
  //              const std::string &topic,
  //              const spSerData &ser_data,
  //              const spMessageInfo &msg_info) {
  //   std::lock_guard<std::mutex> lck(cond_mutex_);
  //   if (stop_flag_) {
  //     return false;
  //   }
  //   task_queue_.emplace(
  //   [func, topic, ser_data, msg_info]() {func(topic, ser_data, msg_info);});
  //   sync_cond_.notify_one();
  //   return true;
  // }

  // bool AddTask(DesCallBackForPrefixMatch func,
  //               const std::string &recv_topic,
  //               const std::string &prefix_topic,
  //               const spSerData &ser_data,
  //               const spMessageInfo &msg_info) {
  //   std::lock_guard<std::mutex> lck(cond_mutex_);
  //   if (stop_flag_) {
  //     return false;
  //   }
  //   task_queue_.emplace(
  //     [func, recv_topic, prefix_topic, ser_data, msg_info]() {
  //       func(recv_topic, prefix_topic, ser_data, msg_info);});
  //   sync_cond_.notify_one();
  //   return true;
  // }

 private:
  explicit ThreadPoolAT(int threads_cnt = 0);

  void ThreadWorker();

 private:
  bool stop_flag_;
  std::mutex cond_mutex_;
  std::condition_variable sync_cond_;

  std::queue<std::function<void()> > task_queue_;
  std::vector<std::thread> threads_vec_;
  int thread_num_;
};

template<typename FuncType, typename... Args>
std::future<typename std::result_of<FuncType(Args...)>::type>
ThreadPoolAT::AddTask(FuncType func, Args... args) {
  using ReturnType = typename std::result_of<FuncType(Args...)>::type;
  auto task = std::make_shared<std::packaged_task<ReturnType()> >(
      std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...));
  std::future<ReturnType> func_future{task->get_future()};
  {
    std::lock_guard<std::mutex> lck{cond_mutex_};
    if (stop_flag_) {
      return std::future<ReturnType>();
    }
    task_queue_.emplace([task]() { (*task)(); });
  }
  sync_cond_.notify_one();
  return func_future;
}

template<typename FuncType, typename... Args>
std::future<typename std::result_of<FuncType(Args...)>::type>
ThreadPoolAT::AddLongRunningTask(FuncType func, Args... args) {
  using ReturnType = typename std::result_of<FuncType(Args...)>::type;
  auto task = std::make_shared<std::packaged_task<ReturnType()> >(
      std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...));
  std::future<ReturnType> func_future{task->get_future()};
  // TODO(maosong): we need a better way to manager threads to handle blocked
  // tasks, avoiding create threads frequently
  try {
    std::thread{[task]() { (*task)(); }}.detach();
  } catch (...) {
  }
  return func_future;
}

}  // namespace communication
}  // namespace hobot
#endif  // COMMUNICATION_UTILS_THREAD_POOL_H_
