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
#include "data_type/geometry.hpp"

namespace senseAD {
namespace perception {
namespace camera {

static inline bool FloatNumEqual(float l, float r) {
    constexpr float EPSILON = 1e-7;
    return fabsf(l - r) < EPSILON;
}

float PointCarDistance(const Eigen::Vector2f &p,
                       std::vector<Eigen::Vector2f> &car);

template <typename _Tp>
class PSMask {
 public:
    PSMask() = default;
    PSMask(uint32_t width, uint32_t height, const _Tp value = 0)
        : width(width), height(height) {
        step = width;
        data.resize(height * step, value);
    }
    PSMask(uint32_t width, uint32_t col, const std::vector<_Tp> &vec)
        : width(width), height(height), data(vec) {
        step = width;
        this->data.resize(height * step);
    }

    // Delete unrecognized type
    template <typename T>
    _Tp &At(T x, T y) const = delete;
    template <typename T>
    _Tp At(T x, T y, _Tp safe_v) const = delete;

    _Tp &At(uint32_t x, uint32_t y) { return data[y * step + x]; }
    _Tp At(uint32_t x, uint32_t y, _Tp safe_v) const {
        return x < width && y < height ? data[y * step + x] : safe_v;
    }
    _Tp &At(float x, float y) {
        return At(static_cast<uint32_t>(x + 0.5F),
                  static_cast<uint32_t>(y + 0.5F));
    }
    _Tp At(float x, float y, _Tp safe_v) const {
        return At(static_cast<uint32_t>(x + 0.5F),
                  static_cast<uint32_t>(y + 0.5F), safe_v);
    }

 public:
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t step = 0;
    std::vector<_Tp> data;
};
typedef PSMask<uint8_t> PSMaskU8;

struct ParkingSlotRect {
    ParkingSlotRect(int left, int top, int w, int h)
        : l_(left), t_(top), w_(w), h_(h) {
        r_ = l_ + w;
        b_ = t_ + h;
    }
    int area() const { return w_ * h_; }
    int left() const { return l_; }
    int right() const { return r_; }
    int top() const { return t_; }
    int bottom() const { return b_; }

 private:
    int l_, t_, r_, b_, w_, h_;
};

/* Describe approx_box's point */
struct ApproxBoxPoints {
    Vec2f p;
    float border_dist = 0.F;
    float point_score = 0.F;
    // clockwise line, with next point
    float line_len = 0.F;
    float line_score = 0.F;
    bool has_border_point = false;
};

/* Quad describes everything for a parkingslot */
struct ParkingSlotQuad {
    Eigen::Vector2f tl, tr, bl, br;
    Eigen::Vector2f ori_tl, ori_tr, ori_bl, ori_br;
    float confidence;
    uint32_t label;
    bool filtered = false;
    int slot_type = -1;
    int map_slot_type = -1;

    float IOU(const ParkingSlotQuad &rhs) const;
    float IOUBoundingBox(const ParkingSlotQuad &rhs) const;

    ParkingSlotRect BoundingRect() const {
        int left = std::min(tl(0), std::min(tr(0), std::min(bl(0), br(0))));
        int right = std::max(tl(0), std::max(tr(0), std::max(bl(0), br(0))));
        int top = std::min(tl(1), std::min(tr(1), std::min(bl(1), br(1))));
        int bottom = std::max(tl(1), std::max(tr(1), std::max(bl(1), br(1))));
        return ParkingSlotRect(left, top, right - left + 1, bottom - top + 1);
    }

    bool IsValidQuad() const {
        bool invalid =
            (FloatNumEqual(tl(0), tr(0)) && FloatNumEqual(tl(1), tr(1))) ||
            (FloatNumEqual(br(0), tr(0)) && FloatNumEqual(br(1), tr(1))) ||
            (FloatNumEqual(tl(0), bl(0)) && FloatNumEqual(tl(1), bl(1))) ||
            (FloatNumEqual(br(0), bl(0)) && FloatNumEqual(br(1), bl(1)));
        return !invalid;
    }

    // score for each point
    float s_tl, s_tr, s_bl, s_br;
    // attribute for each point
    std::shared_ptr<ApproxBoxPoints> p_tl = nullptr;
    std::shared_ptr<ApproxBoxPoints> p_tr = nullptr;
    std::shared_ptr<ApproxBoxPoints> p_bl = nullptr;
    std::shared_ptr<ApproxBoxPoints> p_br = nullptr;

    // addition info for this parkingslot
    Vec2f dir_in;
    Vec2f dir_width;
    Vec2f dir_length;
    Vec2f center;
    bool opp_modify = false;
    bool is_complete = false;
    float width = 0;
    float length = 0;
    bool is_visited = false;
};
typedef std::shared_ptr<ParkingSlotQuad> ParkingSlotQuadPtr;

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
