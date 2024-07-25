import os
import argparse
import shutil

proto_input_name = ""
proto_lower_name = ""
proto_title_name = ""
proto_upper_name = ""
vendor_name = ""

g_data_writer_hpp = '''// Copyright 2022 Horizon Robotics
// Created by shuxin.li on 07/05/2022

#ifndef PLUGIN_XYZ_DATA_WRITER_H_
#define PLUGIN_XYZ_DATA_WRITER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <mutex>
#include <condition_variable>

#include "communication/plugin_spec/plugin_interface.h"

namespace hobot {
namespace communication {
namespace plugin_xyz {

class PluginXyzDataWriter {
 public:
  explicit PluginXyzDataWriter(PluginXyzProperties &);
  ~PluginXyzDataWriter();

  PluginXyzDataWriter(const PluginXyzDataWriter &) = delete;
  PluginXyzDataWriter(PluginXyzDataWriter &&) = delete;
  PluginXyzDataWriter &operator=(const PluginXyzDataWriter &) = delete;
  PluginXyzDataWriter &operator=(PluginXyzDataWriter &&) = delete;

  int Enable();
  bool IsReady(const std::string &topic_name = "");
  bool GetInitStatus();

  int AsyncWrite(
      const std::string &topic_name,
      const spSerData &data,
      const spMsg &msg,
      const spMessageInfo &msg_info);

  int AddListener(const DataWriterResultCallback &callback);
  int AddEventListener(const DataWriterEventCallback &event_cb, void* key);
  int RemoveEventListener(void* key);
  bool IsNeedSerialize();
  void * GetMyPointer();

 private:
  std::vector<DataWriterResultCallback> listeners_;
  std::map<void*, std::shared_ptr<DataWriterEventCallback>> event_listeners_;

  // The socket link process maybe has finished when recved listener adding,
  // then send present_socket_event_ to listener
  DataWriterEventType present_socket_event_ = DATA_WRITER_EVENT_STATUS_MAX;
  std::mutex event_mtx_;
  bool init_status_{true};

 private:
  // TODO(user) Add your private variables
};
}  // namespace plugin_xyz
}  // namespace communication
}  // namespace hobot

#endif  // PLUGIN_XYZ_DATA_WRITER_H_
'''
g_data_writer_cpp = '''// Copyright 2022 Horizon Robotics
// Created by shuxin.li on 07/05/2022

#ifndef PLUGIN_XYZ_DATA_WRITER_CPP_
#define PLUGIN_XYZ_DATA_WRITER_CPP_

#include "plugin_xyz_data_reader.h"
#include "plugin_xyz_data_writer.h"
#include "communication/common/message_wrapper.h"

// TODO(user) If you need these uncomment it
/*
#include "common_protocol_msg/comm_protocol.pb.h"
#include "common_meta_encode/encode_tlv.h"
#include "common_utils/utils.h"
*/

#include <thread>
#include <utility>

namespace hobot {
namespace communication {
namespace plugin_xyz {

PluginXyzDataWriter::PluginXyzDataWriter(PluginXyzProperties &prop) {
  // TODO(user) create your Writer according to prop,
  // for example open an socket

  // TODO(user) set init_status_ to true, if above steps are OK
  init_status_ = true;
}

PluginXyzDataWriter::~PluginXyzDataWriter() {
  // TODO(user) Clean resource
}

int PluginXyzDataWriter::AddEventListener(
                            const DataWriterEventCallback &event_cb,
                            void* key) {
  event_listeners_[key] = event_cb;
  return 0;
}

int PluginXyzDataWriter::RemoveEventListener(void* key) {
  std::lock_guard<std::mutex> lg(event_mtx_);
  event_listeners_.erase(key);
  return 0;
}

bool PluginXyzDataWriter::IsReady(const std::string &topic_name) {
  (void) topic_name;
  // TODO(user) Add implementation to check your link state,
  // if link is OK, return true otherwise return false

  return true;
}

bool PluginXyzDataWriter::GetInitStatus() {
  return init_status_;
}

int PluginXyzDataWriter::AsyncWrite(const std::string &topic_name,
                                       const spSerData &data,
                                       const spMsg &msg,
                                       const spMessageInfo &msg_info) {
  int ret = 0;
  // TODO(user) Add implementation to send packets

  return 0;
}

int PluginXyzDataWriter::Enable() {
  // TODO(user) If your DataWriter is not enabled by default, enable it here.

  return 0;
}

bool PluginXyzDataWriter::IsNeedSerialize() {
  // TODO(user) If your DataWriter need Serialized data return true
  // normally all protocols need Serialized data here.
  return true;
}

int PluginXyzDataWriter::AddListener(
                            const DataWriterResultCallback &callback) {
  listeners_.push_back(callback);
  return 0;
}

void * PluginXyzDataWriter::GetMyPointer() {
  return this;
}

}  // namespace plugin_xyz
}  // namespace communication
}  // namespace hobot
#endif
'''
g_data_reader_hpp = '''// Copyright 2022 Horizon Robotics
// Created by shuxin.li on 07/05/2022

#ifndef PLUGIN_XYZ_DATA_READER_H_
#define PLUGIN_XYZ_DATA_READER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <utility>

#include "communication/plugin_spec/plugin_interface.h"

namespace hobot {
namespace communication {
namespace plugin_xyz {

struct PluginXyzProperties {
  std::string protocol;
  std::string link_info;
  std::string link_type;
  // TODO(user) Define your other config variables
  // int recv_buf_size;
  // int cust_cfg1;
  // int cust_cfg2;

  PluginXyzProperties() {
  }
};

class PluginXyzData {
  void SetTopicName(const std::string &topic_name) {
       topic_name_ = topic_name;
  }
  const std::string &GetTopicName() { return topic_name_; }
  // TODO(user) Add implementation of your data struct

 private:
  std::string topic_name_;
  // TODO(user) Add implementation of your data
};

class PluginXyzDataReader {
 public:
  explicit PluginXyzDataReader(PluginXyzProperties &);
  ~PluginXyzDataReader();

  PluginXyzDataReader(const PluginXyzDataReader &) = delete;
  PluginXyzDataReader(PluginXyzDataReader &&) = delete;
  PluginXyzDataReader &operator=(const PluginXyzDataReader &) = delete;
  PluginXyzDataReader &operator=(PluginXyzDataReader &&) = delete;
  int AddListener(const DataReaderCallback &callback);
  int RemoveListener();
  int AddListenTopic(const std::string &topic);
  int AddEventListener(const DataReaderEventCallback &event_cb, void* key);
  int RemoveEventListener(void* key);
  int Enable();
  bool IsReady(const std::string &topic_name = "");
  bool GetInitStatus();
  void * GetMyPointer();

 private:
  int Start();
  int Stop();

 private:
  std::vector<DataReaderCallback> listeners_;
  std::map<void*, std::shared_ptr<DataReaderEventCallback>> event_listeners_;

  bool start_ = false;
  std::mutex read_msg_mtx_;
  std::condition_variable read_msg_cond_;
  std::queue<std::pair<std::shared_ptr<PluginXyzData>, spMessageInfo>>
                                                        read_msg_cache_;
  uint64_t read_msg_cache_size_limit_ = 100;

  DataReaderEventType present_socket_event_ = DATA_READER_EVENT_STATUS_MAX;
  std::mutex event_mtx_;
  bool init_status_{true};
  bool is_bigendian = false;

  std::shared_ptr<std::thread> read_task_ptr_ = nullptr;
  std::shared_ptr<std::thread> feed_cb_task_ptr_ = nullptr;

 private:
  // TODO(user) Add your private variable
};
}  // namespace plugin_xyz
}  // namespace communication
}  // namespace hobot
#endif  // PLUGIN_XYZ_DATA_READER_H_
'''
g_data_reader_cpp = '''// Copyright 2022 Horizon Robotics
// Created by shuxin.li on 07/05/2022

#ifndef PLUGIN_XYZ_DATA_READER_CPP_
#define PLUGIN_XYZ_DATA_READER_CPP_

#include "plugin_xyz_data_reader.h"
#include "communication/common/message_wrapper.h"

// TODO(user) If you need these, uncomment it
/*
#include "common_protocol_msg/comm_protocol.pb.h"
#include "common_meta_encode/encode_tlv.h"
#include "common_utils/utils.h"
*/

#include <thread>
#include <utility>
#include <memory>
#include <string>

namespace hobot {
namespace communication {
namespace plugin_xyz {

PluginXyzDataReader::PluginXyzDataReader(PluginXyzProperties &prop) {
  // TODO(user) Add implementation here
  // Create link, for example to open a socket and bind it
}

PluginXyzDataReader::~PluginXyzDataReader() {
  Stop();
  // TODO(user) Add implementation to release other resource
}

int PluginXyzDataReader::AddEventListener(
                            const DataReaderEventCallback &event_cb,
                            void* key) {
  event_listeners_[key] = event_cb;
  return 0;
}

int PluginXyzDataReader::RemoveEventListener(void* key) {
  std::lock_guard<std::mutex> lg(event_mtx_);
  event_listeners_.erase(key);
  return 0;
}

int PluginXyzDataReader::Start() {
  int ret = 0;

  if (start_) return 0;
  start_ = true;

  // TODO(user) If you need implement read thread by yourself, change the
  // below two threads totally

  if (!feed_cb_task_ptr_) {
    feed_cb_task_ptr_ = std::make_shared<std::thread>(
        [this]() {
          // Plugin callback task start");
          while (start_) {
            // wait;
            std::unique_lock<std::mutex> lg(read_msg_mtx_);
            read_msg_cond_.wait(lg,
                                [this]() {
                                  return (!read_msg_cache_.empty() || !start_);
                                });
            if (!start_) {
              break;
            }
            // Has been waked, check if there is new msg
            if (read_msg_cache_.empty()) {
              continue;
            }

            // handle msg from queue
            auto msg = read_msg_cache_.front();
            read_msg_cache_.pop();
            lg.unlock();

            if (!listeners_.empty()) {
              for (const auto &cb : listeners_) {
                // run callback
                cb(msg.first->GetTopicName(), msg.first, msg.second, nullptr);
              }
            }
          }
         // feed callback task exit
        });
  }

  if (!read_task_ptr_) {
    // check if topic
    auto CheckIfNotTopic = [](const std::string &s) {
      if (s.empty()) {
        return true;
      }
      // if contain any of non-ASCII , is not topic
      return std::any_of(s.begin(), s.end(), [](char c) {
        return static_cast<unsigned char>(c) > 127;
      });
    };
    read_task_ptr_ = std::make_shared<std::thread>(
        [this, CheckIfNotTopic]() {
          // reader task start
          while (start_) {
            // TODO(user) Implement your receive functions


            auto recv_data = std::make_shared<PluginXyzData>();
            // TODO(user) parse raw data and fill to PluginXyzData

            std::shared_ptr<MessageInfo> msg_info = nullptr;
            // TODO(user) If trans include MessageInfo, uncomment follow and
            // add implementation
            /*
            if (has_msg_info) {
              msg_info = std::make_shared<MessageInfo>();
              msg_info->DeserializeFrom(ptr, size);
            }
            */

            // Put data to read_msg_cache and wake up feed_cb_task_ptr_ to cb
            std::unique_lock<std::mutex> lg(read_msg_mtx_);
            if (read_msg_cache_.size() >= read_msg_cache_size_limit_) {
              // "WARNNING!!! drop read msg from cache, cache limit: {}"
              read_msg_cache_.pop();
            }
            // put msg to cache queue
            read_msg_cache_.emplace(recv_data, msg_info);
            // wake feed thread
            read_msg_cond_.notify_one();
            lg.unlock();
          }
          // read task exit
          // TODO(user) Release some resource if you need
        });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  return ret;
}

int PluginXyzDataReader::Stop() {
  if (!start_) return 0;
  start_ = false;

  // Wait 100ms for thread exit, otherwise close socket directly
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // TODO(user) Clean resource, fd, memory...
  return 0;
}

int PluginXyzDataReader::Enable() {
  return Start();
}

bool PluginXyzDataReader::IsReady(const std::string &topic_name) {
  (void) topic_name;
  // TODO(user) check your link status and return true if it's linked

  return true;
}

bool PluginXyzDataReader::GetInitStatus() {
  return init_status_;
}

int PluginXyzDataReader::AddListener(const DataReaderCallback &callback) {
  listeners_.emplace_back(callback);
  return 0;
}
int PluginXyzDataReader::RemoveListener() {
  return 0;
}
int PluginXyzDataReader::AddListenTopic(const std::string& topic) {
  // TODO(user) If your com lib support topic filter, enable filter here.

  (void) topic;
  return 0;
}
void * PluginXyzDataReader::GetMyPointer() {
  return this;
}
}  // namespace plugin_xyz
}  // namespace communication
}  // namespace hobot
#endif
'''
g_plugin_intf_cpp = '''// Copyright 2022 Horizon Robotics
// Created by shuxin.li on 07/05/2022

#include "communication/plugin_spec/plugin_interface.h"
#include "plugin_xyz_data_reader.h"
#include "plugin_xyz_data_writer.h"

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <stdlib.h>

#ifndef _WIN32
#define DLL_PUBLIC  __attribute__ ((visibility("default")))
#else
#define DLL_PUBLIC
#endif
namespace hobot {
namespace communication {
namespace plugin_xyz {

// Used to save assigned ProtocolType when plugin init.
static int my_protocol_type = -1;
// Used to implement properties, must init entries in PlugInit
static std::map<std::string, std::string> property_ext;

// key is data reader, data writer pointer, record readers and writers
std::map<void*, std::shared_ptr<PluginXyzDataWriter>> writer_list;
std::map<void*, std::shared_ptr<PluginXyzDataReader>> reader_list;

std::string GenerateDefaultLinkinfo() {
  std::string def_link = "";
  // TODO(user): Add your code to generate an default link info

  return def_link;
}


#ifdef __cplusplus
extern "C" {
#endif

int DLLEXPORT
PlugInit(int protocol_type, const std::string &protocol_name,
         std::shared_ptr<PluginRetInfo> &ret_info) {
  if (protocol_name == "PROTOCOL_NAME") {
    // TODO(user): if you have some configuration you can use property_ext
    // example: property_ext["configA"] = "default";

    // TODO(user): Define your version here
    ret_info->plugin_ver = 1;

    ret_info->plugin_interface_ver = COM_PLUGIN_INTF_VER;
    // TODO(user): If your prococol support dynamic discovery,
    // set to 1, otherwise set to 0
    ret_info->dynamic_support = 0;

    // TODO(user): Update your name
    ret_info->vendor = "Vendor_Name_Str";

    ret_info->property_ext = property_ext;
    my_protocol_type = protocol_type;
  } else {
    // TODO(user) Plugin doesn't support protocol {}");
    return -1;
  }

  return 0;
}

int  DLLEXPORT
PlugGenerateAutoDiscoveryLinkInfo(int protocol_type,
                                  const std::string &protocol_name,
                                  std::shared_ptr<std::string> &link_info) {
  (void)protocol_type;
  if (protocol_name == "PROTOCOL_NAME") {
      std::string def_link_info = GenerateDefaultLinkinfo();
      link_info->append(def_link_info);
  } else {
    // protocol name is not correct
    return -1;
  }
  return 0;
}

int DLLEXPORT
PlugGetQosCapability(int protocol_type,
                const std::string &protocol_name,
                QosCapability &qos_capability) {
  (void)protocol_name;
  if (protocol_type != my_protocol_type) {
    return -1;
  }
  // TODO(user): Set QoS support capability according to your capability
  qos_capability.is_transport_priority_qos_supported = false;
  qos_capability.is_reliability_qos_supported = false;
  return 0;
}

int DLLEXPORT
PlugUpdatePropertyExt(int protocol_type,
                                      const std::string &protocol_name,
                                      const std::string & key,
                                      const std::string & value) {
  (void)protocol_name;
  if (protocol_type != my_protocol_type) {
    return -1;
  }
  if (key.size() == 0) {
    return -2;
  }

  if (property_ext.count(key) == 0) {
    // Key {} not exist, return error
    return -3;
  }

  property_ext[key] = value;
  // TODO(user): Add your implementation for key value setting

  return 0;
}

int DLLEXPORT
PlugCreateDataWriter(const DataReaderWriterInfo &info,
                     void* &handle) {
  PluginXyzProperties prop;

  if (info.protocol_ == "PROTOCOL_NAME") {
    prop.protocol = info.protocol_;
    prop.link_info = info.link_info_;
    // TODO(user): Set link_type to "default", "connect" or "bind"
    // If your code support different link type
    prop.link_type = std::string("default");

    // TODO(user): if you have other private config, you can set here
    // for example: prop.cust_cfg1 = 1;

    if (nullptr != info.config_options) {
      // TODO(user) update config from config_options which is parsed from top

    } else if (info.config_options_str.size() != 0) {
      // TODO(user) parse link options from string which is passed from top

    } else {
      // Do nothing
    }
  } else {
    // Not supported protocol
    return -1;
  }

  auto data_writer = std::make_shared<PluginXyzDataWriter>(prop);
  if (data_writer->GetInitStatus()) {
    // Construct PluginXyzDataWriter succeed
    handle = data_writer->GetMyPointer();
    writer_list[handle] = data_writer;
    return 0;
  } else {
    // Construct PluginXyzDataWriter fail
    return -1;
  }
}

int DLLEXPORT
PlugDestroyDataWriter(void* handle) {
  // Destroy DataWriter
  if (writer_list.count(handle) != 0) {
    std::map<void*,  std::shared_ptr<PluginXyzDataWriter>>::iterator pos =
                                                    writer_list.find(handle);
    if (pos != writer_list.end()) {
      // Find and erase record from writer_list
      writer_list.erase(pos);
    }
  }
  return 0;
}

int DLLEXPORT
PlugDestroyDataReader(void* handle) {
  if (reader_list.count(handle) != 0) {
    // Destroy DataReader
    reader_list[handle] = nullptr;
    std::map<void*,  std::shared_ptr<PluginXyzDataReader>>::iterator pos =
                                                    reader_list.find(handle);
    if (pos != reader_list.end()) {
      // Find and erase record from reader list
      reader_list.erase(pos);
    }
  } else {
    // Not find
  }
  return 0;
}

int DLLEXPORT
PlugCreateDataReader(const DataReaderWriterInfo &info,
                     void * &handle) {
  (void)handle;
  PluginXyzProperties prop;

  bool json_parsed = false;
  nlohmann::json config_options_json;
  if (!info.config_options_str.empty()) {
    // TODO(user) If support input config string, parse here

  } else {
    // ("info.config_options_str.empty() == true");
  }

  if (info.protocol_ == "PROTOCOL_NAME") {
      prop.protocol = info.protocol_;
      prop.link_info = info.link_info_;
      prop.link_type = std::string("bind");
      // TODO(user) If you have other private config do it here
      // for example: prop.cust_cfg1 = 1;

      if (info.config_options != nullptr) {
        // TODO(user) If use structure configuration, handle it here
      }
  } else {
    // protocol {info.protocol_} is not supported
    return -1;
  }

  auto data_reader = std::make_shared<PluginXyzDataReader>(prop);
  auto ret = data_reader->GetInitStatus();
  if (ret) {
    handle = data_reader->GetMyPointer();
    reader_list[handle] = data_reader;
    return 0;
  } else {
    return -1;
  }
}

int DLLEXPORT
PlugWriterAsyncWrite(void * handle,
      const std::string &topic_name,
      const spSerData &data,
      const spMsg &msg,
      const spMessageInfo &msg_info) {
  (void)msg_info;
  int ret;
  PluginXyzDataWriter *p_writer;
  p_writer = static_cast<PluginXyzDataWriter*>(handle);
  ret = p_writer->AsyncWrite(topic_name, data, msg, nullptr);
  return ret;
}

int DLLEXPORT
PlugWriterAddListener(void * handle,
                      const DataWriterResultCallback &callback) {
  int ret;
  PluginXyzDataWriter *p_writer;
  p_writer = static_cast<PluginXyzDataWriter*>(handle);
  ret = p_writer->AddListener(callback);
  return ret;
}

int DLLEXPORT
PlugWriterAddEventListener(void * handle,
                           const DataWriterEventCallback &event_cb,
                           void* key) {
  int ret;
  PluginXyzDataWriter *p_writer;
  p_writer = static_cast<PluginXyzDataWriter*>(handle);
  ret = p_writer->AddEventListener(event_cb, key);
  return ret;
}

int DLLEXPORT
PlugWriterRemoveEventListener(void * handle, void* key) {
  int ret;
  PluginXyzDataWriter *p_writer;
  p_writer = static_cast<PluginXyzDataWriter*>(handle);
  ret = p_writer->RemoveEventListener(key);
  return ret;
}

int DLLEXPORT
PlugWriterEnable(void * handle) {
  int ret;
  PluginXyzDataWriter *p_writer;
  p_writer = static_cast<PluginXyzDataWriter*>(handle);
  ret = p_writer->Enable();
  return ret;
}

int DLLEXPORT
PlugReaderAddListener(void * handle, const DataReaderCallback &callback) {
  int ret;
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  ret = p_reader->AddListener(callback);
  return ret;
}

int DLLEXPORT
PlugReaderRemoveListener(void * handle) {
  int ret;
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  ret = p_reader->RemoveListener();
  return ret;
}

int DLLEXPORT
PlugReaderAddEventListener(void * handle,
                           const DataReaderEventCallback &event_cb,
                           void* key) {
  int ret;
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  ret = p_reader->AddEventListener(event_cb, key);
  return ret;
}

int DLLEXPORT
PlugReaderRemoveEventListener(void * handle, void* key) {
  int ret;
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  ret = p_reader->RemoveEventListener(key);
  return ret;
}

int DLLEXPORT
PlugReaderAddListenTopic(void * handle, const std::string &topic) {
  int ret;
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  ret = p_reader->AddListenTopic(topic);
  return ret;
}

int DLLEXPORT
PlugReaderEnable(void * handle) {
  int ret;
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  ret = p_reader->Enable();
  return ret;
}

bool DLLEXPORT
PlugWriterIsReady(void * handle, const std::string &topic) {
  PluginXyzDataWriter *p_writer;
  p_writer = static_cast<PluginXyzDataWriter*>(handle);
  if (p_writer == nullptr) {
    // plugin can't get valid handle;
    return false;
  }

  return p_writer->IsReady(topic);
}

bool DLLEXPORT
PlugReaderIsReady(void * handle, const std::string &topic) {
  PluginXyzDataReader *p_reader;
  p_reader = static_cast<PluginXyzDataReader*>(handle);
  if (p_reader == nullptr) {
    // plugin can't get valid handle"
    return false;
  }

  return p_reader->IsReady(topic);
}

#ifdef __cplusplus
}
#endif

}  // namespace plugin_xyz
}  // namespace communication
}  // namespace hobot
'''


