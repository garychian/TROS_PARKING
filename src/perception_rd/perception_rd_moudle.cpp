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
#include "perception_rd/perception_rd_moudle.h"

#include <functional>
#include <string>
#include <vector>

#include "common/proto_msg_all.h"
#include "common/timestamp.h"
#include "communication/common/types.h"
#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"
#include "message/proto/proto_serializer.hpp"

#include "common/utils.h"
#include "perception_rd/dependencies/j5/j5dvb_system/include/vio/hb_vpm_data_info.h"
#include "ad_msg_bridge/manager/inner_process_communication_pipe.h"
#include "communication/publisher.h"
#include "communication/subscriber.h"
#include "communication/common/comm_log.h"
#include "communication/wait_set/scope_guard.h"
#include <communication/common/types.h>
#include "communication/common/compiler_features.h"
#include "hb_mem_mgr.h"
#include "proto_msg_all.h"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x

namespace fanya {
namespace parking {

    int save_pred_img(QuadParkingSlots parking_slots, std::vector<uchar> &buffer, cv::Mat bgr_mat); 

    using hobot::communication::COMM_CODE_OK;
    using hobot::communication::CommAttr;
    using hobot::communication::CompositeOption;
    using hobot::communication::ErrorCode;
    using hobot::communication::ErrorMsg;
    using hobot::communication::EventType;
    using hobot::communication::LinkInfo;
    using hobot::communication::ParticipantAttr;
    using hobot::communication::Publisher;
    using hobot::communication::ScopeGuard;
    using hobot::communication::Subscriber;

    using hobot::communication::HistoryQoSPolicyKind;
    using hobot::communication::ProtobufSerializer;
    using hobot::communication::ProtoMsg;
    using hobot::communication::QosAdaptor;
    using hobot::communication::QosCapability;
    using hobot::communication::ReliabilityQosPolicyKind;

    using hobot::communication::PROTOCOL_COMPOSITE;
    using hobot::communication::PROTOCOL_DDS;
    using hobot::communication::PROTOCOL_HYBRID;
    using hobot::communication::PROTOCOL_INTRA;
    using hobot::communication::PROTOCOL_INVALID;
    using hobot::communication::PROTOCOL_PCIE;
    using hobot::communication::PROTOCOL_SDIO;
    using hobot::communication::PROTOCOL_SHM;
    using hobot::communication::PROTOCOL_ZMQ_EPGM;
    using hobot::communication::PROTOCOL_ZMQ_IPC;
    using hobot::communication::PROTOCOL_ZMQ_TCP;

    struct Args
    {
      int participant_id = 0;
      int protocol = PROTOCOL_INVALID;
      // the speed of different protocols is different, to avoid lose message, some
      // protocol must sleep a while during between each request
      int pub_interval = 0;
      bool is_dynamic = false;
    };

    static QuadParkingSlots saved_parking_slots_info;
    static int64_t seq = 0;

