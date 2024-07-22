#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include "ad_log/ad_log.hpp"
#include "ad_rscl/comm/node.h"
#include "swc_embed/pubsub_base.h"


#define REGISTER_SUBSCRIBER_TYPE(type_value, class_type)                       \
    namespace {                                                                \
    static struct ImageSubscriberRegister##class_type {                        \
        ImageSubscriberRegister##class_type() {                                \
            ImageSubscriberFactory::Instance().Register<class_type>(           \
                type_value, #class_type);                                      \
        }                                                                      \
    } _register_##class_type;                                                  \
    }  // namespace

namespace senseAD {
namespace swc_embed {

using ImageSubscriberCallback = std::function<void(const std::shared_ptr<ReceiveIpcImage>&)>;

struct ImageSubscriberParam {
    senseAD::rscl::comm::Node *node;
    std::string topic;
    ImageSubscriberCallback cb;
    ImageMemoryType type;
    int channel;

    ImageSubscriberParam(
        senseAD::rscl::comm::Node *node, const std::string &topic,
        const ImageSubscriberCallback &cb, ImageMemoryType type, int channel = 3):
        node(node), topic(topic), cb(cb), type(type), channel(channel)
    {}
};

class ImageSubscriber {
  public:
    using Ptr = std::shared_ptr<ImageSubscriber>;
    virtual ~ImageSubscriber();

  protected:
    ImageSubscriber(const ImageSubscriberCallback &cb);

    ImageSubscriberCallback callback_;
};


typedef std::unordered_map<
    int,
    std::function<bool(const ImageSubscriberParam& param, ImageSubscriber::Ptr* sub)>>
    ImageSubscriberRegisterMap;

class ImageSubscriberFactory {
  public:
    static ImageSubscriberFactory& Instance();
    ImageSubscriberFactory() {}

    std::shared_ptr<ImageSubscriber> Create(
        const ImageSubscriberParam &param);

    template<class T>
    void Register(ImageMemoryType type_value, const std::string &name) {
        if (register_map_.find(static_cast<int>(type_value)) != register_map_.end()) {
            AD_LWARN(ImageSubscriberFactory) << "ImageSubscriber for ImageMemoryType "
                << (int)type_value << " already registered";
            return;
        }
        AD_LDEBUG(ImageSubscriberFactory) << "Register ImageSubscriber " << name << " for ImageMemoryType " << (int)type_value;
        register_map_[static_cast<int>(type_value)] =
            [](const ImageSubscriberParam& param, ImageSubscriber::Ptr* res) {
                *res = std::dynamic_pointer_cast<ImageSubscriber>(
                    std::make_shared<T>(param));
            return true;
        };
    }
  private:
    ImageSubscriberRegisterMap register_map_;
};


class CapnpImageSubscriber : public ImageSubscriber {
  public:
    CapnpImageSubscriber(const ImageSubscriberParam &cb);

  private:
    senseAD::rscl::comm::Subscriber<senseAD::msg::std_msgs::Image>::Ptr subscriber_;
    void OnImage(const std::shared_ptr<ReceivedMsg<senseAD::msg::std_msgs::Image>> &msg);
};

}
}
