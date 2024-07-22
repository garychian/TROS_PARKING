#pragma once
#include <opencv2/core/core.hpp>
#include "core/logger.h"
#include "eigen3/Eigen/Core"
#include "eigen3/Eigen/src/Core/DenseBase.h"

typedef Eigen::Matrix<float, 12, 12> StateMatrixf;

namespace ara {
namespace log {

inline LogStream& operator<<(
    LogStream& out,
    std::basic_ostream<
        char,
        std::char_traits<char>>& (*)(std::basic_ostream<
                                     char,
                                     std::char_traits<char>>&)) noexcept {
    out << "\n";
    return out;
}

inline LogStream& operator<<(LogStream& out,
                             const cv::Point2f& value) noexcept {
    out << value.x << " " << value.y;
    return out;
}

inline LogStream& operator<<(LogStream& out,
                             const cv::Point3f& value) noexcept {
    out << value.x << " " << value.y << " " << value.z;
    return out;
}

inline LogStream& operator<<(LogStream& out, const cv::Mat& value) noexcept {
    out << cv::format(value, cv::Formatter::FMT_DEFAULT);
    return out;
}

inline LogStream& operator<<(LogStream& out,
                             const Eigen::Matrix4d& value) noexcept {
    return out;
}

inline LogStream& operator<<(LogStream& out,
                             const Eigen::Matrix3d& value) noexcept {
    return out;
}

inline LogStream& operator<<(LogStream& out,
                             const StateMatrixf& value) noexcept {
    return out;
}

// template <typename T>
// inline LogStream& operator<<(LogStream& out, const
// Eigen::internal::Transpose& value) noexcept {
////    out << cv::format(value, cv::Formatter::FMT_DEFAULT);
//    return out;
//}

// inline LogStream& operator<<(LogStream& out, const size_t& value) noexcept {
//     out << "xx";
//     return out;
// }

template <typename T>
inline LogStream& operator<<(LogStream& out, const T& value) noexcept {
    std::stringbuf buf;
    std::ostream os(&buf);
    os << value;

    std::stringstream ss;
    ss << os.rdbuf();
    out << ss.str();

    return out;
}

}  // namespace log
}  // namespace ara

#define GET_FILE_NAME(file) \
    std::string(file).substr(std::string(file).rfind('/') + 1)

#define HAF_LOG_VERBOSE_SDK(tag) SDK_HAF_LOG_VERBOSE
#define HAF_LOG_DEBUG_SDK(tag) SDK_HAF_LOG_DEBUG
#define HAF_LOG_INFO_SDK(tag) SDK_HAF_LOG_INFO
#define HAF_LOG_WARN_SDK(tag) SDK_HAF_LOG_WARN
#define HAF_LOG_ERROR_SDK(tag) SDK_HAF_LOG_ERROR
#define HAF_LOG_FATAL_SDK(tag) SDK_HAF_LOG_FATAL
