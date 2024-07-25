#pragma once

#include <memory>
#include <list>
#include <numeric>
#include <set>
#include <unordered_map>

#include "Eigen/Core"

namespace senseAD {
namespace perception {
namespace camera {

typedef struct ParkingSlotResult {
    Eigen::Vector2f tl, tr, bl, br;
    Eigen::Vector2f ori_tl, ori_tr, ori_bl, ori_br;
    float confidence;
    float width;
    float length;
    Eigen::Vector2f center;
    Eigen::Vector2f wide_direction;
    Eigen::Vector2f long_direction;
    bool is_occupied;
    uint8_t type;
} ParkingSlotResult;

enum class SlotState { UNAVALIABLE = 0, IS_PARKING = 1, UNKNOWN = 2 };
typedef struct FusionParkingSlotResult {
    uint64_t id;
    uint8_t ps_type;
    float width;
    float length;
    Eigen::Vector3f center;
    Eigen::Vector3f wide_direction;
    Eigen::Vector3f long_direction;
    bool is_occupancy;
    SlotState slot_state;
    float limiter;
    uint32_t source_type;
} FusionParkingSlotResult;

enum MainStateMachine {
    STATE_START_UP,
    STATE_AVP_FUNC_SELECT,
    // APA
    STATE_AVP_PARKING_QUEST,
    STATE_AVP_PARKING_STOP_DONE,
    STATE_AVP_PARKING_STEER_DOING,
    STATE_AVP_PARKING_STEER_DONE,
    STATE_AVP_PARKING_DRIVE,
    STATE_AVP_PARKING_STOP_DOING,
    STATE_AVP_PARKING_DONE,
    STATE_AVP_PARKING_STANDBY,
    STATE_AVP_PARKING_PAUSE,
    STATE_FLAG_AVP_PARKING_END,
    // HPA
    STATE_HPA_EXIT,
    STATE_HPA_MAPPING,
    STATE_HPA_MAPPING_VERIFY,
    STATE_HPA_MAPPING_DONE,
    STATE_HPA_SEARCHING,
    STATE_HPA_STANDBY,
    STATE_HPA_CRUISE,
    STATE_HPA_CRUISE_STOPPING,
    STATE_HPA_CRUISE_STOP_DONE,
    STATE_HPA_ALONG_PARKING,
    STATE_HPA_CRUISE_PAUSE,
    STATE_FLAG_HPP_CRUISE_END,
    // ELSE
    STATE_AVP_EMERGENCY_BRAKE
};

typedef struct AvpMainStateInfo {
    uint64_t timestamp_ns;
    uint64_t parking_cmd = 0;
    MainStateMachine main_state;
} AvpMainStateInfo;

typedef uint64_t structure_idx_t;
typedef uint64_t frame_idx_t;
class StructureParkingSlot {
 public:
    typedef std::shared_ptr<StructureParkingSlot> Ptr;
    typedef std::vector<std::shared_ptr<StructureParkingSlot>> PtrVec;
    typedef std::list<std::shared_ptr<StructureParkingSlot>> PtrList;
    typedef std::unordered_map<structure_idx_t, StructureParkingSlot::Ptr>
        PtrMap;

    StructureParkingSlot() {}
    StructureParkingSlot(frame_idx_t kf_pkg_idx) {
        kf_pkg_idxes_.emplace(kf_pkg_idx);
    }
    ~StructureParkingSlot() {}

    // @brief: set interface
    void SetCornerPoints(const std::vector<Eigen::Vector3d>& corner_points) {
        cor_tl_ = corner_points.at(0);
        cor_tr_ = corner_points.at(1);
        cor_br_ = corner_points.at(2);
        cor_bl_ = corner_points.at(3);
    }

    // @brief: get four vertex points
    std::vector<Eigen::Vector3d> GetFourVertexPoints() const {
        return std::vector<Eigen::Vector3d>{cor_tl_, cor_tr_, cor_br_, cor_bl_};
    }

    std::vector<Eigen::Vector3d> GetFourVertexPointsRaw() const {
        Eigen::Vector3d pt1 = center_ + 0.5 * width_ * wide_direction_ +
                              0.5 * length_ * long_direction_;
        Eigen::Vector3d pt2 = center_ + 0.5 * width_ * wide_direction_ -
                              0.5 * length_ * long_direction_;
        Eigen::Vector3d pt3 = center_ - 0.5 * width_ * wide_direction_ -
                              0.5 * length_ * long_direction_;
        Eigen::Vector3d pt4 = center_ - 0.5 * width_ * wide_direction_ +
                              0.5 * length_ * long_direction_;
        return std::vector<Eigen::Vector3d>{pt1, pt4, pt3, pt2};
    }

