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
#include "perception_rd/perception_rd_moudle.h"

#include <functional>
#include <string>
#include <vector>

#include "common/proto_msg_all.h"
#include "common/timestamp.h"
#include "communication/common/types.h"
#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"
#include "message/proto/proto_serializer.hpp"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x

namespace fanya {
namespace parking {

PerceptionRdMoudle::PerceptionRdMoudle(
    const hobot::dataflow::ModuleOption &module_option)
    : hobot::dataflow::Module(module_option) {}

void PerceptionRdMoudle::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_apa_status", aph::apa_status);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_pad_point", loc::padPoint);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_pad_vehicle_pose", loc::padVehiclePose);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_camera_frame_array",
                                camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_ps_info", rd::SApaPSInfo);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_ps_rect", rd::SApaPSRect);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_pointI", rd::SApaPoint_I);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_psd_image", rd::Image);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_ps_info_s32g", rd::SApaPSInfo);

  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_quad_parking_slots_s32g",
                                 rd::QuadParkingSlots);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "MsgCenterProc", PerceptionRdMoudle, MsgCenterProc,
      hobot::dataflow::ProcType::DF_MSG_COND_PROC,
      DF_VECTOR("sub_apa_status", "sub_pad_point", "sub_pad_vehicle_pose",
                "sub_camera_frame_array"),
      DF_VECTOR());
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "TimerProc", PerceptionRdMoudle, TimerProc,
      hobot::dataflow::ProcType::DF_MSG_TIMER_PROC, DF_VECTOR(),
      DF_VECTOR("pub_apa_ps_rect", "pub_apa_ps_info", "pub_apa_pointI",
                "pub_psd_image", "pub_apa_ps_info_s32g",
                "pub_quad_parking_slots_s32g"));
}

int32_t PerceptionRdMoudle::Init() { return 0; }

int32_t PerceptionRdMoudle::Start() {
  return hobot::dataflow::Module::Start();
}

int32_t PerceptionRdMoudle::Stop() {
  return hobot::dataflow::Module::Stop();
}

void PerceptionRdMoudle::Reset() { hobot::dataflow::Module::Reset(); }

int32_t PerceptionRdMoudle::DeInit() {
  return hobot::dataflow::Module::DeInit();
}

