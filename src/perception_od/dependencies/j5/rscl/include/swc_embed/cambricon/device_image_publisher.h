#pragma once

#include "swc_embed/image_publisher.h"
#include "swc_embed/device_memory.h"

namespace senseAD {
namespace swc_embed {

class DeviceImagePublisher : public ImagePublisher {
  public:
    static constexpr size_t MaxQueueSize = 8;

    DeviceImagePublisher(const ImagePublisherParam& param);
    bool Publish(const IpcImage &image, cv::Mat *out_mat = nullptr) override;
    bool Publish(const std::shared_ptr<SendMsg<senseAD::msg::std_msgs::Image>> &msg) override;
    bool CopyIn(uint32_t offset, void* src, uint32_t size, bool is_device = false) override;
    int queue_size() { return mem_queue_.size(); }
    int buffer_size() { return image_size_; }
  private:
    size_t image_size_;
    std::atomic<size_t> counter_;
    std::vector<std::shared_ptr<AllocedDeviceMemory>> mem_queue_;
    senseAD::rscl::comm::Publisher<senseAD::msg::std_msgs::ImageVGU>::Ptr notify_publisher_;
    std::shared_ptr<AllocedDeviceMemory> CurrentMemory();
};

}
}
