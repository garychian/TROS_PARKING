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

#include <chrono>
#include <iostream>
#include <thread>

#include <limits>

#include "communication/common/comm_log.h"
#include "communication/subscriber.h"
#include "communication/wait_set/scope_guard.h"
#include "schedulegroup/global_scheduler/global_scheduler.h"
#include "schedulegroup/errors.h"
#include "proto_msg_all.h"


using hobot::communication::COMM_CODE_OK;
using hobot::communication::CommAttr;
using hobot::communication::CompositeOption;
using hobot::communication::ErrorCode;
using hobot::communication::ErrorMsg;
using hobot::communication::EventType;
using hobot::communication::LinkInfo;
using hobot::communication::ParticipantAttr;
using hobot::communication::ScopeGuard;
using hobot::communication::Subscriber;

using hobot::communication::E2ErrorInfo;
using hobot::communication::E2EventType;
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

using hobot::message::TypeWrapper;
namespace ExtendInfoKey = hobot::message::ExtendInfoKey;

using hobot::communication::ProtobufSerializer;

using PadRealTimeLocationSerializer = ProtobufSerializer<loc::padRealTimeLocation>;
using LocStatusSerializer = ProtobufSerializer<loc::LocStatus>;
using VehiclePosSerializer = ProtobufSerializer<loc::VehiclePos>;
using App2emap_DRSerializer = ProtobufSerializer<loc::App2emap_DR>;
using MapInfoSerializer = ProtobufSerializer<loc::MapInfo>;
using ParkingSlotsSerializer = ProtobufSerializer<loc::parkingSlots>;
using LaneLinesSerializer = ProtobufSerializer<loc::laneLines>;
using StopLinesSerializer = ProtobufSerializer<loc::stopLines>;
using BumpsSerializer = ProtobufSerializer<loc::bumps>;
using CrossWalksSerializer = ProtobufSerializer<loc::crossWalks>;
using ArrowsSerializer = ProtobufSerializer<loc::arrows>;

using QuadParkingSlotsSerializer = ProtobufSerializer<rd::QuadParkingSlots>;
using ImageSerializer = ProtobufSerializer<rd::Image>;
using ObstaclesSerializer = ProtobufSerializer<od::Obstacles>;
using HeaderSerializer = ProtobufSerializer<od::Header>;
using FSLineSerializer = ProtobufSerializer<od::FSLine>;

struct Args {
  int participant_id = 0;
  int protocol = PROTOCOL_INVALID;
  bool is_dynamic = false;
};

void PrintUsage() {
  std::cout << "Usage:\n"
            << "     ./subscriber {ProtocolType}\n"
            << "     ./subscriber {ProtocolType} [participant_id]\n"
            << "     ./subscriber {ProtocolType} [dynamic]\n"
            << "note: this example only support following protocols:\n"
            << "protocol id : protocol type\n"
            << "         0  :  hybrid\n"
            << "         2  :  zmq tcp\n"
            << "         3  :  zmq epgm\n"
            << "         4  :  zmq ipc\n"
            << "         6  :  shared memory (not for hbmem)\n"
            << "         7  :  sdio\n"
            << "         8  :  pcie" << std::endl;
}

// only support 0 and positive integer, return -1 if illegal
int Atoi(char *str_num) {
  std::string int_max = std::to_string(INT_MAX);
  if (strlen(str_num) > int_max.size()) return -1;
  for (int i = 0; i < strlen(str_num); ++i) {
    if (str_num[i] < '0' || str_num[i] > '9') {
      return -1;
    }
  }
  if (strlen(str_num) == int_max.size()) {
    for (int i = 0; i < int_max.size(); ++i) {
      if (str_num[i] > int_max[i]) {
        return -1;
      }
    }
  }
  return atoi(str_num);
}

