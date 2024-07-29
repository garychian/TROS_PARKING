/*
 * Copyright (C) 2018-2020 by SenseTime Group Limited. All rights reserved.
 * He Yuan <heyuan@sensetime.com>
 */

#pragma once

#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpragmas"
// #pragma GCC diagnostic ignored "-fpermissive"
// #include "fastcv.hpp"
// #pragma GCC diagnostic pop

#include "Eigen/Core"

#include "data_type/object_label.hpp"
#include "data_type/bbox_2d.hpp"
#include "ad_common/data_type/base.hpp"

namespace senseAD {
namespace perception {
namespace camera {

enum class VisibleSurfaceLabel {
    // all direction
    BACK = 0,
    BACKRIGHT = 1,
    RIGHT = 2,
    RIGHTFRONT = 3,
    FRONT = 4,
    FRONTLEFT = 5,
    LEFTBACK = 6,
    LEFT = 7,
    UNKNOWN = 8
};

struct DetectionInfo {
    ObjectLabel label = ObjectLabel::UNKNOWN;
    float confidence = -1.0f;
    float objectness = 1.0f;
    Bbox2D box;
    Eigen::VectorXf feature;
    // xyz in camera corr, length, width, height,yaw
    Eigen::Matrix<float, 1, 7> box3d;
    int static_state = -1;
    float attr_score = 0.f;
};

struct TrackInfo {
    bool is_tracked = false;
    float tracker_confidence = -1.0f;
    uint64_t track_id = 0;
    uint64_t track_age = 0;
    Eigen::VectorXf feature;
};

const Eigen::Vector3f kDefaultBBox3d = Eigen::Vector3f(4.0, 1.8, 1.55);

struct ShapeInfo {
    // shape: length/width/height
    Eigen::Vector3f shape = kDefaultBBox3d;
};

struct DirectionInfo {
    // direction info
    Eigen::Vector3f direction = Eigen::Vector3f(1, 0, 0);
    // increase anticlockwise
    float theta = 0.0f;  // [rad], range:[-pi,pi]
    VisibleSurfaceLabel surface = VisibleSurfaceLabel::UNKNOWN;
    float surface_score = 0.0f;
};

enum class DepthStatus { DEPTH_INVALID = -1, DEPTH_VALID };

struct PositionInfo {
    // center position info
    Eigen::Vector3f position = Eigen::Vector3f(0, 0, 0);
    DepthStatus depth_status = DepthStatus::DEPTH_INVALID;
};

struct MotionInfo {
    // motion info
    // object motion param: velocity, acceleration. yaw rate
    Eigen::Vector3f velocity = Eigen::Vector3f(0, 0, 0);
    // brief acceleration of the object, required
    Eigen::Vector3f acceleration = Eigen::Vector3f(0, 0, 0);
    // yaw rate
    float yaw_rate = 0.0;       // [rad/s]
    float var_yaw_rate = 0.0f;  // [rad/s] standard deviation
};

struct AttributeInfo {
    bool valid = false;
    // landmark
    std::vector<Eigen::Vector2f> landmark;
    std::vector<float> landmark_scores;
    // landmark 4point
    std::vector<Eigen::Vector2f> landmark4;
    std::vector<float> landmark4_scores;
    // landmark 9point
    std::vector<Eigen::Vector2f> landmark9;
    std::vector<float> landmark9_scores;
};

enum MotionStatus {
    MOTION_STATUS_UNKNOWN,
    MOTION_STATUS_MOVING,
    MOTION_STATUS_STILL
};

struct Trajectory {
    struct TrajectoryPoint {
        uint64_t delta_t_ns;
        Eigen::Vector3f center;
        Eigen::Vector3f direction;
    };
    MotionStatus motion_status = MOTION_STATUS_UNKNOWN;
    float32_t confidence = 0.0f;
    std::vector<TrajectoryPoint> points;
};

typedef std::vector<Trajectory> TrajectoryInfo;

// camera object
struct CameraObject {
    // timestamp
    uint64_t timestamp_ns = 0;
    // frame sequence id
    uint64_t frame_id = 0;
    // source camera name
    std::string camera_name = "unknown_camera";
    // requirement infos
    DetectionInfo det_info;
    TrackInfo track_info;
    ShapeInfo shape_info;
    PositionInfo position_info;
    DirectionInfo direction_info;
    MotionInfo motion_info;
    AttributeInfo attribute_info;
    TrajectoryInfo trajectory_info;
};
typedef std::shared_ptr<CameraObject> CameraObjectPtr;
typedef std::shared_ptr<const CameraObject> CameraObjectConstPtr;

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
