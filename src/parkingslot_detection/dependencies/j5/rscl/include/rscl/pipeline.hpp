#ifndef __INCLUDE_PIPELINE_HPP__
#define __INCLUDE_PIPELINE_HPP__

#include <common/error.hpp>
#include <functional>
#include <image/ADImage.hpp>
#include "proxy.hpp"
#include <string>
#include <vector>
#include "work.hpp"
#include "opflow.hpp"

namespace senseAD {
namespace perception {
namespace camera {

struct CameraImage {
    std::string camera_name;
    ADInference::ADImagePtr image;
};
using CameraImageVec = std::vector<CameraImage>;

template <class T>
struct Ring {
    std::vector<T> arr;
    typename std::vector<T>::iterator iter;
    using Wait_ = std::function<void(T&)>;
    Ring() {}
    ~Ring() {}
    void Init(size_t len) {
        arr.resize(len);
        iter = arr.begin();
    }
    size_t Size() { return arr.size(); }
    void PushT(T& t) { arr.emplace_back(t); }
    T& Get(size_t idx) { return arr.at(idx); }
    T& Get(Wait_ w = {}) {
        if (arr.end() == iter) {
            iter = arr.begin();
        }
        auto& ret = *iter;
        if (w) {
            w(ret);
        }
        ++iter;
        return ret;
    }
};

class PipelineParameter;

class Pipeline {
 public:
    Pipeline() : ring_proxy_(), work_pipe_(), pipeline_depth_(0) {}
    virtual ~Pipeline() {}
    /**
     * @brief, Init a pipeline, with default deployment.
     */
    virtual apStatus_t Init(const std::string& root_path,
                            const std::string& proto_file,
                            const std::string& vehicle_config);

    /**
     * @brief Init a pipelin, with special deployment.
     *        Only input deployment scenarios will be supported later.
     *        like  Init(config_root_path, deployment, vehicle_config)
     *
     * @param root_path perception software path
     * @param proto_file for pipeline proto
     * @param vehicle_config for vehicle config path
     * @param deployment for special op proto, in vehicle/avp/<deployment>.proto
     * @return apStatus_t
     */
    virtual apStatus_t Init(const std::string& root_path,
                            const std::string& proto_file,
                            const std::string& vehicle_config,
                            const std::string& deployment);

    virtual apStatus_t Init(PipelineParameter* proto_param);

    typedef std::function<void(Proxy&)> ProxySetup;
    /**
     * @brief, Process a pipeline generally
     */
    virtual apStatus_t ProcessAsync(ProxySetup setup,
                                    ProcessAsyncCallback callback,
                                    void* flag);

    /**
     * @brief, Process a pipeline
     */
    virtual apStatus_t ProcessAsync(const CameraImageVec& camera_images,
                                    size_t frame_timestamp,
                                    size_t frame_id,
                                    ProcessAsyncCallback callback,
                                    void* flag);

    /**
     * @brief, Process a pipeline
     */
    virtual apStatus_t ProcessAsync(
        const CameraImageVec& camera_images,
        const senseAD::perception::OdometryInfo& odometry,
        size_t frame_timestamp,
        size_t frame_id,
        ProcessAsyncCallback callback,
        void* flag);

    virtual apStatus_t ProcessAsync(
        const StrMap<std::vector<CameraObjectPtr>>& objects,
        const std::shared_ptr<senseAD::perception::OdometryInfo> odometry,
        size_t frame_timestamp,
        size_t frame_id,
        ProcessAsyncCallback callback,
        void* flag);

    virtual apStatus_t WaitDone();
    // apStatus_t ProcessSync();

    apStatus_t GenerateBenchmark(const std::string& path);
    // HACK: Ugly but doesn't have time to modify
    apStatus_t GenerateBenchmarkOD(const std::string& path, float f);
    apStatus_t GenerateBenchmarkRD(const std::string& path, float f);

 protected:
    virtual apStatus_t InitDepth(size_t pipeline_depth);
    apStatus_t IsPipelineReady();
    apStatus_t PushOperator(size_t work_idx, OperatorPtr opptr);

    // Frame* frame_;
    // Proxy* proxy_;

    WorkPipe work_pipe_;
    Ring<Proxy> ring_proxy_;
    size_t pipeline_depth_;
    Proxy* proxy_;
};

class OpFlowPipeline : public Pipeline {
 public:
    OpFlowPipeline() {}
    virtual ~OpFlowPipeline() {}

    virtual apStatus_t Init(const std::string& root_path,
                            const std::string& proto_file,
                            const std::string& vehicle_config) {
        return Pipeline::Init(root_path, proto_file, vehicle_config);
    }

    virtual apStatus_t Init(const std::string& root_path,
                            const std::string& proto_file,
                            const std::string& vehicle_config,
                            const std::string& deployment) {
        return Pipeline::Init(root_path, proto_file, vehicle_config,
                              deployment);
    }

    virtual apStatus_t Init(PipelineParameter* proto_param);

    virtual apStatus_t ProcessAsync(ProxySetup setup,
                                    ProcessAsyncCallback callback,
                                    void* flag);

    virtual apStatus_t ProcessAsync(const CameraImageVec& camera_images,
                                    size_t frame_timestamp,
                                    size_t frame_id,
                                    ProcessAsyncCallback callback,
                                    void* flag);

    virtual apStatus_t ProcessAsync(
        const CameraImageVec& camera_images,
        const senseAD::perception::OdometryInfo& odometry,
        size_t frame_timestamp,
        size_t frame_id,
        ProcessAsyncCallback callback,
        void* flag);

    virtual apStatus_t ProcessAsync(
        const StrMap<std::vector<CameraObjectPtr>>& objects,
        const std::shared_ptr<senseAD::perception::OdometryInfo> odometry,
        size_t frame_timestamp,
        size_t frame_id,
        ProcessAsyncCallback callback,
        void* flag);

    virtual apStatus_t WaitDone();

 protected:
    virtual apStatus_t InitDepth(size_t pipeline_depth);

    std::shared_ptr<OpFlow> thread_pool = nullptr;
};

#define REGISTER_PERCEPTION_PIPELINE(name) \
    REGISTER_COMMON_ENGINE(senseAD::perception::camera, name, Pipeline, name)
#define INFLATE_PERCEPTION_PIPELINE(name)                               \
    CommonFactory<senseAD::perception::camera::Pipeline>::GetInstance() \
        .Create(name)

REGISTER_PERCEPTION_PIPELINE(Pipeline);
REGISTER_PERCEPTION_PIPELINE(OpFlowPipeline);

}  // namespace camera
}  // namespace perception
}  // namespace senseAD

#endif  // __INCLUDE_PIPELINE_HPP__
