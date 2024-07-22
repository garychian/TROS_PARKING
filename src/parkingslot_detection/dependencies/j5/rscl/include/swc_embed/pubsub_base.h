#ifndef __PUBSUB_BASE__
#define __PUBSUB_BASE__

#include <ad_msg_idl/std_msgs/raw_data.capnp.h>
#include "ad_common/data_type/base.hpp"
#include "opencv2/core/core.hpp"

namespace senseAD {
namespace swc_embed {
    enum class ImageMemoryType {
        // HOST内存
        CPU,
        // 设备内存，TODO cambricon publish VB内存
        DEVICE,
        // cnsysMallocExt出的设备内存
        CAMBRICON_SYS,
        // 根据实现自动分配内存存类型
        AUTO,
    };

    struct IpcImage {
        senseAD::ImageHeader_t header;
        int pixel_format;
        int mem_type = 0;
        void *device_ptr;
        void *host_ptr;
        void *context;
    };

    using ReceiveIpcImage = IpcImage;

    int pixel2Cvtype(int format);
    int cvtype2Pixel(int cvtype);

    IpcImage CvMatToIpcImage(const cv::Mat &image, uint64_t timestamp);
    cv::Mat IpcImageToCvMat(const IpcImage &image);

    int encoding2Pixcel(const capnp::Text::Reader &encoding);
    std::string pixcel2Encoding(int format);
}
}

#endif
