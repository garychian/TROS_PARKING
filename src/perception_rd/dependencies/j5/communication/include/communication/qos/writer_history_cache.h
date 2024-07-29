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

#ifndef COMMUNICATION_WRITER_HISTORY_CACHE_H_
#define COMMUNICATION_WRITER_HISTORY_CACHE_H_

#include <atomic>
#include <chrono>
#include <map>
#include <memory>
#include <queue>
#include <vector>
#include <string>

#include "communication/common/message_wrapper.h"
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
struct DLLEXPORT WriterMessageCache {
  int64_t seq_id;
  std::map<std::string, bool> reader_id_acked;
  std::map<std::string, std::chrono::system_clock::time_point> reader_send_time;
  spMsg msg;
  spCommSerData data;
  spMessageInfo msg_info;

  bool AllAcked() {
    for (auto reader_ack : reader_id_acked) {
      if (!reader_ack.second) {
        return false;
      }
    }
    return true;
  }
};

class DLLEXPORT WriterHistoryCache {
 public:
  explicit WriterHistoryCache(int capacity);
  // add msg to buffer_to_send
  bool AddBuff(std::shared_ptr<WriterMessageCache> msg_cache);

  bool RemoveCache(int64_t seq_id);

  std::shared_ptr<WriterMessageCache> GetMsgCacheBySeqID(int64_t seq_id);

  void RemoveMsgReaders(int64_t seq_id,
                        const std::vector<std::string> &readers);

  void MsgAcked(int64_t seq_id, const std::string &reader_id);

  void RemoveAllAckedMsg();

  const std::map<int64_t, std::shared_ptr<WriterMessageCache>>
      &GetAllMsgCaches();

  bool CacheFull();

  size_t CacheSize();

  int DropFirst();

  void PrintDebugInfo();

  std::shared_ptr<WriterMessageCache> NextSendMsg(bool drop);

 private:
  int capacity_;
  std::map<int64_t, std::shared_ptr<WriterMessageCache>> buffer_sended;
  std::map<int64_t, std::shared_ptr<WriterMessageCache>> buffer_to_send;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_WRITER_HISTORY_CACHE_H_
