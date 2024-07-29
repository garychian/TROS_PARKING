/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Wu Wenyao <wuwenyao@sensetime.com>
 * Yin Shaopeng <yinshaopeng@sensetime.com>
 */

#ifndef _ADMETA_CORE_OPTIMIZATION_HPP_
#define _ADMETA_CORE_OPTIMIZATION_HPP_

#if defined(__GNUC__) || defined(__ICL) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif
#endif // _ADMETA_CORE_OPTIMIZATION_HPP_
