/*
 * Copyright (C) 2021-2023 by SenseTime Group Limited. All rights reserved.
 * Ye Zhenbang <yezhenbang@senseauto.com>
 * Chen Zhenan <chenzhenan@senseauto.com>
 */

#pragma once

#include <ad_msg_idl/ad_localization/localization.capnp.h>
#include <ad_msg_idl/ad_monitor/monitor.capnp.h>
#include <ad_msg_idl/ad_perception/fusion_parking_slots.capnp.h>
#include <ad_msg_idl/ad_perception/obstacles.capnp.h>
#include <ad_msg_idl/ad_perception/parking_slots.capnp.h>
#include <ad_msg_idl/ad_perception/quad_parking_slots.capnp.h>
#include <ad_msg_idl/avp_parking_cmd/hmi_command.capnp.h>
#include <ad_msg_idl/avp_parking_cmd/parking_cmd.capnp.h>
#include <ad_msg_idl/avp_smart_slam/avp_smart_slam.capnp.h>
#include <ad_msg_idl/avp_state/avp_main_state.capnp.h>
#include <ad_msg_idl/std_msgs/raw_data.capnp.h>

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
using std::vector;

#include <opencv2/imgproc/types_c.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "Occupancy.hpp"
#include "ad_common/data_type/base.hpp"
#include "ad_msg_idl/ad_perception/perception_result_draw.capnp.h"
#include "ad_rscl/ad_rscl.h"
#include "pipeline.hpp"
#include "swc_embed/pubsub_manager.h"
#include "timesync.hpp"
#include "avp_diag/exception_publisher.hpp"
#define TEST_PER 1
// buffer size set in sensor_camera, to decide our time-sync queue depth
constexpr size_t SENSOR_SHM_BUFFER_SIZE = 3;

namespace senseAD {
namespace avp_perception {

#if TEST_PER
using perception::camera::apStatus_t;
using PerceptionProxy = perception::camera::Proxy;
using swc_embed::ImageMemoryType;
using swc_embed::ImagePublisher;
using swc_embed::ImagePublisherFactory;
using swc_embed::ImagePublisherParam;
using swc_embed::ImageSubscriber;
using swc_embed::ImageSubscriberFactory;
using swc_embed::ImageSubscriberParam;
using swc_embed::ReceiveIpcImage;
using transaction::TimeSync;
using transaction::TransactionSyncedMsg;

typedef std::function<bool(const PerceptionProxy &, cv::Mat &)>
    PublishImageCallback;

template <typename T>
bool GetParamRequired(senseAD::rscl::component::ComponentBase *component,
                      const std::string &key,
                      T &container) {
    if (!component->GetParam(key, container)) {
        AD_LERROR(PERCEPTION) << key << " not specified.";
        return false;
    }
    return true;
}

ADInference::ADImagePtr IpcImageToAdImage(const ReceiveIpcImage &ipc_image);

ReceiveIpcImage AdImageToIpcImage(const ADInference::ADImagePtr &ad_image,
                                  uint64_t timestamp,
                                  int index = 0);

cv::Mat AdImageToCvMat(const ADInference::ADImagePtr &ad_image, int index = 0);

void publishImageMat(const ImagePublisher::Ptr &publisher,
                     const cv::Mat &image,
                     const uint64_t &timestamp_ns);

// for od and fusion
bool publishObject(const std::vector<std::string> &camera_names,
                   const senseAD::rscl::comm::Publisher<
                       senseAD::msg::perception::Obstacles>::Ptr &,
                   const PerceptionProxy &);

class PublishManager {
 public:
    bool RegisterPublisher(
        std::function<bool(const PerceptionProxy &)> publish_callback) {
        publish_callbacks_.push_back(publish_callback);
        return true;
    }
    bool Process(const PerceptionProxy &proxy) {
        bool flag_success = true;
        for (int i = 0; i < publish_callbacks_.size(); ++i) {
            flag_success &= publish_callbacks_.at(i)(proxy);
        }
        return flag_success;
    }

