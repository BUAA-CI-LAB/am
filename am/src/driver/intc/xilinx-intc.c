#include BOARD_H
#include <mmio.h>
#include <driver/xilinx-intc.h>

#define ISR 0x00			/* Interrupt Status Register */
#define IPR 0x04			/* Interrupt Pending Register */
#define IER 0x08			/* Interrupt Enable Register */
#define IAR 0x0c			/* Interrupt Acknowledge Register */
#define SIE 0x10			/* Set Interrupt Enable bits */
#define CIE 0x14			/* Clear Interrupt Enable bits */
#define IVR 0x18			/* Interrupt Vector Register */
#define MER 0x1c			/* Master Enable Register */

#define MER_ME (1<<0)
#define MER_HIE (1<<1)

static void xintc_write(int reg, unsigned int data)
{
	outb(INTC_BASE_PADDR + reg, data);
}

static unsigned int xintc_read(int reg)
{
	return inb(INTC_BASE_PADDR + reg);
}

void intc_ack(int irq) {
	xintc_write(IAR, BIT(irq));
}

unsigned int xintc_get_irq() {
	unsigned int irq = -1;
	unsigned int hwirq;
	hwirq = xintc_read(IVR);
    // TODO make sure irq number is correct.
	if (hwirq != -1U)
		irq = hwirq;
	return irq;
}

void xilinx_intc_of_init() {
	/*
	 * Disable all external interrupts until they are
	 * explicitly requested.
	 */
	xintc_write(IER, 0);

	/* Acknowledge any pending interrupts just in case. */
	xintc_write(IAR, 0xffffffff);

	/* Turn on the Master Enable. */
	xintc_write(MER, MER_HIE | MER_ME);
	
	// Enable UART Interrupt Response Only.
	xintc_write(IER, 0x4);
}