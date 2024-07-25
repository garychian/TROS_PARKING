/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 */

#pragma once

#include <stdint.h>
#include <time.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include <shared_mutex>

namespace senseAD {
namespace perception {

#ifdef _MSC_VER
#include <WinSock.h>
#include <windows.h>

static int gettimeofday(struct timeval* tp, void* tzp) {
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900U;
    tm.tm_mon = wtm.wMonth - 1U;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000UL;
    return (0);
}
#else
#include <sys/time.h>
#endif
class TimerUtil {
 public:
    static uint64_t GetRealTimeUs() {
        timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000000UL + tv.tv_usec;
    }
};

class BenchmarkItem {
 public:
    uint64_t max_cost;
    uint64_t min_cost;
    int64_t avg_cost;

    uint64_t record_count;
    uint64_t enter;

    std::string name;

    explicit BenchmarkItem(const std::string& target_name) {
        this->max_cost = 0UL;
        this->min_cost = std::numeric_limits<uint64_t>::max();
        this->avg_cost = 0L;
        this->record_count = 0UL;
        this->enter = 0UL;
        this->name = target_name;
    }
};

class BenchMark {
 public:
    BenchMark() : enable(false) {}

    inline size_t AddItem(const std::string& name) {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        this->item.emplace_back(name);
        return this->item.size() - 1;
    }

    inline void Enable() { this->enable = true; }
    inline void Disable() { this->enable = false; }

    inline void RecordEnter(const uint32_t target) {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        this->item[target].enter = TimerUtil::GetRealTimeUs();
    }
    inline void RecordExit(const uint32_t target) {
        if (!this->enable) {
            return;
        }
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        uint64_t cur = TimerUtil::GetRealTimeUs() - this->item[target].enter;
        BenchmarkItem& it = this->item[target];
        it.record_count++;
        it.avg_cost += (static_cast<int64_t>(cur) - it.avg_cost) /
                       static_cast<int64_t>(it.record_count);
        it.max_cost = std::max(it.max_cost, cur);
        it.min_cost = std::min(it.min_cost, cur);
    }

    inline const std::vector<BenchmarkItem>& GetData() { return this->item; }

    inline void Reset() {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        for (auto& benchdata : item) {
            benchdata.max_cost = 0UL;
            benchdata.min_cost = std::numeric_limits<uint64_t>::max();
            benchdata.avg_cost = 0L;
            benchdata.record_count = 0UL;
            benchdata.enter = 0UL;
        }
    }

 private:
    std::vector<BenchmarkItem> item;
    bool enable;
    std::shared_timed_mutex mutex_;
};

enum class BenchmarkExcept : uint32_t {
    BENCHMARK_EXCEPT_INVALID_TARGET_NUM,
    BENCHMARK_EXCEPT_TARGET_ALREADY_EXIST
};

using BenchMarkLists = std::map<std::string, std::shared_ptr<BenchMark> >;

inline bool StartWithStr(const std::string& src_str,
                         const std::string& target_str) {
    if (target_str.size() > src_str.size()) {
        return false;
    }

    for (size_t idx = 0; idx < target_str.size(); ++idx) {
        if (target_str.at(idx) != src_str.at(idx)) {
            return false;
        }
    }
    return true;
}

inline bool EndWithStr(const std::string& src_str,
                       const std::string& target_str) {
    if (target_str.size() > src_str.size()) {
        return false;
    }

    size_t ts = target_str.size(), ss = src_str.size();
    for (size_t idx = 1; idx <= ts; ++idx) {
        if (target_str.at(ts - idx) != src_str.at(ss - idx)) {
            return false;
        }
    }
    return true;
}

struct ReportTag {
    ReportTag(const std::string& i, float v = 0.0) : item(i), value(v) {}
    ~ReportTag() {}
    std::string item;
    float value;
};

struct AvpRDReport {
    std::string tag0 = "CVProcess";
    ReportTag cvprocess = ReportTag("Process");
    ReportTag stitching = ReportTag("Stitching");
    ReportTag resize = ReportTag("Resize");
    ReportTag convertto = ReportTag("ConvertTo");
    ReportTag channel_split = ReportTag("ChannelSplit");

    std::string tag1 = "NNForward";
    ReportTag nnforward = ReportTag("");
    ReportTag copyin = ReportTag("PrepareInput");
    ReportTag nn = ReportTag("Forward");
    ReportTag copyout = ReportTag("PrepareOutput");

    std::string tag2 = "ParkingSlot";
    ReportTag parking_slot = ReportTag("Process");

