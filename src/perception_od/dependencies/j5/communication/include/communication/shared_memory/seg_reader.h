// Copyright (c) [2021-2023] [Horizon Robotics][Horizon Bole].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     http: //www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#ifndef COMMUNICATIOIN_SEG_READER_H_
#define COMMUNICATIOIN_SEG_READER_H_
#include <vector>
#include <set>
#include <memory>
#include <string>
#include "seg_data.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#endif


namespace hobot {
namespace communication {

struct ReadableInfo;
class BlockAccesser;
class Referencer;
class NotifyCache;
class RefNotifier;

class DLLEXPORT SegReader {
 public:
  SegReader() = default;

  ~SegReader();

  bool Init();

  void DeInit();

  /*
  * read data that topic in vec_topics from segment
  * @vec_topics: needed topics
  * @timeout_millsec: timeout millseconds
  * @vec_msg: readed msgs
  */
  bool Read(const std::set<std::string>& vec_topics,
            int timeout_millsec,
            std::vector<std::shared_ptr<SegData>>& vec_msg);

 private:
  std::shared_ptr<SegData> read_one_msg(const ReadableInfo& readable_info);

 protected:
  std::shared_ptr<BlockAccesser> p_block_accesser_;
  std::shared_ptr<Referencer> p_ref_;
  std::shared_ptr<NotifyCache> p_noti_cache_;
  std::shared_ptr<RefNotifier> p_ref_noti_;
  bool inited{false};
  uint64_t last_seq_{0};
};

}  // namespace communication
}  // namespace hobot

#endif
