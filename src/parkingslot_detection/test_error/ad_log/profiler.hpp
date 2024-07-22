/*
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@sensetime.com>
 */

#pragma once

#ifdef USE_NVTX
#define CUDA_ENABLED
#endif

#include <cstdint>
#include <chrono>
#include <vector>
#include <array>
#include <mutex>
#include <memory>
#include <map>
#include <atomic>
#include <string>
#include <functional>
#include <utility>
#include <unordered_map>
#ifdef CUDA_ENABLED
#include <cuda_runtime.h>
#endif

namespace senseAD {
namespace profiler {

class BaseTimer {
 public:
    virtual ~BaseTimer() = default;
    /**
     * @brief Start the Timer
     */
    virtual int64_t Start() = 0;
    /**
     * @brief Start the Timer
     */
    virtual int64_t Observe() const = 0;
    /**
     * @brief Return the time interval started from the Start()
     * Unit: ms
     */
    virtual double CountDuration() const = 0;
};

/**
 * @brief Timer Class for time interval measurement
 */
class Timer : public BaseTimer {
 public:
    explicit Timer(int tid = -1);
    virtual ~Timer() = default;
    int64_t Start() final;
    int64_t Observe() const final;
    double CountDuration() const final;

 private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint begin_time_;
    int tid_;
};

/**
 * @brief Timer Class for cpu time interval measurement
 */
class CpuTimer : public BaseTimer {
 public:
    explicit CpuTimer(int tid = -1);
    int64_t Start() final;
    int64_t Observe() const final;
    double CountDuration() const final;

 private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    int64_t begin_time_;
    int tid_;
};

#ifdef CUDA_ENABLED
class CudaStreamTimer : public BaseTimer {
 public:
    virtual ~CudaStreamTimer() = default;
    explicit CudaStreamTimer(cudaStream_t stream);

    int64_t Start() final;
    /**
     * !Important: It will call sync internally
     */
    double CountDuration() const final;

 private:
    cudaStream_t target_stream_;
    cudaEvent_t begin_, end_;
};
#endif

/**
 * @brief Memory Usage Info. Unit: KB
 */
struct RssInfo {
    // Virtual memory
    double vm_size;
    // Actual used memory
    double resident;
    double shared;
};

class ResourceProfiler {
 public:
    /**
     * @brief Get the Current Memory Usage Info
     *
     * @return RssInfo
     */
    static RssInfo GetCurrentRss();
    /**
     * @brief Get the Processor ID for current thread
     *
     * @return uint8_t
     */
    static uint8_t GetCpuId();
};

// Unit: ns
struct ProfilingStat {
 public:
    double min_ms = 0;
    double max_ms = 0;
    double mean_ms = 0;
    double variance = 0;
    uint64_t sample_cnt = 0;

    void AddSample(double measure_ms);
    void Print() const;
};

class ScopedFuncCall {
 public:
    ScopedFuncCall();
    explicit ScopedFuncCall(std::function<void()>&& at_exit);
    ScopedFuncCall(std::function<void()>&& at_start,
                   std::function<void()>&& at_exit);

    ~ScopedFuncCall();

 private:
    std::function<void()> at_exit_;
};

enum class TimeProfileOutputWhat : uint8_t {
    // Log every arrive
    kTracePointLog = 0,
    // kTracePointFile = 1,
    // Log only when signal trigger or destruction
    kSummaryLog = 2,
    // kSummaryFile = 3,
};

/**
 * @brief Provide Time Comsumption Profile Feature
 *
 *   Disabled by default.
 *   Three way to enable it:
 *    - set `PROFILING` env before run
 *    - manuall call TimeProfiler::Instance().Enable();
 *    - send 34 signal to the process
 */
class TimeProfiler {
 public:
    static TimeProfiler& Instance();

    // Taking the nvtx function name as reference

    /**
     * @brief Start a scoped section
     *
     * @param name section name
     * @return ScopedFuncCall RAII object
     */
    ScopedFuncCall Range(const std::string& name);
    /**
     * @brief Start a profile section
     * !IMPORTANT: Please don't split a section across thread
     * @param name The section name
     */
    void PushSection(const std::string& name);
    /**
     * @brief End the previous started section
     */
    void PopSection(const std::string& name = "");

    /**
     * @brief Start a profile msg pub
     * @param topic_name The pub/sub topic name
     *  @param msg_content The pub/sub msg reference
     */
    template <typename T>
    void PushMsg(const std::string& topic_name, const T& msg_content);
    /**
     * @brief End the previous msg pub
     */
    void PopMsg(const std::string& topic_name);

#ifdef CUDA_ENABLED
    //! We assumed the cude device will be not switched during the section
    ScopedFuncCall CudaRange(cudaStream_t stream, const std::string& name);
    void PushCudaSection(cudaStream_t stream, const std::string& name);
#endif

    void PrintSummary();
    inline void Enable() { enable_ = true; }
    inline void Disable() { enable_ = false; }
    inline bool IsEnabled() { return enable_; }

