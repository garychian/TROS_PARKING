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
#include "protocol/vehicleiostate.pb.h"

#include "fanya_protocol/aph.pb.h"
#include "fanya_protocol/fsm.pb.h"
#include "fanya_protocol/psd.pb.h"
#include "fanya_protocol/loc.pb.h"
#include "fanya_protocol/sen.pb.h"

#include  "protocol/image.pb.h"
#include <ctime>

class WrapImageProtoMsg : public hobot::message::ProtoMsg<ImageProto::Image> {
public:
  WrapImageProtoMsg() {}
  virtual ~WrapImageProtoMsg() {
    if (data_) {
      delete[] image_data_;
      data_ = nullptr;
    }
    image_size_ = 0;
  }

  void SetData(hobot::message::spDataRef data) final {
    if (data_) {
      delete[] image_data_;
      data_ = nullptr;
    }
    image_data_ = new uint8_t[data->GetDataSize()];
    image_size_ = data->GetDataSize();
    memcpy(image_data_, data->GetData(), data->GetDataSize());
    data->SetData(image_data_);
    ProtoMsg<ImageProto::Image>::SetData(data);
  }

  uint8_t *image_data_{nullptr};
  int image_size_{0};
};

using spWrapImageProtoMsg = std::shared_ptr<WrapImageProtoMsg>;
using WrapImageProtoSerializer =
    hobot::message::ProtobufSerializer<ImageProto::Image>;

using ImageProtoSerializer =
    hobot::message::ProtobufSerializer<ImageProto::Image>;


using ImageProtoMsg = hobot::message::ProtoMsg<ImageProto::Image>;
using spImageProtoMsg = std::shared_ptr<ImageProtoMsg>;

using CameraProtoMsg = hobot::message::ProtoMsg<camera_frame::CameraFrame>;
using spCameraProtoMsg = std::shared_ptr<CameraProtoMsg>;

using spCameraFrameArrayProtoMsg = std::shared_ptr<CameraFrameArrayProtoMsg>;
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

using PadRealTimeLocationMsg = hobot::message::ProtoMsg<loc::padRealTimeLocation>;
using spPadRealTimeLocationMsg = std::shared_ptr<PadRealTimeLocationMsg>;

using LocStatusMsg = hobot::message::ProtoMsg<loc::LocStatus>;
using spLocStatusMsg = std::shared_ptr<LocStatusMsg>;

using VehiclePosMsg = hobot::message::ProtoMsg<loc::VehiclePos>;
using spVehiclePosMsg = std::shared_ptr<VehiclePosMsg>;

using App2emap_DRMsg = hobot::message::ProtoMsg<loc::App2emap_DR>;
using spApp2emap_DRMsg = std::shared_ptr<App2emap_DRMsg>;

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

using IMUDataMsg = hobot::message::ProtoMsg<sen::IMUData>;
using spIMUDataMsg = std::shared_ptr<IMUDataMsg>;

using GNSSDataMsg = hobot::message::ProtoMsg<sen::GNSSData>;
using spGNSSDataMsg = std::shared_ptr<GNSSDataMsg>;

using INSDataMsg = hobot::message::ProtoMsg<sen::INSData>;
using spINSDataMsg = std::shared_ptr<INSDataMsg>;

using DualAntennaDataMsg = hobot::message::ProtoMsg<sen::DualAntennaData>;
using spDualAntennaDataMsg = std::shared_ptr<DualAntennaDataMsg>;

using CanSignalUnitMsg = hobot::message::ProtoMsg<vehicleiostate::CanSignalUnit>;
using spCanSignalUnitMsg = std::shared_ptr<CanSignalUnitMsg>;

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
