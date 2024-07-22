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

#ifndef COMMUNICATION_QOS_POLICY_H_
#define COMMUNICATION_QOS_POLICY_H_
#include <string>
#include "communication/common/qos_policy_common.h"
namespace hobot {
namespace communication {

enum ReliabilityQosPolicyKind {
  RELIABILITY_DISABLED = 0,
  RELIABILITY_BEST_EFFORT,
  RELIABILITY_RELIABLE,
};

struct ReliabilityQoSPolicy {
  ReliabilityQosPolicyKind kind{RELIABILITY_DISABLED};
  int32_t max_blocking_time{100};  // ms
};

enum HistoryQoSPolicyKind {
  HISTORY_KEEP_LAST = 0,
  HISTORY_KEEP_ALL
};

struct HistoryQoSPolicy {
  HistoryQoSPolicyKind kind{HISTORY_KEEP_LAST};
  int32_t depth{1};
};

enum TransportPriorityQoSPolicyLevel {
  TRANSPORT_PRIORITY_LEVEL_0 = 0,  // level_0 means dosen't support priority qos
  TRANSPORT_PRIORITY_LEVEL_999 = 999
};

struct TransportPriorityQoSPolicy {
  TransportPriorityQoSPolicyLevel level{TRANSPORT_PRIORITY_LEVEL_0};
};

struct ResourceLimitsQosPolicy{
  int32_t max_samples{-1};  // -1 means no limit
};

struct DeadlineQosPolicy {
  int32_t duration{-1};  // -1 means infinite
};

struct LifeSpanQosPolicy {
  int32_t duration{-1};  // -1 means infinite
};

enum LivelinessQosPolicyKind {
  LIVELINESS_AUTOMATIC = 0,
  LIVELINESS_MANUAL_BY_PARTICIPANT,
  LIVELINESS_MANUAL_BY_TOPIC
};

struct LivelinessQosPolicy {
  LivelinessQosPolicyKind kind{LIVELINESS_AUTOMATIC};
  int32_t lease_duration{-1};  // -1 means infinite
};

struct QoSProfile {
  ReliabilityQoSPolicy reliability_qos_policy;
  HistoryQoSPolicy history_qos_policy;
  ResourceLimitsQosPolicy resource_limits_qos_policy;
  TransportPriorityQoSPolicy transport_priority_qos_policy;
  DeadlineQosPolicy deadline_qos_policy;
  LifeSpanQosPolicy life_span_qos_policy;
  LivelinessQosPolicy liveliness_qos_policy;
};

// some built-in qos profile
QoSProfile GetDefaultQosProfile();

QoSProfile GetSensorDataQosProfile();

QoSProfile GetServiceQosProfile();

QoSProfile GetParameterQosProfile();

struct DDSQoSProfile {
  std::string participant_profile;
  std::string topic_profile;
  std::string data_writer_profile;
  std::string data_reader_profile;
};

}  // namespace communication
}  // namespace hobot

#endif
