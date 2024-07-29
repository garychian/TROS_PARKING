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

#ifndef COMMUNICATION_DISCOVERY_TYPES_H_
#define COMMUNICATION_DISCOVERY_TYPES_H_
#include <functional>
#include <vector>
#include <memory>
#include <string>
#include "../participant/participant.h"

namespace hobot {
namespace communication {

typedef
  std::function<void(const std::vector<std::shared_ptr<Participant>>&)>
    ConnChangeCallback;

typedef std::function<void(std::shared_ptr<Participant>,
                      std::string, QoSAttr qos_attr)> ConnAddCallback;

typedef std::function<void(int, std::string)> ConnRemoveCallback;

enum class EntityType {
  ENTITY_PUB = 0,
  ENTITY_SUB,
  ENTITY_CLIENT,
  ENTITY_SERVICE,
  ENTITY_ACTION_CLIENT,
  ENTITY_ACTION_SERVICE
};

struct EntityInfo {
  std::string uuid;
  EntityType entity_type;
  int domain_id;
  std::string topic;
  int protocol_type;
  int seq{0};  // use seq to identity the info changed
};


}  // namespace communication
}  // namespace hobot
#endif
