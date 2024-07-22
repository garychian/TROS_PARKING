#ifndef _MATRIXSDK_MCOMMON_DATAMGR_CACHE_HPP_
#define _MATRIXSDK_MCOMMON_DATAMGR_CACHE_HPP_

#include "data_manager/DataMgrBase.hpp"
#ifdef _MSC_VER
#include <windows.h>
#include <winsock2.h>
#else
#include "sys/time.h"
#endif
#include "time.h"
#include <chrono>
#include <deque>
#include <mutex>
#include <unordered_map>

namespace ADInference {
#ifdef _MSC_VER
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
#endif

static TimeStamp GetTimeStampNow() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return {static_cast< long int >(tv.tv_sec), static_cast< long int >(tv.tv_usec)};
}

class DataBlock final {
public:
    DataBlock(DataMgrBasePtr data_ptr, bool in_use) : data_ptr_(data_ptr) { SetInUse(in_use); };
    DataMgrBasePtr& GetData() { return data_ptr_; };
    void SetInUse(const bool in_use) {
        in_use_ = in_use;
        last_time_updated_ = GetTimeStampNow();
    };
    bool IsInUse() const { return in_use_; };
    TimeStamp GetLastTimeUpdated() const { return last_time_updated_; };

private:
    bool in_use_;
    TimeStamp last_time_updated_;
    DataMgrBasePtr data_ptr_;
};

// a singleton memory pool
class DataMgrCachePool final {
    using DataBlockPtr = std::shared_ptr< DataBlock >;
    using IdHash = std::unordered_map< uint32_t, DataBlockPtr, std::hash< int > >;
    using SizeHash = std::unordered_map< uint32_t, IdHash, std::hash< int > >;
    using MemTypeHash = std::unordered_map< MemoryType, SizeHash, std::hash< int > >;

    // static members
public:
    static DataMgrCachePool& GetInstance() {
        static DataMgrCachePool instance;
        return instance;
    };

    // non-static members
public:
    ~DataMgrCachePool() { pool_.clear(); };
    std::pair< uint32_t, DataMgrBasePtr > Allocate(const MemoryType mem_type, const uint32_t size);
    void Release(const MemoryType mem_type, const uint32_t size, const uint32_t id);
    void ForceRelease(const MemoryType mem_type);

    void SetCapacity(const uint32_t capacity) { capacity_ = capacity; };
    void SetMaxExpandTimes(const uint32_t max_expand_times) {
        max_expand_times_ = max_expand_times;
    };
    void SetUnusedTimeout(const int64_t unused_timeout) { unused_timeout_ = unused_timeout; };
    void Destory() { pool_.clear(); };
    // for debug
    void PrintPool();

private:
    // these methods are private to insure thread-safe
    DataMgrCachePool() = default;
    void Collect(const MemoryType mem_type);
    MStatus Expand();
    MStatus Shrink();
    std::pair< uint32_t, DataMgrBasePtr > CreateDataMgr(const MemoryType mem_type,
                                                        const uint32_t size);

    std::unordered_map< MemoryType, uint32_t, std::hash< int > > current_size_; // unit: bytes
    MemTypeHash pool_;
    uint32_t last_id_ = 0;
    uint32_t capacity_ = 5242880; // unit: bytes
    uint32_t max_expand_times_ = 5;
    uint32_t expand_times_ = 0;
    int64_t unused_timeout_ = 5; // unit: seconds
    std::mutex mutex_;
};

class DataMgrCache final : public DataMgrBase {
public:
    DataMgrCache(std::string mem_type) { SetMemType(mem_type); };
    ~DataMgrCache();
    void* Malloc(const uint32_t size) override;
    void* Setptr(void* virt, void* phy, uint32_t size) override;
    void Free(void* p) override { data_manager_real_->Free(p); };
    DataMgrBasePtr Split(const uint32_t offset, const uint32_t size) const override {
        return data_manager_real_->Split(offset, size);
    };
    DataMgrBasePtr Create() const override { return data_manager_real_->Create(); };
    DataAlignRulePtr GetAlignRule() override;

    AddrType GetPhyDataAddr() override { return data_manager_real_->GetPhyDataAddr(); };
    AddrType GetVirDataAddr() override { return data_manager_real_->GetVirDataAddr(); };
    void* GetPhyDataPtr() override { return data_manager_real_->GetPhyDataPtr(); };
    void* GetVirDataPtr() override { return data_manager_real_->GetVirDataPtr(); };
    MStatus SyncCache(bool io = true) override { return data_manager_real_->SyncCache(io); };
    uint32_t GetSize() override { return data_manager_real_->GetSize(); };

private:
    uint32_t size_ = 0;
    uint32_t id_ = 0;
    bool is_external_ptr_ = false;
    DataMgrBasePtr data_manager_real_ = nullptr;
};
} // namespace ADInference
#endif
