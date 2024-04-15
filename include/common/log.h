// Copyright (c) [2021-2023] [Horizon Robotics].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#pragma once
#include <syscall.h>
#include <unistd.h>

#include <cstdarg>
#include <string>

#include "hlog/logging.h"

#ifndef LOG_TAG
#define LOG_TAG "samples"
#endif
#define gettid() syscall(__NR_gettid)

#define LOGC(FMT, ...) HTLOG_C(LOG_TAG, "[{}] " FMT, gettid(), ##__VA_ARGS__)
#define LOGE(FMT, ...) HTLOG_E(LOG_TAG, "[{}] " FMT, gettid(), ##__VA_ARGS__)
#define LOGW(FMT, ...) HTLOG_W(LOG_TAG, "[{}] " FMT, gettid(), ##__VA_ARGS__)
#define LOGI(FMT, ...) HTLOG_I(LOG_TAG, "[{}] " FMT, gettid(), ##__VA_ARGS__)
#define LOGD(FMT, ...) HTLOG_D(LOG_TAG, "[{}] " FMT, gettid(), ##__VA_ARGS__)
#define LOGT(FMT, ...) HTLOG_T(LOG_TAG, "[{}] " FMT, gettid(), ##__VA_ARGS__)

#define APA_TRACE    0
#define APA_DEBUG    1
#define APA_INFO     2
#define APA_WARN     3
#define APA_ERROR    4
#define APA_CRITICAL 5

#define APA_LOG_CHECK_AND_RETURN(CONDITION, LOGLEVEL, ERRORCODE, MESSAGE, ...) \
{                                           \
  if (CONDITION){                           \
    switch (LOGLEVEL){                      \
      case APA_TRACE:                           \
        LOGT(MESSAGE, ##__VA_ARGS__);   \
      case APA_DEBUG:                           \
        LOGD(MESSAGE, ##__VA_ARGS__);   \
      case APA_INFO:                            \
        LOGI(MESSAGE, ##__VA_ARGS__);   \
      case APA_WARN:                            \
        LOGW(MESSAGE, ##__VA_ARGS__);   \
      case APA_ERROR:                           \
        LOGE(MESSAGE, ##__VA_ARGS__);   \
      case APA_CRITICAL:                        \
        LOGC(MESSAGE, ##__VA_ARGS__);   \
      default:                              \
        LOGI(MESSAGE, ##__VA_ARGS__);}  \
    return ERRORCODE; }                     \
}

#define APA_LOG_CHECK(CONDITION, LOGLEVEL, MESSAGE, ...) \
{                                           \
  if (CONDITION){                           \
    switch (LOGLEVEL){                      \
      case APA_TRACE:                           \
        LOGT(MESSAGE, ##__VA_ARGS__);   \
      case APA_DEBUG:                           \
        LOGD(MESSAGE, ##__VA_ARGS__);   \
      case APA_INFO:                            \
        LOGI(MESSAGE, ##__VA_ARGS__);   \
      case APA_WARN:                            \
        LOGW(MESSAGE, ##__VA_ARGS__);   \
      case APA_ERROR:                           \
        LOGE(MESSAGE, ##__VA_ARGS__);   \
      case APA_CRITICAL:                        \
        LOGC(MESSAGE, ##__VA_ARGS__);   \
      default:                              \
        LOGI(MESSAGE, ##__VA_ARGS__);}} \
}

#define APA_LOGT_CHECK_AND_RETURN(CONDITION, ERRORCODE, MESSAGE, ...) \
    APA_LOG_CHECK_AND_RETURN(CONDITION, APA_TRACE, ERRORCODE, MESSAGE, ##__VA_ARGS__)
#define APA_LOGD_CHECK_AND_RETURN(CONDITION, ERRORCODE, MESSAGE, ...) \
    APA_LOG_CHECK_AND_RETURN(CONDITION, APA_DEBUG, ERRORCODE, MESSAGE, ##__VA_ARGS__)
#define APA_LOGI_CHECK_AND_RETURN(CONDITION, ERRORCODE, MESSAGE, ...) \
    APA_LOG_CHECK_AND_RETURN(CONDITION, APA_INFO, ERRORCODE, MESSAGE, ##__VA_ARGS__)
#define APA_LOGW_CHECK_AND_RETURN(CONDITION, ERRORCODE, MESSAGE, ...) \
    APA_LOG_CHECK_AND_RETURN(CONDITION, APA_WARN, ERRORCODE, MESSAGE, ##__VA_ARGS__)
#define APA_LOGE_CHECK_AND_RETURN(CONDITION, ERRORCODE, MESSAGE, ...) \
    APA_LOG_CHECK_AND_RETURN(CONDITION, APA_ERROR, ERRORCODE, MESSAGE, ##__VA_ARGS__)
#define APA_LOGC_CHECK_AND_RETURN(CONDITION, ERRORCODE, MESSAGE, ...) \
    APA_LOG_CHECK_AND_RETURN(CONDITION, APA_CRITICAL, ERRORCODE, MESSAGE, ##__VA_ARGS__)

#define APA_LOGT_CHECK(CONDITION, MESSAGE, ...) \
    APA_LOG_CHECK(CONDITION, APA_TRACE, MESSAGE, ##__VA_ARGS__)
#define APA_LOGD_CHECK(CONDITION, MESSAGE, ...) \
    APA_LOG_CHECK(CONDITION, APA_DEBUG, MESSAGE, ##__VA_ARGS__)
#define APA_LOGI_CHECK(CONDITION, MESSAGE, ...) \
    APA_LOG_CHECK(CONDITION, APA_INFO, MESSAGE, ##__VA_ARGS__)
#define APA_LOGW_CHECK(CONDITION, MESSAGE, ...) \
    APA_LOG_CHECK(CONDITION, APA_WARN, MESSAGE, ##__VA_ARGS__)
#define APA_LOGE_CHECK(CONDITION, MESSAGE, ...) \
    APA_LOG_CHECK(CONDITION, APA_ERROR, MESSAGE, ##__VA_ARGS__)
#define APA_LOGC_CHECK(CONDITION, MESSAGE, ...) \
    APA_LOG_CHECK(CONDITION, APA_CRITICAL, MESSAGE, ##__VA_ARGS__)
