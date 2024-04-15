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
#include "dataflow/callback/callback.h"
#include "dataflow/module/module.h"
#include "dataflow/module/module_option.h"
#include "dataflow/module/proc.h"
namespace fanya {
namespace parking {

class ObjectDetectMoudle:
  public hobot::dataflow::Module{
 public:
  explicit ObjectDetectMoudle(
    const hobot::dataflow::ModuleOption &module_option);
  ~ObjectDetectMoudle() = default;
  void InitPortsAndProcs() override;
  int32_t Start() override;
  int32_t Stop() override;
  void Reset() override;
  int32_t DeInit() override;
  void Proc_1(
    hobot::dataflow::spNoResourceProc proc);
 protected:
  int32_t Init() override;
};

}  // namespace parking
}  // namespace fanya