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

#ifndef SCHEDULEGROUP_TIME_AFFI_SCHEDULER_LOCKFREEQUEUE_H_
#define SCHEDULEGROUP_TIME_AFFI_SCHEDULER_LOCKFREEQUEUE_H_

#include <atomic>
#include <utility>

namespace hobot {
namespace schedulegroup {

/**
 * @brief The cache line length of the target CPU
 */
constexpr uint32_t lfq_cacheline_size = 64;

template <typename T>
class LockFreeQueue {
 public:
  explicit LockFreeQueue(uint32_t reserve_size = 4096) {
    if (0 == reserve_size) {
      throw std::runtime_error("LockFreeQueue: reserve size = 0");
    }
    recovery_.reserve = reserve_size;

    Node *mhead = new Node();
    main_.head.store(mhead, std::memory_order_relaxed);
    main_.tail.store(mhead, std::memory_order_relaxed);
    main_.size.store(0, std::memory_order_relaxed);

    Node *uhead = new Node();
    unsafe_.head = uhead;
    unsafe_.tail.store(uhead, std::memory_order_relaxed);
    unsafe_.ref.store(0, std::memory_order_relaxed);

    Node *rhead = new Node[recovery_.reserve];
    recovery_.position = &rhead[0];
    recovery_.boundary = &rhead[recovery_.reserve - 1];

    recovery_.ringbuffer = new Node *[recovery_.reserve];
    for (uint64_t i = 0; i < recovery_.reserve; ++i) {
      recovery_.ringbuffer[i] = &rhead[i];
    }
    recovery_.rc.store(0, std::memory_order_relaxed);
    recovery_.wc.store(recovery_.reserve, std::memory_order_relaxed);
  }

  LockFreeQueue() = delete;
  LockFreeQueue(const LockFreeQueue &) = delete;
  LockFreeQueue &operator=(const LockFreeQueue &) = delete;

/// Filter for reserve nodes.
#define COMPARE_AND_DELETE(PTR)                                   \
  if ((recovery_.boundary < PTR) || (PTR < recovery_.position)) { \
    delete PTR;                                                   \
  }

  ~LockFreeQueue() {
    /// Delete all unreserved nodes in main queue.
    Node *mhead = main_.head.load(std::memory_order_acquire);
    while (mhead) {
      Node *backup = mhead->next;
      COMPARE_AND_DELETE(mhead);
      mhead = backup;
    }

    /// Delete all unreserved nodes in unsafe queue.
    Node *utail = unsafe_.tail.load(std::memory_order_acquire);
    utail->next = nullptr;  ///< Make the end of unsafe queue.

    Node *uhead = unsafe_.head;
    while (uhead) {
      Node *backup = uhead->next;
      COMPARE_AND_DELETE(uhead);
      uhead = backup;
    }

    /// Delete all reserved nodes.
    delete[] recovery_.position;
    delete[] recovery_.ringbuffer;
  }

  inline void Enqueue(const T &para) {
    Node *pNew;
    /// Try get a node from recovery queue.
    uint64_t new_rc;
    uint64_t now_rc = recovery_.rc.load(std::memory_order_acquire);
    while (true) {
      new_rc = now_rc + 1;
      if (recovery_.wc.load(std::memory_order_acquire) < new_rc) {
        pNew = new Node();
        pNew->usrdata = para;
        break;
      }

      /// Readout node pointer before CAS.
      pNew = recovery_.ringbuffer[GetIndex(now_rc)];
      if (recovery_.rc.compare_exchange_weak(now_rc, new_rc,
                                             std::memory_order_acq_rel,
                                             std::memory_order_acquire)) {
        pNew->usrdata = para;
        pNew->next = nullptr;
        break;
      }
    }

    /// Push new node to main_queue.
    Node *now_mtail = main_.tail.load(std::memory_order_acquire);
    while (true) {
      if (main_.tail.compare_exchange_weak(now_mtail, pNew,
                                           std::memory_order_acq_rel,
                                           std::memory_order_acquire)) {
        main_.size.fetch_add(1, std::memory_order_relaxed);
        now_mtail->next = pNew;
        break;
      }
    }
  }

