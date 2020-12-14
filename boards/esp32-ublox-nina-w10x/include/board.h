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
 * @brief       Board specific definitions for the ublox NINA-W10 series
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 */

#ifndef BOARD_H
#define BOARD_H

// #include "cpu.h"
// #include "periph_conf.h"
// #include "periph_cpu.h"

/* include common board definitions as last step */
#include "board_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Put here board specific definitions:
 - LEDs/buttons macros,
 - on-board drivers parameters
 - xtimer configuration
 */
/**
 * @brief   Initialize board specific hardware
 */
static inline void board_init(void) {
    /* there is nothing special to initialize on this board */
    board_init_common();
}


#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
