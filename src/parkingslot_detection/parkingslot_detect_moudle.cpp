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
#include "parkingslot_detection/parkingslot_detect_moudle.h"
#include <functional>
#include <vector>
#include <string>

#include "common/utils.h"
#include "parkingslot_detection/dependencies/j5/j5dvb_system/include/vio/hb_vpm_data_info.h"

#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"

// #include "rscl/pub_component.h"
// #include "rscl/sub_component.h"
#include "ad_msg_bridge/manager/inner_process_communication_pipe.h"
#include "communication/publisher.h"
#include "communication/subscriber.h"
#include "communication/common/comm_log.h"
#include "communication/wait_set/scope_guard.h"
#include <communication/common/types.h>
#include "communication/common/compiler_features.h"
#include "hb_mem_mgr.h"
#include "proto_msg_all.h"

// #include "protocol_msg/psd.pb.h"
#include "protocol_msg/simple_image.pb.h"

/**
 * To resolve unused warning, you can delete
 * the marco after variables actually used.
 */
#define UNUSED(x) (void)x
#define TEST 1
#define TEST_1 0

namespace fanya
{
  namespace parking
  {

    int save_pred_img(apaSlotListInfo parking_slots, std::vector<uchar> &buffer, cv::Mat bgr_mat);

#if TEST
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

    using ImageSerial = ProtobufSerializer<psd::Image>;
    using ImageMsg = ProtoMsg<psd::Image>;
    using QuadpldSerial = ProtobufSerializer<psd::SApaPSInfo>;
    using QuadpldMsg = ProtoMsg<psd::SApaPSInfo>;

    // using senseAD::msg::perception::QuadParkingSlots;

    struct Args
    {
      int participant_id = 0;
      int protocol = PROTOCOL_INVALID;
      // the speed of different protocols is different, to avoid lose message, some
      // protocol must sleep a while during between each request
      int pub_interval = 0;
      bool is_dynamic = false;
    };
#endif

    static apaSlotListInfo saved_parking_slots_info;

