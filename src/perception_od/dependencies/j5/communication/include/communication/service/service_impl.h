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

// This is service implementation.

#ifndef COMMUNICATION_SERVICE_SERVICE_IMPL_H_
#define COMMUNICATION_SERVICE_SERVICE_IMPL_H_

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "communication/service/service_base_impl.h"
#include "communication/utils/thread_pool.h"
#include "communication/discovery/discovery_manager.h"
#include "communication/common/types.h"
#ifdef SUPPORT_DDS
#include "communication/participant/dds/dds_reader.h"
#include "communication/participant/dds/dds_writer.h"
#include "communication/participant/dds/dds_ext_info.h"
#endif

namespace hobot {
namespace communication {

template <typename SerializerRequest, typename SerializerResponse,
          typename Enable = void>
class ServiceImpl : public ServiceBaseImpl {
 public:
  ServiceImpl(const CommAttr &comm_attr, const std::string &service_name,
              int domain_id,
              const ServiceCallback<SerializerRequest, SerializerResponse>
              &service_callback,
              ProtocolType type, bool blocked_service_callback = false);

  ~ServiceImpl();

  ErrorCode Init();

  void SetEventListener(const ConnEventCallback &event_callback);
  std::shared_ptr<Executor> GetExecutor();

 private:
  uint64_t service_id_{0u};
  // avoid cyclic dependencies
  std::string tools_service_name_{};
  std::string service_name_{};
  int domain_id_{0};
  bool is_crossnet_{false};
  int service_dup_check_ms_{0};
  ProtocolType proto_type_{};
  std::string entity_uuid_{};
  std::string request_topic_{};  // service_name + some postfix
  std::string response_topic_{};  // service_name + some postfix
  std::string service_file_name_{};
  std::unique_ptr<Reader<SerializerRequest>> reader_{nullptr};
  std::unique_ptr<Writer<SerializerResponse>> writer_{nullptr};
  ToolsServer* tool_server_{nullptr};
  DiscoveryManager* discovery_manager_{nullptr};
  LockService* lock_service_{nullptr};

  ServiceCallback<SerializerRequest, SerializerResponse> service_callback_;
  void HandleRequest(
      const std::shared_ptr<typename SerializerRequest::MsgType> msg_req,
      std::shared_ptr<typename SerializerResponse::MsgType> &msg_resp,
      const spMessageInfo &msg_info);

