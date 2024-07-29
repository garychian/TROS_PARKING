#ifndef TEST_OCCUPANCY_HPP_
#define TEST_OCCUPANCY_HPP_

#include <atomic>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <cmath>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>
#if defined(__ANDROID__) || defined(__linux__)
#include "unistd.h"

#include <sched.h>
#include <sstream>
#include <sys/syscall.h>
#endif

class OccupancyTimer {
    decltype(std::chrono::high_resolution_clock::now()) start_time;

 public:
    OccupancyTimer() : start_time(std::chrono::high_resolution_clock::now()) {}
    ~OccupancyTimer() {}
    template <class T>
    uint32_t GetDuration() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<T>(now - start_time).count();
    }
};

typedef class ProcStat {
 public:
    void CalSum() {
        total = user + nice + system + idle + iowait + irq + softirq +
                stealstolen + guest;
    }
    char name[20];
    unsigned long user{0};
    unsigned long nice{0};
    unsigned long system{0};
    unsigned long idle{0};
    unsigned long iowait{0};
    unsigned long irq{0};
    unsigned long softirq{0};
    unsigned long stealstolen{0};
    unsigned long guest{0};
    unsigned long total{0};
} ProcStat;

typedef class ProcPidStat {
 public:
    void CalSum() { total = utime + stime + cutime + cstime; }
    unsigned long utime{0};
    unsigned long stime{0};
    unsigned long cutime{0};
    unsigned long cstime{0};
    unsigned long total{0};
} ProcPidStat;

typedef class OccupancyStat {
 public:
    OccupancyStat() {
        const int history_len = 500;
        record_history_ = true;
        all_cpu_history_.reserve(history_len);
        bind_cpu_history_.reserve(history_len);
        frame_history_.reserve(history_len);
        mem_history_.reserve(history_len);
    }

    ProcPidStat cur_pid_cpu_stat;
    ProcPidStat last_pid_cpu_stat;
    ProcStat cur_all_cpu_stat;
    ProcStat last_all_cpu_stat;
    ProcStat cur_bind_cpu_stat;
    ProcStat last_bind_cpu_stat;

    float avg_init_time{0};
    uint32_t call_init_times{0};

    float max_frame_time{std::numeric_limits<float>::lowest()};
    float min_frame_time{std::numeric_limits<float>::max()};
    float avg_frame_time{0.0f};
    float cur_frame_time{0.0f};
    uint32_t last_frame_times{0};
    uint32_t call_frame_times{0};

    float frame_fps{0.0};

    float cur_all_cpu_occupancy{0.0f};
    float max_all_cpu_occupancy{0.0f};
    float avg_all_cpu_occupancy{0.0f};

    float cur_bind_cpu_occupancy{0.0f};
    float max_bind_cpu_occupancy{0.0f};
    float avg_bind_cpu_occupancy{0.0f};

    float cur_gpu_occupancy{0.0f};
    float max_gpu_occupancy{0.0f};
    float avg_gpu_occupancy{0.0f};

    float cur_mem_occupancy{0.0f};
    float max_mem_occupancy{0.0f};
    float avg_mem_occupancy{0.0f};

    float cur_pid_jiffies{0.0f};
    float max_pid_jiffies{0.0f};
    float avg_pid_jiffies{0.0f};

    int cpu_count{0};
    int gpu_count{0};
    int mem_count{0};

    std::vector<int> cpu_used_;

    bool record_history_;
    std::vector<float> all_cpu_history_;
    // SD: StandardDeviation
    float all_cpu_sd_{0.0f};
    std::vector<float> bind_cpu_history_;
    float bind_cpu_sd_{0.0f};
    std::vector<float> frame_history_;
    float frame_sd_{0.0f};
    std::vector<float> mem_history_;
    float mem_sd_{0.0f};

} OccupancyStat;

