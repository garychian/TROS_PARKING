/*
 * Horizon Robotics J5 SoC PCIe drivers
 *
 * Copyright (C) 2022 Horizon Robotics Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef INCLUDE_UAPI_LINUX_PCIE_HOBOT_H_
#define INCLUDE_UAPI_LINUX_PCIE_HOBOT_H_

#include <linux/ioctl.h>


#define HOBOT_PCIE_EP_VID 0x16c3
#define HOBOT_PCIE_EP_DID 0x1103
#define HOBOT_PCIE_EP_REV 0x11

#define HOBOT_EP_DRV_NAME "hobot_ep"
#define HOBOT_RC_DRV_NAME "hobot_rc"
#define HOBOT_MAX_DEV_NAME_LEN 32
#define HOBOT_MAX_DEV_NUM 32

#define DMA_MAX_CHANNEL 4
#define DMA_MIN_XFER_LEN ((uint32_t)2)
#define DMA_MIN_WEIGHT 1
#define DMA_MAX_WEIGHT 31
#define HOBOT_MAX_DMA_ELEMENTS 8
#define HOBOT_MAX_MSI_NUM ((uint32_t)16)
#define HOBOT_MAX_USER_IRQ_NUM 32 /* user interrupt */
#define HOBOT_MAX_PCI_MAP_NUM 6 /* outbound limited */
#define BAR_MAX_NUM 6
#define DMA_EP_CLEAR_INT_TIMEOUT 200u		/* ms, RC wait EP clear the interrupt status */
#define DMA_XFER_MIN_TIMEOUT 400u /* ms */
#define DMA_XFER_MAX_TIMEOUT 20000u /* ms */
#define DMA_HANDSHAKE_TIMEOUT 2000u /* ms */
#define MSI_HANDSHAKE_TIMEOUT 2000u /* ms */
#define DMA_RECV_FIFO_SIZE		32
#define MSI_RECV_FIFO_SIZE		32
#define DMAC_RESET_DELAY 100 /* us */
#define PCIE_READ_POLL_TIMEOUT 10 /* ms */
#define DMA_TEST_BUFF_SIZE	0x8000 /* 32KB */

/* ioctl command */
#define HOBOT_PCIE_IOC_MAGIC 0x44
#define HOBOT_PCIE_IOC_INIT					_IO(HOBOT_PCIE_IOC_MAGIC, 0) /* init */
#define HOBOT_PCIE_IOC_DEINIT				_IO(HOBOT_PCIE_IOC_MAGIC, 1) /* deinit*/
#define HOBOT_PCIE_IOC_DMA_WRITE			_IO(HOBOT_PCIE_IOC_MAGIC, 2) /* DMA write */
#define HOBOT_PCIE_IOC_DMA_READ				_IO(HOBOT_PCIE_IOC_MAGIC, 3) /* DMA read */
#define HOBOT_PCIE_IOC_PRE_CHECK			_IO(HOBOT_PCIE_IOC_MAGIC, 4) /* BAR RW check */
#define HOBOT_PCIE_IOC_INSTALL_RECEIVER		_IO(HOBOT_PCIE_IOC_MAGIC, 5) /* install DMA receiver */
#define HOBOT_PCIE_IOC_UNINSTALL_RECEIVER	_IO(HOBOT_PCIE_IOC_MAGIC, 6) /* uninstall DMA receiver */
#define HOBOT_PCIE_IOC_TRIGGER_IRQ			_IO(HOBOT_PCIE_IOC_MAGIC, 7) /* Trigger IRQ */
#define HOBOT_PCIE_IOC_INSTALL_IRQ			_IO(HOBOT_PCIE_IOC_MAGIC, 8) /* install IRQ handler */
#define HOBOT_PCIE_IOC_UNINSTALL_IRQ		_IO(HOBOT_PCIE_IOC_MAGIC, 9) /* uninstall IRQ handler */
#define HOBOT_PCIE_IOC_GET_BAR_INFO			_IO(HOBOT_PCIE_IOC_MAGIC, 10) /* get BAR info */
#define HOBOT_PCIE_IOC_MAP_PCI_MEM			_IO(HOBOT_PCIE_IOC_MAGIC, 11) /* map PCI memory */
#define HOBOT_PCIE_IOC_UNMAP_PCI_MEM		_IO(HOBOT_PCIE_IOC_MAGIC, 12) /* unmap PCI memory */
#define HOBOT_PCIE_IOC_CHAN_REQUEST			_IO(HOBOT_PCIE_IOC_MAGIC, 13) /* request DMA channel */
#define HOBOT_PCIE_IOC_CHAN_RELEASE			_IO(HOBOT_PCIE_IOC_MAGIC, 14) /* release DMA channel */
#define HOBOT_PCIE_IOC_REMOTE_RECV_EXIST	_IO(HOBOT_PCIE_IOC_MAGIC, 15) /* check whether remote receiver exist */
#define HOBOT_PCIE_IOC_BAR_READ_VIA_DMA		_IO(HOBOT_PCIE_IOC_MAGIC, 16) /* read BAR via DMA */
#define HOBOT_PCIE_IOC_GET_BUS_INFO			_IO(HOBOT_PCIE_IOC_MAGIC, 19) /* get EP bus/device/function id */

