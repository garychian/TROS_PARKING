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

// This is reader base definition.

#ifndef COMMUNICATION_PARTICIPANT_READER_BASE_H_
#define COMMUNICATION_PARTICIPANT_READER_BASE_H_
#if defined(_MSC_VER)
#pragma warning(disable: 4251)
#endif
#include <memory>
#include <vector>
#include <atomic>
#include <string>

#include "message/message.h"
#include "communication/common/types.h"

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif
// class ThreadPoolAT;
namespace hobot {
namespace communication {

// class Dispatcher;
class Participant;
class Executor;
class ParticipantManager;
class DiscoveryManager;
class QosAdaptor;

class DLLEXPORT ReaderBase {
 public:
  friend class Dispatcher;
  ReaderBase() = default;
  ReaderBase(const CommAttr &comm_attr, const std::string &topic, int domain_id,
             ProtocolType type);
  virtual ~ReaderBase() = default;

  std::shared_ptr<Executor> GetExecutor() {
    return executor_;
  }
  std::string GetTopicName() {
    return topic_name_;
  }
 protected:
  ErrorCode Init(const std::function<
                     void(const std::vector<std::shared_ptr<Participant>> &)> &,
                 const std::function<void()>&,
                 ErrorCode &);
  bool SetExecutor(std::shared_ptr<Executor> executor);
  bool CheckReady(const std::vector<std::shared_ptr<Participant>> &participants,
                  const std::string &topic_name = "", bool need_all = false);
  bool CheckReadyForDynamic(std::string my_uuid, ProtocolType proto_type);
  std::string UuidStr();

  void HookBeforeCallback(std::shared_ptr<hobot::message::Message> msg,
                          ProtocolType protocol_type);
  void HookAfterCallback(std::shared_ptr<hobot::message::Message> msg,
                          ProtocolType protocol_type);

 protected:
  void Destroy();
  bool CheckReady();

 protected:
  std::string uuid_;
  int domain_id_;
  std::string topic_name_;
  CommAttr comm_attr_;
  ProtocolType protocol_type_;
  bool is_dynamic_;
  bool is_hybrid_;
  // MessageListenerWithTopic msg_listener_;
  ConnEventCallback event_listener_;
  std::vector<std::shared_ptr<Participant>> participants_;
  ParticipantManager *participant_manager_{nullptr};
  DiscoveryManager *discovery_manager_{nullptr};

  std::shared_ptr<QosAdaptor> qos_ptr_;
  // for reliability qos
  std::mutex cv_mtx_;
  std::condition_variable cond_var_;
  std::thread worker_thread_;
  std::atomic<bool> stop_{false};
  std::atomic_int *ref_task_count_{nullptr};

 private:
  std::shared_ptr<Executor> executor_;
};
}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PARTICIPANT_READER_BASE_H_
