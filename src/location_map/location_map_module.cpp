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
#include "location_map/location_map_module.h"
#include <functional>
#include <vector>
#include <string>

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"
#include "common/proto_msg_all.h"
#include "common/timestamp.h"

#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x

namespace fanya {
namespace parking {

LocationMapModule::LocationMapModule(
  const hobot::dataflow::ModuleOption &module_option):
    hobot::dataflow::Module(module_option) {
}

void LocationMapModule::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_vehicleio_data",
    vehicleiostate::CanSignalUnit);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_apa_status",
    aph::apa_status);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_target_slot",
    aph::target_slot);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_imu_data",
    sen::IMUData);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_gnss_data",
    sen::GNSSData);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_ins_data",
    sen::INSData);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_dual_antenna_data",
    sen::DualAntennaData);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_psd_image",
    psd::Image);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_camera_frame_array",
    camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_apa_ps_info",
    psd::SApaPSInfo);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_apa_ps_rect",
    psd::SApaPSRect);
  DF_MODULE_INIT_IDL_INPUT_PORT(
    "sub_apa_pointI",
    psd::SApaPoint_I);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_pad_realtime_loc",
    loc::padRealTimeLocation);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_loc_status",
    loc::LocStatus);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_vehicle_pose",
    loc::VehiclePos);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_app2emap",
    loc::App2emap_DR);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_pad_point",
    loc::padPoint);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_pad_vehicleio_pose",
    loc::padVehiclePose);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_map_info",
    loc::MapInfo);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_parking_slot",
    loc::parkingSlots);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_stio_lane_lines",
    loc::laneLines);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_stop_lines",
    loc::stopLines);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_bumps",
    loc::bumps);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_crosswalks",
    loc::crossWalks);
  DF_MODULE_INIT_IDL_OUTPUT_PORT(
    "pub_arrows",
    loc::arrows);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
    "MsgCenterProc",
    LocationMapModule,
    MsgCenterProc,
    hobot::dataflow::ProcType::DF_MSG_COND_PROC,
    DF_VECTOR("sub_vehicleio_data", "sub_apa_status", "sub_target_slot", "sub_imu_data", "sub_gnss_data", "sub_ins_data", "sub_dual_antenna_data", "sub_psd_image", "sub_camera_frame_array", "sub_apa_ps_info", "sub_apa_ps_rect", "sub_apa_pointI"),
    DF_VECTOR("pub_pad_realtime_loc", "pub_loc_status", "pub_vehicle_pose", "pub_app2emap", "pub_pad_point", "pub_map_info", "pub_parking_slot", "pub_stio_lane_lines", "pub_stop_lines", "pub_bumps", "pub_crosswalks", "pub_arrows", "pub_pad_vehicleio_pose"));

  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
    "TimerProc",
    LocationMapModule,
    TimerProc,
    hobot::dataflow::ProcType::DF_MSG_TIMER_PROC,
    DF_VECTOR(),
    DF_VECTOR("pub_loc_status"));
}

int32_t LocationMapModule::Init() {
  return 0;
}

int32_t LocationMapModule::Start() {
  return hobot::dataflow::Module::Start();
}

int32_t LocationMapModule::Stop() {
  return hobot::dataflow::Module::Stop();
}

void LocationMapModule::Reset() {
  hobot::dataflow::Module::Reset();
}

int32_t LocationMapModule::DeInit() {
  return hobot::dataflow::Module::DeInit();
}

