#ifndef __INCLUDE_OPFLOW_HPP__
#define __INCLUDE_OPFLOW_HPP__

#include <common/error.hpp>
#include <memory>
#include <string>
#include <vector>
#include "operator.hpp"
#include <mutex>
#include <condition_variable>
#include <thread>
// #include "proxy.hpp"
#include "work.hpp"
namespace senseAD {
namespace perception {
namespace camera {

template <typename T>
class ThreadsafeList {
    struct node {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;

        node() : next() {}

        node(T const& value) : data(std::make_shared<T>(value)) {}

        node(std::shared_ptr<T> value) : data(value) {}
    };

    node head;

 public:
    ThreadsafeList() {}

    ~ThreadsafeList() {
        remove_if([](T const&) { return true; });
    }

    ThreadsafeList(ThreadsafeList const& other) = delete;
    ThreadsafeList& operator=(ThreadsafeList const& other) = delete;

    void push_front(T const& value) {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lk(head.m);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }

    void push_front(std::shared_ptr<T> value) {
        std::unique_ptr<node> new_node = std::make_unique<node>(value);
        std::lock_guard<std::mutex> lk(head.m);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }

    template <typename Function>
    void for_each(Function f) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            f(*next->data);
            current = next;
            lk = std::move(next_lk);
        }
    }

    bool empty() {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        if (current->next.get() == nullptr) {
            return true;
        }
        return false;
    }
    int size() {
        int size = 0;
        for_each([&size](T const&) { size++; });
        return size;
    }

    template <typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            if (p(*next->data)) {
                return next->data;
            }
            current = next;
            lk = std::move(next_lk);
        }
        return std::shared_ptr<T>();
    }

    template <typename Predicate>
    std::shared_ptr<T> pop_first_if(Predicate p) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            if (p(*next->data)) {
                std::unique_ptr<node> old_next = std::move(current->next);
                current->next = std::move(next->next);
                auto result = next->data;
                next_lk.unlock();
                return result;
            } else {
                lk.unlock();
                current = next;
                lk = std::move(next_lk);
            }
        }
        return std::shared_ptr<T>();
    }

    std::shared_ptr<T> pop_first_ready() {
        return pop_first_if([](T const& t) { return t.getReady(); });
    }

    template <typename Predicate>
    void remove_if(Predicate p) {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node* const next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            if (p(*next->data)) {
                std::unique_ptr<node> old_next = std::move(current->next);
                current->next = std::move(next->next);
                next_lk.unlock();
            } else {
                lk.unlock();
                current = next;
                lk = std::move(next_lk);
            }
        }
    }

    void remove_according_name(std::string name) {
        remove_if([name](T const& t) { return t.getName() == name; });
    }

    void clear() {
        remove_if([](T const&) { return true; });
    }
};

typedef std::function<apStatus_t(Proxy& proxy)> OperatorProcess;

class OpTask {
 public:
    OpTask(std::string name) : name_(name) {}

    ~OpTask() {}

    void registerTask(OperatorProcess task) { task_ = task; }

    void addDependency(std::shared_ptr<OpTask> dependency);

    void addDownStream(std::shared_ptr<OpTask> downstream);

    apStatus_t operator()(Proxy& proxy);

    void Skip();

    bool getReady() { return deps_wait_count.load() == 0; }

    void setReady() { deps_wait_count.store(dependencies.size()); }

    std::string getName() { return name_; }

 private:
    std::vector<std::shared_ptr<OpTask>> dependencies;
    std::vector<std::shared_ptr<OpTask>> downstreams;
    std::atomic<int> deps_wait_count;
    OperatorProcess task_;
    std::string name_;
};

struct OpTaskGroup {
    std::shared_ptr<OpTask> task;
    int group_id;

    std::string getName() const {
        std::string name;
        if (task) {
            name = task->getName();
        }
        return name;
    }

    bool getReady() const {
        bool ready = false;
        if (task) {
            ready = task->getReady();
        }
        return ready;
    }

    void setReady() {
        if (task) {
            task->setReady();
        }
    }
};

class OpFlow {
 public:
    OpFlow(size_t numThreads);

    apStatus_t WaitDone();

    void Enqueue(std::shared_ptr<OpTask> task, int group_id);

    void Launch(Proxy& proxy, ProcessAsyncCallback cb, void* flag);

    apStatus_t SetCallback(ProcessAsyncCallback cb, void* flag);
    ~OpFlow();

 private:
    std::shared_ptr<OpTaskGroup> popNextReadyTask();

 private:
    std::vector<std::thread> threads;
    std::atomic<int> thread_run{0};
    std::vector<std::shared_ptr<OpTaskGroup>> tasks;
    ThreadsafeList<OpTaskGroup> tasks_snapshot_safe;
    std::vector<std::shared_ptr<Proxy>> proxy_vec;
    std::mutex operator_done_mutex;
    std::mutex run_once_mutex;
    std::condition_variable operator_done;
    std::condition_variable run_once_cond;
    // std::atomic<bool> run_once_flag{false};
    bool stop;
    std::atomic<apStatus_t> status_{apStatus_t::AP_SUCCESS};
    ProcessAsyncCallback cb_;
    void* flag_;
};

}  // namespace camera
}  // namespace perception
}  // namespace senseAD

#endif  // __INCLUDE_OPFLOW_HPP__
