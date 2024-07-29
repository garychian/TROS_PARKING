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

#ifndef COMMUNICATION_SHM_SEG_DATA_H_
#define COMMUNICATION_SHM_SEG_DATA_H_
#include <string>
#include <vector>
#if defined(SUPPORT_HBMEM)
#include "hb_mem_mgr.h"
#endif

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#endif


namespace hobot {
namespace communication {

#define DATA_TYPE_NORMAL 0
#define DATA_TYPE_HBMEM_VADDR 1
#define DATA_TYPE_HBMEM_GRAPH_DATA 2

struct DLLEXPORT SegData {
  SegData();
  virtual ~SegData();
  /**
   * @brief topic is used for filtering the messages which user cares about;
   * 
   */
  std::string topic;

  /*
  * multi data pointer in it,the data pointer's life cycle is:
  * write process: hanle by serialized data
  * read process: new it in seg_reader::read, 
  *               and copyed this pointer to serialized data,
  *               user need delete it after no use of it
  */
  /**
   * @brief @c vec_data supports batch data transportation for saving
   * read/write calls. Besides vec_data supports transporting normal heap data,
   * hbmem_com_buf_t_vaddr data and hbmem_graph_data simultaneously.
   * @note SegMsg @b lifetime managerment:
   * reader process: create SegMsg and read from shared memroy, and user should
   * call @c ReleaseMe() after not using of it
   * writer process: create SegMsg and just write this message, shared memory
   * library will take over this message lifetime ownership.
   */
  std::vector<char*> vec_data;
  std::vector<uint32_t> vec_data_size;

  /**
   * @brief Get vec_data_type
   *
   * @return std::vector<uint8_t>
   */
  std::vector<uint8_t> GetDataType();

  /**
   * @brief release message memory. Reader needs to release memory manually
   * after using message.
   *
   */
  virtual void ReleaseMe();

 private:
  friend class SegWriter;
  friend class SegReader;
  /**
   * @brief @c vec_data_type is @b read-only field, shared memory library will
   * take care this field. Currently shared memory library supports the
   * following data type: 
   * 0-DATA_TYPE_NORMAL
   * 1-DATA_TYPE_HBMEM_VADDR
   * 2-DATA_TYPE_HBMEM_GRAPH_DATA
   *
   */
  std::vector<uint8_t> vec_data_type;
};

/**
 * @brief init shared memory, which should be called before using shared memory
 * communication
 * @param shm_total_size: the total request shared memory size, units : byte,
 * default value: 100 Mb
 * @param one_block_size: one writable block size, units : byte, default value
 * : 1 Mb
 * @param multicast_domain: @c multicast_domain is used for discovery, which
 * means if a pair of SegReader and SegWriter process was inited in different
 * @c multicast_domain, they can not communication each other even their topic
 *  is matched. default multicast_domain value: empty string.
 * @param hbmem_hold_time: when using hbmem, @c hbmem_hold_time sets the maximum
 * keeping time for hbmem data. For example, if SegWriter write one message
 * which contains hbmem data, the shared memory library will keep the message
 * util it has been read or the @c hbmem_hold_time ran out! hbmem_hold_time
 * default value : 10ms if not using hbmem, ignore this parameter
 *
 * @note It's recommend that @c one_block_size best to set equal or great equal
 * to your message size
 */
bool DLLEXPORT shm_init(size_t shm_total_size = 100 * 1024 * 1024,
              size_t one_block_size = 1024 * 1024,
              const std::string& multicast_domain = "",
              int hbmem_hold_time = 10,
              bool hbmem_hold_com_buf = true);

/**
 * @brief deinit shared memory, which should be called before process exit! This
 * api clean up shared memory resource.
 */
void DLLEXPORT shm_deinit();

}  // namespace communication
}  // namespace hobot
#endif
