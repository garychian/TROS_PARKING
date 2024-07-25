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

#ifndef COMMUNICATION_QOS_ADAPTOR_H_
#define COMMUNICATION_QOS_ADAPTOR_H_

#include <string>
#include <memory>

#include "communication/common/errors.h"
#include "communication/common/qos_policy.h"
#include "communication/participant/dispatcher.h"
#include "communication/participant/participant.h"
#include "communication/qos/reliability_qos.h"
#include "communication/qos/transport_priority_qos.h"

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

class DLLEXPORT QosAdaptor {
 public:
  // query qos support
  static ErrorCode GetQosCapability(ProtocolType protocol_type,
                                    QosCapability &qos_capability,
                                    const CommAttr &comm_attr);

 public:
  // default constructor doesn't support any qos
  // QosAdaptor() = default;

  QosAdaptor(ProtocolType protocol_type, const QoSProfile qos_profile,
             int domain_id, const std::string &topic_name, bool is_writer_);

  ErrorCode Init(const std::string &uuid);

  ErrorCode CacheMessage(
      const std::string &topic_name, const spSerData &data, const spMsg &msg,
      const spMessageInfo &msg_info);
  // need_ack is only for reliable writer
  void AddParticipantEntity(const std::string &id,
                            std::shared_ptr<Participant> parti, bool need_ack);
  void GetLinkLevelQosParameterConfig(
                            QosParameterConfig &rel_qos_param);
  void RemoveParticipantEntity(const std::string &id);

  std::shared_ptr<ReaderMessageCache> PopMessage();

 private:
  ProtocolType protocol_type_;
  QoSProfile qos_profile_;
  // used by qos policy
  int domain_id_;
  const std::string topic_name_;
  std::string uuid_;
  bool is_writer_{false};

  bool is_inited_{false};
  std::unique_ptr<ReliabilityQos> reliability_qos_{nullptr};
  std::unique_ptr<TransportPriorityQos> transport_priority_qos_{nullptr};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_QOS_ADAPTOR_H_