class Occupancy {
 public:
    Occupancy()
        : is_enabled_{true},
          thread_{},
          sample_time_{1000},
          thread_loop_time_ms_{30},
          start_time_(),
          target_fps_(10.0) {
        // memset(&statistics_, 0, sizeof(OccupancyStat));
    }
    ~Occupancy() {
        is_enabled_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    float GetPipelineTime() { return statistics_.avg_frame_time; }
    void Start(int const &sample_time = 1000) {  // sample 1000ms
        if (thread_.joinable()) {
            return;
        }
        is_enabled_ = true;
        sample_time_ = sample_time;
        start_time_ = std::chrono::high_resolution_clock::now();
        thread_ = std::thread(&Occupancy::OccupancyThread, this);
    }

    void Stop() { is_enabled_ = false; }

    void SetRefreshRate(int64_t ms) { thread_loop_time_ms_ = ms; }

    void writeOccupancy(std::ofstream &f,
                        Occupancy &o,
                        float avg_time_cost = 0) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto &s = statistics_;

        f << s.avg_mem_occupancy << "," << s.avg_bind_cpu_occupancy << ","
          << s.avg_gpu_occupancy << "," << (float)avg_time_cost / 1000
          << std::endl;
    }

    inline OccupancyStat GetStatistics() {
        std::lock_guard<std::mutex> lock(mutex_);
        return statistics_;
    }

    inline void SetTargetFps(double fps) { target_fps_ = fps; }

    inline void ShowOccupancy(Occupancy &o,
                              const std::vector<std::string> &filenames,
                              float avg_time_cost = 0) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto &s = statistics_;
        std::stringstream ss;

        if (!filenames.empty()) {
            double size = 0.0f;
            for (auto filename : filenames) {
                auto file = fopen(filename.c_str(), "rb");
                if (!file) {
                    std::cerr << "[ERROR] open image file: " << filename
                              << " failed." << std::endl;
                }
                fseek(file, 0, SEEK_END);
                size += (double)ftell(file) / 1048576;
                fclose(file);
            }
            ss << "        [TEST] sample interval : " << sample_time_ << "ms"
               << std::endl;
            ss << "        [TEST] model file size : " << std::fixed
               << std::setprecision(2) << size << "MB" << std::endl;
        }
        ss << "        [TEST] avg pid jiffies : " << std::fixed
           << std::setprecision(2) << s.avg_pid_jiffies << "tick" << std::endl;
        ss << "        [TEST] avg cpu occupy(all)  : " << std::fixed
           << std::setprecision(2) << s.avg_all_cpu_occupancy << "%"
           << std::endl;
        ss << "        [TEST] avg cpu occupy(bind) : " << std::fixed
           << std::setprecision(2) << s.avg_bind_cpu_occupancy << "%"
           << std::endl;
        ss << "        [TEST] avg gpu occupy : " << std::fixed
           << std::setprecision(2) << s.avg_gpu_occupancy << "%" << std::endl;
        ss << "        [TEST] avg mem occupy : " << std::fixed
           << std::setprecision(2) << s.avg_mem_occupancy << "MB" << std::endl;

        if (avg_time_cost != 0) {
            ss << "        [TEST] avg time cost  : " << std::fixed
               << std::setprecision(2) << avg_time_cost << "ms" << std::endl;
        }

