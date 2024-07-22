/*
 * Copyright (C) 2021-2023 by SenseTime Group Limited. All rights reserved.
 * Chen Zhenan <chenzhenan@senseauto.com>
 */

#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace senseAD {
namespace avp_diag {
enum ExceptionCode {
    ERR_NONE,
    // Sensor节点接收camera的延迟异常(header时间戳和rscl系统时间戳差别大，超过150ms)
    ERR_SENSOR_CAMERA_DELAY,
    // 感知收到的数据dalay?
    ERR_PERCEPTION_DELAY,
    // 凑不齐四个鱼眼相机（和一个前视相机）的数据
    ERR_RD_CAMERA_NO_SYNC,
    // 凑不齐四个鱼眼相机（和一个前视相机）的数据
    ERR_OD_CAMERA_NO_SYNC,
    // 凑不Fusion输入的数据
    ERR_FUSION_INPUT_NO_SYNC,
    // RD输入输出，与当前rscl时间比较时间戳差异过大
    ERR_RD_TIME_GAP,
    // OD输入输出，与当前rscl时间比较时间戳差异过大
    ERR_OD_TIME_GAP,
    // 没有道路分割地图
    ERR_NO_RD_SEGMENT,
    // 没有障碍物输出
    ERR_NO_OBJECTS,
    // 没有定位数据输出
    ERR_NO_NAVSTATE,
    // 没有freespace输出
    ERR_NO_FREESPACE,
    // 没有FUSION输出
    ERR_NO_FUSION_OUTPUT,
    // RD帧率慢
    ERR_SLOW_PARKING_SLOT,
    // OD帧率慢
    ERR_SLOW_OBJECTS,
    // FUSION帧率慢
    ERR_SLOW_FUSION,
    // IO缓慢
    ERR_IO_SLOW,
    // 没有网络链接
    ERR_NETWORK_DISCONNECTED,
    // rscl时间小于sensor时间
    ERR_RSCL_TIME_LESS_THEN_SENSOR_TIME,
};

struct DiagException {
    uint64_t timestamp_ns;
    ExceptionCode code;
    std::string message;

    DiagException() : timestamp_ns(0), code(ERR_NONE) { }
    DiagException(uint64_t timestamp_ns, ExceptionCode code, const std::string &message):
        timestamp_ns(timestamp_ns), code(code), message(message) {}

    bool operator > (const DiagException& other) const {
        return (timestamp_ns > other.timestamp_ns);
    }
};

const std::string& getExceptionText(ExceptionCode code);
const std::string& getExceptionMessage(ExceptionCode code);

}  // namespace avp_diag
}  // namespace senseAD
