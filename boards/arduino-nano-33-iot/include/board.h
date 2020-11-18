/*
 * Copyright (C) 2016-2017 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_arduino-nano-33-IoT
 * @{
 *
 * @file
 * @brief       Board specific definitions for the Arduino Nano 33 IoT
 *              board
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"
#include "board_common.h"
#include "arduino_pinmap.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    LED pin definitions and handlers
 * @{
 */
#define LED0_PIN            GPIO_PIN(PA, 17)

#define LED_PORT            PORT->Group[PA]
#define LED0_MASK           (1 << 17)

#define LED0_ON             (LED_PORT.OUTSET.reg = LED0_MASK)
#define LED0_OFF            (LED_PORT.OUTCLR.reg = LED0_MASK)
#define LED0_TOGGLE         (LED_PORT.OUTTGL.reg = LED0_MASK)

#define LED0_NAME           "LED(Amber)"

/** @} */

/**
 * @name    Board configuration for Nina W102 WiFi netdev driver
 * @{
 */
#define NINA_W102_PARAM_SPI             SPI_DEV(1)
#define NINA_W102_PARAM_CS_PIN          GPIO_PIN(PA, 14)
#define NINA_W102_PARAM_ACK_PIN         GPIO_PIN(PA, 28)
#define NINA_W102_PARAM_RSTN_PIN        GPIO_PIN(PA, 8)
#define NINA_W102_PARAM_GPIO0_PIN       GPIO_PIN(PA, 27)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
