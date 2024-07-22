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
#include <memory>
#include <iostream>

#include "../message.h"
#include "proto_msg.hpp"

namespace hobot {
namespace message {

template<typename ProtobufMsg>
struct ProtobufSerializer : SerializerPolicy {
  using MsgType = ProtoMsg<ProtobufMsg>;

  int Serialize(const spMsg &msg, const spSerData &ser_data) const override {
    const auto &proto_msg = std::dynamic_pointer_cast<MsgType>(msg);
    if (!proto_msg) {
      std::cerr
          << "dynamic_pointer_cast failed in ProtobufSerializer::Serialize!"
          << std::endl;
      return -1;
    }
    ser_data->AllocMemMeta(static_cast<int>(proto_msg->proto.ByteSizeLong()));
    if (!proto_msg->proto.SerializeToArray(ser_data->GetMeta(),
                                           ser_data->GetMetaSize())) {
      std::cerr << "SerializeToArray failed in ProtobufSerializer::Serialize!"
                << std::endl;
      return -1;
    }
    ser_data->SetMetaSize(static_cast<int>(proto_msg->proto.ByteSizeLong()));
    return static_cast<int>(proto_msg->proto.ByteSizeLong());
  }

  int Deserialize(const spMsg &msg, const spSerData &ser_data) const override {
    const auto &proto_msg = std::dynamic_pointer_cast<MsgType>(msg);
    if (!proto_msg || !ser_data) {
      std::cerr
          << "dynamic_pointer_cast failed in ProtobufSerializer::Deserialize!"
          << std::endl;
      return -1;
    }
    if (!proto_msg->proto.ParsePartialFromArray(ser_data->GetMeta(),
                                                ser_data->GetMetaSize())) {
      std::cerr
          << "ParsePartialFromArray failed in "
             "ProtobufSerializer::Deserialize!"
          << std::endl;
      return -1;
    }
    return ser_data->GetMetaSize();
  }
};
}  // namespace message
}  // namespace hobot
