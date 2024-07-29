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

#ifndef COMMUNICATION_SEGMENT_WRITER_H_
#define COMMUNICATION_SEGMENT_WRITER_H_
#include <memory>
#include <mutex>
#include <vector>
#include "seg_data.h"

namespace hobot {
namespace communication {

class BlockAccesser;
class Referencer;
class NotifyCache;
class RefNotifier;
#if defined(SUPPORT_HBMEM)
class HbmemHolder;
class HbmemMsgHolder;
#endif

class DLLEXPORT SegWriter {
 public:
  /*
  * @p_ref: Register a reference to the block in the process of writing
  */
  SegWriter() = default;

  ~SegWriter();

  bool Init();

  void DeInit();

  // /*
  // * 1. write buf to blocks(>=1)
  // * 2. generate a ReadableInfo in NotifyCache
  // * 3. find the waiting referencers and post them
  // */
  virtual bool Write(std::shared_ptr<SegData> seg_msg);

 protected:
  /*
  * write buf to block, 
  * perhaps write multi blocks when msg_size > one block size
  * and fill block_indexs with these block's index
  */
  bool write_to_block(SegData& seg_msg,
                      std::vector<uint32_t>& vec_block_indexs,
                      uint32_t& msg_size);

 protected:

  std::shared_ptr<BlockAccesser> p_block_accesser_;
  std::shared_ptr<Referencer> p_ref_;
  std::shared_ptr<NotifyCache> p_noti_cache_;
  std::shared_ptr<RefNotifier> p_ref_noti_;
  uint32_t batch_num_{0};
  bool inited{false};
  std::mutex wr_mutex_;

#if defined(SUPPORT_HBMEM)
  bool hbmem_hold_com_buf_{true};
  std::shared_ptr<HbmemHolder> hbmem_holder_;
  std::shared_ptr<HbmemMsgHolder> hbmem_msg_holder_;
#endif
};

}  // namespace communication
}  // namespace hobot
#endif
