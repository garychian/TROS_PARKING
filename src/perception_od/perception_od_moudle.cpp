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
#include "perception_od/perception_od_moudle.h"
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

namespace fanya
{
  namespace parking
  {

    PerceptionOdMoudle::PerceptionOdMoudle(
        const hobot::dataflow::ModuleOption &module_option) : hobot::dataflow::Module(module_option)
    {
    }

    void PerceptionOdMoudle::InitPortsAndProcs()
    {
      DF_MODULE_INIT_IDL_INPUT_PORT(
          "sub_apa_status",
          aph::apa_status);
      DF_MODULE_INIT_IDL_INPUT_PORT(
          "sub_camera_frame_array",
          camera_frame::CameraFrameArray);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_obstacles",
          od::Obstacles);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_fsline_msg",
          od::FSLine);
      DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
          "MsgCenterProc",
          PerceptionOdMoudle,
          MsgCenterProc,
          hobot::dataflow::ProcType::DF_MSG_COND_PROC,
          DF_VECTOR("sub_apa_status", "sub_camera_frame_array"),
          DF_VECTOR());
      DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
          "TimerProc",
          PerceptionOdMoudle,
          TimerProc,
          hobot::dataflow::ProcType::DF_MSG_TIMER_PROC,
          DF_VECTOR(),
          DF_VECTOR("pub_obstacles", "pub_fsline_msg"));
    }

    int32_t PerceptionOdMoudle::Init()
    {
      return 0;
    }

    int32_t PerceptionOdMoudle::Start()
    {
      return hobot::dataflow::Module::Start();
    }

    int32_t PerceptionOdMoudle::Stop()
    {
      return hobot::dataflow::Module::Stop();
    }

    void PerceptionOdMoudle::Reset()
    {
      hobot::dataflow::Module::Reset();
    }

    int32_t PerceptionOdMoudle::DeInit()
    {
      return hobot::dataflow::Module::DeInit();
    }

    void PerceptionOdMoudle::MsgCenterProc(
        hobot::dataflow::spMsgResourceProc proc,
        const hobot::dataflow::MessageLists &msgs)
    {
      auto &sub_apa_status_msgs = msgs[proc->GetResultIndex("sub_apa_status")];
      for (auto &msg : *(sub_apa_status_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("sub_apa_status msg timestamp: {}",
                 msg->GetGenTimestamp());
        // process msg of sub_apa_status
        auto apa_status = std::dynamic_pointer_cast<ApaStatusMsg>(sub_apa_status_msgs->at(0));
        if (apa_status && apa_status->proto.has_m_current_apa_status())
        {
          DFHLOG_I("sub_apa_status msg timestamp: {}, apa_status = {}",
                   msg->GetGenTimestamp(), apa_status->proto.m_current_apa_status());
        }
      }

      auto &sub_camera_frame_array_msgs = msgs[proc->GetResultIndex("sub_camera_frame_array")];
      for (auto &msg : *(sub_camera_frame_array_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("sub_camera_frame_array msg timestamp: {}",
                 msg->GetGenTimestamp());
        // process msg of sub_camera_frame_array
      }
    }

    void PerceptionOdMoudle::TimerProc(
        hobot::dataflow::spMsgResourceProc proc,
        const hobot::dataflow::MessageLists &msgs)
    {
      auto gen_ts = GetTimeStamp();

      // pub ObstaclesMsg
      {
        // fill proto
        od::Time od_time;
        od_time.set_nanosec(123);
        od::Header od_header;
        od_header.set_seq(1);
        od_header.set_frameid("99");
        od_header.mutable_timestampns()->CopyFrom(od_time);

        od::Vector3d od_vector3d;
        od_vector3d.set_x(1);
        od_vector3d.set_y(1);
        od_vector3d.set_z(1);

        od::Point2f od_point2f;
        od_point2f.set_x(1);
        od_point2f.set_y(1);
        od::Vector3f od_vector3f;
        od_vector3f.set_x(1);
        od_vector3f.set_y(1);
        od_vector3f.set_z(1);
        od::Matrix3f od_matrix3f;
        od_matrix3f.mutable_x()->CopyFrom(od_vector3f);
        od_matrix3f.mutable_y()->CopyFrom(od_vector3f);
        od_matrix3f.mutable_z()->CopyFrom(od_vector3f);

        od::MotionInfo od_motionInfo;
        od_motionInfo.set_isvalid(true);
        od_motionInfo.set_motionstatus(1);
        od_motionInfo.mutable_center()->CopyFrom(od_vector3d);
        od_motionInfo.mutable_centeruncertainty()->CopyFrom(od_matrix3f);
        
        od::ObstacleTrajectoryPoint od_obstacleTrajectoryPoint;
        od_obstacleTrajectoryPoint.set_deltatns(123);
        od_obstacleTrajectoryPoint.mutable_center()->CopyFrom(od_vector3f);
        od_obstacleTrajectoryPoint.mutable_direction()->CopyFrom(od_vector3f);
        
        od::ObstacleTrajectory od_obstacleTrajectory;
        od_obstacleTrajectory.set_motionstatus(od::MotionStatus::Unknown);
        od_obstacleTrajectory.set_confidence(123);
        od_obstacleTrajectory.add_points()->CopyFrom(od_obstacleTrajectoryPoint);

        od::ObstacleCylindrical od_obstacleCylindrical;
        od_obstacleCylindrical.set_camera("camera_name_test");
        od_obstacleCylindrical.set_label(123);
        od_obstacleCylindrical.set_typeconfidence(123);
        od_obstacleCylindrical.set_existenceconfidence(123);
        od_obstacleCylindrical.set_trackid(123);
        od_obstacleCylindrical.mutable_center()->CopyFrom(od_vector3d);
        od_obstacleCylindrical.set_radius(123);
        od_obstacleCylindrical.set_height(123);
        od_obstacleCylindrical.mutable_direction()->CopyFrom(od_vector3d);
        od_obstacleCylindrical.mutable_motioninfo()->CopyFrom(od_motionInfo);
        od_obstacleCylindrical.add_trajectory()->CopyFrom(od_obstacleTrajectory);
        od_obstacleCylindrical.set_staticstate(123);
        od_obstacleCylindrical.set_attrscore(123);

        od::ObstacleCube od_obstacleCube;
        od_obstacleCube.set_camera("camera_name_test");
        od_obstacleCube.set_label(123);
        od_obstacleCube.set_typeconfidence(123);
        od_obstacleCube.set_existenceconfidence(123);
        od_obstacleCube.set_trackid(123);
        od_obstacleCube.mutable_center()->CopyFrom(od_vector3d); 
        od_obstacleCube.mutable_shape()->CopyFrom(od_vector3d); 
        od_obstacleCube.mutable_direction()->CopyFrom(od_vector3d);
        od_obstacleCube.mutable_motioninfo()->CopyFrom(od_motionInfo);
        od_obstacleCube.add_trajectory()->CopyFrom(od_obstacleTrajectory);

        od::Bbox2D od_bbox2D;
        od_bbox2D.set_initialized(true);
        od_bbox2D.set_topleftx(123);
        od_bbox2D.set_toplefty(123);
        od_bbox2D.set_bottomrightx(123);
        od_bbox2D.set_bottomrighty(123);
        od_bbox2D.set_confidence(123);

        od::DirectionInfo od_directionInfo;
        od_directionInfo.set_isvalid(true);
        od_directionInfo.mutable_directionypr()->CopyFrom(od_vector3f);
        od_directionInfo.mutable_directionypruncertainty()->CopyFrom(od_matrix3f);
        od_directionInfo.set_yaw(123);
        od_directionInfo.set_yawuncertainty(123);
        od_directionInfo.set_yawrate(123);
        od_directionInfo.set_yawrateuncertainty(123);
        od_directionInfo.set_surface(123);
        od_directionInfo.set_surfacescore(123);

        od::PositionInfo od_positionInfo;
        od_positionInfo.set_isvalid(true);
        od_positionInfo.mutable_position()->CopyFrom(od_vector3f);

        od::ObstacleRaw od_obstacleRaw;
        od_obstacleRaw.set_camera("camera_name_test");
        od_obstacleRaw.set_label(123);
        od_obstacleRaw.set_typeconfidence(123);
        od_obstacleRaw.set_existenceconfidence(123);

        od_obstacleRaw.mutable_box2d()->CopyFrom(od_bbox2D);

        od_obstacleRaw.mutable_directioninfo()->CopyFrom(od_directionInfo); 
        od_obstacleRaw.add_landmark()->CopyFrom(od_point2f);
        od_obstacleRaw.add_landmarkscores(123);
        od_obstacleRaw.add_landmark4()->CopyFrom(od_point2f);
        od_obstacleRaw.add_landmark4scores(123);
        od_obstacleRaw.add_landmark9()->CopyFrom(od_point2f);
        od_obstacleRaw.add_landmark9scores(123);
        od_obstacleRaw.mutable_positioninfo()->CopyFrom(od_positionInfo);
        od_obstacleRaw.add_box3d(123);
        od_obstacleRaw.set_staticstate(123);
        od_obstacleRaw.set_attrscore(123);

        od::UssRange od_ussRange;
        od_ussRange.set_distanceid("123");
        od_ussRange.set_recvtimens(123);
        od_ussRange.set_distance(123);

        //create msg
        auto obstacles_msg = std::make_shared<ObstaclesMsg>();
        obstacles_msg->proto.mutable_header()->CopyFrom(od_header);
        obstacles_msg->proto.add_cylindricalobstalces()->CopyFrom(od_obstacleCylindrical);
        obstacles_msg->proto.add_cubeobstacles()->CopyFrom(od_obstacleCube);
        obstacles_msg->proto.add_rawobjects()->CopyFrom(od_obstacleRaw);
        obstacles_msg->proto.add_rangeunitarray()->CopyFrom(od_ussRange);
        obstacles_msg->SetGenTimestamp(gen_ts);

        auto pub_obstacles_s32g = proc->GetOutputPort("pub_obstacles");
        if (!pub_obstacles_s32g)
        {
          DFHLOG_E("pub_obstacles_s32g failed to get output port of {}", "pub_obstacles");
          return;
        }
        // send to j5 

        // send to s32g
        pub_obstacles_s32g->Send(obstacles_msg);
        auto obstacles_proto = obstacles_msg->proto;
        auto temp_header = obstacles_proto.mutable_header();
        DFHLOG_W("pub pub_obstacles_s32g info, obstacles header frame id = {}",
                 temp_header->frameid());
      }

      // pub FSLineMsg
      {
        // fill proto
        od::Point2f od_point2f;
        od_point2f.set_x(1);
        od_point2f.set_y(1);

        od::Time od_time;
        od_time.set_nanosec(123);

        od::FSLinePoint od_fsline_point;
        od_fsline_point.mutable_coordinate()->CopyFrom(od_point2f);
        od_fsline_point.set_pointlabel(od::SpaceLabel::vehicle);

        od::Header od_header;
        od_header.set_seq(1);
        od_header.set_frameid("99");
        od_header.mutable_timestampns()->CopyFrom(od_time);

        od::FSLinesimple od_fsline_simple;
        od_fsline_simple.mutable_header()->CopyFrom(od_header);
        od_fsline_simple.set_frametimestampns(123);
        od_fsline_simple.add_fslinepoints()->CopyFrom(od_fsline_point);

        auto fsline_msg = std::make_shared<FSLineMsg>();
        fsline_msg->proto.add_fsline()->CopyFrom(od_fsline_simple);
        fsline_msg->SetGenTimestamp(gen_ts);
        auto pub_fsline_msg_port_s32g = proc->GetOutputPort("pub_fsline_msg");
        if (!pub_fsline_msg_port_s32g)
        {
          DFHLOG_E("failed to get output port of {}", "pub_fsline_msg");
          return;
        }
        pub_fsline_msg_port_s32g->Send(fsline_msg);
        DFHLOG_W("Pub od_fsline_msg,Success!!! ");
      }

    }

    DATAFLOW_REGISTER_MODULE(PerceptionOdMoudle)

  } // namespace parking
} // namespace fanya