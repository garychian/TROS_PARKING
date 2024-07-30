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
#include "perception_od/perception_od_moudle.h"

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
#include "perception_od/dependencies/j5/j5dvb_system/include/vio/hb_vpm_data_info.h"
#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"
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

namespace fanya
{
  namespace parking
  {

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

// std::vector<ObstacleRaw> obstaclesraw;
Obstacles obstacles;
static int64_t seq_ = 0;

static void SimpleObstacleSubCallback(const std::shared_ptr<RawObstacleMsg> &raw_obstacle_msg)
{
  std::cout << "[OD]j5 callback arriverd!" << std::endl;
  std::cout << "[OD]rawobjects_size:" << raw_obstacle_msg->proto.rawobjects_size()<<std::endl;
  obstacles.header.timestamp = raw_obstacle_msg->proto.header().timestampns().nanosec();
  obstacles.header.frame_id = "1";
  obstacles.header.seq = seq_;
  seq_++;
  std::cout<<"[OD] header.timestamp: "<<obstacles.header.timestamp<<std::endl;
  std::cout<<"[OD] header.seq: "<<obstacles.header.seq<<std::endl;
  std::cout<<"[OD] header.frame_id: "<<obstacles.header.frame_id<<std::endl;

  if (raw_obstacle_msg->proto.rawobjects_size() != 0){
      obstacles.obstaclesraw.resize(raw_obstacle_msg->proto.rawobjects_size());
      for(int icnt = 0; icnt < raw_obstacle_msg->proto.rawobjects_size(); icnt++){
          obstacles.obstaclesraw[icnt].label = raw_obstacle_msg->proto.rawobjects(icnt).label();
          obstacles.obstaclesraw[icnt].typeConfidence = raw_obstacle_msg->proto.rawobjects(icnt).typeconfidence();
          obstacles.obstaclesraw[icnt].existenceConfidence = raw_obstacle_msg->proto.rawobjects(icnt).existenceconfidence();
          obstacles.obstaclesraw[icnt].box.topLeftX = raw_obstacle_msg->proto.rawobjects(icnt).box2d().topleftx();
          obstacles.obstaclesraw[icnt].box.topLeftY = raw_obstacle_msg->proto.rawobjects(icnt).box2d().toplefty();
          obstacles.obstaclesraw[icnt].box.bottomRightX = raw_obstacle_msg->proto.rawobjects(icnt).box2d().bottomrightx();
          obstacles.obstaclesraw[icnt].box.bottomRightY = raw_obstacle_msg->proto.rawobjects(icnt).box2d().bottomrighty();
          obstacles.obstaclesraw[icnt].box.confidence = raw_obstacle_msg->proto.rawobjects(icnt).box2d().confidence();
          std::cout<<"[OD] topLeftX: "<<obstacles.obstaclesraw[icnt].box.topLeftX<<" topLeftY: "<<obstacles.obstaclesraw[icnt].box.topLeftY<<" bottomRightX: "<<obstacles.obstaclesraw[icnt].box.bottomRightX<<" bottomRightX: "<<obstacles.obstaclesraw[icnt].box.bottomRightY<<std::endl;
          std::cout<<"[OD] confidence: "<<obstacles.obstaclesraw[icnt].box.confidence<<std::endl;
          std::cout<<"[OD] label: "<<obstacles.obstaclesraw[icnt].label<<" typeConfidence: "<<obstacles.obstaclesraw[icnt].typeConfidence<<" existenceConfidence: "<<obstacles.obstaclesraw[icnt].existenceConfidence<<std::endl;
          if (raw_obstacle_msg->proto.rawobjects(icnt).landmark4_size() != 0){
              obstacles.obstaclesraw[icnt].landmark4.resize(raw_obstacle_msg->proto.rawobjects(icnt).landmark4_size());
              obstacles.obstaclesraw[icnt].landmark4Scores.resize(raw_obstacle_msg->proto.rawobjects(icnt).landmark4scores_size());
              std::cout<<"[OD] landmark4_size: "<<raw_obstacle_msg->proto.rawobjects(icnt).landmark4_size()<<" landmark4scores_size: "<<raw_obstacle_msg->proto.rawobjects(icnt).landmark4scores_size()<<std::endl;
              for (int jcnt = 0; jcnt < raw_obstacle_msg->proto.rawobjects(icnt).landmark4_size(); jcnt++) {
                  obstacles.obstaclesraw[icnt].landmark4[jcnt].x = raw_obstacle_msg->proto.rawobjects(icnt).landmark4(jcnt).x();
                  obstacles.obstaclesraw[icnt].landmark4[jcnt].y = raw_obstacle_msg->proto.rawobjects(icnt).landmark4(jcnt).y();
                  std::cout<<"[OD] landmark4["<<icnt<<"]["<<jcnt<<"] x: "<<obstacles.obstaclesraw[icnt].landmark4[jcnt].x<<" y: "<<obstacles.obstaclesraw[icnt].landmark4[jcnt].y<<std::endl;    
              }
              for (int jcnt = 0; jcnt < raw_obstacle_msg->proto.rawobjects(icnt).landmark4scores_size(); jcnt++){
                  obstacles.obstaclesraw[icnt].landmark4Scores[jcnt] = raw_obstacle_msg->proto.rawobjects(icnt).landmark4scores(jcnt);
                  std::cout<<"[OD] landmark4Scores["<<icnt<<"]["<<jcnt<<"] : "<<obstacles.obstaclesraw[icnt].landmark4Scores[jcnt]<<std::endl;
        }
   
      }
    } 
  }
  std::cout << "[OD]j5 callback sucess!" << std::endl;
  
}

static void FrontCameraImageSubCallback(const std::shared_ptr<ImageMsg> &image_msg)
{
  std::cout << "[OD]FrontCameraImageSubCallback arriverd!" << std::endl;
  senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->NotifyPipe("front_camera_fov195", image_msg.get());
  std::cout << "[OD]FrontCameraImageSubCallback sucess!" << std::endl;
}

static void RearCameraImageSubCallback(const std::shared_ptr<ImageMsg> &image_msg)
{
  std::cout << "[OD]RearCameraImageSubCallback arriverd!" << std::endl;
  senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->NotifyPipe("rear_camera_fov195", image_msg.get());
  std::cout << "[OD]RearCameraImageSubCallback sucess!" << std::endl;
}

static void LeftCameraImageSubCallback(const std::shared_ptr<ImageMsg> &image_msg)
{
  std::cout << "[OD]LeftCameraImageSubCallback arriverd!" << std::endl;
  senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->NotifyPipe("left_camera_fov195", image_msg.get());
  std::cout << "[OD]LeftCameraImageSubCallback sucess!" << std::endl;
}

static void RightCameraImageSubCallback(const std::shared_ptr<ImageMsg> &image_msg)
{
  std::cout << "[OD]RightCameraImageSubCallback arriverd!" << std::endl;
  senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->NotifyPipe("right_camera_fov195", image_msg.get());
  std::cout << "[OD]RightCameraImageSubCallback sucess!" << std::endl;
}

PerceptionOdMoudle::PerceptionOdMoudle(
    const hobot::dataflow::ModuleOption &module_option)
    : hobot::dataflow::Module(module_option) {}

void PerceptionOdMoudle::InitPortsAndProcs() {
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_apa_status", aph::apa_status);
  DF_MODULE_INIT_IDL_INPUT_PORT("sub_camera_frame_array",
                                camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_INPUT_PORT("apa_image_in",
                                camera_frame::CameraFrameArray);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_obstacles", od::Obstacles);
  DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_fsline_msg", od::FSLine);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "MsgCenterProc", PerceptionOdMoudle, MsgCenterProc,
      hobot::dataflow::ProcType::DF_MSG_COND_PROC,
      DF_VECTOR("sub_apa_status", "sub_camera_frame_array","apa_image_in"), DF_VECTOR());
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
      "TimerProc", PerceptionOdMoudle, TimerProc,
      hobot::dataflow::ProcType::DF_MSG_TIMER_PROC, DF_VECTOR(),
      DF_VECTOR("pub_obstacles", "pub_fsline_msg"));
}

EventType g_pub_event = EventType(2);
void pub_connlisteners(const std::shared_ptr<LinkInfo> participants, EventType event)
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

int32_t PerceptionOdMoudle::Init() { 

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

      perception_OD_comp_ = std::make_shared<senseAD::avp_perception::PerceptionOdComponent>();

      CommAttr comm_attr;
      comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      std::string topic = "/perception/camera/fisheye_objects";
      ErrorCode error_code;

      publisher_ = Publisher<RawObstacleSerial>::New(comm_attr,
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

      senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->RegPipe(topic, [this](const void *ptr, const size_t)
                                                                    {
                                                                  std::cout<< "[OD]In Reg Pipe aaaaaaaa"<<std::endl;
                                                                  auto out_ptr = reinterpret_cast<const RawObstacleMsg *>(ptr);
                                                                  auto msg = std::make_shared<RawObstacleMsg>();
                                                                  *msg = *out_ptr;
                                                                  std::cout<< "[OD]In Reg Pipe"<<std::endl;
                                                                  this->publisher_->Pub(msg); });

      // std::shared_ptr<apaSlotListInfo> fanya::parking::ParkingslotDetectMoudle::saved_parking_slots_info_ptr = nullptr;
      CommAttr sub_comm_attr;
      sub_comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      subscriber_ = Subscriber<RawObstacleSerial>::New(
          sub_comm_attr, topic, 0, SimpleObstacleSubCallback, PROTOCOL_SHM);

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
      std::string front_camera_topic = "front_camera_fov195";
      std::string rear_camera_topic =  "rear_camera_fov195";
      std::string left_camera_topic =  "left_camera_fov195";
      std::string right_camera_topic = "right_camera_fov195";
      ErrorCode image_error_code;

      std::cout << "[OD]Before crete Image sub " << std::endl;

      // FRONT CAMERA
      front_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, front_camera_topic, 0, FrontCameraImageSubCallback, PROTOCOL_SHM);

      if (!front_camera_subscriber_)
      {
        std::cout << "[OD] create FRONT CAMERA subscriber failed"<<std::endl;
        return -1;
      }

      front_camera_publisher_ = Publisher<ImageSerial>::New(image_comm_attr,
                                                     front_camera_topic,
                                                     0,
                                                     // image_data_args.protocol,
                                                     PROTOCOL_SHM,
                                                     &image_error_code,
                                                     pub_connlisteners);
      if (!front_camera_publisher_)
      {
        std::cout << "[OD]create FRONT CAMERA publisher failed"<<std::endl;
        return -1;
      }
      // REAR CAMERA
      rear_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, rear_camera_topic, 0, RearCameraImageSubCallback, PROTOCOL_SHM);

