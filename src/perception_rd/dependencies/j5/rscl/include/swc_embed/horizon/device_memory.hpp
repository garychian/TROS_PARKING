#include "swc_embed/device_memory.h"
#include <memory>
#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace swc_embed {
namespace horizon {

using CommonMemory = DeviceMemory;

class HorizonIonMemory : public CommonMemory {
public:
   HorizonIonMemory(size_t count_bytes) {}
   HorizonIonMemory(CommonMemory *cm) {}
   ~HorizonIonMemory() {}

   const CommonMemory* GetMemoryDescribe() {}

private:
   void Release() {}
   void Allocate() {}

    CommonMemory *mem_ptr_;
    bool need_release_;
};

using HorizonIonMemoryPtr = std::shared_ptr<HorizonIonMemory>;

} // namespace horizon 
} // namespace senseAD {
} // namespace swc_embed {
