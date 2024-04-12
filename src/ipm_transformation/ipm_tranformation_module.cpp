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

#include <convert_assistant/gdc_stitch_assistant.h>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <vio/hb_vio_interface.h>

#include "dataflow/callback/callback.h"
#include "dataflow/context/context.h"
#include "dataflow/module/module.h"
#include "dataflow/module/module_option.h"
#include "dataflow/module/proc.h"
#include "dataflow/module_loader/register_module_macro.h"
#include "ipm_transformation/ipm_transformation_module.h"
#include "common/jsonutil.h"
#include "common/log.h"
#include "common/proto_msg_all.h"
#include "common/utils.h"
#include "common/timestamp.h"

#define UNUSED(x) static_cast<void>(x)

using hobot::communication::DataRef;

namespace hobot {
namespace samples {

IpmTransformationModule::IpmTransformationModule(
    const hobot::dataflow::ModuleOption &module_option)
    : hobot::dataflow::Module(module_option) {}
void IpmTransformationModule::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_camera_array", camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_camera_array", camera_frame::CameraFrameArray);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "TransformToIpmProc", IpmTransformationModule, TransformToIpmProc,
      hobot::dataflow::ProcType::DF_MSG_COND_PROC,
      DF_VECTOR("sub_camera_array"),
      DF_VECTOR("pub_camera_array"));
}

int32_t IpmTransformationModule::Init() {
  auto ts_start = GetTimeStamp();
  LOGD("Init [{}] with: {}", module_option_.instance_id,
       module_option_.config_file);
  auto work_dir = dataflow::GlobalContext::Instance()->GetAppContext().work_dir;
  auto correct_path = work_dir + module_option_.config_file;

  for (auto &output : module_option_.outputs) {
    const std::string &port_id = output.first;
    fps_[port_id] = std::make_shared<FpsConfig>();
  }

  auto json = JSONUtil::ParseJson(correct_path);

  process_mode_ = json.value("process_mode", -1);
  gdc_process_start_pipe_ = json.value("gdc_process_start_pipe", 0);
  gdc_stitch_cfg_file_ =
      work_dir + json.value("gdc_stitch_config", std::string(""));
  enable_init_vio_ = json.value("enable_vio_init", 0) == 1;
  pyramid_layer_type_ = json.value("pyramid_layer_type", std::string(""));
  pyramid_layer_id_ = json.value("pyramid_layer_id", -1);

  if (CheckLayerTypeValid(pyramid_layer_type_) != true) {
    LOGE("invalid pyramid_layer_type {}, only support 'base','ds','bl', 'gs' "
         "and 'us'",
         pyramid_layer_type_);
    return -1;
  }

  LOGD("process_mode_ = {}", process_mode_);
  LOGD("gdc_stitch_cfg_file_ = {}", gdc_stitch_cfg_file_);
  LOGD("enable_init_vio_ = {}", enable_init_vio_);
  LOGD("pyramid_layer_type_ = {}", pyramid_layer_type_);
  LOGD("pyramid_layer_id_ = {}", pyramid_layer_id_);

  int32_t ret = ParseGdcStitchCfgFile(gdc_stitch_cfg_file_, gdc_stitch_cfg_);
  if (ret != 0) {
    LOGE("ParseGdcStitchCfgFile {} failed ", gdc_stitch_cfg_file_);
    return -1;
  }
  LOGD("ParseGdcStitchCfgFile {} succeed. ", gdc_stitch_cfg_file_);

  // just for stitch by stitch
  gdc_stitch_cfg_.stitch_lut = work_dir + gdc_stitch_cfg_.stitch_lut;
  gdc_stitch_cfg_.stitch_roi = work_dir + gdc_stitch_cfg_.stitch_roi;
  gdc_stitch_cfg_.vio_cfg_file = work_dir + gdc_stitch_cfg_.vio_cfg_file;
  LOGD("gdc_stitch_cfg_.vio_cfg_file is {}. ", gdc_stitch_cfg_.vio_cfg_file);
  for (auto &iter : gdc_stitch_cfg_.gdc_cfg_prop) {
    iter.second->gdc_cfg_data = work_dir + iter.second->gdc_cfg_data;
    LOGD("gdc bin cfg of pipeline {} is {}.", iter.first,
         iter.second->gdc_cfg_data);
  }

  if (enable_init_vio_) {
    LOGD("running hb_vio_init and hb_vio_start_pipeline "
         "now...");
    ret = hb_vio_init(gdc_stitch_cfg_.vio_cfg_file.c_str());
    if (ret != 0) {
      LOGE("hb_vio_init failed with cfg {}.",
           gdc_stitch_cfg_.vio_cfg_file.c_str());
      return -1;
    }
    for (auto &iter : gdc_stitch_cfg_.gdc_cfg_prop) {

      std::shared_ptr<gdc_stitch_assistant::GdcCfgProperties> &prop =
          iter.second;
      uint32_t pipeline_id = prop->pipeline;
      LOGD("running hb_vio_start_pipeline({}) "
           "now...",
           pipeline_id);
      if (hb_vio_start_pipeline(pipeline_id)) {
        LOGE("hb_vio_start_pipeline failed, pipeline={}", pipeline_id);
        return -1;
      }
      LOGD("running hb_vio_start_pipeline({}) "
           "succeed.",
           pipeline_id);
    }
  }
  LOGD("constructing GdcStitchAssistant....");
  gdc_stitch_assistant_ =
      std::make_shared<gdc_stitch_assistant::GdcStitchAssistant>(
          &gdc_stitch_cfg_, process_mode_, nullptr);

  LOGD("Initializing GdcStitchAssistant....");
  if (gdc_stitch_assistant_->Init()) {
    LOGE("GdcStitchAssistant init failed");
    return -1;
  }
  LOGD("Run IpmTransformationModule::Init succeed.");
  LOGW("init [{}] cast [{}]", module_option_.instance_id, GetTimeStamp() - ts_start);
  return 0;
}