 private:
    std::vector<std::function<bool(const PerceptionProxy &)>>
        publish_callbacks_;
};

struct ImageReceiverUnit {
    std::string image_topic_name;
    std::string camera_name;
    std::shared_ptr<ImageSubscriber> ipc_subscriber;
    // std::shared_ptr<senseAD::msg::std_msgs::Image> input_image;
    // std::thread spin_thread;
    bool image_ready = false;
    uint64_t image_timestamp = 0;
};
typedef std::shared_ptr<ImageReceiverUnit> ImageReceiverUnitPtr;

class PerceptionCameraBaseComponent
    : public senseAD::rscl::component::TimerComponent {
 public:
    PerceptionCameraBaseComponent() {}
    bool OnInit() override;
    bool OnProc() override;
    void OnShutdown() override;

 protected:
    // Check for exceptions and deny entry into the function
#define PERCEPTION_CAMERA_CHECK_ACTION_RETURN(bo)          \
    {                                               \
        if (!bo) {                                  \
            AD_LWARN(PerceptionCameraBaseComponent) \
                << "check failed by " << #bo;       \
            return;                                 \
        }                                           \
    }

#define PERCEPTION_CAMERA_CHECK_ACTION_RETURN_BOOL(bo) \
    {                                                  \
        if (!bo) {                                     \
            AD_LWARN(PerceptionCameraBaseComponent)    \
                << "check failed by " << #bo;          \
            return false;                              \
        }                                              \
    }

    virtual bool checkOnAllMsgReceived(
        std::shared_ptr<transaction::TransactionSyncedMsg> synced_data);
    virtual bool checkFinishCallback(const PerceptionProxy &proxy, void *flag);
    virtual bool checkOnImage(size_t idx,
                              const std::shared_ptr<ReceiveIpcImage> &image);
    virtual bool checkPublishSegmentIpc(std::vector<ImagePublisher::Ptr> &,
                                        std::string seg_name,
                                        const PerceptionProxy &);
    auto GetExceptionPuber() { return exception_pub_; }

 protected:
    bool InitPipeline();
    void onReplay();
    void onPause();

    virtual void onAllMsgReceived(
        std::shared_ptr<transaction::TransactionSyncedMsg> synced_data);
    void InitTimestampSyncThread();
    void FinishCallback(const PerceptionProxy &proxy, void *flag);
    void onImage(size_t idx, const std::shared_ptr<ReceiveIpcImage> &image);
    bool publishSegmentIpc(std::vector<ImagePublisher::Ptr> &,
                           std::string seg_name,
                           const PerceptionProxy &);

    size_t frame_cnt_ = 0;
    bool f_inited_ = false;
    std::string node_name_;
    std::atomic<bool> flag_replaying_;
    std::atomic<bool> flag_shutdown_;

    std::string root_path_;
    std::string pipeline_param_file_;
    std::string config_source_folder_;
    std::string vehicle_id_;
    std::string deployment_;
    bool is_pub_draw_image_ = false;
    bool disable_publish_visual_data_ = false;

    PublishManager publish_manager_;
    std::vector<ImageReceiverUnit> image_receivers_;
    std::vector<std::string> camera_names_;
    std::vector<std::string> camera_topics_;
    std::unique_ptr<senseAD::rscl::task::TimerTask> time_synced_timer_;
    uint64 sync_timestamp_ns_;
    /* main pipeline */
    std::shared_ptr<perception::camera::Pipeline> perception_pipeline_ = nullptr;
    std::mutex pipeline_mtx_;
    transaction::TimeSync timestamp_sync_;
    std::condition_variable timestamp_sync_cv_;
    std::mutex timestamp_sync_mtx_;
    // thread to sync timestamp in loop
    std::thread timestamp_sync_thread_;
    // interval to control fps
    uint64_t timestamp_sync_interval_ns_ = 100;
    senseAD::avp_diag::ExceptionPublisher::Ptr exception_pub_;
    int target_fps_;
    int camera_fps_;
    cv::Size fov120_size_;

    Occupancy occ_;
    std::once_flag occ_call_flag_;

    int benchmark_generate_rate_ = 0;
    std::string benchmark_generate_prefix_ = "./";

    std::string pipeline_type_ = "Pipeline";
   
};
#endif
}  // namespace avp_perception
}  // namespace senseAD
