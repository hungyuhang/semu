/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Joël Porquet-Lupine
 *
 * LupIO-RTC - Realtime clock virtual device
 */
#ifndef RTC_H
#define RTC_H

#define IRQ_RTC 4
#define IRQ_RTC_BIT (1 << IRQ_RTC)
#include <stdint.h>
#include "device.h"

/*
 * LupIO-RTC device interface
 */
typedef uint64_t phys_addr_t;

void lupio_rtc_init(phys_addr_t base,
                    phys_addr_t size);
void lupio_rtc_read(vm_t *vm,
                    rtc_states *rtcState,
                    uint32_t addr,
                    uint8_t width,
                    uint32_t *value);
uint64_t lupio_rtc_reg_read(rtc_states *rtcState, 
                            uint32_t offset, 
                            uint8_t *value);
void lupio_rtc_write(vm_t *vm,
                    rtc_states *rtcState,
                    uint32_t addr,
                    uint8_t width,
                    uint32_t value);
void lupio_rtc_reg_write(rtc_states *rtcState, 
                            uint32_t offset, 
                            uint8_t value);
#endif /* RTC_H */

