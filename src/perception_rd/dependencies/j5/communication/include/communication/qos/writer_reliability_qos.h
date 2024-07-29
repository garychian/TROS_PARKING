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

#ifndef COMMUNICATION_WRITER_RELIABILITY_QOS_H_
#define COMMUNICATION_WRITER_RELIABILITY_QOS_H_

#include <vector>
#include <string>
#include <memory>

#include "communication/common/errors.h"
#include "communication/utils/reliable_timer.h"
#include "communication/common/types.h"
#include "communication/qos/reliability_qos.h"
#include "communication/qos/writer_history_cache.h"

#define RESEND_INTERVAL_MS 100

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

class ReliableWriterImpl;

class DLLEXPORT WriterReliableReliabilityQos : public ReliabilityQos {
 public:
  WriterReliableReliabilityQos(
      ProtocolType protocol_type, int domain_id, const std::string &topic_name,
      const HistoryQoSPolicy &history_qos,
      const ResourceLimitsQosPolicy &resource_limit_qos,
      int32_t max_blocking_time);

  ~WriterReliableReliabilityQos();

  bool Init(const std::string &uuid) override;

  ErrorCode CacheMessage(const std::string &topic_name, const spSerData &data,
                         const spMsg &msg,
                         const spMessageInfo &msg_info) override;

  void Start();

  void Stop();

 private:
  void OnAckCallback(const std::string &writer_id, const std::string &reader_id,
                     const std::vector<int64_t> &acked_seq_ids,
                     const std::vector<int64_t> &no_acked_seq_ids);

  void OnTimerIntervalCallback();

 private:
  int32_t max_blocking_time_ = -1;  // -1 means block util buffer is writeable
  std::atomic<uint64_t> current_seq_id_{0u};

  std::shared_ptr<ReliableWriterImpl> reliable_writer_impl_{nullptr};

  std::shared_ptr<WriterHistoryCache> writer_history_caches_;
  std::mutex writer_history_cache_mutex_;

  std::shared_ptr<ReliableTimer> reliable_timer_;

  std::condition_variable worker_cv_;
  std::mutex worker_cv_mutex_;
  std::thread worker_;
  std::atomic_bool stop_{false};
};

// do nothing, just encode a sequence number;
class DLLEXPORT WriterBestEfforReliabilityQos : public ReliabilityQos {
 public:
  WriterBestEfforReliabilityQos(
      ProtocolType protocol_type, int domain_id, const std::string &topic_name,
      const HistoryQoSPolicy &history_qos,
      const ResourceLimitsQosPolicy &resource_limit_qos);

  ~WriterBestEfforReliabilityQos();

  bool Init(const std::string &uuid) override;
  // just encode a sequence number into every message
  ErrorCode CacheMessage(const std::string &topic_name, const spSerData &data,
                         const spMsg &msg,
                         const spMessageInfo &msg_info) override;

  void Start();

  void Stop();

 private:
  std::atomic<uint64_t> current_seq_id_{0u};
  std::mutex writer_history_cache_mutex_;
  std::shared_ptr<WriterHistoryCache> writer_history_caches_;

  std::condition_variable worker_cv_;
  std::mutex worker_cv_mutex_;
  std::thread worker_;
  std::atomic_bool stop_{false};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_WRITER_RELIABILITY_QOS_H_
