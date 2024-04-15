// Copyright (c) [2021-2023] [Horizon Robotics].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#pragma once
#include <time.h>

#include <chrono>

// namespace fanya {
// namespace parking {

static inline uint64_t GetTimeStamp() {
  auto time_now = std::chrono::system_clock::now();
  auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      time_now.time_since_epoch());

  return static_cast<uint64_t>(time_ms.count());
}

static inline uint64_t GetTimeStampUs() {
  auto time_now = std::chrono::system_clock::now();
  auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(
      time_now.time_since_epoch());

  return static_cast<uint64_t>(time_us.count());
}

static inline uint64_t GetThreadTime() {
  struct timespec time_thread = {0, 0};
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_thread);

  return (time_thread.tv_sec * 1000) + (time_thread.tv_nsec / 1000000);
}

static inline uint64_t GetThreadTimeUs() {
  struct timespec time_thread = {0, 0};
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_thread);

  return (time_thread.tv_sec * 1000000) + (time_thread.tv_nsec / 1000);
}

// }  // namespace parking
// }  // namespace fanya
