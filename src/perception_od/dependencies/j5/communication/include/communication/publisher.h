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

// This is publisher interface definition.

#ifndef COMMUNICATION_PUBLISHER_H_
#define COMMUNICATION_PUBLISHER_H_

#include <chrono>
#include <functional>
#include <memory>
#include <string>

// #include "communication/participant/reliable_writer.h"
#include "communication/common/tools_server.h"
#include "communication/common/types.h"
#ifdef SUPPORT_DDS
#include "communication/participant/dds/dds_writer.h"
#endif
#include "communication/participant/writer.h"
#include "message/message.h"
#include "communication/common/topic_manager.h"

using hobot::message::TypeWrapper;
namespace ExtendInfoKey = hobot::message::ExtendInfoKey;

namespace hobot {
namespace communication {

// declaration
template<typename Serializer>
class Publisher {
 public:
  static std::shared_ptr<Publisher<Serializer>> New(
      const CommAttr &comm_attr, const std::string &topic, int domain_id,
      ProtocolType type = PROTOCOL_HYBRID, ErrorCode *error_code = nullptr,
      const ConnEventCallback &event_callback = nullptr);

  ErrorCode Pub(const std::shared_ptr<typename Serializer::MsgType> &message);

  ~Publisher();

 private:
  Publisher(const CommAttr &comm_attr, const std::string &topic, int domain_id,
            ProtocolType type);

  ErrorCode Init();

  void SetEventListener(const ConnEventCallback &event_callback);

  ErrorCode Destroy();

  uint64_t GetId();

