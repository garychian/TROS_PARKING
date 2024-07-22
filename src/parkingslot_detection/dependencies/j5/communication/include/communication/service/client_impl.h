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

// This is client implementation.

#ifndef COMMUNICATION_SERVICE_CLIENT_IMPL_H_
#define COMMUNICATION_SERVICE_CLIENT_IMPL_H_

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "communication/discovery/discovery_manager.h"
#include "communication/service/service_base_impl.h"

#ifdef SUPPORT_DDS
#include "communication/participant/dds/dds_ext_info.h"
#include "communication/participant/dds/dds_reader.h"
#include "communication/participant/dds/dds_writer.h"
#endif
using hobot::message::SerializerPolicy;
namespace hobot {
namespace communication {

// declaration
template <typename SerializerRequest, typename SerializerResponse,
          typename Enable = void>
class ClientImpl : public ServiceBaseImpl {
  using RespValType =
  std::pair<ErrorCode,
            std::shared_ptr<typename SerializerResponse::MsgType>>;
  using PromiseType = std::shared_ptr<std::promise<RespValType>>;
  using FutureType = std::shared_future<RespValType>;
  using RespCallBackType = std::function<void(
      const std::shared_ptr<typename SerializerResponse::MsgType> &)>;
  using PendingRequestType =
  std::tuple<PromiseType, FutureType, RespCallBackType>;

 public:
  ClientImpl(const CommAttr &comm_attr, const std::string &service_name,
             int domain_id, ProtocolType type);
  ~ClientImpl();
  ErrorCode Init();

  void SetEventListener(const ConnEventCallback &event_callback);

  ErrorCode SendRequest(
      const std::shared_ptr<typename SerializerRequest::MsgType> &request,
      std::shared_ptr<typename SerializerResponse::MsgType> &response,
      int timeout);

  std::shared_future<std::pair<
      ErrorCode, std::shared_ptr<typename SerializerResponse::MsgType>>>
  AsyncSendRequest(
      const std::shared_ptr<typename SerializerRequest::MsgType> &request,
      const ClientCallback<SerializerResponse> &client_callback,
      ErrorCode *code = nullptr,
      uint64_t *seq_id_ptr = nullptr);

  bool ServiceIsReady() const;

  bool WaitForServiceNanoseconds(std::chrono::nanoseconds time_out);

 private:
  uint64_t client_id_{0u};
  uint64_t seq_id_{0u};
  std::mutex request_mtx_;
  std::unordered_map<uint64_t, PendingRequestType> pending_requests_;

  // avoid cyclic dependencies
  std::string tools_service_name_;
  std::string service_name_;
  int domain_id_;
  ProtocolType proto_type_;
  std::string entity_uuid_;
  std::string request_topic_;   // service_name + some postfix
  std::string response_topic_;  // service_name + some postfix
  std::shared_ptr<Writer<SerializerRequest>> writer_{nullptr};
  std::shared_ptr<Reader<SerializerResponse>> reader_{nullptr};
  ToolsServer* tool_server_{nullptr};
  DiscoveryManager* discovery_manager_{nullptr};

