/*
 * Copyright (C) 2020 by SenseTime Group Limited. All rights reserved.
 * He Yuan <heyuan@sensetime.com>
 */

#pragma once

#include "Eigen/Core"
#include "Eigen/Geometry"
// #include "fastcv.hpp"

namespace senseAD {
namespace perception {
namespace camera {

// typedef HPC::fastcv::Point2i Point2i;
// typedef HPC::fastcv::Point2f Point2f;
// typedef HPC::fastcv::Point3f Point3f;
// typedef HPC::fastcv::Recti Rect;

typedef Eigen::NumTraits<double> EigenDouble;
typedef Eigen::MatrixXd MatXd;
typedef Eigen::VectorXd VecXd;
typedef Eigen::Matrix<double, 2, 2> Mat2d;
typedef Eigen::Matrix<double, 3, 3> Mat3d;
typedef Eigen::Matrix<double, 4, 4> Mat4d;
typedef Eigen::Matrix<double, 2, 1> Vec2d;
typedef Eigen::Matrix<double, Eigen::Dynamic, 8> MatX8d;
typedef Eigen::Vector3d Vec3d;
typedef Eigen::Vector4d Vec4d;
typedef Eigen::Vector2f Vec2f;
typedef Eigen::Vector3f Vec3f;
typedef Eigen::Vector4f Vec4f;

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
