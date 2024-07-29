/***************************************************************************
* COPYRIGHT NOTICE
* Copyright 2019 Horizon Robotics, Inc.
* All rights reserved.
***************************************************************************/
#ifndef __HB_CAMERA_COMMON_H__
#define __HB_CAMERA_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
#include "hb_vin_interface.h"
#include "vin_log.h"

#define CAM_MAX_NUM	16u
#define GPIO_NUMBER 6
#define SERDES_NUMBER 4
#define LPWM_NUMBER 4
#define MAX_NUM_LENGTH 128
#define PWM_PIN_NUM  4
#define DES_OUT_NUM_MAX 2
#define MAX_FPS  30u

#define RET_OK 0
#define	RET_ERROR 1

#define SENSOR_IOC_MAGIC   'x'
#define SENSOR_OPEN_CNT        _IOR(SENSOR_IOC_MAGIC, 1, int32_t)
#define SENSOR_SET_START_CNT   _IOW(SENSOR_IOC_MAGIC, 2, int32_t)
#define SENSOR_GET_START_CNT   _IOR(SENSOR_IOC_MAGIC, 3, int32_t)
#define SENSOR_USER_LOCK       _IOW(SENSOR_IOC_MAGIC, 4, int32_t)
#define SENSOR_USER_UNLOCK     _IOW(SENSOR_IOC_MAGIC, 5, int32_t)
#define SENSOR_AE_SHARE	       _IOW(SENSOR_IOC_MAGIC, 6, int32_t)

/* for interface type */
#define INTERFACE_MIPI	"mipi"
#define INTERFACE_BT	"bt"
#define INTERFACE_DVP	"dvp"
#define INTERFACE_SDIO	"sdio"
#define INTERFACE_NET	"net"

/* format for v0.1 */
#define EEPROM_I2C_ADDR_ALIAS_ID (0x51)
#define DEFAULT_EEPROM_I2C_ADDR  (0x50)
#define MAJOR_VERSION_ADDR      (0x0000)
#define MINOR_VERSION_ADDR      (0x0001)
#define VENDOR_ID_ADDR          (0x0004)
#define MODULE_ID_ADDR          (0x0006)
#define MODULE_SERIAL_ADDR      (0x0008)
#define YEAR_ADDR               (0x000C)
#define MONTH_ADDR              (0x000E)
#define DAY_ADDR                (0x000F)
#define CAM_TYPE_ADDR           (0x0010)
#define MODULE_FLAG_ADDR        (0x0016)
#define EFL_FLAG_ADDR           (0x0017)
#define COD_FLAG_ADDR           (0x0018)
#define PP_FLAG_ADDR            (0x0019)
#define DISTORTION_FLAG_ADDR    (0x001A)
#define IMG_HEIGHT_ADDR         (0x001C)
#define IMG_WIDTH_ADDR          (0x001E)
#define FOV_ADDR                (0x0020)
#define CRC32_1_ADDR            (0x003C)
#define EFL_X_ADDR              (0x0040)
#define EFL_Y_ADDR              (0x0048)
#define COD_X_ADDR              (0x0050)
#define COD_Y_ADDR              (0x0058)
#define PP_X_ADDR               (0x0080)
#define PP_Y_ADDR               (0x0088)
#define DISTORT_PARAMS_ADDR     (0x0090)
#define DISTORT_MODEL_TYPE_ADDR (0x0091)
#define CAM_SKEW_ADDR           (0x0098)
#define K1_ADDR                 (0x00C0)
#define K2_ADDR                 (0x00C8)
#define P1_ADDR                 (0x00D0)
#define P2_ADDR                 (0x00D8)
#define K3_ADDR                 (0x00E0)
#define K4_ADDR                 (0x00E8)
#define K5_ADDR                 (0x00F0)
#define K6_ADDR                 (0x00F8)
#define K7_ADDR                 (0x0100)
#define K8_ADDR                 (0x0108)
#define K9_ADDR                 (0x0110)
#define K10_ADDR                (0x0118)
#define K11_ADDR                (0x0120)
#define K12_ADDR                (0x0128)
#define K13_ADDR                (0x0130)
#define K14_ADDR                (0x0138)
#define EFL_X_2_ADDR            (0x0148)
#define EFL_Y_2_ADDR            (0x0150)
#define COD_X_2_ADDR            (0x0158)
#define COD_Y_2_ADDR            (0x0160)
#define K1_2_ADDR               (0x0168)
#define K2_2_ADDR               (0x0170)
#define K3_2_ADDR               (0x0178)
#define K4_2_ADDR               (0x0180)
#define SERIAL_NUM_ADDR         (0x0188)
#define CRC32_GROUP1_ADDR       (0x01C0)

