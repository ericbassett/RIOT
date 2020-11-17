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

nina_w102_t nina_w102_dev;

int nina_w102_init(nina_w102_t *dev, const nina_w102_params_t *params)
{
  dev->params = *params;
  return 0;
}

int nina_w102_get_fw_ver(nina_w102_t *dev, char *version) {
  nina_w102_cmd_response response;

  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);
  
  // Request send firmware version
  _nina_w102_send_cmd(dev, GET_FW_VERSION_CMD, PARAM_NUMS_0, &response);

  // Receive firmware version
  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, version, response.first_param_len);

  // End transaction
  spi_release(dev->params.spi);
  return response.first_param_len;
}

int nina_w102_start_scan_networks(nina_w102_t *dev) {
  nina_w102_cmd_response response;
  
  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);
  
  // Request scan networks
  _nina_w102_send_cmd(dev, START_SCAN_NETWORKS, PARAM_NUMS_0, &response);
  printf("Response: %#x\n", response.cmd);
  printf("Number Params: %d\nFirst Param Length: %d\n", response.num_params, response.first_param_len);
  // End transaction
  spi_release(dev->params.spi);

  return 0;   
}

int nina_w102_get_scan_networks(nina_w102_t *dev, char networks[][50]) {
  nina_w102_cmd_response response;
  uint8_t param_len;
  int num_networks;
  
  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);
  
  // Request networks from scan
  _nina_w102_send_cmd(dev, SCAN_NETWORKS, PARAM_NUMS_0, &response);
  param_len = response.first_param_len;
  num_networks = response.num_params;
  printf("Response: %#x\n", response.cmd);
  printf("Number of networks: %d\nFirst Network Length: %d\n", num_networks, param_len);
  if (response.num_params > 20) { num_networks = 20; }
  for(int i = 0; i < num_networks; i++) {
    _nina_w102_get_param(dev, networks[i], param_len);
    param_len = _nina_w102_get_param_len(dev);
    printf("Network[%d]: %s\nNext Network Length: %d\n", i, networks[i], param_len);
  }

  // End transaction
  spi_release(dev->params.spi);

  return response.num_params;   
}

void _nina_w102_get_param(nina_w102_t *dev, void *param, uint8_t length) {
  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, param, length);
}

uint8_t _nina_w102_get_param_len(nina_w102_t *dev) {
  return spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
}

void _nina_w102_send_cmd(nina_w102_t *dev, uint8_t cmd, uint8_t numParam, nina_w102_cmd_response* resp) {

  // Send Spi START CMD
  spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, START_CMD);

  // Send Spi C + cmd
  resp->cmd = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, cmd & ~(REPLY_FLAG));

  // Send Spi totLen
  //spiTransfer(totLen);

  // Send Spi numParam
  resp->num_params = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, numParam);

  // If numParam == 0 send END CMD
  if (numParam == 0)
  {
    resp->first_param_len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, END_CMD);
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


