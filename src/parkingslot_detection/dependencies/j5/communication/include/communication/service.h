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

// This is service interface definition.

#ifndef COMMUNICATION_SERVICE_H_
#define COMMUNICATION_SERVICE_H_

#include <memory>
#include <string>

#include "message/message.h"
#include "communication/service/service_impl.h"
#include "communication/common/types.h"
#ifdef SUPPORT_DDS
#include "communication/service/service_psm_dds_impl.h"
#endif

namespace hobot {
namespace communication {

template<typename SerializerRequest, typename SerializerResponse>
class Service {
 public:
  using SelfType = Service<SerializerRequest, SerializerResponse>;
  static std::shared_ptr<SelfType>
  New(const CommAttr &comm_attr,
      const std::string &service_name,
      int domain_id,
      const ServiceCallback<SerializerRequest, SerializerResponse>
      &service_callback,
      ProtocolType type = PROTOCOL_HYBRID,
      ErrorCode *error_code = nullptr,
      const ConnEventCallback &event_callback = nullptr);

  ~Service();

 private:
  Service(const CommAttr &comm_attr, const std::string &service_name,
          int domain_id,
          const ServiceCallback<SerializerRequest, SerializerResponse>
          &service_callback,
          ProtocolType type);

  ErrorCode Init();

  void SetEventListener(const ConnEventCallback &event_callback);

  ErrorCode Destroy();

 private:
  std::unique_ptr<ServiceImpl<SerializerRequest, SerializerResponse>> impl_;
};

template<typename SerializerRequest, typename SerializerResponse>
auto Service<SerializerRequest, SerializerResponse>::New(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    const ServiceCallback<SerializerRequest, SerializerResponse>
    &service_callback,
    ProtocolType type, ErrorCode *error_code,
    const ConnEventCallback &event_callback)
-> std::shared_ptr<Service<SerializerRequest, SerializerResponse>> {
  COHLOGI("Create service topic:{}, domain_id:{}, type:{}",
          service_name, domain_id, type);
  bool is_service_cb_null{service_callback == nullptr};
  auto ret = PreConstructServiceCheck(comm_attr, service_name, domain_id, type,
                                      is_service_cb_null);
  if (ret != COMM_CODE_OK) {
    if (error_code != nullptr) {
      *error_code = ret;
    }
    return nullptr;
  }

  auto *raw_ptr = new Service<SerializerRequest, SerializerResponse>(
      comm_attr, service_name, domain_id, service_callback, type);
  auto ptr =
      std::shared_ptr<Service<SerializerRequest, SerializerResponse>>(raw_ptr);
  ptr->SetEventListener(event_callback);
  ret = ptr->Init();
  if (error_code) {
    *error_code = ret;
  }
  if (ret == COMM_CODE_OK) {
    COHLOGI("Create service successfully, topic:{}", service_name);
  }
  return (ret == COMM_CODE_OK) ? ptr : nullptr;
}

template<typename SerializerRequest, typename SerializerResponse>
Service<SerializerRequest, SerializerResponse>::~Service() {
  Destroy();
}

template<typename SerializerRequest, typename SerializerResponse>
Service<SerializerRequest, SerializerResponse>::Service(
    const CommAttr &comm_attr, const std::string &service_name, int domain_id,
    const ServiceCallback<SerializerRequest, SerializerResponse>
    &service_callback,
    ProtocolType type) {
  impl_.reset(new ServiceImpl<SerializerRequest, SerializerResponse>(
      comm_attr, service_name, domain_id, service_callback, type));
}

template<typename SerializerRequest, typename SerializerResponse>
ErrorCode Service<SerializerRequest, SerializerResponse>::Init() {
  return impl_->Init();
}

template<typename SerializerRequest, typename SerializerResponse>
void Service<SerializerRequest, SerializerResponse>::SetEventListener(
    const ConnEventCallback &event_callback) {
  impl_->SetEventListener(event_callback);
}

template<typename SerializerRequest, typename SerializerResponse>
ErrorCode Service<SerializerRequest, SerializerResponse>::Destroy() {
  impl_.reset(nullptr);
  return COMM_CODE_OK;
}

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_SERVICE_H_
