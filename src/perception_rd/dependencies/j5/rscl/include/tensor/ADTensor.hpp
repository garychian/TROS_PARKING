/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Yin Shaopeng <yinshaopeng@sensetime.com>
 */
#ifndef _ADMETA_TENSOR_ADTENSOR_HPP_
#define _ADMETA_TENSOR_ADTENSOR_HPP_

#include "data_manager/DataMgrBase.hpp"
#include "log.hpp"
#include "universal/primtypes.h"

#include <algorithm>
#include <memory>
#include <new>
#include <string.h>
#include <vector>
namespace ADInference {

#define TENSOR_SHAPE_MODE_NCHW ("NCHW")
#define TENSOR_SHAPE_MODE_NHWC ("NHWC")

#define TENSOR_MEMTYPE_CPU MEMTYPE_CPU
#define TENSOR_MEMTYPE_CVFLOW MEMTYPE_CVFLOW
#define TENSOR_MEMTYPE_NNIE MEMTYPE_NNIE
#define TENSOR_MEMTYPE_OCL MEMTYPE_OCL
#define TENSOR_MEMTYPE_TIDL_CPU_DSP_SHARED MEMTYPE_TIDL_CPU_DSP_SHARED

#define TENSOR_FUNC_UNDEF(ret)                        \
    {                                                 \
        SDK_LOG_ERROR("tensor function not define."); \
        return ret;                                   \
    }

#define ADTensorPtrCast(T, Obj) std::dynamic_pointer_cast< T >(Obj)

using ShapeVec = std::vector< uint32_t >;

class EXPORT_API ADTensor : public std::enable_shared_from_this< ADTensor > {
public:
    ADTensor(){};
    virtual ~ADTensor(){};

    ADTensor(const ShapeVec& shape,
             const std::string& shape_mode,
             const std::string& mem_type,
             const PrimitiveType element_type,
             const bool model_adapt);

    ADTensor(const void* virt_ptr,
             const void* phy_ptr,
             const ShapeVec& shape,
             const std::string& shape_mode,
             const std::string& mem_type,
             const PrimitiveType element_type,
             const bool model_adapt);

    ADTensor(const DataMgrBasePtr data_mgr,
             const ShapeVec& shape,
             const std::string& shape_mode,
             const std::string& mem_type,
             const PrimitiveType element_type,
             const bool model_adapt);

    virtual void SetCustomParams(ShapeVec& aligned_shape,
                                 uint32_t& stride,
                                 uint32_t& nscalar,
                                 uint32_t& size) = 0;
    virtual std::shared_ptr< ADTensor >
    ReArrange(const std::string& shape_mode,
              std::shared_ptr< ADTensor > tensor_buffer = nullptr) = 0;
    virtual std::shared_ptr< ADTensor >
    ReArrangeFp32(const std::string& shape_mode,
                  std::shared_ptr< ADTensor > tensor_buffer = nullptr) = 0;
    virtual std::shared_ptr< ADTensor >
    ConvertToFp32(std::shared_ptr< ADTensor > tensor_buffer = nullptr) = 0;
    virtual std::shared_ptr< ADTensor > Clone(const std::string& mem_type) const = 0;

    std::shared_ptr< ADTensor > Clone() const { return Clone(mem_type_); }

    virtual MStatus Reshape(const ShapeVec& shape, bool allow_reshape_n = false);

    inline ShapeVec GetShape() const { return shape_; }
    inline ShapeVec GetAlignedShape() const { return aligned_shape_; }
    inline uint32_t GetShape(uint32_t idx) const { return shape_[idx]; }
    inline uint32_t GetAlignedShape(uint32_t idx) const { return aligned_shape_[idx]; }
    inline uint32_t GetStride() const { return stride_; }
    inline PrimitiveType GetElemType() const { return elem_type_; }
    inline uint32_t GetTypeSize() const { return type_size_; }
    inline uint32_t GetScalar() const { return nscalar_; }
    inline uint32_t GetSize() const { return size_; }
    inline uint32_t GetCount() const { return size_ / type_size_; }
    inline bool GetModelAdapt() const { return model_adapt_; }
    inline bool GetInitDone() const { return init_done_; }
    inline const std::string& GetShapeMode() const { return shape_mode_; }
    inline const std::string& GetName() const { return name_; }
    inline const DataMgrBasePtr& GetDataManager() const { return data_manager_; }
    inline const MemoryType& GetMemType() const { return data_manager_->GetMemType(); }
    inline const std::string& GetMemTypeStr() const { return mem_type_; }
    inline const DataAlignRulePtr& GetAlignRule() const { return align_rule_; }

