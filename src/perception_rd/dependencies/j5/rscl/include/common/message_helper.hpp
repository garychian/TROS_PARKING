/*
 * Copyright (C) 2021-2022 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 */

#pragma once

// #include <ad_perception_msgs/Obstacles.h>
#include <ad_msg_idl/ad_perception/obstacles.capnp.h>
// #include <ad_perception_msgs/ParkingSlots.h>
#include <ad_msg_idl/ad_perception/parking_slots.capnp.h>
#include <ad_msg_idl/ad_perception/fusion_parking_slots.capnp.h>
#include <ad_msg_idl/ad_perception/quad_parking_slots.capnp.h>
#include <ad_msg_idl/avp_smart_slam/avp_smart_slam.capnp.h>
#include "ad_msg_idl/avp_state/avp_main_state.capnp.h"
// #include <ros/ros.h>

#include "data_type/object.hpp"
#include "data_type/slot_info.hpp"
#include "proxy.hpp"
#include "swc_embed/ad_log_helper.h"

namespace senseAD {

using namespace perception::camera;

namespace rscl_ad {

bool convertObstacleToRawObjMsg(
    const senseAD::perception::camera::StrMap<
        std::vector<senseAD::perception::camera::CameraObjectPtr>> &objs_map,
    const std::vector<std::string> &camera_names,
    const uint64_t timestamp_ns,
    senseAD::msg::perception::Obstacles::Builder &msg);

bool convertObstacleToMsg(
    const senseAD::perception::camera::StrMap<
        std::vector<senseAD::perception::camera::CameraObjectPtr>> &objs_map,
    const std::vector<std::string> &camera_names,
    const uint64_t timestamp_ns,
    senseAD::msg::perception::Obstacles::Builder &msg);

bool convertMsgToObstacle(
    const senseAD::msg::perception::Obstacles::Reader &msg,
    uint64_t &timestamp_ns,
    StrMap<std::vector<CameraObjectPtr>> &objs);

bool convertParkingSlotFrameToMsg(
    const std::vector<ParkingSlotResultPtr> &pss,
    const uint64_t timestamp_ns,
    senseAD::msg::perception::ParkingSlots::Builder &msg);

bool convertMsgToParkingSlotFrame(
    const senseAD::msg::perception::ParkingSlots::Reader &msg,
    uint64_t &timestamp_ns,
    std::vector<ParkingSlotResultPtr> &pss);

bool convertMsgToFusionParkingSlot(
    const senseAD::msg::avp_smart_slam::MapInfo::Reader &msg,
    std::vector<FusionParkingSlotResultPtr> &fusion_slot_info_origin,
    std::vector<FusionParkingSlotResultPtr> &fusion_slot_info);

bool convertFusionParkingSlotToMsg(
    const std::vector<FusionParkingSlotResultPtr> &pss,
    const std::vector<FusionParkingSlotResultPtr> &origin_pss,
    const uint64_t timestamp_ns,
    senseAD::msg::perception::FusionParkingSlots::Builder &msg);

bool syncFusionParkingSlot(
    const std::vector<FusionParkingSlotResultPtr> &dst,
    const std::vector<FusionParkingSlotResultPtr> &src);
 
bool convertParkingSlotQuadToMsg(
    const std::vector<ParkingSlotQuadPtr> &pss,
    const uint64_t timestamp_ns,
    senseAD::msg::perception::QuadParkingSlots::Builder &msg);

bool convertMsgToParkingSlotquad(
    const senseAD::msg::perception::QuadParkingSlots::Reader &msg,
    std::vector<ParkingSlotQuadPtr> &parking_slot_quad);

bool convertMsgEnumToMainState(
    senseAD::msg::avp_state::AvpMainState::State msg_state,
    senseAD::perception::camera::MainStateMachine &main_state);

}  // namespace rscl_ad
}  // namespace senseAD
