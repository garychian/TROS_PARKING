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


#pragma once
#include "dataflow/callback/callback.h"
#include "dataflow/module/module.h"
#include "dataflow/module/module_option.h"
#include "dataflow/module/proc.h"

#include "rscl/perception_rd.h"
#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/fy_test/image.capnp.h"
#include "ad_msg_idl/std_msgs/raw_data.capnp.h"
#include "ad_msg_idl/ad_perception/quad_parking_slots.capnp.h"
#include "ad_msg_idl/fy_test/status_dec_fusion.capnp.h"
#include "ad_msg_idl/ad_perception/base.capnp.h"

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"
#include "common/proto_msg_all.h"
#include "common/timestamp.h"
#include "common/apa_define.h"

using hobot::communication::ProtobufSerializer;
using hobot::communication::ProtoMsg;
using ImageSerial = ProtobufSerializer<rd::Image>;
using ImageMsg = ProtoMsg<rd::Image>;
using QuadpldSerial = ProtobufSerializer<rd::QuadParkingSlots>;
using QuadpldMsg = ProtoMsg<rd::QuadParkingSlots>;

namespace fanya {
namespace parking {

class PerceptionRdMoudle:
  public hobot::dataflow::Module{
 public:
  explicit PerceptionRdMoudle(
    const hobot::dataflow::ModuleOption &module_option);
  ~PerceptionRdMoudle() = default;
  void InitPortsAndProcs() override;
  int32_t Start() override;
  int32_t Stop() override;
  void Reset() override;
  int32_t DeInit() override;
  void MsgCenterProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs);
  void TimerProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs);
 protected:
  int32_t Init() override;
 private:
  std::shared_ptr<senseAD::avp_perception::PerceptionRdComponent> perceptioncomp_;

  std::shared_ptr<hobot::communication::Publisher<QuadpldSerial>> publisher_;
  std::shared_ptr<hobot::communication::Subscriber<QuadpldSerial>> subscriber_;

  std::shared_ptr<hobot::communication::Publisher<ImageSerial>> image_publisher_;
  std::shared_ptr<hobot::communication::Subscriber<ImageSerial>> image_subscriber_;

  std::shared_ptr<hobot::communication::Publisher<ImageSerial>> Segimage_publisher_;
  std::shared_ptr<hobot::communication::Subscriber<ImageSerial>> Segimage_subscriber_;

 public:
  cv::Mat NV12ResizedMat;
  cv::Mat resizedMat;
  cv::Mat cropedMat;
};

}  // namespace parking
}  // namespace fanya