/*
 * Copyright (C) 2017-2020 by SenseTime Group Limited. All rights reserved.
 * YU Chendi <yuchendi@sensetime.com>
 * He Yuan <heyuan@sensetime.com>
 */

#pragma once

#include <iostream>

#include "ad_log/ad_log.hpp"

#ifdef WITH_TDA
#define AD_LTRACE(tag) std::cout << std::endl << "[" << #tag << "]: "
#define AD_LDEBUG(tag) std::cout << std::endl << "[" << #tag << "]: "
#define AD_LINFO(tag) std::cout << std::endl << "[" << #tag << "]: "
#define AD_LERROR(tag) std::cout << std::endl << "[" << #tag << "]: "
#define AD_LWARN(tag) std::cout << std::endl << "[" << #tag << "]: "
#define AD_LFATAL(tag) std::cout << std::endl << "[" << #tag << "]: "

// define only use in adas for compile
#define AD_FTRACE(tag) FLOGS_TRACE(tag)
#define AD_FDEBUG(tag) FLOGS_DEBUG(tag)
#define AD_FINFO(tag) FLOGS_INFO(tag)
#define AD_FERROR(tag) FLOGS_ERROR(tag)
#define AD_FWARN(tag) FLOGS_WARN(tag)
#define AD_FFATAL(tag) FLOGS_CRITICAL(tag)

#else

#define AP_LTRACE(tag) AD_LTRACE(tag)
#define AP_LDEBUG(tag) AD_LDEBUG(tag)
#define AP_LINFO(tag) AD_LINFO(tag)
#define AP_LERROR(tag) AD_LERROR(tag)
#define AP_LWARN(tag) AD_LWARN(tag)
#define AP_LFATAL(tag) AD_LFATAL(tag)
#define AP_LWARN_EVERY(tag, n) AD_LWARN_EVERY(tag, n)
#define AP_LERROR_EVERY_SEC(tag, n) AD_LERROR_EVERY_SEC(tag, n)

// with the prefix of AD_F  it will log into background log file
#define AP_FTRACE(tag) AD_FTRACE(tag)
#define AP_FDEBUG(tag) AD_FDEBUG(tag)
#define AP_FINFO(tag) AD_FINFO(tag)
#define AP_FERROR(tag) AD_FERROR(tag)
#define AP_FWARN(tag) AD_FWARN(tag)
#define AP_FFATAL(tag) AD_FFATAL(tag)

#endif  // WITH_TDA
