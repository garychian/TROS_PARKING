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

#ifndef COMMUNICATION_COMMON_TYPES_H_
#define COMMUNICATION_COMMON_TYPES_H_

#include <map>
#include <string>
#include <vector>
#include "communication/core_types.h"
#include "communication/plugin_ctrl.h"
#include "thread_types.h"

#ifdef _WIN32
#define NOMINMAX
#include "winsock2.h"
#else
#include <fcntl.h>
#endif

#ifndef DLLEXPORT

#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable : 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

static constexpr char PREFIX_MATCH_WILDCARD{'*'};

static constexpr int DDS_DOMAIN_ID_LOWER_BOUND{0};
static constexpr int DDS_DOMAIN_ID_UPPER_BOUND{232};

enum CrcEnumType : int {
  CRC8 = 0,
  CRC16,
  CRC32,
};

struct E2EConfig {
  bool E2Enable{false};
  bool EnableCrc{false};
  CrcEnumType CrcType{CrcEnumType::CRC16};
  bool EnableDataID{false};
  uint32_t BaseDataID{0};
  uint32_t DataID{0};
  bool EnableCounter{false};
  bool DiscardDisoderData{false};
  uint16_t RollingCounter{0U};
  uint16_t RollingCounterMax{15U};
  bool EnableFrameID{false};
  uint64_t FrameID{0};
  // not support, just in case for future
  bool EnableTimeOut{false};
  int TimeOutInterval{2000};
  bool NeedProtectData{false};
};

struct E2EParams {
  uint8_t Crc8Value{0U};
  uint16_t Crc16Value{0U};
  uint32_t Crc32Value{0U};
  uint32_t DataID{0U};
  uint64_t FrameID{0U};
  uint16_t RollingCounter{0U};
  uint64_t TimeStamp{0U};
};

bool DLLEXPORT IsE2EHeader(const void *data, int size);

bool DLLEXPORT E2EMsgProtect(const spSerData &seri_data,
                  E2EConfig &config,
                  E2EParams &params);

E2EventType DLLEXPORT E2EMsgCheck(const spSerData &seri_data,
                 E2EConfig &config,
                 E2EParams &params);

E2EventType DLLEXPORT E2EParamsCheck(
                 const E2EParams &pre_params,
                 const E2EParams &cur_params,
                 const E2EConfig &config);

void DLLEXPORT E2ErrorMsg(E2EventType event, std::string &err_msg);

struct ServiceLock {
#ifdef _WIN32
  HANDLE file_lock_fd{nullptr};
#else
  struct flock file_lock {
    F_WRLCK, SEEK_SET, 0, 0, 0
  };
  int file_lock_fd{-1};
#endif
};

// default uninitialized
enum CommmunState {
  UNINITIALIZED = 0,
  INITIALIZED_IS_ALREADY,
  INITIALIZED_TOOL
};


struct SupportedProtocolEntry {
  int enable;
  int protocol_type;
  std::string protocol_name;
  std::string plugin_file;
  int as_hybrid_s2s_protocol;
  SupportedProtocolEntry()
      : enable{0},
        protocol_type{0},
        protocol_name{""},
        plugin_file{""},
        as_hybrid_s2s_protocol{0} {}
};

struct ProtocolToSubidlMap {
  int protocol_type;
  std::string sub_idl_type;
  ProtocolToSubidlMap() : protocol_type{0}, sub_idl_type{""} {}
};

class DLLEXPORT TopicAssemblier {
 public:
  static std::string AssembleTopic(const std::string &topic, int domain_id,
                                   ProtocolType type);
  static int DisassembleTopic(std::string in_topic, std::string *topic,
                              int *domain_id);
  static void ExpandTopic(const std::string &topic, std::string &request_topic,
                          std::string &response_topic, ProtocolType type);
};

class DLLEXPORT TopicChecker {
 public:
  static bool IsValidPubTopic(const std::string &topic);
  static bool IsValidSubTopic(const std::string &topic);
  static bool IsValidDDSTopic(const std::string &topic, ProtocolType type);
};

