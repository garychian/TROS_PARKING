/*
 * Copyright (C) 2020 by SenseTime Group Limited. All rights reserved.
 * Jiang Qinhong <jiangqinhong@sensetime.com>
 */
#pragma once

#include <string>
#include <memory>

// #include "common/class_helper.hpp"
#include "common/error.hpp"
// #include "proxy.hpp"
#include "core/CommonFactory.hpp"

// #include "proto/perception_camera.pb.h"  // NOLINT

namespace senseAD {
namespace perception {
namespace camera {

class Operator {
 public:
    Operator() = default;

    virtual ~Operator() = default;

    // apStatus_t Init(const OperatorParameter &param) { return InnerInit(param); }

    virtual apStatus_t Process(Proxy &proxy_) = 0;

    virtual std::string Name() = 0;

 protected:
    // virtual apStatus_t InnerInit(const OperatorParameter &param) = 0;

    apStatus_t AllocMemory(size_t size, MemoryType type);

    apStatus_t ReleaseMemory();

 private:
    DISABLE_COPY_AND_ASSIGN(Operator);
};  // class Operator

using OperatorPtr = std::shared_ptr<Operator>;

#define REGISTER_OPERATOR(name) \
    REGISTER_COMMON_ENGINE(senseAD::perception::camera, name, Operator, name)
#define INFLATE_OPERATOR(name) \
    CommonFactory<Operator>::GetInstance().Create(name)

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