/* Error No. */
#define HOBOT_PCIE_ERR_OK							0
#define HOBOT_PCIE_ERR_INTERRUPTTED					1 /* interruptted while waitting lock */
#define HOBOT_PCIE_ERR_INVAILD_IOCTL_CMD			2 /* invalid command */
#define HOBOT_PCIE_ERR_CHAN_ALREADY_ALLOC			3 /* channel already in use */
#define HOBOT_PCIE_ERR_DMA_XFER_TIMEOUT				4 /* DMA transfer timeout */
#define HOBOT_PCIE_ERR_DMA_XFER_ABORTED				5 /* DMA transfer aborted */
#define HOBOT_PCIE_ERR_DMA_HANDSHAKE_TIMEOUT		6 /* DMA transfer handshake timeout */
#define HOBOT_PCIE_ERR_VERSION_MISMATCH				7 /* library and driver version mis-match */
#define HOBOT_PCIE_ERR_LINK_DOWN					8 /* PCIe link not ready */
#define HOBOT_PCIE_ERR_STL_NOT_READY				9 /* FUSA not ready */
#define HOBOT_PCIE_ERR_MEM_COPY						10 /* memory copy fail */
#define HOBOT_PCIE_ERR_RAISE_IRQ					11 /* raise user IRQ fail */
#define HOBOT_PCIE_ERR_HANDLER_EXIST				12 /* handler already installed */
#define HOBOT_PCIE_ERR_HANDLER_NOT_OWN				13 /* handler is not owned by requester */
#define HOBOT_PCIE_ERR_MAP_PCI_NO_PCI_MEM			15 /* no free PCI space */
#define HOBOT_PCIE_ERR_MAP_PCI_NO_OUTBOUND			16 /* no free outbound window */
#define HOBOT_PCIE_ERR_MAP_PCI_ALREADY_MAPPED		17 /* the address of PCI already mapped */
#define HOBOT_PCIE_ERR_UNMAP_PCI_NOT_MAPPED			18 /* the address not mapped yet */
#define HOBOT_PCIE_ERR_UNMAP_PCI_NOT_OWN			19 /* the mapped memory is not owned by requester */
#define HOBOT_PCIE_ERR_NOT_INIT_BY_RC				20 /* device not init by RC */
#define HOBOT_PCIE_ERR_IN_SUSPEND					21 /* remote in suspend */
#define HOBOT_PCIE_ERR_REMOTE_RECV_NOT_EXIST		22 /* remote receiver not installed */
#define HOBOT_PCIE_ERR_RECEIVER_EXIST				23 /* receiver already installed */
#define HOBOT_PCIE_ERR_RECEIVER_NOT_OWN				24 /* receiver is not owned by requester */
#define HOBOT_PCIE_ERR_NOMEM						25 /* out of memory */
#define HOBOT_PCIE_ERR_UIRQ_HANDSHAKE_TIMEOUT		26 /* user IRQ handshake fail */
#define HOBOT_PCIE_ERR_UIRQ_RECEIVE_FIFO_FULL		27 /* user IRQ receive fifo full */
#define HOBOT_PCIE_ERR_DMA_RECEIVE_FIFO_FULL		28 /* DMA receive fifo full */


