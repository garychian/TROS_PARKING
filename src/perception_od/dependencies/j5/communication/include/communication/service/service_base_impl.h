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

// This is service base impl.

#ifndef COMMUNICATION_SERVICE_BASE_IMPL_H_
#define COMMUNICATION_SERVICE_BASE_IMPL_H_

#include <string>
#include <memory>

#include "communication/participant/reader.h"
#include "communication/participant/writer.h"
#include "communication/common/types.h"
#include "communication/service/message_info.h"
#include "communication/common/comm_log.h"
#include "communication/common/tools_server.h"

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
class DLLEXPORT ServiceBaseImpl {
 public:
  enum class GenType : uint8_t { CLIENT = 1, SERVICE };
  ServiceBaseImpl();
  ~ServiceBaseImpl();

 protected:
  const std::string GetRequestPostfix() const;

  const std::string GetResponsePostfix() const;

  uint64_t GetUuid();

  CommAttr GenerateCommAttr(const CommAttr &comm_attr, GenType type);

  void RemoveCommAttr();

  int UpdateGlobalParticipant(GenType type, int participant_id);

 private:
  const std::string srv_req_some_postfix_;
  const std::string srv_res_some_postfix_;
  const int client_attr_ext_id_base_;
  const int service_attr_ext_id_base_;
  std::shared_ptr<CommAttr> comm_attr_ext_;
};

}  // namespace communication
}  // namespace hobot

#endif
