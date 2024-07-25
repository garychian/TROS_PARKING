// Copyright (c) [2021-2023] [Horizon Robotics].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#pragma once
#include <memory>
#include <string>
#include <utility>

#include "communication/common/types.h"
#include "sensor_center/utils/camera_msg_convert.hpp"
#include "message/message.h"
#include "message/proto/proto_serializer.hpp"
#include "opencv2/opencv.hpp"
#include "protocol/camera_frame.pb.h"
#include "protocol/vehicleiostate.pb.h"

#include "protocol/image.pb.h"
#include "protocol/target_message.pb.h"

#include "fanya_protocol/loc.pb.h"
#include "fanya_protocol/fanya_can.pb.h"
#include "fanya_protocol/rd.pb.h"
#include "fanya_protocol/aph.pb.h"
#include "fanya_protocol/fsm.pb.h"
#include "fanya_protocol/s32g.pb.h"
#include "fanya_protocol/sen.pb.h"
#include "fanya_protocol/od.pb.h"
namespace fanya {

namespace parking {

using ImageProtoMsg = hobot::message::ProtoMsg<ImageProto::Image>;
using spImageProtoMsg = std::shared_ptr<ImageProtoMsg>;

using CameraProtoMsg = hobot::message::ProtoMsg<camera_frame::CameraFrame>;
using spCameraProtoMsg = std::shared_ptr<CameraProtoMsg>;

using spCameraFrameArrayProtoMsg = std::shared_ptr<CameraFrameArrayProtoMsg>;

using ApaStatusMsg = hobot::message::ProtoMsg<aph::apa_status>;
using spApaStatusMsg = std::shared_ptr<ApaStatusMsg>;

using TargetSlotMsg = hobot::message::ProtoMsg<aph::target_slot>;
using spTargetSlotMsg = std::shared_ptr<TargetSlotMsg>;

using SlotlabelMsg = hobot::message::ProtoMsg<fsm::Slotlabel>;
using spSlotlabelMsg = std::shared_ptr<SlotlabelMsg>;

using LocStatusMsg = hobot::message::ProtoMsg<loc::LocStatus>;
using spLocStatusMsg = std::shared_ptr<LocStatusMsg>;

using SApaPSInfoMsg = hobot::message::ProtoMsg<rd::SApaPSInfo>;
using spSApaPSInfoMsg = std::shared_ptr<SApaPSInfoMsg>;

using SApaPSRectMsg = hobot::message::ProtoMsg<rd::SApaPSRect>;
using spSApaPSRectMsg = std::shared_ptr<SApaPSRectMsg>;

using SApaPoint_IMsg = hobot::message::ProtoMsg<rd::SApaPoint_I>;
using spSApaPoint_IMsg = std::shared_ptr<SApaPoint_IMsg>;

using ImageMsg = hobot::message::ProtoMsg<rd::Image>;
using spImageMsg = std::shared_ptr<ImageMsg>;

using PadRealTimeLocationMsg = hobot::message::ProtoMsg<loc::padRealTimeLocation>;
using spPadRealTimeLocationMsg = std::shared_ptr<PadRealTimeLocationMsg>;

using VehiclePosMsg = hobot::message::ProtoMsg<loc::VehiclePos>;
using spVehiclePosMsg = std::shared_ptr<VehiclePosMsg>;

using App2emap_DRMsg = hobot::message::ProtoMsg<loc::App2emap_DR>;
using spApp2emap_DRMsg = std::shared_ptr<App2emap_DRMsg>;

using PadPointMsg = hobot::message::ProtoMsg<loc::padPoint>;
using spPadPointMsg = std::shared_ptr<PadPointMsg>;

using PadVehiclePoseMsg = hobot::message::ProtoMsg<loc::padVehiclePose>;
using spPadVehiclePoseMsg = std::shared_ptr<PadVehiclePoseMsg>;

using MapInfoMsg = hobot::message::ProtoMsg<loc::MapInfo>;
using spMapInfoMsg = std::shared_ptr<MapInfoMsg>;

using ParkingSlotsMsg = hobot::message::ProtoMsg<loc::parkingSlots>;
using spParkingSlotsMsg = std::shared_ptr<ParkingSlotsMsg>;

using LaneLinesMsg = hobot::message::ProtoMsg<loc::laneLines>;
using spLaneLinesMsg = std::shared_ptr<LaneLinesMsg>;

using StopLinesMsg = hobot::message::ProtoMsg<loc::stopLines>;
using spStopLinesMsg = std::shared_ptr<StopLinesMsg>;

using BumpsMsg = hobot::message::ProtoMsg<loc::bumps>;
using spBumpsMsg = std::shared_ptr<BumpsMsg>;

using CrossWalksMsg = hobot::message::ProtoMsg<loc::crossWalks>;
using spCrossWalksMsg = std::shared_ptr<CrossWalksMsg>;

using ArrowsMsg = hobot::message::ProtoMsg<loc::arrows>;
using spArrowsMsg = std::shared_ptr<ArrowsMsg>;

using CanSignalMsg = hobot::message::ProtoMsg<can::VehicleCanData>;
using spCanSignalMsg = std::shared_ptr<CanSignalMsg>;
//add od 
using ObstaclesMsg = hobot::message::ProtoMsg<od::Obstacles>;
using spObstaclesMsg = std::shared_ptr<ObstaclesMsg>;

using HeaderMsg = hobot::message::ProtoMsg<od::Header>;
using spHeaderMsg = std::shared_ptr<HeaderMsg>;

using TimeMsg = hobot::message::ProtoMsg<od::Time>;
using spTimeMsg = std::shared_ptr<TimeMsg>;

using ObstacleCylindricalMsg = hobot::message::ProtoMsg<od::ObstacleCylindrical>;
using spObstacleCylindricalMsg = std::shared_ptr<ObstacleCylindricalMsg>;

using Vector3dMsg = hobot::message::ProtoMsg<od::Vector3d>;
using spVector3d = std::shared_ptr<Vector3dMsg>;

using MotionInfoMsg = hobot::message::ProtoMsg<od::MotionInfo>;
using spMotionInfo = std::shared_ptr<MotionInfoMsg>;

using Point2fMsg = hobot::message::ProtoMsg<od::Point2f>;
using spPoint2fMsg = std::shared_ptr<Point2fMsg>;

using Vector3fMsg = hobot::message::ProtoMsg<od::Vector3f>;
using spVector3fMsg = std::shared_ptr<Vector3fMsg>;

using Matrix3fMsg = hobot::message::ProtoMsg<od::Matrix3f>;
using spMatrix3fMsg = std::shared_ptr<Matrix3fMsg>;

using ObstacleCubeMsg = hobot::message::ProtoMsg<od::ObstacleCube>;
using spObstacleCubeMsg = std::shared_ptr<ObstacleCubeMsg>;

using ObstacleTrajectoryMsg = hobot::message::ProtoMsg<od::ObstacleTrajectory>;
using spObstacleTrajectoryMsg = std::shared_ptr<ObstacleTrajectoryMsg>;

using ObstacleTrajectoryPointMsg = hobot::message::ProtoMsg<od::ObstacleTrajectoryPoint>;
using spObstacleTrajectoryPointMsg = std::shared_ptr<ObstacleTrajectoryPointMsg>;

using ObstacleRawMsg = hobot::message::ProtoMsg<od::ObstacleRaw>;
using spObstacleRawMsg = std::shared_ptr<ObstacleRawMsg>;

using Bbox2DMsg = hobot::message::ProtoMsg<od::Bbox2D>;
using spBbox2DMsg = std::shared_ptr<Bbox2DMsg>;

using DirectionInfoMsg = hobot::message::ProtoMsg<od::DirectionInfo>;
using spDirectionInfoMsg = std::shared_ptr<DirectionInfoMsg>;

using UssRangeMsg = hobot::message::ProtoMsg<od::UssRange>;
using spUssRangeMsg = std::shared_ptr<UssRangeMsg>;

using PositionInfoMsg = hobot::message::ProtoMsg<od::PositionInfo>;
using spPositionInfoMsg = std::shared_ptr<PositionInfoMsg>;

using FSLineMsg = hobot::message::ProtoMsg<od::FSLine>;
using spFSLineMsg = std::shared_ptr<FSLineMsg>;

using FSLinesimpleMsg = hobot::message::ProtoMsg<od::FSLinesimple>;
using spFSLinesimpleMsg = std::shared_ptr<FSLinesimpleMsg>;

using FSLinePointMsg = hobot::message::ProtoMsg<od::FSLinePoint>;
using spFSLinePointMsg = std::shared_ptr<FSLinePointMsg>;

using QuadParkingSlotsMsg = hobot::message::ProtoMsg<rd::QuadParkingSlots>;
using spQuadParkingSlotsMsg = std::shared_ptr<QuadParkingSlotsMsg>;

using FsmFusionSlotInfo2LocationMsg = hobot::message::ProtoMsg<fsm::FusionSlotInfo2Location>;
using spFsmFusionSlotInfo2LocationMsg = std::shared_ptr<FsmFusionSlotInfo2LocationMsg>;

using AphFusionSlotInfo2LocationMsg = hobot::message::ProtoMsg<aph::FusionSlotInfo2Location>;
using spAphFusionSlotInfo2LocationMsg = std::shared_ptr<AphFusionSlotInfo2LocationMsg>;

} // namespace parking
} // namespace fanya
