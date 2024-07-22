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

#ifndef COMMUNICATION_PARTICIPANT_WRITER_H_
#define COMMUNICATION_PARTICIPANT_WRITER_H_

#include <functional>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <mutex>
#include <utility>

#include "communication/common/comm_log.h"
#include "communication/common/config_op.h"
#include "communication/participant/participant.h"
#include "communication/participant/writer_base.h"
#include "communication/qos/qos_adaptor.h"
#include "communication/common/comm_const.h"
#include "message/message.h"

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

using hobot::message::TypeWrapper;
namespace hobot {
namespace communication {

template<typename Serializer>
class DLLEXPORT Writer : public WriterBase {
 public:
  DISALLOW_COPY_AND_ASSIGNMENT(Writer);
  Writer(const CommAttr &comm_attr, const std::string &topic, int domain_id,
         ProtocolType type);
  virtual ~Writer() = default;

  virtual ErrorCode Write(
        const std::shared_ptr<typename Serializer::MsgType> &message,
        const spMessageInfo &msg_info = nullptr);

  virtual ErrorCode Init();
  virtual bool IsReady();
  virtual void SetEventListener(ConnEventCallback cb);

 protected:
  template <typename U>
  typename std::enable_if<HasBole2Flag<U>::value,
                          int>::type
  SetBoleMsgSubIdlType(int type,
                       std::shared_ptr<Serializer> bole_type_ptr);

  template <typename U>
  typename std::enable_if<!HasBole2Flag<U>::value,
                          int>::type
  SetBoleMsgSubIdlType(int type,
                       std::shared_ptr<Serializer> bole_type_ptr);

 protected:
  std::shared_ptr<Serializer> bole_type_ptr_{nullptr};
};

template<typename Serializer>
Writer<Serializer>::Writer(const CommAttr &comm_attr, const std::string &topic,
                           int domain_id, ProtocolType type)
    : WriterBase(comm_attr, topic, domain_id, type) {
  std::shared_ptr<Serializer> bole_type_ptr{std::make_shared<Serializer>()};
  auto ret =
      SetBoleMsgSubIdlType<typename Serializer::MsgType>(type, bole_type_ptr);
  if (ret < kZero) {
    COHLOGE("Fail to set sub idl type, topic={} prrotocol={}",
            topic, type);
    return;
  }

  bole_type_ptr_ = bole_type_ptr;
}

template <typename Serializer>
template <typename U>
typename std::enable_if<HasBole2Flag<U>::value, int>::type
Writer<Serializer>::SetBoleMsgSubIdlType(
    int type, std::shared_ptr<Serializer> bole_type_ptr) {
  auto sub_idl_type {CommConfigOperate::GetSubidlTypeByProtocolType(type)};
  // COHLOGD("Sub idl type is {}", sub_idl_type);
  if (sub_idl_type.size() == 0) {
    COHLOGW("Not get valid sub idl type for protocol {}, "
            "will use default sub idl type", type);
    return 0;
  }
  COHLOGD("Bole2 TypeSupport, set sub idl type to {}", sub_idl_type);
  return bole_type_ptr->SetSubIdlType(sub_idl_type);
}

template <typename Serializer>
template <typename U>
typename std::enable_if<!HasBole2Flag<U>::value, int>::type
Writer<Serializer>::SetBoleMsgSubIdlType(int, std::shared_ptr<Serializer>) {
  COHLOGD("Not Bole2 TypeSupport, do nothing");
  return kZero;
}

template<typename Serializer>
ErrorCode Writer<Serializer>::Init() {
  return BaseInit();
}

template<typename Serializer>
void Writer<Serializer>::SetEventListener(ConnEventCallback cb) {
  event_listener_ = cb;
}

template <typename Serializer>
bool Writer<Serializer>::IsReady() {
  return CheckReady();
}

template <typename Serializer>
ErrorCode Writer<Serializer>::Write(
    const std::shared_ptr<typename Serializer::MsgType> &message,
    const spMessageInfo &msg_info) {
  COHLOGD("begin write message topic={} ...", topic_name_);
  if (!message) {
    COHLOGE("Write message is NULL!");
    return COMM_CODE_MESSAGE_NULL;
  }

  if (qos_ptr_ && comm_attr_.qos_.qos_profile_.reliability_qos_policy.kind !=
                      RELIABILITY_DISABLED) {
    spCommSerData commSerData{std::make_shared<CommonSerializedData>()};

    auto res = bole_type_ptr_->Serialize(message, commSerData);
    if (res < kZero) {
      COHLOGE("Write message Serialize failed!");
      return COMM_CODE_MESSAGE_SERIALIZE_FAILED;
    }
    WriterBase::FillSerializeMsg(message, commSerData);
    COHLOGD("QoS CacheMessage");
    return qos_ptr_->CacheMessage("", commSerData, message, msg_info);
  }

  std::lock_guard<std::mutex> lg{part_mutex_};
  COHLOGD("participants_ size is {}", participants_.size());
  for (auto parti : participants_) {
    auto need_serialize = IsNeedSerialize(parti);
    spCommSerData ser_data{std::make_shared<CommonSerializedData>()};

    if (need_serialize) {
      // COHLOGD("Writer call Serialize");
      auto res = bole_type_ptr_->Serialize(message, ser_data);
      if (res < kZero) {
        COHLOGE("Writer Serialize failed")
        return COMM_CODE_MESSAGE_SERIALIZE_FAILED;
      }
      WriterBase::FillSerializeMsg(message, ser_data);
    } else {
      // For Intra, we need pass nullptr
      ser_data = nullptr;
    }
    if (parti->IsProetctE2Eable()) {
      if (!parti->E2EMsgProtectByTopic(ser_data, topic_name_)) {
        COHLOGE("E2E Protect Message failed, please check E2E config");
        continue;
      }
    }
    AsyncWrite(parti, topic_name_, ser_data,
             message, msg_info);
  }
  COHLOGD("end write message topic={} ...", topic_name_);
  return COMM_CODE_OK;
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PARTICIPANT_PARTICIPANT_H_
