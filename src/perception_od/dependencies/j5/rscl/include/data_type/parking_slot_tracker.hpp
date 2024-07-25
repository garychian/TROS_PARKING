/*
 * Copyright (C) 2021 by SenseTime Group Limited. All rights reserved.
 * Zhao Ming <zhaoming@senseauto.com>
 */
#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <memory>
#include <vector>
#include <data_type/data_type.hpp>
#include <common/common_log.hpp>

#define PI 3.141592654
#define Rad2Deg (180.0 / PI)  // radius convert to degree ratio
#define Deg2Rad (PI / 180.0)  // degree convert to radius ratio

namespace senseAD {
namespace perception {
namespace camera {

// class holds the parking slot tracker
class ParkingSlotTracker {
 public:
    typedef std::shared_ptr<ParkingSlotTracker> Ptr;

 public:
    ParkingSlotTracker() { parking_slot_.reset(new StructureParkingSlot); }
    explicit ParkingSlotTracker(const StructureParkingSlot::Ptr& parking_slot)
        : parking_slot_(parking_slot) {}
    ~ParkingSlotTracker() {}

    //@brief: predict corners position
    void Predict(const Eigen::Matrix4d& pre_pose) {
        Eigen::Matrix3d pre_rot = pre_pose.block<3, 3>(0, 0);
        Eigen::Vector3d pre_trans = pre_pose.block<3, 1>(0, 3);
        pre_corners = parking_slot_->GetFourVertexPoints();
        pre_prob = parking_slot_->cor_prob_;
        for (int i = 0; i < pre_corners.size(); i++) {
            pre_prob[i] += 0.05 * Eigen::Matrix3d::Identity();
            AD_LINFO(DEBUG_FOR_SLOT)
                << "pred corner" << i << " = " << pre_corners[i].transpose()
                << ", pred corner prob" << i << " = " << pre_prob[i](0, 0)
                << " " << pre_prob[i](1, 1) << " " << pre_prob[i](2, 2);
        }
    }