    static void SimpleSlotSubCallback(const std::shared_ptr<QuadpldMsg> &quadpld_msg)
    {
      std::cout << "[PSD]j5 callback arriverd!" << std::endl;
      std::cout << "[PSD]Slot size:" << quadpld_msg->proto.quadparkingslotlist_size() << std::endl;

      saved_parking_slots_info.header.frameId = quadpld_msg->proto.header().frameid();
      saved_parking_slots_info.header.seq = seq;
      seq++;
      // saved_parking_slots_info.frameTimeStampNs = quadpld_msg->proto.header().timestampns().nanosec();
      saved_parking_slots_info.frameTimeStampNs = quadpld_msg->proto.frametimestampns();
      std::cout << "[PSD]GLOBAL_Slot FrameId:" << saved_parking_slots_info.header.frameId <<std::endl;
      // std::cout << "[PSD]GLOBAL_Slot seq:" << saved_parking_slots_info.header.seq << std::endl;
      std::cout << "[PSD]GLOBAL_Slot frameTimeStampNs:" << saved_parking_slots_info.frameTimeStampNs << ", seq:" << saved_parking_slots_info.header.seq <<std::endl;

  
      if (quadpld_msg->proto.quadparkingslotlist_size() != 0)
      {
        saved_parking_slots_info.quadParkingSlotList.resize(quadpld_msg->proto.quadparkingslotlist_size());
        for (int i = 0; i < saved_parking_slots_info.quadParkingSlotList.size(); i++)
        {
          saved_parking_slots_info.quadParkingSlotList[i].confidence = quadpld_msg->proto.quadparkingslotlist(i).confidence();
          saved_parking_slots_info.quadParkingSlotList[i].label = quadpld_msg->proto.quadparkingslotlist(i).label();
          saved_parking_slots_info.quadParkingSlotList[i].slotType = quadpld_msg->proto.quadparkingslotlist(i).slottype();
          saved_parking_slots_info.quadParkingSlotList[i].filtered = quadpld_msg->proto.quadparkingslotlist(i).filtered();
          saved_parking_slots_info.quadParkingSlotList[i].isComlete = quadpld_msg->proto.quadparkingslotlist(i).iscomplete();
          saved_parking_slots_info.quadParkingSlotList[i].width = quadpld_msg->proto.quadparkingslotlist(i).width();
          saved_parking_slots_info.quadParkingSlotList[i].length = quadpld_msg->proto.quadparkingslotlist(i).length();
          saved_parking_slots_info.quadParkingSlotList[i].isVisited = quadpld_msg->proto.quadparkingslotlist(i).isvisited();
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " confidence:" << saved_parking_slots_info.quadParkingSlotList[i].confidence << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " label:" << saved_parking_slots_info.quadParkingSlotList[i].label << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " slotType:" << saved_parking_slots_info.quadParkingSlotList[i].slotType << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " filtered:" << saved_parking_slots_info.quadParkingSlotList[i].filtered << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " isComlete:" << saved_parking_slots_info.quadParkingSlotList[i].isComlete << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " width:" << saved_parking_slots_info.quadParkingSlotList[i].width << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " length:" << saved_parking_slots_info.quadParkingSlotList[i].length << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " isVisited:" << saved_parking_slots_info.quadParkingSlotList[i].isVisited << std::endl;
        
          saved_parking_slots_info.quadParkingSlotList[i].tl.x = quadpld_msg->proto.quadparkingslotlist(i).tl().x();
          saved_parking_slots_info.quadParkingSlotList[i].tl.y = quadpld_msg->proto.quadparkingslotlist(i).tl().y();
          saved_parking_slots_info.quadParkingSlotList[i].br.x = quadpld_msg->proto.quadparkingslotlist(i).br().x();
          saved_parking_slots_info.quadParkingSlotList[i].br.y = quadpld_msg->proto.quadparkingslotlist(i).br().y();
          saved_parking_slots_info.quadParkingSlotList[i].tr.x = quadpld_msg->proto.quadparkingslotlist(i).tr().x();
          saved_parking_slots_info.quadParkingSlotList[i].tr.y = quadpld_msg->proto.quadparkingslotlist(i).tr().y();
          saved_parking_slots_info.quadParkingSlotList[i].bl.x = quadpld_msg->proto.quadparkingslotlist(i).bl().x();
          saved_parking_slots_info.quadParkingSlotList[i].bl.y = quadpld_msg->proto.quadparkingslotlist(i).bl().y();
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " tl.x:" << saved_parking_slots_info.quadParkingSlotList[i].tl.x << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " tl.y:" << saved_parking_slots_info.quadParkingSlotList[i].tl.y << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " br.x:" << saved_parking_slots_info.quadParkingSlotList[i].br.x << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " br.y:" << saved_parking_slots_info.quadParkingSlotList[i].br.y << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " tr.x:" << saved_parking_slots_info.quadParkingSlotList[i].tr.x << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " tr.y:" << saved_parking_slots_info.quadParkingSlotList[i].tr.y << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " bl.x:" << saved_parking_slots_info.quadParkingSlotList[i].bl.x << std::endl;
          std::cout << "[PSD]GLOBAL_Slot:"<< i << " bl.y:" << saved_parking_slots_info.quadParkingSlotList[i].bl.y << std::endl;
        }
      
      }
    }

static ImageMsg seg_image;
static void SimpleSegimageSubCallback(const std::shared_ptr<ImageMsg> &segimg_msg){
    std::cout << "[PSD]j5 segimage callback arriverd!" << std::endl;
    rd::Time timestamp;
    timestamp.set_nanosec(segimg_msg->proto.header().timestampns().nanosec());
    rd::Header header;
    header.mutable_timestampns()->CopyFrom(timestamp);
    seg_image.proto.mutable_header()->CopyFrom(header);

    seg_image.proto.set_height(segimg_msg->proto.height());
    seg_image.proto.set_width(segimg_msg->proto.width());
    seg_image.proto.set_encoding(segimg_msg->proto.encoding());
    seg_image.proto.set_phyaddr(segimg_msg->proto.phyaddr());
    seg_image.proto.set_viraddr(segimg_msg->proto.viraddr());
    std::cout << "[PSD] seg_image height:" << seg_image.proto.height() << std::endl;
    std::cout << "[PSD] seg_image width:" << seg_image.proto.width() << std::endl;
    std::cout << "[PSD] seg_image encoding:" << seg_image.proto.encoding() << std::endl;
    std::cout << "[PSD] seg_image phyaddr:" << seg_image.proto.phyaddr() << std::endl;
    std::cout << "[PSD] seg_image viraddr:" << seg_image.proto.viraddr() << std::endl;
    auto data_ptr = segimg_msg->proto.data();
    unsigned char* target_buffer = new unsigned char[segimg_msg->proto.data().size()];
    memcpy(target_buffer, &data_ptr[0], segimg_msg->proto.data().size());
    seg_image.proto.set_data(target_buffer, segimg_msg->proto.data().size());
    delete[] target_buffer;
    std::cout << "[PSD] data size:" << seg_image.proto.data().size() << std::endl;
    std::cout << "[PSD] data phyaddr:" << (void*)&seg_image.proto.data()[0] << std::endl;
    std::cout << "[PSD] j5 segimage callback sucess!" << std::endl;
}


static void SimpleImageSubCallback(const std::shared_ptr<ImageMsg> &image_msg)
{
  std::cout << "[PSD]j5 image callback arriverd!" << std::endl;
  senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->NotifyPipe("/sensor/camera/ipm_stitc", image_msg.get());
  std::cout << "[PSD]j5 image callback sucess!" << std::endl;
}

PerceptionRdMoudle::PerceptionRdMoudle(
    const hobot::dataflow::ModuleOption &module_option)
    : hobot::dataflow::Module(module_option) {}

void PerceptionRdMoudle::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_apa_status", aph::apa_status);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_pad_point", loc::padPoint);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_pad_vehicle_pose", loc::padVehiclePose);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_camera_frame_array",
                                camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_ps_info", rd::SApaPSInfo);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_ps_rect", rd::SApaPSRect);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_pointI", rd::SApaPoint_I);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_psd_image", rd::Image);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_psd_image_s32g", rd::Image);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_apa_ps_info_s32g", rd::SApaPSInfo);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("percept_debug",ImageProto::Image);

  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_quad_parking_slots_s32g",
                                 rd::QuadParkingSlots);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "MsgCenterProc", PerceptionRdMoudle, MsgCenterProc,
      hobot::dataflow::ProcType::DF_MSG_COND_PROC,
      DF_VECTOR("sub_apa_status", "sub_pad_point", "sub_pad_vehicle_pose",
                "sub_camera_frame_array"),
      DF_VECTOR());
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "TimerProc", PerceptionRdMoudle, TimerProc,
      hobot::dataflow::ProcType::DF_MSG_TIMER_PROC, DF_VECTOR(),
      DF_VECTOR("percept_debug","pub_apa_ps_rect", "pub_apa_ps_info", "pub_apa_pointI",
                "pub_psd_image", "pub_psd_image_s32g","pub_apa_ps_info_s32g",
                "pub_quad_parking_slots_s32g"));
}

