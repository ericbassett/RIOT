/*
 * Copyright (C) 2020 Eric Bassett
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_nina_w102
 *
 * @{
 * @file
 * @brief       Default configuration
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 */

#ifndef NINA_W102_PARAMS_H
#define NINA_W102_PARAMS_H

#include "board.h"
#include "nina_w102.h"
#include "nina_w102_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   SSID of the AP to be used.
 */
#ifndef WIFI_SSID
#define WIFI_SSID       "RIOT_AP"
#endif

/**
 * @brief   Passphrase used for the AP as clear text (max. 64 chars).
 */
#ifdef DOXYGEN
#define WIFI_PASS       "ThisistheRIOTporttoESP"
#endif

/**
 * @name    Set default configuration parameters
 * @{
 */
#ifndef NINA_W102_PARAM_SPI
#define NINA_W102_PARAM_SPI             SPI_DEV(1)
#endif

#ifndef NINA_W102_PARAM_SPI_CLK
#define NINA_W102_PARAM_SPI_CLK        SPI_CLK_100KHZ
#endif

#ifndef NINA_W102_PARAM_CS_PIN
#define NINA_W102_PARAM_CS_PIN         GPIO_PIN(PA, 14)       /* PA14 */
#endif

#ifndef NINA_W102_PARAM_SPI_MODE
#define NINA_W102_PARAM_SPI_MODE       SPI_MODE_0
#endif

#ifndef NINA_W102_PARAM_ACK_PIN
#define NINA_W102_PARAM_ACK_PIN        GPIO_PIN(PA, 28)
#endif

#ifndef NINA_W102_PARAM_RST_PIN
#define NINA_W102_PARAM_RSTN_PIN       GPIO_PIN(PA, 8)
#endif

#ifndef NINA_W102_PARAM_GPIO0_PIN
#define NINA_W102_PARAM_GPIO0_PIN      GPIO_PIN(PA, 27)
#endif


#ifndef NINA_W102_PARAMS
#define NINA_W102_PARAMS   {                                               \
                              .spi         = NINA_W102_PARAM_SPI,          \
                              .spi_clk     = NINA_W102_PARAM_SPI_CLK,     \
                              .spi_mode    = NINA_W102_PARAM_SPI_MODE,    \
                              .cs_pin      = NINA_W102_PARAM_CS_PIN,       \
                              .ack_pin     = NINA_W102_PARAM_ACK_PIN,      \
                              .rstn_pin    = NINA_W102_PARAM_RSTN_PIN,      \
                              .gpio0_pin   = NINA_W102_PARAM_GPIO0_PIN,    \
                            }
#endif

/**@}*/

/**
 * @brief   Configuration struct
 */
static const nina_w102_params_t nina_w102_params[] =
{
    NINA_W102_PARAMS
};

#ifdef __cplusplus
}
#endif

#endif /* NINA_W102_PARAMS_H */
/** @} */
