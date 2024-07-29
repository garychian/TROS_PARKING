#ifndef _MATRIXSDK_MCOMMON_DATAMGR_DEFAULT_HPP_
#define _MATRIXSDK_MCOMMON_DATAMGR_DEFAULT_HPP_

#include "data_manager/DataMgrBase.hpp"

namespace ADInference {

class DataMgrDefault final : public DataMgrBase {
public:
    DataMgrDefault() : data_(nullptr), size_(0U) {
        SetMemType(MEMTYPE_CPU);
        SetUseCache(true);
    }
    ~DataMgrDefault() {}
    void* Malloc(const uint32_t size) override;
    void* Setptr(void* virt, void* phy, uint32_t size) override;
    void Free(void* p) override;
    DataMgrBasePtr Split(const uint32_t offset, const uint32_t size) const override;
    DataMgrBasePtr Create() const override;
    DataAlignRulePtr GetAlignRule() override {
        return std::make_shared< DataAlignRule >(0U, 0U, 0U);
    }

    AddrType GetPhyDataAddr() override { return 0; };
    AddrType GetVirDataAddr() override { return reinterpret_cast< AddrType >(data_.get()); }
    void* GetPhyDataPtr() override { return nullptr; };
    void* GetVirDataPtr() override { return data_.get(); }
    MStatus SyncCache(bool io = true) override { return MStatus::M_OK; }
    uint32_t GetSize() override { return size_; }

private:
    std::shared_ptr< uint8_t > data_;
    uint32_t size_;
};
} // namespace ADInference
#endif
