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

#ifndef COMMUNICATION_TOPICMANAGER_TOPICMANAGER_H_
#define COMMUNICATION_TOPICMANAGER_TOPICMANAGER_H_

#include <memory>
#include <unordered_map>
#include <string>
#include "message/message.h"
#include "communication/utils/atomic_rw_lock.h"

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

using Message = hobot::message::Message;
using spMsg = std::shared_ptr<Message>;

using Callback = std::function<void(const spMsg &)>;
using SubCallbackMap = std::unordered_map<uint64_t, Callback>;


class DLLEXPORT TopicManager {
 public:
  ~TopicManager();

  void AddCallback(uint64_t topic, uint64_t addr, const Callback &callback);

  void RemoveCallback(uint64_t topic, uint64_t addr);

  void Callback(uint64_t topic, spMsg msg);

  uint64_t GetTopicIndex(const std::string &topic);

  static TopicManager *Instance();

 private:
  AtomicRWLock rwlock_;
  std::mutex topic_index_map_lk_;
  std::unordered_map<uint64_t, SubCallbackMap> topic_callbacks_map_;
  std::unordered_map<std::string, uint64_t> topic_index_map_;
  uint64_t current_index_{0};

  TopicManager() {
  }
  TopicManager(const TopicManager &) = delete;
  TopicManager &operator=(const TopicManager &) = delete;
};

}  // namespace communication
}  // namespace hobot
#endif  // COMMUNICATION_TOPICMANAGER_TOPICMANAGER_H_
