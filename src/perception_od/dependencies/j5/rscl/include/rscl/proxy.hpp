/*
 * Copyright (C) 2021-2022 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include <tensor/ADTensor.hpp>
#include <tensor/ADTensor4D.hpp>
#include <vector>

#include "common/image_meta.hpp"
#include "data_type/data_type.hpp"
#include "data_type/localization_info.hpp"
#include "data_type/object.hpp"
#include "image/ADImage.hpp"
#include "data_type/slot_info.hpp"
#include "utils/grid_map.hpp"

namespace senseAD {
namespace perception {
namespace camera {

using ADInference::ADImagePtr;
using ADInference::ADTensorPtr;

template <typename T>
using StrMap = std::map<std::string, T>;

struct Proxy {
    size_t frame_id = 0;
    size_t timestamp = 0;
    StrMap<ADImagePtr> images;
    StrMap<ADImagePtr> segmentations;

    StrMap<StrMap<ADTensorPtr>> tensors;
    StrMap<std::vector<CameraObjectPtr>> objects;
    std::vector<ParkingSlotResultPtr> parking_slot;
    std::vector<FusionParkingSlotResultPtr> fusion_parking_slot;
    std::vector<FusionParkingSlotResultPtr> fusion_parking_slot_optimize;
    AvpMainStateInfo avp_main_state_info;
    FreespaceInfo freespace_info_;
    std::vector<uint8_t> freespace_grid_;

    bool space_parking_cal;
    // frenet_status: 0: wait_mapping_done, 1: success, 2: fail
    uint8_t frenet_status = 0;

    void* grid_map = nullptr;

    OdometryInfo odometry;

    // ImageMeta for each camera
    std::map<std::string, std::shared_ptr<ImageMeta>> image_meta;
    std::vector<ParkingSlotQuadPtr> parking_slot_quad;

 public:
    // status
    void MergeProxy(Proxy& proxy) {
        for (auto& iter : proxy.images) {
            if (images.count(iter.first) == 0) {
                images.insert(proxy.images.begin(), proxy.images.end());
            } else {
                if (iter.second->GetPixelFormat() !=
                    images[iter.first]->GetPixelFormat()) {
                    AD_LWARN(PERCEPTION) << "Pixel format not match.This may "
                                            "lead to errors, please find a "
                                            "solution.";
                    if (iter.second->GetPixelFormat() ==
                            PixelFormat::M_PIX_FMT_BGR888 ||
                        iter.second->GetPixelFormat() ==
                            PixelFormat::M_PIX_FMT_RGB888) {
                        images[iter.first] = proxy.images[iter.first];
                    }
                }
            }
        }
        tensors.insert(proxy.tensors.begin(), proxy.tensors.end());
        objects.insert(proxy.objects.begin(), proxy.objects.end());
        segmentations.insert(proxy.segmentations.begin(),
                             proxy.segmentations.end());
        parking_slot.insert(parking_slot.end(), proxy.parking_slot.begin(),
                            proxy.parking_slot.end());
        fusion_parking_slot.insert(fusion_parking_slot.end(),
                                   proxy.fusion_parking_slot.begin(),
                                   proxy.fusion_parking_slot.end());
        fusion_parking_slot_optimize.insert(
            fusion_parking_slot_optimize.end(),
            proxy.fusion_parking_slot_optimize.begin(),
            proxy.fusion_parking_slot_optimize.end());
        image_meta.insert(proxy.image_meta.begin(), proxy.image_meta.end());
        parking_slot_quad.insert(parking_slot_quad.end(),
                                 proxy.parking_slot_quad.begin(),
                                 proxy.parking_slot_quad.end());
    }
    bool IsReady() const { return is_ready_; }
    void SetReady(bool s) { is_ready_ = s; }
    bool IsCorrect() const { return is_correct_; }
    void SetCorrect(bool s) { is_correct_ = s; }

    inline void Clean() {
        images.clear();
        tensors.clear();
        objects.clear();
        segmentations.clear();
        parking_slot.clear();
        fusion_parking_slot.clear();
        fusion_parking_slot_optimize.clear();
        parking_slot_quad.clear();
        space_parking_cal = false;
    }

    inline bool GetImageMeta(const std::string& camera_name,
                             ImageMeta& image_meta) {
        auto it = this->image_meta.find(camera_name);
        if (it != this->image_meta.end()) {
            image_meta = *(it->second);
            return true;
        }
        AD_LWARN(Proxy) << "Find no ImageMeta for camera_name: " << camera_name;
        return false;
    }
    inline bool GetImageMeta(const std::vector<std::string>& camera_names,
                             ImageMeta& image_meta) {
        for (const auto& camera_name : camera_names) {
            if (GetImageMeta(camera_name, image_meta)) {
                return true;
            }
        }
        return false;
    }

 private:
    bool is_ready_ = true;
    bool is_correct_ = true;
};

}  // namespace camera
}  // namespace perception
}  // namespace senseAD
