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
#ifndef COMMUNICATION_SERVICE_CLIENT_PSM_DDS_IMPL_H_
#define COMMUNICATION_SERVICE_CLIENT_PSM_DDS_IMPL_H_

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "communication/service/service_base_impl.h"
#include "communication/discovery/discovery_manager.h"
#include "communication/participant/dds/dds_reader.h"
#include "communication/participant/dds/dds_writer.h"

namespace hobot {
namespace communication {


// declaration
template <typename SerializerRequest, typename SerializerResponse>
class ClientImpl<
    SerializerRequest, SerializerResponse,
    typename std::enable_if<
        HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
        HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>
    : public ServiceBaseImpl {
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
  uint64_t client_id_ = 0;
  uint64_t seq_id_ = 0;
  std::mutex request_mtx_;
  std::unordered_map<uint64_t, PendingRequestType> pending_requests_;

  // avoid cyclic dependencies
  std::string tools_service_name_;
  std::string service_name_;
  bool is_dynamic_;
  int domain_id_;
  ProtocolType proto_type_;
  std::string entity_uuid_;
  std::string request_topic_;   // service_name + some postfix
  std::string response_topic_;  // service_name + some postfix
  std::shared_ptr<
    Writer<typename SerializerRequest::DDSCSSupportType>> dds_writer_{nullptr};
  std::shared_ptr<
    Reader<typename SerializerResponse::DDSCSSupportType>> dds_reader_{nullptr};
  std::shared_ptr<Writer<SerializerRequest>> writer_{nullptr};
  std::shared_ptr<Reader<SerializerResponse>> reader_{nullptr};
  ToolsServer* tool_server_{nullptr};
  DiscoveryManager* discovery_manager_{nullptr};

  void HandleResponse(
      const std::shared_ptr<typename SerializerResponse::MsgType> &msg,
      const spMessageInfo &msg_info);
};

// definition
template<typename SerializerRequest, typename SerializerResponse>
ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
    ClientImpl(
      const CommAttr &comm_attr, const std::string &service_name, int domain_id,
      ProtocolType type) {
  COHLOGD("Create client_psm_dds_impl,topic={}", service_name);
  // get client uuid
  client_id_ = ToolsServer::Instance()->AddTopic(comm_attr,
                                                 service_name,
                                                 domain_id,
                                                 TOPIC_TYPE::CLIENT);
  COHLOGD("Create a new Client, service_name={}, protocol={}, client_id={}",
                                          service_name, type, client_id_);
  tools_service_name_ = ToolsServer::Instance()->GetToolsServiceName();
  service_name_ = service_name;
  proto_type_ = type;
  domain_id_ = domain_id;
  if (tools_service_name_ != service_name_) {
    tool_server_ = ToolsServer::Instance();
  }
  if (comm_attr.participant_attrs_.empty()) {
    is_dynamic_ = true;
  } else {
    is_dynamic_ = false;
  }
  TopicAssemblier::ExpandTopic(service_name, request_topic_, response_topic_,
                               type);
  // input comm_attr is for writer
  CommAttr comm_attr_w = GenerateCommAttr(comm_attr, GenType::CLIENT);
  if (type == PROTOCOL_DDS) {
#ifdef SUPPORT_DDS
    dds_writer_.reset(CreateDDSWriter<
      typename SerializerRequest::DDSCSSupportType>(
        comm_attr, request_topic_, domain_id, type));
    if (!dds_writer_) {
      COHLOGE("client CreateDDSWriter failed, rturn nullptr");
    }
    auto psm_dds_read_cb = [this](
      const std::shared_ptr<
        typename SerializerResponse::MsgType::DDSCSMsgType> &msg,
      const spMessageInfo &msg_info) {
        auto cb_msg = msg->comm_user_define_msg();
        auto resp_cb_msg =
            std::make_shared<
              typename SerializerResponse::MsgType>(std::move(*cb_msg));
        HandleResponse(resp_cb_msg, msg_info);
    };
    dds_reader_.reset(CreateDDSReader<
      typename SerializerResponse::DDSCSSupportType>(
        comm_attr, response_topic_, domain_id, psm_dds_read_cb, type));
    if (!dds_reader_) {
      COHLOGE("client CreateDDSReader failed, rturn nullptr");
    }
#endif
  } else {
    if (proto_type_ == PROTOCOL_PCIE) {
      writer_.reset(new Writer<SerializerRequest>(comm_attr, request_topic_,
                                                  domain_id, type));
    } else {
      writer_.reset(new Writer<SerializerRequest>(comm_attr_w, request_topic_,
                                                  domain_id, type));
    }
    auto read_cb = std::bind(
      &ClientImpl<SerializerRequest, SerializerResponse>::HandleResponse, this,
      std::placeholders::_1, std::placeholders::_2);
    reader_.reset(new Reader<SerializerResponse>(comm_attr, response_topic_,
                                                 domain_id, read_cb, type));
  }
}

template<typename SerializerRequest, typename SerializerResponse>
ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      ~ClientImpl() {
  COHLOGD("Destroy a Client, service_name={}, protocol={}, client_id={}",
                                    service_name_, proto_type_, client_id_);
  if (tools_service_name_ != service_name_) {
    if (tool_server_ != nullptr) {
      tool_server_->RemoveTopic(client_id_);
    }
  }
  // pending_requests_.clear();
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
ErrorCode ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
    Init() {
  COHLOGD("Client Init, service_name={}, protocol={}, client_id={}",
                                    service_name_, proto_type_, client_id_);

  discovery_manager_ = DiscoveryManager::instance();
  entity_uuid_ = discovery_manager_->regist_entity(
      EntityType::ENTITY_CLIENT, service_name_, domain_id_, proto_type_);
  ErrorCode ret;
  if (proto_type_ == PROTOCOL_DDS) {
    if (!dds_writer_ || !dds_reader_) {
      COHLOGE("writer or reader is nullptr!!!");
      return COMM_CODE_PARAM_IS_NULL;
    }
    ret = dds_writer_->Init();
    if (ret == COMM_CODE_OK) {
      ret = dds_reader_->Init();
    }
  } else {
    ret = writer_->Init();
    if (ret == COMM_CODE_OK) {
      ret = reader_->Init();
    }
  }

  if (ret != COMM_CODE_OK) {
    COHLOGE("ClientImpl init failed!")
  }
  return ret;
}

template<typename SerializerRequest, typename SerializerResponse>
void ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      SetEventListener(
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
ErrorCode ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      SendRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> &request,
    std::shared_ptr<typename SerializerResponse::MsgType> &response,
    int timeout) {
  ErrorCode ret = COMM_CODE_CLIENT_SEND_FAILED;
  uint64_t seq_id = 0;
  FutureType fut =
      AsyncSendRequest(request, nullptr, &ret, &seq_id);
  if (!fut.valid()) {
    return ret;
  }
  if (timeout >= 0) {
    COHLOGD("Topic:{} client wait for response with timeout:{}ms",
             service_name_, timeout);
    auto status = fut.wait_for(std::chrono::milliseconds(timeout));
    if (status == std::future_status::ready) {
      auto fut_val = fut.get();
      response = fut_val.second;
      ret = fut_val.first;
      return ret;
    } else {
      std::unique_lock<std::mutex> lg{request_mtx_};
      if (pending_requests_.count(seq_id) != 0) {
        pending_requests_.erase(seq_id);
      }
      return COMM_CODE_CLIENT_TIMEOUT;
    }
  } else {
    COHLOGD("client wait for response without timeout setting")
    auto fut_val = fut.get();
    response = fut_val.second;
    ret = fut_val.first;
    return ret;
  }
}

template<typename SerializerRequest, typename SerializerResponse>
std::shared_future<
    std::pair<ErrorCode,
              std::shared_ptr<typename SerializerResponse::MsgType>>>
ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      AsyncSendRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> &request,
    const ClientCallback<SerializerResponse> &client_callback,
    ErrorCode *code,
    uint64_t *seq_id_ptr) {
  uint64_t seq_id = 0;
  {
    std::unique_lock<std::mutex> lg{request_mtx_};
    seq_id = seq_id_++;
    if (seq_id_ptr != nullptr) {
      *seq_id_ptr = seq_id;
    }
  }

  PromiseType prom = std::make_shared<std::promise<RespValType>>();
  FutureType fut = FutureType(prom->get_future());
  RespCallBackType resp_cb = nullptr;
  if (client_callback) {
    resp_cb = std::bind(client_callback, COMM_CODE_OK, std::placeholders::_1);
  }
  PendingRequestType pending_request = std::make_tuple(prom, fut, resp_cb);
  {
    std::unique_lock<std::mutex> lg{request_mtx_};
    pending_requests_[seq_id] = std::move(pending_request);
  }

  ErrorCode ret;
  if (proto_type_ == PROTOCOL_DDS) {
    auto dds_ext_info =
      std::make_shared<typename SerializerRequest::MsgType::DDSCSMsgType>();
    // TODO(heyao.cai):  The current version does not use the move constructor.
    // Add AsyncSendRequestMove interface in version 0330,
    // which uses move constructor to improve performance
    auto request_send_msg =
      std::make_shared<typename SerializerRequest::MsgType>(*(request.get()));
    dds_ext_info->set_comm_user_define_msg(request_send_msg.get(), false);
    ret = dds_writer_->Write(
      dds_ext_info,
      std::make_shared<DDSExtInfo>(client_id_, seq_id, client_id_));
  } else {
    ret = writer_->Write(
        request, std::make_shared<MessageInfo>(client_id_, seq_id, client_id_));
  }
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

  ToolsServer::Instance()->UpdateCount(client_id_, TOPIC_COUNT_TYPE::SEND);

  return fut;
}

template<typename SerializerRequest, typename SerializerResponse>
void ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      HandleResponse(
    const std::shared_ptr<typename SerializerResponse::MsgType> &msg,
    const spMessageInfo &msg_info) {
  if (!msg || !msg_info) {
    COHLOGE("client receive a null response: msg is {}; msg_info is {}!",
            msg ? " not null" : "null", msg_info ? " not null" : "null")
    return;
  }
  if (msg_info->GetSpareId() != client_id_) {
    COHLOGD("client receive a response msg, the spare id[{}] of which"
            "does not match client_id[{}], with seq_id:[{}]!",
            msg_info->GetSpareId(), client_id_, msg_info->GetSeqId());
    return;
  }

  ToolsServer::Instance()->UpdateCount(client_id_, TOPIC_COUNT_TYPE::RECV);

  uint64_t seq_id = msg_info->GetSeqId();
  // COHLOGD("read seq_id: {}", seq_id)
  std::unique_lock<std::mutex> lk{request_mtx_};
  if (this->pending_requests_.count(seq_id) == 0) {
    COHLOGE("client receive a response msg, "
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

template<typename SerializerRequest, typename SerializerResponse>
bool ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
      ServiceIsReady() const {
  if (proto_type_ == PROTOCOL_DDS) {
    if (dds_reader_ && dds_writer_) {
      return dds_reader_->IsReady() && dds_writer_->IsReady();
    } else {
      return false;
    }
  } else {
    if (reader_ && writer_) {
      return reader_->IsReady() && writer_->IsReady();
    } else {
      return false;
    }
  }
}

template<typename SerializerRequest, typename SerializerResponse>
bool ClientImpl<SerializerRequest, SerializerResponse,
  typename std::enable_if<
    HasBole2PSMDDSFlag<typename SerializerRequest::MsgType>::value &&
    HasBole2PSMDDSFlag<typename SerializerResponse::MsgType>::value>::type>::
WaitForServiceNanoseconds(std::chrono::nanoseconds time_out) {
  bool service_is_ready = false;
  auto step_duration = std::chrono::nanoseconds(5 * 1000 * 1000);
  while (time_out.count() > 0) {
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

#endif  // COMMUNICATION_SERVICE_CLIENT_PSM_DDS_IMPL_H_

