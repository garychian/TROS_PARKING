/*
 * Copyright (C) 2018-2020 by SenseTime Group Limited. All rights reserved.
 * He Yuan <heyuan@sensetime.com>
 */

#pragma once

#include <string>
#include <map>
#include <unordered_map>

namespace senseAD {
namespace perception {
namespace camera {

enum class ObjectLabel {
    // all object
    OBJECT_ALL = -1,
    UNKNOWN = 0,
    // Ped and vehicle
    VEHICLE = 1,
    PEDESTRIAN = 2,
    TRAFFICBARREL = 3,
    CART = 4,
    COLUMN = 5,
    WHEELSTOP = 6,
    NOPARKINGSIGN = 7,
    LIMITLOCK = 8,
    BIKE = 9
};

enum class BinaryDirection {
    UNKNOWN = -1,
    BINARY_DIRECTION_FRONT,
    BINARY_DIRECTION_BACK
};

static const std::unordered_map<std::string, ObjectLabel> kLabelToNum{
    {"OBJECT_ALL", ObjectLabel::OBJECT_ALL},
    {"__background__", ObjectLabel::UNKNOWN},
    {"UNKNOWN", ObjectLabel::UNKNOWN},
    // pvbp && cyclist
    {"VEHICLE", ObjectLabel::VEHICLE},
    {"PEDESTRIAN", ObjectLabel::PEDESTRIAN},
    {"TRAFFICBARREL", ObjectLabel::TRAFFICBARREL},
    {"CART", ObjectLabel::CART},
    {"COLUMN", ObjectLabel::COLUMN},
    {"WHEELSTOP", ObjectLabel::WHEELSTOP},
    {"NOPARKINGSIGN", ObjectLabel::NOPARKINGSIGN},
    {"LIMITLOCK", ObjectLabel::LIMITLOCK}};

// static const std::map<ObjectLabel, std::string> kObjectLabelToString{
//     {ObjectLabel::OBJECT_ALL, "OBJECT_ALL"},
//     {ObjectLabel::UNKNOWN, "UNKNOWN"},
//     {ObjectLabel::VEHICLE, "VEHICLE"},
//     {ObjectLabel::PEDESTRIAN, "PEDESTRIAN"},
//     {ObjectLabel::TRAFFICBARREL, "TRAFFICBARREL"},
//     {ObjectLabel::CART, "CART"}};

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
