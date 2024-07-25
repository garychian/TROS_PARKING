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

#ifndef COMMUNICATION_TRANSPORT_PRIORITY_QOS_H_
#define COMMUNICATION_TRANSPORT_PRIORITY_QOS_H_

#include <map>
#include <memory>
#include <string>

#include "communication/common/errors.h"
#include "communication/common/qos_policy.h"
#include "communication/common/types.h"
#include "communication/participant/participant.h"

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
class DLLEXPORT TransportPriorityQos {
 public:
  TransportPriorityQos(
      ProtocolType protocol_type,
      TransportPriorityQoSPolicyLevel transport_priority_level);
  ~TransportPriorityQos() = default;

  ErrorCode Init();

  void AddParticipantEntity(
      const std::string &id, std::shared_ptr<Participant> parti);
  void GetLinkLevelQosParameterConfig(
                            QosParameterConfig &rel_qos_param);
  void RemoveParticipantEntity(const std::string &id);

 private:
  TransportPriorityQoSPolicyLevel transport_priority_level_;
  ProtocolType protocol_type_;
  std::mutex parti_mutex_;
  std::map<std::string, std::shared_ptr<Participant>> participants_;
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_TRANSPORT_PRIORITY_QOS_H_
