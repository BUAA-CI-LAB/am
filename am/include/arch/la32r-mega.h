#ifndef __ARCH_H__

// struct _Context {
//   unsigned int dummy;
// };

#define GPRx dummy

/* BootROM + MIG is pretty smart. DDR and Cache initialized */
#define CONFIG_SKIP_LOWLEVEL_INIT

// #define DEBUG

/*--------------------------------------------
 * CPU configuration
 */
/* CPU Timer rate */
#define CONFIG_SYS_MIPS_TIMER_FREQ	150000000

/* Cache Configuration */
#define CONFIG_SYS_MIPS_CACHE_MODE	CONF_CM_CACHABLE_NONCOHERENT

/* Time Measuring */
#define TRIVIALMIPS_NSCSCC_TIMER_ADDR		0x9fd0e000

/*----------------------------------------------------------------------
 * Memory Layout
 */

/* SDRAM Configuration (for final code, data, stack, heap) */
#define CONFIG_SYS_SDRAM_BASE		0xA0000000
#define CONFIG_SYS_SDRAM_SIZE		0x04000000	/* 128 Mbytes */
#define CONFIG_SYS_INIT_SP_ADDR		\
	(CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_SDRAM_SIZE - 0x1000)

// #define CONFIG_SYS_MEMTEST_START    0xA0000000
// #define CONFIG_SYS_MEMTEST_END      0x04000000

// #define CONFIG_SYS_MALLOC_LEN		(256 << 10)
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/*----------------------------------------------------------------------
 * Commands
 */
//#define CONFIG_SYS_LONGHELP		/* undef to save memory */
//#define CONFIG_CMDLINE_EDITING

/*------------------------------------------------------------
 * Console Configuration
 */
#define CONFIG_SYS_CBSIZE		1024 /* Console I/O Buffer Size   */
#define CONFIG_SYS_MAXARGS		16   /* max number of command args*/


// serial configuration
#define CONFIG_CONS_INDEX 1
#define CONFIG_SYS_NS16550_COM1 0x9d010000
#define CONFIG_SYS_NS16550_CLK 100000000
#define CONFIG_SYS_NS16550_IER 0


/* -------------------------------------------------
 * Environment
 */
//Disable persistent environment variable storage
#define CONFIG_ENV_IS_NOWHERE   1 


#define CONFIG_EXTRA_ENV_SETTINGS \
    "autoload=no\0" \
    "serverip=192.168.1.141\0" \
    "ipaddr=192.168.1.70\0" \
    "netmask=255.255.255.0\0" \
    "ethaddr=19:98:00:01:00:29\0"



/* ---------------------------------------------------------------------
 * Board boot configuration
 */

#define CONFIG_TIMESTAMP	/* Print image info with timestamp */


/* Flash */
#define CONFIG_SYS_MAX_FLASH_BANKS_DETECT   1
#define CONFIG_SYS_MAX_FLASH_SECT           64

#define CONFIG_LA32R
#define CONFIG_SYS_MALLOC_F_LEN (0x600)
#define CONFIG_DM_GPIO
#define CONFIG_DEFAULT_DEVICE_TREE "la32rmega_demo"
#define CONFIG_DEBUG_UART_BASE (0x9d010000)
#define CONFIG_DEBUG_UART_CLOCK (50000000)
#define CONFIG_TARGET_LA32RMEGA_DEMO
#define CONFIG_DEBUG_UART
#define CONFIG_SYS_LOAD_ADDR (0xA0000000)
#define CONFIG_BOOTDELAY (1)
#define CONFIG_USE_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "fatload mmc 0 0xa2000000 vmlinux;bootelf 0xa2000000 g console=ttyS0,230400 root=/dev/mmcblk0p2 rootfstype=ext4 rw rootwait loglevel=9 uio_pdrv_genirq.of_id=generic-uio"
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO_LATE
#define CONFIG_HUSH_PARSER
#define CONFIG_SYS_PROMPT "EuLA Boot@LainSoC# "
// CONFIG_CMD_BDI is not set
// CONFIG_CMD_CONSOLE is not set
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_BOOTMENU
// CONFIG_CMD_EXPORTENV is not set
// CONFIG_CMD_IMPORTENV is not set
#define CONFIG_CMD_MEMTEST
#define CONFIG_CMD_DM
#define CONFIG_CMD_MMC
#define CONFIG_CMD_MMC_SWRITE
#define CONFIG_CMD_SPI
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_TFTPSRV
// CONFIG_CMD_NFS is not set
#define CONFIG_CMD_PING
#define CONFIG_CMD_GETTIME
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_OF_CONTROL
#define CONFIG_OF_EMBED
#define CONFIG_NET_RANDOM_ETHADDR
#define CONFIG_NETCONSOLE
#define CONFIG_DM
#define CONFIG_CLK
#define CONFIG_MMC
#define CONFIG_MMC_OCSDC_AXI
// CONFIG_MMC_HW_PARTITIONING is not set
#define CONFIG_DM_ETH
#define CONFIG_ETHOC
#define CONFIG_XILINX_EMACLITE
#define CONFIG_BAUDRATE (230400)
#define CONFIG_DM_SERIAL
#define CONFIG_DEBUG_UART_ANNOUNCE
#define CONFIG_SYS_NS16550
#define CONFIG_SPI
#define CONFIG_DM_SPI
#define CONFIG_SPI_MEM
#define CONFIG_LOONGSON_SPI
#define CONFIG_USB
#define CONFIG_USB_DWC2
#define CONFIG_USB_DWC2_BUFFER_SIZE (8)
#define CONFIG_CMD_DHRYSTONE
#define CONFIG_SYS_ICACHE_LINE_SIZE (4)
#define CONFIG_SYS_DCACHE_LINE_SIZE (4)

#endif
