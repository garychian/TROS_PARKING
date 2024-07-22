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

// Created by shaoyu.zou on 11/08/2021

#pragma once
#include <memory>
#include <iostream>

#include "../message.h"

namespace hobot {
namespace message {

struct RawSerializer : SerializerPolicy {
  using MsgType = Message;

  int Serialize(const spMsg &msg, const spSerData &ser_data) const override {
    auto data = msg->GetData();
    if (!data) {
      // no ref data and no meta, error
      std::cerr
          << "msg->GetData() is null in RawSerializer::Serialize!"
          << std::endl;
      return -1;
    }
    auto serialize_data =
        std::dynamic_pointer_cast<SerializedData>(data);

    // cast failed or meta size=0, means just have ref data
    if (!serialize_data || serialize_data->GetMetaSize() == 0) {
      return 0;
    }
    // meta size < 0, error
    if (serialize_data->GetMetaSize() < 0) {
      std::cerr
          << "msg->GetData() with meat-size is negative "
             "in RawSerializer::Serialize!"
          << std::endl;
      return -1;
    }
    ser_data->SetMeta(serialize_data->GetMeta());
    ser_data->SetMetaSize(serialize_data->GetMetaSize());
    return ser_data->GetMetaSize();
  }

  int Deserialize(const spMsg &, const spSerData &) const override {
    // do nothing for RawSerializer
    return 0;
  }
};
}  // namespace message
}  // namespace hobot
