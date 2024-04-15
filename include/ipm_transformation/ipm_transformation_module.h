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
#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include "dataflow/callback/callback.h"
#include "dataflow/module/module.h"
#include "dataflow/module/module_option.h"
#include "dataflow/module/proc.h"
#include "common/proto_msg_all.h"
#include "common/utils.h"
#include "convert_assistant/gdc_stitch_assistant.h"

namespace fanya {
namespace parking {
class IpmTransformationModule : public hobot::dataflow::Module {
public:
  explicit IpmTransformationModule(
      const hobot::dataflow::ModuleOption &module_option);
  ~IpmTransformationModule() = default;
  void InitPortsAndProcs() override;
  int32_t Start() override;
  int32_t Stop() override;
  void Reset() override;
  int32_t DeInit() override;
  void TransformToIpmProc(hobot::dataflow::spMsgResourceProc proc,
                          const hobot::dataflow::MessageLists &msgs);
  int32_t CollectSourceImages(const spCameraFrameArrayProtoMsg &camera_array_msg,
                              hobot::gdc_stitch_assistant::VioBufferList &src_images,
                              std::vector<hb_vio_buffer_t> &vio_buf_holder,
                              int64_t &vio_ts, int64_t &vio_us,
                              int64_t &frame_id);
int32_t UpdateIpmProto(spCameraFrameArrayProtoMsg &camera_array_msg,
                      hobot::gdc_stitch_assistant::spWrapVioImage &out);                   

protected:
  int32_t Init() override;

private:
  // std::vector<Target> targets_;
  int32_t process_mode_;
  hobot::gdc_stitch_assistant::GdcStitchCfg gdc_stitch_cfg_;
  bool enable_init_vio_;
  std::string gdc_stitch_cfg_file_;
  std::string pyramid_layer_type_;
  std::int32_t pyramid_layer_id_;
  std::shared_ptr<hobot::gdc_stitch_assistant::GdcStitchAssistant>
      gdc_stitch_assistant_;
  std::map<std::string, std::shared_ptr<FpsConfig>> fps_;
  int32_t gdc_process_start_pipe_{0};
};

}  // namespace parking
}  // namespace fanya