/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * Yin Shaopeng <yinshaopeng@sensetime.com>
 */

#ifndef _ADMETA_CORE_ERROR_HPP_
#define _ADMETA_CORE_ERROR_HPP_

#include "Optimization.hpp"
// #include "log.hpp"

#include <stdint.h>
#include <string>

namespace ADInference {

enum class MStatus : uint32_t {
    M_OK,
    M_FAILED,
    M_INVALID_ARG,
    M_FILE_NOT_FOUND,
    M_INVALID_FILE_FORMAT,
    M_INTERNAL_FAILED,
    M_OUT_OF_MEMORY,
    M_LICENSE_EXPIRE,
    M_UUID_MISMATCH,
    M_ONLINE_ACTIVATE_NO_NEED,
    M_ONLINE_ACTIVATE_FAIL,
    M_ONLINE_ACTIVATE_CODE_INVALID,
    M_ONLINE_ACTIVATE_CONNECT_FAIL,
    M_ONLINE_AUTH_TIME_OUT,
    M_LICENSE_ACTIVATIONS_RUN_OUT,
    M_MAX_STATUS
};

std::string GetErrorName(const MStatus status);

#define IF_TRUE_LOG(cond, ...)          \
    do {                                \
        if (unlikely(cond)) {           \
            SDK_LOG_ERROR(__VA_ARGS__); \
        }                               \
    } while (0)

#define IF_TRUE_RETURN_LOG(cond, ret, ...) \
    do {                                   \
        if (unlikely(cond)) {              \
            SDK_LOG_ERROR(__VA_ARGS__);    \
            return ret;                    \
        }                                  \
    } while (0)

#define IF_TRUE_RETURN_NO_LOG(cond, ret) \
    do {                                 \
        if (unlikely(cond)) {            \
            return ret;                  \
        }                                \
    } while (0)

#define IF_TRUE_BREAK_LOG(cond, ...) \
    if (unlikely(cond)) {            \
        SDK_LOG_ERROR(__VA_ARGS__);  \
        break;                       \
    }

#define IF_TRUE_CONTINUE_LOG(cond, ...) \
    if (unlikely(cond)) {               \
        SDK_LOG_ERROR(__VA_ARGS__);     \
        continue;                       \
    }

#define CHECK_EQ_RETURE_LOG(real, expected, ret) \
    IF_TRUE_RETURN_LOG(expected == real, ret, ": " #expected " == " #real)
#define CHECK_NE_RETURE_LOG(real, expected, ret) \
    IF_TRUE_RETURN_LOG(expected != real, ret, ": " #expected " != " #real)

#define CHECK_LE_RETURE_LOG(real, expected, ret) \
    IF_TRUE_RETURN_LOG(                          \
        real <= expected, ret, ": " #real "({}) <= " #expected "({})", real, expected)
#define CHECK_LT_RETURE_LOG(real, expected, ret) \
    IF_TRUE_RETURN_LOG(real < expected, ret, ": " #real "({}) < " #expected "({})", real, expected)

#define CHECK_GE_RETURE_LOG(real, expected, ret) \
    IF_TRUE_RETURN_LOG(                          \
        real >= expected, ret, ": " #real "({}) >= " #expected "({})", real, expected)
#define CHECK_GT_RETURE_LOG(real, expected, ret) \
    IF_TRUE_RETURN_LOG(real > expected, ret, ": " #real "({}) > " #expected "({})", real, expected)

#define CHECK_NULL_RETURN_LOG(ptr, ret) CHECK_EQ_RETURE_LOG(ptr, nullptr, ret)
#define CHECK_SUCCESS(ret) \
    IF_TRUE_RETURN_LOG((MStatus::M_OK != ret), ret, "Check failed: {}", static_cast< int32_t >(ret))
#define CHECK_SUCCESS_RET_WITH_LOG(ret, ...) \
    IF_TRUE_RETURN_LOG((MStatus::M_OK != ret), ret, __VA_ARGS__)

#define CHECK_PTR(ptr)                       \
    if (unlikely((ptr) == nullptr)) {        \
        SDK_LOG_ERROR("{} is nullptr", #ptr) \
        return MStatus::M_FAILED;            \
    }

#define CHECK_RT(func)                          \
    if (unlikely((func) != MStatus::M_OK)) {    \
        SDK_LOG_ERROR("{} return error", #func) \
        return MStatus::M_FAILED;               \
    }

} // namespace ADInference

#endif // _ADMETA_CORE_ERROR_HPP_
