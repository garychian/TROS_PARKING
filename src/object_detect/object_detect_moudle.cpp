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
#include "./object_detect_moudle.h"
#include <functional>
#include <vector>
#include <string>

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"


#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x

ObjectDetectMoudle::ObjectDetectMoudle(
  const hobot::dataflow::ModuleOption &module_option):
    hobot::dataflow::Module(module_option) {
}

void ObjectDetectMoudle::InitPortsAndProcs() {
  DF_MODULE_REGISTER_NO_INPUT_PROC(
    "Proc_1",
    ObjectDetectMoudle,
    Proc_1,
    hobot::dataflow::ProcType::DF_RESIDENT_PROC,
    DF_VECTOR());
}

int32_t ObjectDetectMoudle::Init() {
  return 0;
}

int32_t ObjectDetectMoudle::Start() {
  return hobot::dataflow::Module::Start();
}

int32_t ObjectDetectMoudle::Stop() {
  return hobot::dataflow::Module::Stop();
}

void ObjectDetectMoudle::Reset() {
  hobot::dataflow::Module::Reset();
}

int32_t ObjectDetectMoudle::DeInit() {
  return hobot::dataflow::Module::DeInit();
}

void ObjectDetectMoudle::Proc_1(hobot::dataflow::spNoResourceProc proc) {
}

DATAFLOW_REGISTER_MODULE(ObjectDetectMoudle)