int32_t IpmTransformationModule::Start() {
  LOGI("Start [{}]", module_option_.instance_id);
  return hobot::dataflow::Module::Start();
}

int32_t IpmTransformationModule::Stop() {
  LOGI("Stop [{}]", module_option_.instance_id);
  return hobot::dataflow::Module::Stop();
}

void IpmTransformationModule::Reset() {
  LOGI("Reset [{}]", module_option_.instance_id);
  hobot::dataflow::Module::Reset();
}

int32_t IpmTransformationModule::DeInit() {
  LOGI("DeInit [{}]", module_option_.instance_id);
  return hobot::dataflow::Module::DeInit();
}

int32_t IpmTransformationModule::CollectSourceImages(
    const spCameraFrameArrayProtoMsg &camera_array_msg,
    gdc_stitch_assistant::VioBufferList &src_images,
    std::vector<hb_vio_buffer_t> &vio_buf_holder, int64_t &vio_ts,
    int64_t &vio_us, int64_t &frame_id) {
  // get image.
  auto len = camera_array_msg->proto.camera_frame_size();
  for (int id = 0; id < len; id++) {
    std::string cam_id_ = camera_array_msg->proto.camera_frame(id).cam_id();
    auto cam_msg = camera_arry_msg_convert(camera_array_msg, cam_id_);
    camera_msg_convert(cam_msg);
    if (cam_msg == nullptr) {
      LOGE("Failed to get CameraProtoMsg from CameraArrayMsg .");
      return -1;
    }

    if (vio_ts < 0) {
      vio_ts = cam_msg->proto.vio_ts();
    }
    if (vio_us < 0) {
      vio_us = cam_msg->proto.vio_us();
    }
    if (frame_id < 0) {
      frame_id = cam_msg->proto.pym_img_info().frame_seq();
    }
    auto pym_img =
        GetCameraYuvAddr(cam_msg, pyramid_layer_type_, pyramid_layer_id_);
    hb_vio_buffer_t &src_buf = vio_buf_holder.at(id);
    memset(&src_buf, 0, sizeof(hb_vio_buffer_t));
    // build src_buf using input pym_imgs
    src_buf.img_info.pipeline_id = id;
    src_buf.img_info.planeCount = 2;
    src_buf.img_info.img_format = 8;
    src_buf.img_info.size[0] = pym_img->width() * pym_img->height();
    src_buf.img_info.size[1] = pym_img->width() * pym_img->height() / 2;
    src_buf.img_addr.width = pym_img->width();
    src_buf.img_addr.height = pym_img->height();
    src_buf.img_addr.stride_size = pym_img->step();
    src_buf.img_addr.addr[0] = reinterpret_cast<char *>(pym_img->y_vaddr());
    src_buf.img_addr.addr[1] = reinterpret_cast<char *>(pym_img->c_vaddr());
    src_buf.img_addr.paddr[0] = pym_img->y_paddr();
    src_buf.img_addr.paddr[1] = pym_img->c_paddr();
    //vio_buf_holder.emplace_back(src_buf);
    src_images[id + gdc_process_start_pipe_] = &vio_buf_holder.at(id);
  }
  return 0;
}

