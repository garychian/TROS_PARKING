// Copyright (c) [2021-2023] [Horizon Robotics][Horizon Bole].
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


#ifndef COMMUNICATION_CORE_TYPES_H_
#define COMMUNICATION_CORE_TYPES_H_

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "communication/common/errors.h"
#include "message/message.h"
#include "communication/common/message_wrapper.h"
#include "message/proto/proto_serializer.hpp"
#include "message/proto/proto_msg.hpp"

#include "communication/utils/cycle_queue.h"
#include "communication/common/executor.h"
#include "communication/common/qos_policy.h"
#ifdef _WIN32
#define NOMINMAX
#include "winsock2.h"
#else
#include <fcntl.h>
#endif

#ifndef DLLEXPORT

#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

using hobot::message::DataRef;
using hobot::message::DefaultSerializer;
using hobot::message::Message;
using hobot::message::MsgBroker;
using hobot::message::ProtobufSerializer;
using hobot::message::ProtoMsg;
using hobot::message::Serializable;
using hobot::message::SerializedData;
using hobot::message::SerializerPolicy;
using hobot::message::spDataRef;
using hobot::message::spMsg;
using hobot::message::spSerData;

// -65535 as dumb domain id
static constexpr int DUMB_DOMAIN_ID{-0xFFFF};
static constexpr int DUMB_PARTICIPANT_ID{-0xFFFF};

using ProtocolType = int;

constexpr int PROTOCOL_INVALID{-1};
constexpr int PROTOCOL_HYBRID{0};
constexpr int PROTOCOL_INTRA{1};
constexpr int PROTOCOL_ZMQ_TCP{2};
constexpr int PROTOCOL_ZMQ_EPGM{3};
constexpr int PROTOCOL_ZMQ_IPC{4};
constexpr int PROTOCOL_DDS{5};
constexpr int PROTOCOL_SHM{6};
constexpr int PROTOCOL_SDIO{7};
constexpr int PROTOCOL_PCIE{8};
constexpr int PROTOCOL_COMPOSITE{9};

/**
 * @brief qos attr
 *
 */
struct QoSAttr {
  QoSProfile qos_profile_;
};

/**
 * @brief participant_id_ is same as the id in config file or the ruturn value
 * of RegisterParticipantExtInfo
 *
 */
struct ParticipantAttr {
  int participant_id_;
};

/**
 * @brief E2E Event type
 *
 * @note It's E2E Check type, also a parameter of E2EventCallback 
 */
enum E2EventType : int {
  E2E_OK = 0,
  E2E_CONFIG_NOT_MATCH,
  E2E_DATA_CRC_FAILED,
  E2E_DATA_REPEATED,
  E2E_DATA_LOST,
  E2E_DATA_DISORDER
};

/**
 * @brief E2E failed infos
 *
 * @note It's a parameter of E2EventCallback 
 */
struct E2ErrorInfo {
  std::string error_msg;
  std::string topic;
  int domain_id;
  std::string link_info;
  int participant_id;
  uint64_t pre_frame;
  uint64_t cur_frame;
  int pre_counter;
  int cur_counter;
};

/**
 * @brief E2E heck failed event Callback func prototype
 * 
 */
using E2EventCallback = std::function<
   void(E2EventType &, const std::shared_ptr<E2ErrorInfo> &)>;

/**
 * @brief Composite option's definition
 * ProtoclType:  PROTOCOL_INTRA ~ PROTOCOL_PCIE or plugin defined
 * protocol;
 * participant_id : participant id;
 */
struct CompositeOption {
  CompositeOption() { }
  explicit CompositeOption(ProtocolType type)
          : protocol_type{type} { }
  CompositeOption(ProtocolType type, int parti_id)
          : protocol_type{type},
            participant_id{parti_id} { }
  ProtocolType protocol_type;
  int participant_id{DUMB_PARTICIPANT_ID};
};

/**
 * @brief Attribute for create communication object
 *
 */
struct CommAttr {
  QoSAttr qos_;
  // only for file-based discovery pattern
  std::vector<ParticipantAttr> participant_attrs_;
  // msg history size for sync subscriber
  // in most cases, users don't care this attr.
  int msg_history_size_for_sync_sub{-1};
  // executor for process callbacks
  std::shared_ptr<Executor> executor_;

  // check service duplicates and ensure that
  //  only one same service is running
  // same service is: domain,topic,protocol all same
  // 0:not check  >0:the max wait time for Synchronize
  int service_dup_check_ms_{0};

