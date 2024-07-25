/*
 * Copyright (C) 2021-2023 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 * Chen Zhenan <chenzhenan@senseauto.com>
 */

#include "perception_camera_base.h"


namespace senseAD {
namespace avp_perception {

class PerceptionRdComponent : public PerceptionCameraBaseComponent {
 public:
    PerceptionRdComponent() {}
    bool OnInit() override;

 private:
    bool publishParkingSlot(const std::string &camera_name,
                            const senseAD::rscl::comm::Publisher<
                                senseAD::msg::perception::ParkingSlots>::Ptr &,
                            const PerceptionProxy &);

    bool publishIpmSeg(const ImagePublisher::Ptr &publisher,
                       const PerceptionProxy &);

    bool publishImage(const std::string &camera_name,
                      size_t dst_height,
                      size_t dst_width,
                      const ImagePublisher::Ptr &,
                      const PerceptionProxy &,
                      PublishImageCallback);

    bool publishParkingSlotsQuad(
        const senseAD::rscl::comm::Publisher<
            senseAD::msg::perception::QuadParkingSlots>::Ptr &,
        const PerceptionProxy &);

    bool checkOnAllMsgReceived(
        std::shared_ptr<transaction::TransactionSyncedMsg> synced_data)
        override;
    bool checkFinishCallback(const PerceptionProxy &proxy, void *flag) override;
    bool checkOnImage(size_t idx,
                      const std::shared_ptr<ReceiveIpcImage> &image) override;
    bool checkPublishSegmentIpc(std::vector<ImagePublisher::Ptr> &,
                                std::string seg_name,
                                const PerceptionProxy &) override;

    std::vector<ImagePublisher::Ptr> ipc_ipm_pub_;
};


ASCL_REGISTER_COMPONENT(PerceptionRdComponent)

}  // namespace avp_perception
}  // namespace senseAD
