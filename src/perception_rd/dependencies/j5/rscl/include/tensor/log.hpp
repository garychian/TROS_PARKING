#pragma once
#ifndef LOG_WRAPPER_H_
#define LOG_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>
///
/// log level enum definition.
///
typedef enum ad_log_level_e {
    /** Trace log for detail debug */
    AD_LOG_TRACE = 0,
    /** log for debug */
    AD_LOG_DEBUG = 1,
    /** log for info */
    AD_LOG_INFO = 2,
    /** log for warning */
    AD_LOG_WARNING = 3,
    /** log for error */
    AD_LOG_ERROR = 4,
} ad_log_level_e;

/// Log meta structure
///
typedef struct ad_log_meta_t {
    ad_log_level_e level; /**< Log level */
    int32_t line_no;
    const char* filename;
} ad_log_meta_t;

/// @brief Set log output level
/// @param[in] level Output level, all log info that has higher level then it will output.
void ad_log_set_level(ad_log_level_e level);

/// @brief Get log output level
/// @return Current log output level
ad_log_level_e ad_log_get_level();

int32_t ad_log(ad_log_meta_t* meta, char* fmt, ...);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#include "ad_log/ad_log.hpp"
#pragma GCC diagnostic pop

#define _SDK_LOG_TRACE(ft, ...)                               \
    do {                                                      \
        std::string fmt_str = fmt::format(ft, ##__VA_ARGS__); \
        AD_LTRACE() << fmt_str;                               \
    } while (0)

#define _SDK_LOG_DEBUG(ft, ...)                               \
    do {                                                      \
        std::string fmt_str = fmt::format(ft, ##__VA_ARGS__); \
        AD_LDEBUG() << fmt_str;                               \
    } while (0)

#define _SDK_LOG_INFO(ft, ...)                                \
    do {                                                      \
        std::string fmt_str = fmt::format(ft, ##__VA_ARGS__); \
        AD_LINFO() << fmt_str;                                \
    } while (0)

#define _SDK_LOG_WARN(ft, ...)                                \
    do {                                                      \
        std::string fmt_str = fmt::format(ft, ##__VA_ARGS__); \
        AD_LWARN() << fmt_str;                                \
    } while (0)

#define _SDK_LOG_ERROR(ft, ...)                               \
    do {                                                      \
        std::string fmt_str = fmt::format(ft, ##__VA_ARGS__); \
        AD_LERROR() << fmt_str;                               \
    } while (0)
#else
#define _SDK_LOG_TRACE(ft, ...)                                  \
    do {                                                         \
        ad_log_meta_t meta = {AD_LOG_TRACE, __LINE__, __FILE__}; \
        ad_log(&meta, ft, ##__VA_ARGS__);                   \
    } while (0)

#define _SDK_LOG_DEBUG(ft, ...)                                  \
    do {                                                         \
        ad_log_meta_t meta = {AD_LOG_DEBUG, __LINE__, __FILE__}; \
        ad_log(&meta, ft, ##__VA_ARGS__);                   \
    } while (0)

#define _SDK_LOG_INFO(ft, ...)                                  \
    do {                                                        \
        ad_log_meta_t meta = {AD_LOG_INFO, __LINE__, __FILE__}; \
        ad_log(&meta, ft, ##__VA_ARGS__);                  \
    } while (0)

#define _SDK_LOG_WARN(ft, ...)                                     \
    do {                                                           \
        ad_log_meta_t meta = {AD_LOG_WARNING, __LINE__, __FILE__}; \
        ad_log(&meta, ft, ##__VA_ARGS__);                     \
    } while (0)

#define _SDK_LOG_ERROR(ft, ...)                                  \
    do {                                                         \
        ad_log_meta_t meta = {AD_LOG_ERROR, __LINE__, __FILE__}; \
        ad_log(&meta, ft, ##__VA_ARGS__);                   \
    } while (0)
#endif

#define SDK_LOG_TRACE(...) \
    { _SDK_LOG_TRACE(__VA_ARGS__, ""); }
#define SDK_LOG_DEBUG(...) \
    { _SDK_LOG_DEBUG(__VA_ARGS__, ""); }
#define SDK_LOG_INFO(...) \
    { _SDK_LOG_INFO(__VA_ARGS__, ""); }
#define SDK_LOG_WARN(...) \
    { _SDK_LOG_WARN(__VA_ARGS__, ""); }
#define SDK_LOG_ERROR(...) \
    { _SDK_LOG_ERROR(__VA_ARGS__, ""); }
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
__declspec(dllexport) void ad_log_set_level_int(int le);
#else
__attribute__((visibility("default"))) void ad_log_set_level_int(int le);
#endif

#ifdef __cplusplus
}

#endif // LOG_WRAPPER_H_