  // If intra_direct_callback_ is set to true,
  // the intra protocol will call the callback directly,
  // but some features of the communication will not be available,
  // such as tools.
  // So if there is no special requirement for performance,
  // do not to set this parameter.
  bool intra_direct_callback_{false};

  // If enable E2E protect, you can set this callback
  // to get failed E2E event type and related infos
  E2EventCallback e2e_callback{nullptr};

  // If use PROTOCOL_COMPOSITE to create pub/sub/client/service/action, then use
  // composite_options to create participant to join.if protocol is
  // PROTOCOL_COMPOSITE and composite_options is empty, then return false
  std::vector<CompositeOption> composite_options;
};

/**
 * @brief used to notify connect event in connevent callback
 *
 */
enum EventType {
  EVENT_CONNECTED = 0,
  EVENT_CONNECT_FAILED,
  EVENT_DISCONNECTED,
  EVENT_STATUS_MAX
};

/**
 * @brief used to notify user linkinfo in connevent callback
 *
 */
struct LinkInfo {
  int id;
  std::string protocol;
  std::string link_info;
};

/**
 * @brief Used by Register ext participant info
 *
 */
struct DLLEXPORT ParticipantInfoExt {
  ParticipantInfoExt()
      : type_{PROTOCOL_INVALID},
        protocol{""},
        link_info{""},
        link_type{""},
        config_options{nullptr},
        config_options_str{""},
        e2e_options_str{""} {}
  ProtocolType type_;
  std::string protocol;
  std::string link_info;
  std::string link_type;
  void *config_options;
  std::string config_options_str;
  std::string e2e_options_str;
};

/**
 * @brief Connect Event Callback func prototype
 * 
 */
using ConnEventCallback = std::function<void(
    const std::shared_ptr<LinkInfo> participants, EventType event)>;

/**
 * @brief Subscriber Callback func prototype
 *
 * @tparam Serializer
 */
template<typename Serializer>
using SubscriberCallback =
std::function<void(const std::shared_ptr<typename Serializer::MsgType> &)>;

/**
 * @brief Subscriber Callback func prototype with topic
 *
 * @tparam Serializer
 */
template<typename Serializer>
using SubscriberWithTopicCallback =
std::function<void(const std::shared_ptr<typename Serializer::MsgType> &,
                   const std::string &)>;

/**
 * @brief Service Callback func prototype
 *
 * @tparam SerializerRequest
 * @tparam SerializerResponse
 */
template<typename SerializerRequest, typename SerializerResponse>
using ServiceCallback =
std::function<void(const std::shared_ptr<typename SerializerRequest::MsgType> &,
                   std::shared_ptr<typename SerializerResponse::MsgType> &)>;

/**
 * @brief Client callback func prototype for AsyncSendRequest
 * 
 * @tparam SerializerResponse 
 */
template<typename SerializerResponse>
using ClientCallback =
std::function<void(ErrorCode, const std::shared_ptr<
    typename SerializerResponse::MsgType> &)>;

/**
 * @brief DoNothing Serializer for user
 * 
 * @tparam T 
 */
template<typename T>
struct DoNothingSerializer : public DefaultSerializer {
  using MsgType = T;
};

/**
 * @brief Common Serializer for user
 * 
 */
struct CommonSerializer : public DefaultSerializer {
  using MsgType = Message;
};

/**
 * @brief Communication Initialize function
 * 
 * @param config_file 
 * @return ErrorCode 
 */
DLLEXPORT ErrorCode Init(const std::string &config_file = "");

/**
 * @brief Update protocol to subidl mapping
 *
 * @param protocol_type
 * @param sub_idl_type 
 * @return ErrorCode 
 */
DLLEXPORT ErrorCode UpdateProtocolToSubidlMap(ProtocolType protocol_type,
                     const std::string &sub_idl_type);

/**
 * @brief Communication DeInit function
 * 
 * @return 
 */
DLLEXPORT void DeInit();

/**
 * @brief Get the communication Version
 * 
 * @param major 
 * @param minor 
 * @param patch 
 * @return str::string 
 */
DLLEXPORT std::string GetVersion(int *major = nullptr,
                                 int *minor = nullptr,
                                 int *patch = nullptr);

/**
 * @brief Get the Soname Version
 * 
 * @param major 
 * @param minor 
 * @param patch 
 * @return std::string 
 */
DLLEXPORT std::string GetSonameVersion(int *major = nullptr,
                                        int *minor = nullptr,
                                        int *patch = nullptr);
/**
 * @brief register ext participant info and return participant id 
 * 
 * @param ext_info 
 * @return int : participant id 
 */
DLLEXPORT int RegisterParticipantExtInfo(const ParticipantInfoExt &ext_info);

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_TYPES_H_
