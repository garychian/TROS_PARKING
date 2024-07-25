#pragma once

#if defined(BUILD_HORIZON) || defined(BUILD_J5)
#include "swc_embed/horizon/device_image_subscriber.h"
#include "swc_embed/horizon/device_image_publisher.h"

namespace senseAD {
namespace swc_embed {
REGISTER_SUBSCRIBER_TYPE(ImageMemoryType::DEVICE, ImageHorizonIONSubscriber)
}
}
#endif
