/**
 * @file hb_spi_errno.h
 * @author binbin.he
 * @brief
 * @version 0.1
 * @date 2021.08.20
 *
 * @copyright Copyright (c) 2021 Horizon
 *
 */
#ifndef SPI_HAL_ERRNO_H
#define SPI_HAL_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif
#define SPI_HAL_E_OK 0                   // General OK
#define SPI_HAL_E_NOK 1                  // General Not OK
#define SPI_HAL_E_CONFIG_FAIL 2          // Config fail
#define SPI_HAL_E_WRONG_CONFIGURATION 3  // wrong configuration
#define SPI_HAL_E_NULL_POINTER 4   // A null pointer was passed as an argument
#define SPI_HAL_E_PARAM_INVALID 5  // A parameter was invalid
#define SPI_HAL_E_LENGTH_TOO_SMALL 6  // length too small
#define SPI_HAL_E_INIT_FAILED 7       // initialization failed
#define SPI_HAL_E_UNINIT 8            // called before initialization
#define SPI_HAL_E_BUFFER_OVERFLOW 9   // Buffer overflow
#define SPI_HAL_E_TIMEOUT 10          // expired the time out
#define SPI_HAL_E_REINIT 11           // Re initilize

#ifdef __cplusplus
}
#endif

#endif
