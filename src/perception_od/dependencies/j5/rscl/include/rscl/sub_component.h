/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Kit Fung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <deque>
#include <vector>
#include <string>

#include "ad_rscl/ad_rscl.h"
#include "ad_msg_idl/fy_test/image.capnp.h"
#include "ad_msg_idl/fy_test/status_dec_fusion.capnp.h"

class SubComponent : public senseAD::rscl::component::TimerComponent {
 public:
    explicit SubComponent(const std::string &platform);
    ~SubComponent() override;

    bool OnInit() override;
    bool OnProc() override;
    bool OnReset() override;
    void OnShutdown() override;

 private:
    rscl::comm::SubscriberBase::Ptr j5_sub_;
    rscl::comm::SubscriberBase::Ptr s32g_sub_;
    std::string platform_;
};
