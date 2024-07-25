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

#ifndef COMMUNICATION_COMMON_CONST_H_
#define COMMUNICATION_COMMON_CONST_H_

#include <cstdint>

namespace hobot {
namespace communication {

constexpr int kOne{1};
constexpr uint32_t kUIntOne{1U};
constexpr int kTwo{2};
constexpr int kZero{0};
constexpr uint32_t kUIntZero{0U};
constexpr int kMinusOne{-1};
constexpr int kKilo{1000};

constexpr char kUnderline{'_'};
constexpr char kColon{':'};
constexpr char kDot{'.'};
constexpr char kAsterisk{'*'};
constexpr char kSlash('/');

// for dds common struct DDSGuid struct
constexpr int kDdsGuidLen{16};

constexpr int kDdsIdOffset{32};

constexpr uint32_t kModeWRX {0777};

constexpr int32_t kInt32_2 {2};
constexpr uint32_t kUInt32_2 {2u};

constexpr int32_t kInt32_3 {3};
constexpr uint32_t kUInt32_3 {3u};

constexpr int32_t kInt32_10 {10};
constexpr uint32_t kUInt32_10 {10u};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_CONST_H_