bool ParseCmd(int argc, char **argv, Args &args) {
  if (argc < 2 || argc > 3) {
    PrintUsage();
    return false;
  }
  if ((args.protocol = Atoi(argv[1])) == -1) {
    std::cout << "invalid protocol type: " << argv[1] << std::endl;
    return false;
  }
  if (argc == 2) {
    args.is_dynamic = false;
    std::cout << "participant id hasn't been input, default value will be use"
              << std::endl;
    return true;
  } else {
    if (std::string(argv[2]) == "dynamic") {
      args.is_dynamic = true;
      return true;
    } else if ((args.participant_id = Atoi(argv[2])) != -1) {
      return true;
    } else {
      PrintUsage();
      return false;
    }
  }
}

bool MatchParticipantId(const Args &args, CommAttr &comm_attr) {
  switch (args.protocol) {
    case PROTOCOL_HYBRID:
      if (args.is_dynamic) {
        return true;
      }
      std::cout << "using protocol hybrid must turn on dynamic discovery"
                << std::endl;
      return false;
    case PROTOCOL_INTRA:
      std::cout << "can not use protocol intra in this example, run "
                   "intra_pub_sub"
                << std::endl;
      return false;
    case PROTOCOL_ZMQ_TCP:
      if (!args.is_dynamic) {
        if (args.participant_id == 0)
          comm_attr.participant_attrs_.push_back(ParticipantAttr{2});
        else
          comm_attr.participant_attrs_.push_back(
              ParticipantAttr{args.participant_id});
      }
      return true;
    case PROTOCOL_ZMQ_EPGM:
      if (!args.is_dynamic) {
        if (args.participant_id == 0)
          comm_attr.participant_attrs_.push_back(ParticipantAttr{7});
        else
          comm_attr.participant_attrs_.push_back(
              ParticipantAttr{args.participant_id});
      }
      return true;
    case PROTOCOL_ZMQ_IPC:
      if (!args.is_dynamic) {
        if (args.participant_id == 0)
          comm_attr.participant_attrs_.push_back(ParticipantAttr{1});
        else
          comm_attr.participant_attrs_.push_back(
              ParticipantAttr{args.participant_id});
      }
      return true;
    case PROTOCOL_DDS:
      std::cout << "this example do not support protocol DDS, please run "
                   "corresponding fastdds example"
                << std::endl;
      return false;
    case PROTOCOL_SHM:
      if (args.is_dynamic) return true;
      std::cout << "using protocol shm must turn on dynamic discovery"
                << std::endl;
      return false;
    case PROTOCOL_SDIO:
      if (args.is_dynamic) {
        std::cout << "dynamic discovery do not support protocol sdio"
                  << std::endl;
        return false;
      }
      if (args.participant_id == 0)
        comm_attr.participant_attrs_.push_back(ParticipantAttr{4});  // 4 is cp
      else
        comm_attr.participant_attrs_.push_back(
            ParticipantAttr{args.participant_id});
      return true;
    case PROTOCOL_PCIE:
      if (args.is_dynamic) {
        std::cout << "dynamic discovery do not support protocol pcie"
                  << std::endl;
        return false;
      }
      if (args.participant_id == 0) {
        std::cout << "if you want to use ep2ep mode, input corresponding "
                     "participant id, in default config file, it is 10"
                  << std::endl;
        comm_attr.participant_attrs_.push_back(ParticipantAttr{9});
      } else {
        comm_attr.participant_attrs_.push_back(
            ParticipantAttr{args.participant_id});
      }
      return true;
    case PROTOCOL_COMPOSITE:
      comm_attr.composite_options.push_back(
          CompositeOption(PROTOCOL_INTRA));
      comm_attr.composite_options.push_back(CompositeOption(PROTOCOL_SHM));
      comm_attr.composite_options.push_back(
          CompositeOption(PROTOCOL_ZMQ_TCP));
      return true;
    default:
      std::cout << "invalid protocol type: " << args.protocol << std::endl;
      return false;
  }
}

