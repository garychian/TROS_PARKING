#pragma once
#include "core/common.h"

namespace senseAD {
namespace perception {
namespace camera {

struct ImageMeta {
    float32_t resize_scale_h = 2.0F;
    float32_t resize_scale_w = 2.0F;
    uint32_t crop_h = 0;
    uint32_t crop_w = 0;
};
}  // namespace camera
}  // namespace perception
}  // namespace senseAD
