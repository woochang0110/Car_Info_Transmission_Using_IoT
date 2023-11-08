/*
  esp32_can.h
  2019 Copyright (c) Musuk Choi  All Rights Reserved.

  Author:Musuk Choi

  2019-3-22
  This library base on ESP32_CAN library and ESP32 CAN driver
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
  1301  USA
*/
#include "esp32_can.h"


/*********************************************************************************************************
** Function name:           ESP32_CAN
** Descriptions:            Public function to declare CAN class
*********************************************************************************************************/
ESP32_CAN::ESP32_CAN(void)
{

}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            Public function to declare controller initialization parameters.
*********************************************************************************************************/
INT8U ESP32_CAN::begin(INT8U txpin,INT8U rxpin, INT8U speedset)
{
    esp_err_t res;

    //Initialize configuration structures using macro initializers
   // CAN_GENERAL_CONFIG_DEFAULT macro cause conversion error due to CAN_IO_UNUSED (-1). 
	can_general_config_t g_config =  {.mode = CAN_MODE_NORMAL, .tx_io = (gpio_num_t)txpin, .rx_io = (gpio_num_t)rxpin, \
                                    .clkout_io = (gpio_num_t)CAN_IO_UNUSED, .bus_off_io = (gpio_num_t)CAN_IO_UNUSED,\
                                    .tx_queue_len = 5, .rx_queue_len = 5,                                           \
                                    .alerts_enabled = CAN_ALERT_NONE,  .clkout_divider = 0};
	//can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT((gpio_num_t)txpin, (gpio_num_t)rxpin, CAN_MODE_NORMAL); // Tx, Rx, mode
    can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
	can_timing_config_t t_config;
	if(speedset == CAN_25KBPS){
		t_config = CAN_TIMING_CONFIG_25KBITS();
	} else if(speedset == CAN_50KBPS){
	    t_config = CAN_TIMING_CONFIG_50KBITS();
	} else if(speedset == CAN_100KBPS){
	    t_config = CAN_TIMING_CONFIG_100KBITS();
	} else if(speedset == CAN_125KBPS){
	    t_config = CAN_TIMING_CONFIG_125KBITS();
	} else if(speedset == CAN_250KBPS){
	    t_config = CAN_TIMING_CONFIG_250KBITS();
	} else if(speedset == CAN_800KBPS){
	    t_config = CAN_TIMING_CONFIG_800KBITS();
	} else if(speedset == CAN_1000KBPS){
	    t_config = CAN_TIMING_CONFIG_1MBITS();
	} else{
		// Set 500K as default
	    t_config = CAN_TIMING_CONFIG_500KBITS();
	}
	
    res = can_driver_install(&g_config, &t_config, &f_config);
	if (res == ESP_OK){
		//Start CAN driver
		if (can_start() == ESP_OK) {
			return CAN_OK;
		}
	}    
    return CAN_FAILINIT;
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message to transmitt buffer
*********************************************************************************************************/
INT8U ESP32_CAN::sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf)
{
    esp_err_t res;
	can_message_t message_tx;
	
	message_tx.identifier = id;
	
	if(ext == CAN_EXT){
		message_tx.flags = CAN_MSG_FLAG_EXTD;
	} else {
		// Default : Standard CAN ID
		message_tx.flags = CAN_MSG_FLAG_NONE;
	}
	
	message_tx.data_length_code = len;
	memcpy(message_tx.data,buf,len);
	
	res = can_transmit(&message_tx, pdMS_TO_TICKS(10));
	
	if(res == ESP_OK){
		return CAN_OK;
	} else {
		return CAN_FAILTX;
	}
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message to transmitt buffer
*********************************************************************************************************/
INT8U ESP32_CAN::sendMsgBuf(INT32U id, INT8U len, INT8U *buf)
{
    INT8U ext = 0;
    esp_err_t res;
	can_message_t message_tx;
    
    if((id & 0x80000000) == 0x80000000)
        ext = 1;
        
	message_tx.identifier = id;
	
	if(ext == CAN_EXT){
		message_tx.flags = CAN_MSG_FLAG_EXTD;
	} else {
		// Default : Standard CAN ID
		message_tx.flags = CAN_MSG_FLAG_NONE;
	}
	
	message_tx.data_length_code = len;
	memcpy(message_tx.data,buf,len);
	
	res = can_transmit(&message_tx, pdMS_TO_TICKS(10));
	
	if(res == ESP_OK){
		return CAN_OK;
	} else {
		return CAN_FAILTX;
	}
}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            Public function, Reads message from receive buffer.
*********************************************************************************************************/
INT8U ESP32_CAN::readMsgBuf(INT32U *id, INT8U *ext, INT8U *len, INT8U buf[])
{
	esp_err_t res;
	can_message_t message_rx;
	
    if(can_receive(&message_rx, pdMS_TO_TICKS(10)) != ESP_OK)
		return CAN_NOMSG;
	
    *id  = message_rx.identifier;
    *len = message_rx.data_length_code;
	if(message_rx.flags == CAN_MSG_FLAG_EXTD){
		*ext = CAN_EXT;
	} else {
		*ext = CAN_STD;		
	}    
	
	memcpy(buf,message_rx.data,message_rx.data_length_code);

    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            Public function, Reads message from receive buffer.
*********************************************************************************************************/
INT8U ESP32_CAN::readMsgBuf(INT32U *id, INT8U *len, INT8U buf[])
{
	esp_err_t res;
	can_message_t message_rx;
	
    if(can_receive(&message_rx, pdMS_TO_TICKS(10)) != ESP_OK)
		return CAN_NOMSG;
	
    *id  = message_rx.identifier;
    *len = message_rx.data_length_code;
	
	memcpy(buf,message_rx.data,message_rx.data_length_code);

    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            Public function, Checks for received data.  (Used if not using the interrupt output)
*********************************************************************************************************/
INT8U ESP32_CAN::checkReceive(void)
{
	can_status_info_t status_info;
	esp_err_t res = can_get_status_info(&status_info);
	/* Number of messages in RX queue waiting to be read */
    if ( status_info.msgs_to_rx )
        return CAN_MSGAVAIL;
    else 
        return CAN_NOMSG;
}

/*********************************************************************************************************
** Function name:           checkError
** Descriptions:            Public function, Returns error register data.
*********************************************************************************************************/
INT8U ESP32_CAN::checkError(void)
{
	uint32_t alerts;
    esp_err_t res = can_read_alerts(&alerts, pdMS_TO_TICKS(10));

    if ((alerts & CAN_ALERT_BUS_ERROR) ||  (alerts & CAN_ALERT_BUS_OFF))
        return CAN_CTRLERROR;
    else
        return CAN_OK;
}

/*********************************************************************************************************
** Function name:           getError
** Descriptions:            Returns error register value.
*********************************************************************************************************/
INT32U ESP32_CAN::getError(void)
{
    uint32_t alerts;
    esp_err_t res = can_read_alerts(&alerts, pdMS_TO_TICKS(10));
	return (INT32U)alerts;
}

/*********************************************************************************************************
** Function name:           mcp2515_errorCountRX
** Descriptions:            Returns REC register value
*********************************************************************************************************/
INT8U ESP32_CAN::errorCountRX(void)                             
{
	can_status_info_t status_info;
	esp_err_t res = can_get_status_info(&status_info);
    return status_info.rx_error_counter;
}

/*********************************************************************************************************
** Function name:           mcp2515_errorCountTX
** Descriptions:            Returns TEC register value
*********************************************************************************************************/
INT8U ESP32_CAN::errorCountTX(void)                             
{
	can_status_info_t status_info;
	esp_err_t res = can_get_status_info(&status_info);
    return status_info.tx_error_counter;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