      if (!rear_camera_subscriber_)
      {
        std::cout << "[OD] create REAR CAMERA subscriber failed"<<std::endl;
        return -1;
      }

      rear_camera_publisher_ = Publisher<ImageSerial>::New(image_comm_attr,
                                                     rear_camera_topic,
                                                     0,
                                                     // image_data_args.protocol,
                                                     PROTOCOL_SHM,
                                                     &image_error_code,
                                                     pub_connlisteners);
      if (!rear_camera_publisher_)
      {
        std::cout << "[OD]create REAR CAMERA publisher failed"<<std::endl;
        return -1;
      }

      // LEFT CAMERA
      left_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, left_camera_topic, 0, LeftCameraImageSubCallback, PROTOCOL_SHM);

      if (!left_camera_subscriber_)
      {
        std::cout << "[OD] create LEFT CAMERA subscriber failed"<<std::endl;
        return -1;
      }

      left_camera_publisher_ = Publisher<ImageSerial>::New(image_comm_attr,
                                                     left_camera_topic,
                                                     0,
                                                     // image_data_args.protocol,
                                                     PROTOCOL_SHM,
                                                     &image_error_code,
                                                     pub_connlisteners);
      if (!left_camera_publisher_)
      {
        std::cout << "[OD]create LEFT CAMERA publisher failed"<<std::endl;
        return -1;
      }

      // RIGHT CAMERA
      right_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, right_camera_topic, 0, RightCameraImageSubCallback, PROTOCOL_SHM);

      if (!right_camera_subscriber_)
      {
        std::cout << "[OD] create LEFT CAMERA subscriber failed"<<std::endl;
        return -1;
      }

      right_camera_publisher_ = Publisher<ImageSerial>::New(image_comm_attr,
                                                     right_camera_topic,
                                                     0,
                                                     // image_data_args.protocol,
                                                     PROTOCOL_SHM,
                                                     &image_error_code,
                                                     pub_connlisteners);
      if (!right_camera_publisher_)
      {
        std::cout << "[OD]create LEFT CAMERA publisher failed"<<std::endl;
        return -1;
      }


      std::cout << "[OD] After create Image sub" << std::endl;

      if (hb_mem_module_open() != 0)
      {
        std::cout << "[OD]hb_mem_module_open failed" << std::endl;
        return 1;
      }

      /************************PUB FROM J5***************************/
  return 0; 
}

