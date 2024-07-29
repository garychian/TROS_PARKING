#pragma once
#include "swc_embed/image_subscriber.h"


namespace senseAD {
namespace swc_embed {

class ImageHorizonIONSubscriber : public ImageSubscriber {
  public:
    ImageHorizonIONSubscriber(const ImageSubscriberParam &param);

  private:
    senseAD::rscl::comm::Subscriber<senseAD::msg::std_msgs::Image>::Ptr subscriber_;
    void OnImage(
        const std::shared_ptr<ReceivedMsg<senseAD::msg::std_msgs::Image>> &msg);
};

}
}
