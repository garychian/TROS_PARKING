// Copyright (c) [2021-2023] [Horizon Robotics].
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

#include "location_map/CanTransModule.h"

#include <functional>
#include <vector>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>

#include "communication/common/types.h"
#include "message/proto/proto_serializer.hpp"

#include "dataflow/module/port.h"
#include "dataflow/module_loader/register_module_macro.h"


namespace fanya {
namespace parking {
using hobot::vehicleio::SignalArray;
namespace{
  static int32_t GetCanSignal(const spVehicleio &vehicleio,
  const std::shared_ptr<CanSignalMsg> &canmsg) {
  // int32_t timeout_ms{3};
  int32_t ret{-1};
  hobot::vehicleio::SignalArray signal_array;

  //0514
  std::vector<hobot::vehicleio::SignalInfo> can_data;
  //0514

  if (vehicleio == nullptr) {
    LOGE("Failed to init Vehicleio client");
    return -1;
  } else {
    ret = vehicleio->GetSignals(signal_array);
  }
  if (ret != 0) {
    LOGE("GetSignals No data now");
    return ret;
  }

  //0514
  ret = signal_array.GetSignalArray("VehSpdAvgNDrvn", can_data);
  if (ret < 0) {
    DFHLOG_E("No speed data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} speed: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_vehspdavgndrvn(iter.value.f32);
  }

  // ret = signal_array.GetSignalArray(
  //   hobot::vehicleio::kCanRawTypeGear, can_data);
  ret = signal_array.GetSignalArray("TrnsShftLvrPos_TCM", can_data);
  if (ret < 0) {
    DFHLOG_E("No gear data this time");
  }
  for (const auto& iter : can_data) {
    auto gear_type = static_cast<can::TrnsShftLvrPos_TCM>(iter.value.u32);
    DFHLOG_D("mcu_ts:{} soc_ts:{} gear: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, gear_type);
    canmsg->proto.set_trnsshftlvrpos_tcm(gear_type);  
  }

  ret = signal_array.GetSignalArray("StrWhAng", can_data);
  if (ret < 0) {
    DFHLOG_E("No WheelAngle data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} WheelAngle: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_strwhang(iter.value.f32);
  }

  // ret = signal_array.GetSignalArray(
  //   hobot::vehicleio::kCanRawTypeYawRate, can_data);
  ret = signal_array.GetSignalArray("IMUYawRtPri", can_data);
  if (ret < 0) {
    DFHLOG_E("No yawrate data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} yawrate: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_imuyawrtpri(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("IMULatACCSec", can_data);
  if (ret < 0) {
    DFHLOG_E("No IMULatACCSec data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} IMULatACCSec: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_imulataccsec(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("IMUYawRtSec", can_data);
  if (ret < 0) {
    DFHLOG_E("No IMUYawRtSec data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} IMUYawRtSec: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_imuyawrtsec(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("IMULonAccSec", can_data);
  if (ret < 0) {
    DFHLOG_E("No IMULonAccSec data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} IMULonAccSec: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_imulonaccsec(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("IIMULatAccPrim", can_data);
  if (ret < 0) {
    DFHLOG_E("No IIMULatAccPrim data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} IIMULatAccPrim: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_iimulataccprim(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("IMULonAccPri", can_data);
  if (ret < 0) {
    DFHLOG_E("No IMULonAccPri data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} IMULonAccPri: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_imulonaccpri(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlAngVelLFrtAuth", can_data);
  if (ret < 0) {
    DFHLOG_E("No FLWheelSpeedInkph data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} FLWheelSpeedInkph: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whlangvellfrtauth(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlAngVelRFrtAuth", can_data);
  if (ret < 0) {
    DFHLOG_E("No FRwheelSpeedInkph data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} FRwheelSpeedInkph: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whlangvelrfrtauth(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlAngVelLRrAuth", can_data);
  if (ret < 0) {
    DFHLOG_E("No RLwheelSpeedInkph data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} RLwheelSpeedInkph: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whlangvellrrauth(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlAngVelRRrAuth", can_data);
  if (ret < 0) {
    DFHLOG_E("No RRwheelSpeedInkph data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} RRwheelSpeedInkph: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whlangvelrrrauth(iter.value.f32);
  }

  // ret = signal_array.GetSignalArray(
  //   hobot::vehicleio::kCanRawTypeWheelPulseFL, can_data);
  ret = signal_array.GetSignalArray("WhlDistEdgeCntrLFHigFreq", can_data);
  if (ret < 0) {
    DFHLOG_E("No FLWheelSpeedPulse data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} FLWheelSpeedPulse: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whldistedgecntrlfhigfreq(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlDistEdgeCntrRFHigFreq", can_data);
  if (ret < 0) {
    DFHLOG_E("No FRwheelSpeedPulse data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} FRwheelSpeedPulse: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whldistedgecntrrfhigfreq(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlDistEdgeCntrLRHigFreq", can_data);
  if (ret < 0) {
    DFHLOG_E("No RLwheelSpeedPulse data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} RLwheelSpeedPulse: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whldistedgecntrlrhigfreq(iter.value.f32);
  }

  ret = signal_array.GetSignalArray("WhlDistEdgeCntrRRHigFreq", can_data);
  if (ret < 0) {
    DFHLOG_E("No RRwheelSpeedPulse data this time");
  }
  for (const auto& iter : can_data) {
    DFHLOG_D("mcu_ts:{} soc_ts:{} RRwheelSpeedPulse: {}",
      iter.ts.mcu_ts, iter.ts.soc_ts, iter.value.f32);
    canmsg->proto.set_whldistedgecntrrrhigfreq(iter.value.f32);
  }
  
  LOGD("GetSignal end, ready to send");
  return 0;
}
}

CanTransModule::CanTransModule(const ModuleOption& module_option):
  hobot::dataflow::Module(module_option) {
}

void CanTransModule::InitPortsAndProcs() {
  LOGD("InitPortsAndProcs in CanTransModule [START]");
  DF_MODULE_INIT_CUSTOM_OUTPUT_PORT("can_signal", CanSignalMsg);
  DF_MODULE_REGISTER_HANDLE_MSGS_PROC(
    "GetSignalProc", CanTransModule, GetSignalProc,
    hobot::dataflow::ProcType::DF_MSG_TIMER_PROC,
    DF_VECTOR(),
    DF_VECTOR("can_signal"));
  LOGD("InitPortsAndProcs in CanTransModule [END]");
}

int32_t CanTransModule::Init() {
  LOGD("Init() in CanTransModule [START]");
  vehicleio_ = std::make_shared<hobot::vehicleio::Vehicleio>();
  if (vehicleio_ == nullptr) {
    LOGE("Failed to init Vehicleio client");
    return -1;
  }

  std::string vehicleio_config = "./config/location_map_process/veh_client.json";
  //auto vehicleio_config = module_option_.config_file;
  if (access(vehicleio_config.c_str(), F_OK)) {
    LOGE("Caution: config file not exist, please check path = [{}]",
      vehicleio_config);
    return -1;
  }
  if (0 != vehicleio_->Init(vehicleio_config)) {
    LOGE("Caution: init failed please check config file");
    return -1;
  }

  LOGI("Sub version_______________________ = [{}]", vehicleio_->GetVersion());
  LOGI("The currunt version should be greater than or equal to 1.4.1");
  LOGD("Init() in CanTransModule [END]");
  return 0;
}

int32_t CanTransModule::Start() {
  LOGI("Start() in CanTransModule");
  return hobot::dataflow::Module::Start();
}

int32_t CanTransModule::Stop() {
  LOGI("Stop() in CanTransModule");
  return hobot::dataflow::Module::Stop();
}

void CanTransModule::Reset() {
  LOGI("Reset() in CanTransModule");
  hobot::dataflow::Module::Reset();
}

int32_t CanTransModule::DeInit() {
  LOGI("DeInit() in CanTransModule");
  return hobot::dataflow::Module::DeInit();
}

void CanTransModule::GetSignalProc(
  hobot::dataflow::spMsgResourceProc proc,
  const hobot::dataflow::MessageLists& msgs) {
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  LOGT("GetSignalProc START {}.{}",
    now.time_since_epoch().count() / 1000000000,
    now.time_since_epoch().count() % 1000000000);

  std::shared_ptr<CanSignalMsg> caniomsg = std::make_shared<CanSignalMsg>();
  if (caniomsg == nullptr) {
    LOGE("Failed to init Vehicleio client");
    return;
  }

  uint32_t ret = GetCanSignal(vehicleio_ , caniomsg);
  if (ret) {
    LOGW("Error : GetCanSignal Failed");
    return;
  } else {
    auto output_port = GetOutputPort("can_signal");
    output_port->Send(caniomsg);
    LOGI("Transport CanSignal Down");
  }
  LOGD("CanTransModule : GetSignalProc [END]");
}



DATAFLOW_REGISTER_MODULE(CanTransModule)


}  // namespace samples
}  // namespace hobot