        std::cout << ss.str() << std::endl;
    }

    inline bool GetOccupancyReport(std::string file_prefix = "",
                                   std::string file_suffix = "",
                                   float avg_time_cost = 0,
                                   uint32_t times = 0) {
        OccupancyCpu(*this);
        CalculatorSD(*this);
        std::lock_guard<std::mutex> lock(mutex_);
        auto &s = statistics_;

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
                ? file_prefix + "_occupancy_" + GetNowTime() + ".csv"
                : file_prefix + "_occupancy_" + file_suffix + ".csv";
        std::ofstream output_file(file_name.c_str(),
                                  std::ios_base::trunc | std::ios_base::out);
        if (!output_file.is_open()) {
            std::cerr << "save occupancy failed: " << file_name << std::endl;
            return false;
        }

        output_file << "算法性能汇总：测试设定 FPS=" << target_fps_ << ","
                    << std::endl;
        output_file << "模型版本,软件版本,算法耗时/ms,单CPU占用%,内存占用/"
                       "MB,磁盘空间/MB,初始化耗时/ms,FPS"
                    << std::endl;
        output_file << model_file_name_ << "," << soft_version_ << ","
                    << s.avg_frame_time << "," << s.avg_bind_cpu_occupancy
                    << "/" << s.avg_all_cpu_occupancy << ","
                    << s.avg_mem_occupancy << ","
                    << "---"
                    << "," << s.avg_init_time << "," << s.frame_fps
                    << std::endl;

        if (s.record_history_) {
            output_file << "ALL CPU    SD," << s.all_cpu_sd_ << std::endl;
            output_file << "BIND CPU   SD," << s.bind_cpu_sd_ << std::endl;
            output_file << "DDR MEM    SD," << s.mem_sd_ << std::endl;
            output_file << "FRAME TIME SD," << s.frame_sd_ << std::endl;

            auto vec_to_csv_str =
                [](const std::vector<float> &v) -> std::string {
                std::stringstream ss;
                std::for_each(v.begin(), v.end(),
                              [&](float f) { ss << f << ","; });
                return ss.str();
            };
            output_file << "ALL CPU    HISTORY,"
                        << vec_to_csv_str(s.all_cpu_history_) << std::endl;
            output_file << "BIND CPU   HISTORY,"
                        << vec_to_csv_str(s.bind_cpu_history_) << std::endl;
            output_file << "DDR MEM    HISTORY,"
                        << vec_to_csv_str(s.mem_history_) << std::endl;
            output_file << "FRAME TIME HISTORY,"
                        << vec_to_csv_str(s.frame_history_) << std::endl;
        }

        return true;
    }

    inline OccupancyTimer StartOneFrameTime() { return {}; }
    // return ms
    inline float EndOneFrameTime(const OccupancyTimer &timer) {
        auto duration = timer.GetDuration<std::chrono::microseconds>() / 1000.0;
        auto &times = statistics_.call_frame_times;
        auto &max = statistics_.max_frame_time;
        auto &avg = statistics_.avg_frame_time;
        auto &min = statistics_.min_frame_time;
        auto &cur = statistics_.cur_frame_time;

        ++times;
        max = max > duration ? max : duration;
        min = min < duration ? min : duration;
        avg += (duration - avg) / times;
        cur = duration;

        auto &fps = statistics_.frame_fps;
        auto total =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start_time_)
                .count();
        fps = (1000.0 * 1000.0 / total) * times;

        OccupancyFrame(*this);

        return duration;
    }
    inline OccupancyTimer StartOneInitTime() { return {}; }
    inline float EndOneInitTime(const OccupancyTimer &timer) {
        auto duration = timer.GetDuration<std::chrono::microseconds>() / 1000.0;
        auto &times = statistics_.call_init_times;
        auto &avg = statistics_.avg_init_time;

        ++times;
        avg += (duration - avg) / times;

        return avg;
    }
    inline void SetModelFileName(const std::string &model) {
        model_file_name_ = model;
    }
    inline void SetSoftVersion(const std::string &ver) { soft_version_ = ver; }

 private:
    template <typename T>
    static T Variance(const std::vector<T> &vec) {
        const size_t sz = vec.size();
        if (sz == 1) {
            return 0.0;
        }

        // Calculate the mean
        const T mean = std::accumulate(vec.begin(), vec.end(), 0.0) / sz;

        // Now calculate the variance
        auto variance_func = [&mean, &sz](T accumulator, const T &val) {
            return accumulator + ((val - mean) * (val - mean) / (sz - 1));
        };

        return std::accumulate(vec.begin(), vec.end(), 0.0, variance_func);
    }

    template <typename T>
    static T StandardDeviation(const std::vector<T> &vec) {
        return sqrt(Variance(vec));
    }

    static void CalculatorSD(Occupancy &o) {
        auto &s = o.statistics_;
        s.all_cpu_sd_ = StandardDeviation(s.all_cpu_history_);
        s.bind_cpu_sd_ = StandardDeviation(s.bind_cpu_history_);
        s.mem_sd_ = StandardDeviation(s.mem_history_);
        s.frame_sd_ = StandardDeviation(s.frame_history_);
    }

    static bool GetPidJiffies(Occupancy &o) {
        auto &s = o.statistics_;
        FILE *pid_cpu_stat_file = fopen("/proc/self/stat", "r");
        if (nullptr == pid_cpu_stat_file) {
            std::cout << "[ERROR] Command fopen /proc/self/stat failed"
                      << std::endl;
            return false;
        }
        std::lock_guard<std::mutex> lock(o.mutex_);
        if (0 > fscanf(pid_cpu_stat_file,
                       "%*s %*s %*c %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s "
                       "%ld %ld %ld %ld",
                       &s.cur_pid_cpu_stat.utime, &s.cur_pid_cpu_stat.stime,
                       &s.cur_pid_cpu_stat.cutime,
                       &s.cur_pid_cpu_stat.cstime)) {
            std::cout << "[ERROR] Command fscanf pid_cpu_stat failed"
                      << std::endl;
            return false;
        }
        fclose(pid_cpu_stat_file);
        return true;
    }

    static bool GetCpuJiffies(Occupancy &o) {
        FILE *all_cpu_stat_file = fopen("/proc/stat", "r");
        if (nullptr == all_cpu_stat_file) {
            std::cout << "[ERROR] /proc/stat" << std::endl;
            return false;
        }

        char line[512];
        std::vector<ProcStat> ProcStatVec;
        while (NULL != fgets(line, sizeof(line), all_cpu_stat_file)) {
            if (0 == strncmp(line, "cpu", 3)) {
                ProcStatVec.resize(ProcStatVec.size() + 1);
                sscanf(line, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                       ProcStatVec.back().name, &(ProcStatVec.back().user),
                       &(ProcStatVec.back().nice), &(ProcStatVec.back().system),
                       &(ProcStatVec.back().idle), &(ProcStatVec.back().iowait),
                       &(ProcStatVec.back().irq), &(ProcStatVec.back().softirq),
                       &(ProcStatVec.back().stealstolen),
                       &(ProcStatVec.back().guest));
            }
        }

        fclose(all_cpu_stat_file);
        if (!GetAllCpuJiffies(ProcStatVec, o)) {
            std::cout << "[ERROR] GetAllCpuJiffies" << std::endl;
            return false;
        }
        if (!GetBindCpuJiffies(ProcStatVec, o)) {
            std::cout << "[ERROR] GetBindCpuJiffies" << std::endl;
            return false;
        }
        return true;
    }

    static bool GetAllCpuJiffies(std::vector<ProcStat> &proc_stat_vec,
                                 Occupancy &o) {
        auto &s = o.statistics_;
        std::lock_guard<std::mutex> lock(o.mutex_);
        s.cur_all_cpu_stat = proc_stat_vec[0];
        return true;
    }

    static bool GetBindCpuJiffies(std::vector<ProcStat> &proc_stat_vec,
                                  Occupancy &o) {
// if you bind core by taskset or sched_setaffinity,the cpu_used will be the
// core you set if not,the core number will be the all online cpu number
#if defined(__ANDROID__) || defined(__linux__)
        auto &s = o.statistics_;
        if (s.cpu_count == 1) {
            cpu_set_t cs;
            CPU_ZERO(&cs);
            sched_getaffinity(getpid(), sizeof(cs), &cs);

            for (int core_idx = 0; core_idx < proc_stat_vec.size();
                 core_idx++) {
                if (CPU_ISSET(core_idx, &cs)) {
                    s.cpu_used_.push_back(core_idx);
                }
            }
            std::cout << "    pid: " << getpid() << " bind cpu is CPU";
            for (auto id : s.cpu_used_) {
                std::cout << std::to_string(id) << ",";
            }
            std::cout << "if that diff with your set of <taskset>, mean that "
                         "some cpu bind failure"
                      << std::endl;
        }

        memset(&s.cur_bind_cpu_stat, 0, sizeof(s.cur_bind_cpu_stat));

        for (auto id : s.cpu_used_) {
            if (id + 1 >= proc_stat_vec.size()) {
                std::cout << "[ERROR] size error " << id << " "
                          << proc_stat_vec.size() << std::endl;
                return false;
            }
            std::lock_guard<std::mutex> lock(o.mutex_);
            s.cur_bind_cpu_stat.user += proc_stat_vec[id + 1].user;
            s.cur_bind_cpu_stat.nice += proc_stat_vec[id + 1].nice;
            s.cur_bind_cpu_stat.system += proc_stat_vec[id + 1].system;
            s.cur_bind_cpu_stat.idle += proc_stat_vec[id + 1].idle;
            s.cur_bind_cpu_stat.iowait += proc_stat_vec[id + 1].iowait;
            s.cur_bind_cpu_stat.irq += proc_stat_vec[id + 1].irq;
            s.cur_bind_cpu_stat.softirq += proc_stat_vec[id + 1].softirq;
            s.cur_bind_cpu_stat.stealstolen +=
                proc_stat_vec[id + 1].stealstolen;
            s.cur_bind_cpu_stat.guest += proc_stat_vec[id + 1].guest;
        }
#endif
        return true;
    }

    static bool OccupancyFrame(Occupancy &o) {
        auto &s = o.statistics_;
        if (s.record_history_) {
            s.frame_history_.push_back(s.cur_frame_time);
        }
        return true;
    }

    static bool OccupancyCpu(Occupancy &o) {
        auto &s = o.statistics_;
        s.cpu_count++;

        if (!GetPidJiffies(o)) {
            std::cout << "[ERROR] GetPidJiffies" << std::endl;
            return false;
        }
        if (!GetCpuJiffies(o)) {
            std::cout << "[ERROR] GetCpuJiffies" << std::endl;
            return false;
        }

        {
            std::lock_guard<std::mutex> lock(o.mutex_);
            s.cur_pid_cpu_stat.CalSum();
            s.cur_bind_cpu_stat.CalSum();
            s.cur_all_cpu_stat.CalSum();

            if (s.cpu_count > 1) {
                auto delta_all_cpu_stat = s.cur_all_cpu_stat.total -
                                          s.last_all_cpu_stat.total +
                                          std::numeric_limits<float>::min();
                auto delta_bind_cpu_stat = s.cur_bind_cpu_stat.total -
                                           s.last_bind_cpu_stat.total +
                                           std::numeric_limits<float>::min();

                s.cur_pid_jiffies = s.cur_pid_cpu_stat.total -
                                    s.last_pid_cpu_stat.total +
                                    std::numeric_limits<float>::min();
                s.max_pid_jiffies =
                    std::max(s.max_pid_jiffies, s.cur_pid_jiffies);
                s.avg_pid_jiffies +=
                    (s.cur_pid_jiffies - s.avg_pid_jiffies) / (s.cpu_count - 1);

                s.cur_all_cpu_occupancy =
                    s.cur_pid_jiffies / delta_all_cpu_stat * 100;
                s.max_all_cpu_occupancy =
                    std::max(s.max_all_cpu_occupancy, s.cur_all_cpu_occupancy);
                s.avg_all_cpu_occupancy +=
                    (s.cur_all_cpu_occupancy - s.avg_all_cpu_occupancy) /
                    (s.cpu_count - 1);

                s.cur_bind_cpu_occupancy = s.cur_pid_jiffies /
                                           delta_bind_cpu_stat *
                                           s.cpu_used_.size() * 100;
                s.max_bind_cpu_occupancy = std::max(s.max_bind_cpu_occupancy,
                                                    s.cur_bind_cpu_occupancy);
                s.avg_bind_cpu_occupancy +=
                    (s.cur_bind_cpu_occupancy - s.avg_bind_cpu_occupancy) /
                    (s.cpu_count - 1);
                if (s.record_history_) {
                    s.bind_cpu_history_.push_back(s.cur_bind_cpu_occupancy);
                    s.all_cpu_history_.push_back(s.cur_all_cpu_occupancy);
                }
            }
            s.last_pid_cpu_stat = s.cur_pid_cpu_stat;
            s.last_all_cpu_stat = s.cur_all_cpu_stat;
            s.last_bind_cpu_stat = s.cur_bind_cpu_stat;
        }
        return true;
    }

    static bool OccupancyMem(Occupancy &o) {
        auto &s = o.statistics_;
        s.mem_count++;

        char line[128];
        auto file = fopen("/proc/self/status", "r");
        if (nullptr == file) {
            std::cout << "[ERROR] Command fopen mem failed" << std::endl;
            return false;
        }
        while (NULL != fgets(line, sizeof(line), file)) {
            if (0 == strncmp(line, "VmRSS:", 6)) {
                std::lock_guard<std::mutex> lock(o.mutex_);
                sscanf(line, "%*s %f", &s.cur_mem_occupancy);
                s.cur_mem_occupancy /= 1024.;
                s.max_mem_occupancy =
                    std::max(s.max_mem_occupancy, s.cur_mem_occupancy);
                s.avg_mem_occupancy +=
                    (s.cur_mem_occupancy - s.avg_mem_occupancy) / s.mem_count;
                break;
            }
        }
        if (s.record_history_) {
            s.mem_history_.push_back(s.cur_mem_occupancy);
        }

        fclose(file);
        return true;
    }
    static bool OccupancyGpu(Occupancy &o) {
        auto &s = o.statistics_;
        s.gpu_count++;
#if defined(__ANDROID__)
        char line[128];
        int gpu_cur, gpu_all;

        auto file = fopen("/sys/class/kgsl/kgsl-3d0/gpubusy", "r");
        if (nullptr == file) {
            std::cout << "[ERROR] Command fopen gpu failed" << std::endl;
            return false;
        }
        if (nullptr == fgets(line, sizeof(line), file)) {
            std::cout << "[ERROR] Command  fgets failed" << std::endl;
            return false;
        }
        sscanf(line, "%d %d", &gpu_cur, &gpu_all);
        std::lock_guard<std::mutex> lock(o.mutex_);
        s.cur_gpu_occupancy = (gpu_all == 0)
                                  ? 0
                                  : static_cast<float>(gpu_cur) /
                                        static_cast<float>(gpu_all) * 100;
        s.max_gpu_occupancy =
            std::max(s.max_gpu_occupancy, s.cur_gpu_occupancy);
        s.avg_gpu_occupancy +=
            (s.cur_gpu_occupancy - s.avg_gpu_occupancy) / s.gpu_count;
        fclose(file);
#endif
        return true;
    }

    static void OccupancyThread(Occupancy *const occupancy) {
        using namespace std::chrono;
        while (occupancy->is_enabled_) {
            auto loop_start_time = high_resolution_clock::now();
            OccupancyCpu(*occupancy);
            if (!OccupancyMem(*occupancy)) {
                std::cerr << "Get mem failed! OccupancyThread exit."
                          << std::endl;
                break;
            }

            auto loop_cost_time =
                duration_cast<milliseconds>(high_resolution_clock::now() -
                                            loop_start_time)
                    .count();
            auto need_sleep = occupancy->thread_loop_time_ms_ - loop_cost_time;
            if (need_sleep > 0) {
                // std::cout << "occ need sleep: " << need_sleep << std::endl;
                std::this_thread::sleep_for(milliseconds(need_sleep));
            }

            // ****************************
            // auto start = high_resolution_clock::now();
            // if (!OccupancyCpu(*occupancy)) break;
            // const int gpu_mem_sample_time = 30;
            // while ((duration_cast<microseconds>(high_resolution_clock::now()
            // -
            //                                     start)
            //             .count()) < 1000 * occupancy->sample_time_) {
            //     auto gpu_mem_start = high_resolution_clock::now();
            //     if (!OccupancyMem(*occupancy)) break;
            //     if (!OccupancyGpu(*occupancy)) break;
            //     auto gpu_mem_end = high_resolution_clock::now();
            //     long gpu_mem_time =
            //         duration_cast<microseconds>(gpu_mem_end - gpu_mem_start)
            //             .count();
            //     if (gpu_mem_time < 1000 * gpu_mem_sample_time) {
            //         std::this_thread::sleep_for(microseconds(
            //             1000 * gpu_mem_sample_time - gpu_mem_time));
            //     }
            // }

            // auto end = high_resolution_clock::now();
            // long time = duration_cast<microseconds>(end - start).count();
            // if (time < 1000 * occupancy->sample_time_) {
            //     std::this_thread::sleep_for(
            //         microseconds(1000 * occupancy->sample_time_ - time));
            // }
        }
    }

 private:
    std::atomic_bool is_enabled_;
    std::thread thread_;
    OccupancyStat statistics_;
    int sample_time_{0};
    int64_t thread_loop_time_ms_;
    std::mutex mutex_;
    decltype(std::chrono::high_resolution_clock::now()) start_time_;
    double target_fps_;
    std::string model_file_name_;
    std::string soft_version_;
};

#endif  // TEST_OCCUPANCY_HPP_
