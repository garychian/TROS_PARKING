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

// This is common action types definition.

#ifndef COMMUNICATION_COMMON_ACTION_TYPES_H_
#define COMMUNICATION_COMMON_ACTION_TYPES_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "communication/common/types.h"

#ifdef SUPPORT_DDS
#include "communication/common/compiler_features.h"
COMM_MSVC_SUPPRESS_WARNING_WITH_PUSH(4819)
#include "communication/dds_adaptor/dds_adaptor_interface.h"
COMM_MSVC_SUPPRESS_WARNING_POP
#endif

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable : 4251)
#else
#define DLLEXPORT
#endif
#endif

#if defined(_MSC_VER)
#pragma warning(disable : 4275)
#endif

namespace hobot {
namespace communication {

namespace util {
using hobot::message::TypeSupport;

// serialize wrapper action result into serialized_data;
// called by WrappedActionResultSerializer::Serialize();
int DLLEXPORT SerializeWrappedActionResultMsg(
    const std::shared_ptr<SerializerPolicy> &serializer, int8_t code,
    const spMsg &msg, const spSerData &data);

int DLLEXPORT SerializeWrappedActionResultMsg(
    const std::shared_ptr<TypeSupport> &serializer, int8_t code,
    const spMsg &msg, const spSerData &data);

// deserialize wrapper action result from serialized_data;
// called by WrappedActionResultSerializer::Deserialize();
int DLLEXPORT DeserializeWrappedActionResultMsg(
    const std::shared_ptr<SerializerPolicy> &serializer, void *code,
    const spMsg &msg, const spSerData &data);

int DLLEXPORT DeserializeWrappedActionResultMsg(
    const std::shared_ptr<TypeSupport> &serializer, void *code,
    const spMsg &msg, const spSerData &data);
}  // namespace util

constexpr char action_goal_service_suffix[]{"_goal_service_"};
constexpr char action_cancel_service_suffix[]{"_cancel_service_"};
constexpr char action_result_service_suffix[]{"_result_service_"};
constexpr char action_feedback_topic_suffix[]{"_feedback_service_"};

// action server generate uuid
class GoalUUID {
 public:
  GoalUUID() : val_{0u} {}
  bool operator<(const GoalUUID &rhs) const { return val_ < rhs.val_; }
  uint64_t val_;
};

class DLLEXPORT GoalResponse : public Message {
 public:
  enum class Code : int8_t {
    REJECT = 1,
    ACCEPT = 2,
  };

 public:
  GoalResponse() : Message() {}
  GoalResponse(const GoalResponse &) = default;
  GoalResponse &operator=(const GoalResponse &) = default;
  GoalResponse(GoalResponse &&) = default;
  GoalResponse &operator=(GoalResponse &&) = default;

  GoalUUID uuid_;
  Code code_{Code::REJECT};
#ifdef SUPPORT_DDS
  DDSMsgTypeGoalResponse dds_data;
  using DDSDataType = DDSMsgTypeGoalResponse;
#ifdef PSMDDS
  using DDSCSMsgType = GoalResponse;
  const bool bole2_psm_dds_support_flag{true};
  void set_comm_user_define_msg(GoalResponse *msg, bool drop_ownship = true);
  GoalResponse *comm_user_define_msg();
#endif
#endif
};

class DLLEXPORT GoalResponseSerializer : public SerializerPolicy {
 public:
  using MsgType = GoalResponse;
#ifdef SUPPORT_DDS
  using DDSDataType = DDSTypeGoalResponse;
#ifdef PSMDDS
  using DDSCSSupportType = GoalResponseSerializer;
#endif
#endif
  int Serialize(const spMsg &msg, const spSerData &data) const override {
    return RealSerialize(msg, data);
  };
  int Deserialize(const spMsg &msg, const spSerData &ser_data) const override {
    return RealDeserialize(msg, ser_data);
  };
 private:
  int RealSerialize(const spMsg &msg, const spSerData &data) const;
  int RealDeserialize(const spMsg &msg, const spSerData &ser_data) const;
};

class DLLEXPORT CancelResponse : public Message {
 public:
  enum class Code : int8_t {
    REJECT = 1,
    ACCEPT = 2,
  };

