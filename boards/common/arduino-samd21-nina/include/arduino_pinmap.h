/*
 * Copyright (C)  2016-2017 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_common_arduino-samd21-nina
 * @{
 *
 * @file
 * @brief       Mapping from MCU pins to Arduino pins
 *
 * You can use the defines in this file for simplified interaction with the
 * Arduino specific pin numbers.
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#ifndef ARDUINO_PINMAP_H
#define ARDUINO_PINMAP_H

#include "periph/gpio.h"
#include "periph/adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Mapping of MCU pins to Arduino pins (comments not currently correct)
 * @{
 */
#define ARDUINO_PIN_0           GPIO_PIN(PB, 23) /* TC4-W0 */
#define ARDUINO_PIN_1           GPIO_PIN(PB, 22) /* TC4-W1 */
#define ARDUINO_PIN_2           GPIO_PIN(PB, 10) /* TCC0-W2 */
#define ARDUINO_PIN_3           GPIO_PIN(PB, 11) /* TCC0-W3 */
#define ARDUINO_PIN_4           GPIO_PIN(PA, 7) /* TCC0-W4 */
#define ARDUINO_PIN_5           GPIO_PIN(PA, 5) /* TCC0-W5 */
#define ARDUINO_PIN_6           GPIO_PIN(PA, 4) /* TCC0-W6, on-board LED */
#define ARDUINO_PIN_7           GPIO_PIN(PA, 6) /* TCC0-W7 */

#define ARDUINO_PIN_8           GPIO_PIN(PA, 18) /* SERCOM1-MOSI */
#define ARDUINO_PIN_9           GPIO_PIN(PA, 20) /* SERCOM1-SCK */
#define ARDUINO_PIN_10          GPIO_PIN(PA, 21) /* SERCOM1-MISO */
#define ARDUINO_PIN_11          GPIO_PIN(PA, 16)  /* SERCOM0-SDA, on-board pull-up */
#define ARDUINO_PIN_12          GPIO_PIN(PA, 19)  /* SERCOM0-SCL, on-board pull-up */
#define ARDUINO_PIN_13          GPIO_PIN(PA, 17) /* SERCOM5-RX from MCU */
#define ARDUINO_PIN_14          GPIO_PIN(PA, 2) /* SERCOM5-TX from MCU */

#define ARDUINO_PIN_A0          GPIO_PIN(PB, 2)  /* AIN0, DAC0 */
#define ARDUINO_PIN_A1          GPIO_PIN(PA, 11)  /* AIN10 */
#define ARDUINO_PIN_A2          GPIO_PIN(PA, 10)  /* AIN11 */
#define ARDUINO_PIN_A3          GPIO_PIN(PB, 8)  /* AIN4 */
#define ARDUINO_PIN_A4          GPIO_PIN(PB, 9)  /* AIN5 */
#define ARDUINO_PIN_A5          GPIO_PIN(PA, 9)  /* AIN6 */
#define ARDUINO_PIN_A6          GPIO_PIN(PB, 3)  /* AIN7 */
/** @} */

/**
 * @name    Mapping of Arduino analog pins to RIOT ADC lines
 * @{
 */
#define ARDUINO_A0              ADC_LINE(0)
#define ARDUINO_A1              ADC_LINE(1)
#define ARDUINO_A2              ADC_LINE(2)
#define ARDUINO_A3              ADC_LINE(3)
#define ARDUINO_A4              ADC_LINE(4)
#define ARDUINO_A5              ADC_LINE(5)
#define ARDUINO_A6              ADC_LINE(6)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_PINMAP_H */
/** @} */
