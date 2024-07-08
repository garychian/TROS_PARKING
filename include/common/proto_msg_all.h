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
#include "fanya_protocol/psd.pb.h"
#include "fanya_protocol/aph.pb.h"
#include "fanya_protocol/fsm.pb.h"
#include "fanya_protocol/s32g.pb.h"
#include "fanya_protocol/sen.pb.h"

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

using SApaPSInfoMsg = hobot::message::ProtoMsg<psd::SApaPSInfo>;
using spSApaPSInfoMsg = std::shared_ptr<SApaPSInfoMsg>;

using SApaPSRectMsg = hobot::message::ProtoMsg<psd::SApaPSRect>;
using spSApaPSRectMsg = std::shared_ptr<SApaPSRectMsg>;

using SApaPoint_IMsg = hobot::message::ProtoMsg<psd::SApaPoint_I>;
using spSApaPoint_IMsg = std::shared_ptr<SApaPoint_IMsg>;

using ImageMsg = hobot::message::ProtoMsg<psd::Image>;
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

} // namespace parking
} // namespace fanya
