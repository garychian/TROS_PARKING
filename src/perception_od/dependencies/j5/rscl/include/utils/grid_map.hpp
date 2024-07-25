#pragma once

#include <array>
#include <mutex>
#include <unordered_map>
#include <memory>

#include <opencv2/core.hpp>

#include "data_type/localization_info.hpp"
#include "common/common_log.hpp"
#include "core/common.h"
#include <image/ADImage.hpp>

namespace senseAD {
namespace perception {
namespace camera {

template <typename T>
struct DataBlock {
    void Init(uint32_t row, uint32_t col) {
        row_ = row;
        col_ = col;
        total_ = row_ * col_;
        uint32_t size = row_ * col_;
        data_.resize(size);
    }

    inline T& At(uint32_t index) { return data_[index]; }
    inline T& At(uint32_t row, uint32_t col) { return At(row * col_ + col); }
    inline T At(uint32_t row, uint32_t col) const {
        return data_[row * col_ + col];
    }

    inline uint32_t Row() { return row_; }
    inline uint32_t Col() { return col_; }
    inline uint32_t Total() { return total_; }
    inline uint32_t Size() { return total_ * sizeof(T); }

 private:
    uint32_t row_ = 0;
    uint32_t col_ = 0;
    uint32_t total_ = 0;
    std::vector<T> data_;
};

class UpdateParameter {
 public:
    float32_t score_decline_coeff;
    float32_t occ_score_thres;
    float32_t free_score_thres;

    float32_t update_dist_diff_thres;
    float32_t always_update_dist_thres;

 public:
    static UpdateParameter& Instance() {
        static UpdateParameter instance;
        return instance;
    }
};

enum GridStatus { UNKNOWN = -1, FS_OCC = 0, FS_FREE = 1, OD = 2, USS = 3 };

#define __CAMERA_COUNT 5
class GridPoint {
 public:
    GridPoint() {
        frame_id_ = std::numeric_limits<uint64_t>::max();
        camera_layer_.visited = false;
        memset(camera_layer_.cache_pixel_occ_free, 0, __CAMERA_COUNT);
        for (int32_t i = 0; i < __CAMERA_COUNT; i++) {
            camera_layer_.cache_dist[i] = std::numeric_limits<float32_t>::max();
        }
        camera_layer_.free_min_dist = std::numeric_limits<float32_t>::max();
        camera_layer_.occ_min_dist = std::numeric_limits<float32_t>::max();
        camera_layer_.score = 0.0f;
        camera_layer_.status = GridStatus::UNKNOWN;
        cache_od_ = false;
        is_od_ = false;
    }

    void UpdateFreeOcc(uint64_t frame_id,
                       GridStatus status,
                       int32_t camera_id,
                       float32_t dist);
    void UpdateOD(uint64_t frame_id);
    void UpdateCache(uint64_t frame_id);
    GridStatus GetStatus(uint64_t frame_id);

 private:
    struct CameraData {
        bool visited;
        uint8_t cache_pixel_occ_free[__CAMERA_COUNT];
        float32_t cache_dist[__CAMERA_COUNT];
        float32_t free_min_dist;
        float32_t occ_min_dist;
        float32_t score;
        GridStatus status;
    };

    uint64_t frame_id_;
    CameraData camera_layer_;
    bool cache_od_;
    bool is_od_;
};

template <typename _TypeFlag = float32_t>
struct GridBlock {
    GridBlock(Eigen::Vector2d center, uint32_t depth)
        : depth_(depth), width_(1 << depth), center_(center) {
        data_.Init(width_, width_);
    }

    /* 暂存要更新的栅格
     * 因为一个栅格可能由多个像素点共同决定，所以需要先把所有像素点结果先缓存起来，最后统一处理更新
     */
    void UpdateFreeOcc(uint64_t frame_id,
                       const uint32_t idx,
                       GridStatus status,
                       int32_t camera_id,
                       float32_t dist) {
        if (idx >= data_.Total()) {
            // AD_LWARN(GridBlock) << "Invalid idx: " << idx;
            return;
        }
        data_.At(idx).UpdateFreeOcc(frame_id, status, camera_id, dist);
    }