  void HandleResponse(
      const std::shared_ptr<typename SerializerResponse::MsgType> &msg,
      const spMessageInfo &msg_info);
};

// definition
template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ClientImpl<SerializerRequest, SerializerResponse, Enable>::ClientImpl(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    ProtocolType type) {
  // get client uuid
  client_id_ = ToolsServer::Instance()->AddTopic(comm_attr,
                                                 service_name,
                                                 domain_id,
                                                 TOPIC_TYPE::CLIENT);
  COHLOGD("Create a new ClientImpl, service_name={}, protocol={}, "
          "client_id={}, domain_id={}",
          service_name, type, client_id_, domain_id);
  tools_service_name_ = ToolsServer::Instance()->GetToolsServiceName();
  service_name_ = service_name;
  proto_type_ = type;
  domain_id_ = domain_id;
  if (tools_service_name_ != service_name_) {
    tool_server_ = ToolsServer::Instance();
  }
  TopicAssemblier::ExpandTopic(service_name, request_topic_, response_topic_,
                               type);
  // input comm_attr is for writer
  CommAttr comm_attr_w = GenerateCommAttr(comm_attr, GenType::CLIENT);

  if (proto_type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    COHLOGD("Create a new Client, request_topic_={}", request_topic_);
    writer_.reset(CreateDDSWriter<SerializerRequest>(comm_attr, request_topic_,
                                                     domain_id, type));
#endif
  } else {
    writer_.reset(new Writer<SerializerRequest>(comm_attr_w, request_topic_,
                                                domain_id, type));
  }

  auto read_cb = std::bind(
      &ClientImpl<SerializerRequest, SerializerResponse>::HandleResponse, this,
      std::placeholders::_1, std::placeholders::_2);

  if (proto_type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    COHLOGD("Create a new Client, response_topic_={}", response_topic_);
    reader_.reset(CreateDDSReader<SerializerResponse>(
        comm_attr, response_topic_, domain_id, read_cb, type));
#endif
  } else {
    reader_.reset(new Reader<SerializerResponse>(comm_attr, response_topic_,
                                                 domain_id, read_cb, type));
  }
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ClientImpl<SerializerRequest, SerializerResponse, Enable>::~ClientImpl() {
  COHLOGD("Destroy a ClientImpl, service_name={}, protocol={}, client_id={}",
                                    service_name_, proto_type_, client_id_);
  if (tools_service_name_ != service_name_) {
    if (tool_server_ != nullptr) {
      tool_server_->RemoveTopic(client_id_);
    }
  }
  // pending_requests_.clear();
  reader_.reset();
  writer_.reset();
  if (discovery_manager_ != nullptr && !entity_uuid_.empty()) {
    discovery_manager_->un_regist_entity(entity_uuid_);
  }
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ErrorCode ClientImpl<SerializerRequest, SerializerResponse, Enable>::Init() {
  // COHLOGD("Client Init, service_name={}, protocol={}, client_id={}",
  //                                   service_name_, proto_type_, client_id_);
  if (reader_ == nullptr || writer_ == nullptr) {
    return COMM_CODE_INIT_FAILED;
  }
  discovery_manager_ = DiscoveryManager::instance();
  entity_uuid_ = discovery_manager_->regist_entity(
      EntityType::ENTITY_CLIENT, service_name_, domain_id_, proto_type_);
  auto ret = writer_->Init();
  if (ret == COMM_CODE_OK) {
    ret = reader_->Init();
  }
  if (ret != COMM_CODE_OK) {
    COHLOGE("ClientImpl init failed!")
  }
  return ret;
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
void ClientImpl<SerializerRequest, SerializerResponse,
                Enable>::SetEventListener(
    const ConnEventCallback &event_callback) {
  COHLOGD("Client SetEventListener, service_name={}, protocol={}, client_id={}",
                                    service_name_, proto_type_, client_id_);

  auto conn_event_cb = [event_callback, this](
      const std::shared_ptr<LinkInfo> participants,
      EventType event) {
    ToolsServer::Instance()->AddEvent(
        client_id_, event, TOPIC_TYPE::CLIENT);
    if (event_callback != nullptr) {
      event_callback(participants, event);
    }
  };
  reader_->SetEventListener(conn_event_cb);
  writer_->SetEventListener(conn_event_cb);
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
ErrorCode ClientImpl<SerializerRequest, SerializerResponse,
                     Enable>::SendRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> &request,
    std::shared_ptr<typename SerializerResponse::MsgType> &response,
    int timeout) {
  ErrorCode ret{COMM_CODE_CLIENT_SEND_FAILED};
  uint64_t seq_id{0u};
  FutureType fut{
      AsyncSendRequest(request, nullptr, &ret, &seq_id)};
  if (!fut.valid()) {
    return ret;
  }
  if (timeout >= 0) {
    COHLOGD("Topic:{} client wait for response with timeout:{}ms",
            service_name_, timeout)
    auto status = fut.wait_for(std::chrono::milliseconds(timeout));
    if (status == std::future_status::ready) {
      auto fut_val = fut.get();
      response = fut_val.second;
      ret = fut_val.first;
      return ret;
    } else {
      std::unique_lock<std::mutex> lg{request_mtx_};
      if (pending_requests_.count(seq_id) != 0u) {
        pending_requests_.erase(seq_id);
      }
      return COMM_CODE_CLIENT_TIMEOUT;
    }
  } else {
    COHLOGD("Topic:{} client wait for response without timeout setting",
            service_name_);
    auto fut_val = fut.get();
    response = fut_val.second;
    ret = fut_val.first;
    return ret;
  }
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
std::shared_future<
    std::pair<ErrorCode,
              std::shared_ptr<typename SerializerResponse::MsgType>>>
ClientImpl<SerializerRequest, SerializerResponse,
           Enable>::AsyncSendRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> &request,
    const ClientCallback<SerializerResponse> &client_callback,
    ErrorCode *code,
    uint64_t *seq_id_ptr) {
  uint64_t seq_id{0u};
  {
    std::unique_lock<std::mutex> lg{request_mtx_};
    seq_id = seq_id_++;
    if (seq_id_ptr != nullptr) {
      *seq_id_ptr = seq_id;
    }
  }

  PromiseType prom = std::make_shared<std::promise<RespValType>>();
  FutureType fut = FutureType(prom->get_future());
  RespCallBackType resp_cb{nullptr};
  if (client_callback) {
    resp_cb = std::bind(client_callback, COMM_CODE_OK, std::placeholders::_1);
  }
  PendingRequestType pending_request = std::make_tuple(prom, fut, resp_cb);

  if (proto_type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    auto dds_ext = std::make_shared<DDSExtInfo>(client_id_, seq_id, client_id_);
    // client begin write relative guid,DDSReader give function GetGuid();
    dds_ext->SetRelativeGuid(GetReaderGuid<SerializerResponse>(reader_));
    std::unique_lock<std::mutex> lg{request_mtx_};
    ErrorCode ret = writer_->Write(request, dds_ext);
    if (ret != COMM_CODE_OK) {
      if (code) {
        *code = ret;
      }
      COHLOGE("client write request failed!");
      return FutureType();
    }
    {
      COHLOGD("Own Seq Id high: {}, low: {}", dds_ext->GetOwnSeqId().high,
              dds_ext->GetOwnSeqId().low);
      if (dds_ext->GetOwnSeqId().low != 0 || dds_ext->GetOwnSeqId().high != 0) {
        seq_id = dds_ext->GetOwnSeqId().high;
        seq_id = (seq_id << 32) | (dds_ext->GetOwnSeqId().low);
        COHLOGD("AsyncSendRequest seq_id is{}", seq_id);
        if (seq_id_ptr != nullptr) {
          *seq_id_ptr = seq_id;
        }
      }
      pending_requests_[seq_id] = std::move(pending_request);
    }
#endif
  } else {
    {
      std::unique_lock<std::mutex> lg{request_mtx_};
      pending_requests_[seq_id] = std::move(pending_request);
    }

    ErrorCode ret = writer_->Write(
        request, std::make_shared<MessageInfo>(client_id_, seq_id, client_id_));
    if (ret != COMM_CODE_OK) {
      if (code) {
        *code = ret;
      }
      {
        std::unique_lock<std::mutex> lg{request_mtx_};
        pending_requests_.erase(seq_id);
      }
      COHLOGE("client write request failed!")
      return FutureType();
    }
  }

  ToolsServer::Instance()->UpdateCount(client_id_, TOPIC_COUNT_TYPE::SEND);

  return fut;
}

template<typename SerializerRequest, typename SerializerResponse,
         typename Enable>
void ClientImpl<SerializerRequest, SerializerResponse,
                Enable>::HandleResponse(
    const std::shared_ptr<typename SerializerResponse::MsgType> &msg,
    const spMessageInfo &msg_info) {
  if (!msg || !msg_info) {
    COHLOGE("Topic:{} client receive a null response:"
            " msg is {}; msg_info is {}!", service_name_,
            msg ? " not null" : "null", msg_info ? " not null" : "null");
    return;
  }

  uint64_t seq_id{0u};
  if (proto_type_ == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    auto dds_ext = std::dynamic_pointer_cast<DDSExtInfo>(msg_info);
    if (dds_ext == nullptr) {
      return;
    }
    // message info own dds ext info and check owner id
    COHLOGD("Own Seq Id high: {}, low: {}", dds_ext->GetOwnSeqId().high,
            dds_ext->GetOwnSeqId().low);
    if (dds_ext->GetOwnSeqId().low != 0 || dds_ext->GetOwnSeqId().high != 0) {
      auto ext_guid = dds_ext->GetRelativeGuid();
      auto reader_guid = GetReaderGuid<SerializerResponse>(reader_);
      if (memcmp(&ext_guid, &reader_guid, sizeof(DDSGuid)) != 0u) {
        return;
      }
      seq_id = dds_ext->GetRelativeSeqId().high;
      seq_id = (seq_id << 32) | (dds_ext->GetRelativeSeqId().low);
      COHLOGD("HandleResponse seq_id is{}", seq_id);
    } else {
      if (msg_info->GetSpareId() != client_id_) {
        COHLOGD(
            "client receive a response msg, the spare id[{}] of which"
            "does not match client_id[{}], with seq_id:[{}]!",
            msg_info->GetSpareId(), client_id_, msg_info->GetSeqId());
        return;
      }
      seq_id = msg_info->GetSeqId();
    }
#endif
  } else {
    if (msg_info->GetSpareId() != client_id_) {
      COHLOGD(
          "client receive a response msg, the spare id[{}] of which"
          "does not match client_id[{}], with seq_id:[{}]!",
          msg_info->GetSpareId(), client_id_, msg_info->GetSeqId());
      return;
    }
    seq_id = msg_info->GetSeqId();
  }
  ToolsServer::Instance()->UpdateCount(client_id_, TOPIC_COUNT_TYPE::RECV);

  // COHLOGD("read seq_id: {}", seq_id)
  std::unique_lock<std::mutex> lk{request_mtx_};
  if (this->pending_requests_.count(seq_id) == 0u) {
    COHLOGE(
        "client receive a response msg, "
        "the seq_id[{}] of which has no according pending request",
        seq_id)
    return;
  } else {
    if (tools_service_name_ == service_name_) {
      // tools client maybe has multi response, so no need to erase id
      auto pending_request = this->pending_requests_[seq_id];
      lk.unlock();
      auto callback = std::get<2>(pending_request);
      if (callback) {
        callback(msg);
      }
    } else {
      auto pending_request = this->pending_requests_[seq_id];
      this->pending_requests_.erase(seq_id);
      lk.unlock();
      auto call_promise = std::get<0>(pending_request);
      auto future = std::get<1>(pending_request);
      auto callback = std::get<2>(pending_request);
      auto resp =
          std::pair<ErrorCode,
                    std::shared_ptr<typename SerializerResponse::MsgType>>(
              COMM_CODE_OK, msg);

      call_promise->set_value(resp);
      if (callback) {
        callback(msg);
      }
    }
  }
}

template <typename SerializerRequest, typename SerializerResponse,
          typename Enable>
bool ClientImpl<SerializerRequest, SerializerResponse,
                Enable>::ServiceIsReady() const {
  if (reader_ && writer_) {
    return reader_->IsReady() && writer_->IsReady();
  } else {
    return false;
  }
}

template <typename SerializerRequest, typename SerializerResponse,
          typename Enable>
bool ClientImpl<SerializerRequest, SerializerResponse, Enable>::
    WaitForServiceNanoseconds(std::chrono::nanoseconds time_out) {
  bool service_is_ready{false};
  auto step_duration = std::chrono::nanoseconds(5 * 1000 * 1000);
  while (time_out.count() > 0u) {
    service_is_ready = ServiceIsReady();
    if (!service_is_ready) {
      std::this_thread::sleep_for(step_duration);
      time_out -= step_duration;
    } else {
      return service_is_ready;
    }
  }
  return service_is_ready;
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SERVICE_CLIENT_IMPL_H_
