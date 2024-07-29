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

#ifndef SCHEDULEGROUP_THREAD_POOL_UTILS_UTIL_H_
#define SCHEDULEGROUP_THREAD_POOL_UTILS_UTIL_H_

#include <atomic>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#elif defined __MACH__
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && \
    __MAC_OS_X_VERSION_MIN_REQUIRED < 101200
#include <libkern/osAtomic.h>
#endif
#endif

namespace hobot {
namespace schedulegroup {

uint64_t GenerateUniqueID() noexcept;

// windows platform
#ifdef _WIN32
// windows already provides a Membarrier macro

// mac platform
#elif defined __APPLE__
inline void MemoryBarrier() {
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && \
    __MAC_OS_X_VERSION_MIN_REQUIRED < 101200
  OSMemoryBarrier();
#else
  std::atomic_thread_fence(std::memory_order_seq_cst);
#endif
}

// linux
#elif defined __linux__
#ifdef __aarch64__
inline void MemoryBarrier() { asm volatile("dmb sy" : : : "memory"); }
#else
inline void MemoryBarrier() { __asm__ __volatile__("" : : : "memory"); }
#endif
#endif

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_THREAD_POOL_UTILS_UTIL_H_