    bool EstimateCorners() {
        std::vector<Eigen::Vector3d> four_corner = GetFourVertexPoints();
        std::vector<Eigen::Matrix3d> four_corner_prob = cor_prob_;
        std::vector<int> bad_points_ids, good_points_ids;
        for (int i = 0; i < four_corner.size(); i++) {
            if (std::isnan(four_corner_prob[i](0, 0))) {
                bad_points_ids.push_back(i);
            } else {
                good_points_ids.push_back(i);
            }
        }
        int bad_point_count = bad_points_ids.size();
        if (bad_point_count >= 3) {
            return false;
        } else if (bad_point_count == 2) {
            Eigen::Vector3d edge, est_edge, axis_z(0, 0, 1);
            Eigen::Vector3d good_point_0, good_point_1, est_p1, est_p2;
            // special condition
            if ((bad_points_ids[1] - bad_points_ids[0]) % 2 == 0) {
                return false;
            }
            // normal condition
            int good_point_id_0 = good_points_ids[0];
            int good_point_id_1 = good_points_ids[1];
            if (good_points_ids[0] == 0 && good_points_ids[1] == 3) {
                std::swap(good_point_id_0, good_point_id_1);
            }
            good_point_0 = four_corner[good_point_id_0];
            good_point_1 = four_corner[good_point_id_1];
            edge = good_point_1 - good_point_0;
            est_edge = (edge.cross(axis_z)).normalized();
            if (est_edge.dot(car_pos_ - good_point_0) > 0) {
                est_edge *= -1;
            }
            if (edge.norm() < 4) {
                // long-edge length
                est_edge *= 5.35;
            } else {
                // short-edge length
                est_edge *= 2.5;
            }
            est_p1 = good_point_0 + est_edge;
            est_p2 = good_point_1 + est_edge;
            if ((four_corner[(good_point_id_0 + 3) % 4] - est_p1).norm() > 4) {
                est_edge *= -1;
                est_p1 = good_point_0 + est_edge;
                est_p2 = good_point_1 + est_edge;
            }
            four_corner[(good_point_id_0 + 3) % 4] = est_p1;
            four_corner[(good_point_id_1 + 1) % 4] = est_p2;
            SetCornerPoints(four_corner);
            return true;
        } else if (bad_point_count == 1) {
            int bad_id = bad_points_ids[0];
            Eigen::Vector3d est_p1, est_p2, edge_1, edge_3;
            edge_1 =
                four_corner[(bad_id + 1) % 4] - four_corner[(bad_id + 2) % 4];
            edge_3 =
                four_corner[(bad_id + 3) % 4] - four_corner[(bad_id + 2) % 4];
            est_p1 = four_corner[(bad_id + 1) % 4] + edge_3;
            est_p2 = four_corner[(bad_id + 3) % 4] + edge_1;
            four_corner[bad_id] = (est_p1 + est_p2) / 2;
            SetCornerPoints(four_corner);
            return true;
        }
        return true;
    }

