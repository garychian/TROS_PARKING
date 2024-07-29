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

// This is writer base definition.

#ifndef COMMUNICATION_PARTICIPANT_WRITER_BASE_H_
#define COMMUNICATION_PARTICIPANT_WRITER_BASE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "message/message.h"

#include "communication/discovery/discovery_manager.h"
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

class DLLEXPORT WriterBase {
 public:
  WriterBase() = default;
  virtual ~WriterBase() { Destroy(); }

 protected:
  WriterBase(const CommAttr &comm_attr, const std::string &topic, int domain_id,
             ProtocolType type);

  ErrorCode BaseInit();

  void PartAddCallback(std::shared_ptr<Participant> part, std::string reader_id,
                       const QoSAttr &qos_attr);
  void PartRemoveCallback(int part_id, std::string reader_id);
  void FillSerializeMsg(const spMsg &message, const spCommSerData &ser_data);
  int AsyncWrite(std::shared_ptr<Participant> participant,
                 const std::string &topic_name, const spSerData &data,
                 const spMsg &msg, const spMessageInfo &msg_info);
  bool IsNeedSerialize(
      const std::vector<std::shared_ptr<Participant>> &participants);
  bool IsNeedSerialize(std::shared_ptr<Participant> participant);
  bool CheckReady(const std::vector<std::shared_ptr<Participant>> &participants,
                  const std::string &topic_name = "", bool need_all = false);
  bool CheckReadyForDynamic(std::string my_uuid, ProtocolType proto_type);
  bool SetQosPriority(std::vector<int> &parti_ids, int priority);

  bool CheckReady();

 private:
  void Destroy();

 protected:
  std::string uuid_;
  int domain_id_;
  std::string topic_name_;
  CommAttr comm_attr_;
  ProtocolType protocol_type_;
  bool is_dynamic_;
  bool is_hybrid_;
  ConnEventCallback event_listener_;
  std::vector<std::shared_ptr<Participant>> participants_;
  std::map<int, int> part_id_count;
  std::map<std::string, int> reader_id_to_part_id_;
  std::mutex part_mutex_;
  DiscoveryManager *discovery_manager_{nullptr};

  std::shared_ptr<QosAdaptor> qos_ptr_;
};
}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PARTICIPANT_WRITER_BASE_H_
