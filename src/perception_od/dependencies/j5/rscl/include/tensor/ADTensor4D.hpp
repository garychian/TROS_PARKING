/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Yin Shaopeng <yinshaopeng@sensetime.com>
 */
#ifndef _ADMETA_TENSOR_ADTENSOR4D_HPP_
#define _ADMETA_TENSOR_ADTENSOR4D_HPP_

#include "tensor/ADTensor.hpp"

namespace ADInference {

class EXPORT_API ADTensor4D : public ADTensor {
public:
    ADTensor4D(){};
    virtual ~ADTensor4D(){};

    ADTensor4D(const ShapeVec& shape,
               const std::string& shape_mode,
               const std::string& mem_type,
               const PrimitiveType element_type,
               const bool model_adapt)
        : ADTensor(shape, shape_mode, mem_type, element_type, model_adapt) {
        this->SetInitDone(Set4DShape());
    };

    ADTensor4D(const void* virt_ptr,
               const void* phy_ptr,
               const ShapeVec& shape,
               const std::string& shape_mode,
               const std::string& mem_type,
               const PrimitiveType element_type,
               const bool model_adapt)
        : ADTensor(virt_ptr, phy_ptr, shape, shape_mode, mem_type, element_type, model_adapt) {
        this->SetInitDone(Set4DShape());
    };

    ADTensor4D(const DataMgrBasePtr data_mgr,
               const ShapeVec& shape,
               const std::string& shape_mode,
               const std::string& mem_type,
               const PrimitiveType element_type,
               const bool model_adapt)
        : ADTensor(data_mgr, shape, shape_mode, mem_type, element_type, model_adapt) {
        this->SetInitDone(Set4DShape());
    };

    virtual std::shared_ptr< ADTensor4D > Create() const = 0;
    virtual std::shared_ptr< ADTensor4D > Create(const ShapeVec& shape) const = 0;
    virtual std::shared_ptr< ADTensor4D > Create(const std::string& mem_type) const = 0;
    virtual std::shared_ptr< ADTensor4D >
    Create(const void* virt, const void* phy, const ShapeVec& shape) const = 0;
    virtual std::shared_ptr< ADTensor4D >
    Create(const void* virt, const void* phy, const std::string& mem) const = 0;
    virtual std::shared_ptr< ADTensor4D > Split(const uint32_t idx) const = 0;
    virtual MStatus Reshape(const ShapeVec& shape, bool allow_reshape_n = false) override {
        auto ret = ADTensor::Reshape(shape, allow_reshape_n);
        if (MStatus::M_OK == ret) {
            ret = this->Set4DShape() ? MStatus::M_OK : MStatus::M_FAILED;
        }
        return ret;
    }
    virtual void SetCustomParams(ShapeVec& aligned_shape,
                                 uint32_t& stride,
                                 uint32_t& nscalar,
                                 uint32_t& size) override TENSOR_FUNC_UNDEF();
    virtual ADTensorPtr ReArrange(const std::string& shape_mode,
                                  ADTensorPtr tensor_buffer = nullptr) override
        TENSOR_FUNC_UNDEF(nullptr);
    virtual ADTensorPtr ReArrangeFp32(const std::string& shape_mode,
                                      ADTensorPtr tensor_buffer = nullptr) override
        TENSOR_FUNC_UNDEF(nullptr);
    virtual ADTensorPtr ConvertToFp32(ADTensorPtr tensor_buffer = nullptr) override
        TENSOR_FUNC_UNDEF(nullptr);
    virtual ADTensorPtr Clone(const std::string& mem_type) const override
        TENSOR_FUNC_UNDEF(nullptr);

    inline uint32_t GetNumber() const { return n_; }
    inline uint32_t GetChannel() const { return c_; }
    inline uint32_t GetHeight() const { return h_; }
    inline uint32_t GetWidth() const { return w_; }
    inline uint32_t IsNchw() const { return (GetShapeMode() == TENSOR_SHAPE_MODE_NCHW); }
    inline uint32_t IsNhwc() const { return (GetShapeMode() == TENSOR_SHAPE_MODE_NHWC); }

    static void DumpTensor(const std::shared_ptr< ADTensor4D >& tensor,
                           const std::string& name,
                           const bool& binary_or_txt = false);

private:
    bool Set4DShape() {
        auto shape = this->GetShape();
        if (shape.size() != 4) {
            // SDK_LOG_ERROR("unsuport shape_mode {}", shape.size());
            return false;
        }
        n_ = shape[0];
        c_ = IsNchw() ? shape[1] : shape[3];
        h_ = IsNchw() ? shape[2] : shape[1];
        w_ = IsNchw() ? shape[3] : shape[2];
        return true;
    }
    uint32_t n_{0u};
    uint32_t c_{0u};
    uint32_t h_{0u};
    uint32_t w_{0u};
};
using ADTensor4DPtr = std::shared_ptr< ADTensor4D >;

} // namespace ADInference

#endif // _ADMETA_TENSOR_ADTENSOR4D_HPP_
