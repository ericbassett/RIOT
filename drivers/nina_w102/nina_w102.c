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
 * @brief       Device driver implementation for the u-blox WiFi Nina W102 module
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 *
 * @}
 */

#include "nina_w102.h"
#include "nina_w102_constants.h"
#include "nina_w102_params.h"
#include <stdio.h>
#include "xtimer.h"

#define NINA_W102_MANUAL_RESET true

nina_w102_t nina_w102_dev;

uint8_t nina_w102_init(nina_w102_t *dev, const nina_w102_params_t *params)
{
  gpio_init(params->ack_pin, GPIO_IN_PU);
  gpio_init(params->rstn_pin, GPIO_OUT);
  gpio_set(params->gpio0_pin);
  gpio_set(params->cs_pin);
  gpio_clear(params->rstn_pin);
  xtimer_usleep(10000);
  gpio_set(params->rstn_pin);
  xtimer_usleep(750000);

  gpio_clear(params->gpio0_pin);
  gpio_init(params->gpio0_pin, GPIO_IN_PU);
  dev->params = *params;
  return 0;
}

uint8_t nina_w102_get_fw_ver(nina_w102_t *dev, char *version) {
  uint8_t len;

  // wait for ready
  _nina_w102_wait_ready(dev);

  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Request send firmware version
  _nina_w102_send_cmd(dev, GET_FW_VERSION_CMD, PARAM_NUMS_0);

  // deselect slave
  _nina_w102_deselect_slave(dev);

  // wait for response
  len = _nina_w102_wait_response_cmd(dev, GET_FW_VERSION_CMD, (uint8_t*) version);

  // release bus
  spi_release(dev->params.spi);

  return len;
}

uint8_t nina_w102_get_conn_stat(nina_w102_t *dev) {
  uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev);  

  // Aquire bus  
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Request send connection status
  _nina_w102_send_cmd(dev, GET_CONN_STATUS_CMD, PARAM_NUMS_0);

  // deselect slave manually to add delay
  _nina_w102_deselect_slave(dev);

  // wait for response
  _nina_w102_wait_response_cmd(dev, GET_CONN_STATUS_CMD, &r);

  // release bus
  spi_release(dev->params.spi);

  return r;
}

uint8_t nina_w102_get_mac_id(nina_w102_t *dev, char* mac_id) {
  uint8_t len;
  uint8_t _dummy = DUMMY_DATA;

  // wait for ready
  _nina_w102_wait_ready(dev);

  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Request send mac address
  _nina_w102_send_cmd(dev, GET_MACADDR_CMD, PARAM_NUMS_1);
  _nina_w102_send_param(dev, &_dummy, 1, LAST_PARAM);

  // Pad to multiple of 4
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);

  // deselect slave
  _nina_w102_deselect_slave(dev);

  // wait for response
  len = _nina_w102_wait_response_cmd(dev, GET_MACADDR_CMD, (uint8_t*) mac_id);

  // release bus
  spi_release(dev->params.spi);

  return len;
}

uint8_t nina_w102_start_scan_networks(nina_w102_t *dev) {
  uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev);  

  // Aquire bus  
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Request start scanning networks
  _nina_w102_send_cmd(dev, START_SCAN_NETWORKS, PARAM_NUMS_0);

  // deselect slave manually to add delay
  _nina_w102_deselect_slave(dev);

  // wait for response
  _nina_w102_wait_response_cmd(dev, START_SCAN_NETWORKS, &r);

  // release bus
  spi_release(dev->params.spi);

  return r;
}

uint8_t nina_w102_disconnect(nina_w102_t *dev) {
  uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev);  

  // Aquire bus  
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Request disconnect
  _nina_w102_send_cmd(dev, DISCONNECT_CMD, PARAM_NUMS_0);

  // deselect slave manually to add delay
  _nina_w102_deselect_slave(dev);

  // wait for response
  _nina_w102_wait_response_cmd(dev, DISCONNECT_CMD, &r);

  // release bus
  spi_release(dev->params.spi);

  return r;
}

uint8_t nina_w102_get_scan_networks(nina_w102_t *dev, char** networks, uint8_t max_networks) {
    uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev);  

  // Aquire bus  
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Request start scanning networks
  _nina_w102_send_cmd(dev, SCAN_NETWORKS, PARAM_NUMS_0);

  // deselect slave manually to add delay
  _nina_w102_deselect_slave(dev);

  // wait for response
  _nina_w102_wait_response(dev, SCAN_NETWORKS, &r, (uint8_t **) networks, max_networks);

  // release bus
  spi_release(dev->params.spi);
  
  return r;
}

void _nina_w102_get_param(nina_w102_t *dev, void *param, uint8_t length, bool cont) {
  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, cont, NULL, param, length);
}

uint8_t _nina_w102_get_param_len(nina_w102_t *dev) {
  return spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
}

void _nina_w102_send_cmd(nina_w102_t *dev, uint8_t cmd, uint8_t numParam) {
  // Send Spi START CMD
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, START_CMD);

  // Send Spi C + cmd
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, cmd & ~(REPLY_FLAG));  

  // Send Spi numParam
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, numParam);

  // If numParam == 0 send END CMD
  if (numParam == 0)
  {
    spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, END_CMD);
    // printf("START\tCMD\tRESP\tACT\tNUM\tFIRST\n%#x\t%#x\t%#x\t%#x\t%d\t%d\n",
    //       resp->start, cmd, cmd | REPLY_FLAG, resp->cmd, resp->num_params, resp->first_param_len);
  }    
}

