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

// This is dispatcher definition.

#ifndef COMMUNICATION_PARTICIPANT_DISPATCHER_H_
#define COMMUNICATION_PARTICIPANT_DISPATCHER_H_

#include <map>
#include <tuple>
#include <utility>
#include <memory>
#include <string>
#include <future>
#include <list>
#include <vector>

#include <iostream>
#include "communication/common/comm_const.h"
#include "communication/common/comm_log.h"
#include "communication/common/config_op.h"
#include "communication/common/types.h"
#include "communication/participant/reader_base.h"
#include "communication/service/message_info.h"
#include "communication/utils/cycle_queue.h"

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
namespace util {
int DLLEXPORT DispatcherDeserialize(
    const std::shared_ptr<hobot::message::TypeSupport> &serialzier,
    const spMsg &msg, const spSerData &ser_data, const std::string &msg_topic);
int DLLEXPORT DispatcherDeserialize(
    const std::shared_ptr<SerializerPolicy> &serialzier,
    const spMsg &msg, const spSerData &ser_data, const std::string &msg_topic);
}  // namespace util
class SerHoldBase {
 public:
  SerHoldBase() = default;
  virtual ~SerHoldBase() = default;
};

template<typename Serializer>
class SerHoldDerived : public SerHoldBase {
 public:
  explicit SerHoldDerived(std::shared_ptr<Serializer> ser_ptr)
      : SerHoldBase{}, hold_ptr_{ser_ptr} {}
  ~SerHoldDerived() = default;
 private:
  std::shared_ptr<Serializer> hold_ptr_;
};

class DLLEXPORT Dispatcher {
 public:
  struct DLLEXPORT ExtraInfo {
    explicit ExtraInfo(bool skip_flag = false)
        : skip_not_found_related_topic_error{skip_flag} {}
    bool skip_not_found_related_topic_error;
  };

  using MessageListener =
  std::function<void(const spMsg &,
                     const spMessageInfo &, const std::string &topic)>;

  using UnionMessageListener =
  std::function<void(const spMsg &, const spMessageInfo &,
                     const std::string &topic)>;

  using DesCallBack =
  std::function<void(const std::string &,
                     const spSerData &,
                     const spMessageInfo &)>;

  using DesCallBackForPrefixMatch =
  std::function<void(const std::string &,
                     const std::string &,
                     const spSerData &,
                     const spMessageInfo &)>;

  explicit Dispatcher(ProtocolType type, bool IsNeedSerialize = true)
      : type_{type},
        listeners_exact_match_{},
        deserialize_callbacks_exact_match_{},
        deserialize_callbacks_exact_match_dedicated_{},
        listeners_prefix_match_{},
        deserialize_callbacks_prefix_match_{},
        deserialize_callbacks_prefix_match_dedicated_{},
        reader_pos_{},
        IsNeedSerialize_{IsNeedSerialize},
        executor_{nullptr},
        mutex_{},
        des_cb_mutex_{},
        create_mutex1_{},
        create_mutex2_{},
        exact_ser_hold_{nullptr},
        prefix_ser_hold_{nullptr} {}

  ~Dispatcher() = default;

  // return handleId to distinguish listener
  template <typename Serializer>
  ErrorCode AddListener(const std::string &topic,
                        const MessageListener &listener,
                        ReaderBase *reader_ptr,
                        int pos);
  int RemoveListener(const std::string &topic, ReaderBase *reader_ptr);

  void OnMessage(const std::string &topic_name,
                 const spSerData &data,
                 const spMsg &msg,
                 const spMessageInfo &msg_info,
                 const std::shared_ptr<ExtraInfo> &extra_info = nullptr);

 private:
  using DesCallBackHelper = int (Dispatcher::*)(const std::string &,
                                                const spSerData &,
                                                const spMessageInfo &, bool);
  using DesCallBackForPrefixMatchHelper =
      int (Dispatcher::*)(const std::string &, const std::string &,
                          const spSerData &, const spMessageInfo &, bool);

