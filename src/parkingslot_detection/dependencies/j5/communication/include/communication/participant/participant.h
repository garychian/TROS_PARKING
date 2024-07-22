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

// This is participant definition.

#ifndef COMMUNICATION_PARTICIPANT_PARTICIPANT_H_
#define COMMUNICATION_PARTICIPANT_PARTICIPANT_H_

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <mutex>

#include "communication/common/types.h"
#include "communication/participant/dispatcher.h"

#ifndef DLLEXPORT
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#else
#define DLLEXPORT
#endif
#endif

namespace hobot {
namespace communication {

enum class PartIdType : uint8_t {
  CLIENT_ATTR_EXT_ID_BASE = 0,
  SERVICE_ATTR_EXT_ID_BASE,
  AUTO_DISCOVERY_WRITER_PARTI_BEGIN,
  AUTO_DISCOVERY_WRITER_PARTI_END,
  SHM_PARTICIPANT_ID,
  DDS_PARTICIPANT_ID,
  INTRA_PARTICIPANT_ID,
  AUTO_DISCOVERY_READER_PARTI_BEGIN,
  AUTO_DISCOVERY_READER_PARTI_END
};

struct DLLEXPORT ParticipantInfo {
  ProtocolType type_{PROTOCOL_INVALID};
  int64_t process_id_{0};  // for interprocess
  int id_{0};
  std::string protocol{};
  std::string link_info{};
  std::string link_type{};
  int priority{-1};
  void * config_options{nullptr};
  std::string config_options_str{};
  void * qos_config{nullptr};
  QosParameterConfig hb_qos_param{};
  std::string e2e_options_str{};

  struct shm {
    std::size_t shm_size{0U};
    std::size_t block_size{0U};
  };

  struct shm shm_ {};

  ParticipantInfo() = default;
  explicit ParticipantInfo(ProtocolType type) { type_ = type; }
};

enum class PARTICIPANT_TYPE : uint8_t { READER = 0, WRITER };

class DataWriter;
class DataReader;
class Dispatcher;

class DLLEXPORT Participant {
 public:
  friend class ParticipantManager;
  explicit Participant(const ParticipantInfo &info);
  DataWriter *GetDataWriter();
  DataReader *GetDataReader();
  Dispatcher *GetDispatcher();
  void AddWriterEventListener(ConnEventCallback &cb, void* key);
  void AddReaderEventListener(ConnEventCallback &cb, void* key);

  void RemoveReaderEventListener(void* key);
  void RemoveWriterEventListener(void* key);

  void AddListenTopicToReader(const std::string &topic);
  ProtocolType GetProtocolType() const;

  virtual ~Participant();

  int GetParticipantID();

  bool IsProetctE2Eable();
  bool IsCheckE2Eable();
  void AddE2EProtectDataID(const std::string &topic,
                        const uint32_t &data_id);
  void AddE2EventCallback(const std::string &topic,
                        E2EventCallback &cb);

  bool E2EMsgProtectByTopic(const spSerData &seri_data,
                  std::string &topic);
  E2EventType ReaderE2ECheck(const spSerData &seri_data,
                            const std::string &topic,
                            const E2EConfig &config);

  bool IsWildcardTopic(const std::string &);
  bool IsPrefixStr(const std::string &main,
                   const std::string &prefix);

 private:
  int CreateFunc(PARTICIPANT_TYPE);
  virtual int CreateDataWriter();
  virtual int CreateDataReader();
  virtual int CreateDispatcher();

 protected:
  ParticipantInfo participant_info_;
  bool writer_created_flag_;
  bool reader_created_flag_;
  bool dispatcher_created_flag_;
  std::mutex event_mutex_;
  std::map<void*, std::shared_ptr<ConnEventCallback>> event_listeners_;

  std::shared_ptr<DataWriter> data_writer_{nullptr};
  std::shared_ptr<DataReader> data_reader_{nullptr};
  std::shared_ptr<Dispatcher> dispatcher_{nullptr};

  bool enable_writer_e2e_protect_{false};
  bool enable_reader_e2e_check_{false};
  E2EConfig writer_e2e_configs_;
  E2EConfig reader_e2e_configs_;
  std::mutex e2e_protect_mtx_;
  std::mutex e2e_check_mtx_;
  std::map<std::string, E2EParams> e2e_protect_params_;
  std::map<uint32_t, E2EParams> e2e_check_params_;
  std::mutex e2e_cb_mtx_;
  std::map<std::string, E2EventCallback> e2e_event_cbs_;
};

class IntroParticipant : public Participant {
 public:
  friend class ParticipantManager;
  using Participant::Participant;

 private:
  int CreateDataWriter() override;
  int CreateDataReader() override;
  int CreateDispatcher() override;
};

class DLLEXPORT ParticipantManager {
 public:
  // first is participant instance, second is use-count
  using ParticipantPairType = std::pair<std::shared_ptr<Participant>, int>;
  static ParticipantManager *Instance();
  void CleanUp();

  std::shared_ptr<Participant> GetParticipant(int participant_id,
                                              PARTICIPANT_TYPE type,
                                              int domain_id = 0);
  void RemoveParticipant(int participant_id);
  std::shared_ptr<Participant> GetInproParticipant(PARTICIPANT_TYPE type);

  ~ParticipantManager() = default;
  int GetParticipantGenId(PartIdType type);

 private:
  ParticipantManager();
  std::shared_ptr<Participant> inpro_participant_;
  // map key is participant id, val is participant pair
  std::map<int, ParticipantPairType> participants_;
  std::mutex mtx_;
  bool stop_{false};
};

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PARTICIPANT_PARTICIPANT_H_
