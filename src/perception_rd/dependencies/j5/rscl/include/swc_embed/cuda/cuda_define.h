#pragma once

#if defined(USE_CUDA) || defined(USE_JETSON)
#define ENABLE_CUDA

#include "ad_rscl/comm/cuda/cuda_publisher.h"
#include "ad_rscl/comm/cuda/cuda_subscriber.h"

#include "cuda_runtime_api.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#if defined(USE_OPENCV_2_X)
#include "opencv2/gpu/gpu.hpp"
#define CvGpuMat cv::gpu::GpuMat
#define CvGpu cv::gpu
#elif defined(USE_OPENCV_4_X)
#include "opencv2/core/cuda.hpp"
#include "opencv2/cudawarping.hpp"
#define CvGpuMat cv::cuda::GpuMat
#define CvGpu cv::cuda
#endif
#pragma GCC diagnostic pop
#endif