  ErrorCode AddListenerHelper(
      const std::string &topic, const MessageListener &listener,
      ReaderBase *reader_ptr, DesCallBackHelper exact_des_cb = nullptr,
      DesCallBackForPrefixMatchHelper prefix_des_cb = nullptr,
      int pos = 0);

  template<typename Serializer>
  int DeserializeCallbackForExactMatch(const std::string &topic,
                                       const spSerData &ser_data,
                                       const spMessageInfo &msg_info,
                                       bool is_dedicated = false);

  int CallListenersForExactMatch(const std::string &topic,
                                 const spMsg &msg,
                                 const spMessageInfo &msg_info,
                                 bool is_dedicated = false);

  template<typename Serializer>
  int DeserializeCallbackForPrefixMatch(const std::string &msg_topic,
                                        const std::string &matched_topic,
                                        const spSerData &ser_data,
                                        const spMessageInfo &msg_info,
                                        bool is_dedicated = false);

  int CallListenersForPrefixMatch(const std::string &msg_topic,
                                  const std::string &matched_topic,
                                  const spMsg &msg,
                                  const spMessageInfo &msg_info,
                                  bool is_dedicated = false);

  int ExactMatchDispatch(const std::string &topic_name,
                         const spSerData &data,
                         const spMsg &msg,
                         const spMessageInfo &msg_info);

  int PrefixMatchDispatch(const std::string &topic_name,
                          const spSerData &data,
                          const spMsg &msg,
                          const spMessageInfo &msg_info);

  bool IsWildcardTopic(const std::string &);

  bool IsPrefixStr(const std::string &main, const std::string &prefix);

  template<typename Serializer, typename U>
  typename std::enable_if<HasBole2Flag<U>::value,
                          int>::type
  SetBoleMsgSubIdlType(ProtocolType type,
                       std::shared_ptr<Serializer> bole_type_ptr);

  template<typename Serializer, typename U>
  typename std::enable_if<!HasBole2Flag<U>::value,
                          int>::type
  SetBoleMsgSubIdlType(ProtocolType type,
                       std::shared_ptr<Serializer> bole_type_ptr);

 private:
  ProtocolType type_;
  std::map<
      std::string,
      std::list<std::pair<UnionMessageListener, ReaderBase *>>>
      listeners_exact_match_;

  std::map<std::string, DesCallBack> deserialize_callbacks_exact_match_;

  std::map<std::string, std::pair<DesCallBack, std::shared_ptr<Executor>>>
      deserialize_callbacks_exact_match_dedicated_;

  std::map<
      std::string,
      std::list<std::pair<UnionMessageListener, ReaderBase *>>>
      listeners_prefix_match_;

  std::map<std::string,
           DesCallBackForPrefixMatch> deserialize_callbacks_prefix_match_;
  std::map<std::string,
           std::pair<DesCallBackForPrefixMatch, std::shared_ptr<Executor>>>
      deserialize_callbacks_prefix_match_dedicated_;

  std::map<ReaderBase*, int> reader_pos_;

  bool IsNeedSerialize_;
  std::shared_ptr<Executor> executor_;
  std::mutex mutex_;
  std::mutex des_cb_mutex_;
  std::mutex create_mutex1_;
  std::mutex create_mutex2_;
  std::shared_ptr<SerHoldBase> exact_ser_hold_;
  std::shared_ptr<SerHoldBase> prefix_ser_hold_;
};

template<typename Serializer, typename U>
  typename std::enable_if<HasBole2Flag<U>::value,
                          int>::type
  Dispatcher::SetBoleMsgSubIdlType(ProtocolType type,
                        std::shared_ptr<Serializer> bole_type_ptr) {
  auto sub_idl_type {CommConfigOperate::GetSubidlTypeByProtocolType(type)};
  // COHLOGD("Sub idl type is {}", sub_idl_type);
  if (sub_idl_type.size() == kUIntZero) {
    COHLOGW("Not get valid sub idl type for protocol {}, "
            "will use default sub idl type", type);
    return kZero;
  }

  COHLOGD("Bole2 TypeSupport, set sub idl type to {}", sub_idl_type);
  return bole_type_ptr->SetSubIdlType(sub_idl_type);
}

