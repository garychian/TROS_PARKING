// Copyright (c) [2021-2023] [Horizon Robotics].
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
#pragma once

#include <memory>

#include "dataflow/callback/callback.h"
#include "dataflow/module/module.h"
#include "dataflow/module/module_option.h"
#include "dataflow/module/proc.h"
#include "common/proto_msg_all.h"
#include "message/message.h"
#include "message/proto/proto_serializer.hpp"

#include "vehicleio/client.h"
#include "common/log.h"
#include "common/jsonutil.h"
#include "common/timestamp.h"
// #include "message/proto_msg.h"


namespace fanya {
namespace parking {

using spVehicleio = std::shared_ptr<hobot::vehicleio::Vehicleio>;
using hobot::dataflow::ModuleOption;

class CanTransModule: public hobot::dataflow::Module{
 public:
  explicit CanTransModule(const ModuleOption& module_option);
  ~CanTransModule() = default;
  void InitPortsAndProcs() override;
  int32_t Start() override;
  int32_t Stop() override;
  void Reset() override;
  int32_t DeInit() override;
  void GetSignalProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists& msgs);

 protected:
  int32_t Init() override;

 private:
  spVehicleio vehicleio_{nullptr};
};

}  // namespace samples
}  // namespace hobot

