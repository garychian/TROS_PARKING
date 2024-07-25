/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Yin Shaopeng <yinshaopeng@sensetime.com>

 ADImage could contain multi-image, but all images should have same width, height, pixel format,
 data mananger and align rule

 Assume we have some images like:
 ============ first image ==========
 ------------ image width ----------
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx image height
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |

 ============ second image =========
 ------------ image width ----------
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx image height
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx |

 Each member of ADImage is defined as:
 ---------------image stride -------------|
 ---------image width aligned --------|
 ------------ image width ----------|
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz image height  |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             image height aligned
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz -             |
 zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz -             |

 ---------------image stride -------------|
 ---------image width aligned --------|
 ------------ image width ----------|
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz image height  |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             image height aligned
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzzzzzz |             |
 zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz -             |
 zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz -             |
 */

#ifndef _ADMETA_IMAGE_ADIMAGE_HPP_
#define _ADMETA_IMAGE_ADIMAGE_HPP_

#include "core/common.h"
#include "data_manager/DataMgrBase.hpp"
// #include "log.hpp"

#include <memory>
#include <new>
#include <string.h>
#include <vector>

class ShareCenterNodes;
using ShareCenterNodesPtr = std::shared_ptr< ShareCenterNodes >;
namespace ADInference {

class EXPORT_API ADImage final {
public:
    ADImage();
    ~ADImage(){};
    // ----------------------------------
    //! @brief Construct image without  memory allocate and data copy
    //! @param[in] width  : The width of image.
    //! @param[in] height : The height of image.
    //! @param[in] number : The width of image.
    //! @param[in] pixel_format : The format of image.
    //! @param[in] time_stamp : The time_stamp of image.
    //! @param[in] virt_ptr : The virtual address point array of image
    //! @param[in] phy_ptr : The physical address point array of image
    //! @param[in] mem_type : The memory type of image.
    //! @note
    //! the size of virt_ptr should equal to  phy_ptr size
    //! the size of time_stamp should equal to  number
    //! Now only support number == 1
    //! Make sure the pointer and memory type correspond
    //! the channel of image determined by pixel_format, etc. bgr is 3
    ADImage(const uint32_t width,
            const uint32_t height,
            const uint32_t number,
            const PixelFormat pixel_format,
            const std::vector< TimeStamp >& time_stamp,
            const std::vector< void* >& virt_ptr,
            const std::vector< void* >& phy_ptr,
            const MemoryType mem_type = MemoryType::MEM_ON_CPU,
            const bool use_cache = false);
    // ----------------------------------
    //! @brief Construct image with memory allocate
    //! @param[in] width  : The width of image.
    //! @param[in] height : The height of image.
    //! @param[in] number : The width of image.
    //! @param[in] pixel_format : The format of image.
    //! @param[in] time_stamp : The time_stamp of image.
    //! @param[in] mem_type : The memory type of image.
    //! @param[in] use_cache : Whether use cache memory.
    //! @param[in] mem_alloced : Alloc memory insight.
    //! @note
    //! the size of time_stamp should equal to  number
    //! Now only support number == 1
    //! the channel of image determined by pixel_format, etc. bgr is 3
    ADImage(const uint32_t width,
            const uint32_t height,
            const uint32_t number,
            const PixelFormat pixel_format,
            const MemoryType mem_type,
            const bool use_cache = false,
            const bool mem_alloced = false);

    // ----------------------------------
    //! @brief Construct image with memory allocate
    //! @param[in] width  : The width of image.
    //! @param[in] height : The height of image.
    //! @param[in] number : The width of image.
    //! @param[in] pixel_format : The format of image.
    //! @param[in] time_stamp : The time_stamp of image.
    //! @param[in] mem_type : The memory type of image.
    //! @note
    //! the size of time_stamp should equal to  number
    //! Now only support number == 1
    //! the channel of image determined by pixel_format, etc. bgr is 3
    ADImage(const uint32_t width,
            const uint32_t height,
            const uint32_t number,
            const PixelFormat pixel_format,
            const std::vector< TimeStamp >& time_stamp,
            const MemoryType mem_type = MemoryType::MEM_ON_CPU,
            const bool use_cache = false);

