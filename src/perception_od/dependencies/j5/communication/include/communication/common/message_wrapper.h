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

// This is communication log definition.

#ifndef COMMUNICATION_COMMON_MESSAGE_WRAPPER_H_
#define COMMUNICATION_COMMON_MESSAGE_WRAPPER_H_

#include <map>
#include <memory>
#include <string>
#include "message/message.h"
#include "message/proto/proto_msg.hpp"
#include "message/proto/proto_serializer.hpp"

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

using hobot::message::Message;
using hobot::message::ProtobufSerializer;
using hobot::message::ProtoMsg;
using hobot::message::SerializedData;
using hobot::message::SerializerPolicy;
using hobot::message::spMsg;
using hobot::message::spSerData;
using hobot::message::TypeWrapper;

class CommonSerializedData : public SerializedData {
 public:
    CommonSerializedData() : SerializedData() { }
    ~CommonSerializedData() override = default;
    CommonSerializedData(const CommonSerializedData &) = default;
    CommonSerializedData(CommonSerializedData &&) = default;
    CommonSerializedData &operator=(const CommonSerializedData &) = default;
    CommonSerializedData &operator=(CommonSerializedData &&) = default;

    int GetMetaTotalSize() { return meta_all_size_; }
    void SetMetaTotalSize(int size) { meta_all_size_ = size; }

    int64_t GetGenTimestamp() const { return gen_ts_; }
    void SetGenTimestamp(int64_t gen_ts) { gen_ts_ = gen_ts; }

    int64_t GetDoneTimestamp() const { return done_ts_; }
    void SetDoneTimestamp(const int64_t done_ts) { done_ts_ = done_ts; }

    std::string GetWriterId() const { return writer_id_; }
    void SetWriterId(const std::string writer_id) { writer_id_ = writer_id; }

    int64_t GetSequenceId() const { return seq_id_; }
    void SetSequenceId(const int64_t seq_id) { seq_id_ = seq_id; }

    int GetFromProto() { return from_proto_; }
    void SetFromProto(int proto) { from_proto_ = proto; }

    std::map<int, TypeWrapper> GetExtInfo() const { return extend_info_; }
    void SetExtInfo(const std::map<int, TypeWrapper> &extend_info);

 protected:
    int64_t gen_ts_{0};
    int64_t done_ts_{0};
    std::string writer_id_;
    int64_t seq_id_{0};
    int meta_all_size_{0};
    int from_proto_{-1};  // now just use in shm proto
    std::map<int, TypeWrapper> extend_info_;
};

using spCommSerData = std::shared_ptr<CommonSerializedData>;

template <typename Serializer>
class SerializableMsg {
 public:
  static spSerData CallSerialize(const spMsg &msg) {
    std::shared_ptr<Serializer> p_seria = std::make_shared<Serializer>();
    spCommSerData serialized_data{std::make_shared<CommonSerializedData>()};
    auto res = p_seria->Serialize(msg, serialized_data);
    if (res < 0) {
      return nullptr;
    }
    serialized_data->SetDataRef(msg->GetData());
    serialized_data->SetGenTimestamp(
                msg->GetGenTimestamp());
    serialized_data->SetDoneTimestamp(
        msg->GetDoneTimestamp());
    return serialized_data;
  }

  static int CallDeserialize(spMsg msg, const spSerData &data) {
    std::shared_ptr<Serializer> p_seria{std::make_shared<Serializer>()};
    auto res = p_seria->Deserialize(msg, data);
    if (res >= 0) {
      msg->SetData(data);
    }
    if (data == nullptr) {
      return res;
    }
    auto commSerData = std::dynamic_pointer_cast<
          CommonSerializedData>(data);
    if (commSerData == nullptr) {
      return res;
    }
    auto *meta = commSerData->GetMeta();
    int meta_size{commSerData->GetMetaSize()};
    int meta_total_size{commSerData->GetMetaTotalSize()};
    if ((meta_size >= 0) &&
      (meta != nullptr)) {
      if (meta_total_size > meta_size) {
        // it's bole communication msg
        msg->SetGenTimestamp(commSerData->GetGenTimestamp());
        msg->SetDoneTimestamp(commSerData->GetDoneTimestamp());
      }
    }
    return res;
  }
};

template<typename Serializer>
using spSerializableMsg = std::shared_ptr<SerializableMsg<Serializer>>;

}  // namespace communication
}  // namespace hobot
#endif  // COMMUNICATION_COMMON_MESSAGE_WRAPPER_H_
