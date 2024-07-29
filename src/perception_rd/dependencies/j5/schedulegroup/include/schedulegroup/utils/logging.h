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

#ifndef SCHEDULEGROUP_UTILS_LOGGING_H_
#define SCHEDULEGROUP_UTILS_LOGGING_H_

#include "hlog/logging.h"

#define SGHLOG_T(FMT, ...) HTLOG_T("schedulegroup", FMT, ##__VA_ARGS__);
#define SGHLOG_D(FMT, ...) HTLOG_D("schedulegroup", FMT, ##__VA_ARGS__);
#define SGHLOG_I(FMT, ...) HTLOG_I("schedulegroup", FMT, ##__VA_ARGS__);
#define SGHLOG_W(FMT, ...) HTLOG_W("schedulegroup", FMT, ##__VA_ARGS__);
#define SGHLOG_E(FMT, ...) HTLOG_E("schedulegroup", FMT, ##__VA_ARGS__);
#define SGHLOG_C(FMT, ...) HTLOG_C("schedulegroup", FMT, ##__VA_ARGS__);

#endif  // SCHEDULEGROUP_UTILS_LOGGING_H_
