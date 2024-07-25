#ifndef __INCLUDE_WORK_HPP__
#define __INCLUDE_WORK_HPP__

#include <common/error.hpp>
#include <memory>
#include <string>
#include <vector>
#include "operator.hpp"
#include <mutex>
#include <condition_variable>
#include <thread>
// #include "proxy.hpp"

namespace senseAD {
namespace perception {
namespace camera {

class Work;
using WorkPtr = std::shared_ptr<Work>;

class Work {
 public:
    Work() : Work(true){};
    Work(bool do_thread);
    virtual ~Work();

    virtual apStatus_t PushOp(OperatorPtr op) {
        ops_.emplace_back(op);
        return apStatus_t::AP_SUCCESS;
    }
    // virtual apStatus_t Launch(Proxy& proxy);
    virtual apStatus_t WaitDone();

    std::string Name() { return name_; }
    void SetName(const std::string& n) { name_ = n; }
    void SetNext(WorkPtr next_work) { next_work_ = next_work; }
    bool IsWorking() { return is_working_; }
    // std::shared_ptr<Proxy> GetProxy() { return proxy_; }

 private:
    virtual apStatus_t WorkInThread();
    void SetWorking(bool r) { is_working_ = r; }

    std::string name_;
    bool is_working_;
    bool work_done_;
    std::vector<OperatorPtr> ops_;
    WorkPtr next_work_;
    std::thread work_thread_;
    std::condition_variable work_cv_;
    std::mutex work_mutex_;
    // std::shared_ptr<Proxy> proxy_;
};

using ProcessAsyncCallback = std::function<apStatus_t(const Proxy&, void*)>;
class WorkDone : public Work {
 public:
    WorkDone() : Work(false) { SetName("WorkDone"); };
    virtual ~WorkDone() {}

    // apStatus_t Launch(Proxy& proxy) override;
    apStatus_t WaitDone() override;
    apStatus_t SetCallback(ProcessAsyncCallback cb, void* flag) {
        cb_ = cb;
        flag_ = flag;
        return apStatus_t::AP_SUCCESS;
    }

 private:
    ProcessAsyncCallback cb_;
    void* flag_;
};
using WorkDonePtr = std::shared_ptr<WorkDone>;

struct WorkPipe {
    std::vector<WorkPtr> works;
    WorkDonePtr workdone;
    void InitWorks(size_t depth);
    size_t GetDepth() { return std::max<int64_t>(works.size(), 0); }
    bool IsWorkPipeReady() {
        return works.empty() ? false : !(*works.begin())->IsWorking();
    }
    WorkPtr GetWork(size_t idx) { return works.at(idx); }
    // apStatus_t Launch(Proxy& proxy, ProcessAsyncCallback cb, void* flag);
    apStatus_t WaitDone();
};

}  // namespace camera
}  // namespace perception
}  // namespace senseAD

#endif  // __INCLUDE_WORK_HPP__
