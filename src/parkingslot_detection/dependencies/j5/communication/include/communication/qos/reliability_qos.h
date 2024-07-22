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

#ifndef COMMUNICATION_RELIABILITY_QOS_INTERFACE_H_
#define COMMUNICATION_RELIABILITY_QOS_INTERFACE_H_

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "communication/common/errors.h"
#include "communication/common/qos_policy.h"
#include "communication/participant/dispatcher.h"
#include "communication/participant/participant.h"
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
struct ReaderMessageCache;
class DLLEXPORT ReliabilityQos {
 public:
  ReliabilityQos(const HistoryQoSPolicy &history_qos,
                 const ResourceLimitsQosPolicy &resource_limit_qos);

  ReliabilityQos(ProtocolType protocol_type, int domain_id,
                 const std::string &topic_name,
                 const HistoryQoSPolicy &history_qos,
                 const ResourceLimitsQosPolicy &resource_limit_qos);

  virtual ~ReliabilityQos() = default;

  virtual bool Init(const std::string &uuid) = 0;

  virtual ErrorCode CacheMessage(const std::string &topic_name,
                                 const spSerData &data, const spMsg &msg,
                                 const spMessageInfo &msg_info) = 0;

  virtual std::shared_ptr<ReaderMessageCache> PopMessage() {
    return nullptr;
  }

  virtual void AddParticipantEntity(const std::string &reader_id,
                            std::shared_ptr<Participant> parti, bool need_ack);
  void GetLinkLevelQosParameterConfig(
                            QosParameterConfig &rel_qos_param);
  void RemoveParticipantEntity(const std::string &reader_id);

 protected:
  // used by reliable reader/writer to create ack reader/writer
  ProtocolType protocol_type_{-1};
  int domain_id_{0};
  const std::string topic_name_;
  std::string uuid_;

  // for read and write data
  std::mutex parti_mutex_;
  std::map<std::string, bool> reader_need_ack_;
  std::map<std::string, std::shared_ptr<Participant>> participants_;

  HistoryQoSPolicy history_qos_;
  ResourceLimitsQosPolicy resource_limit_qos_;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_RELIABILITY_QOS_INTERFACE_H_