template <typename Serializer, typename U>
typename std::enable_if<!HasBole2Flag<U>::value, int>::type
Dispatcher::SetBoleMsgSubIdlType(ProtocolType, std::shared_ptr<Serializer>) {
  COHLOGD("Not Bole2 TypeSupport, do nothing");
  return kZero;
}

template <typename Serializer>
ErrorCode Dispatcher::AddListener(const std::string &input_topic,
                                  const MessageListener &listener,
                                  ReaderBase *reader_ptr,
                                  int pos) {
  auto exact_des_cb = &Dispatcher::DeserializeCallbackForExactMatch<Serializer>;
  auto prefix_des_cb =
      &Dispatcher::DeserializeCallbackForPrefixMatch<Serializer>;
  return AddListenerHelper(input_topic, listener, reader_ptr, exact_des_cb,
                           prefix_des_cb, pos);
}

template<typename Serializer>
int Dispatcher::
DeserializeCallbackForExactMatch(const std::string &topic,
                                 const spSerData &ser_data,
                                 const spMessageInfo &msg_info,
                                 bool is_dedicated) {
  // Deserialize
  COHLOGD("Start to Deserialize, topic={}", topic);
  auto msg = std::make_shared<typename Serializer::MsgType>();

  static std::shared_ptr<Serializer> ser_ptr{nullptr};
  static bool b_has_created{false};
  if (!b_has_created) {
    std::lock_guard<std::mutex> lock_{create_mutex1_};
    if (!b_has_created) {
      ser_ptr = std::make_shared<Serializer>();
      exact_ser_hold_ = std::make_shared<SerHoldDerived<Serializer>>(ser_ptr);
      int ret{0};
      ret = SetBoleMsgSubIdlType<Serializer, typename Serializer::MsgType>(
          type_, ser_ptr);
      if (ret < kZero) {
        COHLOGW("Fail to set sub idl type, topic={} ", topic);
      }
      b_has_created = true;
    }
  }
  auto ret = util::DispatcherDeserialize(ser_ptr, msg, ser_data, topic);
  if (ret < kZero) {
    return ret;
  }
  COHLOGD("Deserialize done CallListenersForExactMatch");
  return CallListenersForExactMatch(topic, msg, msg_info, is_dedicated);
}

template<typename Serializer>
int Dispatcher::
DeserializeCallbackForPrefixMatch(const std::string &msg_topic,
                                  const std::string &matched_topic,
                                  const spSerData &ser_data,
                                  const spMessageInfo &msg_info,
                                  bool is_dedicated) {
  // Deserialize
  COHLOGD("Start to Deserialize, topic={}", msg_topic);
  auto msg = std::make_shared<typename Serializer::MsgType>();

  static std::shared_ptr<Serializer> ser_ptr{nullptr};
  static bool b_has_created{false};
  if (!b_has_created) {
    std::lock_guard<std::mutex> lock_{create_mutex2_};
    if (!b_has_created) {
      ser_ptr = std::make_shared<Serializer>();
      prefix_ser_hold_ = std::make_shared<SerHoldDerived<Serializer>>(ser_ptr);
      int ret{0};
      ret = SetBoleMsgSubIdlType<Serializer, typename Serializer::MsgType>(
          type_, ser_ptr);
      if (ret < kZero) {
        COHLOGW("Fail to set sub idl type, topic={} ", msg_topic);
      }
      b_has_created = true;
    }
  }
  auto ret = util::DispatcherDeserialize(ser_ptr, msg, ser_data, msg_topic);
  if (ret < kZero) {
    return ret;
  }
  COHLOGD("Deserialize done CallListenersForPrefixMatch");
  return CallListenersForPrefixMatch(msg_topic, matched_topic, msg, msg_info,
                                     is_dedicated);
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PARTICIPANT_DISPATCHER_H_
