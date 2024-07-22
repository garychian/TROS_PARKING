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

#ifndef COMMUNICATION_THREAD_NAME_TYPES_H_
#define COMMUNICATION_THREAD_NAME_TYPES_H_
#include <string>

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

#ifdef COM_THREAD_ZMQ
  #define COM_THREAD_EXTEND_NS_HEAD namespace com_thread_ns_zmq {
  #define COM_THREAD_EXTEND_NS_TAIL }
#elif COM_THREAD_PCIE
  #define COM_THREAD_EXTEND_NS_HEAD namespace com_thread_ns_pcie {
  #define COM_THREAD_EXTEND_NS_TAIL }
#else

#endif

#ifndef COM_THREAD_EXTEND_NS_HEAD
#define COM_THREAD_EXTEND_NS_HEAD
#endif

#ifndef COM_THREAD_EXTEND_NS_TAIL
#define COM_THREAD_EXTEND_NS_TAIL
#endif



namespace hobot {
namespace communication {

COM_THREAD_EXTEND_NS_HEAD

namespace ThreadName {
constexpr char threadname_shm[]{"CommShmReader"};
constexpr char threadname_discovery[]{"CommDiscover"};
constexpr char threadname_iowatcher[]{"CommIowatcher"};
constexpr char threadname_udp_server[]{"CommUdpServer"};
constexpr char threadname_zmq_feed_cd[]{"CommZmqFeedCb"};
constexpr char threadname_zmq_read_task[]{"CommZmqReadTk"};
constexpr char threadname_zmq_read_monitor_task[]{"CommZmqReadMoni"};
constexpr char threadname_zmq_write_monitor_task[]{"CommZmqWriMoni"};
constexpr char threadname_pcie_feed_cd[]{"CommPcieFeedCd"};
constexpr char threadname_pcie_read_task[]{"CommPcieReadTk"};
constexpr char threadname_pcie_recovery_task[]{"CommPcieRecover"};
constexpr char threadname_pcie_send_task[]{"CommPcieSendTk"};
constexpr char threadname_pcie_watch_mem_task[]{"CommPcieWatchMe"};
constexpr char threadname_pcie_watch_connect_task[]{"CommPcieWatchCo"};
constexpr char threadname_pcie_heart_beat_task[]{"CommPcieHbTk"};
constexpr char threadname_pcie_ep_heartbeat_task[]{"CommPcieEpHb"};
constexpr char threadname_pcie_map_st_task[]{"CommPcieMapSt"};
constexpr char threadname_long_running_task[]{"CommLongRunning"};
constexpr char threadname_executor_task[]{"CommExecuTask"};
}  // namespace ThreadName

DLLEXPORT void SetThreadName(const std::string &th_name);

COM_THREAD_EXTEND_NS_TAIL
}  // namespace communication
}  // namespace hobot

#endif
