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
#include "parkingslot_detection/parkingslot_detect_moudle.h"
#include <functional>
#include <vector>
#include <string>

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"
#include "common/proto_msg_all.h"

#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x

namespace fanya {
namespace parking {

ParkingslotDetectMoudle::ParkingslotDetectMoudle(
  const hobot::dataflow::ModuleOption &module_option):
    hobot::dataflow::Module(module_option) {
}

void ParkingslotDetectMoudle::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_apa_status",
    aph::apa_status);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_pad_point",
    loc::padPoint);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_pad_vehicle_pose",
    loc::padVehiclePose);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_camera_frame_array",
    camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_apa_ps_info",
    psd::SApaPSInfo);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_apa_ps_rect",
    psd::SApaPSRect);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_apa_pointI",
    psd::SApaPoint_I);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_psd_image",
    psd::Image);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
    "MsgCenterProc",
    ParkingslotDetectMoudle,
    MsgCenterProc,
    hobot::dataflow::ProcType::DF_MSG_COND_PROC,
    DF_VECTOR("sub_apa_status", "sub_pad_point", "sub_pad_vehicle_pose", "sub_camera_frame_array"),
    DF_VECTOR("pub_apa_ps_rect", "pub_apa_ps_info", "pub_apa_pointI", "pub_psd_image"));
}

int32_t ParkingslotDetectMoudle::Init() {
  return 0;
}

int32_t ParkingslotDetectMoudle::Start() {
  return hobot::dataflow::Module::Start();
}

int32_t ParkingslotDetectMoudle::Stop() {
  return hobot::dataflow::Module::Stop();
}

void ParkingslotDetectMoudle::Reset() {
  hobot::dataflow::Module::Reset();
}

int32_t ParkingslotDetectMoudle::DeInit() {
  return hobot::dataflow::Module::DeInit();
}

void ParkingslotDetectMoudle::MsgCenterProc(
  hobot::dataflow::spMsgResourceProc proc,
  const hobot::dataflow::MessageLists &msgs) {
  auto &sub_apa_status_msgs
    = msgs[proc->GetResultIndex("sub_apa_status")];
  for (auto &msg : *(sub_apa_status_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_apa_status msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_status
    auto apa_status = std::dynamic_pointer_cast<ApaStatusMsg>(sub_apa_status_msgs->at(0));
    if (apa_status && apa_status->proto.has_m_current_apa_status()){
      DFHLOG_I("sub_apa_status msg timestamp: {}, apa_status = {}",
        msg->GetGenTimestamp(), apa_status->proto.m_current_apa_status());
    }
  }
  auto &sub_pad_point_msgs
    = msgs[proc->GetResultIndex("sub_pad_point")];
  for (auto &msg : *(sub_pad_point_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_pad_point msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_pad_point
  }
  auto &sub_pad_vehicle_pose_msgs
    = msgs[proc->GetResultIndex("sub_pad_vehicle_pose")];
  for (auto &msg : *(sub_pad_vehicle_pose_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_pad_vehicle_pose msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_pad_vehicle_pose
  }
  auto &sub_camera_frame_array_msgs
    = msgs[proc->GetResultIndex("sub_camera_frame_array")];
  for (auto &msg : *(sub_camera_frame_array_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_camera_frame_array msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_camera_frame_array
  }
  auto pub_apa_ps_rect_port
    = proc->GetOutputPort("pub_apa_ps_rect");
  // do something with output port pub_apa_ps_rect
  UNUSED(pub_apa_ps_rect_port);
  auto pub_apa_ps_info_port
    = proc->GetOutputPort("pub_apa_ps_info");
  // do something with output port pub_apa_ps_info
  UNUSED(pub_apa_ps_info_port);
  auto pub_apa_pointI_port
    = proc->GetOutputPort("pub_apa_pointI");
  // do something with output port pub_apa_pointI
  UNUSED(pub_apa_pointI_port);
  auto pub_psd_image_port
    = proc->GetOutputPort("pub_psd_image");
  // do something with output port pub_psd_image
  UNUSED(pub_psd_image_port);
}

DATAFLOW_REGISTER_MODULE(ParkingslotDetectMoudle)

}  // namespace parking
}  // namespace fanya