/*
 * Copyright (C) 2018 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup sys
 * @{
 *
 * @file
 * @brief CDC ACM stdio implementation for USBUS CDC ACM
 *
 * This file implements a USB CDC ACM callback and read/write functions.
 *
 *
 * @}
 */

#define USB_H_USER_IS_RIOT_INTERNAL

#include <stdio.h>
#include <sys/types.h>

#include "log.h"
#include "isrpipe.h"

#include "usb/usbus.h"
#include "usb/usbus/cdc/acm.h"

#if MODULE_VFS
#include "vfs.h"
#endif

#ifdef MODULE_USB_BOARD_RESET
#include "usb_board_reset_internal.h"
#endif

#define UART_THREAD_CDC_FULL_FLAG   (_U_(0x1) << 3)
#define NINA_BUFF_SZ                (128 * 16)

static usbus_cdcacm_device_t cdcacm;
static uint8_t _cdc_tx_buf_mem[CONFIG_USBUS_CDC_ACM_STDIO_BUF_SIZE];
static uint8_t _cdc_rx_buf_mem[NINA_BUFF_SZ];
static isrpipe_t _cdc_stdio_isrpipe = ISRPIPE_INIT(_cdc_rx_buf_mem);

static uint8_t _boot_buf_mem[CONFIG_USBUS_CDC_ACM_STDIO_BUF_SIZE];
static isrpipe_t _boot_isrpipe = ISRPIPE_INIT(_boot_buf_mem);
isrpipe_t *_boot_isrpipe_ptr = &_boot_isrpipe;
isrpipe_t *_cdc_stdio_isrpipe_ptr = &_cdc_stdio_isrpipe;

static bool data_in_ep_disabled = false;
static uint16_t bulk_out_maxpacketsize = 64;

void stdio_init(void)
{
    /* Initialize this side of the CDC ACM pipe */
#if MODULE_VFS
    vfs_bind_stdio();
#endif
}

ssize_t stdio_read(void* buffer, size_t len)
{
    (void)buffer;
    (void)len;
    unsigned old;
    (void) old;
    // old = irq_disable();
    ssize_t ret = isrpipe_read(&_cdc_stdio_isrpipe, buffer, len);
    if (data_in_ep_disabled && ret > 0) {      
      // data_in_ep_disabled = false;
      // usbus_event_post(cdcacm.usbus, &cdcacm.retry);      
      // printf("\n\nempty read buffer\n\n");
      if (tsrb_empty(&_cdc_stdio_isrpipe.tsrb)) {             
        (void) bulk_out_maxpacketsize;  
        data_in_ep_disabled = false;
        usbus_event_post(cdcacm.usbus, &cdcacm.retry);
      }      
    }
    // irq_restore(old);
    return ret;
}

ssize_t stdio_write(const void* buffer, size_t len)
{
    const char *start = buffer;
    do {
        size_t n = usbus_cdc_acm_submit(&cdcacm, buffer, len);
        usbus_cdc_acm_flush(&cdcacm);
        // usbus_event_post(cdcacm.usbus, &cdcacm.flush);
        /* Use tsrb and flush */
        buffer = (char *)buffer + n;
        len -= n;
    } while (len);
    return (char *)buffer - start;
}

static size_t _cdc_acm_rx_pipe(usbus_cdcacm_device_t *cdcacm,
                             uint8_t *data, size_t len)
{
    (void)cdcacm;
    size_t i;

    if (len > tsrb_free(&_cdc_stdio_isrpipe.tsrb)) {
      data_in_ep_disabled = true;
      // printf("\n\nfull read buffer\n\n");
      return 0;
    }
    for (i = 0; i < len; i++) {
      // isrpipe_write_one(&_cdc_stdio_isrpipe, data[i]);
      (void) _boot_isrpipe;
      if(isrpipe_write_one(&_cdc_stdio_isrpipe, data[i]) < 0) {
        break;
      }     
    }
    return len;
}

void usb_cdc_acm_stdio_init(usbus_t *usbus)
{
  (void) _boot_isrpipe_ptr;
    usbus_cdc_acm_init(usbus, &cdcacm, _cdc_acm_rx_pipe, NULL,
                       _cdc_tx_buf_mem, sizeof(_cdc_tx_buf_mem));
    // bulk_out_maxpacketsize = cdcacm.iface_data.ep->maxpacketsize;
#ifdef MODULE_USB_BOARD_RESET
    usbus_cdc_acm_set_coding_cb(&cdcacm, usb_board_reset_coding_cb);
#endif
}
