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
#include <cmath>
#include <ctime>
#include <yaml-cpp/yaml.h>
#include <mutex>  // For thread safety

#include "common/proto_msg_all.h"
#include "common/timestamp.h"
#include "communication/common/types.h"
#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"
#include "message/proto/proto_serializer.hpp"

#include "common/utils.h"
#include "perception_od/dependencies/j5/j5dvb_system/include/vio/hb_vpm_data_info.h"
#include "perception_od/dependencies/j5/rscl/include/utils/CameraModel.hpp"
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
    int save_pred_img_OD(FSLine fs,Obstacles obstaclesOD, std::vector<uchar> &buffer, cv::Mat rgb_mat_front, cv::Mat rgb_mat_left, cv::Mat rgb_mat_rear, cv::Mat rgb_mat_right);
    // int save_pred_img_FSLine(FSLine fs, std::vector<uchar> &buffer, cv::Mat rgb_mat_front, cv::Mat rgb_mat_left, cv::Mat rgb_mat_rear, cv::Mat rgb_mat_right);
    std::string camera_model_path = "/app/parking/output/parking/lib/perception_alg/opt/senseauto/senseauto-vehicle-config/vehicle/GL8-001/";
    std::map<std::string, CameraModelBasePtr> camera_models;

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

    FSLine fs;
    static int64_t fs_seq_ = 0;
    static void SimpleFSLineSubCallback(const std::shared_ptr<FSLineMsg> &fsline_msg)
    {
       std::cout << "[FSLine]j5 callback arrived!" << std::endl;

        // Get the size of the fsline
        int fsline_size = fsline_msg->proto.fsline_size();
        std::cout << "[FSLine]fsline size: " << fsline_size << std::endl;

        if (fsline_size > 0)
        {
            // Resize the fsline vector to match the message size
            fs.fsline.resize(fsline_size);

            for (int icnt = 0; icnt < fsline_size; ++icnt)
            {
                // Get a reference to the current fsline entry
                const auto& proto_fsline = fsline_msg->proto.fsline(icnt);

                // Set the header values
                fs.fsline[icnt].header.timestamp = proto_fsline.header().timestampns().nanosec();
                fs.fsline[icnt].header.frame_id = "1"; // Hardcoded as "1", change if necessary
                fs.fsline[icnt].header.seq = fs_seq_++;
                
                // Log the header information
                std::cout << "[FSLine] header.timestamp: " << fs.fsline[icnt].header.timestamp << std::endl;
                std::cout << "[FSLine] header.seq: " << fs.fsline[icnt].header.seq << std::endl;
                std::cout << "[FSLine] header.frame_id: " << fs.fsline[icnt].header.frame_id << std::endl;

                // Get the number of fsLinepoints
                int fslinepoints_size = proto_fsline.fslinepoints_size();
                std::cout << "[FSLine] fsline[" << icnt << "] fslinepoints_size: " << fslinepoints_size << std::endl;

                if (fslinepoints_size > 0)
                {
                    // Resize the fsLinepoints vector to match the message size
                    fs.fsline[icnt].fsLinepoints.resize(fslinepoints_size);

                    for (int jcnt = 0; jcnt < fslinepoints_size; ++jcnt)
                    {
                        // Get a reference to the current fsLinepoint
                        const auto& proto_point = proto_fsline.fslinepoints(jcnt);

                        // Set the coordinate and pointLabel values
                        fs.fsline[icnt].fsLinepoints[jcnt].coordinate.x = proto_point.coordinate().x();
                        fs.fsline[icnt].fsLinepoints[jcnt].coordinate.y = proto_point.coordinate().y();
                        fs.fsline[icnt].fsLinepoints[jcnt].pointLabel = static_cast<SpaceLabel>(proto_point.pointlabel());

                        // Log the fsLinepoint coordinates
                        std::cout << "[FSLine] fsline[" << icnt << "] x: " << fs.fsline[icnt].fsLinepoints[jcnt].coordinate.x 
                                  << " y: " << fs.fsline[icnt].fsLinepoints[jcnt].coordinate.y << std::endl;
                    }
                }
            }
        }
        
        std::cout << "[FSLine]j5 callback success!" << std::endl;
    }

    Obstacles obstacles;
    static int64_t seq_ = 0;

    static void SimpleObstacleSubCallback(const std::shared_ptr<RawObstacleMsg> &raw_obstacle_msg)
    {
        std::cout << "[OD]j5 callback arrived!" << std::endl;
        std::cout << "[OD]rawobjects_size:" << raw_obstacle_msg->proto.rawobjects_size() << std::endl;

        const auto& proto_header = raw_obstacle_msg->proto.header();
        obstacles.header.timestamp = proto_header.timestampns().nanosec();
        obstacles.header.frame_id = "1";
        obstacles.header.seq = seq_++;
        
        std::cout << "[OD] header.timestamp: " << obstacles.header.timestamp << ",header.seq:" << obstacles.header.seq << std::endl;

        int object_count = raw_obstacle_msg->proto.rawobjects_size();
        if (object_count > 0)
        {
            obstacles.obstaclesraw.resize(object_count);  // 一次性分配内存
            
            for (int icnt = 0; icnt < object_count; icnt++)
            {
                const auto& raw_object = raw_obstacle_msg->proto.rawobjects(icnt);
                
                auto& current_obstacle = obstacles.obstaclesraw[icnt];
                current_obstacle.camera = raw_object.camera();
                current_obstacle.label = raw_object.label();
                current_obstacle.typeConfidence = raw_object.typeconfidence();
                current_obstacle.existenceConfidence = raw_object.existenceconfidence();
                
                current_obstacle.box.topLeftX = raw_object.box2d().topleftx();
                current_obstacle.box.topLeftY = raw_object.box2d().toplefty();
                current_obstacle.box.bottomRightX = raw_object.box2d().bottomrightx();
                current_obstacle.box.bottomRightY = raw_object.box2d().bottomrighty();
                current_obstacle.box.confidence = raw_object.box2d().confidence();

                current_obstacle.position << raw_object.positioninfo().position().x(),
                                            raw_object.positioninfo().position().y(),
                                            raw_object.positioninfo().position().z();

                if (raw_object.landmark4_size() != 0)
                {
                    current_obstacle.landmark4.resize(raw_object.landmark4_size());
                    current_obstacle.landmark4Scores.resize(raw_object.landmark4scores_size());

                    for (int jcnt = 0; jcnt < raw_object.landmark4_size(); jcnt++)
                    {
                        current_obstacle.landmark4[jcnt].x = raw_object.landmark4(jcnt).x();
                        current_obstacle.landmark4[jcnt].y = raw_object.landmark4(jcnt).y();
                    }

                    for (int jcnt = 0; jcnt < raw_object.landmark4scores_size(); jcnt++)
                    {
                        current_obstacle.landmark4Scores[jcnt] = raw_object.landmark4scores(jcnt);
                    }
                }

                if (raw_object.box3d_size() != 0)
                {
                    current_obstacle.box3d.resize(raw_object.box3d_size());
                    for (int jcnt = 0; jcnt < raw_object.box3d_size(); jcnt++)
                    {
                        current_obstacle.box3d[jcnt] = raw_object.box3d(jcnt);
                    }
                }
            }
        }

        std::cout << "[OD]j5 callback success!" << std::endl;
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

    void PerceptionOdMoudle::InitPortsAndProcs()
    {
      DF_MODULE_INIT_IDL_INPUT_PORT("sub_apa_status", aph::apa_status);
      DF_MODULE_INIT_IDL_INPUT_PORT("sub_camera_frame_array",
                                    camera_frame::CameraFrameArray);
      // DF_MODULE_INIT_IDL_INPUT_PORT("apa_image_in",
      //                               camera_frame::CameraFrameArray);
      DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_obstacles", od::Obstacles);
      DF_MODULE_INIT_IDL_OUTPUT_PORT("pub_fsline_msg", od::FSLine);
      DF_MODULE_INIT_IDL_OUTPUT_PORT("percept_debug_od", ImageProto::Image);
      DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
          "MsgCenterProc", PerceptionOdMoudle, MsgCenterProc,
          hobot::dataflow::ProcType::DF_MSG_COND_PROC,
          DF_VECTOR("sub_apa_status", "sub_camera_frame_array"), DF_VECTOR("pub_obstacles","pub_fsline_msg"));
      DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
          "TimerProc", PerceptionOdMoudle, TimerProc,
          hobot::dataflow::ProcType::DF_MSG_TIMER_PROC, DF_VECTOR(),
          DF_VECTOR( "percept_debug_od"));
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

    int32_t PerceptionOdMoudle::Init()
    {

      /************************SUB FROM PERCEPTION***************************/
      hobot::communication::Init("communication.json");
      std::string version = hobot::communication::GetVersion();
      std::cout << "communication version is " << version << "\n";

      std::cout << "mock example begin ..." << std::endl;

      auto rt = senseAD::rscl::GetCurRuntime();
      DFHLOG_I("Init Runtime");
      int argc = 1;
      const char *test[] = {"OD_node", nullptr};
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
      std::string obstacle_topic = "/perception/camera/fisheye_objects";
      std::string fsline_topic = "/perception/camera/fsline_points";
      ErrorCode error_code;

      // OBSTACLE
      publisher_ = Publisher<RawObstacleSerial>::New(comm_attr,
                                                     obstacle_topic,
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

      senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->RegPipe(obstacle_topic, [this](const void *ptr, const size_t)
                                                                    {
                                                                  std::cout<< "[OD]In Reg Obstacle Pipe"<<std::endl;
                                                                  auto out_ptr = reinterpret_cast<const RawObstacleMsg *>(ptr);
                                                                  auto msg = std::make_shared<RawObstacleMsg>();
                                                                  *msg = *out_ptr;
                                                                  std::cout<< "[OD]In Reg Obstacle Pipe"<<std::endl;
                                                                  this->publisher_->Pub(msg); });

      CommAttr sub_comm_attr;
      sub_comm_attr.qos_.qos_profile_.reliability_qos_policy.kind =
          ReliabilityQosPolicyKind::RELIABILITY_BEST_EFFORT;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.kind =
          HistoryQoSPolicyKind::HISTORY_KEEP_LAST;
      sub_comm_attr.qos_.qos_profile_.history_qos_policy.depth = 5;
      subscriber_ = Subscriber<RawObstacleSerial>::New(
          sub_comm_attr, obstacle_topic, 0, SimpleObstacleSubCallback, PROTOCOL_SHM);

      fsline_publisher_ = Publisher<FSLineSerial>::New(comm_attr,
                                                       fsline_topic,
                                                       0,
                                                       PROTOCOL_SHM,
                                                       &error_code,
                                                       pub_connlisteners);
      if (!fsline_publisher_)
      {
        std::cout << " create fsline_publisher failed";
        return -1;
      }

      senseAD::ad_msg_bridge::GetGlobalCommunicationPipe()->RegPipe(fsline_topic, [this](const void *ptr, const size_t)
                                                                    {
                                                                  std::cout<< "[OD]In Reg FSLINE Pipe "<<std::endl;
                                                                  auto out_ptr = reinterpret_cast<const FSLineMsg *>(ptr);
                                                                  auto msg = std::make_shared<FSLineMsg>();
                                                                  *msg = *out_ptr;
                                                                  std::cout<< "[OD]In Reg FSLINE Pipe"<<std::endl;
                                                                  this->fsline_publisher_->Pub(msg); });

      fsline_subscriber_ = Subscriber<FSLineSerial>::New(
          sub_comm_attr, fsline_topic, 0, SimpleFSLineSubCallback, PROTOCOL_SHM);

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
      std::string rear_camera_topic = "rear_camera_fov195";
      std::string left_camera_topic = "left_camera_fov195";
      std::string right_camera_topic = "right_camera_fov195";
      ErrorCode image_error_code;

      std::cout << "[OD]Before crete Image sub " << std::endl;

      // FRONT CAMERA
      front_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, front_camera_topic, 0, FrontCameraImageSubCallback, PROTOCOL_SHM);

      if (!front_camera_subscriber_)
      {
        std::cout << "[OD] create FRONT CAMERA subscriber failed" << std::endl;
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
        std::cout << "[OD]create FRONT CAMERA publisher failed" << std::endl;
        return -1;
      }
      // REAR CAMERA
      rear_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, rear_camera_topic, 0, RearCameraImageSubCallback, PROTOCOL_SHM);

      if (!rear_camera_subscriber_)
      {
        std::cout << "[OD] create REAR CAMERA subscriber failed" << std::endl;
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
        std::cout << "[OD]create REAR CAMERA publisher failed" << std::endl;
        return -1;
      }

      // LEFT CAMERA
      left_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, left_camera_topic, 0, LeftCameraImageSubCallback, PROTOCOL_SHM);

      if (!left_camera_subscriber_)
      {
        std::cout << "[OD] create LEFT CAMERA subscriber failed" << std::endl;
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
        std::cout << "[OD]create LEFT CAMERA publisher failed" << std::endl;
        return -1;
      }

      // RIGHT CAMERA
      right_camera_subscriber_ = Subscriber<ImageSerial>::New(
          image_comm_attr, right_camera_topic, 0, RightCameraImageSubCallback, PROTOCOL_SHM);

      if (!right_camera_subscriber_)
      {
        std::cout << "[OD] create LEFT CAMERA subscriber failed" << std::endl;
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
        std::cout << "[OD]create LEFT CAMERA publisher failed" << std::endl;
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

    int32_t PerceptionOdMoudle::Start()
    {

      senseAD::rscl::component::ComponentConfig cfg;
      cfg.config_file_path = "./bridge.yaml";
      cfg.timer_conf.set_name("OD");
      cfg.timer_conf.set_interval_ms(1000);
      perception_OD_comp_->Initialize(cfg);

      return hobot::dataflow::Module::Start();
    }

    int32_t PerceptionOdMoudle::Stop()
    {
      // perception_OD_comp_->Shutdown();
      perception_OD_comp_->OnShutdown();
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

// 608*500 to 600*500
static int __nv12CropSplit600( uint8_t* pu8DestY, uint8_t* pu8DestU, uint8_t* pu8DestV, const uint8_t* pu8Src)
{
	if((!pu8DestY) || (!pu8DestU)|| (!pu8DestV)|| (!pu8Src))
		return -1;
	for (int i = 0; i < 500; i++)
	{
		memcpy(pu8DestY,pu8Src,600);
		pu8Src += 608;
		pu8DestY += 600;
	}
	for (int i = 0; i < 250; i++)
	{
		for (int j = 0; j < 300; j++)
		{
			*pu8DestU++ = *pu8Src++;
			*pu8DestV++ = *pu8Src++;
		}
		pu8Src += 8;
	}
	return 0;
}

static int __nv12Merge( uint8_t* pu8Dest, const uint8_t* pu8SrdY, const uint8_t* pu8SrcU, const uint8_t* pu8SrcV, const uint32_t u32Size)
{
	if((!pu8SrdY) || (!pu8SrcU)|| (!pu8SrcV)|| (!pu8Dest))
		return -1;
	const uint32_t u32UVSize = u32Size/4;
	memcpy(pu8Dest, pu8SrdY, u32Size);
	pu8Dest += u32Size;
    #ifdef __ARM_NEON
	if(0 == (u32UVSize&15))
	{
		for (int i = 0; i < u32UVSize; i+=16)
		{
			uint8x16x2_t vu8_16_2_Dest;
			vu8_16_2_Dest.val[0]=vld1q_u8(pu8SrcU);
			vu8_16_2_Dest.val[1]=vld1q_u8(pu8SrcV);
			vst2q_u8(pu8Dest, vu8_16_2_Dest);
			pu8Dest+=32;
			pu8SrcU+=16;
			pu8SrcV+=16;
		}
	}else{
		for (int i = 0; i < u32UVSize; i++)
		{
			*pu8Dest++ = *pu8SrcU++;
			*pu8Dest++ = *pu8SrcV++;
		}
	}
    #else
	for (int i = 0; i < u32UVSize; i++)
	{
		*pu8Dest++ = *pu8SrcU++;
		*pu8Dest++ = *pu8SrcV++;
	}
    #endif
	return 0;
}

int nv12CropResize(uint8_t* pu8Dest, const uint8_t* pu8Src)
 {
    cv::Mat image_y(500, 600, CV_8UC1);
    cv::Mat image_u(250, 300, CV_8UC1);
    cv::Mat image_v(250, 300, CV_8UC1);
	  __nv12CropSplit600((uint8_t*)image_y.data, (uint8_t*)image_u.data, (uint8_t*)image_v.data, pu8Src);
	  cv::resize(image_y, image_y, cv::Size(640, 360), cv::INTER_LINEAR);
	  cv::resize(image_u, image_u, cv::Size(320, 180), cv::INTER_LINEAR);
	  cv::resize(image_v, image_v, cv::Size(320, 180), cv::INTER_LINEAR);
	  __nv12Merge(pu8Dest, (uint8_t*)image_y.data, (uint8_t*)image_u.data, (uint8_t*)image_v.data, 640*360);
    return 0;
 }

    void PerceptionOdMoudle::MsgCenterProc(
        hobot::dataflow::spMsgResourceProc proc,
        const hobot::dataflow::MessageLists &msgs)
    {
      auto &sub_apa_status_msgs = msgs[proc->GetResultIndex("sub_apa_status")];
      for (auto &msg : *(sub_apa_status_msgs.get()))
      {
        if (nullptr == msg)
        {
          continue;
        }
        DFHLOG_I("[OD]sub_apa_status msg timestamp: {}", msg->GetGenTimestamp());
        // process msg of sub_apa_status
        auto apa_status =
            std::dynamic_pointer_cast<ApaStatusMsg>(sub_apa_status_msgs->at(0));
        if (apa_status && apa_status->proto.has_m_current_apa_status())
        {
          DFHLOG_I("[OD]sub_apa_status msg timestamp: {}, apa_status = {}",
                   msg->GetGenTimestamp(),
                   apa_status->proto.m_current_apa_status());
        }
      }

      // GET FRONT/REAR/LEFT/RIGHT CAMERA IMAGE
      std::cout << "[OD]ImageReceiveProc start" << std::endl;
      auto msg_list = msgs[proc->GetResultIndex("sub_camera_frame_array")];
      std::cout << "[OD]msg_list size: " << msg_list->size() << std::endl;

      if (!msg_list->empty())
      {
        std::shared_ptr<CameraFrameArrayProtoMsg> camera_group_msg =
            std::dynamic_pointer_cast<CameraFrameArrayProtoMsg>(msg_list->front());
        auto len = camera_group_msg->proto.camera_frame_size();
        auto gen_image_ts = camera_group_msg->proto.gdc_vio_ts();
        DFHLOG_W("[OD] income frame rate: {}", gen_image_ts);
        DFHLOG_W("[OD] camera_frame_size: {} ", len);
        
        // Load camera configuration once
        YAML::Node cam_config;
        try {
            cam_config = YAML::LoadFile("/app/parking/fisheye_dataflow/config/camera/camera.yaml");
        } catch (const YAML::BadFile& e) {
            std::cerr << "Failed to load camera configuration file: " << e.what() << std::endl;
            return;
        }

        // Build the camera_map once
        std::map<std::string, std::string> camera_map;
        for (const auto& camera : cam_config["camera_id_mapping"]) {
            std::string camera_id = camera["adapter_id"].as<std::string>();
            std::string camera_name = camera["camera_name"].as<std::string>();
            camera_map[camera_id] = camera_name;
        }

        for (auto i = 0; i < len; i++)
        {
          auto cam_id_str = camera_group_msg->proto.camera_frame(i).cam_id();
          std::string cam_name;

          std::cout << "[OD] cam_id_str: " << cam_id_str << std::endl;
          auto camera_proto_msg = camera_arry_msg_convert(camera_group_msg, cam_id_str);

          // Lookup cam_name
          auto it = camera_map.find(cam_id_str);
          if (it != camera_map.end()) {
              cam_name = it->second;
          } else {
              std::cerr << "Camera ID " << cam_id_str << " not found in camera map." << std::endl;
              continue; // Skip processing this camera frame
          }

          std::cout<<"OD cam_name:"<<cam_name<<std::endl;

          if (camera_proto_msg != nullptr)
          {
            camera_msg_convert(camera_proto_msg);
            
            if (camera_proto_msg->proto.pym_img_info().down_scale_num() >= 1 &&
                camera_proto_msg->proto.pym_img_info().down_scale_size() >= 1)
            {
              auto frame_id = camera_proto_msg->proto.pym_img_info().frame_seq();
              const camera_frame::YuvAddrInfo &yuv_proto = camera_proto_msg->proto.pym_img_info().down_scale(0);
              auto pym_buf = reinterpret_cast<char *>(yuv_proto.y_vaddr());
              auto width = yuv_proto.width();
              auto height = yuv_proto.height();
              cv::Mat image_nv12(height * 1.5, width, CV_8UC1, pym_buf, yuv_proto.step());
              cv::Mat rgb_mat;
              DFHLOG_W("Original width={},height={}", width, height);
              int32_t new_width = 640;
              int32_t new_height = 360;

              
              nv12CropResize(pu8Dest, (uint8_t*)pym_buf);

              cv::cvtColor(image_nv12, rgb_mat, cv::COLOR_YUV2BGR_NV12); 
              cv::Mat resizedMat;
              cv::resize(rgb_mat, resizedMat, cv::Size(new_width, new_height));
            
              uint64_t addr_value = reinterpret_cast<uint64_t>(pu8Dest);


              auto image = std::make_shared<ImageMsg>();
              rd::Time image_time;
              image_time.set_nanosec(gen_image_ts);
              rd::Header od_header;
              od_header.mutable_timestampns()->CopyFrom(image_time);
              image->proto.mutable_header()->CopyFrom(od_header);

              NV12ResizedMat_front = resizedMat.clone();
              image->proto.set_height(new_height);
              image->proto.set_width(new_width);
              image->proto.set_phyaddr(addr_value);
              image->proto.set_viraddr(addr_value);
              image->proto.set_step(1.5);

              std::cout << "[OD]ImageReceiveProc cameraID " << cam_id_str << std::endl;
              std::cout << "[OD] timestamp " << gen_image_ts << std::endl;
              std::cout << "[OD] HEIGHT " << new_height << std::endl;
              std::cout << "[OD] WIDTH " << new_width << std::endl;
              std::cout << "[OD] PADDR " << yuv_proto.y_paddr() << std::endl;
              // std::cout << "[OD] VADDR " << addr_value <<std::endl;
              std::cout << "[OD] STEP " << camera_proto_msg->proto.pym_img_info().down_scale(0).step() << std::endl;
              
              if (cam_name == "FRONT_FISHEYE")
              {
                NV12ResizedMat_front = resizedMat.clone();
                front_camera_publisher_->Pub(image);
              }
              else if (cam_name == "LEFT_FISHEYE")
              {
                NV12ResizedMat_left = resizedMat.clone();
                left_camera_publisher_->Pub(image);
              }
              else if (cam_name == "REAR_FISHEYE")
              {
                NV12ResizedMat_rear = resizedMat.clone();
                rear_camera_publisher_->Pub(image);
              }
              else if (cam_name == "RIGHT_FISHEYE")
              {
                NV12ResizedMat_right = resizedMat.clone();
                right_camera_publisher_->Pub(image);
              }
            }
          }
        }
      }

              // pub ObstaclesMsg
              {
                // fill proto
                od::Time od_time;
                od_time.set_nanosec(obstacles.header.timestamp);
                od::Header od_header;
                od_header.set_seq(obstacles.header.seq);
                od_header.set_frameid(obstacles.header.frame_id);
                od_header.mutable_timestampns()->CopyFrom(od_time);
                DFHLOG_W("pub_obstacles_s32g info, obstacles header, timestamp = {}, seq = {}",
                             obstacles.header.timestamp,obstacles.header.seq);
                // create msg
                auto obstacles_msg = std::make_shared<ObstaclesMsg>();
                obstacles_msg->proto.mutable_header()->CopyFrom(od_header);
                if (obstacles.obstaclesraw.size() != 0)
                {
                  for (int icnt = 0; icnt < obstacles.obstaclesraw.size(); icnt++)
                  {
                    od::ObstacleRaw od_obstacleRaw;
                    // od_obstacleRaw.set_camera("camera_name_test");
                    od_obstacleRaw.set_label(obstacles.obstaclesraw[icnt].label);
                    od_obstacleRaw.set_typeconfidence(obstacles.obstaclesraw[icnt].typeConfidence);
                    od_obstacleRaw.set_existenceconfidence(obstacles.obstaclesraw[icnt].existenceConfidence);
                    od_obstacleRaw.set_camera(obstacles.obstaclesraw[icnt].camera);
                    DFHLOG_W("[OD] Before pub to S32g camera = {} " , obstacles.obstaclesraw[icnt].camera);

                    od::Bbox2D box;
                    box.set_topleftx(obstacles.obstaclesraw[icnt].box.topLeftX);
                    box.set_toplefty(obstacles.obstaclesraw[icnt].box.topLeftY);
                    box.set_bottomrightx(obstacles.obstaclesraw[icnt].box.bottomRightX);
                    box.set_bottomrighty(obstacles.obstaclesraw[icnt].box.bottomRightY);
                    box.set_confidence(obstacles.obstaclesraw[icnt].box.confidence);
                    od_obstacleRaw.mutable_box2d()->CopyFrom(box);
                    DFHLOG_W("[OD] Before pub to S32g box2d tx = {}, ty = {},  br = {}, by = {}", 
                    obstacles.obstaclesraw[icnt].box.topLeftX, obstacles.obstaclesraw[icnt].box.topLeftY, 
                    obstacles.obstaclesraw[icnt].box.bottomRightX, obstacles.obstaclesraw[icnt].box.bottomRightY);

                    for (int jcnt = 0; jcnt < obstacles.obstaclesraw[icnt].landmark4.size(); jcnt++)
                    {
                      od::Point2f od_point2f;
                      od_point2f.set_x(obstacles.obstaclesraw[icnt].landmark4[jcnt].x);
                      od_point2f.set_y(obstacles.obstaclesraw[icnt].landmark4[jcnt].y);
                      DFHLOG_W( "[OD] Before pub to S32g landmark4 x= {}, y = {} ", 
                           obstacles.obstaclesraw[icnt].landmark4[jcnt].x, obstacles.obstaclesraw[icnt].landmark4[jcnt].y );
                      od_obstacleRaw.add_landmark4()->CopyFrom(od_point2f);
                    }

                    for (int jcnt = 0; jcnt < obstacles.obstaclesraw[icnt].landmark4Scores.size(); jcnt++)
                    {
                      od_obstacleRaw.add_landmark4scores(obstacles.obstaclesraw[icnt].landmark4Scores[jcnt]);
                    }

                    for (int jcnt = 0; jcnt < obstacles.obstaclesraw[icnt].box3d.size(); jcnt++)
                    {
                      od_obstacleRaw.add_box3d(obstacles.obstaclesraw[icnt].box3d[jcnt]);
                    }
                    
                    obstacles_msg->proto.add_rawobjects()->CopyFrom(od_obstacleRaw);

                   
                  }
                  auto pub_obstacles_s32g = proc->GetOutputPort("pub_obstacles");
                  if (!pub_obstacles_s32g)
                  {
                    DFHLOG_E("pub_obstacles_s32g failed to get output port of {}",
                               "pub_obstacles");
                    return;
                  }
                  // send to s32g
                  pub_obstacles_s32g->Send(obstacles_msg);
                }
              }

              // pub FSLineMsg
              {
                // fill proto
                auto fsline_msg = std::make_shared<FSLineMsg>();
                od::Time od_time;
                od::Header od_header;
                od::FSLinesimple od_fsline_simple_front;
                od::FSLinesimple od_fsline_simple_left;
                od::FSLinesimple od_fsline_simple_right;
                od::FSLinesimple od_fsline_simple_rear;
                DFHLOG_W("Pub od_fsline_msg size = {}", fs.fsline.size());
                for (int icnt = 0; icnt < fs.fsline.size(); icnt++)
                {
                  od_time.set_nanosec(fs.fsline[icnt].header.timestamp);
                  od_header.set_seq(fs.fsline[icnt].header.seq);
                  od_header.set_frameid(fs.fsline[icnt].header.frame_id);
                  od_header.mutable_timestampns()->CopyFrom(od_time);
                  DFHLOG_W("Pub od_fsline_msg,obstacles header, timestamp = {}, seq = {}",
                             fs.fsline[icnt].header.timestamp,fs.fsline[icnt].header.seq);
                  if (icnt == 0){
                    for (int jcnt = 0; jcnt < fs.fsline[icnt].fsLinepoints.size(); jcnt++)
                    {
                      od::Point2f fs_point2f;
                      fs_point2f.set_x(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.x);
                      fs_point2f.set_y(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.y);

                      od::FSLinePoint fsline_point;
                      fsline_point.mutable_coordinate()->CopyFrom(fs_point2f);
                      fsline_point.set_pointlabel(od::SpaceLabel::vehicle);
                      od_fsline_simple_front.add_fslinepoints()->CopyFrom(fsline_point);
                    }
                    od_fsline_simple_front.mutable_header()->CopyFrom(od_header);
                    fsline_msg->proto.add_fsline()->CopyFrom(od_fsline_simple_front);
                  }else if(icnt == 1){
                    for (int jcnt = 0; jcnt < fs.fsline[icnt].fsLinepoints.size(); jcnt++)
                    {
                      od::Point2f fs_point2f;
                      fs_point2f.set_x(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.x);
                      fs_point2f.set_y(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.y);

                      od::FSLinePoint fsline_point;
                      fsline_point.mutable_coordinate()->CopyFrom(fs_point2f);
                      fsline_point.set_pointlabel(od::SpaceLabel::vehicle);
                      od_fsline_simple_left.add_fslinepoints()->CopyFrom(fsline_point);
                    }
                    od_fsline_simple_left.mutable_header()->CopyFrom(od_header);
                    fsline_msg->proto.add_fsline()->CopyFrom(od_fsline_simple_left);
                  }else if (icnt == 2){
                    for (int jcnt = 0; jcnt < fs.fsline[icnt].fsLinepoints.size(); jcnt++)
                    {
                      od::Point2f fs_point2f;
                      fs_point2f.set_x(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.x);
                      fs_point2f.set_y(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.y);

                      od::FSLinePoint fsline_point;
                      fsline_point.mutable_coordinate()->CopyFrom(fs_point2f);
                      fsline_point.set_pointlabel(od::SpaceLabel::vehicle);
                      od_fsline_simple_right.add_fslinepoints()->CopyFrom(fsline_point);
                    }
                    od_fsline_simple_right.mutable_header()->CopyFrom(od_header);
                    fsline_msg->proto.add_fsline()->CopyFrom(od_fsline_simple_right);
                  }else if (icnt == 3){
                    for (int jcnt = 0; jcnt < fs.fsline[icnt].fsLinepoints.size(); jcnt++)
                    {
                      od::Point2f fs_point2f;
                      fs_point2f.set_x(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.x);
                      fs_point2f.set_y(fs.fsline[icnt].fsLinepoints[jcnt].coordinate.y);

                      od::FSLinePoint fsline_point;
                      fsline_point.mutable_coordinate()->CopyFrom(fs_point2f);
                      fsline_point.set_pointlabel(od::SpaceLabel::vehicle);
                      od_fsline_simple_rear.add_fslinepoints()->CopyFrom(fsline_point);
                    }
                    od_fsline_simple_rear.mutable_header()->CopyFrom(od_header);
                    fsline_msg->proto.add_fsline()->CopyFrom(od_fsline_simple_rear);
                  }
                }
                DFHLOG_W("after set fsline msg size ={}",fsline_msg->proto.fsline_size());
                auto pub_fsline_msg_port_s32g = proc->GetOutputPort("pub_fsline_msg");
                if (!pub_fsline_msg_port_s32g)
                {
                  DFHLOG_E("failed to get output port of {}", "pub_fsline_msg");
                  return;
                }
                pub_fsline_msg_port_s32g->Send(fsline_msg);
              }
      
    }

  void saveFSLineToFile(const FSLine& fslineData, std::time_t t) {
    
    char filename[100];
    std::strftime(filename, sizeof(filename), "%Y%m%d%H%M%S.bin", std::localtime(&t));

    // Open binary file for writing
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Write the number of FSLinesimple elements
    uint64_t fslineCount = fslineData.fsline.size();
    outFile.write(reinterpret_cast<const char*>(&fslineCount), sizeof(fslineCount));

    // Iterate over each FSLinesimple and serialize it
    for (const auto& fslineSimple : fslineData.fsline) {
        // Serialize Header
        const Header& header = fslineSimple.header;

        // Write seq
        outFile.write(reinterpret_cast<const char*>(&header.seq), sizeof(header.seq));

        // Write stamp
        outFile.write(reinterpret_cast<const char*>(&header.timestamp), sizeof(header.timestamp));

        // Write frame_id length and data
        uint64_t frameIdLength = header.frame_id.size();
        outFile.write(reinterpret_cast<const char*>(&frameIdLength), sizeof(frameIdLength));
        outFile.write(header.frame_id.data(), frameIdLength);

        // Write frameTimestampNs
        outFile.write(reinterpret_cast<const char*>(&fslineSimple.frameTimestampNs), sizeof(fslineSimple.frameTimestampNs));

        // Write the number of FSLinePoint elements
        uint64_t fsLinePointCount = fslineSimple.fsLinepoints.size();
        outFile.write(reinterpret_cast<const char*>(&fsLinePointCount), sizeof(fsLinePointCount));

        // Iterate over each FSLinePoint and serialize it
        for (const auto& fsLinePoint : fslineSimple.fsLinepoints) {
            // Serialize coordinate (cv::Point2f)
            outFile.write(reinterpret_cast<const char*>(&fsLinePoint.coordinate.x), sizeof(fsLinePoint.coordinate.x));
            outFile.write(reinterpret_cast<const char*>(&fsLinePoint.coordinate.y), sizeof(fsLinePoint.coordinate.y));

            // Serialize pointLabel
            uint32_t pointLabel = static_cast<uint32_t>(fsLinePoint.pointLabel);
            outFile.write(reinterpret_cast<const char*>(&pointLabel), sizeof(pointLabel));
        }
    }

    outFile.close();
    std::cout << "Data saved to file: " << filename << std::endl;
}

void saveNV12imageTOFile(cv::Mat front, cv::Mat left, cv::Mat rear, cv::Mat right, std::time_t t)
{
   // List of images and their corresponding names
    std::vector<std::pair<cv::Mat, std::string>> images = {
        {front, "front"},
        {left, "left"},
        {rear, "rear"},
        {right, "right"}
    };

     // Convert time_t to tm structure in a thread-safe manner
    std::tm tm_buf;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_buf, &t);
#else
    localtime_r(&t, &tm_buf);
#endif

  char timestamp[20];
  std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", &tm_buf);

   for (const auto& img_pair : images) {
        const cv::Mat& img = img_pair.first;
        const std::string& name = img_pair.second;

        // Construct the filename: timestamp + name + ".nv12"
        std::string filename = std::string(timestamp) + "_" + name + ".nv12";

        // Open file for binary writing
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            continue;
        }

        // Write the NV12 image data directly
        file.write(reinterpret_cast<const char*>(img.data), img.total() * img.elemSize());

        file.close();

        std::cout << "Saved image to file: " << filename << std::endl;
    }
}

    void PerceptionOdMoudle::TimerProc(hobot::dataflow::spMsgResourceProc proc,
                                       const hobot::dataflow::MessageLists &msgs)
    {
      std::vector<uchar> buffer;
      if (!NV12ResizedMat_front.empty() && !NV12ResizedMat_left.empty() && !NV12ResizedMat_right.empty() && !NV12ResizedMat_rear.empty())
      {
        // cv::Mat padding_mat_front, padding_mat_rear, padding_mat_left, padding_mat_right;
        // cv::copyMakeBorder(NV12ResizedMat_front, padding_mat_front, 0, 24, 0,0, cv::BorderTypes::BORDER_CONSTANT, cv::Scalar(0,0,0));
        // cv::copyMakeBorder(NV12ResizedMat_rear, padding_mat_rear, 0, 24, 0,0, cv::BorderTypes::BORDER_CONSTANT, cv::Scalar(0,0,0));
        // cv::copyMakeBorder(NV12ResizedMat_left, padding_mat_left, 0, 24, 0,0, cv::BorderTypes::BORDER_CONSTANT, cv::Scalar(0,0,0));
        // cv::copyMakeBorder(NV12ResizedMat_right, padding_mat_right, 0, 24, 0,0, cv::BorderTypes::BORDER_CONSTANT, cv::Scalar(0,0,0));
        // Get current timestamp for filename
        std::time_t now = std::time(nullptr);
        // saveFSLineToFile(fs,now);
        // saveNV12imageTOFile(NV12ResizedMat_front,NV12ResizedMat_left,NV12ResizedMat_right,NV12ResizedMat_rear,now);
        save_pred_img_OD(fs, obstacles, buffer, NV12ResizedMat_front, NV12ResizedMat_left, NV12ResizedMat_rear, NV12ResizedMat_right);
        // save_pred_img_FSLine(fs, buffer, NV12ResizedMat_front, NV12ResizedMat_left, NV12ResizedMat_rear, NV12ResizedMat_right);
        {
          uint64_t send_start = GetTimeStamp();
          auto out = std::make_shared<WrapImageProtoMsg>();
          // std::cout<<"Detect_Cornerpoint_gpsd"<<__LINE__<<std::endl;
          out->proto.set_width(1280);
          out->proto.set_height(768);
          // out->proto.set_channel(msg->proto.channel());
          out->proto.set_send_mode(0);
          out->proto.set_format(2);
          out->SetData(std::make_shared<hobot::message::DataRef>(buffer.data(), buffer.size()));

          DFHLOG_W("percept_debug_od size: {}, ts = {}.", buffer.size(), GetTimeStamp());
          auto pub_image_port = proc->GetOutputPort("percept_debug_od");
          pub_image_port->Send(out);
        }
      }
    }

    inline std::array<uint8_t, 3> get_space_label_color_bgr(SpaceLabel label) {
      static std::map<SpaceLabel, std::array<uint8_t, 3>> color_map{
        {SpaceLabel::vehicle, {255, 0, 0}},   // blue
        {SpaceLabel::pedestrian, {0, 255, 0}},   // green
        {SpaceLabel::RoadEdge, {0, 0, 255}},   // red   
        {SpaceLabel::Wall, {0, 255, 255}}, // yellow     
        {SpaceLabel::TrafficCone, {255, 0, 255}}, // magenta
        {SpaceLabel::Other, {255, 0, 127}}, // purple
      };
      std::array<uint8_t, 3> color;
      auto iter = color_map.find(label);
      if (iter != color_map.end()) {
          color = iter->second;
      } else {
          color = {0, 0, 0};
      }
      return color;
    };

    inline std::string get_obs_label(int label){
      switch (label){
        case -1:
          return "OBJECT_ALL";
          break;
        case 0:
          return "UNKNOWN";
          break;
        case 1:
          return "VEHICLE";
          break;
        case 2:
          return "PEDESTRIAN";
          break;
        case 3:
          return "TRAFFICBARREL";
          break;
        case 4:
          return "CART";
          break;
        case 5:
          return "COLUMN";
          break;
        case 6:
          return "WHEELSTOP";
          break;
        case 7:
          return "NOPARKINGSIGN";
          break;
        case 8:"LIMITLOCK";
          break;
        case 9:
          return "BIKE";
          break;
      }
    }

     void DrawBev3DBox(const ObstacleRaw &obj, cv::Mat &img, std::string camera_name,const std::shared_ptr<CameraModelBase> camera_model)
    {
      static std::map<std::string, std::function<bool(ObstacleRaw)>>
        obj_map_filter{
            {"front_camera_fov195",
             [](ObstacleRaw obj) -> bool {
                 if (obj.position(0) > 0) return true;
                 return false;
             }},
            {"rear_camera_fov195",
             [](ObstacleRaw obj) -> bool {
                 if (obj.position(0) < 0) return true;
                 return false;
             }},
            {"left_camera_fov195",
             [](ObstacleRaw obj) -> bool {
                 if (obj.position(1) > 0) return true;
                 return false;
             }},
            {"right_camera_fov195",
             [](ObstacleRaw obj) -> bool {
                 if (obj.position(1) < 0) return true;
                 return false;
             }},
        };
        auto world = camera_model->getTransformation();
        Eigen::Matrix4d world2cam = world.inverse();//
        if (obj_map_filter.count(camera_name) == 0) {
          std::cout <<"[DrawBev3DBbox] "
            << "obj_map_filter.count(camera_name), camera_name = "
            << camera_name << std::endl;
            return;
        }
        if (camera_model == nullptr) {
            AD_LWARN(DrawBev3DBbox) << "camera_model == nullptr";
            return;
        }
         Eigen::Vector3f p, s;
         p << obj.box3d[0], obj.box3d[1], obj.box3d[2];
         p << obj.box3d[3], obj.box3d[5], obj.box3d[4];
         float32_t obj_yaw_to_car = obj.box3d[6];
         if (p(0) == 0 && p(1) == 0 && p(2) == 0) return;
         if (!obj_map_filter[camera_name](obj)) {
            std::cout << "obj_map_filter.count(camera_name), camera_name = "
                << camera_name << std::endl;
            return;
        }
        std::vector<Eigen::Vector3f> box_corners_selfcar;
        Eigen::Vector4d point_camera_4d = {p(0), p(1), p(2), 1.0};
        Eigen::Vector3f center_selfcar(p(0), p(1), p(2));
        Eigen::Vector3f dir_len(cos(obj_yaw_to_car), sin(obj_yaw_to_car),
                                0);  // len direction
        Eigen::Vector3f dir_width(-dir_len.y(), dir_len.x(), 0);
        const Eigen::Vector3f object_shape = {s(1), s(0), s(2)};
        box_corners_selfcar.push_back(center_selfcar +
                                      dir_len * object_shape(0) / 2 +
                                      dir_width * object_shape(1) / 2);
        box_corners_selfcar.push_back(center_selfcar +
                                      dir_len * object_shape(0) / 2 -
                                      dir_width * object_shape(1) / 2);
        box_corners_selfcar.push_back(center_selfcar -
                                      dir_len * object_shape(0) / 2 -
                                      dir_width * object_shape(1) / 2);
        box_corners_selfcar.push_back(center_selfcar -
                                      dir_len * object_shape(0) / 2 +
                                      dir_width * object_shape(1) / 2);
        for (size_t i = 0; i < 4; ++i) {
            box_corners_selfcar.push_back(box_corners_selfcar[i]);
            box_corners_selfcar[i](2) = 0;
            box_corners_selfcar.back()(2) = object_shape(2);
        }
        box_corners_selfcar.push_back(center_selfcar);
        box_corners_selfcar.back()(2) = object_shape(2) / 2.0;
        std::vector<Eigen::Vector2d> pixel_corners(box_corners_selfcar.size());
        for (size_t i = 0; i < box_corners_selfcar.size(); ++i) {
            Eigen::Vector4d point3d_pad = {box_corners_selfcar[i](0),
                                           box_corners_selfcar[i](1),
                                           box_corners_selfcar[i](2), 1.0};
            auto point_unit = world2cam * point3d_pad;//
            Eigen::Vector3d point3d{point_unit(0), point_unit(1),
                                    point_unit(2)};
            camera_model->unit2image(point3d, pixel_corners[i]);
        }
      
      static const std::array<std::pair<int, int>, 12> lines = {
          std::make_pair(0, 1), std::make_pair(1, 2), std::make_pair(2, 3),
          std::make_pair(3, 0), std::make_pair(0, 4), std::make_pair(1, 5),
          std::make_pair(2, 6), std::make_pair(3, 7), std::make_pair(4, 5),
          std::make_pair(5, 6), std::make_pair(6, 7), std::make_pair(7, 4)};
      for (const auto &pair : lines) {
            cv::line(img,
                     cv::Point(pixel_corners[pair.first](0),
                               pixel_corners[pair.first](1)),
                     cv::Point(pixel_corners[pair.second](0),
                               pixel_corners[pair.second](1)),
                     cv::Scalar(127, 201, 127), 2);
        }
   }

    // Declare a mutex to protect shared resources
