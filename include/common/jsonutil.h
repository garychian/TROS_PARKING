// Copyright (c) [2021-2023] [Horizon Robotics].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "nlohmann_json/json.hpp"

// namespace fanya {
// namespace parking {

typedef uint64_t TimeStamp;

class JSONUtil {
 public:
  static nlohmann::json ParseJson(const std::string& config_file) {
    std::ifstream cfg_file(config_file);
    if (!cfg_file.is_open()) {
      std::cout << "open cfg_file:" << config_file << " failed!" << std::endl;
      return nullptr;
    }
    nlohmann::json json_file = nlohmann::json::parse(cfg_file);
    if (json_file == nullptr) {
      std::cout << "config file empty" << std::endl;
      return nullptr;
    }
    return json_file;
  }

  static std::string GetWorkingDir(const std::string& config_file) {
    std::string config_dir;
    size_t pos = config_file.rfind('/');
    if (pos == std::string::npos) {
      pos = config_file.rfind('\\');
    }
    if (pos == std::string::npos) {
      config_dir = "./";
    } else {
      config_dir = config_file.substr(0, pos);
    }
    return config_dir;
  }
};

// }  // namespace parking
// }  // namespace fanya