#if 0  // not used in current file
static void SimpleImageSubCallback(
    const std::shared_ptr<ImgProtoMsg> &image_msg) {
  std::cout << "received image msg info: \n "
            << "height=" << image_msg->proto.height()
            << "; width=" << image_msg->proto.width()
            << "; channel=" << image_msg->proto.channel() << ";";
  auto data_vec = image_msg->GetData()->GetDataVec();
  auto data_size_vec = image_msg->GetData()->GetDataSizeVec();
  size_t size = image_msg->GetData()->GetDataVec().size();
  for (int i = 0; i < size; ++i) {
    std::cout << "data" << i << " header=" << (char *)data_vec[i] << "; data"
              << i << " size=" << data_size_vec[i] << std::endl;
  }
  std::cout << "msg gen_ts: " << image_msg->GetGenTimestamp()
            << ",msg done_ts: " << image_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = image_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = image_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}
#endif

static void PadRealTimeLocationSubCallbackWithTopic(
    const std::shared_ptr<PadRealTimeLocationMsg> &pad_real_time_loc_msg, const std::string &topic) {
  std::cout << "sub_pad_real_time_loc msg topic:" << topic
            << "msg gen_ts: " << pad_real_time_loc_msg->GetGenTimestamp()
            << ",msg done_ts: " << pad_real_time_loc_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = pad_real_time_loc_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = pad_real_time_loc_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void LocStatusSubCallbackWithTopic(
    const std::shared_ptr<LocStatusMsg> &loc_status_msg, const std::string &topic) {
  std::cout << "sub_loc_status msg topic:" << topic
            << "msg gen_ts: " << loc_status_msg->GetGenTimestamp()
            << ",msg done_ts: " << loc_status_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = loc_status_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = loc_status_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void VehiclePosSubCallbackWithTopic(
    const std::shared_ptr<VehiclePosMsg> &vehicle_pose_msg, const std::string &topic) {
  std::cout << "sub_vehicle_pose msg topic:" << topic
            << "msg gen_ts: " << vehicle_pose_msg->GetGenTimestamp()
            << ",msg done_ts: " << vehicle_pose_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = vehicle_pose_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = vehicle_pose_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void App2emap_DRSubCallbackWithTopic(
    const std::shared_ptr<App2emap_DRMsg> &app2emap_msg, const std::string &topic) {
  std::cout << "sub_app2emap msg topic:" << topic
            << "msg gen_ts: " << app2emap_msg->GetGenTimestamp()
            << ",msg done_ts: " << app2emap_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = app2emap_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = app2emap_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void MapInfoSubCallbackWithTopic(
    const std::shared_ptr<MapInfoMsg> &map_info_msg, const std::string &topic) {
  std::cout << "sub_map_info msg topic:" << topic
            << "msg gen_ts: " << map_info_msg->GetGenTimestamp()
            << ",msg done_ts: " << map_info_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = map_info_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = map_info_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void ParkingSlotsSubCallbackWithTopic(
    const std::shared_ptr<ParkingSlotsMsg> &parking_slot_msg, const std::string &topic) {
  std::cout << "sub_parking_slot msg topic:" << topic
            << "msg gen_ts: " << parking_slot_msg->GetGenTimestamp()
            << ",msg done_ts: " << parking_slot_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = parking_slot_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = parking_slot_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void LaneLinesSubCallbackWithTopic(
    const std::shared_ptr<LaneLinesMsg> &stio_lane_lines_msg, const std::string &topic) {
  std::cout << "sub_stio_lane_lines msg topic:" << topic
            << "msg gen_ts: " << stio_lane_lines_msg->GetGenTimestamp()
            << ",msg done_ts: " << stio_lane_lines_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = stio_lane_lines_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = stio_lane_lines_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void StopLinesSubCallbackWithTopic(
    const std::shared_ptr<StopLinesMsg> &stop_lines_msg, const std::string &topic) {
  std::cout << "sub_stop_lines msg topic:" << topic
            << "msg gen_ts: " << stop_lines_msg->GetGenTimestamp()
            << ",msg done_ts: " << stop_lines_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = stop_lines_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = stop_lines_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void BumpsSubCallbackWithTopic(
    const std::shared_ptr<BumpsMsg> &bumps_msg, const std::string &topic) {
  std::cout << "sub_bumps msg topic:" << topic
            << "msg gen_ts: " << bumps_msg->GetGenTimestamp()
            << ",msg done_ts: " << bumps_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = bumps_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = bumps_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void CrossWalksSubCallbackWithTopic(
    const std::shared_ptr<CrossWalksMsg> &crosswalks_msg, const std::string &topic) {
  std::cout << "sub_crosswalks msg topic:" << topic
            << "msg gen_ts: " << crosswalks_msg->GetGenTimestamp()
            << ",msg done_ts: " << crosswalks_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = crosswalks_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = crosswalks_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void ArrowsSubCallbackWithTopic(
    const std::shared_ptr<ArrowsMsg> &arrows_msg, const std::string &topic) {
  std::cout << "sub_arrows msg topic:" << topic
            << "msg gen_ts: " << arrows_msg->GetGenTimestamp()
            << ",msg done_ts: " << arrows_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = arrows_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = arrows_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }
}

static void SubE2EventCallback(E2EventType &event,
          const std::shared_ptr<E2ErrorInfo> &info) {
  std::cout << "E2E check failed error code " << event
            << ", error info is " << info->error_msg
            << ", participant info is " << info->link_info << std::endl;
}

// add rd sub topic callback
static void RdParkingSlotsSubCallbackWithTopic(
    const std::shared_ptr<QuadParkingSlotsMsg> & rd_quad_parking_slots_msg, const std::string &topic) {
  std::cout << "rd_quad_parking_slots_msg topic:" << topic
            << "msg gen_ts: " << rd_quad_parking_slots_msg->GetGenTimestamp()
            << ",msg done_ts: " << rd_quad_parking_slots_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = rd_quad_parking_slots_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = rd_quad_parking_slots_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }

  auto proto_ = rd_quad_parking_slots_msg->proto;
  std::cout << "this message proto info :" << proto_.DebugString() << std::endl;

}

// add rd sub topic callback
static void RdImageSubCallbackWithTopic(
    const std::shared_ptr<ImageMsg> & rd_image_msg, const std::string &topic) {
  std::cout << "rd_image_msg topic:" << topic
            << "msg gen_ts: " << rd_image_msg->GetGenTimestamp()
            << ",msg done_ts: " << rd_image_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = rd_image_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = rd_image_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }

  //auto proto_ = rd_image_msg->proto;
  //std::cout << "this message proto info :" << proto_.DebugString() << std::endl;
  auto psd_image = std::dynamic_pointer_cast<ImageMsg>(rd_image_msg);
  // cv::Mat received_image(psd_image->proto.height(), psd_image->proto.width(), CV_8UC3);
  // std::vector<uint8_t> data_vec(psd_image->proto.data().begin(), psd_image->proto.data().end());
  // imdecode(data_vec, cv::IMREAD_COLOR, &received_image);
  static int num = 0;
  ++num;
  if (num%2 == 0){
    num = 0;
  }
  std::string image_name = std::string("save_psd_image") + std::to_string(num) + std::string(".yuv");
  FILE *output = fopen(image_name.c_str(), "wb");
  // 假定psd_image是一个结构体，包含了proto成员
  const char *psd_data = psd_image->proto.data().data();
  size_t psd_size = psd_image->proto.data().size();
  fwrite(psd_data, 1, psd_size, output);
  fclose(output);
  // DFHLOG_I("sub_psd_image msg timestamp: {}, height = {}, width = {}",
  //   rd_image_msg->GetGenTimestamp(), psd_image->proto.height(), psd_image->proto.width());
}

// add od sub topic callback
static void OdObstaclesSubCallbackWithTopic(
    const std::shared_ptr<ObstaclesMsg> &od_obstacles_msg, const std::string &topic) {
  std::cout << "od_obstacles_msg msg topic:" << topic
            << "msg gen_ts: " << od_obstacles_msg->GetGenTimestamp()
            << ",msg done_ts: " << od_obstacles_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = od_obstacles_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = od_obstacles_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }

  auto obstacles_proto = od_obstacles_msg->proto;
  std::cout << "this message proto info: " << obstacles_proto.DebugString() << std::endl;

}

static void OdFSLineSubCallbackWithTopic(
    const std::shared_ptr<FSLineMsg> &od_fsline_msg, const std::string &topic) {
  std::cout << "od_fsline_msg msg topic:" << topic
            << "msg gen_ts: " << od_fsline_msg->GetGenTimestamp()
            << ",msg done_ts: " << od_fsline_msg->GetDoneTimestamp() << std::endl;
  TypeWrapper send_time, recv_time;
  bool res1 = od_fsline_msg->GetExtInfo(ExtendInfoKey::kSendTime, send_time);
  bool res2 = od_fsline_msg->GetExtInfo(ExtendInfoKey::kRecvTime, recv_time);
  if (res1 && res2) {
    std::cout << "recv_time = " << recv_time.GetValueInt64()
              << ", send time = " << send_time.GetValueInt64() << std::endl;
    int64_t latency = recv_time.GetValueInt64() - send_time.GetValueInt64();
    std::cout << "this message latency is " << latency / 1000 << " us"
              << std::endl;
  }

  auto fsline_proto = od_fsline_msg->proto;
  std::cout << "this message proto info: " << fsline_proto.DebugString() << std::endl;
}

int main(int argc, char *argv[]) {
  // parse input parameter
  Args args;
  if (!ParseCmd(argc, argv, args)) {
    return -1;
  }

  hobot::schedulegroup::GlobalSchedulerParam param;
  int32_t create_ret = hobot::schedulegroup::GlobalScheduler::Init(param);
  if (create_ret != hobot::schedulegroup::ErrorCode::OK) {
    std::cout << "GlobalScheduler::Init failed" << std::endl;
    return -1;
  }


  // // hobot::communication::Init support parase json string
  // std::string json_str = R"({"discovery_pattern": 1,
  //                             "participants": [
  //                               {
  //                                 "id": 1,
  //                                 "protocol": "zmq_ipc",
  //                                 "link_info": "/tmp/precipi.ipc",
  //                                 "link_type": "default"
  //                               },
  //                               {
  //                                 "id": 2,
  //                                 "protocol": "zmq_tcp",
  //                                 "link_info": "127.0.0.1:8890",
  //                                 "link_type": "default"
  //                               }
  //                             ]
  //                           })";
  // hobot::communication::Init(json_str);
  hobot::communication::Init("communication.json");
  ScopeGuard gurad([]() {
    hobot::communication::DeInit();
    hobot::schedulegroup::GlobalScheduler::DeInit();
  });

  // you can print version of communication
  std::string version = hobot::communication::GetVersion();
  std::cout << "communication version is " << version << std::endl;

  CommAttr comm_attr;
  if (!MatchParticipantId(args, comm_attr)) {
    return -1;
  }
  comm_attr.e2e_callback = SubE2EventCallback;

  // sub PadRealTimeLocation
  std::shared_ptr<Subscriber<PadRealTimeLocationSerializer>> pad_real_time_loc_subscriber = 
    Subscriber<PadRealTimeLocationSerializer>::NewExt(comm_attr, "/loc/pad_realtime_loc", 0,
    PadRealTimeLocationSubCallbackWithTopic, args.protocol);
  if (!pad_real_time_loc_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub LocStatus
  std::shared_ptr<Subscriber<LocStatusSerializer>> loc_status_subscriber = 
    Subscriber<LocStatusSerializer>::NewExt(comm_attr, "/loc/loc_status", 0,
    LocStatusSubCallbackWithTopic, args.protocol);
  if (!loc_status_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub VehiclePos
  std::shared_ptr<Subscriber<VehiclePosSerializer>> vehicle_pose_subscriber = 
    Subscriber<VehiclePosSerializer>::NewExt(comm_attr, "/loc/vehicle_pose", 0,
    VehiclePosSubCallbackWithTopic, args.protocol);
  if (!vehicle_pose_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub App2emap_DR
  std::shared_ptr<Subscriber<App2emap_DRSerializer>> app2emap_subscriber = 
    Subscriber<App2emap_DRSerializer>::NewExt(comm_attr, "/loc/app2emap", 0,
    App2emap_DRSubCallbackWithTopic, args.protocol);
  if (!app2emap_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub MapInfo
  std::shared_ptr<Subscriber<MapInfoSerializer>> map_info_subscriber = 
    Subscriber<MapInfoSerializer>::NewExt(comm_attr, "/loc/map_info", 0,
    MapInfoSubCallbackWithTopic, args.protocol);
  if (!map_info_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub parkingSlots
  std::shared_ptr<Subscriber<ParkingSlotsSerializer>> parking_slot_subscriber = 
    Subscriber<ParkingSlotsSerializer>::NewExt(comm_attr, "/loc/parking_slot", 0,
    ParkingSlotsSubCallbackWithTopic, args.protocol);
  if (!parking_slot_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub LaneLines
  std::shared_ptr<Subscriber<LaneLinesSerializer>> stio_lane_lines_subscriber = 
    Subscriber<LaneLinesSerializer>::NewExt(comm_attr, "/loc/stio_lane_lines", 0,
    LaneLinesSubCallbackWithTopic, args.protocol);
  if (!stio_lane_lines_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub StopLines
  std::shared_ptr<Subscriber<StopLinesSerializer>> stop_lines_subscriber = 
    Subscriber<StopLinesSerializer>::NewExt(comm_attr, "/loc/stop_lines", 0,
    StopLinesSubCallbackWithTopic, args.protocol);
  if (!stop_lines_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub Bumps
  std::shared_ptr<Subscriber<BumpsSerializer>> bumps_subscriber = 
    Subscriber<BumpsSerializer>::NewExt(comm_attr, "/loc/bumps", 0,
    BumpsSubCallbackWithTopic, args.protocol);
  if (!bumps_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub CrossWalks
  std::shared_ptr<Subscriber<CrossWalksSerializer>> crosswalks_subscriber = 
    Subscriber<CrossWalksSerializer>::NewExt(comm_attr, "/loc/crosswalks", 0,
    CrossWalksSubCallbackWithTopic, args.protocol);
  if (!crosswalks_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub Arrows
  std::shared_ptr<Subscriber<ArrowsSerializer>> arrows_subscriber = 
    Subscriber<ArrowsSerializer>::NewExt(comm_attr, "/loc/arrows", 0,
    ArrowsSubCallbackWithTopic, args.protocol);
  if (!arrows_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub rd QuadParkingSlots
  std::shared_ptr<Subscriber<QuadParkingSlotsSerializer>> rd_quad_parking_slots_subscriber = 
    Subscriber<QuadParkingSlotsSerializer>::NewExt(comm_attr, "/psd/quad_parking_slots", 0,
    RdParkingSlotsSubCallbackWithTopic, args.protocol);
  if (!rd_quad_parking_slots_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub rd image
  std::shared_ptr<Subscriber<ImageSerializer>> rd_image_subscriber = 
    Subscriber<ImageSerializer>::NewExt(comm_attr, "/psd/image", 0,
    RdImageSubCallbackWithTopic, args.protocol);
  if (!rd_image_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  // sub od topcis
  std::shared_ptr<Subscriber<ObstaclesSerializer>> od_obstacles_subscriber = 
    Subscriber<ObstaclesSerializer>::NewExt(comm_attr, "/od/obstacles", 0,
    OdObstaclesSubCallbackWithTopic, args.protocol);
  if (!od_obstacles_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  std::shared_ptr<Subscriber<FSLineSerializer>> od_fsline_subscriber = 
    Subscriber<FSLineSerializer>::NewExt(comm_attr, "/od/fsline", 0,
    OdFSLineSubCallbackWithTopic, args.protocol);
  if (!od_fsline_subscriber) {
    std::cout << " create subscriber failed" << std::endl;
    return -1;
  }

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << " subscriber is running ..." << std::endl;
  }

  return 0;
}
