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

#ifndef COMMUNICATION_READER_RELIABILITY_QOS_H_
#define COMMUNICATION_READER_RELIABILITY_QOS_H_

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "communication/common/errors.h"
#include "communication/common/types.h"
#include "communication/qos/reader_history_cache.h"
#include "communication/qos/reliability_qos.h"

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

class ReliableReaderImpl;

class DLLEXPORT ReaderReliableReliabilityQos : public ReliabilityQos {
 public:
  ReaderReliableReliabilityQos(
      ProtocolType protocol_type, int domain_id, const std::string &topic_name,
      const HistoryQoSPolicy &history_qos,
      const ResourceLimitsQosPolicy &resource_limit_qos);

  ~ReaderReliableReliabilityQos() = default;

  bool Init(const std::string &uuid) override;

  ErrorCode CacheMessage(const std::string &topic_name, const spSerData &data,
                         const spMsg &msg,
                         const spMessageInfo &msg_info) override;

  std::shared_ptr<ReaderMessageCache> PopMessage() override;

 private:
  void AckMessage(const std::string &writer_id,
                  const std::vector<int64_t> &ack_list,
                  const std::vector<int64_t> &noack_list);

  void AckOneSuccess(const std::string &writer_id, int64_t seq_id);

 private:
  std::mutex reader_history_cache_mutex_;
  // key : writer id, value : history cache
  std::map<std::string, std::shared_ptr<ReaderHistoryCache>>
      reader_history_caches_;
  int cache_max_num_{0};
  std::shared_ptr<ReliableReaderImpl> reliable_reader_impl_{nullptr};
  int check_count_{0};  // check writer exit by msg count
};

class DLLEXPORT ReaderBestEffortReliabilityQos : public ReliabilityQos {
 public:
  ReaderBestEffortReliabilityQos(
      const HistoryQoSPolicy &history_qos,
      const ResourceLimitsQosPolicy &resource_limit_qos);

  ~ReaderBestEffortReliabilityQos() = default;

  bool Init(const std::string &uuid) override;

  ErrorCode CacheMessage(const std::string &topic_name, const spSerData &data,
                         const spMsg &msg,
                         const spMessageInfo &msg_info) override;

  std::shared_ptr<ReaderMessageCache> PopMessage() override;

 public:
  std::mutex reader_history_cache_mutex_;
  std::map<std::string, std::shared_ptr<ReaderHistoryCache>>
      reader_history_caches_;
  int cache_max_num_{0};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_READER_RELIABILITY_QOS_H_
