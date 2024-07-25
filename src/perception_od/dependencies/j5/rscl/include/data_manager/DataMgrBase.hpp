
/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Yin Shaopeng <yinshaopeng@sensetime.com>
 */

#ifndef _ADMETA_CORE_DATAMGR_HPP_
#define _ADMETA_CORE_DATAMGR_HPP_

#include "core/Error.hpp"
#include "core/Optimization.hpp"
#include "core/common.h"
#include "tensor/log.hpp"

#include <memory>
#include <new>
#include <unordered_map>
#include <vector>
namespace ADInference {

/* align up to z^n */
template < typename T_ > inline T_ AlignUp2N(const T_ v, uint32_t r) {
    return (((v + ((static_cast< T_ >(0x1) << r) - static_cast< T_ >(0x1))) >> r) << r);
}
/* align down to z^n */
template < typename T_ > inline T_ AlignDownN(const T_ v, uint32_t r) {
    return ((v >> r) << r);
}

#define ALIGN_UP_16(x) AlignUp2N(x, 4)
#define ALIGN_DOWN_16(x) AlignDownN(x, 4)

#define ALIGN_UP_2(x) AlignUp2N(x, 1)
#define ALIGN_DOWN_2(x) AlignDownN(x, 1)

#define IS_ALIGN_16(x) ((( x )&0xF) == 0x0)
#define IS_ALIGN_2(x) ((( x )&0x1) == 0x0)

#define MEMTYPE_CPU ("CPU")
#define MEMTYPE_CVFLOW ("CVFLOW")
#define MEMTYPE_NNIE ("NNIE")
#define MEMTYPE_OCL ("OCL")
#define MEMTYPE_OCL_SVM ("OCL_SVM")
#define MEMTYPE_OCL_ION ("OCL_ION")
#define MEMTYPE_HEXAGON_DSP ("HEXAGON_DSP")
#define MEMTYPE_TIDL_CPU_DSP_SHARED ("TIDL_CPU_DSP_SHARED")
#define MEMTYPE_CADENCE_DSP ("CADENCE_DSP")
#define MEMTYPE_CUDA_HOST ("CUDA_HOST")
#define MEMTYPE_CUDA_DEV ("CUDA_DEV")
#define MEMTYPE_CUDA_UM ("CUDA_UM")
#define MEMTYPE_KESTREL_DEV ("KESTREL_DEV")
#define MEMTYPE_CAMBRICON ("CAMBRICON")
#define MEMTYPE_INVALID ("INVALID")

#if defined (_WIN32) && !defined (_WIN64)
typedef uint32_t AddrType;
#else
typedef uint64_t AddrType;
#endif

struct DataAlignRule {
    uint32_t width;  /* align 2^n */
    uint32_t height; /* align 2^n */
    uint32_t stride; /* align 2^n */
    DataAlignRule(uint32_t w, uint32_t h, uint32_t s) : width(w), height(h), stride(s) {}
};

const static std::unordered_map< std::string, MemoryType > mem_type_map_ = {
    {MEMTYPE_CPU, MemoryType::MEM_ON_CPU},
    {MEMTYPE_CUDA_HOST, MemoryType::MEM_ON_CUDA_HOST},
    {MEMTYPE_CUDA_DEV, MemoryType::MEM_ON_CUDA_DEV},
    {MEMTYPE_CVFLOW, MemoryType::MEM_ON_AMBA_CVFLOW},
    {MEMTYPE_NNIE, MemoryType::MEM_ON_HISI_NNIE},
    {MEMTYPE_OCL, MemoryType::MEM_ON_OCL},
    {MEMTYPE_OCL_SVM, MemoryType::MEM_ON_OCL_SVM},
    {MEMTYPE_HEXAGON_DSP, MemoryType::MEM_ON_HEXAGON_DSP},
    {MEMTYPE_TIDL_CPU_DSP_SHARED, MemoryType::MEM_ON_TIDL_CPU_DSP_SHARED},
    {MEMTYPE_CADENCE_DSP, MemoryType::MEM_ON_CADENCE_DSP},
    {MEMTYPE_KESTREL_DEV, MemoryType::MEM_ON_KESTREL_DEV},
    {MEMTYPE_CAMBRICON, MemoryType::MEM_ON_CAMBRICON},
    {MEMTYPE_OCL_ION, MemoryType::MEM_ON_OCL_ION}};

// use for detach format .etc M_PIX_FMT_NV12_DETACH. detach_size is decide by format
class DataDetachRule {
public:
    DataDetachRule() {}
    DataDetachRule(const std::vector< void* >& v,
                   const std::vector< void* >& p,
                   uint32_t detach_size) {
        if (v.size() != p.size() || v.size() != detach_size || detach_size < 2) {
            // SDK_LOG_ERROR("ptr size err,vir {},phy {} need {}", v.size(), p.size(), detach_size);
            return;
        }
        vir_detach_ptr_ = v;
        phy_detach_ptr_ = p;
        vir_detach_offset_.resize(detach_size, 0);
        phy_detach_offset_.resize(detach_size, 0);

        for (uint32_t i = 1; i < detach_size; i++) {
            vir_detach_offset_[i] = reinterpret_cast< int64_t >(static_cast< uint8_t* >(v[i])) -
                                    reinterpret_cast< int64_t >(static_cast< uint8_t* >(v[i - 1]));
            phy_detach_offset_[i] = reinterpret_cast< int64_t >(static_cast< uint8_t* >(p[i])) -
                                    reinterpret_cast< int64_t >(static_cast< uint8_t* >(p[i - 1]));
        }
    }
    const std::vector< void* > GetVirDetachPtr() const { return vir_detach_ptr_; }
    const std::vector< void* > GetPhyDetachPtr() const { return phy_detach_ptr_; }
    const std::vector< int64_t > GetVirDetachOffset() const { return vir_detach_offset_; }
    const std::vector< int64_t > GetPhyDetachOffset() const { return phy_detach_offset_; }

private:
    std::vector< void* > vir_detach_ptr_;
    std::vector< void* > phy_detach_ptr_;
    std::vector< int64_t > vir_detach_offset_;
    std::vector< int64_t > phy_detach_offset_;
};

class EXPORT_API DataMgrBase {
public:
    DataMgrBase()
        : mem_type_(MemoryType::MEM_ON_CPU),
          mem_type_str_(MEMTYPE_CPU),
          use_cache_(false),
          split_offset_(0){};
    virtual ~DataMgrBase(){};
    virtual void* Malloc(const uint32_t size) = 0;
    virtual void* Setptr(void* virt, void* phy, uint32_t size) = 0;
    virtual void Free(void* p) = 0;
    virtual std::shared_ptr< DataMgrBase > Split(const uint32_t offset,
                                                 const uint32_t size) const = 0;
    virtual std::shared_ptr< DataMgrBase > Create() const = 0;
    virtual std::shared_ptr< DataAlignRule > GetAlignRule() = 0;
    virtual AddrType GetPhyDataAddr() = 0;
    virtual AddrType GetVirDataAddr() = 0;
    virtual void* GetPhyDataPtr() = 0;
    virtual void* GetVirDataPtr() = 0;
    // virtual MStatus SyncCache(bool io = true) = 0;
    virtual uint32_t GetSize() = 0;

