// Copyright (c) [2021-2022] [Horizon Robotics][Horizon Bole].
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

// Created by peizhe.chen on 1/11/2021


#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <map>

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace message {

// The following 4 keys are already occupied
namespace ExtendInfoKey {
  // kMessageType used for intra, will not be serialized in communication,
  // and will not be transmitted to the remote
  constexpr int kMessageType = 0;
  // kMessageId,kSendTime,kRecvTime will be serialized in communication,
  // and will be transmitted to the remote
  constexpr int kMessageId = 1;  // used to trace message,seted by user
  constexpr int kSendTime = 2;  // used for lantency Statistics in dataflow,
                                // the send time, seted in communication pub
  constexpr int kRecvTime = 3;  // used for lantency Statistics in dataflow,
                                // the receive time, seted in communication sub
}  // namespace ExtendInfoKey

class TypeWrapper {
 public:
  enum TypeEnum {
    kUnknow = 0,
    kInt64,
    kString
  };

 public:
  TypeWrapper(): type_(kUnknow) {
  }

  explicit TypeWrapper(const int64_t& value):
    type_(kInt64), value_int64_(value) {
  }

  explicit TypeWrapper(const std::string& value):
    type_(kString), value_string_(value) {
  }

  TypeEnum GetType() {
    return type_;
  }

  int64_t GetValueInt64() {
    return value_int64_;
  }

  std::string GetValueString() {
    return value_string_;
  }

 private:
  TypeEnum type_{kUnknow};
  int64_t value_int64_{};
  std::string value_string_{};
};

class DataRef {
 public:
  DataRef(void *data, const int data_size) {
    data_vec_.push_back(data);
    data_size_vec_.push_back(data_size);
  }
  DataRef() = default;
  virtual ~DataRef() = default;
  DataRef(const DataRef &) = default;
  DataRef &operator=(const DataRef &) = default;
  DataRef(DataRef &&) = default;
  DataRef &operator=(DataRef &&) = default;

  void *GetData() {
    if (data_vec_.empty()) {
      return nullptr;
    }
    return data_vec_[0];
  }
  int GetDataSize() {
    if (data_size_vec_.empty()) {
      return 0;
    }
    return data_size_vec_[0];
  }
  void SetData(void *data) {
    if (!data) {
      return;
    }
    if (data_vec_.empty()) {
      data_vec_.push_back(data);
    } else {
      data_vec_[0] = data;
    }
  }
  void SetDataSize(const int data_size) {
    if (data_size <= 0) {
      return;
    }
    if (data_size_vec_.empty()) {
      data_size_vec_.push_back(data_size);
    } else {
      data_size_vec_[0] = data_size;
    }
  }

  std::vector<void *> GetDataVec() const { return data_vec_; }
  std::vector<int> GetDataSizeVec() const { return data_size_vec_; }
  void PushData(void *data, const int size) {
    if (!data || size <= 0) {
      return;
    }
    data_vec_.push_back(data);
    data_size_vec_.push_back(size);
  }

  virtual void Reset() {
    data_vec_.clear();
    data_size_vec_.clear();
  }

 protected:
  std::vector<void *> data_vec_;
  std::vector<int> data_size_vec_;
};

using spDataRef = std::shared_ptr<DataRef>;

class SerializedData : public DataRef {
 public:
  SerializedData(void *data, int data_size, void *meta, int meta_size)
      : DataRef(data, data_size) {
    meta_ = meta;
    meta_size_ = meta_size;
    meta_allocated_ = std::make_shared<std::vector<uint8_t> >();
  }
  SerializedData(void *meta, int meta_size) : DataRef() {
    meta_ = meta;
    meta_size_ = meta_size;
    meta_allocated_ = std::make_shared<std::vector<uint8_t> >();
  }
  SerializedData() : DataRef() {
    meta_ = nullptr;
    meta_size_ = 0;
    meta_allocated_ = std::make_shared<std::vector<uint8_t> >();
  }

  ~SerializedData() override = default;
  SerializedData(const SerializedData &) = default;
  SerializedData(SerializedData &&) = default;
  SerializedData &operator=(const SerializedData &) = default;
  SerializedData &operator=(SerializedData &&) = default;

  void AllocMemMeta(const int size) {
    if (meta_allocated_->size() < static_cast<size_t>(size)) {
      meta_allocated_->resize(size);
    }
    meta_ = meta_allocated_->data();
    meta_size_ = size;
  }

  void SetDataRef(const spDataRef &data_ref) {
    if (data_ref) {
      const auto &data = data_ref->GetDataVec();
      const auto &data_size = data_ref->GetDataSizeVec();
      if (data.size() != data_size.size()) {
        std::cerr << "data vec size is not equal to data_size vec size!"
                  << std::endl;
        return;
      }
      // clear previous ref data, first
      data_vec_.clear();
      data_size_vec_.clear();
      // set ref data, now
      for (size_t i = 0; i < data.size(); ++i) {
        this->PushData(data[i], data_size[i]);
      }
    }
  }

  void *GetMeta() const { return meta_; }
  int GetMetaSize() const { return meta_size_; }
  void SetMeta(void *meta) { meta_ = meta; }
  void SetMetaSize(int meta_size) { meta_size_ = meta_size; }

  int64_t GetSendTimestamp() const { return send_ts_; }
  uint32_t GetSendCounter() const { return counter_; }

  void Reset() override {
    meta_ = nullptr;
    meta_size_ = 0;
    meta_allocated_.reset();
    send_ts_ = 0;
    counter_ = 0;

    DataRef::Reset();
  }

 protected:
  void *meta_{nullptr};
  int meta_size_{0};
  std::shared_ptr<std::vector<uint8_t> > meta_allocated_;
  int64_t send_ts_{0};
  uint32_t counter_{0};
};

using spSerData = std::shared_ptr<SerializedData>;

class Message {
 public:
  static constexpr int kExtendInfoNum = 8;
  Message() = default;

  Message(const Message &) = default;
  Message &operator=(const Message &) = default;
  Message(Message &&) = default;
  Message &operator=(Message &&) = default;

  virtual ~Message() = default;

  int64_t GetGenTimestamp() const { return gen_ts_; }
  void SetGenTimestamp(int64_t gen_ts) { gen_ts_ = gen_ts; }

  int64_t GetDoneTimestamp() const { return done_ts_; }
  void SetDoneTimestamp(const int64_t done_ts) { done_ts_ = done_ts; }

  virtual void SetData(spDataRef data) { data_ = std::move(data); }
  spDataRef GetData() const { return data_; }

  virtual std::string Info() const {
    return "[" + std::to_string(gen_ts_) + "_" + std::to_string(done_ts_) + "]";
  }

  bool SetExtInfo(int key, const TypeWrapper& value) {
    if (key < 0 || key >= kExtendInfoNum) {
      return false;
    }
    ext_info_[key] = value;
    ext_info_flag_ |= 1 << key;
    return true;
  }

  bool GetExtInfo(int key, TypeWrapper& value) {
    if (key < 0 || key >= kExtendInfoNum) {
      return false;
    }
    if (!((ext_info_flag_ >> key)&1)) {
      return false;
    }
    value = ext_info_[key];
    return true;
  }

  bool DelExtendInfo(int key) {
    if (key < 0 || key >= kExtendInfoNum) {
      return false;
    }
    ext_info_flag_ &= ~(1 << key);
    return true;
  }

  std::map<int, TypeWrapper> ListExendInfo() {
    std::map<int, TypeWrapper> result;
    for (int i = 0; i < kExtendInfoNum; i++) {
      if ((ext_info_flag_ >> i)&1) {
        result[i] = ext_info_[i];
      }
    }
    return result;
  }

 protected:
  int64_t gen_ts_{0};
  int64_t done_ts_{0};
  spDataRef data_;
  TypeWrapper ext_info_[kExtendInfoNum];
  char ext_info_flag_{0};
};

using spMsg = std::shared_ptr<Message>;

class TypeSupport {
 public:
  TypeSupport() = default;
  TypeSupport(const TypeSupport &) = default;
  TypeSupport &operator=(const TypeSupport &) = default;
  TypeSupport(TypeSupport &&) = default;
  TypeSupport &operator=(TypeSupport &&) = default;

  virtual ~TypeSupport() {}

  virtual int Serialize(const spMsg &msg, const spSerData &ser_data) = 0;
  virtual int Deserialize(const spMsg &msg,
                          const spSerData &ser_data) = 0;
  virtual void* ConvertToSubIdlMsg(const spMsg &msg) = 0;
  virtual int ConvertToBoleMsg(void * data, const spMsg &msg) = 0;
};
using spTypeSupport = std::shared_ptr<TypeSupport>;

struct SerializerPolicy {
  using MsgType = Message;

  virtual ~SerializerPolicy() = default;
  SerializerPolicy() = default;
  SerializerPolicy(const SerializerPolicy &) = delete;
  SerializerPolicy &operator=(const SerializerPolicy &) = delete;
  SerializerPolicy(SerializerPolicy &&) = delete;
  SerializerPolicy &operator=(SerializerPolicy &&) = delete;

  virtual int Serialize(const spMsg &msg, const spSerData &data) const = 0;
  virtual int Deserialize(const spMsg &msg, const spSerData &data) const = 0;
};

using Serializable = SerializerPolicy;

struct DefaultSerializer : Serializable {
  int Serialize(const spMsg &, const spSerData &) const override { return 0; }
  int Deserialize(const spMsg &, const spSerData &) const override {
    return 0;
  }
};

class MsgBroker final : public SerializedData {
 public:
  explicit MsgBroker(const spMsg &raw_msg) : raw_msg_(raw_msg) {
    if (raw_msg_ && raw_msg_->GetData()) {
      *static_cast<DataRef *>(this) = *raw_msg_->GetData();
    }
  }

 private:
  spMsg raw_msg_;
};
}  // namespace message
}  // namespace hobot
