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

// #define NINA_W102_ALT_CS            GPIO_PIN(PA, 14)
// #define NINA_W102_ACK_PIN           GPIO_PIN(PA, 27)
// #define NINA_W102_SEL_WIFI_PIN      GPIO_PIN(PA, 9)
// #define NINA_W102_RESET_PIN         GPIO_PIN(PA, 8)

nina_w102_t nina_w102_dev;

int nina_w102_init(nina_w102_t *dev, const nina_w102_params_t *params)
{
  #ifdef NINA_W102_ALT_CS
    gpio_init(NINA_W102_ALT_CS, GPIO_OUT);
    _nina_w102_deselect_slave();
  #endif
  gpio_init(params->ack_pin, GPIO_IN);
  gpio_init(params->rstn_pin, GPIO_OUT);
  gpio_set(params->gpio0_pin);
  gpio_set(params->cs_pin);
  gpio_clear(params->rstn_pin);
  xtimer_usleep(10000);
  gpio_set(params->rstn_pin);
  xtimer_usleep(750000);

  gpio_clear(params->gpio0_pin);
  gpio_init(params->gpio0_pin, GPIO_IN);
  dev->params = *params;
  return 0;
}

int nina_w102_get_fw_ver(nina_w102_t *dev, char *version) {
  nina_w102_cmd_response response;
  uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev);

  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);
  
  // select slave
  _nina_w102_select_slave();

  // Request send firmware version
  _nina_w102_send_cmd(dev, GET_FW_VERSION_CMD, PARAM_NUMS_0, &response, false);

  // deselect slave
  _nina_w102_deselect_slave();

  // wait for ready
  _nina_w102_wait_ready(dev);

  // select slave
  _nina_w102_select_slave();

  // Receive firmware version
  // spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, mac_id, response.first_param_len);
  int c = 0;
  do {        
    r = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);    
    printf("c: %d\t%#X\n", c,r);
  } while (r != (GET_FW_VERSION_CMD | REPLY_FLAG) && c++ < 10);

  // param num
  response.cmd = r;
  response.num_params = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  response.first_param_len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);

  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, version, response.first_param_len);
  
  r = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA);

  // deselect slave
  _nina_w102_deselect_slave();

  if (r == END_CMD) {printf("woohoo!\n");}
  spi_release(dev->params.spi);

  return response.first_param_len;
}

uint8_t nina_w102_get_conn_stat(nina_w102_t *dev) {
  nina_w102_cmd_response response;
  uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev);  

  // Aquire bus  
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // select slave
  _nina_w102_select_slave();

  // Request send connection status
  _nina_w102_send_cmd(dev, GET_CONN_STATUS_CMD, PARAM_NUMS_0, &response, false);

  // deselect slave
  _nina_w102_deselect_slave();

  // wait for ready
  _nina_w102_wait_ready(dev);

  // select slave
  _nina_w102_select_slave();   

  // Receive connection status
  int c = 0;
  do {
    r = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);    
    printf("c: %d\t%#X\n", c,r);
  } while (r != (GET_CONN_STATUS_CMD | REPLY_FLAG) && c++ < 10);

  response.cmd = r;
  response.num_params = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  response.first_param_len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  r = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  if (END_CMD == spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA)) {printf("woohoo!\n");}

  // deselect slave
  _nina_w102_deselect_slave();

  spi_release(dev->params.spi);

  return r;
}

int nina_w102_get_mac_id(nina_w102_t *dev, char* mac_id) {
  nina_w102_cmd_response response;
  uint8_t r;

  // wait for ready
  _nina_w102_wait_ready(dev); 

  // Aquire bus
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);

  // select slave
  _nina_w102_select_slave();

  // Request send mac ID  
  _nina_w102_send_cmd(dev, GET_MACADDR_CMD, PARAM_NUMS_0, &response, false);

  // deselect slave
  _nina_w102_deselect_slave();

  // wait for ready
  _nina_w102_wait_ready(dev); 

  // select slave
  _nina_w102_select_slave(); 

  // Receive mac ID
  // spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, mac_id, response.first_param_len);
  int c = 0;
  do {        
    r = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);    
    printf("c: %d\t%#X\n", c,r);
  } while (r != (GET_MACADDR_CMD | REPLY_FLAG) && c++ < 10);

  // param num
  response.cmd = r;
  response.num_params = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  response.first_param_len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);

  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, true, NULL, mac_id, response.first_param_len);
  
  r = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, false, DUMMY_DATA);

  // deselect slave
  _nina_w102_deselect_slave(); 

  if (r == END_CMD) {printf("woohoo!\n");}
  spi_release(dev->params.spi);

  return response.first_param_len;
}

