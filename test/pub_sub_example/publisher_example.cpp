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

#include <iostream>
#include <thread>
#include <chrono>

#include "communication/common/comm_log.h"
#include "communication/publisher.h"
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
using hobot::communication::Publisher;
using hobot::communication::ScopeGuard;

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

using hobot::communication::ProtobufSerializer;

// for proto serialize

using SlotlabelSerializer = ProtobufSerializer<fsm::Slotlabel>;
using ApastatusreqSerializer = ProtobufSerializer<fsm::Apastatusreq>;

struct Args {
  int participant_id = 0;
  int protocol = PROTOCOL_INVALID;
  // the speed of different protocols is different, to avoid lose message, some
  // protocol must sleep a while during between each request
  int pub_interval = 0;
  bool is_dynamic = false;
};

void PrintUsage() {
  std::cout << "Usage:\n"
            << "     ./publisher {ProtocolType} [dynamic]\n"
            << "     ./publisher {ProtocolType} [participant_id]\n"
            << "protocol id : protocol type\n"
            << "         0  :  hybrid\n"
            << "         2  :  zmq tcp\n"
            << "         3  :  zmq epgm\n"
            << "         4  :  zmq ipc\n"
            << "         6  :  shared memory (not for hbmem)\n"
            << "         7  :  sdio\n"
            << "         8  :  pcie" << std::endl;
}

// only support positive integer, return -1 if illegal
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

bool MatchParticipantId(Args &args, CommAttr &comm_attr) {
  switch (args.protocol) {
    case PROTOCOL_ZMQ_TCP:
      if (!args.is_dynamic) {
        if (args.participant_id == 0)
          comm_attr.participant_attrs_.push_back(ParticipantAttr{2});
        else
          comm_attr.participant_attrs_.push_back(
              ParticipantAttr{args.participant_id});
      }
      args.pub_interval = 10;
      return true;
    case PROTOCOL_ZMQ_IPC:
      if (!args.is_dynamic) {
        if (args.participant_id == 0)
          comm_attr.participant_attrs_.push_back(ParticipantAttr{1});
        else
          comm_attr.participant_attrs_.push_back(
              ParticipantAttr{args.participant_id});
      }
      args.pub_interval = 10;
      return true;
    case PROTOCOL_SHM:
      if (args.is_dynamic) {
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

// event global value
EventType g_pub_event = EventType(2);
void pub_connlisteners(const std::shared_ptr<LinkInfo> participants,
                       EventType event) {
  const std::string& protocol = participants->protocol;
  std::cout << "connection status changed, participant info:\n"
            << "            participant id: " << participants->id << std::endl
            << "                  protocol: " << participants->protocol
            << std::endl
            << "                 link info: " << participants->link_info
            << std::endl;
  COHLOGI("****pub_connlisteners call succefull!");
  g_pub_event = event;
  if (event == EventType::EVENT_CONNECTED) {
    COHLOGI("++++++ {} Pub connected succeful! ++++", protocol);
  }
  if (event == EventType::EVENT_CONNECT_FAILED) {
    COHLOGI("----- {} Pub connected failed!! -----", protocol);
  }
  if (event == EventType::EVENT_DISCONNECTED) {
    COHLOGI("========{} Pub disconnected! ======", protocol);
  }
}

int main(int argc, char** argv) {
  
  hobot::schedulegroup::GlobalSchedulerParam param;
  int32_t create_ret = hobot::schedulegroup::GlobalScheduler::Init(param);

  if (create_ret != hobot::schedulegroup::ErrorCode::OK) {
    std::cout << "GlobalScheduler::Init failed" << std::endl;
    return -1;
  }

  hobot::communication::Init("communication.json");
  ScopeGuard gurad([]() {
    hobot::communication::DeInit();
    hobot::schedulegroup::GlobalScheduler::DeInit();
  });

  // you can print version of communication
  std::string version = hobot::communication::GetVersion();
  std::cout << "communication version is " << version << std::endl;

  CommAttr slot_label_comm_attr;
  Args slot_label_args;
  slot_label_args.protocol = PROTOCOL_ZMQ_TCP;
  slot_label_args.participant_id = 1;

  if (!MatchParticipantId(slot_label_args, slot_label_comm_attr)) {
    return -1;
  }
  
  // pub slotlabel
  ErrorCode error_code;
  auto slot_label_publisher =
      Publisher<SlotlabelSerializer>::New(slot_label_comm_attr,
                                          "/fsm/slot_label",
                                          0,
                                          slot_label_args.protocol,
                                          &error_code,
                                          pub_connlisteners);
  if (!slot_label_publisher) {
    std::cout << " create publisher failed";
    return -1;
  }

  CommAttr apa_status_req_comm_attr;
  Args apa_status_req_args;
  apa_status_req_args.protocol = PROTOCOL_ZMQ_TCP;
  apa_status_req_args.participant_id = 2;

  if (!MatchParticipantId(apa_status_req_args, apa_status_req_comm_attr)) {
    return -1;
  }

  auto apa_status_req_publisher =
      Publisher<ApastatusreqSerializer>::New(apa_status_req_comm_attr,
                                          "/fsm/apa_status_req",
                                          0,
                                          apa_status_req_args.protocol,
                                          &error_code,
                                          pub_connlisteners);
  if (!apa_status_req_publisher) {
    std::cout << " create publisher failed";
    return -1;
  }


  while (true){
    
    {
      auto gen_ts = GetTimeStamp();
      auto msg = std::make_shared<SlotlabelMsg>();
      msg->SetGenTimestamp(gen_ts);
      msg->proto.set_targetslotlabel(33);
      msg->SetDoneTimestamp(GetTimeStamp());
      auto ret = slot_label_publisher->Pub(msg);
      if (ret != COMM_CODE_OK) {
        std::cout << "pub failed, reason: " << ErrorMsg(ret) << std::endl;
      }else{
        std::cout << "pub SlotlabelMsg successful, ts = " << gen_ts << std::endl;
      }

    }

    {
      auto gen_ts = GetTimeStamp();
      auto msg = std::make_shared<ApastatusreqMsg>();
      msg->SetGenTimestamp(gen_ts);
      msg->proto.set_apastatusreq(fsm::apaStatusReqType::enable);
      auto ret = apa_status_req_publisher->Pub(msg);
      if (ret != COMM_CODE_OK) {
        std::cout << "pub failed, reason: " << ErrorMsg(ret) << std::endl;
      }else{
        std::cout << "pub ApastatusreqMsg successful, ts = " <<  gen_ts << std::endl;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  // pub apa status req
  COHLOGI("publish all process finished!");
  return 0;
}
