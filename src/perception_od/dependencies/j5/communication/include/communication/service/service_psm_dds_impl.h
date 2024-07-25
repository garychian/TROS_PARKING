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

#ifndef COMMUNICATION_SERVICE_SERVICE_PSM_DDS_IMPL_H_
#define COMMUNICATION_SERVICE_SERVICE_PSM_DDS_IMPL_H_

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
#include "communication/participant/dds/dds_reader.h"
#include "communication/participant/dds/dds_writer.h"

namespace hobot {
namespace communication {

template<typename SerializerRequest, typename SerializerResponse>
class ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>  :
      public ServiceBaseImpl {
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
  bool is_dynamic_{false};
  int service_dup_check_ms_{0};
  ProtocolType proto_type_{};
  std::string entity_uuid_{};
  std::string request_topic_{};  // service_name + some postfix
  std::string response_topic_{};  // service_name + some postfix
  std::string service_file_name_{};
  std::unique_ptr<
    Reader<typename SerializerRequest::DDSCSSupportType>> dds_reader_{nullptr};
  std::unique_ptr<Writer<
        typename SerializerResponse::DDSCSSupportType>> dds_writer_{nullptr};
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

template<typename SerializerRequest, typename SerializerResponse>
ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      ServiceImpl(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    const ServiceCallback<SerializerRequest, SerializerResponse>
    &service_callback,
    ProtocolType type, bool blocked_service_callback)
    : service_callback_(service_callback) {
  COHLOGD("Create service_psm_dds_impl");
  // get service uuid
  service_id_ = ToolsServer::Instance()->AddTopic(comm_attr,
                                                  service_name, domain_id,
                                                  TOPIC_TYPE::SERVICE);
  COHLOGD("Create a new Service, service_name={}, protocol={}, service_id={},"
          "domain_id={}", service_name, type, service_id_, domain_id);

  tools_service_name_ = ToolsServer::Instance()->GetToolsServiceName();
  service_name_ = service_name;
  proto_type_ = type;
  domain_id_ = domain_id;
  if (comm_attr.participant_attrs_.empty()) {
    is_dynamic_ = true;
  }
  service_dup_check_ms_ = comm_attr.service_dup_check_ms_;
  service_file_name_ = service_name + "_" +
      std::to_string(domain_id) + "_" + std::to_string(type);

  if (tools_service_name_ != service_name_) {
    tool_server_ = ToolsServer::Instance();
  }

  // input comm_attr is for reader
  CommAttr comm_attr_r = GenerateCommAttr(comm_attr, GenType::SERVICE);
  TopicAssemblier::ExpandTopic(service_name, request_topic_, response_topic_,
                               type);
  if (type == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
  auto psm_dds_read_cb =
      [this, service_callback, service_name, blocked_service_callback](
          const std::shared_ptr<
            typename SerializerRequest::DDSCSSupportType::MsgType> &req,
          const spMessageInfo &msg_info) {
        ToolsServer::Instance()->UpdateCount(service_id_,
                                             TOPIC_COUNT_TYPE::RECV);
        auto resp = std::make_shared<typename SerializerResponse::MsgType>();
        auto req_raw_msg = req->comm_user_define_msg();
        auto req_msg = std::make_shared<
          typename SerializerRequest::MsgType>(*req_raw_msg);

        if (!blocked_service_callback) {
          // ThreadPoolAT::Instance()->AddTask(task);
          HandleRequest(req_msg, resp, msg_info);
        } else {
          auto task = std::bind(
              &ServiceImpl<SerializerRequest,
                           SerializerResponse>::HandleRequest,
              this, req_msg, resp, msg_info);
          Executor::GetThreadPoolExecutor()->AddLongRunningTask(task);
        }
  };
  dds_reader_.reset(CreateDDSReader<
    typename SerializerRequest::DDSCSSupportType>(comm_attr, request_topic_,
                                                  domain_id,
                                                  psm_dds_read_cb, type));
  if (!dds_reader_) {
    COHLOGE("service CreateDDSReader failed, return nullptr");
  }

  dds_writer_.reset(CreateDDSWriter<
    typename SerializerResponse::DDSCSSupportType>(comm_attr, response_topic_,
                                                   domain_id, type));
  if (!dds_writer_) {
    COHLOGE("service CreateDDSWriter failed, return nullptr");
  }
#endif
  } else {
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
    if (proto_type_ == PROTOCOL_PCIE) {
      reader_.reset(new Reader<SerializerRequest>(comm_attr, request_topic_,
                                                  domain_id, read_cb, type));
    } else {
      reader_.reset(new Reader<SerializerRequest>(comm_attr_r, request_topic_,
                                                  domain_id, read_cb, type));
    }
    writer_.reset(new Writer<SerializerResponse>(comm_attr, response_topic_,
                                                 domain_id, type));
  }

  discovery_manager_ = DiscoveryManager::instance();
}

template<typename SerializerRequest, typename SerializerResponse>
ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      ~ServiceImpl() {
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
  if (proto_type_ == PROTOCOL_DDS) {
    dds_reader_.reset();
    dds_writer_.reset();
  } else {
    reader_.reset();
    writer_.reset();
  }

  if (discovery_manager_ != nullptr && !entity_uuid_.empty()) {
    discovery_manager_->un_regist_entity(entity_uuid_);
  }
}

template<typename SerializerRequest, typename SerializerResponse>
bool ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
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

template<typename SerializerRequest, typename SerializerResponse>
ErrorCode ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      Init() {
  // COHLOGD("Service Init, service_name={}, protocol={}, service_id={}",
  //                                   service_name_, proto_type_, service_id_);

  if (is_dynamic_ && (proto_type_ == PROTOCOL_DDS ||
                      proto_type_ == PROTOCOL_ZMQ_EPGM ||
                      proto_type_ == PROTOCOL_ZMQ_TCP ||
                      proto_type_ == PROTOCOL_HYBRID)) {
    if (!HandleServiceDuplicates()) {
      return COMM_CODE_INIT_FAILED;
    }
  }

  ErrorCode ret{COMM_CODE_OK};
  if (proto_type_ == PROTOCOL_DDS) {
    if (!dds_reader_ || !dds_writer_) {
      COHLOGE("writer or reader is nullptr!!!");
      return COMM_CODE_PARAM_IS_NULL;
    }
    ret = dds_reader_->Init();
    if (ret == COMM_CODE_OK) {
      ret = dds_writer_->Init();
    }
  } else {
    ret = reader_->Init();
    if (ret == COMM_CODE_OK) {
      ret = writer_->Init();
    }
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

template<typename SerializerRequest, typename SerializerResponse>
void ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      SetEventListener(
    const ConnEventCallback &event_callback) {
  COHLOGD("Service SetEventListener, service_name={},protocol={},service_id={}",
                                    service_name_, proto_type_, service_id_);

  auto conn_event_cb = [event_callback, this](
      const std::shared_ptr<LinkInfo> participants,
      EventType event) {
    ToolsServer::Instance()->AddEvent(
        service_id_, event, TOPIC_TYPE::SERVICE);
    if (event_callback != nullptr) {
      event_callback(participants, event);
    }
  };
  if (proto_type_ == PROTOCOL_DDS) {
    if (dds_reader_) {
      dds_reader_->SetEventListener(conn_event_cb);
    }
    if (dds_writer_) {
      dds_writer_->SetEventListener(conn_event_cb);
    }
  } else {
    reader_->SetEventListener(conn_event_cb);
    writer_->SetEventListener(conn_event_cb);
  }
}

template<typename SerializerRequest, typename SerializerResponse>
std::shared_ptr<Executor> ServiceImpl
    <SerializerRequest, SerializerResponse,
    typename std::enable_if<
      HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
      HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
        GetExecutor() {
  if (proto_type_ == PROTOCOL_DDS) {
    if (dds_reader_) {
      return dds_reader_->GetExecutor();
    } else {
      COHLOGE("GetExecutor fail, dds_reader_ is nullptr");
      return nullptr;
    }
  } else {
    if (reader_) {
      return reader_->GetExecutor();
    } else {
      COHLOGE("GetExecutor fail, reader_ is nullptr");
      return nullptr;
    }
  }
}

template<typename SerializerRequest, typename SerializerResponse>
void ServiceImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      HandleRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> msg_req,
    std::shared_ptr<typename SerializerResponse::MsgType> &msg_resp,
    const spMessageInfo &msg_info) {
  COHLOGD("Topic:{} read seq_id: {}", service_name_, msg_info->GetSeqId());
  // 1. handle msg from reader with service_callback
  service_callback_(msg_req, msg_resp);

  // 2. send response msg from service_callback with writer
  if (dds_writer_ || writer_) {
    // TODO(maosong): if we start client, then start service, sometimes
    // ZMQ_EVENT_HANDSHAKE_SUCCEEDED event in serice comes later than
    // client on Linux platform. We have to make sure writer
    // is ready before data transform on first time
    static bool first_ready_flag{false};
    if (!first_ready_flag) {
      std::chrono::milliseconds time_out{250};
      std::chrono::milliseconds step_duration{10};
      while (time_out.count() > 0) {
        bool ret{false};
        if (proto_type_ == PROTOCOL_DDS) {
          ret = dds_writer_->IsReady();
        } else {
          ret = writer_->IsReady();
        }
        if (!ret) {
          COHLOGD("wait for dds_writer_ ready...");
          std::this_thread::sleep_for(step_duration);
          time_out -= step_duration;
        } else {
          first_ready_flag = true;
          break;
        }
      }
    }

    spMessageInfo write_msg_info(msg_info);
    write_msg_info->SetWriterId(service_id_);
    if (proto_type_ == PROTOCOL_DDS) {
      auto dds_ext_info = std::make_shared<
        typename SerializerResponse::DDSCSSupportType::MsgType>();
      dds_ext_info->set_comm_user_define_msg(msg_resp.get(), false);
      dds_writer_->Write(dds_ext_info, write_msg_info);
    } else {
      writer_->Write(msg_resp, write_msg_info);
    }

    COHLOGD("write seq_id: {}", write_msg_info->GetSeqId());
    ToolsServer::Instance()->UpdateCount(service_id_, TOPIC_COUNT_TYPE::SEND);
  }
}
}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SERVICE_SERVICE_IMPL_H_
// #endif // PSMDDS