    void UpdateOD(uint64_t frame_id, const uint32_t idx) {
        if (idx >= data_.Total()) {
            return;
        }
        data_.At(idx).UpdateOD(frame_id);
    }

    inline GridPoint& At(uint32_t row, uint32_t col) {
        return data_.At(row, col);
    }
    inline GridPoint& At(uint32_t idx) { return data_.At(idx); }

    // 获取上次更新时的时间点/距离点
    inline _TypeFlag& LastUpdateFlag() { return latest_flag_; }

 private:
    uint32_t depth_;
    uint32_t width_;
    Eigen::Vector2d center_;
    DataBlock<GridPoint> data_;

    // uint64_t last_timestamp_;
    // 上次修改时累计行驶距离
    _TypeFlag latest_flag_;
};

struct GridIdx {
    // 栅格地图块下标
    uint32_t block_idx;
    // 栅格在块内下标
    uint32_t grid_idx;
};

struct FreespaceInfo {
    double left, top, cx, cy;
    double unit = 0.1;
    uint32_t width, height;
};

template <typename _TypeFlag = uint64_t>
class GridMap {
 public:
    typedef UpdateParameter ElemUpdateParam;

    GridMap(Eigen::Vector2d center,
            uint32_t block_depth,
            uint32_t map_depth,
            float32_t grid_scale = 0.1)
        : center_(center),
          grid_scale_(grid_scale),
          block_depth_(block_depth),
          map_depth_(std::max(map_depth, block_depth + 2)) {
        grid_stride_ = 1 << block_depth;
        block_size_ = grid_stride_ * grid_scale_;
        map_size_ = (1 << map_depth_) * grid_scale_;
        block_stride_ = (1 << (map_depth_ - block_depth));
        left_ = center_.x() - (map_size_ / 2.0);
        top_ = center_.y() - (map_size_ / 2.0);

        blocks_.clear();
        blocks_.resize(block_stride_ * block_stride_, nullptr);
        frame_id_ = std::numeric_limits<uint64_t>::max();
    }
    virtual ~GridMap() = default;

    bool NeedAdjust(const Eigen::Vector3d& cur_car_pose,
                    const Eigen::Vector3d& last_car_pose,
                    bool* jump_ptr = nullptr,
                    Eigen::Vector2d* dd_ptr = nullptr) const {
        Eigen::Vector2d dd{cur_car_pose(0) - center_(0),
                           cur_car_pose(1) - center_(1)};
        AD_LDEBUG(GridMap) << "Adjust dd: " << dd(0) << "," << dd(1);
        dd = dd / block_size_;
        if (dd_ptr) {
            *dd_ptr = dd;
        }
        // go very far or jump(maybe cause by loop)
        bool go_very_far = fabs(dd(0)) > (block_stride_ / 4) ||
                           fabs(dd(1)) > (block_stride_ / 4);

        // TODO:(hangmeng) tune dist thresh (meter)
        bool jump = (cur_car_pose.head(2) - last_car_pose.head(2)).norm() > 1.;
        if (jump_ptr) {
            *jump_ptr = jump;
        }

        if (!go_very_far && !jump) {
            int num_wayPoint = local_traj_stamped_.size();
            double traj_len = 0;
            for (int i = 1; i < num_wayPoint; i++) {
                traj_len += (local_traj_stamped_[i].second.head(2) -
                             local_traj_stamped_[i - 1].second.head(2))
                                .norm();
            }
            // TODO:(hangmeng) tune dist thresh (meter)
            go_very_far = traj_len > 25;
        }

        return go_very_far || jump;
    }

    bool Adjust(const Eigen::Vector3d& cur_car_pose,
                const Eigen::Vector3d& last_car_pose) {
        pos_ = cur_car_pose;
        Eigen::Vector2d dd;
        if (NeedAdjust(cur_car_pose, last_car_pose, nullptr, &dd)) {
            AdjustCenter(static_cast<int>(dd(0)), static_cast<int>(dd(1)));
            return true;
        }
        return false;
    }

