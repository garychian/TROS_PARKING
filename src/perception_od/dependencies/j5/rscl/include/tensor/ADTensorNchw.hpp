/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Yin Shaopeng <yinshaopeng@sensetime.com>
 */
#ifndef _ADMETA_TENSOR_ADTENSOR_NCHW_HPP_
#define _ADMETA_TENSOR_ADTENSOR_NCHW_HPP_

#include "tensor/ADTensor4D.hpp"

namespace ADInference {

class EXPORT_API ADTensorNchw final : public ADTensor4D {
public:
    ADTensorNchw(){};
    virtual ~ADTensorNchw(){};

    ADTensorNchw(const ShapeVec& shape,
                 const std::string& mem_type = TENSOR_MEMTYPE_CPU,
                 const PrimitiveType elem_type = PrimitiveType::HPC_FLOAT32,
                 const bool model_adapt = false)
        : ADTensor4D(shape, TENSOR_SHAPE_MODE_NCHW, mem_type, elem_type, model_adapt) {
        this->SetInitDone((Init() == MStatus::M_OK));
    }
    ADTensorNchw(const ShapeVec& shape,
                 const PrimitiveType elem_type,
                 const std::string& mem_type = TENSOR_MEMTYPE_CPU,
                 const bool model_adapt = false)
        : ADTensorNchw(shape, mem_type, elem_type, model_adapt) {}

    ADTensorNchw(const void* vir,
                 const void* phy,
                 const ShapeVec& shape,
                 const std::string& mem_type = TENSOR_MEMTYPE_CPU,
                 const PrimitiveType elem_type = PrimitiveType::HPC_FLOAT32,
                 const bool model_adapt = false)
        : ADTensor4D(vir, phy, shape, TENSOR_SHAPE_MODE_NCHW, mem_type, elem_type, model_adapt) {
        this->SetInitDone((Init() == MStatus::M_OK));
    }

    ADTensorNchw(const DataMgrBasePtr data_mgr,
                 const ShapeVec& shape,
                 const std::string& mem_type = TENSOR_MEMTYPE_CPU,
                 const PrimitiveType elem_type = PrimitiveType::HPC_FLOAT32,
                 const bool model_adapt = false)
        : ADTensor4D(data_mgr, shape, TENSOR_SHAPE_MODE_NCHW, mem_type, elem_type, model_adapt) {
        this->SetInitDone((Init() == MStatus::M_OK));
    }

    ADTensor4DPtr Create() const override;
    ADTensor4DPtr Create(const ShapeVec& shape) const override;
    ADTensor4DPtr Create(const std::string& mem_type) const override;
    ADTensor4DPtr Create(const void* virt, const void* phy, const ShapeVec& shape) const override;
    ADTensor4DPtr Create(const void* virt, const void* phy, const std::string& mem) const override;

    ADTensor4DPtr Split(const uint32_t idx) const override;

    void SetCustomParams(ShapeVec& aligned_shape,
                         uint32_t& stride,
                         uint32_t& nscalar,
                         uint32_t& size) override;
    ADTensorPtr ReArrange(const std::string& shape_mode,
                          ADTensorPtr tensor_buffer = nullptr) override;
    ADTensorPtr ReArrangeFp32(const std::string& shape_mode,
                              ADTensorPtr tensor_buffer = nullptr) override;
    ADTensorPtr ConvertToFp32(ADTensorPtr tensor_buffer = nullptr) override;
    ADTensorPtr Clone(const std::string& mem_type) const override;

private:
    template < typename T > void DoReArrange(ADTensorPtr tensor_buffer);
};
} // namespace ADInference
#endif // _ADMETA_TENSOR_ADTENSOR_NCHW_HPP_
