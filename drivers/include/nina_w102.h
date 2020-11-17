/*
 * Copyright (C) 2020 Eric Bassett
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_nina_w102 u-blox WiFi Nina W102 module
 * @ingroup     drivers_netdev
 * @brief       u-blox WiFi Nina W102 module present on some Arduino boards
 *
 * @{
 *
 * @file
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 */

#ifndef NINA_W102_H
#define NINA_W102_H

#include "net/netdev.h"
#include "periph/gpio.h"
#include "periph/spi.h"

/* Add header includes here */

#ifdef __cplusplus
extern "C" {
#endif

/* Declare the API of the driver */

/**
 * @brief   Device initialization parameters
 */
typedef struct {
    spi_t spi;              /**< SPI device */
    spi_clk_t spi_clk;      /**< SPI clock speed used */
    spi_mode_t spi_mode;    /**< SPI mode used */
    gpio_t cs_pin;          /**< SPI CS pin (chip select LOW active) */    
    /* add initialization params here */
} nina_w102_params_t;

/**
 * @brief   Device descriptor for the driver
 */
typedef struct {
    netdev_t netdev;            /**< Pulls in the netdev fields */
    nina_w102_params_t params; /**< Device initialization parameters */

    bool connected;             /**< Indicates whether connected to an AP */
    char ap[6];                 /**< BSSID of current AP */

    uint8_t* rx_buf;            /**< Incoming packet in receive buffer */
    uint16_t rx_len;            /**< Length of an incoming packet, if there
                                     is no packet in the buffer, it is 0 */  
} nina_w102_t;

/**
 * @brief   Initialize the given device
 *
 * @param[inout] dev        Device descriptor of the driver
 * @param[in]    params     Initialization parameters
 *
 * @return                  0 on success
 */
int nina_w102_init(nina_w102_t *dev, const nina_w102_params_t *params);

/**
 * @brief   Get device firmware version
 *
 * @param[inout] dev        Device descriptor of the driver
 * @param[inout] char       Address to return firmware version to
 *
 */
int nina_w102_get_fw_ver(nina_w102_t *dev, char *version);

typedef struct{
  uint8_t cmd;
  uint8_t num_params;
  uint8_t first_param_len;
} nina_w102_cmd_response;

void _nina_w102_send_cmd(nina_w102_t *dev, uint8_t cmd, uint8_t numParam, nina_w102_cmd_response* resp);
uint8_t _nina_w102_read_param_len8(nina_w102_t *dev, uint8_t* param_len);
void _nina_w102_get_param(nina_w102_t *dev, void *param, uint8_t length);
uint8_t _nina_w102_get_param_len(nina_w102_t *dev);
int nina_w102_get_scan_networks(nina_w102_t *dev, char networks[][50]);
int nina_w102_start_scan_networks(nina_w102_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* NINA_W102_H */
/** @} */