    std::string tag3 = "MMLightSeg";
    ReportTag argmax = ReportTag("Argmax");
    ReportTag mm_resize = ReportTag("Resize");
    ReportTag mm_expand_image = ReportTag("ExpandImage");
    ReportTag mmlightseg = ReportTag("Process");

    ReportTag pipeline = ReportTag("ALL");

    void SetPipelineTime(float f) { pipeline.value = f; }

    void Set(BenchMarkLists& benc) {
        for (auto& b : benc) {
            auto& func_benchmark = b.second->GetData();

            if (tag0 == b.first) {
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == cvprocess.item) cvprocess.value = v;
                    if (EndWithStr(n, stitching.item)) stitching.value = v;
                    if (EndWithStr(n, resize.item)) resize.value = v;
                    if (EndWithStr(n, convertto.item)) convertto.value = v;
                    if (EndWithStr(n, channel_split.item))
                        channel_split.value = v;
                }
            }
            if (tag1 == b.first) {
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == copyin.item) copyin.value = v;
                    if (n == copyout.item) copyout.value = v;
                    if (n == nn.item) nn.value = v;
                }
                nnforward.value = copyin.value + copyout.value + nn.value;
            }
            if ((tag2 == b.first)) {
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == parking_slot.item) parking_slot.value += v;
                }
            }
            if (StartWithStr(b.first, tag3)) {
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == argmax.item) argmax.value = v;
                    if (n == mm_resize.item) mm_resize.value = v;
                    if (n == mm_expand_image.item) mm_expand_image.value = v;
                    if (n == mmlightseg.item) mmlightseg.value = v;
                }
            }
        }
    }

    std::string output() {
        size_t buf_len = 1024 * 2;
        char buffer[buf_len];
        size_t newlen = snprintf(
            buffer, buf_len,
            "算法, Operator, Kernel,耗时(n路) \n"
            "RD, CVProcess, IPMSitching, %f\n"
            ", , resize, %f\n"
            ", , ConvertTo, %f\n"
            ", , ChannelSplit, %f\n"
            ", , ALL, %f\n"
            ", NNForward, PrepareInput, %f\n"
            ", , Forward, %f\n"
            ", , PrepareOutput, %f\n"
            ", , ALL, %f\n"
            ", ParkingSlot, ALL, %f\n"
            ", MMLightSeg, argmax, %f\n"
            ", , resize, %f\n"
            ", , ExpandImage, %f\n"
            ", , ALL, %f\n"
            ", ALL, , %f\n",

            stitching.value, resize.value, convertto.value, channel_split.value,
            cvprocess.value, copyin.value, nn.value, copyout.value,
            nnforward.value, parking_slot.value, argmax.value, mm_resize.value,
            mm_expand_image.value, mmlightseg.value, pipeline.value);

        std::string ret(newlen, 0);
        memcpy((char*)ret.c_str(), buffer, newlen);

        return ret;
    }
};

struct AvpODReport {
    std::string tag0 = "CVProcess";
    ReportTag cvprocess = ReportTag("Process");
#if defined(USE_CUDA)
    ReportTag crop = ReportTag("CudaOpCrop");
    ReportTag resize = ReportTag("CudaOpResize");
    ReportTag convertto = ReportTag("CudaOpConvertTo");
#elif defined USE_ARM
    ReportTag crop = ReportTag("WispOpCrop");
    ReportTag resize = ReportTag("WispOpResize");
    ReportTag arm_crop_resize = ReportTag("ArmOpCropResize");
    ReportTag convertto = ReportTag("WispOpConvertTo");
#else
    ReportTag crop = ReportTag("WispOpCrop");
    ReportTag resize = ReportTag("WispOpResize");
    ReportTag convertto = ReportTag("WispOpConvertTo");
#endif

    std::string tag1 = "NNForward";
    ReportTag nnforward = ReportTag("");
    ReportTag copyin = ReportTag("PrepareInput");
    ReportTag nn = ReportTag("Forward");
    ReportTag copyout = ReportTag("PrepareOutput");

    // std::string tag2 = "MMDetect_Pose";
    // ReportTag mmdetect = ReportTag("DetectHeadPose_GetBboxes0_Once");
    // ReportTag topk = ReportTag("DetectHeadPose_Topk_Once");
    // ReportTag gropuing = ReportTag("DetectHeadPose_Gropuing_Once");
    // ReportTag refine = ReportTag("DetectHeadPose_GetBboxes1_RefineOnce");
    std::string tag2 = "MMDetect";
    ReportTag mmdetect = ReportTag("Process");
    ReportTag prior = ReportTag("PriorGenerate");
    ReportTag bboxdecode = ReportTag("BboxDecode");
    ReportTag getbbox = ReportTag("GetBboxes");
    ReportTag bboxnms = ReportTag("BboxNms");
    ReportTag bbox3dnms = ReportTag("Bbox3DNms");

