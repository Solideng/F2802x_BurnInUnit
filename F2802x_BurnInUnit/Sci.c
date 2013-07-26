/*
 * Sci.c
 *
 *  Created on: 3 May 2012
 *      Author: Toby
 */

#include "Common.h"

static interrupt void sciRxIsr(void);
static interrupt void sciTxIsr(void);

// TODO change sciEchoEnable to be a SCPI register bit
static Uint16 sciEchoEnable = 0;		/* Sets the SCI to echo any input instead of doing anything else with it. */

Uint16 sciInit (Uint32 baud) {
	/* Initialise the SCI-A module for using FIFOs. */
	double brr = 0;

	if (baud < SCIBAUD_MIN)
		return VALUE_OOB;

	EALLOW;								/* Map SCI interrupts to ISR functions. */
	PieVectTable.SCIRXINTA = &sciRxIsr;
	PieVectTable.SCITXINTA = &sciTxIsr;
	EDIS;
	SciaRegs.SCICCR.all = 0x0007;		/* 1 Stop bit, no loopback no parity, 8 char bits, asynchronous mode,
										 * idle-line protocol.
										 */
	#ifdef ENABLE_SCI_FIFO_LOOPBACK		/* FOR TESTING - Define ENABLE_SCI_FIFO_LOOPBACK. */
		SciaRegs.SCICCR.bit.LOOPBKENA = 1;
	#endif
	SciaRegs.SCICTL1.all = 0x03;		/* Enable TX, RX, internal SCICLK, disable RX ERR, SLEEP, TXWAKE. */

	brr = baud * 8;						/* Calculate BRR. */
	brr = (15000000 / brr) - 1;			/* BRR = (LSPCLK / [BAUD x 8]) - 1, where LSPCLK = 15 MHz @ 60 MHz SYSCLK. */
	baud = (Uint16) (brr + 0.5);		/* Get |BRR|. */
										/* Assign |BRR| high and low byte to respective registers. */
	SciaRegs.SCIHBAUD = (baud & 0xFF00) > 8;
	SciaRegs.SCILBAUD = (baud & 0xFF);
										/* Hold SCI in reset, enable FIFO, hold FIFO in reset, clear FIFO status,
										 * clear interrupt, enable interrupt, set interrupt level to SCIFFTX_INT_LVL.
										 */
	SciaRegs.SCIFFTX.all = 0xC060 | SCIFFTX_INT_LVL;
										/* Clear overflow flag, hold FIFO in reset, clear FIFO status, clear
										 * interrupt, enable interrupt, set interrupt level to SCIFFRX_INT_LVL.
										 */
	SciaRegs.SCIFFRX.all = 0x4060 | SCIFFRX_INT_LVL;
	SciaRegs.SCIFFCT.all = 0x00;		/* Disable auto-baud feature and set transmit delay to 0. */

	SciaRegs.SCICTL1.bit.SWRESET = 1;	/* Relinquish SCI and FIFOs from Reset. */
	SciaRegs.SCICTL1.all = 0x0023;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	/* Enable the PIE block. */
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;	/* PIE Group 9, INT1. */
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;	/* PIE Group 9, INT2. */
	IER |= M_INT9;						/* Enable INT 9 group in IER. */
	return 0;
}

void sciTx (void) {
	/* Pop data from the SCPI output queue and start transmitting it. */
	Uint16 popResult = 0;
	char dataByte = 0;
										/* Fill the FIFO until it is full or the SCPI output queue is empty. */
	while ((SciaRegs.SCIFFTX.bit.TXFFST < SCIFFTX_FILL_LVL) && (popResult == 0)) {
		msgs.flag.brq = 1;				/* Make sure brq is set, as if the FIFO takes more than 1 byte the
										 * pop is called without first running the response state code.
										 */
		popResult = popOQueue(&dataByte);
		if (popResult == 0)
			SciaRegs.SCITXBUF = (Uint16) dataByte;
	}
	SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;/* Clear SCIA FIFO transmit interrupt flag. */
}

static interrupt void sciTxIsr(void) {
	/* SCI interrupt (SCITXINTA) indicating the SCI is ready to accept more data. */
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;		/* Acknowledge interrupt in PIE. */
}

static interrupt void sciRxIsr (void) {
	/* SCI interrupt (SCIRXINTA) indicating the SCI has received some data. */
    Uint16 pushResult = 0;
    char buf[2] = {0};

    if (!sciEchoEnable) {				/* Check that SCI is not set to echo. */
										/* Check if there is data remaining on the receive FIFO and that the last
										 * push did not result in an error.
										 */
		while ((SciaRegs.SCIFFRX.bit.RXFFST > 0) && (pushResult == 0)) {
										/* Cast the next word on the receive buffer to a character. */
			buf[0] = (char) SciaRegs.SCIRXBUF.bit.RXDT;
			pushResult = pushIBuff(buf);/* Push the character onto the SCPI input buffer. */
		}
    } else {							/* Check if there is data remaining in the RX FIFO and there is space
										 * remaining in the TX FIFO.
										 */
    	while ((SciaRegs.SCIFFRX.bit.RXFFST > 0) && (SciaRegs.SCIFFTX.bit.TXFFST < SCIFFTX_FILL_LVL))
    									/* Copy the words from the RX FIFO into the TX FIFO. */
    		SciaRegs.SCITXBUF = SciaRegs.SCIRXBUF.bit.RXDT;
    }
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;	/* Clear Overflow flag. */
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;	/* Clear Interrupt flag. */
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;	/* Acknowledge interrupt in PIE. */
}
