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
    can::VehicleCanData);
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
    DF_VECTOR());

  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
    "TimerProc",
    LocationMapModule,
    TimerProc,
    hobot::dataflow::ProcType::DF_MSG_TIMER_PROC,
    DF_VECTOR(),
    DF_VECTOR("pub_pad_realtime_loc", "pub_loc_status", "pub_vehicle_pose", "pub_app2emap", "pub_pad_point", "pub_map_info", "pub_parking_slot", "pub_stio_lane_lines", "pub_stop_lines", "pub_bumps", "pub_crosswalks", "pub_arrows", "pub_pad_vehicleio_pose"));
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
    DFHLOG_I("sub_vehicleio_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_vehicleio_data
    auto can_msg = std::dynamic_pointer_cast<CanSignalMsg>(msg);
    DFHLOG_I("speed: {}", can_msg->proto.vehspdavgndrvn());
    //auto gear_type = static_cast<can::TrnsShftLvrPos_TCM>(can_msg->proto.trnsshftlvrpos_tcm());
    DFHLOG_I("gear: {}", can_msg->proto.trnsshftlvrpos_tcm());
    DFHLOG_I("WheelAngle: {}",can_msg->proto.whlangvellrrauth());
    DFHLOG_I("yawrate: {}",can_msg->proto.imuyawrtpri());
    DFHLOG_I("IMULatACCSec: {}",can_msg->proto.imulataccsec());
    DFHLOG_I("IMUYawRtSec: {}",can_msg->proto.imuyawrtsec());
    DFHLOG_I("IMULonAccSec: {}",can_msg->proto.imulonaccsec());
    DFHLOG_I("IIMULatAccPrim: {}",can_msg->proto.iimulataccprim());
    DFHLOG_I("IMULonAccPri: {}",can_msg->proto.imulonaccpri());
    DFHLOG_I("FLWheelSpeedInkph: {}",can_msg->proto.whlangvellfrtauth());
    DFHLOG_I("FRwheelSpeedInkph: {}",can_msg->proto.whlangvellrrauth());
    DFHLOG_I("RLwheelSpeedInkph: {}",can_msg->proto.whlangvelrfrtauth());
    DFHLOG_I("RRwheelSpeedInkph: {}",can_msg->proto.whlangvelrrrauth());
    DFHLOG_I("FLWheelSpeedPulse: {}",can_msg->proto.whldistedgecntrlfhigfreq());
    DFHLOG_I("FRwheelSpeedPulse: {}",can_msg->proto.whldistedgecntrlrhigfreq());
    DFHLOG_I("RLwheelSpeedPulse: {}",can_msg->proto.whldistedgecntrrfhigfreq());
    DFHLOG_I("RRwheelSpeedPulse: {}",can_msg->proto.whldistedgecntrrrhigfreq());
  }
  auto &sub_apa_status_msgs
    = msgs[proc->GetResultIndex("sub_apa_status")];
  for (auto &msg : *(sub_apa_status_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_apa_status msg timestamp: {}",
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
    DFHLOG_I("sub_target_slot msg timestamp: {}",
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
    DFHLOG_I("sub_imu_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_imu_data
  }
  auto &sub_gnss_data_msgs
    = msgs[proc->GetResultIndex("sub_gnss_data")];
  for (auto &msg : *(sub_gnss_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_gnss_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_gnss_data
  }
  auto &sub_ins_data_msgs
    = msgs[proc->GetResultIndex("sub_ins_data")];
  for (auto &msg : *(sub_ins_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_ins_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_ins_data
  }
  auto &sub_dual_antenna_data_msgs
    = msgs[proc->GetResultIndex("sub_dual_antenna_data")];
  for (auto &msg : *(sub_dual_antenna_data_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_dual_antenna_data msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_dual_antenna_data
  }
  auto &sub_psd_image_msgs
    = msgs[proc->GetResultIndex("sub_psd_image")];
  for (auto &msg : *(sub_psd_image_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_psd_image msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_psd_image
  }
  auto &sub_camera_frame_array_msgs
    = msgs[proc->GetResultIndex("sub_camera_frame_array")];
  for (auto &msg : *(sub_camera_frame_array_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_camera_frame_array msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_camera_frame_array
  }
  auto &sub_apa_ps_info_msgs
    = msgs[proc->GetResultIndex("sub_apa_ps_info")];
  for (auto &msg : *(sub_apa_ps_info_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_W("sub_apa_ps_info msg timestamp: {}",
      msg->GetGenTimestamp());
      auto location_msg = std::dynamic_pointer_cast<SApaPSInfoMsg>(msg);
      std::cout<< "locationmsg:*************"<< location_msg->proto.ullframeid()<<std::endl;

    // process msg of sub_apa_ps_info
  }
  auto &sub_apa_ps_rect_msgs
    = msgs[proc->GetResultIndex("sub_apa_ps_rect")];
  for (auto &msg : *(sub_apa_ps_rect_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_apa_ps_rect msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_ps_rect
  }
  auto &sub_apa_pointI_msgs
    = msgs[proc->GetResultIndex("sub_apa_pointI")];
  for (auto &msg : *(sub_apa_pointI_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_apa_pointI msg timestamp: {}",
      msg->GetGenTimestamp());
    // process msg of sub_apa_pointI
  }
}

void LocationMapModule::TimerProc(
  hobot::dataflow::spMsgResourceProc proc,
  const hobot::dataflow::MessageLists &msgs) {
  UNUSED(msgs);
  auto gen_ts = GetTimeStamp();
  {// do something with output port pub_loc_status
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

  {// do something with output port pub_pad_realtime_loc
    // fill proto
    auto pad_realtime_loc = std::make_shared<PadRealTimeLocationMsg>();
    pad_realtime_loc->proto.set_reserve(88);
    pad_realtime_loc->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_pad_realtime_loc_port = proc->GetOutputPort("pub_pad_realtime_loc");
    if (!pub_pad_realtime_loc_port) {
      DFHLOG_E("failed to get output port of {}", "pub_pad_realtime_loc");
      return;
    }
    pub_pad_realtime_loc_port->Send(pad_realtime_loc);
    DFHLOG_I("pub pad_realtime_loc_port info, reserve = {}",
                          pad_realtime_loc->proto.reserve());
  }

  {// do something with output port pub_vehicle_pose
    // fill proto
    auto vehicle_pose = std::make_shared<VehiclePosMsg>();
    vehicle_pose->proto.set_x(1920);
    vehicle_pose->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_vehicle_pose_port = proc->GetOutputPort("pub_vehicle_pose");
    if (!pub_vehicle_pose_port) {
      DFHLOG_E("failed to get output port of {}", "pub_vehicle_pose");
      return;
    }
    pub_vehicle_pose_port->Send(vehicle_pose);
    DFHLOG_I("pub vehicle_pose_port info, x = {}",
                          vehicle_pose->proto.x());
  }

  {// do something with output port pub_app2emap
    // fill proto
    auto app2emap = std::make_shared<App2emap_DRMsg>();
    app2emap->proto.set_x(1920);
    app2emap->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_app2emap_port = proc->GetOutputPort("pub_app2emap");
    if (!pub_app2emap_port) {
      DFHLOG_E("failed to get output port of {}", "pub_app2emap");
      return;
    }
    pub_app2emap_port->Send(app2emap);
    DFHLOG_I("pub app2emap_port info, x = {}",
                          app2emap->proto.x());
  }

  {// do something with output port pub_pad_point
    // fill proto
    auto pad_point = std::make_shared<PadPointMsg>();
    pad_point->proto.set_x(1920);
    pad_point->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_pad_point_port = proc->GetOutputPort("pub_pad_point");
    if (!pub_pad_point_port) {
      DFHLOG_E("failed to get output port of {}", "pub_pad_point");
      return;
    }
    pub_pad_point_port->Send(pad_point);
    DFHLOG_I("pub pad_point_port info, x = {}",
                          pad_point->proto.x());
  }

  {// do something with output port pub_pad_vehicleio_pose
    // fill proto
    auto pad_vehicleio_pose = std::make_shared<PadVehiclePoseMsg>();
    pad_vehicleio_pose->proto.set_status(88);
    pad_vehicleio_pose->proto.set_yaw(123);
    
   auto pad_point = std::make_shared<PadPointMsg>();
   // pad_point ->proto.set_x(100);
    //pad_point ->proto.set_y(200);
   // auto coord = pad_vehicleio_pose->proto.coord(); 
   // pad_vehicleio_pose->proto.set_coord(coord);
   loc::padPoint *temp =   pad_vehicleio_pose->proto.mutable_coord();
    temp ->set_x(2);
    temp ->set_y(3);

   // pad_vehicleio_pose->proto.set_allocated_coord(temp);
    pad_vehicleio_pose->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_pad_vehicleio_pose_port = proc->GetOutputPort("pub_pad_vehicleio_pose");
    if (!pub_pad_vehicleio_pose_port) {
      DFHLOG_E("failed to get output port of {}", "pub_pad_vehicleio_pose");
      return;
    }
    pub_pad_vehicleio_pose_port->Send(pad_vehicleio_pose);
    DFHLOG_I("pub pad_vehicleio_pose_port info, status = {} , pub_pad_vehicleio_pose_port info x={}",
                          pad_vehicleio_pose->proto.status(), pad_vehicleio_pose->proto.coord().x());
  }

  {// do something with output port pub_map_info
    // fill proto
    auto map_info = std::make_shared<MapInfoMsg>();
    map_info->proto.set_mapid(88);
    map_info->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_map_info_port = proc->GetOutputPort("pub_map_info");
    if (!pub_map_info_port) {
      DFHLOG_E("failed to get output port of {}", "pub_map_info");
      return;
    }
    pub_map_info_port->Send(map_info);
    DFHLOG_I("pub map_info_port info, mapid = {}",
                          map_info->proto.mapid());
  }

  {// do something with output port pub_parking_slot
    // fill proto
    auto parking_slot = std::make_shared<ParkingSlotsMsg>();
    parking_slot->proto.set_width(1920);
    parking_slot->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_parking_slot_port = proc->GetOutputPort("pub_parking_slot");
    if (!pub_parking_slot_port) {
      DFHLOG_E("failed to get output port of {}", "pub_parking_slot");
      return;
    }
    pub_parking_slot_port->Send(parking_slot);
    DFHLOG_I("pub parking_slot_port info, width = {}",
                          parking_slot->proto.width());
  }

  {// do something with output port pub_stio_lane_lines
    // fill proto
    auto stio_lane_lines = std::make_shared<LaneLinesMsg>();
    stio_lane_lines->proto.set_linetype(88);
    stio_lane_lines->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_stio_lane_lines_port = proc->GetOutputPort("pub_stio_lane_lines");
    if (!pub_stio_lane_lines_port) {
      DFHLOG_E("failed to get output port of {}", "pub_stio_lane_lines");
      return;
    }
    pub_stio_lane_lines_port->Send(stio_lane_lines);
    DFHLOG_I("pub stio_lane_lines_port info, linetype = {}",
                          stio_lane_lines->proto.linetype());
  }

  {// do something with output port pub_stop_lines
    // fill proto
    auto stop_lines = std::make_shared<StopLinesMsg>();
    stop_lines->proto.set_id(88);
    stop_lines->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_stop_lines_port = proc->GetOutputPort("pub_stop_lines");
    if (!pub_stop_lines_port) {
      DFHLOG_E("failed to get output port of {}", "pub_stop_lines");
      return;
    }
    pub_stop_lines_port->Send(stop_lines);
    DFHLOG_I("pub stop_lines_port info, id = {}",
                          stop_lines->proto.id());
  }

  {// do something with output port pub_bumps
    // fill proto
    auto bumps = std::make_shared<BumpsMsg>();
    bumps->proto.set_id(88);
    bumps->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_bumps_port = proc->GetOutputPort("pub_bumps");
    if (!pub_bumps_port) {
      DFHLOG_E("failed to get output port of {}", "pub_bumps");
      return;
    }
    pub_bumps_port->Send(bumps);
    DFHLOG_I("pub bumps_port info, id = {}",
                          bumps->proto.id());
  }

  {// do something with output port pub_crosswalks
    // fill proto
    auto crosswalks = std::make_shared<CrossWalksMsg>();
    crosswalks->proto.set_id(88);
    crosswalks->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_crosswalks_port = proc->GetOutputPort("pub_crosswalks");
    if (!pub_crosswalks_port) {
      DFHLOG_E("failed to get output port of {}", "pub_crosswalks");
      return;
    }
    pub_crosswalks_port->Send(crosswalks);
    DFHLOG_I("pub crosswalks_port info, id = {}",
                          crosswalks->proto.id());
  }

  {// do something with output port pub_arrows
    // fill proto
    auto arrows = std::make_shared<ArrowsMsg>();
    arrows->proto.set_id(88);
    arrows->SetGenTimestamp(gen_ts);
    //pub msg
    auto pub_arrows_port = proc->GetOutputPort("pub_arrows");
    if (!pub_arrows_port) {
      DFHLOG_E("failed to get output port of {}", "pub_arrows");
      return;
    }
    pub_arrows_port->Send(arrows);
    DFHLOG_I("pub arrows_port info, id = {}",
                          arrows->proto.id());
  }
}

DATAFLOW_REGISTER_MODULE(LocationMapModule)

}  // namespace parking
}  // namespace fanya