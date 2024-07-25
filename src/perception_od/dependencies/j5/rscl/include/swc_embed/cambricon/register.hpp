#pragma once

#ifdef BUILD_CAMBRICON
#include "swc_embed/cambricon/device_image_subscriber.h"
#include "swc_embed/cambricon/device_image_publisher.h"

namespace senseAD {
namespace swc_embed {
REGISTER_SUBSCRIBER_TYPE(ImageMemoryType::DEVICE, ImageVGUSubscriber)
REGISTER_PUBLISHER_TYPE(ImageMemoryType::CAMBRICON_SYS, DeviceImagePublisher)
REGISTER_SUBSCRIBER_TYPE(ImageMemoryType::CAMBRICON_SYS, DeviceImageSubscriber)
}
}
#endif
