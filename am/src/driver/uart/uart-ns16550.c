#include ISA_H

#include BOARD_H
#include <driver/uart-ns16550.h>

#define WATCHDOG_RESET() {}
#define UART_LCRVAL UART_LCR_8N1		/* 8 data, 1 stop, no parity */
#define UART_MCRVAL (UART_MCR_DTR | \
		     UART_MCR_RTS)		/* RTS/DTR */

#define serial_out(x, y)	outb((uintptr_t)y, x)
#define serial_in(y)		inb((uintptr_t)y)

static void ns16550_setbrg(struct ns16550 *com_port, int baud_divisor)
{
	/* to keep serial format, read lcr before writing BKSE */
	int lcr_val = serial_in(&com_port->lcr) & ~UART_LCR_BKSE;

	serial_out(UART_LCR_BKSE | lcr_val, &com_port->lcr);
	serial_out(baud_divisor & 0xff, &com_port->dll);
	serial_out((baud_divisor >> 8) & 0xff, &com_port->dlm);
	serial_out(lcr_val, &com_port->lcr);
}

static unsigned int ns16550_getfcr(struct ns16550 *port)
{
	struct ns16550_plat *plat = port->plat;

	return plat->fcr;
}

void ns16550_init(struct ns16550 *com_port, int baud_divisor)
{
	while (!(serial_in(&com_port->lsr) & UART_LSR_TEMT));

	serial_out(CONFIG_SYS_NS16550_IER, &com_port->ier);

	serial_out(UART_MCRVAL, &com_port->mcr);
	serial_out(ns16550_getfcr(com_port), &com_port->fcr);
	/* initialize serial config to 8N1 before writing baudrate */
	serial_out(UART_LCRVAL, &com_port->lcr);
	if (baud_divisor != -1)
		ns16550_setbrg(com_port, baud_divisor);
}

void ns16550_putc(struct ns16550 *com_port, char c)
{
	while ((serial_in(&com_port->lsr) & UART_LSR_THRE) == 0);
	serial_out(c, &com_port->thr);

	/*
	 * Call watchdog_reset() upon newline. This is done here in putc
	 * since the environment code uses a single puts() to print the complete
	 * environment upon "printenv". So we can't put this watchdog call
	 * in puts().
	 */
	if (c == '\n')
		WATCHDOG_RESET();
}

char ns16550_getc(struct ns16550 *com_port)
{
	while ((serial_in(&com_port->lsr) & UART_LSR_DR) == 0) {
		WATCHDOG_RESET();
	}
	return serial_in(&com_port->rbr);
}