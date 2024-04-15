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
#include <dirent.h>
#include <fstream>
#include <string>
#include <vector>

namespace fanya {
namespace parking {

static inline void GetSubFiles(const std::string& path,
                               std::vector<std::string>& vec_files) {
  vec_files.clear();

  struct dirent* entry;
  DIR* dp = opendir(path.c_str());
  if (NULL == dp) {
    return;
  }

  while ((entry = readdir(dp))) {
    if ((0 == strcmp(entry->d_name, ".")) ||
        (0 == strcmp(entry->d_name, ".."))) {
      continue;
    }
    if (DT_REG == entry->d_type) {
      vec_files.emplace_back(entry->d_name);
    }
  }

  std::sort(
      vec_files.begin(), vec_files.end(), [](std::string& a, std::string& b) {
        try {
          uint64_t ull_a = std::stoull(
              a.substr(a.rfind('_') + 1, a.rfind('.') - a.rfind('_') - 1));

          uint64_t ull_b = std::stoull(
              b.substr(b.rfind('_') + 1, b.rfind('.') - b.rfind('_') - 1));
          return ull_a < ull_b;
        } catch (std::exception& e) {
          throw;
        }
      });

  closedir(dp);
}

static inline void LoadFile(const std::string& file_path,
                            std::vector<uint8_t>& content) {
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    return;
  }
  uint64_t size = file.tellg();
  file.seekg(0);
  content.resize(size);
  file.read(reinterpret_cast<char*>(content.data()), size);
}

}  // namespace parking
}  // namespace fanya