/* not use now*/
enum GPIO_DEF_VALUE{
	ISP_RESET,
	SENSOR_POWER_ON,
	SENSOR_RESET,
	S954_POWER_ON,
	S954_RESET
};

/* vin init and start status*/
typedef enum cam_state_s {
	CAM_INIT = 1,
	CAM_DEINIT,
	CAM_START,
	CAM_STOP,
	CAM_POWERON,
	CAM_POWEROFF,
	CAM_STATE_INVALID,
} cam_state_e;

typedef struct spi_data {
	int32_t spi_mode;
	int32_t spi_cs;
	uint32_t spi_speed;
}spi_data_t;

typedef struct sensor_info_s {
	uint32_t port;
	uint32_t bus_type;
	uint32_t bus_num;
	uint32_t isp_addr;
	uint32_t sensor_addr;
	uint32_t sensor1_addr;
	uint32_t serial_addr;
	uint32_t serial_addr1;
	uint32_t imu_addr;
	uint32_t sensor_clk;
	uint32_t eeprom_addr;
	uint32_t power_mode;
	uint32_t sensor_mode;
	uint32_t entry_num;
	uint32_t reg_width;
	uint32_t gpio_num;
	int32_t gpio_pin[GPIO_NUMBER];
	int32_t gpio_level[GPIO_NUMBER];
	uint32_t fps;
	uint32_t width;
	uint32_t height;
	uint32_t format;
	uint32_t resolution;
	uint32_t extra_mode;
	uint32_t power_delay;
	int32_t deserial_index;
	int32_t deserial_port;
	char *sensor_name;
	char *config_path;
	char *data_path_info;
	void *sensor_ops;
	void *sensor_fd;
	void *deserial_info;
	uint32_t stream_control;
	uint32_t config_index;
	spi_data_t spi_info;
	cam_state_e init_state;
	cam_state_e start_state;
	int32_t sen_devfd;
	int32_t dev_port;
	int32_t init_cnt;
	int32_t start_cnt;
	uint32_t bus_timeout;
	uint32_t reserved[16];
}sensor_info_t;

typedef struct deserial_info_s {
	uint32_t bus_type;
	uint32_t bus_num;
	uint32_t deserial_addr;
	uint32_t power_mode;
	uint32_t physical_entry;
	uint32_t mfp_index;
	uint32_t gpio_num;
	int32_t gpio_pin[GPIO_NUMBER];
	int32_t gpio_level[GPIO_NUMBER];
	char *deserial_name;
	char *deserial_config_path;
	void *deserial_ops;
	void *deserial_fd;
	void  *sensor_info[CAM_MAX_NUM];
	int32_t init_state;
	pthread_t init_thread_id;
	int32_t thread_created;
	uint32_t bus_timeout;
	uint32_t reserved[16];
}deserial_info_t;

typedef struct lpwm_info_s {
	int32_t fd;
	int32_t ext_timer_en;
	int32_t ext_trig_en;
	int32_t lpwm_index;
	int32_t lpwm_enable;
	int32_t trigger_mode;
	int32_t offset_us[PWM_PIN_NUM];
	int32_t period_us[PWM_PIN_NUM];
	int32_t duty_us[PWM_PIN_NUM];
	int32_t lpwm_start;
}lpwm_info_t;

typedef struct board_info_s {
	uint32_t  config_number;
	char *board_name;
	char *interface_type;
	uint32_t  deserial_num;
	uint32_t  port_number;
	uint32_t  port_mask;
	uint32_t  lpwm_num;
	lpwm_info_t lpwm_info[LPWM_NUMBER];
	deserial_info_t deserial_info[SERDES_NUMBER];
	sensor_info_t sensor_info[CAM_MAX_NUM];
	int32_t board_init_ret;
	uint32_t init_in_seq;
}board_info_t;

#define HAL_LINE_CONTROL       0x00000001
#define HAL_GAIN_CONTROL       0x00000002
#define HAL_AWB_CONTROL        0x00000004
#define HAL_AF_CONTROL     0x00000008
#define HAL_ZOOM_CONTROL   0x00000010
#define HAL_AWB_CCT_CONTROL        0x00000020
#define HAL_AE_LINE_GAIN_CONTROL   0x00000040