int32_t PerceptionOdMoudle::Start() {
  
  senseAD::rscl::component::ComponentConfig cfg;
  cfg.config_file_path = "./bridge.yaml";
  cfg.timer_conf.set_interval_ms(1000);
  perception_OD_comp_->Initialize(cfg);

  return hobot::dataflow::Module::Start();   
}

int32_t PerceptionOdMoudle::Stop() { 
  perception_OD_comp_->Shutdown();
  printf("Finish Shutdown\n");
  hobot::communication::DeInit();
  hb_mem_module_close();
  return hobot::dataflow::Module::Stop(); 
}

void PerceptionOdMoudle::Reset() { hobot::dataflow::Module::Reset(); }

int32_t PerceptionOdMoudle::DeInit()
{
  return hobot::dataflow::Module::DeInit();
}

void PerceptionOdMoudle::MsgCenterProc(
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

  auto &sub_camera_frame_array_msgs =
      msgs[proc->GetResultIndex("sub_camera_frame_array")];
  for (auto &msg : *(sub_camera_frame_array_msgs.get())) {
    if (nullptr == msg) {
      continue;
    }
    DFHLOG_I("sub_camera_frame_array msg timestamp: {}",
             msg->GetGenTimestamp());

  }

  // GET FRONT/REAR/LEFT/RIGHT CAMERA IMAGE
  std::cout << "[OD]ImageReceiveProc start" << std::endl;
  auto msg_list = msgs[proc->GetResultIndex("sub_camera_frame_array")];
  auto gen_image_ts = GetTimeStamp();
  std::cout << "[OD]msg_list size: " << msg_list->size() << std::endl;
  if (!msg_list->empty()){
    std::shared_ptr<CameraFrameArrayProtoMsg> camera_group_msg =
        std::dynamic_pointer_cast<CameraFrameArrayProtoMsg>(msg_list->front());


    // spImageProtoMsg ipm_msg = std::make_shared<ImageProtoMsg>();
    // // fill ipm msg
    // FillArrayMsgToIpmBuffer(ipm_msg, camera_group_msg);

    // address_info_t addr_info;
    // if (!GetImageProtoAddrInfo(ipm_msg, addr_info))
    // {
    //    DFHLOG_E("Get image proto addr info failed.");
    //    return;
    // }
    // int32_t width = addr_info.width;
    // int32_t height = addr_info.height;

    // cv::Mat image_nv12(height * 1.5, width, CV_8UC1);
    // cv::Mat rgb_mat(height, width, CV_8UC3);
    // int32_t size;
    // size = width * height;

    // int32_t new_width = 640;
    // int32_t new_height = 384;

    // auto y_addr = image_nv12.data;
    // auto u_addr = y_addr + size;
    // auto v_addr = u_addr + size / 4;
    // auto srcBuf = addr_info.addr[0];
    // auto srcBuf1 = addr_info.addr[1];
    // std::memcpy(y_addr, srcBuf, size);
    // for (int i = 0; i < size / 4; ++i)
    // {
    //   *u_addr++ = *srcBuf1++;
    //   *v_addr++ = *srcBuf1++;
    // }

    // cv::cvtColor(image_nv12, rgb_mat, cv::COLOR_YUV2BGR_NV12);
    // cv::resize(rgb_mat, resizedMat, cv::Size(new_width, new_height));
    // cv::cvtColor(resizedMat, NV12ResizedMat, cv::COLOR_BGR2YUV_I420);
    // uint64_t addr_value = reinterpret_cast<uint64_t>(NV12ResizedMat.data);

    auto len = camera_group_msg->proto.camera_frame_size();
    std::cout<<"[OD] camera_frame_size: " << len <<std::endl;
    for (auto i = 0; i < len; i++) {
      auto cam_id_str = camera_group_msg->proto.camera_frame(i).cam_id();
      auto camera_proto_msg = camera_arry_msg_convert(camera_group_msg, cam_id_str);
      
      if (camera_proto_msg != nullptr) {
        camera_msg_convert(camera_proto_msg);
        // std::cout << "[OD]camera_proto_msg->proto.DebugString():" << camera_proto_msg->proto.DebugString().c_str() << std::endl;
        if (camera_proto_msg->proto.pym_img_info().down_scale_num() >= 1 &&
            camera_proto_msg->proto.pym_img_info().down_scale_size() >= 1) {
          
        const camera_frame::YuvAddrInfo &yuv_proto = camera_proto_msg->proto.pym_img_info().down_scale(0);
        auto pym_buf = reinterpret_cast<char *>(yuv_proto.y_vaddr());
        auto width = yuv_proto.width();
        auto height = yuv_proto.height();
        cv::Mat image_nv12(height * 1.5, width, CV_8UC1);
        cv::Mat rgb_mat(height, width, CV_8UC3);
        int32_t size;
        size = width * height*1.5;
        int32_t new_width = 640;
        int32_t new_height = 384;

        auto yuv_addr = image_nv12.data;
        std::memcpy(yuv_addr, pym_buf, size);

        cv::cvtColor(image_nv12, rgb_mat, cv::COLOR_YUV2BGR_NV12);
        cv::resize(rgb_mat, resizedMat, cv::Size(new_width, new_height));
        cv::cvtColor(resizedMat, NV12ResizedMat, cv::COLOR_BGR2YUV_I420);
        uint64_t addr_value = reinterpret_cast<uint64_t>(NV12ResizedMat.data);

        auto image = std::make_shared<ImageMsg>();
        rd::Time image_time;
        image_time.set_nanosec(gen_image_ts);
        rd::Header od_header;
        od_header.mutable_timestampns()->CopyFrom(image_time);
        image->proto.mutable_header()->CopyFrom(od_header);

        if (cam_id_str == "camera_0"){
          NV12ResizedMat_front = NV12ResizedMat.clone();
          image->proto.set_height(new_height);
          image->proto.set_width(new_width);
          image->proto.set_phyaddr(addr_value);
          image->proto.set_viraddr(addr_value);
          image->proto.set_step(1.5);
          std::cout << "[OD]ImageReceiveProc cameraID " << cam_id_str << std::endl;
          std::cout << "[OD] timestamp " << gen_image_ts <<std::endl;
          std::cout << "[OD] HEIGHT " << new_height <<std::endl;
          std::cout << "[OD] WIDTH " << new_width <<std::endl;
          std::cout << "[OD] PADDR " <<  yuv_proto.y_paddr() <<std::endl;
          // std::cout << "[OD] VADDR " << addr_value <<std::endl;
          std::cout << "[OD] STEP " << camera_proto_msg->proto.pym_img_info().down_scale(0).step() <<std::endl;
          front_camera_publisher_->Pub(image);
        }else if (cam_id_str == "camera_1"){
          NV12ResizedMat_rear = NV12ResizedMat.clone();
          image->proto.set_height(new_height);
          image->proto.set_width(new_width);
          image->proto.set_phyaddr(addr_value);
          image->proto.set_viraddr(addr_value);
          image->proto.set_step(1.5);
          std::cout << "[OD]ImageReceiveProc cameraID " << cam_id_str << std::endl;
          std::cout << "[OD] timestamp " << gen_image_ts <<std::endl;
          std::cout << "[OD] HEIGHT " << new_height <<std::endl;
          std::cout << "[OD] WIDTH " << new_width <<std::endl;
          std::cout << "[OD] PADDR " <<  yuv_proto.y_paddr() <<std::endl;
          // std::cout << "[OD] VADDR " << addr_value <<std::endl;
          std::cout << "[OD] STEP " << camera_proto_msg->proto.pym_img_info().down_scale(0).step() <<std::endl;

          rear_camera_publisher_->Pub(image);
        }else if (cam_id_str == "camera_2"){
          NV12ResizedMat_left = NV12ResizedMat.clone();
          image->proto.set_height(new_height);
          image->proto.set_width(new_width);
          image->proto.set_phyaddr(addr_value);
          image->proto.set_viraddr(addr_value);
          image->proto.set_step(1.5);
          std::cout << "[OD]ImageReceiveProc cameraID " << cam_id_str << std::endl;
          std::cout << "[OD] timestamp " << gen_image_ts <<std::endl;
          std::cout << "[OD] HEIGHT " << new_height <<std::endl;
          std::cout << "[OD] WIDTH " << new_width <<std::endl;
          std::cout << "[OD] PADDR " <<  yuv_proto.y_paddr() <<std::endl;
          // std::cout << "[OD] VADDR " << addr_value <<std::endl;
          std::cout << "[OD] STEP " << camera_proto_msg->proto.pym_img_info().down_scale(0).step() <<std::endl;

          left_camera_publisher_->Pub(image);
        }else if (cam_id_str == "camera_3"){
          NV12ResizedMat_right = NV12ResizedMat.clone();
          image->proto.set_height(new_height);
          image->proto.set_width(new_width);
          image->proto.set_phyaddr(addr_value);
          image->proto.set_viraddr(addr_value);
          image->proto.set_step(1.5);
          std::cout << "[OD]ImageReceiveProc cameraID " << cam_id_str << std::endl;
          std::cout << "[OD] timestamp " << gen_image_ts <<std::endl;
          std::cout << "[OD] HEIGHT " << new_height <<std::endl;
          std::cout << "[OD] WIDTH " << new_width <<std::endl;
          std::cout << "[OD] PADDR " <<  yuv_proto.y_paddr() <<std::endl;
          // std::cout << "[OD] VADDR " << addr_value <<std::endl;
          std::cout << "[OD] STEP " << camera_proto_msg->proto.pym_img_info().down_scale(0).step() <<std::endl;

          right_camera_publisher_->Pub(image);
        }
    }
        // if (camera_proto_msg->proto.pym_img_info().down_scale_num() >= 1 &&
        //     camera_proto_msg->proto.pym_img_info().down_scale_size() >= 1) {
        //     auto frame_id = camera_proto_msg->proto.pym_img_info().frame_seq();
        //     std::cout << "[OD]ImageReceiveProc receive " << cam_id_str << " image, frame id " << frame_id << " w:" 
        //                                                  << camera_proto_msg->proto.pym_img_info().down_scale(0).width() << " h:" 
        //                                                  << camera_proto_msg->proto.pym_img_info().down_scale(0).height() << " step:" 
        //                                                  << camera_proto_msg->proto.pym_img_info().down_scale(0).step() << std::endl;
        
        //   }
      }
    }
  }
}