    void AdjustCenter(int dx, int dy) {
        // Let's move step by step.....
        dx = std::min(std::max(-1, dx), 1);
        dy = std::min(std::max(-1, dy), 1);
        AD_LDEBUG(GridMap) << "AdjustCenter: x:" << dx << " y:" << dy;

        int32_t block_stride = static_cast<int32_t>(block_stride_);
        std::vector<std::shared_ptr<GridBlock<_TypeFlag>>> new_blocks(
            block_stride * block_stride, nullptr);
        for (int32_t row = 0; row < block_stride; row++) {
            for (int32_t col = 0; col < block_stride; col++) {
                int32_t ori_row = row + dy;
                int32_t ori_col = col + dx;
                if (ori_row > 0 && ori_row < block_stride && ori_col > 0 &&
                    ori_col < block_stride) {
                    new_blocks[row * block_stride + col] =
                        blocks_[ori_row * block_stride + ori_col];
                }
            }
        }
        blocks_.swap(new_blocks);
        center_(0) = center_(0) + static_cast<float32_t>(dx) *
                                      static_cast<float32_t>(grid_stride_) *
                                      grid_scale_;
        center_(1) = center_(1) + static_cast<float32_t>(dy) *
                                      static_cast<float32_t>(grid_stride_) *
                                      grid_scale_;
        left_ = center_.x() - (map_size_ / 2.0);
        top_ = center_.y() - (map_size_ / 2.0);
        AD_LDEBUG(GridMap) << "AdjustCenter: center_:" << center_(0) << ","
                           << center_(1) << " left_:" << left_
                           << ", top_:" << top_;
    }

    void Prune(
        _TypeFlag flag,
        std::function<bool(_TypeFlag& block_flag, _TypeFlag& flag)> func) {
        for (size_t i = 0; i < blocks_.size(); i++) {
            // 抛弃这个block，如果他满足某些条件
            if (blocks_[i] && func(blocks_[i]->LastUpdateFlag(), flag)) {
                blocks_[i] = nullptr;
            }
        }
    }

    bool UpdateFreeOcc(_TypeFlag flag,
                       const GridIdx& loc,
                       GridStatus status,
                       int32_t camera_id,
                       float32_t dist) {
        if (!IsValidBlockId(loc.block_idx)) {
            // AD_LWARN(GridMap) << "Invalid block idx: " << loc.block_idx;
            return false;
        }
        BlockRef(loc.block_idx)
            .UpdateFreeOcc(frame_id_, loc.grid_idx, status, camera_id, dist);
        return true;
    }

    bool UpdateOD(const GridIdx& loc) {
        if (!IsValidBlockId(loc.block_idx)) {
            return false;
        }
        BlockRef(loc.block_idx).UpdateOD(frame_id_, loc.grid_idx);
        return true;
    }

    void LocatePoint(float32_t x,
                     float32_t y,
                     GridIdx& grid,
                     bool enable_adjust = false) const {
        static float32_t remap = 1.0F / grid_scale_;
        const int x_grid = static_cast<int>((x - left_) * remap);
        const int qx = x_grid / grid_stride_;
        const int qxx = x_grid % grid_stride_;
        const int y_grid = static_cast<int>((y - top_) * remap);
        const int qy = y_grid / grid_stride_;
        const int qyy = y_grid % grid_stride_;

        grid.block_idx = (qy * block_stride_) + qx;
        grid.grid_idx = (qyy * grid_stride_) + qxx;

        // AP_LTRACE(GridMapLocatePoint)
        //     << "Point: " << x << "," << y << "  Loc: block-" << block_idx
        //     << ", grid-" << grid_idx;
    }