    inline const MemoryType& GetMemType() const { return mem_type_; }
    inline const std::string& GetMemTypeStr() const { return mem_type_str_; }
    inline const DataDetachRule& GetDetachRule() const { return detach_rule_; }
    inline uint32_t GetSplitOffset() const { return split_offset_; }

    inline void SetDetachRule(const DataDetachRule& detach) { detach_rule_ = detach; }
    inline void SetSplitOffset(const uint32_t offset) { split_offset_ = offset; }
    inline void SetMemType(const std::string& type) {
        mem_type_str_ = type;
        mem_type_ = MemTypeStrToMemType(mem_type_str_);
    }

    inline bool IsUseCache() const { return use_cache_; };

    inline void SetUseCache(const bool use_cache) { use_cache_ = use_cache; };

    static const std::string MemTypeToMemTypeStr(const MemoryType type) {
        for (auto& item : mem_type_map_) {
            if (item.second == type) {
                return item.first;
            }
        }
        // SDK_LOG_ERROR("unsupport mem type {}", static_cast< int32_t >(type));
        return "invalid";
    }

    static MemoryType MemTypeStrToMemType(const std::string& type) {
        auto it = mem_type_map_.find(type);
        if (it == mem_type_map_.end()) {
            // SDK_LOG_ERROR("unsupport mem type {}", type);
            return MemoryType::MEM_ON_MEMORY_TYPE_MAX;
        }
        return it->second;
    }

private:
    MemoryType mem_type_;
    std::string mem_type_str_;
    bool use_cache_;
    uint32_t split_offset_;
    DataDetachRule detach_rule_;
};

using DataMgrBasePtr = std::shared_ptr< DataMgrBase >;
using DataAlignRulePtr = std::shared_ptr< DataAlignRule >;
} // namespace ADInference
#endif // _ADMETA_CORE_DATAMGR_HPP_