int nina_w102_start_scan_networks(nina_w102_t *dev) {
  nina_w102_cmd_response response;
  char version[10];
  

  // Aquire bus
  printf("Slave ready: %d\n", gpio_read(GPIO_PIN(PB, 03)));
  spi_acquire(dev->params.spi, dev->params.cs_pin, dev->params.spi_mode, dev->params.spi_clk);
  // gpio_set(GPIO_PIN(PA, 16));  
  // Request scan networks
  _nina_w102_send_cmd(dev, START_SCAN_NETWORKS, PARAM_NUMS_0, &response, false);
  // xtimer_sleep(2);

  // if (response.start != START_CMD || response.start == 0) {
  //   while (response.start != START_CMD) {
  //     response.start = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  //   }
  //   response.cmd = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  //   response.num_params = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  //   response.first_param_len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
  // }
  // printf("START\tCMD\tRESP\tACT\tNUM\tFIRST\n%#x\t%#x\t%#x\t%#x\t%d\t%d\n",
  //       response.start, START_SCAN_NETWORKS, START_SCAN_NETWORKS | REPLY_FLAG, response.cmd, response.num_params, response.first_param_len);

  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, false, NULL, version, 1);
  // printf("Response: %#x\n", version[0]);
  // gpio_clear(GPIO_PIN(PA, 16));
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
  _nina_w102_send_cmd(dev, SCAN_NETWORKS, PARAM_NUMS_0, &response, true);
  xtimer_sleep(2);
  param_len = response.first_param_len;
  num_networks = response.num_params;
  printf("Response: %#x\n", response.cmd);
  printf("Number of networks: %d\nFirst Network Length: %d\n", num_networks, param_len);
  // if (response.num_params > 20) { num_networks = 20; }
  for(int i = 0; i < num_networks - 1; i++) {
    if (i < 20) {
      _nina_w102_get_param(dev, networks[i], param_len, true);
    }
    else {
      _nina_w102_get_param(dev, NULL, param_len, true);
    }
    param_len = _nina_w102_get_param_len(dev);
    printf("Network[%d]: %s\nNext Network Length: %d\n", i, networks[i], param_len);
  }

  _nina_w102_get_param(dev, networks[num_networks], param_len, false);
    param_len = _nina_w102_get_param_len(dev);
    printf("Network[%d]: %s\nNext Network Length: %d\n", num_networks, networks[num_networks], param_len);

  // End transaction
  spi_release(dev->params.spi);

  return response.num_params;   
}

void _nina_w102_get_param(nina_w102_t *dev, void *param, uint8_t length, bool cont) {
  spi_transfer_bytes(dev->params.spi, dev->params.cs_pin, cont, NULL, param, length);
}

uint8_t _nina_w102_get_param_len(nina_w102_t *dev) {
  return spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, DUMMY_DATA);
}

void _nina_w102_send_cmd(nina_w102_t *dev, uint8_t cmd, uint8_t numParam, nina_w102_cmd_response* resp, bool cont) {
  // Send Spi START CMD
  resp->start = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, START_CMD);

  // Send Spi C + cmd
  resp->cmd = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, cmd & ~(REPLY_FLAG));  

  // Send Spi numParam
  resp->num_params = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, true, numParam);

  // If numParam == 0 send END CMD
  if (numParam == 0)
  {
    resp->first_param_len = spi_transfer_byte(dev->params.spi, dev->params.cs_pin, cont, END_CMD);
    printf("START\tCMD\tRESP\tACT\tNUM\tFIRST\n%#x\t%#x\t%#x\t%#x\t%d\t%d\n",
          resp->start, cmd, cmd | REPLY_FLAG, resp->cmd, resp->num_params, resp->first_param_len);
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

void _nina_w102_wait_ready(nina_w102_t *dev) {
  unsigned long t = 0;
  printf("waiting\n");
  xtimer_usleep(50);
  while(gpio_read(dev->params.ack_pin)) {t++;}
  printf("Waited %lu cycles\n", t);  
}

void _nina_w102_select_slave(void) {
  #ifdef NINA_W102_ALT_CS
  gpio_clear(NINA_W102_ALT_CS);
  #endif
}

void _nina_w102_deselect_slave(void) {
  #ifdef NINA_W102_ALT_CS
  gpio_set(NINA_W102_ALT_CS);
  #endif
}