    // ----------------------------------
    //! @brief Construct  with specified format, only memory alloc, no data copy
    //! @param[in] pixel_format : The format of image.
    ADImage(const ADImage& other, const PixelFormat format, const bool use_cache = false)
        : ADImage(other.GetWidth(),
                  other.GetHeight(),
                  other.GetNumber(),
                  format,
                  other.GetTimestamp(),
                  other.data_manager_->GetMemType(),
                  use_cache) {}

    // ----------------------------------
    //! @brief Construct  with specified  h w, only memory alloc, no data copy
    //! @param[in] width  : The width of image.
    //! @param[in] height : The height of image.
    ADImage(const ADImage& other,
            const uint32_t width,
            const uint32_t height,
            const bool use_cache = false)
        : ADImage(width,
                  height,
                  other.GetNumber(),
                  other.GetPixelFormat(),
                  other.GetTimestamp(),
                  other.data_manager_->GetMemType(),
                  use_cache) {}
    // ----------------------------------
    //! @brief Construct  with specified  mem type , only memory alloc, no data copy
    //! @param[in] mem_type  : mem_type.
    ADImage(const ADImage& other, const MemoryType mem_type, const bool use_cache = false)
        : ADImage(other.GetWidth(),
                  other.GetHeight(),
                  other.GetNumber(),
                  other.GetPixelFormat(),
                  other.GetTimestamp(),
                  mem_type,
                  use_cache) {}

    // ----------------------------------
    //! @brief Split image with index, No data copy
    //! @param[in] idx : The index of number.
    //! @param[out] image_out : output ADImage.
    //! @note
    //! image_out will not take control of memory.
    //! the memory will be controled by raw image.
    //! User need to make sure split_image is free before raw_image
    MStatus MImageSplit(const uint32_t idx, ADImage& image_out) const;

    // ----------------------------------
    //! @brief Split image with channel, No data copy
    //! @param[in] idx : The index of number.
    //! @param[in] channel : The channel of number.
    //! @param[out] image_out : output ADImage.
    //! @note
    //! image_out will not take control of memory.
    //! the memory will be controled by raw image.
    //! User need to make sure split_image is free before raw_image
    MStatus
    MImageSplitChannel(const uint32_t batch, const uint32_t channel, ADImage& image_out) const;

    // ----------------------------------
    //! @brief Reshape image without changing data
    //! @param[in] width  : The width of image.
    //! @param[in] height : The height of image.
    //! @param[in] number : The width of image.
    //! @param[in] pixel_format : The format of image.
    //! @note
    //! etc .use for YUV reshape to Y
    MStatus MImageReshape(const uint32_t width,
                          const uint32_t height,
                          const uint32_t number,
                          const PixelFormat pixel_format);

    // ----------------------------------
    //! @brief repalce data manager
    MStatus MImageDataMgrReplace(const DataMgrBasePtr& data_mgr);

    bool operator==(const ADImage& other);
    bool operator!=(const ADImage& other);

    // ----------------------------------
    //! @brief Get Vitual Address pointer if image
    //! @param[in] n  : The idx of image number.
    template < typename T > inline T* GetData(size_t n = 0) const {
        if (number_ <= 0U || data_manager_ == nullptr) {
            return nullptr;
        }
        uint8_t* data_u8 = static_cast< uint8_t* >(data_manager_->GetVirDataPtr());
        void* data = static_cast< void* >(data_u8 + n * nscalar_);
        return static_cast< T* >(data);
    }

