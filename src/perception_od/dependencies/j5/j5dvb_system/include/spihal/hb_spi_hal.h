/**
 * @file hb_spi_hal.h
 * @author binbin.he
 * @brief
 * @version 0.1
 * @date 2021.08.10
 *
 * @copyright Copyright (c) 2021 Horizon
 *
 */

#ifndef HORIZON_SPI_HAL_H
#define HORIZON_SPI_HAL_H
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define SPIHAL_GROUP_SERVICE_MAX_NUM (64u)
#define SPIHAL_GROUP_ID_UNUSED (1u)
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/
struct SPIHAL_Group {
  int id = -SPIHAL_GROUP_ID_UNUSED;
  char name[32] = {0};
  int spi_dev = 1;
  int cache_size;
  int srv_num;
  struct SrvType_st {
    uint16_t type;
    uint16_t sub;
  } srv_type[SPIHAL_GROUP_SERVICE_MAX_NUM];
};
/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
/**
 * @brief Get Hal version.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_GetVersion(void);

/**
 * @brief Handshake initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_HandShake_Init(void);

/**
 * @brief Receive Handshake data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_HandShake_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send Handshake data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_HandShake_Send(uint8_t *data, int16_t length);

/**
 * @brief Handshake de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_HandShake_Deinit(void);

/**
 * @brief Time sync initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_TimeSync_Init(void);

/**
 * @brief Receive Time sync data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_TimeSync_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Time sync de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_TimeSync_Deinit(void);

/**
 * @brief System Manager initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Init(void);

/**
 * @brief Receive System Manager API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send System Manager QA API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Send_QA(uint8_t *data, int16_t length);

/**
 * @brief Send System Manager soc state API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Send_SocState(uint8_t *data, int16_t length);

/**
 * @brief Send System Manager soc info API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Send_SocInfo(uint8_t *data, int16_t length);

/**
 * @brief Send System Manager calibration result API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Send_CaliResult(uint8_t *data, int16_t length);

/**
 * @brief Send System Manager calibration param value API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Send_CaliValue(uint8_t *data, int16_t length);

/**
 * @brief System Manager de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_SysMgr_Deinit(void);

/**
 * @brief CAN percept initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_CANPercept_Init(void);

/**
 * @brief Receive CAN ODO data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_CANPercept_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send CAN perception data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_CANPercept_Send(uint8_t *data, int16_t length);

/**
 * @brief Send CAN filter API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_CANPercept_Send_Filter(uint8_t *data, int16_t length);

/**
 * @brief CAN percept de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_CANPercept_Deinit(void);

/**
 * @brief CAN extra initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_CANExtra_Init(void);

/**
 * @brief Receive CAN extra data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_CANExtra_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send CAN extra data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_CANExtra_Send(uint8_t *data, int16_t length);

/**
 * @brief Send CAN extra data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_CANExtra_Send_Filter(uint8_t *data, int16_t length);

/**
 * @brief CAN extra de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_CANExtra_Deinit(void);

/**
 * @brief Diagnosis initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_Diag_Init(void);

/**
 * @brief Receive Diagnosis data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_Diag_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send Diagnosis data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_Diag_Send(uint8_t *data, int16_t length);

/**
 * @brief Diagnosis de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_Diag_Deinit(void);

/**
 * @brief Mcu utity initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_McuUitity_Init(void);

/**
 * @brief Receive Mcu utity data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_McuUitity_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send Mcu utity data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_McuUitity_Send(uint8_t *data, int16_t length);

/**
 * @brief Mcu utity de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_McuUitity_Deinit(void);

/**
 * @brief Mcu utity initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_SpiTest_Init(void);

/**
 * @brief Receive Mcu utity data API.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_SpiTest_Recv(uint8_t *data, int16_t length, int32_t timeout);

/**
 * @brief Send Mcu utity data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_SpiTest_Send(uint8_t *data, int16_t length);

/**
 * @brief Mcu utity de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_SpiTest_Deinit(void);

/**
 * @brief OTA initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_OTA_Init(void);

/**
 * @brief Send OTA Reboot output data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_OTA_Send_Reboot(uint8_t *data, int16_t length);

/**
 * @brief Send OTA Boot Select Reboot output data API.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_Set_BootSel_Reboot(uint8_t *data, int16_t length);

/**
 * @brief OTA de-initialize API.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_OTA_Deinit(void);

/**
 * @brief SPIHAL init.
 * @param[in] group: group.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_Group_Init(SPIHAL_Group *group);

/**
 * @brief SPIHAL Group get config info.
 * @param[in] gname: group name.
 * @param[in] group: get group info.
 * @param[in] json_file: config info.
 * @return return >=0 on success, nagative value on failure.
 */
int HorizonHal_Group_GetGroupbyJson(
    const char *gname, SPIHAL_Group *group,
    const char *json_file);

/**
 * @brief SPIHAL Group config.
 * @param[in] group: group info.
 * @return return >=0 on success, nagative value on failure.
 */
int HorizonHal_Group_Config(SPIHAL_Group *group);

/**
 * @brief SPIHAL recv.
 * @param[in] length: max length of data.
 * @param[in] timeout: 0 nonblock, >0 block timout in ms, -1 block infinite.
 * @param[in] group: group
 * @param[in] en_parser: Whether to enable the old parser function,
 * If you need to use a universal interface, en_parser is 0.
 * @param[out] data: received one data.
 * @return data length on success, nagative value on failure.
 */
int HorizonHal_Group_Recv(uint8_t *data, int16_t length, int32_t timeout,
                                 SPIHAL_Group *group, uint8_t en_parser = 0);
/**
 * @brief SPIHAL send.
 * @param[in] data: send data.
 * @param[in] length: length of data.
 * @param[in] group: group.
 * @return send length on success, nagative value on failure.
 */
int HorizonHal_Group_Send(const uint8_t *data, int16_t length,
                                 SPIHAL_Group *group);
/**
 * @brief SPIHAL deinit.
 * @param[in] group: group.
 * @return return 0 on success, nagative value on failure.
 */
int HorizonHal_Group_Deinit(SPIHAL_Group *group);

#ifdef __cplusplus
}
#endif

#endif  // HORIZON_SPI_HAL_H