 private:
  std::unique_ptr<Writer < Serializer>> writer_;
  uint64_t id_;
  int domain_id_;
  bool intra_direct_callback_{false};
  bool is_composite_intra_only_{false};
  ProtocolType proto_type_;
  std::string entity_uuid_;
  std::string tools_service_name_;
  std::string topic_name_;
  uint64_t assemblier_topic_;
  ToolsServer* tool_server_{nullptr};
  DiscoveryManager* discovery_manager_{nullptr};
};

// definition
template<typename Serializer>
Publisher<Serializer>::Publisher(const CommAttr &comm_attr,
                                 const std::string &topic, int domain_id,
                                 ProtocolType type) {
  COHLOGI("Create a new publisher, topic={}, protocol={}, domain_id={}", topic,
          type, domain_id);
  // if (comm_attr.qos_.qos_profile_.reliability_qos_policy.kind
  //           == ReliabilityQosPolicyKind::RELIABILITY_RELIABLE &&
  //     type != PROTOCOL_DDS) {
  //   COHLOGD("Publisher qos is RELIABILITY_RELIABLE, create ReliableWriter"
  //           " topic {} domain_id {} type {}", topic, domain_id, type);
  //   writer_.reset(
  //       new ReliableWriter<Serializer>(comm_attr, topic, domain_id, type));
  // } else {
  //   writer_.reset(
  //       new Writer<Serializer>(comm_attr, topic, domain_id, type));
  // }
  if (type == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    std::string new_topic(topic);
    if (IsRos2Compatible()) {
      new_topic = std::string("rt/") + topic;
    }
    COHLOGD("publisher new_topic = {}", new_topic);
    writer_.reset(
        CreateDDSWriter<Serializer>(comm_attr, new_topic, domain_id, type));
#endif
  } else {
    writer_.reset(
        new Writer<Serializer>(comm_attr, topic, domain_id, type));
  }

  id_ = ToolsServer::Instance()->AddTopic(comm_attr, topic, domain_id,
                                          TOPIC_TYPE::PUB);
  COHLOGD("Publisher uuid = {}", id_);
  tools_service_name_ = ToolsServer::Instance()->GetToolsServiceName();
  topic_name_ = topic;
  proto_type_ = type;
  domain_id_ = domain_id;
  std::string assemblier_topic_str =
    TopicAssemblier::AssembleTopic(topic, domain_id, type);
  assemblier_topic_ =
    TopicManager::Instance()->GetTopicIndex(assemblier_topic_str);
  intra_direct_callback_ = comm_attr.intra_direct_callback_;
  if (tools_service_name_ != topic_name_) {
    tool_server_ = ToolsServer::Instance();
  }
}

template<typename Serializer>
Publisher<Serializer>::~Publisher() {
  COHLOGD("Destroy publisher, topic={}, protocol={}, id={}",
                              topic_name_, proto_type_, id_);
  Destroy();
  if (tools_service_name_ != topic_name_) {
    if (tool_server_ != nullptr) {
      tool_server_->RemoveTopic(id_);
    }
  }
  if (discovery_manager_ != nullptr && !entity_uuid_.empty()) {
    discovery_manager_->un_regist_entity(entity_uuid_);
  }
}

template<typename Serializer>
ErrorCode Publisher<Serializer>::Init() {
  // COHLOGD("Publisher Init, topic={}, protocol={}, id_={}",
  //                                       topic_name_, proto_type_, id_);
  if (writer_ == nullptr) {
    return COMM_CODE_INIT_FAILED;
  }
  if (intra_direct_callback_ &&
      (ProtocolChecker::IsIntra(proto_type_) || is_composite_intra_only_)) {
    COHLOGD("is_composite_intra_only_ is {}", is_composite_intra_only_);
    return COMM_CODE_OK;
  }
  discovery_manager_ = DiscoveryManager::instance();
  entity_uuid_ = discovery_manager_->regist_entity(
      EntityType::ENTITY_PUB, topic_name_, domain_id_, proto_type_);
  return writer_->Init();
}

template<typename Serializer>
ErrorCode Publisher<Serializer>::Destroy() {
  writer_.reset(nullptr);
  return COMM_CODE_OK;
}

template<typename Serializer>
std::shared_ptr<Publisher<Serializer>>
Publisher<Serializer>::New(const CommAttr &comm_attr, const std::string &topic,
                           int domain_id, ProtocolType type,
                           ErrorCode *error_code,
                           const ConnEventCallback &event_callback) {
  ErrorCode ret = PreConstructPubCheck(comm_attr, topic, domain_id, type);
  if (ret != COMM_CODE_OK) {
    if (error_code) *error_code = ret;
    return nullptr;
  }
  auto *raw_ptr =
      new Publisher<Serializer>(comm_attr, topic, domain_id, type);
  auto ptr = std::shared_ptr<Publisher<Serializer>>(raw_ptr);

  if (ProtocolChecker::IsComposite(type)) {
    ptr->is_composite_intra_only_ = CompositeOptionChecker::CheckProtocolOnly(
        PROTOCOL_INTRA, comm_attr.composite_options);
  }
  uint64_t pub_id = ptr->GetId();
  auto conn_event_cb = [event_callback, pub_id](
      const std::shared_ptr<LinkInfo> participants,
      EventType event) {
    ToolsServer::Instance()->AddEvent(
        pub_id, event, TOPIC_TYPE::PUB);
    if (event_callback != nullptr) {
      event_callback(participants, event);
    }
  };
  ptr->SetEventListener(conn_event_cb);
  ret = ptr->Init();
  if (ret != COMM_CODE_OK) {
    COHLOGE("Publisher initialize failed!")
  } else {
    COHLOGI("Publisher initialized successfully!,topic:{}", topic);
  }
  if (error_code) {
    *error_code = ret;
  }
  return (ret == COMM_CODE_OK) ? ptr : nullptr;
}

template<typename Serializer>
ErrorCode Publisher<Serializer>::Pub(
    const std::shared_ptr<typename Serializer::MsgType> &message) {
  if (!message) {
    COHLOGE("Write message is NULL!");
    return COMM_CODE_MESSAGE_NULL;
  }
  uint64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
  COHLOGD("Topic[{}]Pub message time = {}", topic_name_, now);
  message->SetExtInfo(ExtendInfoKey::kSendTime, TypeWrapper(now));
  if (intra_direct_callback_ &&
      ((ProtocolChecker::IsIntra(proto_type_) || is_composite_intra_only_))) {
    TopicManager::Instance()->Callback(assemblier_topic_, message);
    return COMM_CODE_OK;
  }
  if (tools_service_name_ != topic_name_) {
    if (ToolsServer::Instance()->GetTopicStatus(id_) ==
        TopicStatus::TOPIC_OPEN) {
      ToolsServer::Instance()->UpdateCount(id_,
                                           TOPIC_COUNT_TYPE::SEND);
      return writer_->Write(message);
    }
    return ErrorCode::COMM_CODE_TOPIC_CLOSE;
  }
  return writer_->Write(message);
}

template<typename Serializer>
void Publisher<Serializer>::SetEventListener(
    const ConnEventCallback &event_callback) {
  if (event_callback != nullptr && writer_) {
    COHLOGD("Publisher SetEventListener, topic={}, protocol={}, id_={}",
                                         topic_name_, proto_type_, id_);
    writer_->SetEventListener(event_callback);
  }
}

template<typename Serializer>
uint64_t Publisher<Serializer>::GetId() {
  return id_;
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SERVICE_H_
