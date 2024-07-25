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
#include "common/proto_msg_all.h"
#include "vio/hb_vio_interface.h"


#include <cstdint>
#include <mutex>
namespace fanya {
namespace parking {

struct FpsConfig{
  std::mutex fps_mutex;
  uint64_t fps_timestamp_start{0};
  uint64_t fps_frame_count{0};
};

constexpr std::size_t kStdSizeZero = 0;

const camera_frame::YuvAddrInfo *GetCameraYuvAddr(const spCameraProtoMsg &msg,
                                                const std::string &layer_type,
                                                const int layer);

bool CheckLayerTypeValid(const std::string &layer_type);
bool GetImageProtoAddrInfo(const spImageProtoMsg &msg, address_info_t &addr_info);
int32_t FillArrayMsgToIpmBuffer(spImageProtoMsg &out_msg, 
                            const spCameraFrameArrayProtoMsg &msg);
void ReportFps(std::string module_name, std::string port_id, std::string topic,
                std::shared_ptr<FpsConfig> fps_config);
} // namespace parking
} // namespace fanya
