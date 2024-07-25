/*
 * Copyright (C) 2021-2023 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 * Chen Zhenan <chenzhenan@senseauto.com>
 */

#include "perception_camera_base.h"

namespace senseAD {
namespace avp_perception {

class PerceptionOdComponent : public PerceptionCameraBaseComponent {
 public:
    PerceptionOdComponent() {}
    bool OnInit() override;

 private:
    bool publishPinholeObject(const std::vector<std::string> &camera_names,
                              const ImagePublisher::Ptr &,
                              const PerceptionProxy &);
    bool publishPinholeObjectToDraw(
        const std::vector<std::string> &camera_names,
        const senseAD::rscl::comm::Publisher<
            senseAD::msg::perception::ObstacleImages>::Ptr &,
        const PerceptionProxy &);
    bool VisualizeAvpFreespace(const std::string &camera_name,
                               const std::string &seg_name,
                               const PerceptionProxy &proxy);

    static std::unordered_map<int, const cv::Scalar> rectangle_colors;

    std::vector<ImagePublisher::Ptr> ipc_freespace_pub_;
    std::vector<ImagePublisher::Ptr> mono3d_ipc_freespace_pub_;
    int draw_image_size_od_ = 960;
    int image_todraw_h_ = 360;
    int image_todraw_w_ = 480;
    bool draw_freespace_ = true;

    bool checkOnAllMsgReceived(
        std::shared_ptr<transaction::TransactionSyncedMsg> synced_data)
        override;
    bool checkFinishCallback(const PerceptionProxy &proxy, void *flag) override;
    bool checkOnImage(size_t idx,
                      const std::shared_ptr<ReceiveIpcImage> &image) override;
    bool checkPublishSegmentIpc(std::vector<ImagePublisher::Ptr> &,
                                std::string seg_name,
                                const PerceptionProxy &) override;
};

ASCL_REGISTER_COMPONENT(PerceptionOdComponent)

}  // namespace avp_perception
}  // namespace senseAD