void PerceptionOdMoudle::TimerProc(hobot::dataflow::spMsgResourceProc proc,
                                   const hobot::dataflow::MessageLists &msgs) {
  auto gen_ts = GetTimeStamp();
  // pub ObstaclesMsg
  {
    // fill proto
    od::Time od_time;
    od_time.set_nanosec(123);
    od::Header od_header;
    od_header.set_seq(1);
    od_header.set_frameid("99");
    od_header.mutable_timestampns()->CopyFrom(od_time);

    if (obstacles.obstaclesraw.size()!=0){
      for (int icnt = 0; icnt < obstacles.obstaclesraw.size(); icnt++){
        od::ObstacleRaw od_obstacleRaw;
        // od_obstacleRaw.set_camera("camera_name_test");
        od_obstacleRaw.set_label(obstacles.obstaclesraw[icnt].label);
        od_obstacleRaw.set_typeconfidence(obstacles.obstaclesraw[icnt].typeConfidence);
        od_obstacleRaw.set_existenceconfidence(obstacles.obstaclesraw[icnt].existenceConfidence);

        for (int jcnt = 0; jcnt < obstacles.obstaclesraw[icnt].landmark4.size(); jcnt++){
          od::Point2f od_point2f;
          od_point2f.set_x(obstacles.obstaclesraw[icnt].landmark4[jcnt].x);
          od_point2f.set_y(obstacles.obstaclesraw[icnt].landmark4[jcnt].y);
          std::cout<<"[OD] Before pub to S32g landmark4 x: " << obstacles.obstaclesraw[icnt].landmark4[jcnt].x << ", y: " << obstacles.obstaclesraw[icnt].landmark4[jcnt].y <<std::endl;
          od_obstacleRaw.add_landmark4()->CopyFrom(od_point2f);
        }

        for (int jcnt = 0; jcnt < obstacles.obstaclesraw[icnt].landmark4Scores.size(); jcnt++){
          od_obstacleRaw.add_landmark4scores(obstacles.obstaclesraw[icnt].landmark4Scores[jcnt]);
        }
        // create msg
        auto obstacles_msg = std::make_shared<ObstaclesMsg>();
        obstacles_msg->proto.mutable_header()->CopyFrom(od_header);
   
        obstacles_msg->proto.add_rawobjects()->CopyFrom(od_obstacleRaw);

        obstacles_msg->SetGenTimestamp(gen_ts);

        auto pub_obstacles_s32g = proc->GetOutputPort("pub_obstacles");
        if (!pub_obstacles_s32g) {
          DFHLOG_E("pub_obstacles_s32g failed to get output port of {}",
                   "pub_obstacles");
          return;
        }

        // send to s32g
        pub_obstacles_s32g->Send(obstacles_msg);
        auto obstacles_proto = obstacles_msg->proto;
        auto temp_header = obstacles_proto.mutable_header();
        DFHLOG_W("pub pub_obstacles_s32g info, obstacles header frame id = {}",
                  temp_header->frameid());

      }
    }  
  }

  // pub FSLineMsg
  {
    // fill proto
    // od::Point2f od_point2f;
    // od_point2f.set_x(1);
    // od_point2f.set_y(1);

    // od::Time od_time;
    // od_time.set_nanosec(123);

    // od::FSLinePoint od_fsline_point;
    // od_fsline_point.mutable_coordinate()->CopyFrom(od_point2f);
    // od_fsline_point.set_pointlabel(od::SpaceLabel::vehicle);

    // od::Header od_header;
    // od_header.set_seq(1);
    // od_header.set_frameid("99");
    // od_header.mutable_timestampns()->CopyFrom(od_time);

    // od::FSLinesimple od_fsline_simple;
    // od_fsline_simple.mutable_header()->CopyFrom(od_header);
    // od_fsline_simple.set_frametimestampns(123);
    // od_fsline_simple.add_fslinepoints()->CopyFrom(od_fsline_point);

    // auto fsline_msg = std::make_shared<FSLineMsg>();
    // fsline_msg->proto.add_fsline()->CopyFrom(od_fsline_simple);
    // fsline_msg->SetGenTimestamp(gen_ts);
    // auto pub_fsline_msg_port_s32g = proc->GetOutputPort("pub_fsline_msg");
    // if (!pub_fsline_msg_port_s32g) {
    //   DFHLOG_E("failed to get output port of {}", "pub_fsline_msg");
    //   return;
    // }
    // pub_fsline_msg_port_s32g->Send(fsline_msg);
    // DFHLOG_W("Pub od_fsline_msg,Success!!! ");
  }
}

    DATAFLOW_REGISTER_MODULE(PerceptionOdMoudle)

} // namespace parking
} // namespace fanya