void LocationMapModule::MsgCenterProc(
  hobot::dataflow::spMsgResourceProc proc,
  const hobot::dataflow::MessageLists &msgs) {
  auto &sub_vehicleio_data_msgs
    = msgs[proc->GetResultIndex("sub_vehicleio_data")];
  for (auto &msg : *(sub_vehicleio_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_vehicleio_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_vehicleio_data
  }
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
  auto &sub_target_slot_msgs
    = msgs[proc->GetResultIndex("sub_target_slot")];
  for (auto &msg : *(sub_target_slot_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_target_slot msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_target_slot
    auto target_slot = std::dynamic_pointer_cast<TargetSlotMsg>(sub_target_slot_msgs->at(0));
    if (target_slot && target_slot->proto.has_m_user_select_slot_label_idx()){
      DFHLOG_I("sub_target_slot msg timestamp: {}, apa_status = {}",
        msg->GetGenTimestamp(), target_slot->proto.m_user_select_slot_label_idx());
    }
  }
  auto &sub_imu_data_msgs
    = msgs[proc->GetResultIndex("sub_imu_data")];
  for (auto &msg : *(sub_imu_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_imu_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_imu_data
  }
  auto &sub_gnss_data_msgs
    = msgs[proc->GetResultIndex("sub_gnss_data")];
  for (auto &msg : *(sub_gnss_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_gnss_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_gnss_data
  }
  auto &sub_ins_data_msgs
    = msgs[proc->GetResultIndex("sub_ins_data")];
  for (auto &msg : *(sub_ins_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_ins_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_ins_data
  }
  auto &sub_dual_antenna_data_msgs
    = msgs[proc->GetResultIndex("sub_dual_antenna_data")];
  for (auto &msg : *(sub_dual_antenna_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_dual_antenna_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_dual_antenna_data
  }
  auto &sub_psd_image_msgs
    = msgs[proc->GetResultIndex("sub_psd_image")];
  for (auto &msg : *(sub_psd_image_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_psd_image msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_psd_image
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
  auto &sub_apa_ps_info_msgs
    = msgs[proc->GetResultIndex("sub_apa_ps_info")];
  for (auto &msg : *(sub_apa_ps_info_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_apa_ps_info msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_ps_info
  }
  auto &sub_apa_ps_rect_msgs
    = msgs[proc->GetResultIndex("sub_apa_ps_rect")];
  for (auto &msg : *(sub_apa_ps_rect_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_apa_ps_rect msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_ps_rect
  }
  auto &sub_apa_pointI_msgs
    = msgs[proc->GetResultIndex("sub_apa_pointI")];
  for (auto &msg : *(sub_apa_pointI_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_D("sub_apa_pointI msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_pointI
  }
  auto pub_pad_realtime_loc_port
    = proc->GetOutputPort("pub_pad_realtime_loc");
  // do something with output port pub_pad_realtime_loc
  UNUSED(pub_pad_realtime_loc_port);
  auto pub_loc_status_port
    = proc->GetOutputPort("pub_loc_status");
  // do something with output port pub_loc_status
  UNUSED(pub_loc_status_port);
  auto pub_vehicle_pose_port
    = proc->GetOutputPort("pub_vehicle_pose");
  // do something with output port pub_vehicle_pose
  UNUSED(pub_vehicle_pose_port);
  auto pub_app2emap_port
    = proc->GetOutputPort("pub_app2emap");
  // do something with output port pub_app2emap
  UNUSED(pub_app2emap_port);
  auto pub_pad_point_port
    = proc->GetOutputPort("pub_pad_point");
  // do something with output port pub_pad_point
  UNUSED(pub_pad_point_port);
  auto pub_map_info_port
    = proc->GetOutputPort("pub_map_info");
  // do something with output port pub_map_info
  UNUSED(pub_map_info_port);
  auto pub_parking_slot_port
    = proc->GetOutputPort("pub_parking_slot");
  // do something with output port pub_parking_slot
  UNUSED(pub_parking_slot_port);
  auto pub_stio_lane_lines_port
    = proc->GetOutputPort("pub_stio_lane_lines");
  // do something with output port pub_stio_lane_lines
  UNUSED(pub_stio_lane_lines_port);
  auto pub_stop_lines_port
    = proc->GetOutputPort("pub_stop_lines");
  // do something with output port pub_stop_lines
  UNUSED(pub_stop_lines_port);
  auto pub_bumps_port
    = proc->GetOutputPort("pub_bumps");
  // do something with output port pub_bumps
  UNUSED(pub_bumps_port);
  auto pub_crosswalks_port
    = proc->GetOutputPort("pub_crosswalks");
  // do something with output port pub_crosswalks
  UNUSED(pub_crosswalks_port);
  auto pub_arrows_port
    = proc->GetOutputPort("pub_arrows");
  // do something with output port pub_arrows
  UNUSED(pub_arrows_port);
  auto pub_pad_vehicleio_pose_port
    = proc->GetOutputPort("pub_pad_vehicleio_pose");
  // do something with output port pub_pad_vehicleio_pose
  UNUSED(pub_pad_vehicleio_pose_port);
}

void LocationMapModule::TimerProc(
  hobot::dataflow::spMsgResourceProc proc,
  const hobot::dataflow::MessageLists &msgs) {
  UNUSED(msgs);
  // do something with output port pub_location
  auto gen_ts = GetTimeStamp();
  auto loc_status = std::make_shared<LocStatusMsg>();
  loc_status->SetGenTimestamp(gen_ts);

  static int cnt = 0;
  ++cnt;
  cnt = cnt%4;

  loc_status->proto.set_locstatus(static_cast<loc::LocStatusType>(cnt));

  auto pub_loc_status_port
    = proc->GetOutputPort("pub_loc_status");
  if (!pub_loc_status_port) {
    DFHLOG_E("failed to get output port of {}", "pub_loc_status");
    return;
  }
  pub_loc_status_port->Send(loc_status);
  DFHLOG_I("pub_loc_status msg timestamp: {}, loc_status = {}", 
            loc_status->GetGenTimestamp(), loc_status->proto.locstatus());
}

DATAFLOW_REGISTER_MODULE(LocationMapModule)

}  // namespace parking
}  // namespace fanya