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

#ifndef SRC_PUB_SUB_HBMEM_MESSAGE_SAMPLE_H_
#define SRC_PUB_SUB_HBMEM_MESSAGE_SAMPLE_H_
#include <communication/common/types.h>
#include <protocol_msg/simple_image.pb.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "hb_mem_mgr.h"

using hobot::communication::Message;
using hobot::communication::ProtoMsg;
using hobot::communication::ProtobufSerializer;
using hobot::communication::DataRef;
using hobot::communication::DefaultSerializer;

using ImgProtoMsg = ProtoMsg<simple_proto::SimpleImage>;

// for normal serialize
using ImagProtoSerializer = ProtobufSerializer<simple_proto::SimpleImage>;

uint64_t one_data_size = 1024*1024*1;

int64_t flags = HB_MEM_USAGE_CPU_READ_OFTEN |
                HB_MEM_USAGE_CPU_WRITE_OFTEN |
                HB_MEM_USAGE_CACHED;

class HbmemFakeImage : public ImgProtoMsg {
 public:
  HbmemFakeImage(int width,
            int height)
        : ImgProtoMsg() {
    data_ref_ = std::make_shared<DataRef>();
    SetData(data_ref_);

    proto.set_width(width);
    proto.set_height(height);
    proto.set_channel(3);
    proto.set_image_desc("This is a test image!");
    // just for test set
    SetGenTimestamp(width + 1000);
    SetDoneTimestamp(width + 2000);
  }

  ~HbmemFakeImage() {
    if (com_buf_.fd) {
      hb_mem_free_buf(com_buf_.fd);
    }
    if (data_normal_) {
      delete data_normal_;
    }
  }

  void add_data(char* data, int size) {
    data_ref_->PushData(data, size);
  }

  bool init(int seq) {
    if (hb_mem_alloc_com_buf(one_data_size, flags, &com_buf_) != 0) {
      std::cout << "hb_mem_alloc_com_buf failed" << std::endl;
      return false;
    }
    char* data = (char*)com_buf_.virt_addr;
    std::string data_msg = std::string("this is message ")
                          + std::to_string(seq) + std::string("_0");
    memcpy(data, data_msg.c_str(), data_msg.size());
    data[data_msg.size()] = 0;

    data_ref_->PushData(data, one_data_size);

    // you can pub hbmem and normal mem at one msg
    data_normal_ = new char[one_data_size];
    data_msg = std::string("this is message ")
                          + std::to_string(seq) + std::string("_1");
    memcpy(data_normal_, data_msg.c_str(), data_msg.size());
    data_normal_[data_msg.size()] = 0;
    data_ref_->PushData(data_normal_, one_data_size);
    
    return true;
  }
  
 private:
  hobot::message::spDataRef data_ref_;
  hb_mem_common_buf_t com_buf_{0,};
  char* data_normal_{nullptr};
};


#endif