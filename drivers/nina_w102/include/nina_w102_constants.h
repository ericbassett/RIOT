/*
 * Copyright (C) 2020 Eric Bassett
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_nina_w102
 * @{
 *
 * @file
 * @brief       Internal addresses, registers and constants
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 */

#ifndef NINA_W102_CONSTANTS_H
#define NINA_W102_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* define here the addresses, register and constants of the driver */
#define SET_NET_CMD			        			0x10
#define SET_PASSPHRASE_CMD						0x11
#define SET_KEY_CMD			        			0x12
//	TEST_CMD	        = 			0x13,
#define SET_IP_CONFIG_CMD			  			0x14
#define SET_DNS_CONFIG_CMD						0x15
#define SET_HOSTNAME_CMD			  			0x16
#define SET_POWER_MODE_CMD						0x17
#define SET_AP_NET_CMD			    			0x18
#define SET_AP_PASSPHRASE_CMD					0x19
#define SET_DEBUG_CMD			      			0x1A
#define GET_TEMPERATURE_CMD						0x1B
#define GET_REASON_CODE_CMD						0x1F

#define GET_CONN_STATUS_CMD						0x20
#define GET_IPADDR_CMD			    			0x21
#define GET_MACADDR_CMD			    			0x22
#define GET_CURR_SSID_CMD			  			0x23
#define GET_CURR_BSSID_CMD						0x24
#define GET_CURR_RSSI_CMD			  			0x25
#define GET_CURR_ENCT_CMD			  			0x26
#define SCAN_NETWORKS			      			0x27
#define START_SERVER_TCP_CMD					0x28
#define GET_STATE_TCP_CMD			  			0x29
#define DATA_SENT_TCP_CMD			  			0x2A
#define AVAIL_DATA_TCP_CMD						0x2B
#define GET_DATA_TCP_CMD			  			0x2C
#define START_CLIENT_TCP_CMD					0x2D
#define STOP_CLIENT_TCP_CMD			    	0x2E
#define GET_CLIENT_STATE_TCP_CMD			0x2F
#define DISCONNECT_CMD						    0x30
//	GET_IDX_SSID_CMD	= 0x31,
#define GET_IDX_RSSI_CMD						  0x32
#define GET_IDX_ENCT_CMD						  0x33
#define REQ_HOST_BY_NAME_CMD					0x34
#define GET_HOST_BY_NAME_CMD					0x35
#define START_SCAN_NETWORKS						0x36
#define GET_FW_VERSION_CMD						0x37
//	GET_TEST_CMD		= 	0x38,
#define SEND_DATA_UDP_CMD						  0x39
#define GET_REMOTE_DATA_CMD						0x3A
#define GET_TIME_CMD						      0x3B
#define GET_IDX_BSSID						      0x3C
#define GET_IDX_CHANNEL_CMD						0x3D
#define PING_CMD						          0x3E
#define GET_SOCKET_CMD						    0x3F

// All command with DATA_FLAG 0x40 send a 16bit Len
#define SET_ENT_CMD						        0x40

#define SEND_DATA_TCP_CMD						  0x44
#define GET_DATABUF_TCP_CMD						0x45
#define INSERT_DATABUF_CMD						0x46

// regular format commands
#define SET_PIN_MODE						      0x50
#define SET_DIGITAL_WRITE						  0x51
#define SET_ANALOG_WRITE						  0x52

// regular format commands
#define WRITE_FILE						        0x60
#define READ_FILE						          0x61
#define DELETE_FILE						        0x62
#define EXISTS_FILE						        0x63
#define DOWNLOAD_FILE						      0x64
#define APPLY_OTA_COMMAND						  0x65
#define RENAME_FILE						        0x66
#define DOWNLOAD_OTA						      0x67

#define PARAM_NUMS_0			            0
#define PARAM_NUMS_1			            1
#define PARAM_NUMS_2			            2
#define PARAM_NUMS_3			            3
#define PARAM_NUMS_4			            4
#define PARAM_NUMS_5			            5
#define PARAM_NUMS_6			            6
#define MAX_PARAM_NUMS                7

#define TCP_CLOSED				            0
#define TCP_LISTEN				            1
#define TCP_SYN_SENT				          2
#define TCP_SYN_RCVD				          3
#define TCP_ESTABLISHED				        4
#define TCP_FIN_WAIT_1				        5
#define TCP_FIN_WAIT_2				        6
#define TCP_CLOSE_WAIT				        7
#define TCP_CLOSING				            8
#define TCP_LAST_ACK				          9
#define TCP_TIME_WAIT				          10

#define MAX_PARAMS                    MAX_PARAM_NUMS-1
#define PARAM_LEN_SIZE                1
#define DUMMY_DATA                    0xFF

#define START_CMD                     0xE0
#define END_CMD                       0xEE
#define REPLY_FLAG                    1<<7

#ifdef __cplusplus
}
#endif

#endif /* NINA_W102_CONSTANTS_H */
/** @} */
