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

#ifndef COMMUNICATION_UTILS_WAIT_FOR_MESSAGE_H_
#define COMMUNICATION_UTILS_WAIT_FOR_MESSAGE_H_

#include <map>
#include <string>
#include <memory>
#include <utility>
#include "communication/utils/cycle_queue.h"
#include "communication/subscriber.h"
#include "communication/common/comm_log.h"

namespace hobot {

namespace communication {

// for sync subscriber
template<typename Serializer>
std::shared_ptr<typename Serializer::MsgType>
WaitForMessage(const std::shared_ptr<Subscriber<Serializer>> &subscriber,
               int timeout = -1, std::string *topic = nullptr) {
  using SpMessage = std::shared_ptr<typename Serializer::MsgType>;
  using SpHistoryMsgType = std::shared_ptr<std::pair<SpMessage, std::string>>;
  if (!subscriber) {
    COHLOGE("subscriber param is null!");
    return nullptr;
  }
  auto queue = SubRcvGlobalObj::Instance()
               ->GetSubHistoryQueue(subscriber.get());
  if (queue == nullptr) {
    COHLOGE("this subscriber is not a sync subscriber,"
            "cannot use WaitForMessage()!");
    return nullptr;
  }
  auto real_queue =
      dynamic_cast<FixedSizeQueue<SpHistoryMsgType> *>(queue);
  if (real_queue == nullptr) {
    COHLOGE("dynamic_cast FixedSizeQueue failed!");
    return nullptr;
  }
  auto item = real_queue->Pop(timeout);
  if (item) {
    if (topic != nullptr) {
      *topic = item->second;
    }
    return item->first;
  } else {
    return nullptr;
  }
}

}  // namespace communication
}  // namespace hobot
#endif  // COMMUNICATION_UTILS_WAIT_FOR_MESSAGE_H_