    ReportTag pipeline = ReportTag("ALL");

    void SetPipelineTime(float f) { pipeline.value = f; }

    void Set(BenchMarkLists& benc) {
        for (auto& b : benc) {
            auto& func_benchmark = b.second->GetData();

            if (tag0 == b.first) {
                int cvprocess_scale = 2;
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == cvprocess.item) cvprocess.value = v * 2;
                    if (n == crop.item) crop.value = v * 4;
                    if (n == resize.item) resize.value = v;
#ifdef USE_ARM
                    if (n == arm_crop_resize.item) {
                        arm_crop_resize.value = v * 4;
                        cvprocess_scale = 1;
                        if (cvprocess.value) cvprocess.value /= 2;
                    }
#endif
                    if (n == convertto.item) convertto.value = v;
                }
            }
            if (tag1 == b.first) {
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == copyin.item) copyin.value = v;
                    if (n == copyout.item) copyout.value = v;
                    if (n == nn.item) nn.value = v;
                }
                nnforward.value = copyin.value + copyout.value + nn.value;
            }
            if (StartWithStr(b.first, tag2)) {
                for (auto& r : func_benchmark) {
                    if (!r.record_count) continue;
                    const auto n = r.name;
                    const auto v = r.avg_cost / 1000.0;
                    if (n == prior.item) prior.value = v;
                    if (n == bboxdecode.item) bboxdecode.value = v;
                    if (n == getbbox.item) getbbox.value = v;
                    if (n == bboxnms.item) bboxnms.value = v;
                    if (n == bbox3dnms.item) bbox3dnms.value = v;
                    if (n == mmdetect.item) mmdetect.value = v;
                }
            }
        }
    }

    std::string output() {
        size_t buf_len = 1024 * 2;
        char buffer[buf_len];
        size_t newlen = snprintf(
            buffer, buf_len,
            "算法,     Operator,    Kernel,耗时 \n OD, CVProcess, "
            "crop, "
            "% f\n, , resize(nearest), % f\n, , "
#ifdef USE_ARM
            "arm_crop_resize(nearest), %f\n, , "
#endif
            "convertto, % f\n, , ALL, % "
            "f\n, NNForward, copy input, % f\n, , nn, % f\n, , copy "
            "output, % "
            "f\n, , ALL, % f\n, MMDetect, PriorGenerate, % f\n, , BboxDecode, "
            "% f\n, "
            ", "
            "GetBboxes, %f\n, , BboxNms, % f\n, , Bbox3dNms, % f\n, , ALL, %f "
            "\n,ALL, , % f\n ",
            crop.value, resize.value,
#ifdef USE_ARM
            arm_crop_resize.value,
#endif
            convertto.value, cvprocess.value, copyin.value, nn.value,
            copyout.value, nnforward.value, prior.value, bboxdecode.value,
            getbbox.value, bboxnms.value, bbox3dnms.value, mmdetect.value,
            pipeline.value);

        std::string ret(newlen, 0);
        memcpy((char*)ret.c_str(), buffer, newlen);

        return ret;
    }
};

class BenchMarkInstance {
 public:
    static std::shared_ptr<BenchMark>& CreateBenchmark(
        const std::string& benchmark_name) {
        BenchMarkInstance* instance = BenchMarkInstance::GetInstance();
        std::string new_benchmark_name = benchmark_name;
        int suffix = 0;
        while (instance->benchmarks.find(new_benchmark_name) !=
               instance->benchmarks.end()) {
            new_benchmark_name =
                benchmark_name + "_" + std::to_string(suffix++);
        }
        instance->benchmarks[new_benchmark_name] =
            std::make_shared<BenchMark>();

        std::shared_ptr<BenchMark>& ret =
            instance->benchmarks[new_benchmark_name];
        ret->Enable();
        return ret;
    }

    static const std::map<std::string, std::shared_ptr<BenchMark> >&
    GetBenchmarks() {
        return BenchMarkInstance::GetInstance()->benchmarks;
    }

    static void ResetBenchmark() {
        auto& all_benchmarks = BenchMarkInstance::GetInstance()->benchmarks;
        for (auto& benchmark : all_benchmarks) {
            if (benchmark.second != nullptr) {
                benchmark.second->Reset();
            }
        }
    }

