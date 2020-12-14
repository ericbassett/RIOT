/*
 * Copyright (C) 2020 Eric Bassett
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_esp32-ublox-nina-w10x
 * @{
 *
 * @file
 * @brief       Configuration of CPU peripherals for ublox NINA-W10 series board
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* put here the board peripherals definitions:
- Available clocks
- Timers
- UARTs
- PWMs
- SPIs
- I2C
- ADC
- RTC
- RTT
etc
 */

/* include common board definitions as last step */
#include "periph_conf_common.h"

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
