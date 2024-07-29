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

// This is reader definition.

#ifndef COMMUNICATION_PARTICIPANT_READER_H_
#define COMMUNICATION_PARTICIPANT_READER_H_

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "communication/qos/qos_adaptor.h"

#include "communication/common/comm_log.h"
#include "communication/discovery/discovery_manager.h"
#include "communication/participant/participant.h"
#include "communication/participant/reader_base.h"
#include "communication/qos/reader_history_cache.h"
#include "message/message.h"

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

template <typename Serializer>
class DLLEXPORT Reader : public ReaderBase {
 public:
  using MessageListener =
  std::function<void(const std::shared_ptr<typename Serializer::MsgType> &,
                     const spMessageInfo &)>;

  using MessageListenerWithTopic =
  std::function<void(const std::shared_ptr<typename Serializer::MsgType> &,
                     const spMessageInfo &, const std::string &)>;

  DISALLOW_COPY_AND_ASSIGNMENT(Reader);

  Reader(const CommAttr &comm_attr, const std::string &topic, int domain_id,
         const MessageListener &cb, ProtocolType type);
  Reader(const CommAttr &comm_attr, const std::string &topic, int domain_id,
         ProtocolType type, const MessageListenerWithTopic &cb);
  virtual ErrorCode Init();
  virtual void SetEventListener(ConnEventCallback cb);
  virtual void OnMessage(const std::shared_ptr<typename Serializer::MsgType> &,
                         const spMessageInfo &, const std::string &, bool);
  virtual bool IsReady();
  virtual void ProcessMsgThread();
  virtual ~Reader();

 protected:
  // return: false-drop msg
  inline bool FilterMsgForHybrid(
                  const std::shared_ptr<typename Serializer::MsgType> &msg) {
    if (is_hybrid_) {
      // drop the message that matched the followed rule:
      // 1. same process 2. writer is hybrid 3. shm channel
      auto commSerData
        = std::dynamic_pointer_cast<CommonSerializedData>(msg->GetData());
      if (commSerData != nullptr &&
                  commSerData->GetFromProto() == PROTOCOL_SHM) {
        std::string writer_id{commSerData->GetWriterId()};
        if (discovery_manager_->writer_is_hybrid_in_my_endpoint(writer_id)) {
          // drop
          return false;
        }
      }
    }
    return true;
  }

 protected:
  MessageListenerWithTopic msg_listener_;
};

template<typename Serializer>
Reader<Serializer>::Reader(const CommAttr &comm_attr, const std::string &topic,
                           int domain_id, const MessageListener &cb,
                           ProtocolType type)
    : Reader(comm_attr, topic, domain_id, type, nullptr) {
  auto cb_with_topic =
      [cb](const std::shared_ptr<typename Serializer::MsgType> &msg,
           const spMessageInfo &info, const std::string &) {
        cb(msg, info);
      };
  msg_listener_ = cb_with_topic;
}

template<typename Serializer>
Reader<Serializer>::Reader(const CommAttr &comm_attr, const std::string &topic,
                           int domain_id, ProtocolType type,
                           const MessageListenerWithTopic &cb)
    : ReaderBase(comm_attr, topic, domain_id, type) {
  auto cb_recover_topic =
      [cb, this](const std::shared_ptr<typename Serializer::MsgType> &msg,
                 const spMessageInfo &info, const std::string &topic) {
        std::string user_topic{};
        if (this->domain_id_ == DUMB_DOMAIN_ID ||
            this->protocol_type_ == PROTOCOL_DDS) {
          user_topic = topic;
        } else {
          int unused_id{0};
          TopicAssemblier::DisassembleTopic(topic, &user_topic, &unused_id);
        }
        HookBeforeCallback(msg, protocol_type_);
        cb(msg, info, user_topic);
        HookAfterCallback(msg, protocol_type_);
      };
  msg_listener_ = cb_recover_topic;
}

