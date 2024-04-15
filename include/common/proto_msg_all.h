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
#include "fanya_protocol/can.pb.h"
#include "fanya_protocol/psd.pb.h"
#include "fanya_protocol/aph.pb.h"
#include "fanya_protocol/fsm.pb.h"
#include "fanya_protocol/s32g.pb.h"
#include "fanya_protocol/sen.pb.h"

// namespace fanya {

// namespace parking {

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

//} // namespace parking
//} // namespace fanya