def DoReplaceOnePattern(filename):
    # loop all files
    fo = open(filename, mode="r")
    content = fo.read(-1)
    fo.close()

    content = content.replace("PROTOCOL_NAME", proto_input_name)
    content = content.replace("PluginXyz", proto_title_name)
    content = content.replace("plugin_xyz", proto_lower_name)
    content = content.replace("PLUGIN_XYZ", proto_upper_name)
    content = content.replace("Vendor_Name_Str", vendor_name)

    fo = open(filename, mode="w")
    fo.write(content)
    fo.close()


def DoReplaceForAllFiles(dirPath):
    # loop all files
    for file in os.listdir(dirPath):
        # check if is file
        if os.path.isfile(os.path.join(dirPath, file)) is True:
            print("Do replace for file %s" % file)
            DoReplaceOnePattern(os.path.join(dirPath, file))


def CopyFiles(srcPath, dstPath):
    # loop all files
    for file in os.listdir(srcPath):
        # check if is file
        if os.path.isfile(os.path.join(srcPath, file)) is True:
            shutil.copy(os.path.join(srcPath, file),
                        os.path.join(dstPath, file))


def RenameFiles(dirPath, pattern):
    # loop all files
    for file in os.listdir(dirPath):
        # check if is file
        if os.path.isfile(os.path.join(dirPath, file)) is True:
            newFilename = file.replace("plugin_xyz", pattern)
            os.rename(os.path.join(dirPath, file),
                      os.path.join(dirPath, newFilename))


