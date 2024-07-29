/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Zeng Chao <zengchao@senseauto.com>
 */

#pragma once

#include "common/benchmark/Benchmark.hpp"

namespace senseAD {
namespace perception {

#ifdef ENABLE_BENCHMARK

#define OP_LOCAL_BENCHMARK_CREATE(NAME) \
    AD_LOCAL_BENCHMARK_CREATE(NAME);    \
    static std::set<std::string> bench_names {}

#define OP_LOCAL_BENCHMARK_REG(BENCHMARK_TARGET) \
    AD_LOCAL_BENCHMARK_REG(BENCHMARK_TARGET);    \
    static auto reg_flags_##BENCHMARK_TARGET =   \
        bench_names.emplace(#BENCHMARK_TARGET)

#define OP_LOCAL_BENCHMARK_REG_COMPLETE(OPNAMESTR) \
    for (auto& iter : bench_names) {               \
        auto complete_name = OPNAMESTR + iter;     \
        AD_LOCAL_BENCHMARK_REG_STR(complete_name); \
    }

#define OP_LOCAL_BENCHMARK_RECORD_IN(BENCHMARK_TARGET, OPNAMESTR) \
    AD_LOCAL_BENCHMARK_RECORD_IN(BENCHMARK_TARGET);               \
    AD_LOCAL_BENCHMARK_RECORD_IN_STR(OPNAMESTR + #BENCHMARK_TARGET)

#define OP_LOCAL_BENCHMARK_RECORD_OUT(BENCHMARK_TARGET, OPNAMESTR) \
    AD_LOCAL_BENCHMARK_RECORD_OUT(BENCHMARK_TARGET);               \
    AD_LOCAL_BENCHMARK_RECORD_OUT_STR(OPNAMESTR + #BENCHMARK_TARGET)

#define OP_LOCAL_BENCHMARK_SCOPE_RECORD(BENCHMARK_TARGET, OPNAMESTR) \
    AD_LOCAL_BENCHMARK_SCOPE_RECORD(BENCHMARK_TARGET);               \
    AD_LOCAL_BENCHMARK_SCOPE_RECORD_STR(BENCHMARK_TARGET##PART,      \
                                        OPNAMESTR + #BENCHMARK_TARGET)

#else

#define OP_LOCAL_BENCHMARK_CREATE(NAME)
#define OP_LOCAL_BENCHMARK_REG(BENCHMARK_TARGET)
#define OP_LOCAL_BENCHMARK_REG_COMPLETE(OPNAMESTR)
#define OP_LOCAL_BENCHMARK_RECORD_IN(BENCHMARK_TARGET, OPNAMESTR)
#define OP_LOCAL_BENCHMARK_RECORD_OUT(BENCHMARK_TARGET, OPNAMESTR)
#define OP_LOCAL_BENCHMARK_SCOPE_RECORD(BENCHMARK_TARGET, OPNAMESTR)

#endif
}  // namespace perception
}  // namespace senseAD