template<typename Serializer>
ErrorCode Reader<Serializer>::Init() {
  ErrorCode code{COMM_CODE_OK};
  auto conn_change_callback = [this, &code](
                const std::vector<std::shared_ptr<Participant>>& partis) {
    ref_task_count_ = new std::atomic_int[partis.size()]();
    for (auto part : partis) {
      part->AddReaderEventListener(event_listener_, this);
      participants_.push_back(part);
      if (qos_ptr_) {
        qos_ptr_->AddParticipantEntity(this->uuid_, part, false);
      }
    }
    int parti_pos{0};
    for (auto &elem : participants_) {
      bool is_intra{elem->GetProtocolType() == PROTOCOL_INTRA};
      auto callback =
          [this, is_intra](const spMsg &msg, const spMessageInfo &msg_info,
                           const std::string &topic) {
            auto conver_msg =
              std::dynamic_pointer_cast<typename Serializer::MsgType>(msg);
            if (!conver_msg) {
              COHLOGE("Topic:{} convert base msg to derived msg failed!"
                      " Please make sure that the sender and"
                      " receiver data types are the same", topic);
            }
            OnMessage(conver_msg, msg_info, topic, is_intra);
          };
      // int handleId;
      if (elem->GetDispatcher()) {
        code = elem->GetDispatcher()->template AddListener<Serializer>(
            topic_name_, callback, this, parti_pos++);
        if (code != COMM_CODE_OK) {
          break;
        }
        elem->AddListenTopicToReader(topic_name_);
        // listener_handler_ids_.push_back(handleId);
        if (elem->IsCheckE2Eable() && (comm_attr_.e2e_callback != nullptr)) {
          elem->AddE2EventCallback(topic_name_, comm_attr_.e2e_callback);
        }
      }
    }
      };
  auto process_msg_thread = [this]() { ProcessMsgThread(); };
  return ReaderBase::Init(conn_change_callback, process_msg_thread, code);
}

template<typename Serializer>
Reader<Serializer>::~Reader() {
  Destroy();
}

template<typename Serializer>
void Reader<Serializer>::SetEventListener(ConnEventCallback cb) {
  event_listener_ = cb;
}

template<typename Serializer>
void Reader<Serializer>::OnMessage(
    const std::shared_ptr<typename Serializer::MsgType> &msg,
    const spMessageInfo &msg_info, const std::string &topic, bool is_intra) {
  if (!FilterMsgForHybrid(msg)) {
    return;
  }
  // if support reliable qos
  if (comm_attr_.qos_.qos_profile_.reliability_qos_policy.kind !=
          RELIABILITY_DISABLED &&
      qos_ptr_) {
    if (is_intra) {
      msg_listener_(msg, msg_info, topic);
      return;
    }
    std::lock_guard<std::mutex> lk{cv_mtx_};
    qos_ptr_->CacheMessage(topic, nullptr, msg, msg_info);
    cond_var_.notify_one();
  } else {
    // reliability_qos_disabled
    msg_listener_(msg, msg_info, topic);
  }
}

template <typename Serializer>
void Reader<Serializer>::ProcessMsgThread() {
  while (!stop_.load()) {
    std::shared_ptr<ReaderMessageCache> msg_cache{nullptr};
    std::unique_lock<std::mutex> lk{cv_mtx_};
    cond_var_.wait(lk, [this, &msg_cache]() {
      return stop_.load() || (msg_cache = qos_ptr_->PopMessage());
    });
    if (stop_.load()) {
      break;
    }
    lk.unlock();
    if (msg_cache) {
      auto msg = std::dynamic_pointer_cast<typename Serializer::MsgType>(
          msg_cache->msg);
      msg_listener_(msg, msg_cache->msg_info, msg_cache->topic_name);
    }
  }
}

template <typename Serializer>
bool Reader<Serializer>::IsReady() {
  return CheckReady();
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PARTICIPANT_READER_H_
