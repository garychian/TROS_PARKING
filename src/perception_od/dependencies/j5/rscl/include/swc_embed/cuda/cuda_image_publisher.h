#pragma once
#include "swc_embed/image_publisher.h"
#include "swc_embed/cuda/cuda_define.h"


namespace senseAD {
namespace swc_embed {
class CudaImagePublisher : public ImagePublisher {
  public:
    CudaImagePublisher(const ImagePublisherParam& param);
    ~CudaImagePublisher() override;

    bool Publish(const IpcImage &image, cv::Mat *out_mat = nullptr) override;
    bool Publish(const std::shared_ptr<SendMsg<senseAD::msg::std_msgs::Image>> &msg) override;
    bool Publish() override;
    bool CopyIn(uint32_t offset, void* src, uint32_t size, bool is_device = false) override;
    int buffer_size() { return image_size_ + sizeof(senseAD::ImageHeader_t); }

  private:
    size_t image_size_;
    senseAD::rscl::comm::CudaPublisher::Ptr cuda_publisher_;
#if BUILD_XAVIER
    void *cache_ptr_ = nullptr;
#endif
    void *cuda_ptr_ = nullptr;
};


#if defined(DEV_INPLACE)
class CudaImageInplacePublisher : public ImagePublisher {
  public:
    static constexpr size_t MaxQueueSize = 8;
    CudaImageInplacePublisher(const ImagePublisherParam& param);
    ~CudaImageInplacePublisher() override;

    bool Publish(const IpcImage &image, cv::Mat *out_mat = nullptr) override;
    bool Publish(const std::shared_ptr<SendMsg<senseAD::msg::std_msgs::Image>> &msg) override;
    bool Publish() override;
    bool CopyIn(uint32_t offset, void* src, uint32_t size, bool is_device = false) override;
    int buffer_size() { return image_size_ + sizeof(senseAD::ImageHeader_t); }
    int queue_size() { return mem_queue_.size(); }

  private:
    size_t image_size_;
    senseAD::rscl::comm::CudaPublisher::Ptr cuda_publisher_;
    std::vector<std::shared_ptr<senseAD::rscl::comm::PreallocatedMemblock>> mem_queue_;
    std::atomic<size_t> counter_;
    std::shared_ptr<senseAD::rscl::comm::Memblock> CurrentBlock();
};
#endif
}
}
