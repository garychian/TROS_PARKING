/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Chen Zhenan <chenzhenan@senseauto.com>
 */

#pragma once
#include "ad_log/ad_log.hpp"


#define AD_LPTRACE(tag, fmt, ...) LOGP_TRACE("[" #tag "] " fmt, __VA_ARGS__)
#define AD_LPDEBUG(tag, fmt, ...) LOGP_DEBUG("[" #tag "] " fmt, __VA_ARGS__)
#define AD_LPINFO(tag, fmt, ...) LOGP_INFO("[" #tag "] " fmt, __VA_ARGS__)
#define AD_LPWARN(tag, fmt, ...) LOGP_WARN("[" #tag "] " fmt, __VA_ARGS__)
#define AD_LPERROR(tag, fmt, ...) LOGP_ERROR("[" #tag "] " fmt, __VA_ARGS__)
#define AD_LPCRITICAL(tag, fmt, ...) LOGP_CRITICAL("[" #tag "] " fmt, __VA_ARGS__)