class DLLEXPORT CompositeOptionChecker {
 public:
  static bool CheckProtocolOnly(
      ProtocolType type, const std::vector<CompositeOption> &composite_options);
  static bool HasStaticParticipant(
      const std::vector<CompositeOption> &composite_options);
  static bool HasProtocol(
      ProtocolType type, const std::vector<CompositeOption> &composite_options);
  static bool IsValid(const std::vector<CompositeOption> &composite_options);

  static bool HasCrossNetDynamicProtocol(
      const std::vector<CompositeOption> &composite_options);
};

class DLLEXPORT ProtocolChecker {
 public:
  static bool IsHybrid(ProtocolType type);
  static bool IsHybridAndCrossNet(ProtocolType type);
  static bool IsDynamic(ProtocolType type);
  static bool IsStatic(ProtocolType type);
  static bool IsCrossNet(ProtocolType type);
  static bool IsIntra(ProtocolType type);
  static bool IsNeedE2E(ProtocolType type);
  static bool IsBuildIn(ProtocolType type);
  static bool IsComposite(ProtocolType type);
};

class DLLEXPORT UUIDHelper {
 public:
  static uint64_t GenerateUUID();
};

class DLLEXPORT SubRcvGlobalObj {
 public:
  static SubRcvGlobalObj *Instance();

  void RegisterSubHistroyQueue(void *, BaseQueue *);
  void UnRegisterSubHistroyQueue(void *);
  BaseQueue *GetSubHistoryQueue(void *);

  void RegisterSubRcvSwitch(void *, std::atomic_bool *);
  void UnRegisterSubRcvSwitch(void *);
  std::atomic_bool *GetSubRcvSwitch(void *subscriber);

  ~SubRcvGlobalObj() = default;

 private:
  SubRcvGlobalObj() = default;
  std::map<void *, std::atomic_bool *> subscriber_rcv_switch;
  std::mutex subscriber_rcv_switch_mutex;
  std::map<void *, BaseQueue *> subscriber_msg_history;
  std::mutex subscriber_msg_history_mutex;
};

class DLLEXPORT LockService {
 public:
  static LockService *Instance();
  bool GetServiceInitStatus(const std::string &service_name, ProtocolType type);
  void UnLockService(const std::string &service_name, ProtocolType type);
  ~LockService() = default;

 private:
  LockService();
  std::map<std::string, struct ServiceLock> reg_service_info;
  std::mutex reg_service_mutex;
#ifdef WIN32
  std::string com_service_lock_path{"C:/Windows/Temp/communication/"};
#elif ANDROID
  std::string ComServiceLockPath(){return std::string(getenv("COMMUNICATION_CACHE_DIR"));};
#else
  std::string com_service_lock_path{"/tmp/communication/"};
#endif

  std::string FormatFileName(const std::string &str);
};

#define DISALLOW_COPY(TypeName) TypeName(const TypeName &) = delete

#define DISALLOW_ASSIGNMENT(TypeName) \
  TypeName &operator=(const TypeName &) = delete

#define DISALLOW_COPY_AND_ASSIGNMENT(TypeName) \
  DISALLOW_COPY(TypeName);                     \
  DISALLOW_ASSIGNMENT(TypeName)

#define DISALLOW_MOVE(TypeName)   \
  TypeName(TypeName &&) = delete; \
  TypeName &operator=(TypeName &&) = delete

#define DISALLOW_COPY_ASSIGNMENT_AND_MOVE(TypeName) \
  DISALLOW_COPY_AND_ASSIGNMENT(TypeName);           \
  DISALLOW_MOVE(TypeName)

template <typename T>
struct HasBole2DDSFlag {
  template <typename U>
  static void check(decltype(&U::bole2_dds_support_flag));

  template <typename U>
  static int check(...);

  enum { value = std::is_void<decltype(check<T>(0))>::value };
};

