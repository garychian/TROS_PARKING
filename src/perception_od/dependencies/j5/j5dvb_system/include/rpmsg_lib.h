/*
 *			 COPYRIGHT NOTICE
 *		 Copyright 2020 Horizon Robotics, Inc.
 *			 All rights reserved.
 */

#ifndef RPMSG_LIB_H_
#define RPMSG_LIB_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* transfer features */
#define RPMSG_F_BLOCK      (1 << 0)
#define RPMSG_F_NONBLOCK   (0)
#define RPMSG_F_CRC_CHECK  (1 << 1)

/* error code */
#define RPMSG_ERR_INVALID_ARG           (-1)
#define RPMSG_ERR_INVALID_SERVER        (-2)
#define RPMSG_ERR_OUT_OF_RES            (-3)
#define RPMSG_ERR_KER_USR_TRANS         (-4)
#define RPMSG_ERR_SEND_BUF_OVERSIZE     (-5)
#define RPMSG_ERR_NO_MEM                (-6)
#define RPMSG_ERR_TIMEOUT               (-7)
#define RPMSG_ERR_SIGNAL_STOP           (-8)
#define RPMSG_ERR_RECV_BUF_OVERFLOW     (-9)
#define RPMSG_ERR_NOT_START_SERVER      (-10)
#define RPMSG_ERR_CRC_CHECK             (-11)

struct rpmsg_handle;

/*
* hb_rpmsg_connect_server - establish connection with communication service
* @server_name: communication service name
* @flags: transfer features, such as block or nonblock
* @timeout: timeout in block transfer mode in microseconds
* @handle: out parameter, return communication handle
* return: on success return 0; if fail return negative error code
*/
int32_t hb_rpmsg_connect_server(const char *server_name, int32_t flags, int32_t timeout,
struct rpmsg_handle **handle);

/*
* hb_rpmsg_disconnect_server - abolish connection with communication service specified by handle
* @handle: communicatin handle want to disconnect
* return: on success return 0; if fail return negative error code
*/
int32_t hb_rpmsg_disconnect_server(struct rpmsg_handle *handle);

/*
* hb_rpmsg_send - send frame on specific communication service
* @handle: communication handle want to send
* @buf: contain frame data want to send
* @len: data length want to send
* return: on success return the number of bytes sent; if fail return negative error code
*/
int32_t hb_rpmsg_send(const struct rpmsg_handle *handle, const char *buf, int32_t len);

/*
* hb_rpmsg_recv - receive frame on specific communication service
* @handle: communication handle want to send
* @buf: contain received data
* @len: received data length
* return: on success return the number of bytes received; if fail return negative error code
*/
int32_t hb_rpmsg_recv(const struct rpmsg_handle *handle, char *buf, int32_t len);

/*
* hb_rpmsg_error_message - return string type error message according to error_code
* @error_code: error code return from rpmsg lib API
* return: string type error message
*/
const char *hb_rpmsg_error_message(int32_t error_code);

#ifdef __cplusplus
}
#endif

#endif /*RPMSG_LIB_H_*/

