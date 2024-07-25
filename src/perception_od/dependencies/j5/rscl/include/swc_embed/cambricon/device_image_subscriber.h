#pragma once
#include "swc_embed/image_subscriber.h"


namespace senseAD {
namespace swc_embed {
class ImageVGUSubscriber : public ImageSubscriber {
  public:
    ImageVGUSubscriber(const ImageSubscriberParam &param);

  private:
    senseAD::rscl::comm::Subscriber<senseAD::msg::std_msgs::ImageVGU>::Ptr subscriber_;
    void OnImageVGU(const std::shared_ptr<ReceivedMsg<senseAD::msg::std_msgs::ImageVGU>> &msg);
};


class DeviceImageSubscriber : public ImageSubscriber {
  public:
    DeviceImageSubscriber(const ImageSubscriberParam &param);

  private:
    int channel_;
    senseAD::rscl::comm::Subscriber<senseAD::msg::std_msgs::ImageVGU>::Ptr device_subscriber_;
    void OnImageVGU(const std::shared_ptr<ReceivedMsg<senseAD::msg::std_msgs::ImageVGU>> &memblock);
};

}
}
