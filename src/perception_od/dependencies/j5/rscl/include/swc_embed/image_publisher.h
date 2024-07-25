#pragma once

#include <memory>
#include <unordered_map>
#include <ad_rscl/comm/node.h>
#include "ad_log/ad_log.hpp"
#include "swc_embed/pubsub_base.h"

#define REGISTER_PUBLISHER_TYPE(type_value, class_type)                        \
    namespace {                                                                \
    static struct ImagePublisherRegister##class_type {                         \
        ImagePublisherRegister##class_type() {                                 \
            ImagePublisherFactory::Instance().Register<class_type>(            \
                type_value, #class_type);                                      \
        }                                                                      \
    } _register_##class_type;                                                  \
    }  // namespace

namespace senseAD {
namespace swc_embed {

struct ImagePublisherParam {
    senseAD::rscl::comm::Node *node;
    std::string topic;
    ImageMemoryType type;
    size_t image_size;
    cv::Size dst_size;
    size_t queue_size;

    ImagePublisherParam(
        senseAD::rscl::comm::Node *node, const std::string &topic,
        ImageMemoryType type = ImageMemoryType::CPU, size_t image_size = 0, cv::Size dst_size = cv::Size{0,0},
        size_t queue_size = 6):
        node(node), topic(topic), type(type), image_size(image_size), dst_size(dst_size), queue_size(queue_size)
    {}
};

class ImagePublisher {
  public:
    using Ptr = std::shared_ptr<ImagePublisher>;

    ImagePublisher(cv::Size dst_size = cv::Size{0,0});
    virtual ~ImagePublisher();
    virtual bool Publish(const IpcImage &image, cv::Mat *out_mat = nullptr) = 0;
    bool Publish(const cv::Mat &image, uint64_t timestamp, cv::Mat *out_mat = nullptr);
    virtual bool Publish(const std::shared_ptr<SendMsg<senseAD::msg::std_msgs::Image>> &msg) = 0;
    // for device
    virtual bool Publish() { return false; }
    virtual bool CopyIn(uint32_t offset, void* src, uint32_t size, bool is_device = false) { return false; }

  protected:
    cv::Size dst_size_;
};


typedef std::unordered_map<
    int,
    std::function<bool(const ImagePublisherParam& param, ImagePublisher::Ptr* sub)>>
    ImagePublisherRegisterMap;

class ImagePublisherFactory {
  public:
    static ImagePublisherFactory& Instance();
    ImagePublisherFactory() {}

    std::shared_ptr<ImagePublisher> Create(
        const ImagePublisherParam &param);

    template<class T>
    void Register(ImageMemoryType type_value, const std::string &name) {
        if (register_map_.find(static_cast<int>(type_value)) != register_map_.end()) {
            AD_LWARN(ImagePublisherFactory) << "ImagePublisher for ImageMemoryType "
                << (int)type_value << " already registered";
            return;
        }
        AD_LDEBUG(ImagePublisherFactory) << "Register ImagePublisher " << name << " for ImageMemoryType " << (int)type_value;
        register_map_[static_cast<int>(type_value)] =
            [](const ImagePublisherParam& param, ImagePublisher::Ptr* res) {
                *res = std::dynamic_pointer_cast<ImagePublisher>(
                    std::make_shared<T>(param));
            return true;
        };
    }
  private:
    ImagePublisherRegisterMap register_map_;
};


class CapnpImagePublisher : public ImagePublisher {
  public:
    CapnpImagePublisher(const ImagePublisherParam& param);

    bool Publish(const IpcImage &image, cv::Mat *out_mat = nullptr) override;
    bool Publish(const std::shared_ptr<SendMsg<senseAD::msg::std_msgs::Image>> &msg) override;

  private:
    senseAD::rscl::comm::Publisher<senseAD::msg::std_msgs::Image>::Ptr publisher_;
};

}
}