  inline void Enqueue(T &&para) {
    Node *pNew;
    /// Try get a node from recovery queue.
    uint64_t new_rc;
    uint64_t now_rc = recovery_.rc.load(std::memory_order_acquire);
    while (true) {
      new_rc = now_rc + 1;
      if (recovery_.wc.load(std::memory_order_acquire) < new_rc) {
        pNew = new Node();
        pNew->usrdata = std::move(para);
        break;
      }

      /// Readout node pointer before CAS.
      pNew = recovery_.ringbuffer[GetIndex(now_rc)];
      if (recovery_.rc.compare_exchange_weak(now_rc, new_rc,
                                             std::memory_order_acq_rel,
                                             std::memory_order_acquire)) {
        pNew->usrdata = std::move(para);
        pNew->next = nullptr;
        break;
      }
    }

    /// Push new node to main_queue.
    Node *now_mtail = main_.tail.load(std::memory_order_acquire);
    while (true) {
      if (main_.tail.compare_exchange_weak(now_mtail, pNew,
                                           std::memory_order_acq_rel,
                                           std::memory_order_acquire)) {
        main_.size.fetch_add(1, std::memory_order_relaxed);
        now_mtail->next = pNew;
        break;
      }
    }
  }

  inline bool Dequeue(T *p) {
    unsafe_.ref.fetch_add(1, std::memory_order_acq_rel);

    /// Try get the head node of main queue.
    Node *now_mnext;
    Node *now_mhead = main_.head.load(std::memory_order_acquire);
    while (true) {
      now_mnext = now_mhead->next;
      if (!now_mnext) {
        unsafe_.ref.fetch_sub(1, std::memory_order_release);
        return false;
      }

      if (main_.head.compare_exchange_weak(now_mhead, now_mnext,
                                           std::memory_order_acq_rel,
                                           std::memory_order_acquire)) {
        main_.size.fetch_sub(1, std::memory_order_relaxed);
        *p = std::move(now_mnext->usrdata);

        /// now_mhead is the Node token down, push it to unsafe queue.
        Node *now_utail = unsafe_.tail.load(std::memory_order_acquire);
        while (true) {
          if (unsafe_.tail.compare_exchange_weak(now_utail, now_mhead,
                                                 std::memory_order_acq_rel,
                                                 std::memory_order_acquire)) {
            now_utail->next = now_mhead;
            break;
          }
        }
        break;
      }
    }

    if (1 == unsafe_.ref.load(std::memory_order_acquire)) {
      /// Only one thread can run here.
      Node *pTmp;
      do {
        pTmp = unsafe_.head;
        /// now_mhead is the dividing line of unsafe node.
        if (pTmp == now_mhead) {
          break;
        }
        unsafe_.head = pTmp->next;

        COMPARE_AND_DELETE(pTmp)
        // clang-format off
        else
        {
          /// recovery_.ringbuffer is definitely not full.
          uint64_t now_wc = recovery_.wc.load(std::memory_order_acquire);
          recovery_.ringbuffer[GetIndex(now_wc)] = pTmp;
          recovery_.wc.fetch_add(1, std::memory_order_release);
        }
        // clang-format on
      } while (true);
    }

    unsafe_.ref.fetch_sub(1, std::memory_order_release);
    return true;
  }

  inline uint64_t Clear() {
    T data;
    uint64_t i = 0;
    uint64_t len = main_.size.load(std::memory_order_relaxed);
    while (len-- && Dequeue(&data)) {
      i++;
    }
    return i;
  }

  /**
   * @brief Get the size of queue
   * @return The size of queue
   * @note Call Dequeue() maybe failed even if 0 < size_
   */
  inline uint64_t Size() { return main_.size.load(std::memory_order_relaxed); }

  inline bool Empty() {
    return main_.size.load(std::memory_order_relaxed) == 0;
  }

 private:
  inline uint64_t GetIndex(uint64_t num) {
    return num - (num / recovery_.reserve) * recovery_.reserve;
  }

 private:
  struct Node {
    Node() : next(nullptr) {}
    Node *next;
    T usrdata;
  };

  /// MPMC: for user data
  struct {
    alignas(lfq_cacheline_size) std::atomic<Node *> head;
    alignas(lfq_cacheline_size) std::atomic<Node *> tail;
    alignas(lfq_cacheline_size) std::atomic<uint64_t> size;
  } main_;

  /// MPSC: for cache and filter
  struct {
    alignas(lfq_cacheline_size) Node *head;
    alignas(lfq_cacheline_size) std::atomic<Node *> tail;
    alignas(lfq_cacheline_size) std::atomic<uint64_t> ref;
  } unsafe_;

  /// SPMC: for recovery
  struct {
    alignas(lfq_cacheline_size) std::atomic<uint64_t> rc;  ///< read counter
    alignas(lfq_cacheline_size) std::atomic<uint64_t> wc;  ///< write counter

    alignas(lfq_cacheline_size) Node *position;
    Node *boundary;
    Node **ringbuffer;
    uint32_t reserve;
  } recovery_;
};

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_TIME_AFFI_SCHEDULER_LOCKFREEQUEUE_H_