template <typename T>
struct HasBole2PSMDDSFlag {
  template <typename U>
  static void check(decltype(&U::bole2_psm_dds_support_flag));

  template <typename U>
  static int check(...);

  enum { value = std::is_void<decltype(check<T>(0))>::value };
};

template <typename T>
struct HasBole2Flag {
  template <typename U>
  static void check(decltype(&U::bole2_flag));

  template <typename U>
  static int check(...);

  enum { value = std::is_void<decltype(check<T>(0))>::value };
};

template <typename T>
struct HasMemberDDSData {
  template <typename U>
  static void check(decltype(&U::dds_data));

  template <typename U>
  static int check(...);

  enum { value = std::is_void<decltype(check<T>(0))>::value };
};

template <typename T>
struct HasMemberSpareId {
  template <typename U>
  static auto Check(int)
      -> decltype(std::declval<U>().spare_id(), std::true_type());
  template <typename U>
  static std::false_type Check(...);
  enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value };
};

template <typename T>
struct HasMemberSeqId {
  template <typename U>
  static auto Check(int)
      -> decltype(std::declval<U>().seq_id(), std::true_type());
  template <typename U>
  static std::false_type Check(...);
  enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value };
};

template <typename T>
struct HasMemberWriterId {
  template <typename U>
  static auto Check(int)
      -> decltype(std::declval<U>().writer_id(), std::true_type());
  template <typename U>
  static std::false_type Check(...);
  enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value };
};

template <typename T>
struct HasMemberMsginfo {
  enum {
    value = HasMemberSpareId<T>::value && HasMemberSeqId<T>::value &&
            HasMemberWriterId<T>::value
  };
};

inline bool IsValidDomainID(int domain_id) {
  return (domain_id >= DDS_DOMAIN_ID_LOWER_BOUND &&
          domain_id <= DDS_DOMAIN_ID_UPPER_BOUND);
}

inline bool CheckDDSSupport(ProtocolType protocol_type, int domain_id) {
  static_cast<void>(domain_id);
  if (protocol_type != PROTOCOL_DDS) {
    return true;
  }
  bool res{false};
#ifdef SUPPORT_DDS
  res = IsValidDomainID(domain_id);
#endif
  return res;
}

DLLEXPORT CommmunState GetCommunState(const std::string &topic_name);

DLLEXPORT bool CheckProtocolType(ProtocolType type);

DLLEXPORT bool IsMatchProtocolAndAttr(ProtocolType type,
                                      const CommAttr &comm_attr, int domain_id);

DLLEXPORT std::string GetDDSConfigFilePath();

DLLEXPORT bool IsRos2Compatible();

DLLEXPORT std::string PrintParticipantExtInfo();

DLLEXPORT bool IsDiscoveryEnable();

DLLEXPORT ErrorCode PreConstructSubCheck(const CommAttr &comm_attr,
                                         const std::string &topic,
                                         int domain_id, ProtocolType type,
                                         bool is_sub_cb_null);

DLLEXPORT ErrorCode PreConstructPubCheck(const CommAttr &comm_attr,
                                         const std::string &topic,
                                         int domain_id, ProtocolType type);

DLLEXPORT ErrorCode PreConstructClientCheck(const CommAttr &comm_attr,
                                            const std::string &topic,
                                            int domain_id, ProtocolType type);
DLLEXPORT ErrorCode PreConstructServiceCheck(const CommAttr &comm_attr,
                                             const std::string &topic,
                                             int domain_id, ProtocolType type,
                                             bool is_service_cb_null);

DLLEXPORT ErrorCode PreConstructActionClientCheck(const CommAttr &comm_attr,
                                                  const std::string &topic,
                                                  int domain_id,
                                                  ProtocolType type);

DLLEXPORT ErrorCode PreConstructActionServiceCheck(const CommAttr &comm_attr,
                                                   const std::string &topic,
                                                   int domain_id,
                                                   ProtocolType type,
                                                   bool is_action_cb_null);

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_COMMON_TYPES_H_