  bool HandleServiceDuplicates();
};

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ServiceImpl<SerializerRequest, SerializerResponse,
            Enable>::ServiceImpl(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    const ServiceCallback<SerializerRequest, SerializerResponse>
    &service_callback,
    ProtocolType type, bool blocked_service_callback)
    : service_callback_(service_callback) {
  // get service uuid
  service_id_ = ToolsServer::Instance()->AddTopic(comm_attr,
                                                  service_name, domain_id,
                                                  TOPIC_TYPE::SERVICE);
  COHLOGD("Create a new Service, service_name={}, protocol={}, service_id={},"
          " domain_id={}", service_name, type, service_id_, domain_id);

  tools_service_name_ = ToolsServer::Instance()->GetToolsServiceName();
  service_name_ = service_name;
  proto_type_ = type;
  domain_id_ = domain_id;
  if (comm_attr.participant_attrs_.empty() &&
      ProtocolChecker::IsCrossNet(proto_type_)) {
    is_crossnet_ = true;
  }
  if (ProtocolChecker::IsHybridAndCrossNet(proto_type_)) {
    is_crossnet_ = true;
  }
  if (ProtocolChecker::IsComposite(proto_type_) &&
      CompositeOptionChecker::HasCrossNetDynamicProtocol(
          comm_attr.composite_options)) {
    is_crossnet_ = true;
  }
  service_dup_check_ms_ = comm_attr.service_dup_check_ms_;
  service_file_name_ = service_name + "_" +
      std::to_string(domain_id) + "_" + std::to_string(type);

  if (tools_service_name_ != service_name_) {
    tool_server_ = ToolsServer::Instance();
  }
  auto read_cb =
      [this, service_callback, service_name, blocked_service_callback](
          const std::shared_ptr<typename SerializerRequest::MsgType> &req,
          const spMessageInfo &msg_info) {
        ToolsServer::Instance()->UpdateCount(service_id_,
                                             TOPIC_COUNT_TYPE::RECV);
        auto resp = std::make_shared<typename SerializerResponse::MsgType>();
        if (!blocked_service_callback) {
          // ThreadPoolAT::Instance()->AddTask(task);
          HandleRequest(req, resp, msg_info);
        } else {
          auto task = std::bind(
              &ServiceImpl<SerializerRequest,
                           SerializerResponse>::HandleRequest,
              this, req, resp, msg_info);
          Executor::GetThreadPoolExecutor()->AddLongRunningTask(task);
        }
      };

  // input comm_attr is for reader
  CommAttr comm_attr_r = GenerateCommAttr(comm_attr, GenType::SERVICE);
  TopicAssemblier::ExpandTopic(service_name, request_topic_, response_topic_,
                               type);
  if (proto_type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    reader_.reset(CreateDDSReader<SerializerRequest>(
        comm_attr_r, request_topic_, domain_id, read_cb, type));
    writer_.reset(CreateDDSWriter<SerializerResponse>(
        comm_attr, response_topic_, domain_id, type));
#endif
  } else {
    reader_.reset(new Reader<SerializerRequest>(comm_attr_r, request_topic_,
                                                domain_id, read_cb, type));
    writer_.reset(new Writer<SerializerResponse>(comm_attr, response_topic_,
                                                 domain_id, type));
  }

  discovery_manager_ = DiscoveryManager::instance();
}  // namespace communication

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ServiceImpl<SerializerRequest, SerializerResponse,
            Enable>::~ServiceImpl() {
  COHLOGD("Destroy a Service, service_name={}, protocol={}, service_id={}",
                                    service_name_, proto_type_, service_id_);
  RemoveCommAttr();
  if (tools_service_name_ != service_name_) {
    if (tool_server_ != nullptr) {
      tool_server_->RemoveTopic(service_id_);
    }
    if (lock_service_ != nullptr) {
      lock_service_->UnLockService(service_file_name_, proto_type_);
    }
  }

  reader_.reset();
  writer_.reset();
  if (discovery_manager_ != nullptr && !entity_uuid_.empty()) {
    discovery_manager_->un_regist_entity(entity_uuid_);
  }
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
bool ServiceImpl<SerializerRequest, SerializerResponse, Enable>::
                                                HandleServiceDuplicates() {
  if (tools_service_name_ == service_name_) {
    return true;
  }
  auto get_same_service = [this]()->std::vector<std::shared_ptr<EntityInfo>> {
    std::vector<std::shared_ptr<EntityInfo>> same_entitys;
    auto all_entitys = DiscoveryManager::instance()->get_all_entitys();
    for (auto entity_info : all_entitys) {
      if (entity_info.second->uuid == entity_uuid_) {
        continue;
      }

      if (entity_info.second->entity_type != EntityType::ENTITY_SERVICE) {
        continue;
      }

      if (entity_info.second->domain_id == domain_id_ &&
          entity_info.second->topic == service_name_ &&
          entity_info.second->protocol_type == proto_type_) {
            same_entitys.push_back(entity_info.second);
      }
    }
    return same_entitys;
  };

  if (!get_same_service().empty()) {
    COHLOGE("Already have same service, return");
    return false;
  }
  entity_uuid_ = DiscoveryManager::instance()->regist_entity(
      EntityType::ENTITY_SERVICE, service_name_, domain_id_, proto_type_);

  if (service_dup_check_ms_ <= 0) {
    return true;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(
                                                  service_dup_check_ms_));
  auto same_service_entitys = get_same_service();
  if (!same_service_entitys.empty()) {
    for (auto same_service_entity : same_service_entitys) {
      std::size_t my_hash = std::hash<std::string>{}(entity_uuid_);
      std::size_t  other_hash = std::hash<std::string>{}(
                                        same_service_entity->uuid);
      if (my_hash < other_hash) {
        COHLOGE("Already have same service, return");
        return false;
      }
    }
  }
  return true;
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ErrorCode ServiceImpl<SerializerRequest, SerializerResponse,
                      Enable>::Init() {
  // COHLOGD("Service Init, service_name={}, protocol={}, service_id={}",
  //                                   service_name_, proto_type_, service_id_);

  if (reader_ == nullptr || writer_ == nullptr) {
    return COMM_CODE_INIT_FAILED;
  }

  if (IsDiscoveryEnable() && is_crossnet_) {
    if (!HandleServiceDuplicates()) {
      return COMM_CODE_INIT_FAILED;
    }
  }

  auto ret = reader_->Init();
  if (ret == COMM_CODE_OK) {
    ret = writer_->Init();
  }
  if (tools_service_name_ != service_name_) {
    lock_service_ = LockService::Instance();
    if (!lock_service_->GetServiceInitStatus(service_file_name_, proto_type_)) {
      COHLOGE("GetServiceInitStatus fail");
      return COMM_CODE_INIT_FAILED;
    }
  }
  return ret;
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
void ServiceImpl<SerializerRequest, SerializerResponse,
                 Enable>::SetEventListener(
    const ConnEventCallback &event_callback) {
  COHLOGD("Service SetEventListener, service_name={},protocol={},service_id={}",
                                    service_name_, proto_type_, service_id_);
  if (reader_ == nullptr || writer_ == nullptr) {
    return;
  }
  auto conn_event_cb = [event_callback, this](
      const std::shared_ptr<LinkInfo> participants,
      EventType event) {
    ToolsServer::Instance()->AddEvent(
        service_id_, event, TOPIC_TYPE::SERVICE);
    if (event_callback != nullptr) {
      event_callback(participants, event);
    }
  };
  reader_->SetEventListener(conn_event_cb);
  writer_->SetEventListener(conn_event_cb);
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
std::shared_ptr<Executor> ServiceImpl
    <SerializerRequest, SerializerResponse, Enable>::GetExecutor() {
  if (reader_) {
    return reader_->GetExecutor();
  } else {
    COHLOGE("GetExecutor fail, reader_ is nullptr");
    return nullptr;
  }
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
void ServiceImpl<SerializerRequest, SerializerResponse,
                 Enable>::HandleRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> msg_req,
    std::shared_ptr<typename SerializerResponse::MsgType> &msg_resp,
    const spMessageInfo &msg_info) {
  COHLOGD("Topic:{} read seq_id: {}", service_name_, msg_info->GetSeqId());
  // 1. handle msg from reader with service_callback
  service_callback_(msg_req, msg_resp);

  // 2. send response msg from service_callback with writer
  if (writer_) {
    // TODO(maosong): if we start client, then start service, sometimes
    // ZMQ_EVENT_HANDSHAKE_SUCCEEDED event in serice comes later than
    // client on Linux platform. We have to make sure writer
    // is ready before data transform on first time
    static bool first_ready_flag{false};
    if (!first_ready_flag) {
      std::chrono::milliseconds time_out{250};
      std::chrono::milliseconds step_duration{10};
      while (time_out.count() > 0) {
        if (!writer_->IsReady()) {
          COHLOGD("wait for writer_ ready...");
          std::this_thread::sleep_for(step_duration);
          time_out -= step_duration;
        } else {
          first_ready_flag = true;
          break;
        }
      }
    }

    if (proto_type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
      auto ext_info = std::dynamic_pointer_cast<DDSExtInfo>(msg_info);
      if (ext_info == nullptr) {
        COHLOGE("dds recvive not compatible !");
        return;
      }
      if (ext_info->GetOwnSeqId().low != 0u ||
          ext_info->GetOwnSeqId().high != 0u) {
        ext_info->SetRelativeSeqId(ext_info->GetOwnSeqId());
        writer_->Write(msg_resp, ext_info);
      } else {
        msg_info->SetWriterId(service_id_);
        writer_->Write(msg_resp, msg_info);
      }
#endif
    } else {
      spMessageInfo write_msg_info{msg_info};
      write_msg_info->SetWriterId(service_id_);
      writer_->Write(msg_resp, write_msg_info);
      COHLOGD("write seq_id: {}", write_msg_info->GetSeqId());
    }

    ToolsServer::Instance()->UpdateCount(service_id_, TOPIC_COUNT_TYPE::SEND);
  }
}
}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SERVICE_SERVICE_IMPL_H_