    bool EstimatePS() {
        std::vector<Eigen::Vector3d> four_corner = GetFourVertexPoints();
        std::vector<Eigen::Matrix3d> four_corner_prob = cor_prob_;
        std::vector<int> bad_points_ids, good_points_ids;
        for (int i = 0; i < four_corner.size(); i++) {
            if (std::isnan(four_corner[i][0])) {
                bad_points_ids.push_back(i);
            } else {
                good_points_ids.push_back(i);
            }
        }
        int bad_point_count = bad_points_ids.size();
        if (bad_point_count >= 3) {
            return false;
        } else if (bad_point_count == 2) {
            Eigen::Vector3d edge, est_edge, axis_z(0, 0, 1);
            Eigen::Vector3d good_point_0, good_point_1, est_p1, est_p2;
            // special condition
            if ((bad_points_ids[1] - bad_points_ids[0]) % 2 == 0) {
                return false;
            }
            // normal condition
            int good_point_id_0 = good_points_ids[0];
            int good_point_id_1 = good_points_ids[1];
            if (good_points_ids[0] == 0 && good_points_ids[1] == 3) {
                std::swap(good_point_id_0, good_point_id_1);
            }
            good_point_0 = four_corner[good_point_id_0];
            good_point_1 = four_corner[good_point_id_1];
            edge = good_point_1 - good_point_0;
            est_edge = (edge.cross(axis_z)).normalized();
            if (est_edge.dot(car_pos_ - good_point_0) > 0) {
                est_edge *= -1;
            }
            if (edge.norm() < 4) {
                // long-edge length
                est_edge *= 5.35;
            } else {
                // short-edge length
                est_edge *= 2.5;
            }
            est_p1 = good_point_0 + est_edge;
            est_p2 = good_point_1 + est_edge;
            if ((four_corner[(good_point_id_0 + 3) % 4] - est_p1).norm() > 4) {
                est_edge *= -1;
                est_p1 = good_point_0 + est_edge;
                est_p2 = good_point_1 + est_edge;
            }
            four_corner[(good_point_id_0 + 3) % 4] = est_p1;
            four_corner[(good_point_id_1 + 1) % 4] = est_p2;
            // SetCornerPoints(four_corner);
        } else if (bad_point_count == 1) {
            int bad_id = bad_points_ids[0];
            Eigen::Vector3d est_p1, est_p2, edge_1, edge_3;
            edge_1 =
                four_corner[(bad_id + 1) % 4] - four_corner[(bad_id + 2) % 4];
            edge_3 =
                four_corner[(bad_id + 3) % 4] - four_corner[(bad_id + 2) % 4];
            est_p1 = four_corner[(bad_id + 1) % 4] + edge_3;
            est_p2 = four_corner[(bad_id + 3) % 4] + edge_1;
            four_corner[bad_id] = (est_p1 + est_p2) / 2;
            // SetCornerPoints(four_corner);
        }

        center_ = std::accumulate(four_corner.begin(), four_corner.end(),
                                  Eigen::Vector3d{0, 0, 0}) /
                  four_corner.size();
        wide_direction_ = (four_corner[1] + four_corner[2] - four_corner[0] -
                           four_corner[3]) /
                          2.0;
        long_direction_ = (four_corner[3] + four_corner[2] - four_corner[0] -
                           four_corner[1]) /
                          2.0;
        if (wide_direction_.norm() > long_direction_.norm()) {
            if (ps_type_ == 0) {
                std::swap(wide_direction_, long_direction_);
            }
        } else {
            if (ps_type_ == 1) {
                std::swap(wide_direction_, long_direction_);
            }
        }
        return true;
    }

    void AddKFPkgIdx(frame_idx_t idx) { kf_pkg_idxes_.emplace(idx); }

    void AddKFPkgIdxes(const std::set<frame_idx_t>& idxes) {
        for (const auto& idx : idxes) kf_pkg_idxes_.emplace(idx);
    }

    void SetKFPkgIdxes(const std::set<frame_idx_t>& idxes) {
        kf_pkg_idxes_ = idxes;
    }

    void SetKFPkgIdxes(std::set<frame_idx_t>&& idxes) {
        kf_pkg_idxes_ = std::move(idxes);
    }

    frame_idx_t GetEarliestKFPkgIdx() const { return *(kf_pkg_idxes_.begin()); }

    frame_idx_t GetLastKFPkgIdx() const { return *(kf_pkg_idxes_.rbegin()); }

    structure_idx_t id_;                  // unique id for structure
    uint8_t ps_type_;                     // parking slot type
    std::set<frame_idx_t> kf_pkg_idxes_;  // associated KFP indexes

    double width_;   // parking slot short side length, unit: m
    double length_;  // parking slot long side length, unit: m

    Eigen::Vector3d center_;          // parking slot center point, unit: m
    Eigen::Vector3d wide_direction_;  // parking slot short side direction
    Eigen::Vector3d long_direction_;  // parking slot long side direction
    Eigen::Vector3d cor_tl_;
    Eigen::Vector3d cor_tr_;
    Eigen::Vector3d cor_bl_;
    Eigen::Vector3d cor_br_;
    std::vector<Eigen::Matrix3d> cor_prob_;
    Eigen::Vector3d car_pos_;

    bool is_occupancy_{true};  // parking slot occupamcy property
    double confidence_{0.5};   // parking slot confidence, scaled: 0.0 ~ 1.0
};

typedef std::shared_ptr<FusionParkingSlotResult> FusionParkingSlotResultPtr;
typedef std::shared_ptr<ParkingSlotResult> ParkingSlotResultPtr;
typedef std::shared_ptr<const ParkingSlotResult> ParkingSlotResultConstPtr;

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