typedef struct hal_control_info_s {
	uint32_t port;
	uint32_t bus_type;
	uint32_t bus_num;
	uint32_t sensor_addr;
	uint32_t sensor1_addr;
	uint32_t serial_addr;
	uint32_t serial_addr1;
	uint32_t sensor_mode;
	uint32_t eeprom_addr;
	spi_data_t sensor_spi_info;
	uint32_t af_bus_num;
	uint32_t af_addr;
	uint32_t af_info[4];
	uint32_t zoom_bus_num;
	uint32_t zoom_addr;
	uint32_t zoom_info[4];
} hal_control_info_t;

typedef struct {
	const char *module;
	int32_t (*init)(sensor_info_t *sensor_info);
	int32_t (*deinit)(sensor_info_t *sensor_info);
	int32_t (*start)(sensor_info_t *sensor_info);
	int32_t (*stop)(sensor_info_t *sensor_info);
	int32_t (*power_on)(sensor_info_t *sensor_info);
	int32_t (*power_off)(sensor_info_t *sensor_info);
	int32_t (*power_reset)(sensor_info_t *sensor_info);
	int32_t (*extern_isp_poweron)(sensor_info_t *sensor_info);
	int32_t (*extern_isp_poweroff)(sensor_info_t *sensor_info);
	int32_t (*extern_isp_reset)(sensor_info_t *sensor_info);
	int32_t (*spi_read)(sensor_info_t *sensor_info,  uint32_t reg_addr, char *buffer, uint32_t sizee);
	int32_t (*spi_write)(sensor_info_t *sensor_info, uint32_t reg_addr, char *buffer, uint32_t sizee);
	int32_t (*set_awb)(int32_t i2c_bus, int32_t sensor_addr, float rg_gain, float b_gain);
	int32_t (*set_ex_gain)( int32_t i2c_bus, int32_t sensor_addr, uint32_t exposure_setting,
			uint32_t gain_setting_0, uint16_t gain_setting_1);
	int32_t (*dynamic_switch_fps)(sensor_info_t *sensor_info, uint32_t fps);
	int32_t (*ae_share_init)(uint32_t flag);
	int32_t (*get_vts)(sensor_info_t *sensor_info, uint32_t *vts);
	int32_t (*get_hts)(sensor_info_t *sensor_info, uint32_t *hts);
	int32_t (*set_vts)(sensor_info_t *sensor_info, uint32_t *vts);
	int32_t (*set_hts)(sensor_info_t *sensor_info, uint32_t *hts);
	int32_t (*get_sns_params)(sensor_info_t *sensor_info, cam_parameter_t *sp, uint8_t type);
	int32_t (*aexp_gain_control)(hal_control_info_t *info, uint32_t mode,
		uint32_t *again, uint32_t *dgain, uint32_t gain_num);
	int32_t (*aexp_line_control)(hal_control_info_t *info, uint32_t mode, uint32_t *line, uint32_t line_num);
	int32_t (*aexp_line_gain_control)(hal_control_info_t *info, uint32_t mode, uint32_t *line,
		    uint32_t line_num, uint32_t *again, uint32_t *dgain, uint32_t gain_num);
	int32_t (*awb_control)(hal_control_info_t *info, uint32_t mode, uint32_t rgain,
		    uint32_t bgain, uint32_t grgain, uint32_t gbgain);
	int32_t (*awb_cct_control)(hal_control_info_t *info, uint32_t mode, uint32_t rgain,
		    uint32_t bgain, uint32_t grgain, uint32_t gbgain, uint32_t temper);
	int32_t (*af_control)(hal_control_info_t *info, uint32_t mode, uint32_t pos);
	int32_t (*zoom_control)(hal_control_info_t *info, uint32_t mode, uint32_t pos);
	int32_t (*userspace_control)(uint32_t port, uint32_t *enable);
	int32_t (*stream_off)(sensor_info_t *sensor_info);
	int32_t (*stream_on)(sensor_info_t *sensor_info);
	int32_t (*parse_embed_data)(sensor_info_t *sensor_info, char* embed_raw, embed_data_info_t* embed_info);
}sensor_module_t;

typedef struct {
	const char *module;
	int32_t (*init)(deserial_info_t *deserial_info);
	int32_t (*stream_on)(deserial_info_t *deserial_info, uint32_t port);
	int32_t (*stream_off)(deserial_info_t *deserial_info, uint32_t port);
	int32_t (*deinit)(deserial_info_t *deserial_info);
	int32_t (*start_physical)(const deserial_info_t *deserial_info);
	int32_t (*reset)(const deserial_info_t *deserial_info);
}deserial_module_t;

extern int32_t hb_cam_htoi(char s[]);

#ifdef __cplusplus
}
#endif

#endif


