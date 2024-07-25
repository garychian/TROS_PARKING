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

#ifndef COMMUNICATION_QOS_POLICY_COMMON_H_
#define COMMUNICATION_QOS_POLICY_COMMON_H_
#include <string>

namespace hobot {
namespace communication {

#define PLUGIN_QOS_VERSION 1

// for qos capability query
struct QosCapability {
  int32_t version = PLUGIN_QOS_VERSION;  // version number
  bool is_transport_priority_qos_supported{false};
  bool is_reliability_qos_supported{false};
};

// for qos parameter setting
// -2 means do nothing
// transport_priority_qos_paramters = 0 : means transport_priority_level_0;
// reliability_qos_parameters = -1: means unlimit resource _cache or depth
struct QosParameterConfig {
  int version = PLUGIN_QOS_VERSION;
  int32_t transport_priority_qos_paramters{-2};
  int32_t reliability_qos_parameters{-2};
};

}  // namespace communication
}  // namespace hobot

#endif
