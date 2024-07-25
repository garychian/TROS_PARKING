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

#ifndef COMMUNICATION_READER_HISTORY_CACHE_H_
#define COMMUNICATION_READER_HISTORY_CACHE_H_
#include <map>
#include <memory>
#include <string>
#include "communication/participant/dispatcher.h"
#include "communication/service/message_info.h"
#include "message/message.h"

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

using hobot::message::spMsg;
struct DLLEXPORT ReaderMessageCache {
  int64_t seq_id;
  std::string topic_name;
  spMsg msg;
  spMessageInfo msg_info;
};

class DLLEXPORT ReaderHistoryCache {
 public:
  explicit ReaderHistoryCache(int capacity, int64_t seq_id);

  void AddMessage(std::shared_ptr<ReaderMessageCache> msg);

  void RemoveMessage(int64_t seq_id);

  std::shared_ptr<ReaderMessageCache> GetSmallestMsg();

  std::shared_ptr<ReaderMessageCache> GetSmallestNackMsg();

  bool HasSeq(int64_t seq_id);

  int64_t GetLastSeqId();

  void SetLastSeqId(int64_t seq_id);

  bool CacheFull();

  void DropFirst();

  void PrintDebugInfo();

 private:
  // last_seq_id has different meaning in Reliable Reliability
  // and Best Effort Reliability
  // Reliable: all msg which seq_id <= last_seq_id has been recieved
  // Best effort: the most recent recieved msg
  int64_t last_seq_id_;
  std::map<int64_t, std::shared_ptr<ReaderMessageCache>> message_cache_;
  int capacity_;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_READER_HISTORY_CACHE_H_
