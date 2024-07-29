#pragma once

#include "swc_embed/cuda/cuda_define.h"

#ifdef ENABLE_CUDA
#include "swc_embed/cuda/cuda_image_subscriber.h"
#include "swc_embed/cuda/cuda_image_publisher.h"

namespace senseAD {
namespace swc_embed {
REGISTER_PUBLISHER_TYPE(ImageMemoryType::DEVICE, CudaImagePublisher)
REGISTER_SUBSCRIBER_TYPE(ImageMemoryType::DEVICE, CudaImageSubscriber)
}
}
#endif