def CreateFile(dirPath, filename, content):
    fo = open(os.path.join(dirPath, filename), mode="w")
    fo.write(content)
    fo.close()


def CreateFiles(dirPath):

    global g_data_writer_hpp
    global g_data_writer_cpp
    global g_data_reader_hpp
    global g_data_reader_cpp
    global g_plugin_intf_cpp
    CreateFile(dstPath, r"plugin_xyz_data_reader.cpp", g_data_reader_cpp)
    CreateFile(dstPath, r"plugin_xyz_data_reader.h", g_data_reader_hpp)
    CreateFile(dstPath, r"plugin_xyz_data_writer.cpp", g_data_writer_cpp)
    CreateFile(dstPath, r"plugin_xyz_data_writer.h", g_data_writer_hpp)
    CreateFile(dstPath, r"plugin_xyz_interface.cpp", g_plugin_intf_cpp)


def GenReplaceStr(input_proto):

    global proto_lower_name
    global proto_upper_name
    global proto_title_name
    proto_lower_name = "plugin_" + str.lower(input_proto)
    proto_upper_name = "PLUGIN_" + str.upper(input_proto)
    proto_title_name = "Plugin" + str.title(input_proto)


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='ArgUtils')
    parser.add_argument('-p', type=str, default='', help="protocol name")
    parser.add_argument('-m', type=str, default='', help="vendor name")
    args = parser.parse_args()

    if (args.p == '' or args.m == ''):
        print("Usage: -p <protocol name, without space charactor>")
        print("Usage: -m <manufacture name>")
        print("")
        exit

    proto_input_name = args.p
    vendor_name = args.m
    print("input protocol name=%s" % proto_input_name)
    print("manufactory name =%s" % vendor_name)

    GenReplaceStr(proto_input_name)

    print("lower string: %s" % proto_lower_name)
    print("upper string: %s" % proto_upper_name)
    print("title string: %s" % proto_title_name)

    srcPath = r"template"
    dstPath = proto_lower_name
    if (os.path.exists(dstPath)):
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("Folder %s already exist" % dstPath)
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        exit

    os.mkdir(dstPath)
    # CopyFiles(srcPath, dstPath)
    CreateFiles(dstPath)
    RenameFiles(dstPath, proto_lower_name)
    DoReplaceForAllFiles(dstPath)
    print("")
    print("Plugin template code %s is generated successfully" % dstPath)
    print("Please move folder %s to protocol_impl_plugin" % dstPath)
    print("")
