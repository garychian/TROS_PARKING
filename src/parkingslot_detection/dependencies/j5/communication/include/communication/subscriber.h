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

// This is subscriber interface definition.

#ifndef COMMUNICATION_SUBSCRIBER_H_
#define COMMUNICATION_SUBSCRIBER_H_

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "communication/common/tools_server.h"
#include "communication/common/types.h"
#ifdef SUPPORT_DDS
#include "communication/participant/dds/dds_reader.h"
#endif
#include "communication/participant/reader.h"
#include "communication/utils/cycle_queue.h"
// #include "communication/participant/reliable_reader.h"
#include "communication/wait_set/subscriber_base.h"
#include "message/message.h"
#include "communication/common/topic_manager.h"

using hobot::message::TypeWrapper;
namespace ExtendInfoKey = hobot::message::ExtendInfoKey;

namespace hobot {
namespace communication {

// declaration
template<typename Serializer>
class Subscriber : public SubscriberBase {
 public:
  using SpMessage = std::shared_ptr<typename Serializer::MsgType>;
  using HistoryMsgType = std::pair<SpMessage, std::string>;

  static std::shared_ptr<Subscriber<Serializer>>
  New(const CommAttr &comm_attr, const std::string &topic, int domain_id,
      const SubscriberCallback<Serializer> &sub_callback,
      ProtocolType protocol_type = PROTOCOL_HYBRID,
      ErrorCode *error_code = nullptr,
      const ConnEventCallback &event_callback = nullptr);

  static std::shared_ptr<Subscriber<Serializer>>
  NewExt(const CommAttr &comm_attr, const std::string &topic, int domain_id,
         const SubscriberWithTopicCallback<Serializer> &sub_callback,
         ProtocolType protocol_type = PROTOCOL_HYBRID,
         ErrorCode *error_code = nullptr,
         const ConnEventCallback &event_callback = nullptr);

  SpMessage Take(int timeout = -1, ErrorCode *error_code = nullptr);

  ~Subscriber();

 private:
  Subscriber(const CommAttr &comm_attr,
             const std::string &topic,
             int domain_id,
             const SubscriberWithTopicCallback<Serializer> &sub_callback,
             ProtocolType protocol_type);

  ErrorCode Init();

  void SetEventListener(const ConnEventCallback &event_callback);

  ErrorCode Destroy();

  uint64_t GetId();

