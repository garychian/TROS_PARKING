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

#include "message/message.h"
#include "message/proto/proto_serializer.hpp"

#include "fanya_protocol/aph.pb.h"
#include "fanya_protocol/fsm.pb.h"

#include <ctime>

// namespace fanya {

// namespace parking {

// using ImageProtoMsg = hobot::message::ProtoMsg<ImageProto::Image>;
// using spImageProtoMsg = std::shared_ptr<ImageProtoMsg>;

// using CameraProtoMsg = hobot::message::ProtoMsg<camera_frame::CameraFrame>;
// using spCameraProtoMsg = std::shared_ptr<CameraProtoMsg>;

// using spCameraFrameArrayProtoMsg = std::shared_ptr<CameraFrameArrayProtoMsg>;

using ApaStatusMsg = hobot::message::ProtoMsg<aph::apa_status>;
using spApaStatusMsg = std::shared_ptr<ApaStatusMsg>;

using TargetSlotMsg = hobot::message::ProtoMsg<aph::target_slot>;
using spTargetSlotMsg = std::shared_ptr<TargetSlotMsg>;


using SlotlabelMsg = hobot::message::ProtoMsg<fsm::Slotlabel>;
using spSlotlabelMsg = std::shared_ptr<SlotlabelMsg>;

using ApastatusreqMsg = hobot::message::ProtoMsg<fsm::Apastatusreq>;
using spApastatusreqMsg = std::shared_ptr<ApastatusreqMsg>;

static uint64_t GetTimeStamp() {
    auto time_now = std::chrono::system_clock::now();
    auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_now.time_since_epoch());
    return static_cast<uint64_t>(time_ms.count());
}

static uint64_t GetTimeStampUs() {
    auto time_now = std::chrono::system_clock::now();
    auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(
        time_now.time_since_epoch());
    return static_cast<uint64_t>(time_us.count());
}


//} // namespace parking
//} // namespace fanya
