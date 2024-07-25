/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 */

#pragma once

#include <map>
#include <string>
#include <memory>
#include <shared_mutex>

#include "common/benchmark/BenchmarkType.hpp"

namespace senseAD {
namespace perception {

#ifdef ENABLE_BENCHMARK

#define AD_LOCAL_BENCHMARK g_LocalBenchmark

#define AD_LOCAL_BENCHMARK_CREATE(NAME)                         \
    static std::shared_ptr<BenchMark>& g_LocalBenchmark =       \
        BenchMarkInstance::CreateBenchmark(#NAME);              \
    static std::map<std::string, size_t> g_LocalBenchmarkStrId; \
    static std::shared_timed_mutex g_LocalBenchmarkMtx

#define AD_LOCAL_BENCHMARK_ID(BENCHMARK_TARGET) \
    g_LocalBenchmarkId##BENCHMARK_TARGET

#define AD_LOCAL_BENCHMARK_REG(BENCHMARK_TARGET)                  \
    static const size_t AD_LOCAL_BENCHMARK_ID(BENCHMARK_TARGET) = \
        AD_LOCAL_BENCHMARK->AddItem(#BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_RECORD_IN(BENCHMARK_TARGET) \
    AD_LOCAL_BENCHMARK->RecordEnter(AD_LOCAL_BENCHMARK_ID(BENCHMARK_TARGET))
#define AD_LOCAL_BENCHMARK_RECORD_OUT(BENCHMARK_TARGET) \
    AD_LOCAL_BENCHMARK->RecordExit(AD_LOCAL_BENCHMARK_ID(BENCHMARK_TARGET))

#define AD_LOCAL_BENCHMARK_REG_STR(BENCHMARK_TARGET)     \
    g_LocalBenchmarkMtx.lock();                          \
    g_LocalBenchmarkStrId[(BENCHMARK_TARGET)] =          \
        AD_LOCAL_BENCHMARK->AddItem((BENCHMARK_TARGET)); \
    g_LocalBenchmarkMtx.unlock()
#define AD_LOCAL_BENCHMARK_RECORD_IN_STR(BENCHMARK_TARGET) \
    g_LocalBenchmarkMtx.lock_shared();                     \
    AD_LOCAL_BENCHMARK->RecordEnter(                       \
        g_LocalBenchmarkStrId[(BENCHMARK_TARGET)]);        \
    g_LocalBenchmarkMtx.unlock_shared()
#define AD_LOCAL_BENCHMARK_RECORD_OUT_STR(BENCHMARK_TARGET)                    \
    g_LocalBenchmarkMtx.lock_shared();                                         \
    AD_LOCAL_BENCHMARK->RecordExit(g_LocalBenchmarkStrId[(BENCHMARK_TARGET)]); \
    g_LocalBenchmarkMtx.unlock_shared()

/* Scope Recorder */
class BenchmarkScopeRecorder {
 public:
    explicit BenchmarkScopeRecorder(const size_t target,
                                    std::shared_ptr<BenchMark> local_benmark)
        : target_(target), local_benmark_(local_benmark) {
        this->local_benmark_->RecordEnter(target_);
    }
    ~BenchmarkScopeRecorder() { this->local_benmark_->RecordExit(target_); }

 private:
    size_t target_;
    std::shared_ptr<BenchMark> local_benmark_;
};

#define AD_LOCAL_BENCHMARK_SCOPE_RECORD(BENCHMARK_TARGET)  \
    BenchmarkScopeRecorder __benchmark_##BENCHMARK_TARGET( \
        AD_LOCAL_BENCHMARK_ID(BENCHMARK_TARGET), AD_LOCAL_BENCHMARK)
#define AD_LOCAL_BENCHMARK_SCOPE_RECORD_STR(BENCHMARK_TAG, BENCHMARK_NAME) \
    BenchmarkScopeRecorder __benchmark_##BENCHMARK_TAG(                    \
        g_LocalBenchmarkStrId[(BENCHMARK_NAME)], AD_LOCAL_BENCHMARK)

#else

#define AD_LOCAL_BENCHMARK
#define AD_LOCAL_BENCHMARK_CREATE(NAME)
#define AD_LOCAL_BENCHMARK_REG(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_ID(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_RECORD_IN(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_RECORD_OUT(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_SCOPE_RECORD(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_REG_STR(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_RECORD_IN_STR(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_RECORD_OUT_STR(BENCHMARK_TARGET)
#define AD_LOCAL_BENCHMARK_SCOPE_RECORD_STR(BENCHMARK_TAG, BENCHMARK_TARGET)

#endif
}  // namespace perception
}  // namespace senseAD
