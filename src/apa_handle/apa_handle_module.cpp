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
#include "apa_handle/apa_handle_module.h"
#include <functional>
#include <vector>
#include <string>

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"
#include "common/proto_msg_all.h"

#include "fanya_protocol/aph_idl_wrapper.h"
#include "fanya_protocol/fsm_idl_wrapper.h"

#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x

ApaHandleModule::ApaHandleModule(
  const hobot::dataflow::ModuleOption &module_option):
    hobot::dataflow::Module(module_option) {
}

void ApaHandleModule::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_slot_label",
    fsm::Slotlabel);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_apa_status_req",
    fsm::Apastatusreq);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_apa_status",
    aph::apa_status);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_target_slot",
    aph::target_slot);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
    "MsgCenterProc",
    ApaHandleModule,
    MsgCenterProc,
    hobot::dataflow::ProcType::DF_MSG_COND_PROC,
    DF_VECTOR("sub_slot_label", "sub_apa_status_req"),
    DF_VECTOR("pub_apa_status", "pub_target_slot"));
}

int32_t ApaHandleModule::Init() {
  return 0;
}

int32_t ApaHandleModule::Start() {
  return hobot::dataflow::Module::Start();
}

int32_t ApaHandleModule::Stop() {
  return hobot::dataflow::Module::Stop();
}

void ApaHandleModule::Reset() {
  hobot::dataflow::Module::Reset();
}

int32_t ApaHandleModule::DeInit() {
  return hobot::dataflow::Module::DeInit();
}

void ApaHandleModule::MsgCenterProc(
  hobot::dataflow::spMsgResourceProc proc,
  const hobot::dataflow::MessageLists &msgs) {
  auto &sub_slot_label_msgs
    = msgs[proc->GetResultIndex("sub_slot_label")];
  for (auto &msg : *(sub_slot_label_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_slot_label msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_slot_label
  }
  auto &sub_apa_status_req_msgs
    = msgs[proc->GetResultIndex("sub_apa_status_req")];
  for (auto &msg : *(sub_apa_status_req_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_apa_status_req msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_status_req
  }
  auto pub_apa_status_port
    = proc->GetOutputPort("pub_apa_status");
  // do something with output port pub_apa_status
  UNUSED(pub_apa_status_port);
  auto pub_target_slot_port
    = proc->GetOutputPort("pub_target_slot");
  // do something with output port pub_target_slot
  UNUSED(pub_target_slot_port);
}

DATAFLOW_REGISTER_MODULE(ApaHandleModule)
