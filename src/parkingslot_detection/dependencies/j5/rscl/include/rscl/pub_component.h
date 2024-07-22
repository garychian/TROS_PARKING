/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <vector>
#include <memory>

#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/fy_test/image.capnp.h"
#include "ad_msg_idl/std_msgs/raw_data.capnp.h"
#include "ad_msg_idl/ad_perception/quad_parking_slots.capnp.h"
#include "ad_msg_idl/fy_test/status_dec_fusion.capnp.h"
#include "ad_msg_idl/ad_perception/base.capnp.h"

// typedef struct QuadParkingSlot{
//    Point2f oriTl;
//    Point2f oriTr;
//    Point2f oriBl;
//    Point2f oriBr;

//    int32_t slotType;
//    uint32_t label;

// }QuadParkingSlot;

// typedef struct QuadParkingSlotsResult{
//    uint64_t frameTimestampNs;
//    int32_t sensorId;
//    QuadParkingSlot quadParkingSlotList[];
// }QuadParkingSlotsResult;

// typedef std::shared_ptr<QuadParkingSlotsResult> QuadParkingSlotsResultPtr;

class PubComponent : public senseAD::rscl::component::TimerComponent {
 public:
    PubComponent(const std::string &platform);
    ~PubComponent() override;

    void OnQuadParkingSlotsCmd(const std::shared_ptr<ReceivedMsg<senseAD::msg::perception::QuadParkingSlots>> &msg);
   //  bool convertMsgToQuadParkingSlot(const senseAD::msg::perception::QuadParkingSlots::Reader &msg,
   //                               std::vector<QuadParkingSlotsResultPtr> &quad_slots_info);
    bool OnInit() override;
    bool OnProc() override;
    bool OnReset() override;
    void OnShutdown() override;

 private:
    rscl::comm::PublisherBase::Ptr s32g_pub_;
    std::string platform_;
    // QCG add
    rscl::comm::PublisherBase::Ptr perception_pub_;
    rscl::comm::SubscriberBase::Ptr image_sub_;
    std::shared_ptr<senseAD::rscl::comm::Subscriber<senseAD::msg::perception::QuadParkingSlots>> sub_quad_parking_slots_;
    std::shared_ptr<senseAD::rscl::comm::Publisher<senseAD::msg::std_msgs::Image>> pub_image_;
   //  std::shared_ptr<std::vector<QuadParkingSlotsResultPtr>> quad_parking_slots_;
};