EventType g_pub_event = EventType(2);
    void pub_connlisteners(const std::shared_ptr<LinkInfo> participants,
                           EventType event)
    {
      const std::string &protocol = participants->protocol;
      std::cout << "connection status changed, participant info:\n"
                << "            participant id: " << participants->id << std::endl
                << "                  protocol: " << participants->protocol
                << std::endl
                << "                 link info: " << participants->link_info
                << std::endl;
      COHLOGI("****pub_connlisteners call succefull!");
      g_pub_event = event;
      if (event == EventType::EVENT_CONNECTED)
      {
        COHLOGI("++++++ {} Pub connected succeful! ++++", protocol);
      }
      if (event == EventType::EVENT_CONNECT_FAILED)
      {
        COHLOGI("----- {} Pub connected failed!! -----", protocol);
      }
      if (event == EventType::EVENT_DISCONNECTED)
      {
        COHLOGI("========{} Pub disconnected! ======", protocol);
      }
    }

int32_t PerceptionRdMoudle::Init() { 
   /************************SUB FROM PERCEPTION***************************/
      hobot::communication::Init("communication.json");
      std::string version = hobot::communication::GetVersion();
      std::cout << "communication version is " << version << "\n";

      std::cout << "mock example begin ..." << std::endl;

      auto rt = senseAD::rscl::GetCurRuntime();
      DFHLOG_I("Init Runtime");
      int argc = 1;
      const char *test[] = {"j5_test_node", nullptr};
      char **argv = const_cast<char **>(test);
      rt->Init(argc, argv);
      DFHLOG_I("Init Component");

      perceptioncomp_ = std::make_shared<senseAD::avp_perception::PerceptionRdComponent>();

      CommAttr comm_attr;
      comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      std::string topic = "/parking/perception/camera/parking_bboxes";
      std::string seg_topic = "perception_camera_ipm_segmentation";
      ErrorCode error_code;

      publisher_ = Publisher<QuadpldSerial>::New(comm_attr,
                                                 topic,
                                                 0,
                                                 PROTOCOL_SHM,
                                                 &error_code,
                                                 pub_connlisteners);
      if (!publisher_)
      {
        std::cout << " create publisher failed";
        return -1;
      }

      if (hb_mem_module_open() != 0)
      {
        std::cout << "hb_mem_module_open failed" << std::endl;
        return 1;
      }

      senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->RegPipe("/parking/perception/camera/parking_bboxes", [this](const void *ptr, const size_t)
                                                                    {
                                                                  std::cout<< "[PSD]In Reg Pipe aaaaaaaa"<<std::endl;
                                                                  auto out_ptr = reinterpret_cast<const QuadpldMsg *>(ptr);
                                                                  auto msg = std::make_shared<QuadpldMsg>();
                                                                  *msg = *out_ptr;
                                                                  std::cout<< "[PSD]In Reg Pipe"<<std::endl;
                                                                  this->publisher_->Pub(msg); });

      CommAttr sub_comm_attr;
      sub_comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      subscriber_ = Subscriber<QuadpldSerial>::New(
          sub_comm_attr, topic, 0, SimpleSlotSubCallback, PROTOCOL_SHM);

      // SEG image
      Segimage_publisher_ = Publisher<ImageSerial>::New(comm_attr,
                                                 seg_topic,
                                                 0,
                                                 PROTOCOL_SHM,
                                                 &error_code,
                                                 pub_connlisteners);
      if (!Segimage_publisher_)
      {
        std::cout << " create publisher failed";
        return -1;
      }
      senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->RegPipe(seg_topic, [this](const void *ptr, const size_t)
                                                                    {
                                                                  std::cout<< "[PSD]In Seg Reg Pipe aaaaaaaa"<<std::endl;
                                                                  auto out_ptr = reinterpret_cast<const ImageMsg *>(ptr);
                                                                  auto msg = std::make_shared<ImageMsg>();
                                                                  *msg = *out_ptr;
                                                                  std::cout<< "[PSD]In Seg Reg Pipe"<<std::endl;
                                                                  this->Segimage_publisher_->Pub(msg); });
      Segimage_subscriber_ = Subscriber<ImageSerial>::New(
          sub_comm_attr, seg_topic, 0, SimpleSegimageSubCallback, PROTOCOL_SHM);


      DFHLOG_I("SUB REGISTER!");
      /************************SUB FROM PERCEPTION***************************/

      /************************PUB FROM J5***************************/
      CommAttr image_comm_attr;
      image_comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      image_comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      image_comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      Args image_data_args;
      image_data_args.protocol = PROTOCOL_ZMQ_TCP;
      image_data_args.participant_id = 7;
      std::string image_topic = "/sensor/camera/ipm_stitc";
      ErrorCode image_error_code;

      std::cout << "[PSD]Before crete Image sub " << std::endl;
      image_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, image_topic, 0, SimpleImageSubCallback, PROTOCOL_SHM);

      if (!image_subscriber_)
      {
        std::cout << "[PSD] create subscriber failed";
        return -1;
      }

      image_publisher_ = Publisher<ImageSerial>::New(image_comm_attr,
                                                     "/sensor/camera/ipm_stitc",
                                                     0,
                                                     // image_data_args.protocol,
                                                     PROTOCOL_SHM,
                                                     &image_error_code,
                                                     pub_connlisteners);
      if (!image_publisher_)
      {
        std::cout << "[PSD]create publisher failed";
        return -1;
      }
      std::cout << "[PSD] After create Image sub" << std::endl;

      if (hb_mem_module_open() != 0)
      {
        std::cout << "[PSD]hb_mem_module_open failed" << std::endl;
        return 1;
      }

      /************************PUB FROM J5***************************/
  return 0; 
}