 private:
  std::unique_ptr<Reader < Serializer>> reader_;
  uint64_t id_;
  int domain_id_;
  std::string topic_name_;
  uint64_t assemblier_topic_;
  ProtocolType proto_type_;
  std::string entity_uuid_;
  FixedSizeQueue<std::shared_ptr<HistoryMsgType>> history_queue_;
  std::atomic_bool rcv_switch_on_{true};
  std::string tools_service_name_;
  bool intra_direct_callback_{false};
  bool add_intra_callback_{false};
  ToolsServer* tool_server_{nullptr};
  DiscoveryManager* discovery_manager_{nullptr};
  SubRcvGlobalObj* sub_recv_global_{nullptr};
};

// definition
template <typename Serializer>
std::shared_ptr<Subscriber<Serializer>> Subscriber<Serializer>::New(
    const CommAttr &comm_attr, const std::string &topic, int domain_id,
    const SubscriberCallback<Serializer> &sub_callback,
    ProtocolType protocol_type, ErrorCode *error_code,
    const ConnEventCallback &event_callback) {
  if (!sub_callback) {
    return NewExt(comm_attr, topic, domain_id, nullptr,
                  protocol_type, error_code, event_callback);
  } else {
    auto sub_cb_with_time =
        [sub_callback, topic](
            const std::shared_ptr<typename Serializer::MsgType> &msg) {
          int64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
          COHLOGD("topic[{}] Sub message time = {}", topic, now);
          if (msg) {
            msg->SetExtInfo(ExtendInfoKey::kRecvTime, TypeWrapper(now));
          }
          sub_callback(msg);
        };

    auto callback_with_topic =
        [sub_cb_with_time](
            const std::shared_ptr<typename Serializer::MsgType> &msg,
            const std::string &) {
          sub_cb_with_time(msg);
        };
    auto sub_ptr = NewExt(comm_attr, topic, domain_id, callback_with_topic,
                  protocol_type, error_code, event_callback);

    if (!sub_ptr) {
      COHLOGE("New subscriber failed!");
      return nullptr;
    }
    bool add_intra_callback{false};
    if ((comm_attr.intra_direct_callback_ &&
         ProtocolChecker::IsIntra(protocol_type)) ||
        ProtocolChecker::IsHybrid(protocol_type)) {
      add_intra_callback = true;
    }
    if (ProtocolChecker::IsComposite(protocol_type) &&
        CompositeOptionChecker::HasProtocol(PROTOCOL_INTRA,
                                            comm_attr.composite_options)) {
      add_intra_callback = true;
    }
    if (add_intra_callback) {
      COHLOGD("begin add Callback !");
      auto sub_cb = [sub_cb_with_time](const spMsg &msg) {
        sub_cb_with_time(
            std::dynamic_pointer_cast<typename Serializer::MsgType>(msg));
      };
      std::string assemblier_topic_str = TopicAssemblier::AssembleTopic(topic,
                                     domain_id, protocol_type);
      uint64_t assemblier_topic{
        TopicManager::Instance()->GetTopicIndex(assemblier_topic_str)};
      hobot::communication::TopicManager::Instance()->AddCallback(
          assemblier_topic, sub_ptr->GetId(), sub_cb);
    }
    sub_ptr->add_intra_callback_ = add_intra_callback;
    return sub_ptr;
  }
}

template<typename Serializer>
std::shared_ptr<Subscriber<Serializer>>
Subscriber<Serializer>
::NewExt(const CommAttr &comm_attr,
         const std::string &topic,
         int domain_id,
         const SubscriberWithTopicCallback<Serializer> &sub_callback,
         ProtocolType protocol_type,
         ErrorCode *error_code,
         const ConnEventCallback &event_callback) {
  bool is_sub_cb_null{sub_callback == nullptr};
  auto ret = PreConstructSubCheck(comm_attr, topic, domain_id, protocol_type,
                                  is_sub_cb_null);
  if (ret != COMM_CODE_OK) {
    if (error_code != nullptr) {
      *error_code = ret;
    }
    return nullptr;
  }
  auto *raw_ptr = new Subscriber<Serializer>(
      comm_attr, topic, domain_id, sub_callback, protocol_type);
  auto ptr = std::shared_ptr<Subscriber<Serializer>>(raw_ptr);
  uint64_t sub_id{ptr->GetId()};
  auto conn_event_cb = [event_callback, sub_id](
      const std::shared_ptr<LinkInfo> participants,
      EventType event) {
    ToolsServer::Instance()->AddEvent(
        sub_id, event, TOPIC_TYPE::SUB);
    if (event_callback != nullptr) {
      event_callback(participants, event);
    }
  };
  ptr->SetEventListener(conn_event_cb);

  ret = ptr->Init();
  if (ret != COMM_CODE_OK) {
    COHLOGE("Subscriber initialize failed!");
  } else {
    COHLOGI("Subscriber initialized successfully!");
  }
  if (error_code != nullptr) {
    *error_code = ret;
  }
  return (ret == COMM_CODE_OK) ? ptr : nullptr;
}

template<typename Serializer>
Subscriber<Serializer>::Subscriber(
    const CommAttr &comm_attr, const std::string &topic, int domain_id,
    const SubscriberWithTopicCallback<Serializer> &sub_callback,
    ProtocolType protocol_type)
    :history_queue_(comm_attr.msg_history_size_for_sync_sub,
                    [this](bool state) { UpdateHasData(state); }) {
  COHLOGI("Create a new Subscriber, topic={}, protocol={}, domain_id={}",
                                     topic, protocol_type, domain_id);
  sub_recv_global_ = SubRcvGlobalObj::Instance();
  sub_recv_global_->RegisterSubRcvSwitch(this, &rcv_switch_on_);
  intra_direct_callback_ = comm_attr.intra_direct_callback_;
  if (history_queue_.GetSize() >= 0) {
    SetCacheOn(true);
    sub_recv_global_->RegisterSubHistroyQueue(this, &history_queue_);
    auto read_cb = [this](
        const std::shared_ptr<typename Serializer::MsgType> &msg,
        const spMessageInfo &, const std::string &topic) {
      if (rcv_switch_on_) {
        ToolsServer::Instance()->UpdateCount(id_,
                                             TOPIC_COUNT_TYPE::RECV);
        auto spHistoryMsg = std::make_shared<HistoryMsgType>(msg, topic);
        history_queue_.Push(spHistoryMsg);
      } else {
        COHLOGD("subscriber [topic={}] is already switched off.", topic)
      }
    };
  // if (comm_attr.qos_.qos_profile_.reliability_qos_policy.kind
  //         == ReliabilityQosPolicyKind::RELIABILITY_RELIABLE &&
  //     protocol_type != PROTOCOL_DDS) {
  //   COHLOGD("Subscriber qos is RELIABILITY_RELIABLE, create ReliableReader"
  //     " topic {} domain_id {} type {}", topic, domain_id, protocol_type);
  //   reader_.reset(new ReliableReader<Serializer>(comm_attr, topic, domain_id,
  //                                     protocol_type, read_cb));
  // } else {
  //   reader_.reset(new Reader<Serializer>(comm_attr, topic, domain_id,
  //                                      protocol_type, read_cb));
  // }
    if (protocol_type == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
      std::string new_topic{topic};
      if (IsRos2Compatible()) {
        new_topic = std::string("rt/") + topic;
      }
      COHLOGD("subscriber new_topic = {}", new_topic);
      reader_.reset(CreateDDSReader<Serializer>(comm_attr, new_topic, domain_id,
                                                protocol_type, read_cb));
#endif
    } else {
      reader_.reset(new Reader<Serializer>(comm_attr, topic, domain_id,
                                           protocol_type, read_cb));
    }

  } else {
    auto read_cb = [sub_callback, this](
        const std::shared_ptr<typename Serializer::MsgType> &msg,
        const spMessageInfo &, const std::string &topic) {
      if (rcv_switch_on_) {
        ToolsServer::Instance()->UpdateCount(id_,
                                             TOPIC_COUNT_TYPE::RECV);
        sub_callback(msg, topic);
      } else {
        COHLOGD("subscriber [topic={}] is already switched off.", topic)
      }
    };
    // if (comm_attr.qos_.qos_profile_.reliability_qos_policy.kind
    //         == ReliabilityQosPolicyKind::RELIABILITY_RELIABLE &&
    //     protocol_type != PROTOCOL_DDS) {
    //   COHLOGD("Subscriber qos is RELIABILITY_RELIABLE, create ReliableReader"
    //     " topic {} domain_id {} type {}", topic, domain_id, protocol_type);
    //   reader_.reset(new ReliableReader<Serializer>(comm_attr, topic,
    //                                      domain_id, protocol_type, read_cb));
    // } else {
    //   reader_.reset(new Reader<Serializer>(comm_attr, topic, domain_id,
    //                                      protocol_type, read_cb));
    // }
    if (protocol_type == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
      std::string new_topic{topic};
      if (IsRos2Compatible()) {
        new_topic = std::string("rt/") + topic;
      }
      COHLOGD("subscriber new_topic = {}", new_topic);
      reader_.reset(CreateDDSReader<Serializer>(comm_attr, new_topic, domain_id,
                                                protocol_type, read_cb));
#endif
    } else {
      reader_.reset(new Reader<Serializer>(comm_attr, topic, domain_id,
                                           protocol_type, read_cb));
    }
  }

  id_ = ToolsServer::Instance()->AddTopic(comm_attr, topic, domain_id,
                                          TOPIC_TYPE::SUB);
  COHLOGD("Subscriber uuid = {}", id_);
  topic_name_ = topic;
  proto_type_ = protocol_type;
  domain_id_ = domain_id;
  std::string assemblier_topic_str = TopicAssemblier::AssembleTopic(topic,
                                                     domain_id,
                                                    protocol_type);
  assemblier_topic_ =
    TopicManager::Instance()->GetTopicIndex(assemblier_topic_str);
  tools_service_name_ = ToolsServer::Instance()->GetToolsServiceName();
  if (tools_service_name_ != topic_name_) {
    tool_server_ = ToolsServer::Instance();
  }
}

template<typename Serializer>
Subscriber<Serializer>::~Subscriber() {
  COHLOGD("Destroy Subscriber, topic={}, protocol={}, id_={}",
                                 topic_name_, proto_type_, id_);
  Destroy();
  if (sub_recv_global_ != nullptr) {
    sub_recv_global_->UnRegisterSubRcvSwitch(this);
  }
  if (history_queue_.GetSize() >= 0) {
    if (sub_recv_global_ != nullptr) {
      sub_recv_global_->UnRegisterSubHistroyQueue(this);
    }
  }
  if (tools_service_name_ != topic_name_
      && tool_server_ != nullptr) {
    tool_server_->RemoveTopic(id_);
  }
  if (discovery_manager_ != nullptr
      && !entity_uuid_.empty()) {
    discovery_manager_->un_regist_entity(entity_uuid_);
  }
  if (add_intra_callback_) {
    TopicManager::Instance()->RemoveCallback(assemblier_topic_,  id_);
  }
}

template<typename Serializer>
ErrorCode Subscriber<Serializer>::Init() {
  // COHLOGD("Subscriber Init, topic={}, protocol={}, id_={}",
  //                                         topic_name_, proto_type_, id_);
  if (reader_ == nullptr) {
    return COMM_CODE_INIT_FAILED;
  }
  discovery_manager_ = DiscoveryManager::instance();
  entity_uuid_ = discovery_manager_->regist_entity(
      EntityType::ENTITY_SUB, topic_name_, domain_id_, proto_type_);
  return reader_->Init();
}

template<typename Serializer>
ErrorCode Subscriber<Serializer>::Destroy() {
  reader_.reset(nullptr);
  return COMM_CODE_OK;
}

template<typename Serializer>
void Subscriber<Serializer>::SetEventListener(
    const ConnEventCallback &event_callback) {
  COHLOGD("Subscriber SetEventListener, topic={}, protocol={}, id_",
                                          topic_name_, proto_type_, id_);

  if (event_callback != nullptr) {
    reader_->SetEventListener(event_callback);
  }
}

template<typename Serializer>
uint64_t Subscriber<Serializer>::GetId() {
  return id_;
}

template<typename Serializer>
typename Subscriber<Serializer>::SpMessage
Subscriber<Serializer>::Take(int timeout, ErrorCode *error_code) {
  if (history_queue_.GetSize() < 0) {
    if (error_code != nullptr) {
      *error_code = COMM_CODE_ILLEGAL_INVOCATION;
      COHLOGE("can not invoke Take when the history queue not be turnned on!")
      return {};
    }
  }
  if (error_code != nullptr) {
    *error_code = COMM_CODE_OK;
  }
  COHLOGD("queue size: {}", history_queue_.Size());
  return history_queue_.Pop(timeout)->first;
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SUBSCRIBER_H_
