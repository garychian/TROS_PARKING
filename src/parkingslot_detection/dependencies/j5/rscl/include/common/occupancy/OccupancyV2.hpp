#ifndef TEST_OCCUPANCY_V2_HPP_
#define TEST_OCCUPANCY_V2_HPP_

/****************  README  ******************
Occupancy支持代码触发和代码+信号触发。
使用信号触发Occupancy，需要在启动程序前加入环境变量 OCCUPANCY_SIGNAL_CONTROL=1
执行时，向进程发送信号，命令如下：
# 发送启动记录的命令（需要在代码中先调用 OccupancyV2::Occupancy::Instance()->Start()）
kill -s $(kill -l RTMIN+10) $(pgrep -f <your exec>)
# 发送保存性能报告的命令（需要在代码中先调用 OccupancyV2::Occupancy::Instance()->SetDefaultReportPath(xx,xxx) ）
kill -s $(kill -l RTMIN+11) $(pgrep -f <your exec>)
# 发送停止记录的命令
kill -s $(kill -l RTMIN+12) $(pgrep -f <your exec>)
/****************  README  ******************/

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
#include <cstdlib>
#include <csignal>
#include <signal.h>
#if defined(__ANDROID__) || defined(__linux__)

#include "unistd.h"

#include <sched.h>
#include <sstream>
#include <sys/syscall.h>
#endif

namespace OccupancyV2 {

struct OccupancyUtils {
    static std::string GetEnv(const std::string &var) {
        const char *val = std::getenv(var.c_str());
        if (val == nullptr) {  // invalid to assign nullptr to std::string
            return "";
        } else {
            return val;
        }
    }
};

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

struct ProcStat {
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
};

struct ProcPidStat {
    void CalSum() { total = utime + stime + cutime + cstime; }
    unsigned long utime{0};
    unsigned long stime{0};
    unsigned long cutime{0};
    unsigned long cstime{0};
    unsigned long total{0};
};

struct OccupancyStat {
    OccupancyStat() : history_len_(500), record_history_(true) {
        all_cpu_history_.reserve(500);
        bind_cpu_history_.reserve(500);
        frame_history_.reserve(500);
        mem_history_.reserve(500);
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
    std::vector<float> bind_cpu_history_;
    std::vector<float> frame_history_;
    std::vector<float> mem_history_;

    const size_t history_len_;
};

class Occupancy {
 private:
    Occupancy()
        : is_started_(false),
          is_running_(false),
          env_control_state_(ENV_CONTROL_UNKNOW),
          env_control_str_("OCCUPANCY_SIGNAL_CONTROL"),
          thread_(),
          thread_loop_time_ms_(30),
          start_time_(),
          target_fps_(10.0) {
        UpdateEnvControlState();
        ProcessEnvControlLogic(env_control_state_);
    }

