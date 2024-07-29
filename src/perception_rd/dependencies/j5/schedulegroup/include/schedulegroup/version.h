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

#ifndef SCHEDULEGROUP_VERSION_H_
#define SCHEDULEGROUP_VERSION_H_

#include <string>

#include "schedulegroup/utils/visibility_control.h"

namespace hobot {
namespace schedulegroup {

BOLE_PUBLIC std::string GetVersion(int32_t *major = nullptr,
                                   int32_t *minor = nullptr,
                                   int32_t *patch = nullptr);

}  // namespace schedulegroup
}  // namespace hobot

#endif  // SCHEDULEGROUP_VERSION_H_
