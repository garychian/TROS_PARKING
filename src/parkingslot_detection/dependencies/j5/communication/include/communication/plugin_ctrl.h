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

#ifndef COMMUNICATION_PLUGIN_CTRL_H_
#define COMMUNICATION_PLUGIN_CTRL_H_

#include <string>
#include <map>
#include "communication/core_types.h"

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

/**
 * @brief Load plugin
 *
 * @param protocol_type
 * @param protocol_name
 * @param plugin_name
 * @param as_hybrid_s2s_protocol
 * @return ProtocolType
 */
DLLEXPORT ErrorCode LoadPlugin(ProtocolType protocol_type,
                                  const std::string &protocol_name,
                                  const std::string &plugin_name,
                                  const int as_hybrid_s2s_protocol);
/**
 * @brief Unload plugin
 *
 * @param protocol_type
 * @param protocol_name
 * @param plugin_name
 * @return ErrorCode :  unload resut is 0/success or -1/failed
 */
DLLEXPORT ErrorCode UnloadPlugin(ProtocolType protocol_type,
                                 const std::string &protocol_name,
                                 const std::string &plugin_name);

/**
 * @brief Update one plugin property
 *
 * @param protocol_type
 * @param protocol_name
 * @param key
 * @param value
 * @return ErrorCode :  Update one property result is 0/success or -1/failed
 */
DLLEXPORT ErrorCode UpdatePluginPropertyExt(ProtocolType protocol_type,
                                            const std::string & key,
                                            const std::string & value);
/**
 * @brief Get all plugin property
 *
 * @param protocol_type
 * @param protocol_name
 * @param prop_kv_map
 * @return ErrorCode :  Get Plugin properties result is 0/success or -1/failed
 */
DLLEXPORT ErrorCode GetPluginPropertyExt(ProtocolType protocol_type,
                    std::map<std::string, std::string> & prop_kv_map);

}  // namespace communication
}  // namespace hobot

#endif  // COMMUNICATION_PLUGIN_CTRL_H_
