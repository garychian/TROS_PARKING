#pragma once
#include <stdint.h>
#include <stddef.h>

namespace senseAD {
namespace swc_embed {
class VideoEncoder {
  public:
    virtual bool encode(uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) = 0;
};

class VideoDecoder {
  public:
    virtual bool encode(uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) = 0;
};
}
}
