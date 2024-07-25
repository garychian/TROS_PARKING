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

// This is the implementation of a sub callback swtich

#ifndef COMMUNICATION_SUB_RCV_SWITCH_H_
#define COMMUNICATION_SUB_RCV_SWITCH_H_

#include <string>
#include <memory>
#include <utility>
#include "communication/common/errors.h"

namespace hobot {
namespace communication {

class SubscriberBase;

//  close the switch of sub callback
ErrorCode SwitchOff(const std::shared_ptr<SubscriberBase> &subscriber);

//  open the switch of sub callback
ErrorCode SwitchOn(const std::shared_ptr<SubscriberBase> &subscriber);

// return the status of the switch
bool IsSwitchOff(const std::shared_ptr<SubscriberBase> &subscriber);

}  // namespace communication
}  // namespace hobot
#endif  // COMMUNICATION_SUB_RCV_SWITCH_H_
