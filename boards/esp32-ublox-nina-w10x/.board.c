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
 * @brief       Board specific implementations for the ublox NINA-W10 series board
 *
 * @author      Eric Bassett <eric.bassett@outlook.com>
 *
 * @}
 */

#include "cpu.h"
#include "board.h"
#include "periph/gpio.h"

void board_init(void)
{
    /* initialize the CPU */
    cpu_init();
}
