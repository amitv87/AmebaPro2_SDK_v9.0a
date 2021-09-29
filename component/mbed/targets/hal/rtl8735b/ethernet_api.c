/**************************************************************************//**
 * @file     ethernet_api.c
 * @brief    This file implement the Ethernet HAL API.
 *
 * @version  V1.00
 * @date     2017-06-22
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2016 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 ******************************************************************************/
#if 0
#include "ethernet_api.h"
#include "ethernet_ex_api.h"
#include "hal_eth.h"



#if defined(CONFIG_MII_EN) && (CONFIG_MII_EN == 1)

void ethernet_irq_hook(ethernet_callback callback)
{
	if (callback == NULL) {
		DBG_ETH_ERR("ethernet_irq_hook(): Invalid parameter !!\r\n");
		return;
	}

	hal_eth_callback_hook(callback);
}


void ethernet_task_yield_hook(ethernet_task_yield task_yield)
{
	if (task_yield == NULL) {
		DBG_ETH_ERR("ethernet_task_yield_hook(): Invalid parameter !!\r\n");
		return;
	}

	hal_eth_task_yield_hook(task_yield);
}


/* Set the numbers of Tx/Rx descriptor */
void ethernet_set_descnum(uint8_t txdescCnt, uint8_t rxdescCnt)
{
	if ((txdescCnt == 0) || (rxdescCnt == 0)) {
		DBG_ETH_ERR("ethernet_set_descnum(): Invalid parameter !!\r\n");
		return;
	}
	if (((txdescCnt * ETH_TX_DESC_SIZE) % 32) || ((rxdescCnt * ETH_RX_DESC_SIZE) % 32)) {
		DBG_ETH_ERR("ethernet_set_descnum(): The size of Tx/Rx descriptor ring must be 32-Byte alignment !!\r\n");
		return;
	}

	hal_eth_set_desc_num(txdescCnt, rxdescCnt);
}


/* Set the start address of Tx/Rx descriptor and packet buffer */
void ethernet_trx_pre_setting(uint8_t *TxDescAddr, uint8_t *RxDescAddr, uint8_t *pTxPktBuf, uint8_t *pRxPktBuf)
{
	if ((TxDescAddr == NULL) || (RxDescAddr == NULL) || (pTxPktBuf == NULL) || (pRxPktBuf == NULL)) {
		DBG_ETH_ERR("ethernet_trx_pre_setting(): Invalid parameter !!\r\n");
		return;
	}
	if ((((uint32_t)TxDescAddr) & 0x1F) || (((uint32_t)RxDescAddr) & 0x1F) || (((uint32_t)pTxPktBuf) & 0x1F) || (((uint32_t)pRxPktBuf) & 0x1F)) {
		DBG_ETH_ERR("ethernet_trx_pre_setting(): The address must be 32-Byte alignment !!\r\n");
		return;
	}

	hal_eth_set_desc_addr(TxDescAddr, RxDescAddr);
	hal_eth_set_pkt_buf(pTxPktBuf, pRxPktBuf);
}


// Set the ethernet address
void ethernet_set_address(char *mac)
{
	if (mac == NULL) {
		DBG_ETH_ERR("ethernet_set_address(): Invalid parameter !!\r\n");
		return;
	}

	hal_eth_set_mac_addr((u8 *)mac);
}


int ethernet_init(void)
{
	return hal_eth_init(ETH_IF_SEL, ETH_PIN_SEL);
}


int ethernet_init_force_spd(void)
{
	return hal_eth_init_force_spd(ETH_IF_SEL, ETH_PIN_SEL);
}


void ethernet_free(void)
{
	hal_eth_deinit();
}


// write size bytes from data to ethernet buffer
// return num bytes written
// or -1 if size is too big
int ethernet_write(const char *data, int size)
{
	if ((data == NULL) || (size == 0)) {
		DBG_ETH_ERR("ethernet_write(): Invalid parameter !!\r\n");
		return 0;
	}
	if (size > 1514) {
		DBG_ETH_ERR("ethernet_write(): The size is too big !!\r\n");
		return (-1);
	}

	return hal_eth_write_data((u8 *)data, size);
}


// send ethernet write buffer, returning the packet size sent
int ethernet_send(void)
{
	return hal_eth_send_pkt();
}


// recieve from ethernet buffer, returning packet size, or 0 if no packet
int ethernet_receive(void)
{
	return hal_eth_receive_pkt();
}


// read size bytes in to data, return actual num bytes read (0..size)
// if data == NULL, throw the bytes away
int ethernet_read(char *data, int size)
{
	if ((data == NULL) || (size == 0)) {
		DBG_ETH_ERR("ethernet_read(): Invalid parameter !!\r\n");
		return 0;
	}

	return hal_eth_read_data((u8 *)data, size);
}


// get the ethernet address
void ethernet_address(char *mac)
{
	if (mac == NULL) {
		DBG_ETH_ERR("ethernet_address(): Invalid parameter !!\r\n");
		return;
	}

	hal_eth_get_mac_addr((u8 *)mac);
}


// see if the link is up
int ethernet_link(void)
{
	return hal_eth_get_link_status();
}


// force link settings
void ethernet_set_link(int speed, int duplex)
{
	hal_eth_set_link(speed, duplex);
}
#endif  // end of "#if defined(CONFIG_MII_EN) && (CONFIG_MII_EN == 1)"
#endif