    static void SimpleSlotSubCallback(const std::shared_ptr<QuadpldMsg> &quadpld_msg)
    {
      std::cout << "[PSD]j5 callback arriverd!" << std::endl;
      std::cout << "[PSD]Slot size:" << quadpld_msg->proto.worldoutrect_size() << std::endl;
      std::cout << "[PSD]Slot frameid:" << quadpld_msg->proto.ullframeid() << std::endl;

      saved_parking_slots_info.ullFrameId = quadpld_msg->proto.ullframeid();
      std::cout << "[PSD]GLOBAL_Slot ullFrameId:" << saved_parking_slots_info.ullFrameId << std::endl;

      saved_parking_slots_info.WorldoutRect.resize(quadpld_msg->proto.worldoutrect_size());
      if (saved_parking_slots_info.WorldoutRect.size() != 0)
      {
        for (int i = 0; i < saved_parking_slots_info.WorldoutRect.size(); i++)
        {
          saved_parking_slots_info.WorldoutRect[i].rectInfo.label = quadpld_msg->proto.worldoutrect(i).label();
          saved_parking_slots_info.WorldoutRect[i].rectInfo.iRectType = quadpld_msg->proto.worldoutrect(i).islottype();

          for (int j = 0; j < 4; j++)
          {
            saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].x = quadpld_msg->proto.worldoutrect(i).pt(j).x();
            saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].y = quadpld_msg->proto.worldoutrect(i).pt(j).y();
            std::cout << "[PSD]GLOBAL_Slot x:" << saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].x << std::endl;
            std::cout << "[PSD]GLOBAL_Slot y:" << saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].y << std::endl;
          }
        }
      }
      if (quadpld_msg->proto.worldoutrect_size() != 0)
      {
        for (int i = 0; i < quadpld_msg->proto.worldoutrect_size(); i++)
        {

          std::cout << "[PSD]Slot recttype:" << quadpld_msg->proto.worldoutrect(i).label() << std::endl;
          std::cout << "[PSD]Slot label:" << quadpld_msg->proto.worldoutrect(i).islottype() << std::endl;
          for (int j = 0; j < 4; j++)
          {
            std::cout << "[PSD]Slot x:" << quadpld_msg->proto.worldoutrect(i).pt(j).x() << std::endl;
            std::cout << "[PSD]Slot y:" << quadpld_msg->proto.worldoutrect(i).pt(j).y() << std::endl;
          }
        }
      }
    }

    static void SimpleImageSubCallback(const std::shared_ptr<ImageMsg> &image_msg)
    {
      std::cout << "[PSD]j5 image callback arriverd!" << std::endl;
      senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->NotifyPipe("/sensor/camera/ipm_stitc", image_msg.get());
      std::cout << "[PSD]j5 image callback sucess!" << std::endl;
    }

    ParkingslotDetectMoudle::ParkingslotDetectMoudle(
        const hobot::dataflow::ModuleOption &module_option) : hobot::dataflow::Module(module_option)
    {
    }

    void ParkingslotDetectMoudle::InitPortsAndProcs()
    {
      DF_MODULE_INIT_IDL_INPUT_PORT(
          "sub_apa_status",
          aph::apa_status);
      DF_MODULE_INIT_IDL_INPUT_PORT(
          "sub_pad_point",
          loc::padPoint);
      DF_MODULE_INIT_IDL_INPUT_PORT(
          "sub_pad_vehicle_pose",
          loc::padVehiclePose);
      DF_MODULE_INIT_IDL_INPUT_PORT(
          "sub_camera_frame_array",
          camera_frame::CameraFrameArray);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_apa_ps_info",
          psd::SApaPSInfo);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_apa_ps_rect",
          psd::SApaPSRect);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_apa_pointI",
          psd::SApaPoint_I);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_psd_image",
          psd::Image);

      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "percept_debug",
          ImageProto::Image);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "percept_debug2",
          ImageProto::Image);
      DF_MODULE_INIT_IDL_OUTPUT_PORT(
          "pub_apa_ps_info_s32g",
          psd::SApaPSInfo);
      DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
          "MsgCenterProc",
          ParkingslotDetectMoudle,
          MsgCenterProc,
          hobot::dataflow::ProcType::DF_MSG_COND_PROC,
          DF_VECTOR("sub_apa_status", "sub_pad_point", "sub_pad_vehicle_pose", "sub_camera_frame_array"),
          DF_VECTOR());

      DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
          "TimerProc",
          ParkingslotDetectMoudle,
          TimerProc,
          hobot::dataflow::ProcType::DF_MSG_TIMER_PROC,
          DF_VECTOR(),
          DF_VECTOR("percept_debug", "pub_apa_ps_rect", "pub_apa_ps_info", "pub_apa_pointI", "pub_psd_image", "pub_apa_ps_info_s32g"));
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

    bool MatchParticipantId(Args &args, CommAttr &comm_attr)
    {
      switch (args.protocol)
      {
      case PROTOCOL_ZMQ_TCP:
        if (!args.is_dynamic)
        {
          if (args.participant_id == 0)
            comm_attr.participant_attrs_.push_back(ParticipantAttr{2});
          else
            comm_attr.participant_attrs_.push_back(
                ParticipantAttr{args.participant_id});
        }
        args.pub_interval = 10;
        return true;
      case PROTOCOL_ZMQ_IPC:
        if (!args.is_dynamic)
        {
          if (args.participant_id == 0)
            comm_attr.participant_attrs_.push_back(ParticipantAttr{1});
          else
            comm_attr.participant_attrs_.push_back(
                ParticipantAttr{args.participant_id});
        }
        args.pub_interval = 10;
        return true;
      case PROTOCOL_SHM:
        if (args.is_dynamic)
        {
          args.pub_interval = 10;
          return true;
        }
        std::cout << "using protocol shm must turn on dynamic discovery"
                  << std::endl;
        return false;
      default:
        std::cout << "invalid protocol type: " << args.protocol << std::endl;
        return false;
      }
    }

    // ParkingslotDetectMoudle::saved_parking_slots_info_ptr = std::make_shared<apaSlotListInfo>();
    // std::shared_ptr<apaSlotListInfo> Parkisaved_parking_slots_info_ptr = nullptr;

    int32_t ParkingslotDetectMoudle::Init()
    {

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
      // std::string process_name = "j5 test node";
      rt->Init(argc, argv);
      DFHLOG_I("Init Component");
      // #if TEST_1

      // e_ = std::make_shared<PubComponent>("j5");
      perceptioncomp_ = std::make_shared<senseAD::avp_perception::PerceptionRdComponent>();

      CommAttr comm_attr;
      comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      std::string topic = "/parking/perception/camera/parking_bboxes";
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

      // std::shared_ptr<apaSlotListInfo> fanya::parking::ParkingslotDetectMoudle::saved_parking_slots_info_ptr = nullptr;
      CommAttr sub_comm_attr;
      sub_comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      subscriber_ = Subscriber<QuadpldSerial>::New(
          sub_comm_attr, topic, 0, SimpleSlotSubCallback, PROTOCOL_SHM);

      // std::cout<<"[PSDTEST] TEST ullframeid:"<<subscriber_->Take().get()->proto.ullframeid();

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

    int32_t ParkingslotDetectMoudle::Start()
    {
      senseAD::rscl::component::ComponentConfig cfg;
      cfg.config_file_path = "./bridge.yaml";
      cfg.timer_conf.set_interval_ms(1000);
      // e_->Initialize(cfg);
      perceptioncomp_->Initialize(cfg);
      // perceptioncomp_->OnInit();
      return hobot::dataflow::Module::Start();
    }

    int32_t ParkingslotDetectMoudle::Stop()
    {
      // e_->Shutdown();
      perceptioncomp_->Shutdown();
      printf("Finish Shutdown\n");
      hobot::communication::DeInit();
      hb_mem_module_close();
      return hobot::dataflow::Module::Stop();
    }

    void ParkingslotDetectMoudle::Reset()
    {
      hobot::dataflow::Module::Reset();
    }

    int32_t ParkingslotDetectMoudle::DeInit()
    {
      return hobot::dataflow::Module::DeInit();
    }

    void ParkingslotDetectMoudle::MsgCenterProc(
        hobot::dataflow::spMsgResourceProc proc,
        const hobot::dataflow::MessageLists &msgs)
    {
      DFHLOG_I("MsgCenterProc starts");
      auto &sub_apa_status_msgs = msgs[proc->GetResultIndex("sub_apa_status")];
      for (auto &msg : *(sub_apa_status_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("sub_apa_status msg timestamp: {}",
                 msg->GetGenTimestamp());
        // process msg of sub_apa_status
        auto apa_status = std::dynamic_pointer_cast<ApaStatusMsg>(sub_apa_status_msgs->at(0));
        if (apa_status && apa_status->proto.has_m_current_apa_status())
        {
          DFHLOG_I("sub_apa_status msg timestamp: {}, apa_status = {}",
                   msg->GetGenTimestamp(), apa_status->proto.m_current_apa_status());
        }
      }
      auto &sub_pad_point_msgs = msgs[proc->GetResultIndex("sub_pad_point")];
      for (auto &msg : *(sub_pad_point_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("sub_pad_point msg timestamp: {}",
                 msg->GetGenTimestamp());
        // process msg of sub_pad_point
      }
      auto &sub_pad_vehicle_pose_msgs = msgs[proc->GetResultIndex("sub_pad_vehicle_pose")];
      for (auto &msg : *(sub_pad_vehicle_pose_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("sub_pad_vehicle_pose msg timestamp: {}",
                 msg->GetGenTimestamp());
        // process msg of sub_pad_vehicle_pose
      }
      auto &sub_camera_frame_array_msgs = msgs[proc->GetResultIndex("sub_camera_frame_array")];

      DFHLOG_I("sub_camera_frame_array_msgs:");
      for (auto &msg : *(sub_camera_frame_array_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("sub_camera_frame_array msg timestamp: {}",
                 msg->GetGenTimestamp());
        // process msg of sub_camera_array
        std::shared_ptr<CameraFrameArrayProtoMsg> camera_array_msg = std::dynamic_pointer_cast<CameraFrameArrayProtoMsg>(msg);

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
          // int32_t resized_bgr_width = resized_bgr_addr_info.width;
          // int32_t resized_bgr_height = resized_bgr_addr_info.height;

          std::cout << "[PSD]Image height:" << new_height << std::endl;
          std::cout << "[PSD]Image width:" << new_width << std::endl;
          std::cout << "[PSD]Image paddr:" << addr_info.paddr[0] << std::endl;
          std::cout << "[PSD]Image vaddr:" << addr_value << std::endl;

          image->proto.set_height(new_height);
          image->proto.set_width(new_width);

          // image->proto.set_encoding(10);
          image->proto.set_phyaddr(addr_value);
          image->proto.set_memtype(0);
          image->proto.set_viraddr(addr_value);
          auto ret = image_publisher_->Pub(image);
          std::cout << "[PSD] After Image pub" << std::endl;
          if (saved_parking_slots_info.WorldoutRect.size() > 0)
          {
            std::cout << "[PSD]GLOBAL_Slot recttype:" << saved_parking_slots_info.WorldoutRect[0].rectInfo.label << std::endl;
            std::cout << "[PSD]GLOBAL_Slot label:" << saved_parking_slots_info.WorldoutRect[0].rectInfo.iRectType << std::endl;
          }

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
    }

    void ParkingslotDetectMoudle::TimerProc(
        hobot::dataflow::spMsgResourceProc proc,
        const hobot::dataflow::MessageLists &msgs)
    {
      auto gen_ts = GetTimeStamp();
      { // do something with output port pub_apa_ps_info
        // fill proto
        auto apa_ps_info = std::make_shared<SApaPSInfoMsg>();
        apa_ps_info->proto.set_ullframeid(88);
        apa_ps_info->SetGenTimestamp(gen_ts);
        // pub msg
        auto pub_apa_ps_info_port = proc->GetOutputPort("pub_apa_ps_info");
        if (!pub_apa_ps_info_port)
        {
          // DFHLOG_E("failed to get output port of {}", "pub_apa_ps_info");
          return;
        }
        pub_apa_ps_info_port->Send(apa_ps_info);
        DFHLOG_I("pub pub_apa_ps_info_port info, ullframeid = {}",
                 apa_ps_info->proto.ullframeid());
      }

      { // do something with output port pub_apa_ps_rect
        // fill proto
        auto apa_ps_rect = std::make_shared<SApaPSRectMsg>();
        apa_ps_rect->proto.set_label(88);
        apa_ps_rect->SetGenTimestamp(gen_ts);
        // pub msg
        auto pub_apa_ps_rect_port = proc->GetOutputPort("pub_apa_ps_rect");
        if (!pub_apa_ps_rect_port)
        {
          // DFHLOG_E("failed to get output port of {}", "pub_apa_ps_rect");
          return;
        }
        pub_apa_ps_rect_port->Send(apa_ps_rect);
        DFHLOG_I("pub apa_ps_rect_port info, label = {}",
                 apa_ps_rect->proto.label());
      }

      { // do something with output port pub_apa_pointI
        // fill proto
        auto apa_pointI = std::make_shared<SApaPoint_IMsg>();
        apa_pointI->proto.set_x(88);
        apa_pointI->SetGenTimestamp(gen_ts);
        // pub msg
        auto pub_apa_pointI_port = proc->GetOutputPort("pub_apa_pointI");
        if (!pub_apa_pointI_port)
        {
          DFHLOG_E("failed to get output port of {}", "pub_apa_pointI");
          return;
        }
        pub_apa_pointI_port->Send(apa_pointI);
        DFHLOG_I("pub apa_pointI_port info, x = {}",
                 apa_pointI->proto.x());
      }

      { // do something with output port pub_psd_image
        // fill proto
        auto image = std::make_shared<ImageMsg>();
        image->proto.set_width(500);
        image->SetGenTimestamp(gen_ts);
        // pub msg
        auto pub_image_port = proc->GetOutputPort("pub_psd_image");
        if (!pub_image_port)
        {
          DFHLOG_E("failed to get output port of {}", "pub_psd_image");
          return;
        }
        pub_image_port->Send(image);

        // image->proto.set_height(500);
        // image->proto.set_encoding(10);

        // auto ret = image_publisher_->Pub(image);
        // std::cout << "[PSDTEST]after Image pub"<<std::endl;
        // if (ret != COMM_CODE_OK) {
        //     std::cout << "[PSDTEST]pub failed, reason: " << ErrorMsg(ret) << std::endl;
        // }else{
        //     std::cout << "[PSDTEST]pub Image successful! " << std::endl;
        // }
        DFHLOG_I("pub image_port info, width = {}",
                 image->proto.width());
      }

      { // PUB FROM J5 TO S32G
        // do something with output port pub_apa_ps_info
        // fill proto vector<int> fusion_out_rect_id;
        static int slot_id = 0;
        auto apa_ps_info = std::make_shared<SApaPSInfoMsg>();
        apa_ps_info->proto.set_ullframeid(saved_parking_slots_info.ullFrameId);
        std::cout << "*********fusion_out size**********" << saved_parking_slots_info.WorldoutRect.size() << std::endl;
        // 一级子结构体 repeated为 数组形式
        if (saved_parking_slots_info.WorldoutRect.size() != 0)
        {
          for (int i = 0; i < saved_parking_slots_info.WorldoutRect.size(); i++)
          {
            psd::SApaPSRect *sapa_ps_rect = apa_ps_info->proto.add_worldoutrect();
            sapa_ps_rect->set_iscenetype(i);
            sapa_ps_rect->set_label(saved_parking_slots_info.WorldoutRect[i].rectInfo.iRectType);
            sapa_ps_rect->set_pstype(static_cast<psd::Pstype>(saved_parking_slots_info.WorldoutRect[i].rectInfo.label));
            for (int j = 0; j < 4; j++)
            {
              // 二级子结构体 repeated为 数组形式
              psd::SApaPoint_I *sapa_point = sapa_ps_rect->add_pt();
              sapa_point->set_x(saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].x);
              sapa_point->set_y(saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].y);
              std::cout << "666 coord.x: " << saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].x << std::endl;
              std::cout << "667 coord.y: " << saved_parking_slots_info.WorldoutRect[i].rectInfo.pt[j].y << std::endl;
            }
          }
        }

        // pub msg
        //  auto pub_apa_ps_info_port = proc->GetOutputPort("pub_apa_ps_info");
        //  if (!pub_apa_ps_info_port) {
        //    DFHLOG_E("failed to get output port of {}", "pub_apa_ps_info");
        //    return;
        //  }

        auto pub_apa_ps_rect_port_s32g = proc->GetOutputPort("pub_apa_ps_info_s32g");
        if (!pub_apa_ps_rect_port_s32g)
        {
          DFHLOG_E("pub_apa_ps_rect_port_s32g failed to get output port of {}", "pub_apa_ps_rect");
          return;
        }

        // pub_apa_ps_info_port->Send(apa_ps_info);
        pub_apa_ps_rect_port_s32g->Send(apa_ps_info);
        DFHLOG_W("pub pub_apa_ps_info_port info, ullframeid = {}",
                 apa_ps_info->proto.ullframeid());
      }

      // SEND TO HVIZ
      std::vector<uchar> buffer;
      // std::cout<<"Detect_Cornerpoint_gpsd"<<__LINE__<<std::endl;
      //  save_pred_img_point(image_input->rgb_mat, addr_info.height, addr_info.width,output->incoming_pts_P,filename,output->nums_output, buffer);
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

    int save_pred_img(apaSlotListInfo parking_slots, std::vector<uchar> &buffer, cv::Mat rgb_mat)
    {

      cv::Mat bgr_mat;
      cv::cvtColor(rgb_mat, bgr_mat, cv::COLOR_YUV2BGR_NV12);
      int height = bgr_mat.rows;
      int width = bgr_mat.cols;
      int size = bgr_mat.cols * bgr_mat.rows;

      cv::Mat final_mat = bgr_mat.clone();

      for (int i = 0; i < parking_slots.WorldoutRect.size(); i++)
      {
        auto output_result = parking_slots.WorldoutRect[i].rectInfo;
        if (true)
        {
          cv::circle(final_mat, cv::Point(output_result.pt[0].x, output_result.pt[0].y), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
          cv::circle(final_mat, cv::Point(output_result.pt[1].x, output_result.pt[1].y), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
          cv::line(final_mat, cv::Point(output_result.pt[0].x, output_result.pt[0].y), cv::Point(output_result.pt[3].x, output_result.pt[3].y), cv::Scalar(0, 255, 0), 1);
          cv::line(final_mat, cv::Point(output_result.pt[1].x, output_result.pt[1].y), cv::Point(output_result.pt[2].x, output_result.pt[2].y), cv::Scalar(0, 255, 0), 1);
          cv::line(final_mat, cv::Point(output_result.pt[0].x, output_result.pt[0].y), cv::Point(output_result.pt[1].x, output_result.pt[1].y), cv::Scalar(0, 0, 255), 1);
        }
      }
      cv::imencode(".jpg", final_mat, buffer);
      //  cv::imwrite(target_image_file, final_mat);
      return 0;
    }

    DATAFLOW_REGISTER_MODULE(ParkingslotDetectMoudle)

  } // namespace parking
} // namespace fanya