int32_t
IpmTransformationModule::UpdateIpmProto(spCameraFrameArrayProtoMsg &camera_array_msg,
                                      gdc_stitch_assistant::spWrapVioImage &out) {
  auto ipm = camera_array_msg->proto.add_gdc_frame();
  ipm->set_width(
          static_cast<int32_t>(out->GetVioBuffer()->img_addr.width));
  ipm->set_height(
          static_cast<int32_t>(out->GetVioBuffer()->img_addr.height));
  ipm->set_step(
          static_cast<int32_t>(out->GetVioBuffer()->img_addr.stride_size));
  ipm->set_y_paddr(out->GetVioBuffer()->img_addr.paddr[0]);
  ipm->set_c_paddr(out->GetVioBuffer()->img_addr.paddr[1]);
  ipm->set_y_vaddr(reinterpret_cast<uint64_t>(out->GetVioBuffer()->img_addr.addr[0]));
  ipm->set_c_vaddr(reinterpret_cast<uint64_t>(out->GetVioBuffer()->img_addr.addr[1]));

  ipm->set_com_buf_y_offset(0);
  ipm->set_com_buf_c_offset(ipm->width() * ipm->height());
  camera_array_msg->GetData()->PushData(out->GetVioBuffer()->img_addr.addr[0],
          ipm->width()*ipm->height()*3/2);

  return 0;
}

void IpmTransformationModule::TransformToIpmProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs) {
  UNUSED(proc);
  auto ts_start = GetTimeStamp();
  int64_t vio_ts = -1;
  int64_t vio_us = -1;
  int64_t frame_id = -1;
  gdc_stitch_assistant::VioBufferList src_images;
  auto camera_array_msg = std::dynamic_pointer_cast<CameraFrameArrayProtoMsg>(msgs[0]->at(0));
  if (!camera_array_msg) {
    LOGE("Empty camera array frame");
    return;
  }
  std::vector<hb_vio_buffer_t> src_image_holder(camera_array_msg->proto.camera_frame_size());
  int ret = CollectSourceImages(camera_array_msg, src_images, src_image_holder, vio_ts,
                                vio_us, frame_id);
  if (ret != 0 || src_images.size() != gdc_stitch_cfg_.gdc_cfg_prop.size()) {
    LOGW("Run CollectSourceImages failed, Dropped!  src_images_size = {}", src_images.size());
    return;
  }
  std::vector<gdc_stitch_assistant::spWrapVioImage> out;
  ret = gdc_stitch_assistant_->Process(src_images, out);
  UpdateIpmProto(camera_array_msg, out[0]);
  auto output_port = GetOutputPort("pub_camera_array");
  if (output_port) {
    LOGD("Publishing pub_camera_array, cost = {}",  GetTimeStamp() - ts_start);
    output_port->Send(camera_array_msg);
  }

  // report fps
  ReportFps(module_option_.module_name, "pub_camera_array", 
            output_port->GetPub()->GetTopic(), fps_["pub_camera_array"]);
}

DATAFLOW_REGISTER_MODULE(IpmTransformationModule)
} // namespace samples
} // namespace hobot
