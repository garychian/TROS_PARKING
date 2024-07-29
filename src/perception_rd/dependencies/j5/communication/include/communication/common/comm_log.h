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

// This is communication log definition.

#ifndef COMMUNICATION_COMMON_COMM_LOG_H_
#define COMMUNICATION_COMMON_COMM_LOG_H_

#include <cstdarg>
#include <string>
#include <memory>
#include <iostream>
#include <cstdio>
#include "communication/common/errors.h"

// todo @chenjian  here change HTLOG to cout to avoid compile error for unknown reason

#define COHLOGC(...)                    \
    do                                  \
    {                                   \
        std::cout << "communication: "; \
        printf(__VA_ARGS__);            \
        std::cout << std::endl;         \
    } while (0);
#define COHLOGE(...)                    \
    do                                  \
    {                                   \
        std::cout << "communication: "; \
        printf(__VA_ARGS__);            \
        std::cout << std::endl;         \
    } while (0);
#define COHLOGW(...)                    \
    do                                  \
    {                                   \
        std::cout << "communication: "; \
        printf(__VA_ARGS__);            \
        std::cout << std::endl;         \
    } while (0);
#define COHLOGI(...)                    \
    do                                  \
    {                                   \
        std::cout << "communication: "; \
        printf(__VA_ARGS__);            \
        std::cout << std::endl;         \
    } while (0);
#define COHLOGD(...)                    \
    do                                  \
    {                                   \
        std::cout << "communication: "; \
        printf(__VA_ARGS__);            \
        std::cout << std::endl;         \
    } while (0);
#define COHLOGT(...)                    \
    do                                  \
    {                                   \
        std::cout << "communication: "; \
        printf(__VA_ARGS__);            \
        std::cout << std::endl;         \
    } while (0);

// #include "communication/common/compiler_features.h"
// COMM_MSVC_SUPPRESS_WARNING_WITH_PUSH(4996)
// #include "hlog/logging.h"
// COMM_MSVC_SUPPRESS_WARNING_POP

// #ifndef _WIN32
// #define COHLOGC(...)  HTLOG_C("communication", ##__VA_ARGS__);
// #define COHLOGE(...)  HTLOG_E("communication", ##__VA_ARGS__);
// #define COHLOGW(...)  HTLOG_W("communication", ##__VA_ARGS__);
// #define COHLOGI(...)  HTLOG_I("communication", ##__VA_ARGS__);
// #define COHLOGD(...)  HTLOG_D("communication", ##__VA_ARGS__);
// #define COHLOGT(...)  HTLOG_T("communication", ##__VA_ARGS__);
// #else
// #define COHLOGC(FMT, ...)  HTLOG_C("communication", FMT, ##__VA_ARGS__);
// #define COHLOGE(FMT, ...)  HTLOG_E("communication", FMT, ##__VA_ARGS__);
// #define COHLOGW(FMT, ...)  HTLOG_W("communication", FMT, ##__VA_ARGS__);
// #define COHLOGI(FMT, ...)  HTLOG_I("communication", FMT, ##__VA_ARGS__);
// #define COHLOGD(FMT, ...)  HTLOG_D("communication", FMT, ##__VA_ARGS__);
// #define COHLOGT(FMT, ...)  HTLOG_T("communication", FMT, ##__VA_ARGS__);
// #endif  // _WIN32

#endif // COMMUNICATION_COMMON_COMM_LOG_H_