    inline void SetInitDone(const bool done) { init_done_ = done; }
    inline void SetName(const std::string& name) { name_ = name; }
    inline void SetModelAdapt(const bool adapt) { model_adapt_ = adapt; }

    // ----------------------------------
    //! @brief Get Vitual Address pointer if tensor
    //! @param[in] n  : The idx of tensor number.
    template < typename T > inline T* GetData(const size_t n = 0) const {
        if (shape_.size() == 0U || data_manager_ == nullptr) {
            return nullptr;
        }
        uint8_t* data = static_cast< uint8_t* >(data_manager_->GetVirDataPtr());
        void* v_data = static_cast< void* >(data + n * nscalar_);
        return static_cast< T* >(v_data);
    }

    /// @brief 返回第offset个_Tp数据
    /// @param[in] offset 偏移量
    /// @note It's the user's duty to pass a correct _Tp (according to elem_type_) at compile time
    template < typename T > inline T GetDataAt(const uint32_t offset = 0) {
        if (offset > GetCount() || data_manager_ == nullptr) {
            SDK_LOG_ERROR("input error {} vs {}", offset, GetCount());
            return T(0);
        }
        T* data = static_cast< T* >(data_manager_->GetVirDataPtr());
        return data[offset];
    }
    bool operator==(const ADTensor& other);
    bool operator!=(const ADTensor& other);

protected:
    MStatus CreatDataManagerAndRule();
    MStatus Init();

private:
    ShapeVec shape_;
    ShapeVec aligned_shape_;
    std::string shape_mode_;
    std::string mem_type_;
    std::string name_;
    PrimitiveType elem_type_;
    bool model_adapt_;
    bool init_done_;
    uint32_t stride_;
    uint32_t nscalar_;
    uint32_t size_;
    uint32_t type_size_;
    DataMgrBasePtr data_manager_;
    DataAlignRulePtr align_rule_;
    std::vector< const void* > only_set_ptr_;
};

using ADTensorPtr = std::shared_ptr< ADTensor >;

#define LogTensorCommon(prefix, tensor, func)                                                   \
    do {                                                                                        \
        func("{} Tensor: Shape:{}/{}/{}/{} AlignedShape : {}/{}/{}/{}, Scalar {}, MemType {} ," \
             "Data 0x{:x}",                                                                     \
             prefix,                                                                            \
             (tensor).GetShape(0),                                                              \
             (tensor).GetShape(1),                                                              \
             (tensor).GetShape(2),                                                              \
             (tensor).GetShape(3),                                                              \
             (tensor).GetAlignedShape(0),                                                       \
             (tensor).GetAlignedShape(1),                                                       \
             (tensor).GetAlignedShape(2),                                                       \
             (tensor).GetAlignedShape(3),                                                       \
             (tensor).GetScalar(),                                                              \
             (tensor).GetMemType(),                                                             \
             reinterpret_cast< uint64_t >((tensor).GetData< uint8_t >()));                      \
    } while (0);

#define LogTensorDbg(prefix, tensor)                   \
    do {                                               \
        LogTensorCommon(prefix, tensor, SDK_LOG_DEBUG) \
    } while (0);

#define LogTensorErr(prefix, tensor)                   \
    do {                                               \
        LogTensorCommon(prefix, tensor, SDK_LOG_ERROR) \
    } while (0);

#define LogTensorDbg2(prefix, tensor1, tensor2)                     \
    do {                                                            \
        LogTensorDbg(prefix, tensor1) LogTensorDbg(prefix, tensor2) \
    } while (0);

#define LogTensorErr2(prefix, tensor1, tensor2)                     \
    do {                                                            \
        LogTensorErr(prefix, tensor1) LogTensorErr(prefix, tensor2) \
    } while (0);

} // namespace ADInference

#endif // _ADMETA_TENSOR_ADTENSOR_HPP_