 public:
    ~Occupancy() {
        is_started_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    static std::shared_ptr<Occupancy> Instance() {
        static std::shared_ptr<Occupancy> ptr(new Occupancy());
        return ptr;
    }

    void SetTargetFps(double fps) { target_fps_ = fps; }
    void SetRefreshRate(int64_t ms) { thread_loop_time_ms_ = ms; }
    void SetSoftVersion(const std::string &ver) { soft_version_ = ver; }
    void SetModelFileName(const std::string &m) { model_file_name_ = m; }
    void SetDefaultReportPath(const std::string &file_prefix,
                              const std::string &file_suffix) {
        if (file_prefix.size()) file_prefix_ = file_prefix;
        if (file_suffix.size()) file_suffix_ = file_suffix;
    }
    bool IsEnvEnableSignal() {
        return env_control_state_ == ENV_CONTROL_ENABLE_SINGAL;
    }
    bool IsRunning() { return is_running_; }

    void Start(int const &sample_time = 1000);

    void Stop() { is_started_ = false; }

    float GetPipelineTime() { return statistics_.avg_frame_time; }

    bool SaveOccupancyReport(const std::string &file_prefix,
                             const std::string &file_suffix);
    bool GetOccupancyReport(const std::string &file_prefix = "",
                            const std::string &file_suffix = "") {
        return SaveOccupancyReport(file_prefix, file_suffix);
    }
    bool SaveOccupancyReport() {
        return SaveOccupancyReport(file_prefix_, file_suffix_);
    }

    OccupancyTimer StartOneFrameTime() { return {}; }
    /*return ms*/ float EndOneFrameTime(const OccupancyTimer &timer);

    OccupancyTimer StartOneInitTime() { return {}; }
    /*return ms*/ float EndOneInitTime(const OccupancyTimer &timer);

 public:
    // static const int SIGNAL_CONTROL_UNKNOW = -1;
    // static const int SIGNAL_CONTROL_RUN = SIGRTMIN + 10;
    // static const int SIGNAL_CONTROL_REPORT = SIGRTMIN + 11;
    // static const int SIGNAL_CONTROL_STOP = SIGRTMIN + 12;
    static const int SIGNAL_CONTROL_UNKNOW;
    static const int SIGNAL_CONTROL_RUN;
    static const int SIGNAL_CONTROL_REPORT;
    static const int SIGNAL_CONTROL_STOP;
    enum ENV_CONTROL_STATE {
        ENV_CONTROL_UNKNOW = -1,
        ENV_CONTROL_ENABLE_SINGAL = 1,
        ENV_CONTROL_UNVAL,
    };

 protected:
    bool GetPidJiffies();
    bool GetCpuJiffies();
    bool GetAllCpuJiffies(std::vector<ProcStat> &proc_stat_vec);
    bool GetBindCpuJiffies(std::vector<ProcStat> &proc_stat_vec);
    bool OccupancyFrame();
    bool OccupancyCpu();
    bool OccupancyMem();
    bool OccupancyGpu();
    void OccupancyThread();

    void UpdateEnvControlState();
    bool ProcessEnvControlLogic(int state);
    bool RegistSingalControl();
    static void SingalControlHandler(int sig);
    bool IsRunEnabled();
    void WaitingForRun();

 private:
    std::atomic_bool is_started_;
    std::atomic_bool is_running_;
    std::string env_control_str_;
    ENV_CONTROL_STATE env_control_state_;
    int signal_control_state_;
    std::thread thread_;
    OccupancyStat statistics_;
    int64_t thread_loop_time_ms_;
    std::mutex mutex_;
    decltype(std::chrono::high_resolution_clock::now()) start_time_;
    double target_fps_;
    std::string model_file_name_;
    std::string soft_version_;
    std::string file_prefix_;
    std::string file_suffix_;
};

inline bool Occupancy::GetPidJiffies() {
    auto &s = statistics_;
    FILE *pid_cpu_stat_file = fopen("/proc/self/stat", "r");
    if (nullptr == pid_cpu_stat_file) {
        std::cout << "[ERROR] Command fopen /proc/self/stat failed"
                  << std::endl;
        return false;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    if (0 > fscanf(pid_cpu_stat_file,
                   "%*s %*s %*c %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s "
                   "%ld %ld %ld %ld",
                   &s.cur_pid_cpu_stat.utime, &s.cur_pid_cpu_stat.stime,
                   &s.cur_pid_cpu_stat.cutime, &s.cur_pid_cpu_stat.cstime)) {
        std::cout << "[ERROR] Command fscanf pid_cpu_stat failed" << std::endl;
        return false;
    }
    fclose(pid_cpu_stat_file);
    return true;
}

inline bool Occupancy::GetCpuJiffies() {
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
    if (!GetAllCpuJiffies(ProcStatVec)) {
        std::cout << "[ERROR] GetAllCpuJiffies" << std::endl;
        return false;
    }
    if (!GetBindCpuJiffies(ProcStatVec)) {
        std::cout << "[ERROR] GetBindCpuJiffies" << std::endl;
        return false;
    }
    return true;
}

inline bool Occupancy::GetAllCpuJiffies(std::vector<ProcStat> &proc_stat_vec) {
    auto &s = statistics_;
    std::lock_guard<std::mutex> lock(mutex_);
    s.cur_all_cpu_stat = proc_stat_vec[0];
    return true;
}

inline bool Occupancy::GetBindCpuJiffies(std::vector<ProcStat> &proc_stat_vec) {
// if you bind core by taskset or sched_setaffinity,the cpu_used will be the
// core you set if not,the core number will be the all online cpu number
#if defined(__ANDROID__) || defined(__linux__)
    auto &s = statistics_;
    if (s.cpu_count == 1) {
        cpu_set_t cs;
        CPU_ZERO(&cs);
        sched_getaffinity(getpid(), sizeof(cs), &cs);

        for (int core_idx = 0; core_idx < proc_stat_vec.size(); core_idx++) {
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
        std::lock_guard<std::mutex> lock(mutex_);
        s.cur_bind_cpu_stat.user += proc_stat_vec[id + 1].user;
        s.cur_bind_cpu_stat.nice += proc_stat_vec[id + 1].nice;
        s.cur_bind_cpu_stat.system += proc_stat_vec[id + 1].system;
        s.cur_bind_cpu_stat.idle += proc_stat_vec[id + 1].idle;
        s.cur_bind_cpu_stat.iowait += proc_stat_vec[id + 1].iowait;
        s.cur_bind_cpu_stat.irq += proc_stat_vec[id + 1].irq;
        s.cur_bind_cpu_stat.softirq += proc_stat_vec[id + 1].softirq;
        s.cur_bind_cpu_stat.stealstolen += proc_stat_vec[id + 1].stealstolen;
        s.cur_bind_cpu_stat.guest += proc_stat_vec[id + 1].guest;
    }
#endif
    return true;
}

inline bool Occupancy::OccupancyFrame() {
    auto &s = statistics_;
    if (s.record_history_) {
        s.frame_history_.push_back(s.cur_frame_time);
    }
    return true;
}

inline bool Occupancy::OccupancyCpu() {
    auto &s = statistics_;
    s.cpu_count++;

    if (!GetPidJiffies()) {
        std::cout << "[ERROR] GetPidJiffies" << std::endl;
        return false;
    }
    if (!GetCpuJiffies()) {
        std::cout << "[ERROR] GetCpuJiffies" << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
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
            s.max_pid_jiffies = std::max(s.max_pid_jiffies, s.cur_pid_jiffies);
            s.avg_pid_jiffies +=
                (s.cur_pid_jiffies - s.avg_pid_jiffies) / (s.cpu_count - 1);

            s.cur_all_cpu_occupancy =
                s.cur_pid_jiffies / delta_all_cpu_stat * 100;
            s.max_all_cpu_occupancy =
                std::max(s.max_all_cpu_occupancy, s.cur_all_cpu_occupancy);
            s.avg_all_cpu_occupancy +=
                (s.cur_all_cpu_occupancy - s.avg_all_cpu_occupancy) /
                (s.cpu_count - 1);

            s.cur_bind_cpu_occupancy = s.cur_pid_jiffies / delta_bind_cpu_stat *
                                       s.cpu_used_.size() * 100;
            s.max_bind_cpu_occupancy =
                std::max(s.max_bind_cpu_occupancy, s.cur_bind_cpu_occupancy);
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

inline bool Occupancy::OccupancyMem() {
    auto &s = statistics_;
    s.mem_count++;

    char line[128];
    auto file = fopen("/proc/self/status", "r");
    if (nullptr == file) {
        std::cout << "[ERROR] Command fopen mem failed" << std::endl;
        return false;
    }
    while (NULL != fgets(line, sizeof(line), file)) {
        if (0 == strncmp(line, "VmRSS:", 6)) {
            std::lock_guard<std::mutex> lock(mutex_);
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

inline bool Occupancy::OccupancyGpu() {
    auto &s = statistics_;
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
    s.cur_gpu_occupancy =
        (gpu_all == 0)
            ? 0
            : static_cast<float>(gpu_cur) / static_cast<float>(gpu_all) * 100;
    s.max_gpu_occupancy = std::max(s.max_gpu_occupancy, s.cur_gpu_occupancy);
    s.avg_gpu_occupancy +=
        (s.cur_gpu_occupancy - s.avg_gpu_occupancy) / s.gpu_count;
    fclose(file);
#endif
    return true;
}

inline void Occupancy::OccupancyThread() {
    using namespace std::chrono;
    is_running_ = false;
    start_time_ = high_resolution_clock::now();
    std::cout << "[Occupancy Info] Waiting For Run OccupancyThread."
              << std::endl;
    WaitingForRun();
    std::cout << "[Occupancy Info] Run OccupancyThread." << std::endl;
    while (IsRunEnabled()) {
        is_running_ = true;
        auto loop_start_time = high_resolution_clock::now();
        OccupancyCpu();
        if (!OccupancyMem()) {
            std::cerr << "Get mem failed! OccupancyThread exit." << std::endl;
            break;
        }

        auto loop_cost_time =
            duration_cast<milliseconds>(high_resolution_clock::now() -
                                        loop_start_time)
                .count();
        auto need_sleep = thread_loop_time_ms_ - loop_cost_time;
        if (need_sleep > 0) {
            // std::cout << "occ need sleep: " << need_sleep << std::endl;
            std::this_thread::sleep_for(milliseconds(need_sleep));
        }
    }
    is_running_ = false;
}

inline void Occupancy::UpdateEnvControlState() {
    auto val = OccupancyUtils::GetEnv(env_control_str_);
    if (val.empty()) {
        env_control_state_ = ENV_CONTROL_UNKNOW;
    } else {
        int val_int = atoi(val.c_str());
        if (val_int >= ENV_CONTROL_UNVAL) {
            env_control_state_ = ENV_CONTROL_UNKNOW;
        } else {
            env_control_state_ = static_cast<ENV_CONTROL_STATE>(val_int);
        }
    }
}

inline bool Occupancy::ProcessEnvControlLogic(int state) {
    if (ENV_CONTROL_ENABLE_SINGAL == env_control_state_)
        return RegistSingalControl();

    return true;
}

inline void Occupancy::SingalControlHandler(int sig) {
    std::cout << "[Occupancy Info] Get singal " << sig << std::endl;
    auto instance = Occupancy::Instance();
    if (SIGNAL_CONTROL_REPORT == sig) {
        instance->SaveOccupancyReport();
    } else if (SIGNAL_CONTROL_RUN == sig) {
        instance->signal_control_state_ = sig;
    } else if (SIGNAL_CONTROL_STOP == sig) {
        instance->signal_control_state_ = sig;
    }
}

inline bool Occupancy::RegistSingalControl() {
    bool ret;
    ret = signal(SIGNAL_CONTROL_RUN, SingalControlHandler) == SIG_ERR ? false
                                                                      : true;
    std::cout << "[Occupancy Info] Regist singal RUN " << SIGNAL_CONTROL_RUN
              << std::string(ret ? "true" : "false") << std::endl;
    ret = signal(SIGNAL_CONTROL_STOP, SingalControlHandler) == SIG_ERR ? false
                                                                       : true;
    std::cout << "[Occupancy Info] Regist singal STOP " << SIGNAL_CONTROL_STOP
              << std::string(ret ? "true" : "false") << std::endl;
    ret = signal(SIGNAL_CONTROL_REPORT, SingalControlHandler) == SIG_ERR ? false
                                                                         : true;
    std::cout << "[Occupancy Info] Regist singal REPORT " << SIGNAL_CONTROL_REPORT
              << std::string(ret ? "true" : "false") << std::endl;
    return ret;
}

inline bool Occupancy::IsRunEnabled() {
    if (ENV_CONTROL_ENABLE_SINGAL == env_control_state_) {
        return is_started_ && SIGNAL_CONTROL_RUN == signal_control_state_;
    } else {
        return is_started_;
    }
}

inline void Occupancy::WaitingForRun() {
    while (!IsRunEnabled()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

inline void Occupancy::Start(int const &sample_time) {  // sample 1000ms
    is_started_ = true;
    thread_ = std::thread(&Occupancy::OccupancyThread, this);
}

inline bool Occupancy::SaveOccupancyReport(const std::string &file_prefix,
                                           const std::string &file_suffix) {
    OccupancyCpu();
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
        file_prefix + file_suffix + "_occupancy_" + GetNowTime() + ".csv";
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
                << s.avg_frame_time << "," << s.avg_bind_cpu_occupancy << "/"
                << s.avg_all_cpu_occupancy << "," << s.avg_mem_occupancy << ","
                << "---"
                << "," << s.avg_init_time << "," << s.frame_fps << std::endl;

    if (s.record_history_) {
        auto vec_to_csv_str = [](const std::vector<float> &v) -> std::string {
            std::stringstream ss;
            std::for_each(v.begin(), v.end(), [&](float f) { ss << f << ","; });
            return ss.str();
        };
        output_file << "ALL CPU    HISTORY,"
                    << vec_to_csv_str(s.all_cpu_history_) << std::endl;
        output_file << "BIND CPU   HISTORY,"
                    << vec_to_csv_str(s.bind_cpu_history_) << std::endl;
        output_file << "DDR MEM    HISTORY," << vec_to_csv_str(s.mem_history_)
                    << std::endl;
        output_file << "FRAME TIME HISTORY," << vec_to_csv_str(s.frame_history_)
                    << std::endl;
    }

    return true;
}

inline float Occupancy::EndOneFrameTime(const OccupancyTimer &timer) {
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
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(
                     std::chrono::high_resolution_clock::now() - start_time_)
                     .count();
    fps = (1000.0 * 1000.0 / total) * times;

    OccupancyFrame();

    return duration;
}

inline float Occupancy::EndOneInitTime(const OccupancyTimer &timer) {
    auto duration = timer.GetDuration<std::chrono::microseconds>() / 1000.0;
    auto &times = statistics_.call_init_times;
    auto &avg = statistics_.avg_init_time;

    ++times;
    avg += (duration - avg) / times;

    return avg;
}

const int Occupancy::SIGNAL_CONTROL_UNKNOW = -1;
const int Occupancy::SIGNAL_CONTROL_RUN = SIGRTMIN + 10;
const int Occupancy::SIGNAL_CONTROL_REPORT = SIGRTMIN + 11;
const int Occupancy::SIGNAL_CONTROL_STOP = SIGRTMIN + 12;

}  // namespace OccupancyV2

#endif  // TEST_OCCUPANCY_V2_HPP_