void _nina_w102_send_param(nina_w102_t *dev, uint8_t* param, uint8_t param_len, uint8_t lastParam) {
  // send length
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, param_len);

  // send param
  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, param, NULL, param_len);

  // end
  if (lastParam) {
    spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, END_CMD);
  }
}

uint8_t _nina_w102_read_param_len8(nina_w102_t *dev, uint8_t* param_len)
{
    uint8_t _param_len;
    uint8_t dummy = DUMMY_DATA;
    spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, &dummy, &_param_len, 1);
    // spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, false, NULL, &_param_len, 1);

    if (param_len != NULL)
    {
        *param_len = _param_len;
    }
    return _param_len;
}

uint8_t _nina_w102_wait_response_cmd(nina_w102_t *dev, uint8_t cmd, uint8_t *param) {
  // timeout
  uint8_t c = 0;

  // temp vars
  uint8_t resp, len;

  // wait for device ready
  _nina_w102_wait_ready(dev);
  
  // Receive response
  do {
    resp = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
    printf("c: %d\tr: %#X\n", c, resp);    
  } while (resp != START_CMD && c++ < 100 && resp != ERR_CMD);

  // timeout
  if (c >= 100) {
    return -1;
  }

  // Check for error
  if (resp == ERR_CMD) {
    if (PARAM_NUMS_0 == spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA))
    {
      if (END_CMD == spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA)) {
        return -1;
      }
    }
    return -1;
  }

  // Check for replied command
  if ((REPLY_FLAG | cmd) != spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA)) {
    return -1;
  }

  // Check for wrong number of response commands
  if (PARAM_NUMS_1 != spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA)) {
    // Get END_CMD
    spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA);
    return -1;
  };
  
  // Get response param length
  len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);

  // Get param
  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, param, len);
  
  // Get END_CMD
  if (END_CMD != spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA)) {
    return -1;
  }

  return len;
}

uint8_t _nina_w102_wait_response(nina_w102_t *dev, uint8_t cmd, uint8_t* num_params_read, uint8_t** params, uint8_t max_num_params) {
  // timeout
  uint8_t c = 0;

  // line up pointer array
  char *index[WL_SSID_MAX_LENGTH];

  for (int8_t i = 0; i < WL_NETWORKS_LIST_MAXNUM; i++) {
    index[i] = (char *) params + WL_SSID_MAX_LENGTH * i;
  }

  // temp vars
  uint8_t resp, n_params, n_params_raw;
  uint8_t len = 0;
  int8_t i_param;

  // wait for device ready
  _nina_w102_wait_ready(dev);
  
  // Receive response
  do {
    resp = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
    printf("c: %d\tr: %#X\n", c, resp);    
  } while (resp != START_CMD && c++ < 100 && resp != ERR_CMD);

  // timeout
  if (c >= 100) {
    return -1;
  }

  // Check for error
  if (resp == ERR_CMD) {
    if (PARAM_NUMS_0 == spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA))
    {
      if (END_CMD == spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA)) {
        return -1;
      }
    }
    return -1;
  }

  // Check for replied command
  if ((REPLY_FLAG | cmd) != spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA)) {
    return -1;
  }

  // Check number of response commands
  n_params_raw = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);

  if (n_params_raw > max_num_params) {
    n_params = max_num_params;
  }
  else {
    n_params = n_params_raw;
  }
  
  // Get response params
  for (i_param = 0; i_param < n_params; ++i_param) {
    len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
    spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, index[i_param], len);
    
    // null terminate
    index[i_param][len] = '\0';
  }
  
  // Get END_CMD
  do {
    resp = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
    printf("c: %d\tr: %#X\n", c, resp);    
  } while (resp != END_CMD && c++ < 100);

  // timeout
  if (c >= 100) {
    return -1;
  }

  *num_params_read = i_param;

  return len;  
}

uint8_t nina_w102_connect(nina_w102_t *dev, const char* ssid, uint8_t ssid_len, const char *passphrase, uint8_t pass_len) {
  uint8_t len, r;

  // wait for ready
  _nina_w102_wait_ready(dev);

  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // Send ssid then passphrase
  _nina_w102_send_cmd(dev, SET_PASSPHRASE_CMD, PARAM_NUMS_2);
  _nina_w102_send_param(dev, (uint8_t *)ssid, ssid_len, NO_LAST_PARAM);
  _nina_w102_send_param(dev, (uint8_t *)passphrase, pass_len, LAST_PARAM);

  // Pad to multiple of 4
  uint8_t cmd_sz = 6 + ssid_len + pass_len;
  while (cmd_sz % 4) {
    spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
    ++cmd_sz;
  }

  // deselect slave
  _nina_w102_deselect_slave(dev);

  // wait for response
  len = _nina_w102_wait_response_cmd(dev, SET_PASSPHRASE_CMD, &r);

  // release bus
  spi_release(dev->params.spi);

  return len; 
}

void _nina_w102_wait_ready(nina_w102_t *dev) {
  unsigned long t = 0;
  printf("waiting\n");
  // xtimer_usleep(1);
  while(gpio_read(dev->params.ack_pin)) {t++;}
  printf("Waited %lu cycles\n", t);  
}

void _nina_w102_select_slave(nina_w102_t *dev) {
  #if NINA_W102_MANUAL_RESET
    gpio_clear(dev->params.cs_pin);    
  #else
    if (dev) {}
  #endif
}

void _nina_w102_deselect_slave(nina_w102_t *dev) {
  #if NINA_W102_MANUAL_RESET
    xtimer_nanosleep(1);
    gpio_set(dev->params.cs_pin);    
  #else
    if (dev) {}
  #endif
}