    inline uint32_t GetWidth() const { return width_; }
    inline uint32_t GetHeight() const { return height_; }
    inline uint32_t GetChannel() const { return channel_; }
    inline uint32_t GetStride() const { return stride_; }
    inline uint32_t GetNumber() const { return number_; }
    inline uint32_t GetTypeSize() const { return type_size_; }
    inline uint32_t GetScalar() const { return nscalar_; }
    inline uint32_t GetSize() const { return number_ * nscalar_; }
    inline uint32_t GetAlignedWidth() const { return aligned_width_; }
    inline uint32_t GetAlignedHeight() const { return aligned_height_; }
    inline uint32_t GetDetachSize() const { return detach_size_; }
    inline const DataMgrBasePtr& GetDataManager() const { return data_manager_; }
    inline const DataAlignRulePtr& GetAlignRule() const { return align_rule_; }
    inline const ShareCenterNodesPtr& GetShareCenterNodes() const { return share_nodes_; }
    inline const MemoryType& GetMemType() const { return data_manager_->GetMemType(); }
    inline const std::string& GetMemTypeStr() const { return data_manager_->GetMemTypeStr(); }
    inline const PixelFormat& GetPixelFormat() const { return pixel_format_; }
    inline const std::vector< TimeStamp >& GetTimestamp() const { return time_stamp_; }
    const std::string& GetPixelFormatStr() const { return pixel_format_str_; };

    // ----------------------------------
    //! @brief Deep Clone
    ADImage Clone() const { return Clone(data_manager_->GetMemType()); }
    ADImage Clone(MemoryType type) const;
    std::shared_ptr< ADImage > CloneShared(MemoryType type) const;

    // ----------------------------------
    //! @brief print image data
    void DumpImage(const std::string& filename) const;

private:
    void CloneTo(ADImage& target) const;
    bool ConvertFormat2Channel();
    void CreatDataManagerAndRule(const MemoryType mem_type);

    uint32_t width_;
    uint32_t height_;
    uint32_t stride_;
    uint32_t nscalar_;
    uint32_t number_;
    uint32_t channel_;
    uint32_t type_size_;
    uint32_t aligned_width_;
    uint32_t aligned_height_;
    uint32_t detach_size_;
    PixelFormat pixel_format_;
    std::string pixel_format_str_;
    std::vector< TimeStamp > time_stamp_;
    DataMgrBasePtr data_manager_;
    DataAlignRulePtr align_rule_;
    ShareCenterNodesPtr share_nodes_;
    bool use_cache_ = false;
};
using ADImagePtr = std::shared_ptr< ADImage >;

#define LogImageCommon(prefix, image, func)                                                        \
    do {                                                                                           \
        func("{} Image: Number {}, Width {}/{},  Height {}/{}, Stride {}, Scalar {}, MemType {} ," \
             " Format {},Data 0x{:x}",                                                             \
             prefix,                                                                               \
             (image).GetNumber(),                                                                  \
             (image).GetWidth(),                                                                   \
             (image).GetAlignedWidth(),                                                            \
             (image).GetHeight(),                                                                  \
             (image).GetAlignedHeight(),                                                           \
             (image).GetStride(),                                                                  \
             (image).GetScalar(),                                                                  \
             (image).GetMemType(),                                                                 \
             (image).GetPixelFormatStr(),                                                          \
             reinterpret_cast< uint64_t >((image).GetData< uint8_t >()));                          \
    } while (0);

#define LogImageDbg(prefix, image)                   \
    do {                                             \
        LogImageCommon(prefix, image, SDK_LOG_DEBUG) \
    } while (0);

#define LogImageErr(prefix, image)                   \
    do {                                             \
        LogImageCommon(prefix, image, SDK_LOG_ERROR) \
    } while (0);

#define LogImageDbg2(prefix, image1, image2)                    \
    do {                                                        \
        LogImageDbg(prefix, image1) LogImageDbg(prefix, image2) \
    } while (0);

#define LogImageErr2(prefix, image1, image2)                    \
    do {                                                        \
        LogImageErr(prefix, image1) LogImageErr(prefix, image2) \
    } while (0);

} // namespace ADInference

#endif // _ADMETA_IMAGE_ADIMAGE_HPP_