    inline uint32_t GridLenBlock() const { return grid_stride_; }
    inline uint32_t GridNumBlock() const {
        return GridLenBlock() * GridLenBlock();
    }
    inline uint32_t BlockLen() const { return block_stride_; }
    inline uint32_t BlockNum() const { return BlockLen() * BlockLen(); }
    inline float32_t BlockReso() const { return block_size_; }
    inline uint32_t GridLenMap() const { return GridLenBlock() * BlockLen(); }
    inline uint32_t GridNumMap() const { return GridLenMap() * GridLenMap(); }
    inline float32_t MinX() const { return left_; }
    inline float32_t MinY() const { return top_; }
    inline float32_t CenterX() const { return center_(0); }
    inline float32_t CenterY() const { return center_(1); }
    inline float32_t GridScale() const { return grid_scale_; }

    inline bool IsValidBlockId(uint32_t idx) const {
        return idx < blocks_.size();
    }

    inline std::shared_ptr<GridBlock<_TypeFlag>> Block(uint32_t row,
                                                       uint32_t col) const {
        return Block(row * BlockLen() + col);
    }
    inline std::shared_ptr<GridBlock<_TypeFlag>> Block(uint32_t idx) const {
        return blocks_[idx];
    }

    inline GridBlock<_TypeFlag>& BlockRef(uint32_t row, uint32_t col) {
        return BlockRef(row * BlockLen() + col);
    }
    inline GridBlock<_TypeFlag>& BlockRef(uint32_t idx) {
        if (blocks_[idx] == nullptr) {
            uint32_t row = idx / block_stride_;
            uint32_t col = idx % block_stride_;
            float32_t block_cx = left_ + row * block_size_ + block_size_ / 2;
            float32_t block_cy = top_ + col * block_size_ + block_size_ / 2;
            Eigen::Vector2d block_center(block_cx, block_cy);
            blocks_[idx] = std::make_shared<GridBlock<_TypeFlag>>(block_center,
                                                                  block_depth_);
        }

        return *blocks_[idx];
    }

    inline const Eigen::Vector3d& GetPos() const { return pos_; }
    inline void SetPos(const Eigen::Vector3d& pos) { pos_ = pos; }

    void SetParam(const UpdateParameter& param) { param_ = param; }
    const UpdateParameter& GetParam() const { return param_; }

    void SetFrameId(uint64_t frame_id) { frame_id_ = frame_id; }
    uint64_t GetFrameId() const { return frame_id_; }

 public:
    std::vector<std::pair<uint64_t, Eigen::Vector3d>> local_traj_stamped_;
    size_t cur_grid_id_ = 0;

 private:
    // 地图中心 m
    Eigen::Vector2d center_;
    // 栅格大小 m
    float32_t grid_scale_ = 0.1;
    // 块深度，块边长=2^n
    uint32_t block_depth_ = 0;
    // 块边长=2^n
    uint32_t grid_stride_ = 0;
    // 地图深度，块数量=2^(n-block_depth_)
    uint32_t map_depth_ = 0;
    // 块数量=2^(n-block_depth_)
    uint32_t block_stride_ = 0;

    // 块边长m
    float32_t block_size_;
    // 地图边长m
    float32_t map_size_;
    // 地图左上坐标m
    float32_t left_, top_;

    std::vector<std::shared_ptr<GridBlock<_TypeFlag>>> blocks_;

    Eigen::Vector3d pos_;
    // std::vector<Eigen::Vector2d> pos_traj_;

    UpdateParameter param_;

    uint64_t frame_id_;
};

void VisualizeGridMap(GridMap<>& grid_map,
                      ADInference::ADImage& image,
                      ADInference::ADImage& image_vis,
                      FreespaceInfo& info);

bool ComputeTrajectoryLocalGrid(const std::unique_ptr<GridMap<>>& grid_map,
                                FreespaceInfo* info_ptr,
                                cv::Mat& local_grid);

void SaveLocalGrid(const GridMap<>* gridmap,
                   const std::string& grid_image_dir,
                   const std::string& grid_info_file_path,
                   bool jump = false);

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
