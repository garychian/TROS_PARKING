#pragma once
#include "swc_embed/image_subscriber.h"
#include "swc_embed/cuda/cuda_define.h"


namespace senseAD {
namespace swc_embed {
class CudaImageSubscriber : public ImageSubscriber {
  public:
    CudaImageSubscriber(const ImageSubscriberParam &param);

  private:
    int channel_;
    senseAD::rscl::comm::CudaSubscriber::Ptr cuda_subscriber_;
    void OnCudaImage(const std::shared_ptr<senseAD::rscl::comm::Memblock> &memblock);
};
}
}