int32_t PerceptionRdMoudle::Start() {
  senseAD::rscl::component::ComponentConfig cfg;
  cfg.config_file_path = "./bridge.yaml";
  cfg.timer_conf.set_interval_ms(1000);
  perceptioncomp_->Initialize(cfg);
      
  return hobot::dataflow::Module::Start();
}

int32_t PerceptionRdMoudle::Stop() {
  perceptioncomp_->Shutdown();
  printf("Finish Shutdown\n");
  hobot::communication::DeInit();
  hb_mem_module_close();

  return hobot::dataflow::Module::Stop();
}

void PerceptionRdMoudle::Reset() { hobot::dataflow::Module::Reset(); }

int32_t PerceptionRdMoudle::DeInit() {
  return hobot::dataflow::Module::DeInit();
}

void PerceptionRdMoudle::MsgCenterProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs) {
  auto &sub_apa_status_msgs = msgs[proc->GetResultIndex("sub_apa_status")];
  for (auto &msg : *(sub_apa_status_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_apa_status msg timestamp: {}", msg->GetGenTimestamp());
    // process msg of sub_apa_status
    auto apa_status =
        std::dynamic_pointer_cast<ApaStatusMsg>(sub_apa_status_msgs->at(0));
    if (apa_status && apa_status->proto.has_m_current_apa_status()) {
      DFHLOG_I("sub_apa_status msg timestamp: {}, apa_status = {}",
               msg->GetGenTimestamp(),
               apa_status->proto.m_current_apa_status());
    }
  }
  auto &sub_pad_point_msgs = msgs[proc->GetResultIndex("sub_pad_point")];
  for (auto &msg : *(sub_pad_point_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_pad_point msg timestamp: {}", msg->GetGenTimestamp());
    // process msg of sub_pad_point
  }
  auto &sub_pad_vehicle_pose_msgs =
      msgs[proc->GetResultIndex("sub_pad_vehicle_pose")];
  for (auto &msg : *(sub_pad_vehicle_pose_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_pad_vehicle_pose msg timestamp: {}", msg->GetGenTimestamp());
    auto location_msg = std::dynamic_pointer_cast<PadVehiclePoseMsg>(msg);
    if (!location_msg) {
      DFHLOG_C("DetectionProc Empty location msg");
      return;
    } else {
      DFHLOG_W("DetectionProc get location_msg OK.");

      std::cout << "x_position: " << location_msg->proto.coord().x()
                << std::endl;
      std::cout << "y_position: " << location_msg->proto.coord().y()
                << std::endl;
      std::cout << "yaw_position: " << location_msg->proto.yaw() << std::endl;
      // location_msg->proto.status();
      // LOGC("SLOT TYPE: {}, ts = {}",
      // location_msg->proto.slottype(),msg->GetGenTimestamp());
    }

    // process msg of sub_pad_vehicle_pose
  }

  auto &sub_camera_frame_array_msgs =
      msgs[proc->GetResultIndex("sub_camera_frame_array")];
  auto gen_image_ts = GetTimeStamp(); 
  for (auto &msg : *(sub_camera_frame_array_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_camera_frame_array msg timestamp: {}",
             msg->GetGenTimestamp());
    // process msg of sub_camera_frame_array
    std::shared_ptr<CameraFrameArrayProtoMsg> camera_array_msg = 
            std::dynamic_pointer_cast<CameraFrameArrayProtoMsg>(msg);

    if (!camera_array_msg || camera_array_msg->proto.gdc_frame_size() == 0)
    {
      DFHLOG_E("Empty ipm frame, return.");
      return;
    }
    else
    {
      // test_camera = true;
      DFHLOG_W("get camera_array_msg OK.");
      spImageProtoMsg ipm_msg = std::make_shared<ImageProtoMsg>();
      // fill ipm msg
      FillArrayMsgToIpmBuffer(ipm_msg, camera_array_msg);
      
      // std::shared_ptr<DNNTensor> input_tensor = std::make_shared<DNNTensor>();
      address_info_t addr_info;
      if (!GetImageProtoAddrInfo(ipm_msg, addr_info))
      {
        DFHLOG_E("Get image proto addr info failed.");
        return;
      }
      int32_t width = addr_info.width;
      int32_t height = addr_info.height;
      int32_t mem_size = width * height;

      cv::Mat image_nv12(height * 1.5, width, CV_8UC1);
      cv::Mat rgb_mat(height, width, CV_8UC3);
      int32_t size;
      size = width * height;

      int32_t new_width = 352;
      int32_t new_height = 352;

      auto y_addr = image_nv12.data;
      auto u_addr = y_addr + size;
      auto v_addr = u_addr + size / 4;
      auto srcBuf = addr_info.addr[0];
      auto srcBuf1 = addr_info.addr[1];
      std::memcpy(y_addr, srcBuf, size);
      for (int i = 0; i < size / 4; ++i)
      {
        *u_addr++ = *srcBuf1++;
        *v_addr++ = *srcBuf1++;
      }

      cv::cvtColor(image_nv12, rgb_mat, cv::COLOR_YUV2BGR_NV12);
      cv::resize(rgb_mat, resizedMat, cv::Size(new_width, new_height));
      cv::cvtColor(resizedMat, NV12ResizedMat, cv::COLOR_BGR2YUV_I420);

      uint64_t addr_value = reinterpret_cast<uint64_t>(NV12ResizedMat.data);
      /**************************PUB******************************/
      auto image = std::make_shared<ImageMsg>();
      
      rd::Time image_time;
      image_time.set_nanosec(gen_image_ts);
      rd::Header od_header;
      od_header.mutable_timestampns()->CopyFrom(image_time);

      image->proto.mutable_header()->CopyFrom(od_header);
      image->proto.set_height(new_height);
      image->proto.set_width(new_width);
      image->proto.set_phyaddr(addr_value);
      image->proto.set_viraddr(addr_value);
      std::cout << "[PSD]Image timestamp:" << gen_image_ts << std::endl;
      std::cout << "[PSD]Image height:" << new_height << std::endl;
      std::cout << "[PSD]Image width:" << new_width << std::endl;
      std::cout << "[PSD]Image paddr:" << addr_info.paddr[0] << std::endl;
      std::cout << "[PSD]Image vaddr:" << addr_value << std::endl;

      auto ret = image_publisher_->Pub(image);
      std::cout << "[PSD] After Image pub" << std::endl;
      
      if (ret != COMM_CODE_OK)
      {
        std::cout << "[PSD]pub failed, reason: " << ErrorMsg(ret) << std::endl;
      }
      else
      {
        std::cout << "[PSD]pub Image successful! " << std::endl;
      }
      /**************************PUB******************************/
    }
  }

  { // PUB FROM J5 TO S32G
        // do something with output port pub_apa_ps_info
        // fill proto vector<int> fusion_out_rect_id;
        // static int slot_id = 0;
        auto apa_ps_info = std::make_shared<QuadParkingSlotsMsg>();
        
        rd::Header rd_header;
        rd_header.set_seq(saved_parking_slots_info.header.seq);
        // rd_header.set_frameid(saved_parking_slots_info.header.frameId);
        apa_ps_info->proto.mutable_header()->CopyFrom(rd_header);

        apa_ps_info->proto.set_frametimestampns(saved_parking_slots_info.frameTimeStampNs);
        apa_ps_info->proto.set_sensorid(saved_parking_slots_info.sensorId);
        DFHLOG_W("pub_quad_parking_slots_s32g info, timestamp = {},seq = {}",saved_parking_slots_info.frameTimeStampNs, saved_parking_slots_info.header.seq);
        // DFHLOG_W("pub_quad_parking_slots_s32g info, seq = {}",saved_parking_slots_info.header.seq);
        std::cout << "*********fusion_out size**********" << saved_parking_slots_info.quadParkingSlotList.size() << std::endl;
        // 一级子结构体 repeated为 数组形式
        if (saved_parking_slots_info.quadParkingSlotList.size() != 0)
        {
          for (int i = 0; i < saved_parking_slots_info.quadParkingSlotList.size(); i++)
          {
            rd::QuadParkingSlot *sapa_ps_rect = apa_ps_info->proto.add_quadparkingslotlist();
            sapa_ps_rect->set_confidence(saved_parking_slots_info.quadParkingSlotList[i].confidence);
            sapa_ps_rect->set_label(saved_parking_slots_info.quadParkingSlotList[i].label);
            sapa_ps_rect->set_filtered(saved_parking_slots_info.quadParkingSlotList[i].filtered);
            sapa_ps_rect->set_slottype(saved_parking_slots_info.quadParkingSlotList[i].slotType);
            sapa_ps_rect->set_width(saved_parking_slots_info.quadParkingSlotList[i].width);
            sapa_ps_rect->set_length(saved_parking_slots_info.quadParkingSlotList[i].length);
            sapa_ps_rect->set_isvisited(saved_parking_slots_info.quadParkingSlotList[i].isVisited);
          
            
              // 二级子结构体 repeated为 数组形式
              rd::Point2f point_tl;
              point_tl.set_x(saved_parking_slots_info.quadParkingSlotList[i].tl.x);
              point_tl.set_y(saved_parking_slots_info.quadParkingSlotList[i].tl.y);
              sapa_ps_rect->mutable_tl()->CopyFrom(point_tl);
              std::cout << "666 point_tl.x: " << saved_parking_slots_info.quadParkingSlotList[i].tl.x << std::endl;
              std::cout << "667 point_tl.y: " << saved_parking_slots_info.quadParkingSlotList[i].tl.y << std::endl;

              rd::Point2f point_tr;
              point_tr.set_x(saved_parking_slots_info.quadParkingSlotList[i].tr.x);
              point_tr.set_y(saved_parking_slots_info.quadParkingSlotList[i].tr.y);
              sapa_ps_rect->mutable_tr()->CopyFrom(point_tr);
              std::cout << "666 point_tr.x: " << saved_parking_slots_info.quadParkingSlotList[i].tr.x << std::endl;
              std::cout << "667 point_tr.y: " << saved_parking_slots_info.quadParkingSlotList[i].tr.y << std::endl;

              rd::Point2f point_bl;
              point_bl.set_x(saved_parking_slots_info.quadParkingSlotList[i].bl.x);
              point_bl.set_y(saved_parking_slots_info.quadParkingSlotList[i].bl.y);
              sapa_ps_rect->mutable_bl()->CopyFrom(point_bl);
              std::cout << "666 point_bl.x: " << saved_parking_slots_info.quadParkingSlotList[i].bl.x << std::endl;
              std::cout << "667 point_bl.y: " << saved_parking_slots_info.quadParkingSlotList[i].bl.y << std::endl;

              rd::Point2f point_br;
              point_br.set_x(saved_parking_slots_info.quadParkingSlotList[i].br.x);
              point_br.set_y(saved_parking_slots_info.quadParkingSlotList[i].br.y);
              sapa_ps_rect->mutable_br()->CopyFrom(point_br);
              std::cout << "666 point_br.x: " << saved_parking_slots_info.quadParkingSlotList[i].br.x << std::endl;
              std::cout << "667 point_br.y: " << saved_parking_slots_info.quadParkingSlotList[i].br.y << std::endl;
            
          }
        }

        auto pub_apa_ps_rect_port_s32g = proc->GetOutputPort("pub_quad_parking_slots_s32g");
        if (!pub_apa_ps_rect_port_s32g)
        {
          DFHLOG_E("pub_apa_ps_rect_port_s32g failed to get output port of {}", "pub_apa_ps_rect");
          return;
        }

        // pub_apa_ps_info_port->Send(apa_ps_info);
        pub_apa_ps_rect_port_s32g->Send(apa_ps_info);
        DFHLOG_W("pub pub_quad_parking_slots_s32g info, ullframeid = {}",
                 apa_ps_info->proto.header().frameid());
    }

}

void PerceptionRdMoudle::TimerProc(
    hobot::dataflow::spMsgResourceProc proc,
    const hobot::dataflow::MessageLists &msgs) {

      // SEND TO HVIZ
      std::vector<uchar> buffer;
     
      if (!NV12ResizedMat.empty())
      {
        save_pred_img(saved_parking_slots_info, buffer, NV12ResizedMat);
        {

          // std::cout<<"Detect_Cornerpoint_gpsd"<<__LINE__<<std::endl;
          uint64_t send_start = GetTimeStamp();
          auto out = std::make_shared<WrapImageProtoMsg>();
          // std::cout<<"Detect_Cornerpoint_gpsd"<<__LINE__<<std::endl;
          out->proto.set_width(352);
          out->proto.set_height(352);
          // out->proto.set_channel(msg->proto.channel());
          out->proto.set_send_mode(0);
          out->proto.set_format(2);
          out->SetData(std::make_shared<hobot::message::DataRef>(buffer.data(), buffer.size()));

          DFHLOG_W("percept_debug size: {}, ts = {}.", buffer.size(), GetTimeStamp());
          auto pub_image_port = proc->GetOutputPort("percept_debug");
          pub_image_port->Send(out);
          // std::cout<<"Detect_Cornerpoint_gpsd"<<__LINE__<<std::endl;
          //  uint64_t send_end = GetTimeStamp();
          //  //std::cout << "send time:" << send_end - send_start << "ms" << std::endl;
        }
      }
    }

    int save_pred_img(QuadParkingSlots parking_slots, std::vector<uchar> &buffer, cv::Mat rgb_mat)
    {

      cv::Mat bgr_mat;
      cv::cvtColor(rgb_mat, bgr_mat, cv::COLOR_YUV2BGR_NV12);
      int height = bgr_mat.rows;
      int width = bgr_mat.cols;
      int size = bgr_mat.cols * bgr_mat.rows;

      cv::Mat final_mat = bgr_mat.clone();

      for (int i = 0; i < parking_slots.quadParkingSlotList.size(); i++)
      {
        auto output_result = parking_slots.quadParkingSlotList[i];
        if (true)
        {
          cv::circle(final_mat, cv::Point(output_result.tl.x, output_result.tl.y), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
          cv::circle(final_mat, cv::Point(output_result.tr.x, output_result.tr.y), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
          cv::line(final_mat, cv::Point(output_result.tl.x, output_result.tl.y), cv::Point(output_result.tr.x, output_result.tr.y), cv::Scalar(0, 255, 0), 1);
          cv::line(final_mat, cv::Point(output_result.tr.x, output_result.tr.y), cv::Point(output_result.br.x, output_result.br.y), cv::Scalar(0, 255, 0), 1);
          cv::line(final_mat, cv::Point(output_result.tl.x, output_result.tl.y), cv::Point(output_result.bl.x, output_result.bl.y), cv::Scalar(0, 0, 255), 1);
        }
      }
      cv::imencode(".jpg", final_mat, buffer);
      //  cv::imwrite(target_image_file, final_mat);
      return 0;

    }
DATAFLOW_REGISTER_MODULE(PerceptionRdMoudle)

}  // namespace parking
}  // namespace fanya