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

//  Copyright 2022 Horizon Robotics
//  Created by shuxin.li on 11/10/2022

#ifndef COMMUNICATION_COMMON_CONFIG_OP_H_
#define COMMUNICATION_COMMON_CONFIG_OP_H_

#include <string>
#include "communication/core_types.h"
#include "communication/common/types.h"

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable : 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

class DLLEXPORT CommConfigOperate {
 public:
  static std::string GetSubidlTypeByProtocolType(int protocol_type);
  static bool IsNeedE2EProtect(int part_id, int protocol_type);
  static bool GetE2EConfig(int parti_id,
                           std::string topic,
                           int protocol_type,
                           E2EConfig &config);
  static uint32_t GetE2EConfigDataID(int parti_id,
                           std::string topic,
                           int protocol_type);
};


}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_CONFIG_OP_H_
