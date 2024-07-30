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

#include "rscl/perception_od.h"
#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/std_msgs/raw_data.capnp.h"

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"
#include "common/proto_msg_all.h"
#include "common/timestamp.h"

using hobot::communication::ProtobufSerializer;
using hobot::communication::ProtoMsg;

using ImageSerial = ProtobufSerializer<rd::Image>;
using ImageMsg = ProtoMsg<rd::Image>;

using RawObstacleSerial = ProtobufSerializer<od::Obstacles>;
using RawObstacleMsg = ProtoMsg<od::Obstacles>;

namespace fanya {
namespace parking {

struct Header{
  uint32_t seq;
  uint64_t timestamp;
  std::string frame_id;
};

struct Point2f
{
  float32_t x;
  float32_t y;
};

struct Box2D  
{
  float32_t topLeftX;
  float32_t topLeftY;
  float32_t bottomRightX;
  float32_t bottomRightY;
  float32_t confidence;
};
struct ObstacleRaw
{
  std::string camera;
  int32_t label;
  float32_t typeConfidence;
  float32_t existenceConfidence;
  Box2D box;
  std::vector<Point2f> landmark4;
  std::vector<float32_t> landmark4Scores;
};

struct Obstacles{
  Header header;
  std::vector<ObstacleRaw> obstaclesraw;
};


class PerceptionOdMoudle:
  public hobot::dataflow::Module{
 public:
  explicit PerceptionOdMoudle(
    const hobot::dataflow::ModuleOption &module_option);
  ~PerceptionOdMoudle() = default;
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
  std::shared_ptr<senseAD::avp_perception::PerceptionOdComponent> perception_OD_comp_;

  std::shared_ptr<hobot::communication::Publisher<RawObstacleSerial>> publisher_;
  std::shared_ptr<hobot::communication::Subscriber<RawObstacleSerial>> subscriber_;

  std::shared_ptr<hobot::communication::Publisher<ImageSerial>> front_camera_publisher_;
  std::shared_ptr<hobot::communication::Subscriber<ImageSerial>> front_camera_subscriber_;

  std::shared_ptr<hobot::communication::Publisher<ImageSerial>> rear_camera_publisher_;
  std::shared_ptr<hobot::communication::Subscriber<ImageSerial>> rear_camera_subscriber_;

  std::shared_ptr<hobot::communication::Publisher<ImageSerial>> left_camera_publisher_;
  std::shared_ptr<hobot::communication::Subscriber<ImageSerial>> left_camera_subscriber_;

  std::shared_ptr<hobot::communication::Publisher<ImageSerial>> right_camera_publisher_;
  std::shared_ptr<hobot::communication::Subscriber<ImageSerial>> right_camera_subscriber_;
 
 public:
  cv::Mat resizedMat;
  cv::Mat NV12ResizedMat;
  cv::Mat NV12ResizedMat_front;
  cv::Mat NV12ResizedMat_rear;
  cv::Mat NV12ResizedMat_left;
  cv::Mat NV12ResizedMat_right;
};

}  // namespace parking
}  // namespace fanya