 public:
  CancelResponse();
  explicit CancelResponse(Code code);

  Code code_{Code::REJECT};
#ifdef SUPPORT_DDS
  DDSMsgTypeCancelResponse dds_data;
  using DDSDataType = DDSMsgTypeCancelResponse;
#endif
};

class DLLEXPORT CancelResponseSerializer : public SerializerPolicy {
 public:
  using MsgType = CancelResponse;

#ifdef SUPPORT_DDS
  using DDSDataType = DDSTypeCancelResponse;
#endif

  int Serialize(const spMsg &msg, const spSerData &data) const override {
    return RealSerialize(msg, data);
  };
  int Deserialize(const spMsg &msg, const spSerData &data) const override {
    return RealDeserialize(msg, data);
  };
 private:
  int RealSerialize(const spMsg &msg, const spSerData &data) const;
  int RealDeserialize(const spMsg &msg, const spSerData &data) const;
};

class DLLEXPORT CancelRequest : public Message {
 public:
  CancelRequest();
  GoalUUID uuid_;
#ifdef SUPPORT_DDS
  DDSMsgTypeCancelRequest dds_data;
  using DDSDataType = DDSMsgTypeCancelRequest;
#endif
};

class DLLEXPORT CancelRequestSerializer : public SerializerPolicy {
 public:
  using MsgType = CancelRequest;
#ifdef SUPPORT_DDS
  using DDSDataType = DDSTypeCancelRequest;
#endif

  int Serialize(const spMsg &msg, const spSerData &data) const override {
    return RealSerialize(msg, data);
  };
  int Deserialize(const spMsg &msg, const spSerData &data) const override {
    return RealDeserialize(msg, data);
  };
 private:
  int RealSerialize(const spMsg &msg, const spSerData &data) const;
  int RealDeserialize(const spMsg &msg, const spSerData &data) const;
};

class DLLEXPORT ResultRequest : public Message {
 public:
  ResultRequest();
  GoalUUID uuid_;
#ifdef SUPPORT_DDS
  DDSMsgTypeResultRequest dds_data;
  using DDSDataType = DDSMsgTypeResultRequest;
#endif
};

class DLLEXPORT ResultRequestSerializer : public SerializerPolicy {
 public:
  using MsgType = ResultRequest;
#ifdef SUPPORT_DDS
  using DDSDataType = DDSTypeResultRequest;
#endif
  int Serialize(const spMsg &msg, const spSerData &data) const override {
    return RealSerialize(msg, data);
  };
  int Deserialize(const spMsg &msg, const spSerData &data) const override {
    return RealDeserialize(msg, data);
  };
 private:
  int RealSerialize(const spMsg &msg, const spSerData &data) const;
  int RealDeserialize(const spMsg &msg, const spSerData &data) const;
};

// wrap action result
template <typename Serializer>
class WrappedActionResult : public Message {
 public:
  enum class Code : int8_t {
    SUCCEEDED = 1,
    ABORTED = 2,
    CANCELED = 3,
    MAX = 4
  };
  WrappedActionResult() : Message() {
#ifdef SUPPORT_DDS
    bole_type_ptr_ = std::make_shared<Serializer>();
#endif
  }
  WrappedActionResult(Code r_code,
                      std::shared_ptr<typename Serializer::MsgType> msg)
      : Message(), code(r_code), result(msg) {
    this->SetData(msg->GetData());
#ifdef SUPPORT_DDS
    bole_type_ptr_ = std::make_shared<Serializer>();
#endif
  }
  Code code{Code::SUCCEEDED};  // result code
  std::shared_ptr<typename Serializer::MsgType> result;
  std::shared_ptr<Serializer> bole_type_ptr_;
#ifdef SUPPORT_DDS

  using DDSDataType = DDSMsgTypeWrappedActionResult;

  DDSMsgTypeWrappedActionResult dds_data;

  template <typename U = Serializer>
  typename std::enable_if<!HasMemberDDSData<typename U::MsgType>::value &&
                              !HasBole2DDSFlag<typename U::MsgType>::value,
                          void>::type
  ConvertToDDSUnderlayResult() {}