 private:
    bool enable_ = false;
    int32_t pid_ = -1;
    int time_type_ = 0;  // 0 for counting actual time, 1 for counting cpu time,
    TimeProfileOutputWhat output_content_ =
        TimeProfileOutputWhat::kTracePointLog;

    using ProfilingStatMap = std::map<std::string, ProfilingStat>;
    static constexpr int kExpectedMaxThread = 32;

    std::array<ProfilingStatMap, kExpectedMaxThread> stats_;
    std::array<std::mutex, kExpectedMaxThread> stat_mtxs;

    //* Possible improvement, remove the vector, use circular buffer instead
    std::array<int32_t, kExpectedMaxThread> section_cur_tid_;
    std::array<std::string, kExpectedMaxThread> section_cur_key_;
    std::array<std::string, kExpectedMaxThread> section_cur_func_;
    std::array<std::vector<uint32_t>, kExpectedMaxThread>
        section_keysize_stack_;
    std::array<std::vector<std::unique_ptr<BaseTimer>>, kExpectedMaxThread>
        section_timer_stack_;
    std::array<std::vector<int64_t>, kExpectedMaxThread> section_ts_stack_;

    // rscl msg profile component
    struct MwProfComponent {
        uint32_t msg_size{0};
        uint32_t msg_seq{0};
        std::shared_ptr<BaseTimer> msg_timer = std::make_shared<Timer>();
        MwProfComponent() = default;
        MwProfComponent(uint32_t size, uint32_t seq)
            : msg_size(size), msg_seq(seq) {
            msg_timer->Start();
        }
    };

    std::unordered_map<std::string, MwProfComponent> mw_prof_;

    TimeProfiler();
    void RewindStack(int32_t tid);
    void ThreadWorker();
};

template <typename T>
void TimeProfiler::PushMsg(const std::string& topic_name, const T& reader) {
    if (!enable_) {
        return;
    }
    mw_prof_[topic_name] = MwProfComponent(reader.totalSize().wordCount * 8,
                                           reader.getHeader().getSeq());
}

}  // namespace profiler
}  // namespace senseAD

#define TIME_PROFILE_FORCE_ENABLE() \
    senseAD::profiler::TimeProfiler::Instance().Enable();

#define TIME_PROFILE_RANGE(name) \
    auto _profiler_ =            \
        senseAD::profiler::TimeProfiler::Instance().Range((#name));
#define TIME_PROFILE_PUSH(name) \
    senseAD::profiler::TimeProfiler::Instance().PushSection((#name));
#define TIME_PROFILE_PUSH_UNIQUE() TIME_PROFILE_PUSH(__FILENAME__ + __LINE__)
#define TIME_PROFILE_POP_0_ARGS() \
    senseAD::profiler::TimeProfiler::Instance().PopSection();
#define TIME_PROFILE_POP_1_ARGS(name) \
    senseAD::profiler::TimeProfiler::Instance().PopSection((#name));
#define GET_2ND_ARG(arg1, arg2, FUNC, ...) FUNC
#define TIME_PROFILE_POP(...)                                          \
    GET_2ND_ARG(, ##__VA_ARGS__, TIME_PROFILE_POP_1_ARGS(__VA_ARGS__), \
                TIME_PROFILE_POP_0_ARGS(__VA_ARGS__))
#define TIME_PROFILE_RANGE_STR(name) \
    auto _profiler_ = senseAD::profiler::TimeProfiler::Instance().Range(name);
#define TIME_PROFILE_PUSH_STR(name) \
    senseAD::profiler::TimeProfiler::Instance().PushSection(name);
#define TIME_PROFILE_POP_STR_0_ARGS() \
    senseAD::profiler::TimeProfiler::Instance().PopSection();
#define TIME_PROFILE_POP_STR_1_ARGS(name) \
    senseAD::profiler::TimeProfiler::Instance().PopSection(name);
#define TIME_PROFILE_POP_STR(...)                                          \
    GET_2ND_ARG(, ##__VA_ARGS__, TIME_PROFILE_POP_STR_1_ARGS(__VA_ARGS__), \
                TIME_PROFILE_POP_STR_0_ARGS(__VA_ARGS__))

// TODO(wangxiantao) This should embdeed to rscl publish
#define MSG_PROFILE_PUSH(name, msg) \
    senseAD::profiler::TimeProfiler::Instance().PushMsg(name, msg);
#define MSG_PROFILE_POP(name) \
    senseAD::profiler::TimeProfiler::Instance().PopMsg(name);

#define MSG_PROFILE_TAG(name, msg)                                  \
    senseAD::profiler::TimeProfiler::Instance().PushMsg(name, msg); \
    senseAD::profiler::TimeProfiler::Instance().PopMsg(name);

#define TIME_PROFILE_CUDA_RANGE(name) \
    auto _profiler_ =                 \
        senseAD::profiler::TimeProfiler::Instance().Range((#name));
#define TIME_PROFILE_CUDA_PUSH(name) \
    senseAD::profiler::TimeProfiler::Instance().PushSection((#name));
#define TIME_PROFILE_CUDA_PUSH_UNIQUE() \
    TIME_PROFILE_PUSH(__FILENAME__ + __LINE__)

// https://github.com/facebook/folly/tree/main/folly/tracing
