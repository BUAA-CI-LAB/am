#ifndef __EULASOC_H__
#define __EULASOC_H__

#include <klib-macros.h>

#include ISA_H // "x86.h", "mips32.h", ...

#define CONFREG_BASE_PADDR        (0xbfaf0000)
#define CONFREG_CR0_PADDR            (CONFREG_BASE_PADDR + 0x8000)
#define CONFREG_CR1_PADDR            (CONFREG_BASE_PADDR + 0x8010)
#define CONFREG_CR2_PADDR            (CONFREG_BASE_PADDR + 0x8020)
#define CONFREG_CR3_PADDR            (CONFREG_BASE_PADDR + 0x8030)
#define CONFREG_CR4_PADDR            (CONFREG_BASE_PADDR + 0x8040)
#define CONFREG_CR5_PADDR            (CONFREG_BASE_PADDR + 0x8050)
#define CONFREG_CR6_PADDR            (CONFREG_BASE_PADDR + 0x8060)
#define CONFREG_CR7_PADDR            (CONFREG_BASE_PADDR + 0x8070)
#define CONFREG_LED_PADDR            (CONFREG_BASE_PADDR + 0xf020)   //32'hbfaf_f020 
#define CONFREG_LED_RG0_PADDR        (CONFREG_BASE_PADDR + 0xf030)  //32'hbfaf_f030 
#define CONFREG_LED_RG1_PADDR        (CONFREG_BASE_PADDR + 0xf040)   //32'hbfaf_f040 
#define CONFREG_NUM_PADDR            (CONFREG_BASE_PADDR + 0xf050)   //32'hbfaf_f050 
#define CONFREG_SWITCH_PADDR         (CONFREG_BASE_PADDR + 0xf060)   //32'hbfaf_f060 
#define CONFREG_BTN_KEY_PADDR        (CONFREG_BASE_PADDR + 0xf070)   //32'hbfaf_f070
#define CONFREG_BTN_STEP_PADDR       (CONFREG_BASE_PADDR + 0xf080)   //32'hbfaf_f080
#define CONFREG_SW_INTER_PADDR       (CONFREG_BASE_PADDR + 0xf090)   //32'hbfaf_f090 
#define CONFREG_TIMER_PADDR          (CONFREG_BASE_PADDR + 0xe000)   //32'hbfaf_e000

#define CONFREG_FREQ_PADDR           (0xbfd0f030)   //32'hbfd0_f030

#define CONFREG_IO_SIMU_PADDR        (CONFREG_BASE_PADDR + 0xff00)   //32'hbfaf_ff00
#define CONFREG_VIRTUAL_UART_PADDR   (CONFREG_BASE_PADDR + 0xff10)   //32'hbfaf_ff10
#define CONFREG_SIMU_FLAG_PADDR      (CONFREG_BASE_PADDR + 0xff20)   //32'hbfaf_ff20 
#define CONFREG_OPEN_TRACE_PADDR     (CONFREG_BASE_PADDR + 0xff30)   //32'hbfaf_ff30
#define CONFREG_NUM_MONITOR_PADDR    (CONFREG_BASE_PADDR + 0xff40)   //32'hbfaf_ff40


#endif
