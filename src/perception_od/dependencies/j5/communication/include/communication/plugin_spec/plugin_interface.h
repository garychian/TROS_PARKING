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

#ifndef COMMUNICATION_PLUGIN_SPEC_PLUGIN_INTERFACE_H_
#define COMMUNICATION_PLUGIN_SPEC_PLUGIN_INTERFACE_H_

#include "message/message.h"
#include "communication/service/message_info.h"
#include "communication/common/qos_policy_common.h"

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <set>
#include <map>

#ifndef DLLEXPORT
# if defined(_MSC_VER)
#   define DLLEXPORT __declspec(dllexport)
# else
#   define DLLEXPORT
# endif
#endif

// This is plugin interface version, if this file is update later,
// This version need be updated also.
#define COM_PLUGIN_INTF_VER    1

namespace hobot {
namespace communication {

using hobot::message::DataRef;
using hobot::message::DataRef;
using hobot::message::Message;
using hobot::message::SerializerPolicy;
using hobot::message::spMsg;
using hobot::message::spSerData;

struct PluginRetInfo {
  int plugin_ver;            // user defined version
  int plugin_interface_ver;  // Must return "COM_PLUGIN_INTF_VER"
  int dynamic_support;       // Whether support dynamic creation
  std::string vendor;        // Vendor name
  std::map<std::string, std::string> property_ext;  // Extend property
  PluginRetInfo()
      : plugin_ver{0},
        plugin_interface_ver{0},
        dynamic_support{0},
        vendor{},
        property_ext{} {}
};

// Below are moved from data_reader.h and data_writer.h,
// since all these are related to plugin interface
struct DataReaderWriterInfo {
  int participant_id_;
  int protocol_type_;
  std::string protocol_;
  std::string link_info_;
  std::string link_type_;
  std::string config_options_str;
  void * config_options;
  QosParameterConfig hb_qos_parameter;
  std::string qos_parameter_config_str;
  void * qos_parameter_config;
  DataReaderWriterInfo()
      : participant_id_{0},
        protocol_type_{0},
        protocol_{},
        link_info_{},
        link_type_{},
        config_options_str{},
        config_options{nullptr},
        hb_qos_parameter{},
        qos_parameter_config_str{},
        qos_parameter_config{nullptr} {}
};

enum DataReaderEventType {
  DATA_READER_EVENT_CONNECTED = 0,
  DATA_READER_EVENT_CONNECT_FAILED,
  DATA_READER_EVENT_DISCONNECTED,
  DATA_READER_EVENT_STATUS_MAX
};

struct DataReaderLinkInfo {
  int id;
  std::string protocol;
  std::string link_info;
};

struct DataReaderExtraInfo {
  explicit DataReaderExtraInfo(bool skip_flag = false)
      : skip_not_found_related_topic_error(skip_flag) {}
  bool skip_not_found_related_topic_error{false};
};

using DataReaderCallback =
std::function<void(const std::string &topic_name,
                   spSerData data,
                   const spMessageInfo &msg_info,
                   const std::shared_ptr<DataReaderExtraInfo> extra_info)>;

using DataReaderEventCallback = std::function<void(
    const std::shared_ptr<DataReaderLinkInfo> participants,
    void* key,
    DataReaderEventType event)>;

enum DataWriterEventType {
  DATA_WRITER_EVENT_CONNECTED = 0,
  DATA_WRITER_EVENT_CONNECT_FAILED,
  DATA_WRITER_EVENT_DISCONNECTED,
  DATA_WRITER_EVENT_STATUS_MAX
};

struct DataWriterLinkInfo {
  int id;
  std::string protocol;
  std::string link_info;
};

using DataWriterEventCallback = std::function<void(
    const std::shared_ptr<DataWriterLinkInfo> participants,
    void* key,
    DataWriterEventType event)>;

using DataWriterResultCallback =
std::function<void(const spMsg &msg, int ret_code)>;

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT int
PlugInit(int protocol_type,
         const std::string &protocol_name,
         std::shared_ptr<hobot::communication::PluginRetInfo> &ret_info);

DLLEXPORT int
PlugGenerateAutoDiscoveryLinkInfo(int protocol_type,
                const std::string &protocol_name,
                std::shared_ptr<std::string> &link_info);

DLLEXPORT int
PlugGetQosCapability(int protocol_type,
                const std::string &protocol_name,
                QosCapability &qos_capability);

DLLEXPORT int
PlugUpdatePropertyExt(int protocol_type,
                                      const std::string &protocol_name,
                                      const std::string & key,
                                      const std::string & value);

DLLEXPORT int
PlugWriterEnable(void * handle);

DLLEXPORT bool
PlugWriterIsReady(void * handle, const std::string &topic);

DLLEXPORT int
PlugCreateDataWriter(const hobot::communication::DataReaderWriterInfo &info,
                void* &handle);

DLLEXPORT int
PlugDestroyDataWriter(void* handle);

DLLEXPORT int
PlugWriterAsyncWrite(void * handle,
                const std::string &topic_name,
                const hobot::message::spSerData &data,
                const hobot::message::spMsg &msg,
                const hobot::communication::spMessageInfo &msg_info = nullptr);

DLLEXPORT int
PlugWriterAddListener(void * handle,
                const hobot::communication::DataWriterResultCallback &callback);

DLLEXPORT int
PlugWriterAddEventListener(void * handle,
                const hobot::communication::DataWriterEventCallback &event_cb,
                void* key);

DLLEXPORT int
PlugWriterRemoveEventListener(void * handle, void* key);

DLLEXPORT int
PlugReaderEnable(void * handle);

DLLEXPORT bool
PlugReaderIsReady(void * handle, const std::string &topic);

DLLEXPORT bool
PlugDiscoveryBySelf();

DLLEXPORT void
PluginSetUseSchedulerGroup(bool use_scheduler_group);

DLLEXPORT void
PluginSetDefEth(const std::string &def_eth);

DLLEXPORT int
PlugCreateDataReader(const hobot::communication::DataReaderWriterInfo &info,
                void * &handle);

DLLEXPORT int
PlugDestroyDataReader(void* handle);

DLLEXPORT int
PlugReaderAddListener(void * handle,
                const hobot::communication::DataReaderCallback &callback);

DLLEXPORT int
PlugReaderRemoveListener(void * handle);

DLLEXPORT int
PlugReaderAddEventListener(void * handle,
                const hobot::communication::DataReaderEventCallback &event_cb,
                void* key);

DLLEXPORT int
PlugReaderRemoveEventListener(void * handle,
                void* key);

DLLEXPORT int
PlugReaderAddListenTopic(void * handle,
                const std::string &topic);

#ifdef __cplusplus
}
#endif

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PLUGIN_SPEC_PLUGIN_INTERFACE_H_
