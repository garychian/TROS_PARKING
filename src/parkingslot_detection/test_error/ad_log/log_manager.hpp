/*
 * Copyright (C) 2019 by SenseTime Group Limited. All rights reserved.
 * LiangYu <liangyu@sensetime.com>
 */

#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <atomic>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

#include "spdlog/spdlog.h"

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_CRITICAL 5
#define LOG_LEVEL_OFF 6

namespace senseAD {

struct LogConfig {
    bool color = true;
    bool simple_file = true;
    bool rotating_file = false;
    bool daily_file = false;
    bool async = false;
    uint64_t max_size = 1024;
    uint32_t max_files = 3;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t async_thread_nums = 1;
    uint32_t async_queue_length = 1024;
    uint8_t async_overflow_policy = 0;
    uint8_t tostderr = 0;
    std::string severity = "info";
    std::string path = "/tmp";
    std::string pattern = "%D %X [%l%$] %v";
};

class StartLogTime {
 public:
    StartLogTime(const StartLogTime &) = delete;
    StartLogTime &operator=(const StartLogTime &) = delete;
    static StartLogTime &Instance();
    std::string GetStartTime() { return startTime_; }

 private:
    void SetStartTime(std::string startTime) { this->startTime_ = startTime; }
    StartLogTime();
    std::string startTime_;
};

class LogManager {
 public:
    LogManager(const LogManager &) = delete;
    LogManager &operator=(const LogManager &) = delete;

    static LogManager &Instance();
    ~LogManager();

 public:
    template <typename... Args>
    void TRACE(const Args &... args) {
        logger_->trace(args...);
    }

    template <typename... Args>
    void DEBUG(const Args &... args) {
        logger_->debug(args...);
    }

    template <typename... Args>
    void INFO(const Args &... args) {
        logger_->info(args...);
    }

    template <typename... Args>
    void WARN(const Args &... args) {
        logger_->warn(args...);
    }

    template <typename... Args>
    void ERROR(const Args &... args) {
        logger_->error(args...);
    }

    template <typename... Args>
    void CRITICAL(const Args &... args) {
        logger_->critical(args...);
    }

    inline bool ShouldLogging(int severity) { return severity >= level_; }

    void Flush();

    void SetLogger(const std::string &);

    void Reset();

 private:
    LogManager();

    template <typename Derived, typename... Ts>
    std::shared_ptr<spdlog::logger> SetupLogger(Ts &&... args) {
        auto logger = std::make_shared<Derived>(std::forward<Ts>(args)...);
        return logger;
    }

 private:
    std::shared_ptr<spdlog::logger> logger_ = nullptr;

    LogConfig config_;
    int level_ = LOG_LEVEL_ERROR;
    std::unordered_map<std::string, int> level_map_ = {
        {"trace", LOG_LEVEL_TRACE}, {"debug", LOG_LEVEL_DEBUG},
        {"info", LOG_LEVEL_INFO},   {"warn", LOG_LEVEL_WARN},
        {"error", LOG_LEVEL_ERROR}, {"critical", LOG_LEVEL_CRITICAL},
        {"off", LOG_LEVEL_OFF}};
};

class FileLogManager {
 public:
    FileLogManager(const FileLogManager &) = delete;
    FileLogManager &operator=(const FileLogManager &) = delete;

    static FileLogManager &Instance();
    ~FileLogManager();

 public:
    template <typename... Args>
    void TRACE(const Args &... args) {
        logger_->trace(args...);
    }

    template <typename... Args>
    void DEBUG(const Args &... args) {
        logger_->debug(args...);
    }

    template <typename... Args>
    void INFO(const Args &... args) {
        logger_->info(args...);
    }

    template <typename... Args>
    void WARN(const Args &... args) {
        logger_->warn(args...);
    }

    template <typename... Args>
    void ERROR(const Args &... args) {
        logger_->error(args...);
    }

    template <typename... Args>
    void CRITICAL(const Args &... args) {
        logger_->critical(args...);
    }

    void Flush();

    void SetLogger(const std::string &);

    void Reset();

    inline bool ShouldLogging(int severity) { return severity >= level_; }

 private:
    FileLogManager();

    template <typename Derived, typename... Ts>
    std::shared_ptr<spdlog::logger> SetupLogger(Ts &&... args) {
        auto logger = std::make_shared<Derived>(std::forward<Ts>(args)...);
        return logger;
    }

 private:
    std::shared_ptr<spdlog::logger> logger_ = nullptr;
    LogConfig config_;
    int level_ = LOG_LEVEL_ERROR;
    std::unordered_map<std::string, int> level_map_ = {
        {"trace", LOG_LEVEL_TRACE}, {"debug", LOG_LEVEL_DEBUG},
        {"info", LOG_LEVEL_INFO},   {"warn", LOG_LEVEL_WARN},
        {"error", LOG_LEVEL_ERROR}, {"critical", LOG_LEVEL_CRITICAL},
        {"off", LOG_LEVEL_OFF}};
};

}  // namespace senseAD