    // @brief: update by one parking slot observation or parking slot tracker
    void Update(const StructureParkingSlot::Ptr& observ) {
        AD_LINFO(observ_confidence)
            << "before, observ confidence is " << observ->confidence_;

        AD_LINFO(DEBUG_FOR_SLOT)
            << " parking_slot_ slots data:   " << std::endl
            << " center : " << parking_slot_->center_.transpose() << std::endl
            << " wd     : " << parking_slot_->wide_direction_.transpose()
            << std::endl
            << " ld     : " << parking_slot_->long_direction_.transpose()
            << std::endl
            << " type   : " << +parking_slot_->ps_type_ << std::endl
            << " width  : " << parking_slot_->width_ << std::endl
            << " length : " << parking_slot_->length_ << std::endl
            << std::endl
            << " observ slots data: " << std::endl
            << " center : " << observ->center_.transpose() << std::endl
            << " wd     : " << observ->wide_direction_.transpose() << std::endl
            << " ld     : " << observ->long_direction_.transpose() << std::endl
            << " type   : " << +observ->ps_type_ << std::endl
            << " width  : " << observ->width_ << std::endl
            << " length : " << observ->length_ << std::endl
            << " confidence : " << observ->confidence_ << std::endl;
        // add KFP index
        parking_slot_->AddKFPkgIdxes(observ->kf_pkg_idxes_);

        // weight average center, width and length
        std::vector<Eigen::Vector3d> four_corner_pred =
            parking_slot_->GetFourVertexPoints();
        std::vector<Eigen::Vector3d> four_corner_measure =
            observ->GetFourVertexPoints();
        std::vector<Eigen::Matrix3d> four_corner_pred_prob =
            parking_slot_->cor_prob_;
        std::vector<Eigen::Matrix3d> four_corner_measure_prob =
            observ->cor_prob_;
        auto isBadPoint = [](Eigen::Vector3d point) {
            return (std::isnan(point[0]) || std::isnan(point[1]) ||
                    std::isnan(point[2]));
        };

        int add_id = -1;
        for (int i = 0; i < four_corner_measure.size(); i++) {
            if (isBadPoint(four_corner_measure[i])) continue;
            double min_dist = std::numeric_limits<double>::max(), dist;
            int min_id = 0;
            for (int j = 0; j < four_corner_pred.size(); j++) {
                if (isBadPoint(four_corner_pred[j])) continue;
                dist = (four_corner_measure[i] - four_corner_pred[j]).norm();
                if (dist < min_dist) {
                    min_dist = dist;
                    min_id = j;
                }
            }
            if (min_dist > 0.8) continue;
            add_id = (i - min_id);
            break;
        }
        add_id = add_id < 0 ? add_id + four_corner_pred.size() : add_id;

        int updated_corners_count = 0;
        for (int i = 0; i < four_corner_pred.size(); i++) {
            if (isBadPoint(four_corner_measure[(i + add_id) % 4])) {
                continue;
            }
            AD_LINFO(UpdatePS)
                << "corner id : " << i << ", " << add_id
                << ", pred = " << four_corner_pred[i].transpose()
                << ", measure = " << four_corner_measure[(i + add_id) % 4];
            AD_LINFO(UpdatePS)
                << "raw corner prob = " << four_corner_pred_prob[i](0, 0) << " "
                << four_corner_pred_prob[i](1, 1) << " "
                << four_corner_pred_prob[i](2, 2);
            AD_LINFO(UpdatePS)
                << "obs corner prob = "
                << four_corner_measure_prob[(i + add_id) % 4](0, 0) << " "
                << four_corner_measure_prob[(i + add_id) % 4](1, 1) << " "
                << four_corner_measure_prob[(i + add_id) % 4](2, 2);
            if ((four_corner_pred[i] - four_corner_measure[(i + add_id) % 4])
                    .norm() > 1) {
                continue;
            }
            four_corner_pred[i] = WeightAverage(
                pre_corners[i], four_corner_measure[(i + add_id) % 4],
                pre_prob[i], four_corner_measure_prob[(i + add_id) % 4]);
            four_corner_pred_prob[i] = pre_prob[i];
            updated_corners_count++;
            AD_LINFO(UpdatePS)
                << "updated corner prob = " << four_corner_pred_prob[i](0, 0)
                << " " << four_corner_pred_prob[i](1, 1) << " "
                << four_corner_pred_prob[i](2, 2);
        }
        if (updated_corners_count == 4) {
            is_all_corners_updated = true;
        } else {
            is_all_corners_updated = false;
        }

        if (std::abs(observ->confidence_) > 0.01) {
            if (parking_slot_->wide_direction_.dot(observ->wide_direction_) >
                0) {
                parking_slot_->wide_direction_ =
                    (parking_slot_->wide_direction_ *
                         parking_slot_->confidence_ +
                     observ->wide_direction_ * observ->confidence_ * 0.5)
                        .normalized();
            } else {
                parking_slot_->wide_direction_ =
                    (parking_slot_->wide_direction_ *
                         parking_slot_->confidence_ -
                     observ->wide_direction_ * observ->confidence_ * 0.5)
                        .normalized();
            }
            if (parking_slot_->long_direction_.dot(observ->long_direction_) >
                0) {
                parking_slot_->long_direction_ =
                    (parking_slot_->long_direction_ *
                         parking_slot_->confidence_ +
                     observ->long_direction_ * observ->confidence_ * 0.5)
                        .normalized();
            } else {
                parking_slot_->long_direction_ =
                    (parking_slot_->long_direction_ *
                         parking_slot_->confidence_ -
                     observ->long_direction_ * observ->confidence_ * 0.5)
                        .normalized();
            }
        }

        parking_slot_->SetCornerPoints(four_corner_pred);
        parking_slot_->cor_prob_ = four_corner_pred_prob;
        parking_slot_->car_pos_ = observ->car_pos_;
        parking_slot_->confidence_ =
            (observ->confidence_ * 0.8 + parking_slot_->confidence_ * 0.2);
        parking_slot_->EstimateCorners();

        AD_LINFO(DEBUG_FOR_SLOT)
            << " after Update, slots data :   " << std::endl
            << " center : " << parking_slot_->center_.transpose() << std::endl
            << " wd     : " << parking_slot_->wide_direction_.transpose()
            << std::endl
            << " ld     : " << parking_slot_->long_direction_.transpose()
            << std::endl
            << " type   : " << +parking_slot_->ps_type_ << std::endl
            << " width  : " << parking_slot_->width_ << std::endl
            << " length : " << parking_slot_->length_ << std::endl
            << " confidence : " << parking_slot_->confidence_ << std::endl
            << " fusion center: " << parking_slot_->id_ << " "
            << four_corner_pred[0].transpose() << " "
            << four_corner_pred[1].transpose() << " "
            << four_corner_pred[2].transpose() << " "
            << four_corner_pred[3].transpose() << " "
            << parking_slot_->car_pos_.transpose();
    }

    // @brief: get parking slot interface
    StructureParkingSlot::Ptr& GetParkingSlot() { return parking_slot_; }
    const StructureParkingSlot::Ptr& GetParkingSlot() const {
        return parking_slot_;
    }

    // @brief: KF compute
    template <typename T>
    T WeightAverage(const T& pre_data,
                    const T& obs_data,
                    Eigen::Matrix3d& pre_prob,
                    Eigen::Matrix3d& obs_prob) {
        if (std::isnan(pre_prob(0, 0))) {
            AD_LINFO(DEBUG_FOR_SLOT) << "predicted corner is bad";
            pre_prob = obs_prob;
            return obs_data;
        }
        if ((pre_data - obs_data).norm() > 0.8) {
            AD_LINFO(DEBUG_FOR_SLOT) << "abnomal measurement";
            return pre_data;
        }

        Eigen::Matrix3d K = pre_prob * (pre_prob + obs_prob).inverse();
        T weighted_data = pre_data + K * (obs_data - pre_data);
        pre_prob = (Eigen::Matrix3d::Identity() - K) * pre_prob;
        AD_LINFO(DEBUG_FOR_SLOT) << "K = " << K << std::endl
                                 << "Update Corner Succeed";
        return weighted_data;
    }

    // fused parking slot
    StructureParkingSlot::Ptr parking_slot_;
    std::vector<Eigen::Vector3d> pre_corners;
    std::vector<Eigen::Matrix3d> pre_prob;
    bool is_all_corners_updated;
};

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
