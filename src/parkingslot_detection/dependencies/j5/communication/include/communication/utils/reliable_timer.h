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

#ifndef COMMUNICATION_RELIABLE_TIMER_H_
#define COMMUNICATION_RELIABLE_TIMER_H_
#include <functional>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>
#include <string>

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable : 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

typedef std::function<void()> TimerCallback;

class DLLEXPORT ReliableTimer {
 public:
  ReliableTimer(int interval_ms, TimerCallback call_back,
                const std::string& th_name = "commReliableTimer");

  void start();
  void stop();

  void ChangeInterval(int interval_ms);
 private:
  int interval_ms_;
  TimerCallback call_back_;
  std::string th_name_;

  std::atomic_bool _expired{true};  // timer stopped status
  std::atomic_bool _try_to_expire{false};  // timer is in stop process
  std::condition_variable _expired_cond;
  std::mutex run_mutex_;
};

}  // namespace communication
}  // namespace hobot
#endif