    static bool GetBenchMarkReportRD(std::string file_prefix = "",
                                     std::string file_suffix = "",
                                     float pipeline_time = 0.0) {
        auto GetNowTime = []() -> std::string {
            char buf[30] = {0};
            time_t t = time(nullptr);
            struct tm local;
            localtime_r(&t, &local);
            strftime(buf, 30, "%Y-%m-%d-%H-%M-%S", &local);
            return std::string(buf);
        };

        // auto& all_benchmarks = BenchMarkInstance::GetInstance()->benchmarks;

        std::string file_name =
            file_suffix.empty()
                ? file_prefix + "_benchmark_" + GetNowTime() + ".csv"
                : file_prefix + "_benchmark_" + file_suffix + ".csv";
        std::ofstream benchmark_file(file_name.c_str(),
                                     std::ios_base::trunc | std::ios_base::out);
        if (!benchmark_file.is_open()) {
            std::cerr << "save benchmark failed: " << file_name << std::endl;
            return false;
        }

        AvpRDReport rep;
        rep.Set(BenchMarkInstance::GetInstance()->benchmarks);
        rep.SetPipelineTime(pipeline_time);
        benchmark_file << rep.output();

        return true;
    }

    static bool GetBenchMarkReportOD(std::string file_prefix = "",
                                     std::string file_suffix = "",
                                     float pipeline_time = 0.0) {
        auto GetNowTime = []() -> std::string {
            char buf[30] = {0};
            time_t t = time(nullptr);
            struct tm local;
            localtime_r(&t, &local);
            strftime(buf, 30, "%Y-%m-%d-%H-%M-%S", &local);
            return std::string(buf);
        };

        // auto& all_benchmarks = BenchMarkInstance::GetInstance()->benchmarks;

        std::string file_name =
            file_suffix.empty()
                ? file_prefix + "_benchmark_" + GetNowTime() + ".csv"
                : file_prefix + "_benchmark_" + file_suffix + ".csv";
        std::ofstream benchmark_file(file_name.c_str(),
                                     std::ios_base::trunc | std::ios_base::out);
        if (!benchmark_file.is_open()) {
            std::cerr << "save benchmark failed: " << file_name << std::endl;
            return false;
        }

        AvpODReport odrep;
        odrep.Set(BenchMarkInstance::GetInstance()->benchmarks);
        odrep.SetPipelineTime(pipeline_time);
        benchmark_file << odrep.output();

        return true;
    }

    static bool GetBenchMarkReport(std::string file_prefix = "",
                                   std::string file_suffix = "") {
        auto GetNowTime = []() -> std::string {
            char buf[30] = {0};
            time_t t = time(nullptr);
            struct tm local;
            localtime_r(&t, &local);
            strftime(buf, 30, "%Y-%m-%d-%H-%M-%S", &local);
            return std::string(buf);
        };

        auto& all_benchmarks = BenchMarkInstance::GetInstance()->benchmarks;

        std::string file_name =
            file_suffix.empty()
                ? file_prefix + "_benchmark_" + GetNowTime() + ".csv"
                : file_prefix + "_benchmark_" + file_suffix + ".csv";
        std::ofstream benchmark_file(file_name.c_str(),
                                     std::ios_base::trunc | std::ios_base::out);
        if (!benchmark_file.is_open()) {
            std::cerr << "save benchmark failed: " << file_name << std::endl;
            return false;
        }

        benchmark_file << "Target,Avg(ms),Max(ms),Min(ms),Times" << std::endl;
        for (auto& b : all_benchmarks) {
            auto& func_benchmark = b.second->GetData();
            bool bypass = all_of(func_benchmark.begin(), func_benchmark.end(),
                                 [&](const BenchmarkItem& item) {
                                     return item.record_count == 0;
                                 });
            if (bypass) continue;

            benchmark_file << std::endl
                           << std::endl
                           << "Benchmark Name: " << b.first << std::endl;
            for (auto& r : func_benchmark) {
                if (r.record_count == 0) continue;
                float min = (std::numeric_limits<uint64_t>::max() == r.min_cost)
                                ? 0.0f
                                : static_cast<float>(r.min_cost) / 1000.0f;
                float max = static_cast<float>(r.max_cost) / 1000.0f;
                float avg = static_cast<float>(r.avg_cost) / 1000.0f;
                benchmark_file
                    << r.name << "," << std::fixed << std::setprecision(3)
                    << avg << "," << std::fixed << std::setprecision(3) << max
                    << "," << std::fixed << std::setprecision(3) << min << ","
                    << r.record_count << std::endl;
            }
        }
        return true;
    }

 public:
    std::map<std::string, std::shared_ptr<BenchMark> > benchmarks;
    static BenchMarkInstance* GetInstance() {
        static BenchMarkInstance* instance = new BenchMarkInstance();
        return instance;
    }
};

}  // namespace perception
}  // namespace senseAD
