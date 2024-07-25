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

// This is common tools server definition.


#ifndef COMMUNICATION_COMMON_TOOLS_SERVER_H_
#define COMMUNICATION_COMMON_TOOLS_SERVER_H_

#include "communication/common/types.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <utility>

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

class ToolsServerImpl;

enum class TOPIC_TYPE {
  PUB = 0,
  SUB = 1,
  SERVICE = 2,
  CLIENT = 3,
  ACTION_SERVICE = 4,
  ACTION_CLIENT = 5
};

enum class TOPIC_COUNT_TYPE {
  SEND = 0,
  RECV = 1
};

enum class TopicStatus {
  TOPIC_OPEN = 0,
  TOPIC_CLOSE = 1
};

class DLLEXPORT ToolsServer {
 public:
  static ToolsServer* Instance();
  void CleanUp();
  ~ToolsServer();
  uint64_t AddTopic(const CommAttr &comm_attr,
                    const std::string &topic,
                    int domain_id,
                    TOPIC_TYPE type,
                    const std::vector<std::string> &action_client_server = {});
  void AddEvent(uint64_t id, EventType event_type,
                TOPIC_TYPE type);
  void RemoveTopic(uint64_t id);

  void UpdateCount(uint64_t id, TOPIC_COUNT_TYPE type);
  std::string GetToolsServiceName() const;

  TopicStatus GetTopicStatus(uint64_t id);

  int AddPluginInfo(ProtocolType protocol_type,
                    std::string protocol_name,
                    std::string plugin_name,
                    std::string vendor,
                    std::string ext_info,
                    int plugin_version,
                    int plugin_interface_version);
  int RemovePluginInfo(ProtocolType protocol_type);
  int AddPluginDataReaderInfo(ProtocolType protocol_type,
                                    void * handle,
                                    int participant_id,
                                    std::string link_info,
                                    std::string config_str);
  int RemovePluginDataReaderInfo(ProtocolType protocol_type,
                                       void * handle);
  int AddPluginDataWriterInfo(ProtocolType protocol_type,
                                    void * handle,
                                    int participant_id,
                                    std::string link_info,
                                    std::string config_str);
  int RemovePluginDataWriterInfo(ProtocolType protocol_type,
                                       void * handle);

 private:
  ToolsServer();
 private:
  std::shared_ptr<ToolsServerImpl> impl_{nullptr};
  bool tool_switch_{true};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_TOOLS_SERVER_H_
