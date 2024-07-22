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

// Created by hengzhao.liu on 8/30/2021

#include "../message.h"

#pragma once

namespace hobot {
namespace message {
template <typename ProtobufMsg>
struct ProtoMsg : Message {
  using Message::Message;
  ProtobufMsg proto;
};
}  // namespace message
}  // namespace hobot
