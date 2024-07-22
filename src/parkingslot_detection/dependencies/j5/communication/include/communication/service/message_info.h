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

// This is message info.

#ifndef MESSAGE_INFO_H_
#define MESSAGE_INFO_H_

#include <string>
// #include <cstring>
#include <memory>
#include <iostream>

namespace hobot {
namespace communication {
class MessageInfo {
 public:
  explicit MessageInfo(uint64_t writer_id, uint64_t seq_id, uint64_t spare_id)
    : writer_id_(writer_id), seq_id_(seq_id), spare_id_(spare_id) {
  }

  MessageInfo(const MessageInfo& another)
    : MessageInfo(another.writer_id_, another.seq_id_, another.spare_id_) {
  }

  MessageInfo() {}

  virtual ~MessageInfo() = default;

  MessageInfo& operator=(const MessageInfo& another) {
    if (this != &another) {
      writer_id_ = another.writer_id_;
      seq_id_ = another.seq_id_;
      spare_id_ = another.spare_id_;
    }
    return *this;
  }

  bool SerializeTo(std::string &dst) const {
    dst.assign(reinterpret_cast<const char*>(&writer_id_), sizeof(writer_id_));
    dst.append(reinterpret_cast<const char*>(&seq_id_), sizeof(seq_id_));
    dst.append(reinterpret_cast<const char*>(&spare_id_), sizeof(spare_id_));
    return true;
  }

  bool DeserializeFrom(const std::string& src) {
    return DeserializeFrom(src.data(), src.size());
  }

  bool DeserializeFrom(const char* src, std::size_t len) {
    if (src == nullptr) {
      return false;
    }
    if (sizeof(writer_id_) + sizeof(seq_id_) + sizeof(spare_id_) != len) {
      std::cout << "ERROR! len unmatch: " << len << std::endl;
      return false;
    }
    std::memcpy(&writer_id_, src, sizeof(writer_id_));
    std::memcpy(&seq_id_, src + sizeof(writer_id_), sizeof(seq_id_));
    std::memcpy(&spare_id_, src + sizeof(writer_id_) + sizeof(seq_id_),
                sizeof(spare_id_));
    return true;
  }

  uint64_t GetWriterId() const { return writer_id_; }
  void SetWriterId(const uint64_t& writer_id) { writer_id_ = writer_id; }

  uint64_t GetSeqId() const { return seq_id_; }
  void SetSeqId(uint64_t seq_id) { seq_id_ = seq_id; }

  uint64_t GetSpareId() const { return spare_id_; }
  void SetSpareId(uint64_t spare_id) { spare_id_ = spare_id; }

 private:
  uint64_t writer_id_{0u};
  uint64_t seq_id_{0u};
  uint64_t spare_id_{0u};
};

using spMessageInfo = std::shared_ptr<MessageInfo>;
}  // namespace communication
}  // namespace hobot
#endif
