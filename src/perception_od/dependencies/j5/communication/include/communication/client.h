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

// This is client interface definition.

#ifndef COMMUNICATION_CLIENT_H_
#define COMMUNICATION_CLIENT_H_

#include <future>
#include <memory>
#include <string>
#include <utility>

#include "communication/common/types.h"
#include "communication/service/client_impl.h"
#include "message/message.h"
#ifdef SUPPORT_DDS
#include "communication/service/client_psm_dds_impl.h"
#endif

namespace hobot {
namespace communication {

template <typename SerializerRequest, typename SerializerResponse>
class Client {
 public:
  using SelfType = Client<SerializerRequest, SerializerResponse>;
  static std::shared_ptr<SelfType> New(
      const CommAttr &comm_attr,
      const std::string &service_name,
      int domain_id,
      ProtocolType type = PROTOCOL_HYBRID,
      ErrorCode *error_code = nullptr,
      const ConnEventCallback &event_callback = nullptr);

  ErrorCode SendRequest(
      const std::shared_ptr<typename SerializerRequest::MsgType> &request,
      std::shared_ptr<typename SerializerResponse::MsgType> &response,
      int timeout = -1);

  std::shared_future<std::pair<
      ErrorCode, std::shared_ptr<typename SerializerResponse::MsgType>>>
  AsyncSendRequest(
      const std::shared_ptr<typename SerializerRequest::MsgType> &request,
      const ClientCallback<SerializerResponse> &client_callback = nullptr);

  bool ServiceIsReady() const { return impl_->ServiceIsReady(); }

  template<typename RepT = int64_t, typename RatioT = std::milli>
  bool WaitForService(std::chrono::duration<RepT, RatioT> timeout =
  std::chrono::duration<RepT, RatioT>(-1)) {
    return impl_->WaitForServiceNanoseconds(
        std::chrono::duration_cast<std::chrono::nanoseconds>(timeout));
  }

  ~Client();

 private:
  Client(const CommAttr &attr, const std::string &service_name, int domain_id,
         ProtocolType type);

  ErrorCode Init();

  void SetEventListener(const ConnEventCallback &event_callback);

  ErrorCode Destroy();

 private:
  std::unique_ptr<ClientImpl<SerializerRequest, SerializerResponse>> impl_;
};

template<typename SerializerRequest, typename SerializerResponse>
auto Client<SerializerRequest, SerializerResponse>::New(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    ProtocolType type, ErrorCode *error_code,
    const ConnEventCallback &event_callback)
-> std::shared_ptr<Client<SerializerRequest, SerializerResponse>> {
  COHLOGI("Create client topic:{}, domain_id:{}, type:{}",
          service_name, domain_id, type);
  auto ret = PreConstructClientCheck(comm_attr, service_name, domain_id, type);
  if (ret != COMM_CODE_OK) {
    if (error_code != nullptr) {
      *error_code = ret;
    }
    return nullptr;
  }
  auto *raw_ptr = new Client<SerializerRequest, SerializerResponse>(
      comm_attr, service_name, domain_id, type);
  auto ptr =
      std::shared_ptr<Client<SerializerRequest, SerializerResponse>>(raw_ptr);
  ptr->SetEventListener(event_callback);
  ret = ptr->Init();
  if (error_code != nullptr) {
    *error_code = ret;
  }
  if (ret == COMM_CODE_OK) {
    COHLOGI("Created client successfully, topic:{}", service_name);
  }
  return (ret == COMM_CODE_OK) ? ptr : nullptr;
}

template<typename SerializerRequest, typename SerializerResponse>
ErrorCode Client<SerializerRequest, SerializerResponse>::SendRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> &request,
    std::shared_ptr<typename SerializerResponse::MsgType> &response,
    int timeout) {
  return impl_->SendRequest(request, response, timeout);
}

template<typename SerializerRequest, typename SerializerResponse>
std::shared_future<
    std::pair<ErrorCode,
              std::shared_ptr<typename SerializerResponse::MsgType>>>
Client<SerializerRequest, SerializerResponse>::AsyncSendRequest(
    const std::shared_ptr<typename SerializerRequest::MsgType> &request,
    const ClientCallback<SerializerResponse> &client_callback) {
  return impl_->AsyncSendRequest(request, client_callback);
}

template<typename SerializerRequest, typename SerializerResponse>
Client<SerializerRequest, SerializerResponse>::~Client() {
  Destroy();
  impl_ = nullptr;
}

template<typename SerializerRequest, typename SerializerResponse>
Client<SerializerRequest, SerializerResponse>::Client(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    ProtocolType type) {
  impl_.reset(new ClientImpl<SerializerRequest, SerializerResponse>(
      comm_attr, service_name, domain_id, type));
}

template<typename SerializerRequest, typename SerializerResponse>
ErrorCode Client<SerializerRequest, SerializerResponse>::Init() {
  return impl_->Init();
}

template<typename SerializerRequest, typename SerializerResponse>
void Client<SerializerRequest, SerializerResponse>::SetEventListener(
    const ConnEventCallback &event_callback) {
  impl_->SetEventListener(event_callback);
}

template<typename SerializerRequest, typename SerializerResponse>
ErrorCode Client<SerializerRequest, SerializerResponse>::Destroy() {
  return COMM_CODE_OK;
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_CLIENT_H_