std::mutex camera_model_mutex;

int save_pred_img_OD(FSLine fs, Obstacles obstaclesOD, std::vector<uchar> &buffer, 
                     cv::Mat rgb_mat_front, cv::Mat rgb_mat_left, 
                     cv::Mat rgb_mat_rear, cv::Mat rgb_mat_right)
{
    // Lock the mutex before accessing shared resources
    std::lock_guard<std::mutex> lock(camera_model_mutex);
    
    // Initialize camera models safely
    if (camera_models.empty()) {
        try {
            camera_models.emplace("front_camera_fov195",
                                  std::make_shared<FisheyeCameraModel>(camera_model_path, "front_camera_fov195"));
            camera_models.emplace("left_camera_fov195",
                                  std::make_shared<FisheyeCameraModel>(camera_model_path, "left_camera_fov195"));
            camera_models.emplace("right_camera_fov195",
                                  std::make_shared<FisheyeCameraModel>(camera_model_path, "right_camera_fov195"));
            camera_models.emplace("rear_camera_fov195",
                                  std::make_shared<FisheyeCameraModel>(camera_model_path, "rear_camera_fov195"));
        } catch (const std::exception &e) {
            std::cerr << "Error initializing camera models: " << e.what() << std::endl;
            return -1;
        }
    }

    // Clone input matrices
    cv::Mat predfront_mat, predleft_mat, predrear_mat, predright_mat;
    try {
        predfront_mat = rgb_mat_front.clone();
        predleft_mat = rgb_mat_left.clone();
        predrear_mat = rgb_mat_rear.clone();
        predright_mat = rgb_mat_right.clone();
    } catch (const std::exception &e) {
        std::cerr << "Error cloning matrices: " << e.what() << std::endl;
        return -1;
    }

    // Process obstacles, draw landmarks, and lines
    for (const auto &obstacle : obstaclesOD.obstaclesraw) {
        cv::Mat *target_mat = nullptr;

        if (obstacle.camera == "right_camera_fov195") {
            target_mat = &predright_mat;
        } else if (obstacle.camera == "left_camera_fov195") {
            target_mat = &predleft_mat;
        } else if (obstacle.camera == "rear_camera_fov195") {
            target_mat = &predrear_mat;
        } else if (obstacle.camera == "front_camera_fov195") {
            target_mat = &predfront_mat;
        }

        if (target_mat) {
            // Safely draw the obstacle in the corresponding image
            try {
                cv::Point topLeft(obstacle.box.topLeftX, obstacle.box.topLeftY);
                cv::Point bottomRight(obstacle.box.bottomRightX, obstacle.box.bottomRightY);

                cv::circle(*target_mat, topLeft, 4, cv::Scalar(0, 0, 255), -1);
                cv::circle(*target_mat, bottomRight, 4, cv::Scalar(0, 0, 255), -1);
                // Draw other features similarly...

            } catch (const std::exception &e) {
                std::cerr << "Error drawing on image: " << e.what() << std::endl;
            }
        }
    }

    // Process fsline
    for (int i = 0; i < fs.fsline.size(); ++i) {
    // Select the appropriate matrix based on i (0 for front, 1 for rear, etc.)
    const auto& output_result_FS = fs.fsline[i];

    // Check if the fsLinepoints vector is not empty before processing
    if (output_result_FS.fsLinepoints.empty()) {
        continue;  // Skip if there are no points to process
    }

    // Reserve space for the points to avoid reallocation
    std::vector<std::vector<cv::Point>> pls_fsline;
    pls_fsline.reserve(output_result_FS.fsLinepoints.size());

    if (i == 0) {
        for (int j = 0; j < output_result_FS.fsLinepoints.size(); ++j) {
            const auto& point = output_result_FS.fsLinepoints[j];
            cv::Point pt{point.coordinate.x, point.coordinate.y};

            // Get the color based on the point label and draw it on the front matrix
            auto color_bgr = get_space_label_color_bgr(point.pointLabel);
            cv::circle(predfront_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
        }

    } else if (i == 1) {
        for (int j = 0; j < output_result_FS.fsLinepoints.size(); ++j) {
            const auto& point = output_result_FS.fsLinepoints[j];
            cv::Point pt{point.coordinate.x, point.coordinate.y};

            // Get the color and draw it on the rear matrix
            auto color_bgr = get_space_label_color_bgr(point.pointLabel);
            cv::circle(predrear_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
        }

    } else if (i == 3) {
        for (int j = 0; j < output_result_FS.fsLinepoints.size(); ++j) {
            const auto& point = output_result_FS.fsLinepoints[j];
            cv::Point pt{point.coordinate.x, point.coordinate.y};

            // Get the color and draw it on the left matrix
            auto color_bgr = get_space_label_color_bgr(point.pointLabel);
            cv::circle(predleft_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
        }

    } else if (i == 2) {
        for (int j = 0; j < output_result_FS.fsLinepoints.size(); ++j) {
            const auto& point = output_result_FS.fsLinepoints[j];
            cv::Point pt{point.coordinate.x, point.coordinate.y};

            // Get the color and draw it on the right matrix
            auto color_bgr = get_space_label_color_bgr(point.pointLabel);
            cv::circle(predright_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
        }
    }
}
    

    // Concatenate and encode images
    cv::Mat combined, top_row, bottom_row;
    try {
        cv::hconcat(predfront_mat, predrear_mat, top_row);
        cv::hconcat(predleft_mat, predright_mat, bottom_row);
        cv::vconcat(top_row, bottom_row, combined);

        // Encode combined image
        if (!cv::imencode(".jpg", combined, buffer)) {
            std::cerr << "Image encoding failed!" << std::endl;
            return -1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during image processing: " << e.what() << std::endl;
        return -1;
    }

    return 0;  // Success
}
    // int save_pred_img_OD(FSLine fs,Obstacles obstaclesOD, std::vector<uchar> &buffer, cv::Mat rgb_mat_front, cv::Mat rgb_mat_left, cv::Mat rgb_mat_rear, cv::Mat rgb_mat_right)
    // {
    //   if(camera_models.empty()){
    //     camera_models.emplace(
    //         "front_camera_fov195",
    //         std::make_shared<FisheyeCameraModel>(
    //             FisheyeCameraModel(camera_model_path, "front_camera_fov195")));
    //     camera_models.emplace(
    //         "left_camera_fov195",
    //         std::make_shared<FisheyeCameraModel>(
    //             FisheyeCameraModel(camera_model_path, "left_camera_fov195")));
    //     camera_models.emplace(
    //         "right_camera_fov195",
    //         std::make_shared<FisheyeCameraModel>(
    //             FisheyeCameraModel(camera_model_path, "right_camera_fov195")));
    //     camera_models.emplace(
    //         "rear_camera_fov195",
    //         std::make_shared<FisheyeCameraModel>(
    //             FisheyeCameraModel(camera_model_path, "rear_camera_fov195")));
    //   }
    //   cv::Mat predfront_mat = rgb_mat_front.clone(),
    //           predleft_mat = rgb_mat_left.clone(),
    //           predrear_mat = rgb_mat_rear.clone(),
    //           predright_mat = rgb_mat_right.clone();
    //   if (obstaclesOD.obstaclesraw.size() != 0)
    //   {
    //     for (int i = 0; i < obstaclesOD.obstaclesraw.size(); i++)
    //     {
    //       auto output_result = obstaclesOD.obstaclesraw[i];
    //       if (true)
    //       {
    //         if (output_result.camera == "right_camera_fov195")
    //         {
    //           cv::circle(predright_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::circle(predright_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::line(predright_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predright_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predright_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predright_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
              
    //           cv::circle(predright_mat, cv::Point(output_result.landmark4[0].x, output_result.landmark4[0].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predright_mat, cv::Point(output_result.landmark4[1].x, output_result.landmark4[1].y), 6, cv::Scalar(0,255,0),-2, 8, 0);
    //           cv::circle(predright_mat, cv::Point(output_result.landmark4[2].x, output_result.landmark4[2].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predright_mat, cv::Point(output_result.landmark4[3].x, output_result.landmark4[3].y), 6, cv::Scalar(0,255,0),-2, 8, 0);
    //           // DrawBev3DBox(output_result, predright_mat, output_result.camera, camera_models["right_camera_fov195"]);

    //           std::string text = get_obs_label(output_result.label) + " " + std::to_string(output_result.typeConfidence);
    //           int baseLine;
    //           cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    //           cv::Point textOrg(output_result.box.topLeftX, output_result.box.topLeftY - 5);
    //           cv::rectangle(predright_mat, textOrg + cv::Point(0, baseLine), textOrg + cv::Point(textSize.width, -textSize.height), cv::Scalar(0,255,0), cv::FILLED);
    //           cv::putText(predright_mat, text, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0),1);
    //         }
    //         else if (output_result.camera == "left_camera_fov195")
    //         {
    //           cv::circle(predleft_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::circle(predleft_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::line(predleft_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predleft_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predleft_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predleft_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
            
    //           cv::circle(predleft_mat, cv::Point(output_result.landmark4[0].x, output_result.landmark4[0].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predleft_mat, cv::Point(output_result.landmark4[1].x, output_result.landmark4[1].y), 6, cv::Scalar(0,255,0),-2, 8, 0);
    //           cv::circle(predleft_mat, cv::Point(output_result.landmark4[2].x, output_result.landmark4[2].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predleft_mat, cv::Point(output_result.landmark4[3].x, output_result.landmark4[3].y), 6, cv::Scalar(0,255,0),-2, 8, 0);

    //           // DrawBev3DBox(output_result, predright_mat, output_result.camera, camera_models["right_camera_fov195"]);

    //           std::string text = get_obs_label(output_result.label) + " " + std::to_string(output_result.typeConfidence);
    //           int baseLine;
    //           cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    //           cv::Point textOrg(output_result.box.topLeftX, output_result.box.topLeftY - 5);
    //           cv::rectangle(predleft_mat, textOrg + cv::Point(0, baseLine), textOrg + cv::Point(textSize.width, -textSize.height), cv::Scalar(0,255,0), cv::FILLED);
    //           cv::putText(predleft_mat, text, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0),1);
    //         }
    //         else if (output_result.camera == "rear_camera_fov195")
    //         {
    //           cv::circle(predrear_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::circle(predrear_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::line(predrear_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predrear_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predrear_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predrear_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
            
    //           cv::circle(predrear_mat, cv::Point(output_result.landmark4[0].x, output_result.landmark4[0].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predrear_mat, cv::Point(output_result.landmark4[1].x, output_result.landmark4[1].y), 6, cv::Scalar(0,255,0),-2, 8, 0);
    //           cv::circle(predrear_mat, cv::Point(output_result.landmark4[2].x, output_result.landmark4[2].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predrear_mat, cv::Point(output_result.landmark4[3].x, output_result.landmark4[3].y), 6, cv::Scalar(0,255,0),-2, 8, 0);

    //           // DrawBev3DBox(output_result, predright_mat, output_result.camera, camera_models["right_camera_fov195"]);

    //           std::string text = get_obs_label(output_result.label) + " " + std::to_string(output_result.typeConfidence);
    //           int baseLine;
    //           cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    //           cv::Point textOrg(output_result.box.topLeftX, output_result.box.topLeftY - 5);
    //           cv::rectangle(predrear_mat, textOrg + cv::Point(0, baseLine), textOrg + cv::Point(textSize.width, -textSize.height), cv::Scalar(0,255,0), cv::FILLED);
    //           cv::putText(predrear_mat, text, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0),1);
    //         }
    //         else if (output_result.camera == "front_camera_fov195")
    //         {
    //           cv::circle(predfront_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::circle(predfront_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), 4, cv::Scalar(0, 0, 255), -1, 8, 0);
    //           cv::line(predfront_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predfront_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.topLeftX, output_result.box.bottomRightY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predfront_mat, cv::Point(output_result.box.bottomRightX, output_result.box.bottomRightY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
    //           cv::line(predfront_mat, cv::Point(output_result.box.topLeftX, output_result.box.topLeftY), cv::Point(output_result.box.bottomRightX, output_result.box.topLeftY), cv::Scalar(0, 255, 0), 1);
            
    //           cv::circle(predfront_mat, cv::Point(output_result.landmark4[0].x, output_result.landmark4[0].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predfront_mat, cv::Point(output_result.landmark4[1].x, output_result.landmark4[1].y), 6, cv::Scalar(0,255,0),-2, 8, 0);
    //           cv::circle(predfront_mat, cv::Point(output_result.landmark4[2].x, output_result.landmark4[2].y), 6, cv::Scalar(0,255,0), -2, 8, 0);
    //           cv::circle(predfront_mat, cv::Point(output_result.landmark4[3].x, output_result.landmark4[3].y), 6, cv::Scalar(0,255,0),-2, 8, 0);

    //           // DrawBev3DBox(output_result, predright_mat, output_result.camera, camera_models["right_camera_fov195"]);

    //           std::string text = get_obs_label(output_result.label) + " " + std::to_string(output_result.typeConfidence);
    //           int baseLine;
    //           cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    //           cv::Point textOrg(output_result.box.topLeftX, output_result.box.topLeftY - 5);
    //           cv::rectangle(predfront_mat, textOrg + cv::Point(0, baseLine), textOrg + cv::Point(textSize.width, -textSize.height), cv::Scalar(0,255,0), cv::FILLED);
    //           cv::putText(predfront_mat, text, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0),1);
    //         }
    //       }
    //     }
    //   }
    //   if (fs.fsline.size()!=0){
    //     for(int i = 0 ;i< fs.fsline.size();i++)
    //     {
    //       auto output_result_FS = fs.fsline[i];
    //       std::vector<std::vector<cv::Point>> pls_fsline;
    //       if (true)
    //       {
    //         if (i == 0)
    //         {
    //           for (int j = 0; j < output_result_FS.fsLinepoints.size(); j++)
    //           {
    //             cv::Point pt{output_result_FS.fsLinepoints[j].coordinate.x, output_result_FS.fsLinepoints[j].coordinate.y};
    //             auto color_bgr = get_space_label_color_bgr(output_result_FS.fsLinepoints[j].pointLabel);
    //             cv::circle(predfront_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
    //           }
              
    //         }
    //         else if(i == 1)
    //         {
    //           for (int j = 0; j < output_result_FS.fsLinepoints.size(); j++)
    //           {
    //             cv::Point pt{output_result_FS.fsLinepoints[j].coordinate.x, output_result_FS.fsLinepoints[j].coordinate.y};
    //             auto color_bgr = get_space_label_color_bgr(output_result_FS.fsLinepoints[j].pointLabel);
    //             cv::circle(predrear_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
    //           }
    //         }
    //         else if(i == 3)
    //         {
    //           for (int j = 0; j < output_result_FS.fsLinepoints.size(); j++)
    //           {
    //             cv::Point pt{output_result_FS.fsLinepoints[j].coordinate.x, output_result_FS.fsLinepoints[j].coordinate.y};
    //             auto color_bgr = get_space_label_color_bgr(output_result_FS.fsLinepoints[j].pointLabel);
    //             cv::circle(predleft_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
    //           }
              
    //         }
    //         else if(i == 2)
    //         {
    //           for (int j = 0; j < output_result_FS.fsLinepoints.size(); j++)
    //           {
    //             cv::Point pt{output_result_FS.fsLinepoints[j].coordinate.x, output_result_FS.fsLinepoints[j].coordinate.y};
    //             auto color_bgr = get_space_label_color_bgr(output_result_FS.fsLinepoints[j].pointLabel);
    //             cv::circle(predright_mat, pt, 2, cv::Scalar(color_bgr[0], color_bgr[1], color_bgr[2]), -1);
    //           }
              
    //         }
    //       }
          
    //     }  
    //   }

    //   cv::Mat combined, top_row, bottom_row;
    //   cv::hconcat(predfront_mat, predrear_mat, top_row);
    //   cv::hconcat(predleft_mat, predright_mat, bottom_row);
    //   cv::vconcat(top_row, bottom_row, combined);

    //   // static int cnt = 0;
    //   // cv::imwrite("./combined_image_" + std::to_string(cnt) + ".jpg", combined);
    //   // cnt++;
    //   cv::imencode(".jpg", combined, buffer);
    //   return 0;
    // }

    
    DATAFLOW_REGISTER_MODULE(PerceptionOdMoudle)

  } // namespace parking
} // namespace fanya