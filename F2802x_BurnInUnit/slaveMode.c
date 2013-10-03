/*
 * slaveMode.c
 *
 *  Created on: 3 Oct 2013
 *      Author: Toby
 */

#include "Common.h"

static slaveMode status = master;

static interrupt void modeChangedIsr (void);
static Uint16 changeMode (slaveMode mode);

Uint16 initSlaveModeDetect (void) {
	Uint16 err = 0;
											/* Get current state of GPIO7 and save as current status. */
	status = (slaveMode) GpioDataRegs.GPADAT.bit.GPIO7;
	err = changeMode (status);				/* Setup things that depend on the status. */

	EALLOW;
	GpioIntRegs.GPIOXINT1SEL.all = 0x07;	/* Select GPIO7 as external interrupt 7. */
	PieVectTable.XINT1 = & modeChangedIsr;	/* Map interrupt to ISR function. */
	EDIS;
											/* Set external interrupt 1 as falling and rising edge activated */
	XIntruptRegs.XINT1CR.bit.POLARITY = (Uint16) risingAndFallingEdge;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;	/* Enable external interrupt 1. */

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		/* Enable the PIE block. */
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;		/* PIE Group 1, INT4. */
	IER |= M_INT1;							/* Enable INT 1 group in IER. */
	return err;
}

slaveMode getSlaveMode (void) {
	return status;
}

static interrupt void modeChangedIsr (void) {
	status = (slaveMode) GpioDataRegs.GPADAT.bit.GPIO7;
	changeMode(status);
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;	/* Acknowledge interrupt in PIE. */
}

static Uint16 changeMode (slaveMode mode) {
	Uint16 err = 0;
//gitiignore /scpi
	return err;
}
