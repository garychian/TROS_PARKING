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

#ifndef COMMUNICATION_COMMON_ATOMIC_RW_LOCK_H_
#define COMMUNICATION_COMMON_ATOMIC_RW_LOCK_H_


#include <atomic>
#include <cstdint>

#include "rw_lock_guard.h"

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#endif

namespace hobot {
namespace communication {

class DLLEXPORT AtomicRWLock {
  friend class ReadLockGuard<AtomicRWLock>;
  friend class WriteLockGuard<AtomicRWLock>;

 public:
  static const int32_t RW_LOCK_FREE{0};
  static const int32_t WRITE_EXCLUSIVE{-1};
  static const uint32_t MAX_RETRY_TIMES{5u};
  AtomicRWLock();
  explicit AtomicRWLock(bool write_first);

 private:
  // all these function only can used by ReadLockGuard/WriteLockGuard;
  void ReadLock();
  void WriteLock();

  void ReadUnlock();
  void WriteUnlock();

  AtomicRWLock(const AtomicRWLock&) = delete;
  AtomicRWLock& operator=(const AtomicRWLock&) = delete;
  std::atomic<uint32_t> write_lock_wait_num_{0u};
  std::atomic<int32_t> lock_num_{0};
  bool write_first_{true};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_ATOMIC_RW_LOCK_H_
