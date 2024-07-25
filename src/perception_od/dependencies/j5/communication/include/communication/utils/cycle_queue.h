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

// This is the implementation of a simple cycle queue

#ifndef COMMUNICATION_UTILS_CYCLE_QUEUE_H_
#define COMMUNICATION_UTILS_CYCLE_QUEUE_H_

#if defined(_MSC_VER)
#pragma warning(disable: 4251)
#endif

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {

namespace communication {
template<typename T, typename Lk>
class DLLEXPORT CycleQueue {
 public:
  explicit CycleQueue(int cycle_size) :
      vec_(cycle_size), size_(cycle_size),
      next_pos_(0) {}

  void Lock() {
    lock_.lock();
  }

  void UnLock() {
    lock_.unlock();
  }

  void Push(const T &item) {
    if (next_pos_ == size_) {
      next_pos_ = 0;
    }
    vec_[next_pos_++] = item;
  }

  std::vector<T> &AllItems() {
    return vec_;
  }

  ~CycleQueue() = default;

 private:
  std::vector<T> vec_;
  int size_;
  int next_pos_;
  Lk lock_;
};

class DLLEXPORT BaseQueue {
 public:
  explicit BaseQueue(std::function<void(bool)> state_cb)
      : state_cb_{state_cb}, type_{0} {}
  virtual ~BaseQueue() = default;
  int GetType() {
    return type_;
  }
 protected:
  std::function<void(bool)> state_cb_;
 private:
  int type_;
};

// Type T must be pointer-like type.
template<typename T>
class DLLEXPORT FixedSizeQueue : public BaseQueue {
 public:
  explicit FixedSizeQueue(int size,
                          std::function<void(bool)> state_cb = nullptr)
    : BaseQueue(state_cb), size_(size) {}
  ~FixedSizeQueue() = default;

  void Push(const T &item) {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    if (size_ > 0) {
      if (queue_.size() ==
                        static_cast<typename std::queue<T>::size_type>(size_)) {
        queue_.pop();
      }
      queue_.push(item);
      state_cb_(true);
      variable_.notify_all();
    } else if (size_ == 0) {
      only_one = item;
      state_cb_(true);
      variable_.notify_all();
    } else {
      return;
    }
  }

  T Pop(int timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (size_ > 0) {
      if (!queue_.empty()) {
        auto t = queue_.front();
        queue_.pop();
        state_cb_(!queue_.empty());
        return t;
      }
      auto pred = [this] {
        return !queue_.empty();
      };
      if (timeout < 0) {
        variable_.wait(lock, pred);
      } else {
        variable_.wait_for(lock, std::chrono::milliseconds(timeout), pred);
      }
      if (queue_.empty()) {
        // just for pointer-like type
        state_cb_(false);
        return null_one;
      }
      auto t = queue_.front();
      queue_.pop();
      state_cb_(!queue_.empty());
      return t;
    } else if (size_ == 0) {
      only_one = nullptr;
      auto pred = [this] {
        return only_one != nullptr;
      };
      if (timeout < 0) {
        variable_.wait(lock, pred);
      } else {
        variable_.wait_for(lock, std::chrono::milliseconds(timeout), pred);
      }
      state_cb_(false);
      return only_one;
    } else {
      return null_one;
    }
  }

  int GetSize() {
    return size_;
  }

  int Size() {
    std::unique_lock<std::mutex> lock(mutex_);
    return static_cast<int>(queue_.size());
  }

 private:
  T null_one;
  T only_one;  // just for size = 0
  std::queue<T> queue_;
  int size_;
  std::mutex mutex_;
  std::condition_variable variable_;
};

}  // namespace communication
}  // namespace hobot
#endif  // COMMUNICATION_UTILS_CYCLE_QUEUE_H_
