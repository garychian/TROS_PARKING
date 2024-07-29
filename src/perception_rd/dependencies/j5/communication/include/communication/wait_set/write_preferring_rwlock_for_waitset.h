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

#ifndef COMMUNICATION_WRITE_PREFERRING_RWLOCK_FOR_WAITSET_H_
#define COMMUNICATION_WRITE_PREFERRING_RWLOCK_FOR_WAITSET_H_

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

#include <condition_variable>
#include <functional>
#include <mutex>

namespace hobot {
namespace communication {

// write preferring read write lock for wait set
// It is different from the ordinary RW Lock in the way that it can wake up
// waitset when write mutex lock, and different reader mutually exclusive with
// each other, which means concurrent reading is prohibited.
class DLLEXPORT WritePrefRWLockForWS final {
 public:
  explicit WritePrefRWLockForWS(
      std::function<void()> writer_lock_function = nullptr);
  ~WritePrefRWLockForWS() = default;

  class DLLEXPORT WriteMutex {
   public:
    void lock();
    void unlock();

   private:
    friend class WritePrefRWLockForWS;
    explicit WriteMutex(WritePrefRWLockForWS &parent_lock);
    WritePrefRWLockForWS &parent_lock_;
  };

  class DLLEXPORT ReadMutex {
   public:
    void lock();
    void unlock();

   private:
    friend class WritePrefRWLockForWS;
    explicit ReadMutex(WritePrefRWLockForWS &parent_lock);
    WritePrefRWLockForWS &parent_lock_;
  };

  WriteMutex &GetWriteMutex();
  ReadMutex &GetReadMutex();

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
  WriteMutex write_mutex_;
  ReadMutex read_mutex_;
  std::function<void()> writer_lock_function_;
  bool reader_active_{false};
  bool writer_active_{false};
  uint16_t num_writer_waiting_{0U};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_WRITE_PREFERRING_RWLOCK_FOR_WAITSET
