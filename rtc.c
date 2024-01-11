/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Joël Porquet-Lupine
 *
 * LupIO-RTC - Realtime clock virtual device
 */

#include <assert.h>
#include <inttypes.h>
#include <time.h>
#include <stdio.h>

#include "riscv_private.h"
#include "riscv.h"
#include "rtc.h"
#define DBG_ITEM DBG_RTC

/*
 * Frontend: LupIO-RTC virtual device implementation
 */
enum {
    LUPIO_RTC_SECD  = 0x0,
    LUPIO_RTC_MINT,
    LUPIO_RTC_HOUR,
    LUPIO_RTC_DYMO,
    LUPIO_RTC_MNTH,
    LUPIO_RTC_YEAR,
    LUPIO_RTC_CENT,
    LUPIO_RTC_DYWK,
    LUPIO_RTC_DYYR,

    /* Top offset in register map */
    LUPIO_RTC_OFFSET_MAX,
};

/*
 * Memory accesses
 */
// enum iomem_size_log2 {
//     IOMEM_1BYTE_SIZE_LOG2 = 0,
//     IOMEM_2BYTE_SIZE_LOG2 = 1,
//     IOMEM_4BYTE_SIZE_LOG2 = 2,
//     IOMEM_8BYTE_SIZE_LOG2 = 3
// };

/* I/O devices */
// #define IOMEM_DEV_SIZE8  (1 << IOMEM_1BYTE_SIZE_LOG2)
// struct iomem_dev_ops {
//     void (*write)(void *dev, phys_addr_t offset, uint64_t val,
//                   enum iomem_size_log2 size_log2);
//     uint64_t (*read)(void *dev, phys_addr_t offset,
//                      enum iomem_size_log2 size_log2);
//     int flags;
// };


void lupio_rtc_read(vm_t *vm,
                    rtc_states *rtcState,
                    uint32_t addr,
                    uint8_t width,
                    uint32_t *value)
{
    uint8_t u8value;
    switch (width) {
    case RV_MEM_LW:
    case RV_MEM_LBU:
        lupio_rtc_reg_read(rtcState, addr, &u8value);
        *value = (uint32_t) u8value;
        break;
    case RV_MEM_LB:
    case RV_MEM_LHU:
    case RV_MEM_LH:
        vm_set_exception(vm, RV_EXC_LOAD_MISALIGN, vm->exc_val);
        return;
    default:
        vm_set_exception(vm, RV_EXC_ILLEGAL_INSTR, 0);
        return;
    }
}

uint64_t lupio_rtc_reg_read(rtc_states *rtcState, 
                            uint32_t offset, 
                            uint8_t *value)
{
    // uint32_t val = 0;
    time_t time_sec;
    struct tm time_bd;
    char buffer[256];

    /* Get real time  in seconds */
    time_sec = time(NULL);
    /* Break down time representation */
    gmtime_r(&time_sec, &time_bd);

    strftime(buffer, 256, "%c", &time_bd);
    // dbg_more("%s: read time = %s", __func__, buffer);


    switch(offset) {
        case LUPIO_RTC_SECD:
            *value = time_bd.tm_sec;                 /* 0-60 (for leap seconds) */
            return true;
        case LUPIO_RTC_MINT:
            *value = time_bd.tm_min;                 /* 0-59 */
            return true;
        case LUPIO_RTC_HOUR:
            *value = time_bd.tm_hour;                /* 0-23 */
            return true;
        case LUPIO_RTC_DYMO:
            *value = time_bd.tm_mday;                /* 1-31 */
            return true;
        case LUPIO_RTC_MNTH:
            *value = time_bd.tm_mon + 1;             /* 1-12 */
            return true;
        case LUPIO_RTC_YEAR:
            *value = (time_bd.tm_year + 1900) % 100; /* 0-99 */
            return true;
        case LUPIO_RTC_CENT:
            *value = (time_bd.tm_year + 1900) / 100; /* 0-99 */
            return true;
        case LUPIO_RTC_DYWK:
            *value = 1 + (time_bd.tm_wday + 6) % 7;  /* 1-7 (Monday is 1) */
            return true;
        case LUPIO_RTC_DYYR:
            *value = time_bd.tm_yday + 1;            /* 1-366 (for leap years) */
            return true;

        default:
            printf("%d: invalid read access at offset = " , offset);
            return true;
    }

    return true;
}

void lupio_rtc_write(vm_t *vm,
                    rtc_states *rtcState,
                    uint32_t addr,
                    uint8_t width,
                    uint32_t value)
{
    switch (width) {
    case RV_MEM_SB:
        lupio_rtc_reg_write(rtcState, addr, value);
        return;
    case RV_MEM_SW:
    case RV_MEM_SH:
        vm_set_exception(vm, RV_EXC_STORE_MISALIGN, vm->exc_val);
        return;
    default:
        vm_set_exception(vm, RV_EXC_ILLEGAL_INSTR, 0);
        return;
    }
}

void lupio_rtc_reg_write(rtc_states *rtcState, 
                            uint32_t offset, 
                            uint8_t value)
{
    return;
}


// static struct iomem_dev_ops lupio_rtc_ops = {
//     .read = lupio_rtc_reg_read,
//     .write = lupio_rtc_write,
//     .flags = IOMEM_DEV_SIZE8,
// };

// void lupio_rtc_init(phys_addr_t base, phys_addr_t size)
// {
//     assert(size >= LUPIO_RTC_OFFSET_MAX);

//     iomem_register_device(base, size, NULL, &lupio_rtc_ops);
// }
