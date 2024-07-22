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

#ifndef COMMUNICATION_DISCOVERY_MANAGER_H_
#define COMMUNICATION_DISCOVERY_MANAGER_H_
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include <map>
#include <chrono>
#include "discovery_types.h"
#include "communication/common/qos_policy.h"
#include "communication/qos/qos_adaptor.h"

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

class ConnectionManager;
class GlobalView;
class ComRawSocket;
class DiscoveryMsgHandler;
class EndpointSnapshot;
class ComPersistentThread;
struct EndpointInfo;
struct EndpointReaderInfo;
struct EndpointWriterInfo;
class DLLEXPORT DiscoveryManager {
 public:
  enum class InitState {
    INIT_STATE_DISABLE = -2,
    INIT_STATE_ERROR = -1,
    INIT_STATE_NONE = 0,
    INIT_STATE_BEGIN,
    INIT_STATE_RECVING,
    INIT_STATE_DONE,
    INIT_STATE_DEINITING,
    INIT_STATE_DEINITED
  };

 public:
  static DiscoveryManager *instance();

  bool init();

  bool de_init();

  /*
   * Regist a new reader to DiscoveryManager
   * @return:uuid of the new reader,you must hold it cause un_regist need it.
   */
  std::string regist_reader(const std::string& topic,
                            ProtocolType protocol_type,
                            const CommAttr& comm_attr,
                            std::shared_ptr<QosAdaptor>& qos_ptr,
                            ConnChangeCallback conn_change_callback,
                            int domain_id);

  /*
  *
  * un_regist_reader use uuid
  */
  void un_regist_reader(const std::string& uuid);

  std::map<std::string, EndpointReaderInfo> get_all_reader_infos();

  /*
  * Regist a new writer to DiscoveryManager
  * @return:uuid of the new writer,you must hold it cause un_regist need it.
  */
  std::string regist_writer(const std::string& topic,
                            ProtocolType protocol_type,
                            const CommAttr& comm_attr,
                            std::shared_ptr<QosAdaptor>& qos_ptr,
                            ConnAddCallback conn_add_callback,
                            ConnRemoveCallback conn_remove_callback,
                            int domain_id);
  /*
  *
  * un_regist_writer use uuid
  */
  void un_regist_writer(const std::string& uuid);

  std::map<std::string, EndpointWriterInfo> get_all_writer_infos();

  std::string regist_entity(EntityType entity_type, const std::string& topic,
                            int domain_id, ProtocolType proto_type);

  void un_regist_entity(const std::string& uuid);

  std::map<std::string, std::shared_ptr<EntityInfo>> get_all_entitys();

  bool writer_is_hybrid_in_my_endpoint(const std::string& writer_id);

  ~DiscoveryManager() = default;

 private:
  DiscoveryManager() = default;

  void start();

  void stop();

  void recv_endpoint_info_callback(std::shared_ptr<EndpointInfo> endpoint_info);

  void OnMsgRecvCallback(char* buf, int len);

  bool SendFullRequest(std::string con_info = "");

  bool SendHeartBeat();

  bool SendEndpointChangeMsg();

  void MakeSnapShort();

  void UpdateProtosAndParticipants(ProtocolType protocol_type,
                                   const CommAttr& comm_attr,
                                   std::vector<ProtocolType>& dynamic_protols,
                                   std::vector<int>& static_participant_ids);

 private:
  InitState init_state_{InitState::INIT_STATE_NONE};
  // bool inited_{false};
  // std::shared_ptr<AutoDiscovery> auto_discovery_;
  std::shared_ptr<ComRawSocket> com_raw_socket_;
  std::shared_ptr<DiscoveryMsgHandler> dis_msg_handler_;
  std::shared_ptr<GlobalView> global_view_;
  std::shared_ptr<ConnectionManager> conn_manager_;
  std::mutex manager_mutex_;

  std::atomic_bool _expired{true};  // timer stopped status
  std::atomic_bool _try_to_expire{false};  // timer is in stop process

  bool endpoint_chaned_{false};
  std::chrono::steady_clock::time_point endpoint_change_time_;

  std::shared_ptr<EndpointSnapshot> endpoint_snapshot_{nullptr};

  std::shared_ptr<ComPersistentThread> com_thread_{nullptr};

  uint32_t my_net_mask_int_{0U};
  uint32_t my_net_int_{0U};
  bool cross_different_subnets_{true};

  std::map<std::string, bool> same_nets;
};


}  // namespace communication
}  // namespace hobot
#endif