  template <typename U = Serializer>
  typename std::enable_if<!HasMemberDDSData<typename U::MsgType>::value &&
                              !HasBole2DDSFlag<typename U::MsgType>::value,
                          void>::type
  ConvertFromDDSUnderlayResult() {}

  template <typename U = Serializer>
  typename std::enable_if<HasMemberDDSData<typename U::MsgType>::value,
                          void>::type
  ConvertToDDSUnderlayResult() {
    if (result == nullptr) {
      return;
    }
    WrappedActionResult2DDSMsgTypeWrappedActionResult(
        static_cast<int>(code), result->dds_data, dds_data);
  }

  template <typename U = Serializer>
  typename std::enable_if<HasMemberDDSData<typename U::MsgType>::value,
                          void>::type
  ConvertFromDDSUnderlayResult() {
    result = std::make_shared<typename U::MsgType>();
    DDSMsgTypeWrappedActionResult2WrappedActionResult(dds_data, &code,
                                                      result->dds_data);
  }

  template <typename U = Serializer>
  typename std::enable_if<HasBole2DDSFlag<typename U::MsgType>::value,
                          void>::type
  ConvertToDDSUnderlayResult() {
    using DDSMsgType = typename Serializer::MsgType::DDSDataType;
    // using DDSType = typename Serializer::MsgType::DDSType;
    DDSMsgType *bole_sub_msg{nullptr};
    if (bole_type_ptr_) {
      bole_sub_msg = reinterpret_cast<DDSMsgType *>(
          bole_type_ptr_->ConvertToSubIdlMsg(result));
    }
    if (bole_sub_msg == nullptr) {
      return;
    }
    WrappedActionResult2DDSMsgTypeWrappedActionResult(static_cast<int>(code),
                                                      *bole_sub_msg, dds_data);
  }

  template <typename U = Serializer>
  typename std::enable_if<HasBole2DDSFlag<typename U::MsgType>::value,
                          void>::type
  ConvertFromDDSUnderlayResult() {
    using DDSMsgType = typename Serializer::MsgType::DDSDataType;
    result = std::make_shared<typename U::MsgType>();
    DDSMsgType underlay_msg;
    DDSMsgTypeWrappedActionResult2WrappedActionResult(dds_data, &code,
                                                      underlay_msg);
    if (bole_type_ptr_) {
      bole_type_ptr_->ConvertToBoleMsg(static_cast<void *>(&underlay_msg),
                                       result);
    }
  }

#endif
};

template <typename Serializer>
class WrappedActionResultSerializer : public SerializerPolicy {
 public:
  using MsgType = WrappedActionResult<Serializer>;
  using UserMsgType = typename Serializer::MsgType;

#ifdef SUPPORT_DDS
  using DDSDataType = DDSTypeWrappedActionResult;
#endif

  int Serialize(const spMsg &msg, const spSerData &data) const override {
    const auto &wrap_result_msg = std::dynamic_pointer_cast<MsgType>(msg);
    if (!wrap_result_msg || !data) {
      return -1;
    }

    auto &user_result_msg = wrap_result_msg->result;
    // As we want to re-use user Serializer, we need to create temp_ser_data
    auto sp_user_result_msg_ser = std::make_shared<Serializer>();
    // serialize userdata(result) and wrapped code(code)
    return util::SerializeWrappedActionResultMsg(
        sp_user_result_msg_ser, static_cast<int8_t>(wrap_result_msg->code),
        user_result_msg, data);
  }
  int Deserialize(const spMsg &msg, const spSerData &data) const override {
    const auto &wrap_result_msg = std::dynamic_pointer_cast<MsgType>(msg);
    if (!wrap_result_msg || !data) {
      return -1;
    }
    // memcpy(&wrap_result_msg->code, data->GetMeta(), sizeof(int8_t));

    // when deser, we need create UserMsgType
    wrap_result_msg->result = std::make_shared<UserMsgType>();
    auto &user_result_msg = wrap_result_msg->result;
    auto sp_user_result_msg_ser = std::make_shared<Serializer>();
    return util::DeserializeWrappedActionResultMsg(
        sp_user_result_msg_ser, &wrap_result_msg->code, user_result_msg, data);
  }
};
}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_ACTION_TYPES_H_
