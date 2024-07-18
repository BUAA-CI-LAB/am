#ifndef __MEGA_H__
#define __MEGA_H__

#include <klib-macros.h>

#include ISA_H // "x86.h", "mips32.h", ...

// This file contains the megaSoC device address definition & some device config

// spi control
#define SPI_CTRL_BASE_PADDR      (0x1d000000)

// uart addr
#define UART_BASE_PADDR              (0x1d010000)
#define UART_BAUD                   (6250000)
#define UART_FREQ                   (100000000)

// I2C Master
#define I2CM_BASE_PADDR              (0x1d020000)

// cdbus
#define CDBUS_BASE_PADDR             (0x1d030000)

// confreg
#define CONFREG_BASE_PADDR        (0x1d040000)
#define CONFREG_TIMER_PADDR          (CONFREG_BASE_PADDR + 0xe000)
#define CONFREG_VIRTUAL_UART_PADDR   (CONFREG_BASE_PADDR + 0xff10)
#define CONFREG_FINISH_PADDR         (CONFREG_BASE_PADDR + 0xff20)

// Ethernet
#define ETHERNET_BASE_PADDR          (0x1d050000)

// interrupt controller
#define INTC_BASE_PADDR              (0x1d060000)

// sd controller
#define SDC_BASE_PADDR               (0x1d070000)

// usb controller
#define USBC_BASE_PADDR              (0x1d100000)


// memory virtual address range, initialize in _dmw_init()
#define MEM_BASE_PADDR              (0x0)
#define MEM_BASE_VADDR              (0x80000000)
#define MEM_SIZE_BYTES              (0x8000000) // 128 MB


#endif
