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

#include "common/utils.h"
#include "common/log.h"
#include "common/timestamp.h"
#include "hb_mem_mgr.h"

namespace hobot {
namespace samples {
using hobot::communication::DataRef;
namespace{
  constexpr int32_t KYuvPlaneCount = 2;
}

const camera_frame::YuvAddrInfo *GetCameraYuvAddr(const spCameraProtoMsg &msg,
                                                const std::string &layer_type,
                                                const int layer) {
  auto &pym_img_info = msg->proto.pym_img_info();
  auto &gdc_img_info = msg->proto.gdc_frame_info();
  if (layer_type == "base" && pym_img_info.base_layer_size() > layer) {
    return &pym_img_info.base_layer(layer);
  } else if (layer_type == "ds" && pym_img_info.down_scale_size() > layer) {
    return &pym_img_info.down_scale(layer);
  } else if (layer_type == "gs" && pym_img_info.gs_layer_size() > layer) {
    return &pym_img_info.gs_layer(layer);
  } else if (layer_type == "bl" && pym_img_info.bl_layer_size() > layer) {
    return &pym_img_info.bl_layer(layer);
  } else if (layer_type == "us" && pym_img_info.up_scale_size() > layer) {
    return &pym_img_info.up_scale(layer);
  } else if (layer_type == "gdc" && gdc_img_info.gdc_frame_size() > layer){
    return &gdc_img_info.gdc_frame(layer);
  } else {
    return nullptr;
  }
}

bool CheckLayerTypeValid(const std::string &layer_type) {
  if (layer_type != "base" && layer_type != "ds" && layer_type != "gs" &&
      layer_type != "bl" && layer_type != "us" && layer_type != "gdc") {
    LOGE("invalid layer_type [{}], only support 'base','ds','bl', 'gs', 'us'"
         "and 'gdc'",
         layer_type);
    return false;
  }
  return true;
}

bool GetImageProtoAddrInfo(const spImageProtoMsg &msg, address_info_t &addr_info){
  auto data_vec = msg->GetData()->GetDataVec();
  if (data_vec.size() < KYuvPlaneCount){
    LOGE("msg's data_ref size error, size = {}", data_vec.size());
    return false;
  }
  addr_info.height = msg->proto.height();
  addr_info.width = msg->proto.width();
  addr_info.stride_size = msg->proto.step();

  //YUV420 y paddr
  hb_mem_common_buf_t buf_y;
  int ret_y = hb_mem_get_com_buf_with_vaddr(reinterpret_cast<uint64_t>(data_vec[0]), &buf_y);
  if(ret_y != 0){
    LOGE("get y paddr error");
    return false;
  }
  addr_info.addr[0] = reinterpret_cast<char *>(data_vec[0]);
  addr_info.paddr[0] = buf_y.phys_addr + buf_y.offset;
  //YUV420 uv paddr
  hb_mem_common_buf_t buf_uv;
  int ret_uv = hb_mem_get_com_buf_with_vaddr(reinterpret_cast<uint64_t>(data_vec[1]), &buf_uv);
  if(ret_uv != 0){
    LOGE("get uv paddr error");
    return false;
  }
  addr_info.addr[1] = reinterpret_cast<char *>(data_vec[1]);
  addr_info.paddr[1] = buf_uv .phys_addr + buf_uv.offset;

  addr_info.addr[2] = 0;
  addr_info.paddr[2] = 0;
  return true;
}

int32_t FillArrayMsgToIpmBuffer(spImageProtoMsg &out_msg, 
                            const spCameraFrameArrayProtoMsg &msg) {
  auto& ipm_proto = msg->proto.gdc_frame(0);

  auto ipm_height = ipm_proto.height();
  auto ipm_width = ipm_proto.width();
  auto ipm_stride = ipm_proto.step();

  out_msg->proto.set_height(ipm_height);
  out_msg->proto.set_width(ipm_width);
  out_msg->proto.set_step(ipm_stride);
  out_msg->proto.set_time_stamp(msg->GetGenTimestamp());
  out_msg->SetGenTimestamp(msg->GetGenTimestamp());
  out_msg->SetDoneTimestamp(msg->GetDoneTimestamp());

  auto data_vec = msg->GetData()->GetDataVec();
  auto len = data_vec.size();
  hobot::message::spDataRef data_ref = std::make_shared<DataRef>();

  data_ref->PushData(data_vec[len - 1], ipm_height*ipm_width);
  data_ref->PushData(reinterpret_cast<void *>(reinterpret_cast<uint64_t>(data_vec[len - 1]) + ipm_proto.com_buf_c_offset()), ipm_height*ipm_width/2);
  out_msg->SetData(data_ref);

  return 0;
}

void ReportFps(std::string module_name, std::string port_id, std::string topic, 
                std::shared_ptr<FpsConfig> fps_config){
    std::lock_guard<std::mutex> lk(fps_config->fps_mutex);
    uint64_t now = GetTimeStamp();
    if (0 == fps_config->fps_timestamp_start) {
      fps_config->fps_timestamp_start = now;
    }
    (fps_config->fps_frame_count)++;
    if (0 == fps_config->fps_frame_count % 100) {
      double fps = 100000.0 / (now - fps_config->fps_timestamp_start);
      LOGI("[{}], port_id:[{}], topic:[{}], fps: {:.2f}", module_name, port_id, topic, fps);
      fps_config->fps_frame_count = 0;
      fps_config->fps_timestamp_start = now;
    }
}

} // namespace samples
} // namespace hobot