void PerceptionRdMoudle::MsgCenterProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs) {
  auto &sub_apa_status_msgs = msgs[proc->GetResultIndex("sub_apa_status")];
  for (auto &msg : *(sub_apa_status_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_apa_status msg timestamp: {}", msg->GetGenTimestamp());
    // process msg of sub_apa_status
    auto apa_status =
        std::dynamic_pointer_cast<ApaStatusMsg>(sub_apa_status_msgs->at(0));
    if (apa_status && apa_status->proto.has_m_current_apa_status()) {
      DFHLOG_I("sub_apa_status msg timestamp: {}, apa_status = {}",
               msg->GetGenTimestamp(),
               apa_status->proto.m_current_apa_status());
    }
  }
  auto &sub_pad_point_msgs = msgs[proc->GetResultIndex("sub_pad_point")];
  for (auto &msg : *(sub_pad_point_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_pad_point msg timestamp: {}", msg->GetGenTimestamp());
    // process msg of sub_pad_point
  }
  auto &sub_pad_vehicle_pose_msgs =
      msgs[proc->GetResultIndex("sub_pad_vehicle_pose")];
  for (auto &msg : *(sub_pad_vehicle_pose_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_pad_vehicle_pose msg timestamp: {}", msg->GetGenTimestamp());
    auto location_msg = std::dynamic_pointer_cast<PadVehiclePoseMsg>(msg);
    if (!location_msg) {
      DFHLOG_C("DetectionProc Empty location msg");
      return;
    } else {
      DFHLOG_W("DetectionProc get location_msg OK.");

      std::cout << "x_position: " << location_msg->proto.coord().x()
                << std::endl;
      std::cout << "y_position: " << location_msg->proto.coord().y()
                << std::endl;
      std::cout << "yaw_position: " << location_msg->proto.yaw() << std::endl;
      // location_msg->proto.status();
      // LOGC("SLOT TYPE: {}, ts = {}",
      // location_msg->proto.slottype(),msg->GetGenTimestamp());
    }

    // process msg of sub_pad_vehicle_pose
  }

  auto &sub_camera_frame_array_msgs =
      msgs[proc->GetResultIndex("sub_camera_frame_array")];
  for (auto &msg : *(sub_camera_frame_array_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_camera_frame_array msg timestamp: {}",
             msg->GetGenTimestamp());
    // process msg of sub_camera_frame_array
  }
}

void PerceptionRdMoudle::TimerProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs) {
  auto gen_ts = GetTimeStamp();
  {  // do something with output port pub_apa_ps_info
    // fill proto
    auto apa_ps_info = std::make_shared<SApaPSInfoMsg>();
    apa_ps_info->proto.set_ullframeid(88);
    apa_ps_info->SetGenTimestamp(gen_ts);
    // pub msg
    auto pub_apa_ps_info_port = proc->GetOutputPort("pub_apa_ps_info");
    if (!pub_apa_ps_info_port) {
      DFHLOG_E("failed to get output port of {}", "pub_apa_ps_info");
      return;
    }

    auto pub_apa_ps_rect_port_s32g =
        proc->GetOutputPort("pub_apa_ps_info_s32g");
    if (!pub_apa_ps_rect_port_s32g) {
      DFHLOG_E("pub_apa_ps_rect_port_s32g failed to get output port of {}",
               "pub_apa_ps_rect");
      return;
    }

    pub_apa_ps_info_port->Send(apa_ps_info);
    pub_apa_ps_rect_port_s32g->Send(apa_ps_info);
    DFHLOG_W("pub pub_apa_ps_info_port info, ullframeid = {}",
             apa_ps_info->proto.ullframeid());
  }

  {  // do something with output port pub_apa_ps_rect
    // fill proto
    auto apa_ps_rect = std::make_shared<SApaPSRectMsg>();
    apa_ps_rect->proto.set_label(88);
    apa_ps_rect->SetGenTimestamp(gen_ts);
    // pub msg
    auto pub_apa_ps_rect_port = proc->GetOutputPort("pub_apa_ps_rect");
    if (!pub_apa_ps_rect_port) {
      DFHLOG_E("failed to get output port of {}", "pub_apa_ps_rect");
      return;
    }

    // auto pub_apa_ps_rect_port_s32g =
    // proc->GetOutputPort("pub_apa_ps_info_s32g"); if
    // (!pub_apa_ps_rect_port_s32g) {
    //   DFHLOG_E("pub_apa_ps_rect_port_s32g failed to get output port of {}",
    //   "pub_apa_ps_rect"); return;
    // }

    pub_apa_ps_rect_port->Send(apa_ps_rect);
    // pub_apa_ps_rect_port_s32g->Send(apa_ps_rect);
    DFHLOG_I("pub apa_ps_rect_port info, label = {}",
             apa_ps_rect->proto.label());
  }

  {  // do something with output port pub_apa_pointI
    // fill proto
    auto apa_pointI = std::make_shared<SApaPoint_IMsg>();
    apa_pointI->proto.set_x(88);
    apa_pointI->SetGenTimestamp(gen_ts);
    // pub msg
    auto pub_apa_pointI_port = proc->GetOutputPort("pub_apa_pointI");
    if (!pub_apa_pointI_port) {
      DFHLOG_E("failed to get output port of {}", "pub_apa_pointI");
      return;
    }
    pub_apa_pointI_port->Send(apa_pointI);
    DFHLOG_I("pub apa_pointI_port info, x = {}", apa_pointI->proto.x());
  }


   { // do something with output port pub_psd_image
        // fill proto
        auto rd_image = std::make_shared<ImageMsg>();
        // image->proto.set_width(1920);
        // image->SetGenTimestamp(gen_ts);

        std::vector<uint8_t> image_data;
        static cv::Mat image = cv::imread("test.jpg");
        imencode(".jpg", image, image_data);
        rd::Time rd_time;
        rd_time.set_nanosec(123);
        rd::Header rd_header;
        rd_header.set_seq(1);
        rd_header.set_frameid("99");
        rd_header.mutable_timestampns()->CopyFrom(rd_time);
        // rd::Image rd_image;
        rd_image->proto.set_data(image_data.data(), image_data.size());
        rd_image->proto.mutable_header()->CopyFrom(rd_header);
        rd_image->proto.set_oriheight(123);
        rd_image->proto.set_oriwidth(123);
        rd_image->proto.set_height(image.rows);
        rd_image->proto.set_width(image.cols);
        rd_image->proto.set_encoding("jpg");
        rd_image->proto.set_step(123);
        // rd_image->proto.mutable_data()->CopyFrom(rd_data);
        rd_image->proto.set_phyaddr(123);
        rd_image->proto.set_viraddr(123);
        rd_image->proto.set_memtype(rd::Memtype::cambriconVgu);

        // pub msg
        auto pub_image_port = proc->GetOutputPort("pub_psd_image");
        if (!pub_image_port)
        {
          DFHLOG_E("failed to get output port of {}", "pub_psd_image");
          return;
        }
        pub_image_port->Send(rd_image);
        DFHLOG_I("pub image_port info, width = {}",
                 rd_image->proto.width());
      }

  // pub_quad_parking_slots_s32g
  {
    // fill proto
    rd::Time rd_time;
    rd_time.set_nanosec(123);
    rd::Header rd_header;
    rd_header.set_seq(1);
    rd_header.set_frameid("99");
    rd_header.mutable_timestampns()->CopyFrom(rd_time);

    rd::Point2f rd_point2f;
    rd_point2f.set_x(1);
    rd_point2f.set_y(1);

    rd::ApproxBoxPoints rd_approx_box_points;
    rd_approx_box_points.mutable_point()->CopyFrom(rd_point2f);
    rd_approx_box_points.set_borderdist(123);
    rd_approx_box_points.set_pointscore(123);
    rd_approx_box_points.set_linelen(123);
    rd_approx_box_points.set_linescore(123);
    rd_approx_box_points.set_hasborderpoint(true);

    rd::QuadParkingSlot rd_quad_parking_slot;
    rd_quad_parking_slot.mutable_tl()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.mutable_tr()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.mutable_bl()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.mutable_br()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.set_confidence(123);
    rd_quad_parking_slot.set_label(123);
    rd_quad_parking_slot.set_filtered(true);
    rd_quad_parking_slot.set_slottype(123);
    rd_quad_parking_slot.set_stl(123);
    rd_quad_parking_slot.set_str(123);
    rd_quad_parking_slot.set_sbl(123);
    rd_quad_parking_slot.set_sbr(123);
    rd_quad_parking_slot.mutable_dirin()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.mutable_dirwidth()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.mutable_dirlength()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.mutable_center()->CopyFrom(rd_point2f);
    rd_quad_parking_slot.set_oppmodify(true);
    rd_quad_parking_slot.set_iscomplete(true);
    rd_quad_parking_slot.set_isvisited(true);
    rd_quad_parking_slot.set_width(123);
    rd_quad_parking_slot.set_length(123);

    auto quad_parking_slots_msg = std::make_shared<QuadParkingSlotsMsg>();
    quad_parking_slots_msg->proto.mutable_header()->CopyFrom(rd_header);
    quad_parking_slots_msg->proto.set_frametimestampns(99);
    quad_parking_slots_msg->proto.set_sensorid(0);
    quad_parking_slots_msg->proto.add_quadparkingslotlist()->CopyFrom(
        rd_quad_parking_slot);

    // pub msg
    auto pub_quad_parking_slots_s32g =
        proc->GetOutputPort("pub_quad_parking_slots_s32g");
    if (!pub_quad_parking_slots_s32g) {
      DFHLOG_E("failed to get output port of {}", "pub_psd_image");
      return;
    }
    pub_quad_parking_slots_s32g->Send(quad_parking_slots_msg);
    DFHLOG_W("pub_quad_parking_slots_s32g , Success!!! ");
  }
}

DATAFLOW_REGISTER_MODULE(PerceptionRdMoudle)

}  // namespace parking
}  // namespace fanya