/* internal use in BAR2, refer struct reserved_bar */
#define RESERVED_BAR_ID 2
#define RESERVED_END_OFFSET	0x4000 /* reserve for internal use, large than struct reserved_bar */
#define RESERVED_BAR_SIZE RESERVED_END_OFFSET

#define MSI_BACKUP_SIZE 8

struct dma_ele_desc {
	uint32_t ctrl;
	uint32_t size;
	uint32_t src_low;
	uint32_t src_high;
	uint32_t dest_low;
	uint32_t dest_high;
};

#define HOBOT_PCIE_DMA_COUNT_ENABLE 0
#define HOBOT_PCIE_DMA_TS_STAT_ENABLE 1

struct dma_timev {
    uint64_t sec;
    uint64_t nsec;
};

enum hobot_dma_ts_id {
	HOBOT_DMA_TS_U_START = 0,
	HOBOT_DMA_TS_K_IOCTL,
	HOBOT_DMA_TS_K_XFER_START,
	HOBOT_DMA_TS_K_XFER_END_LOCAL,
	HOBOT_DMA_TS_K_XFER_END_REMOTE,
	HOBOT_DMA_TS_K_FIFO_IN,
	HOBOT_DMA_TS_K_READ_RET,
	HOBOT_DMA_TS_U_INVOKE_CB,
	HOBOT_DMA_TS_MAX,
};

struct reserved_bar {
	uint32_t ep_bus_id;
	uint32_t ep_dev_id;
	uint32_t ep_fun_id;
	uint32_t dev_status;
	uint32_t stl_status;
	uint32_t uirq_id;
	uint64_t bar_cpu_addrs[BAR_MAX_NUM];
	uint64_t bar_pci_addrs[BAR_MAX_NUM];
	uint32_t ep_dma_buf_size;
	uint32_t rc_dma_buf_size;
	uint64_t ep_dma_buf_phys;
	uint64_t rc_dma_buf_phys;
	uint32_t ep_chan_alloc;
	uint32_t rc_chan_alloc;
	uint32_t ep_recv_installed;
	uint32_t rc_recv_installed;
	uint32_t msi_backup[MSI_BACKUP_SIZE];
	uint32_t write_chan_status[DMA_MAX_CHANNEL];
	uint32_t read_chan_status[DMA_MAX_CHANNEL];
	uint32_t write_chan_interrupts[DMA_MAX_CHANNEL];
	uint32_t read_chan_interrupts[DMA_MAX_CHANNEL];
	struct dma_ele_desc write_chan_descs[DMA_MAX_CHANNEL][HOBOT_MAX_DMA_ELEMENTS + 1];
	struct dma_ele_desc read_chan_descs[DMA_MAX_CHANNEL][HOBOT_MAX_DMA_ELEMENTS + 1];
#if HOBOT_PCIE_DMA_TS_STAT_ENABLE
	uint64_t dma_wr_id[DMA_MAX_CHANNEL];
	uint64_t dma_rd_id[DMA_MAX_CHANNEL];
	struct dma_timev dma_wr_ts[DMA_MAX_CHANNEL][HOBOT_DMA_TS_K_XFER_END_REMOTE];
	struct dma_timev dma_rd_ts[DMA_MAX_CHANNEL][HOBOT_DMA_TS_K_XFER_END_REMOTE];
#endif
};

enum hobot_msi_id {
	HOBOT_MSI_DMA_ALL_CH = 0,
	HOBOT_MSI_DMA_WRITE_CH0,
	HOBOT_MSI_DMA_WRITE_CH1,
	HOBOT_MSI_DMA_WRITE_CH2,
	HOBOT_MSI_DMA_WRITE_CH3,
	HOBOT_MSI_DMA_READ_CH0,
	HOBOT_MSI_DMA_READ_CH1,
	HOBOT_MSI_DMA_READ_CH2,
	HOBOT_MSI_DMA_READ_CH3,
	HOBOT_MSI_USER_IRQ,
};

#define DEV_STATUS_NEED_ENUM		0x1
#define DEV_STATUS_ENUMED			0x2
#define DEV_STATUS_SUSPEND			0x3
#define DEV_STATUS_INVALID			0xffffffffU

#define STL_STATUS_UN_INITED 0u
#define STL_STATUS_RC_READY 1u
#define STL_STATUS_EP_READY 2u
#define STL_STATUS_DISABLE  3u

#define DMA_IGR_TRIGGERD     0u
#define DMA_RC_TRIGGERD      1u
#define DMA_EP_TRIGGERD      2u
#define DMA_EP_TEST_TRIGGERD 3u
#define DMA_HANDSHAKE_OK     4u
#define DMA_RECV_FIFO_FULL   5u

#define UIRQ_HANDSHAKE_OK      0u
#define UIRQ_RECV_FIFO_FULL    (u32)(HOBOT_MAX_USER_IRQ_NUM + 1)

#define LOW_64_MASK 0xffffffffffffffffU
#define LOW_32_MASK 0xffffffffU
#define LOW_16_MASK 0xffffU

#define CHANNEL_0 0
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3

#define BAR_ID_0 0
#define BAR_ID_1 1
#define BAR_ID_2 2
#define BAR_ID_3 3
#define BAR_ID_4 4
#define BAR_ID_5 5

struct dma_element {
	uint32_t size; /* size */
	uint64_t dest; /* destitation physics address */
	uint64_t src; /* source physics address */
};

struct hobot_pcie_cmd_init_data {
	int32_t revision;
	uint64_t bar_size[BAR_MAX_NUM]; /* BAR size */
	int32_t max_user_irq_num;
	int32_t read_bar_via_dma;
};

struct hobot_pcie_cmd_request_channel_data {
	int32_t chan; /* channel id */
	/*
	 * channel alloc mask
	 * chan_alloc[0:3]: write chan
	 * chan_alloc[16:19]: read chan
	 */
	uint32_t chan_alloc;
};

struct hobot_pcie_cmd_dma_data {
	int32_t chan; /* channel id */
	int32_t wei; /* priority */
	int32_t ele_nr; /* number of eles */
	struct dma_element eles[HOBOT_MAX_DMA_ELEMENTS];
#if HOBOT_PCIE_DMA_TS_STAT_ENABLE
	uint64_t id;
	struct dma_timev ts[HOBOT_DMA_TS_MAX];
#endif
};

struct hobot_pcie_cmd_uirq_data {
	uint32_t id; /* user interrupt id */
	uint32_t dummy;
};

struct hobot_pcie_cmd_map_pci_data {
	uint64_t size; /* map size */
	uint64_t pci_phys; /* PCI physics address */
	uint64_t mapped_phys; /* mapped EP PCI address */
};

struct hobot_pcie_cmd_bar_info {
	int32_t bar_id;
	uint64_t cpu_phys;
	uint64_t pci_phys;
};

struct hobot_pcie_cmd_read_bar {
	int32_t bar_id;
	uint64_t offset;
	uint64_t size;
	void *buf;
};

struct hobot_pcie_cmd_bus_info {
	uint32_t bus_id;
	uint32_t dev_id;
	uint32_t fun_id;
};

union hobot_pcie_cmd_data {
	struct hobot_pcie_cmd_init_data init;
	struct hobot_pcie_cmd_request_channel_data channel_req;
	struct hobot_pcie_cmd_dma_data dma;
	struct hobot_pcie_cmd_map_pci_data map;
	struct hobot_pcie_cmd_bar_info bar_info;
	struct hobot_pcie_cmd_uirq_data uirq;
	struct hobot_pcie_cmd_read_bar read_bar;
	struct hobot_pcie_cmd_bus_info bus_info;
	int32_t channel_id;
};

#endif //INCLUDE_UAPI_LINUX_PCIE_HOBOT_H_
