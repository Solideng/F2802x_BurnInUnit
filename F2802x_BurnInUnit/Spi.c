/*
 * Sci.c
 *
 *  Created on: 1 Oct 2013
 *      Author: Toby
 */

#include "Common.h"


// Example code for setting up external interrupt ISR-------------------

// xint on gpio7 to detect if slave or master mode
// master if HIGH, slave if LOW
// mode may be selected before or after startup/reset
//  maybe have the int falling edge activated,
//  but also run a check on startup for preset mode level
//  could also have the ISR change the int settings to rising edge
//  to detect if slave mode is removed (changed back to master)

//static void tmpAlrtInit(void) {
//	// Setup temperature external interrupt on GPIO-06 from LM73 ALERT
//	// pin to indicate if temperature is above the user set limit
//	EALLOW;
//	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;		// Pull up enabled
//	GpioIntRegs.GPIOXINT1SEL.all = 0x07;	// Select GPIO7 as external interrupt 1
//	PieVectTable.XINT1 = &tmpAlrt_isr;		// Map int to isr
//	EDIS;
//	XIntruptRegs.XINT1CR.bit.POLARITY = 0;	// Cfg XINT1 as falling edge activated
//	XIntruptRegs.XINT1CR.bit.ENABLE = 1;	// Enable XINT1
//	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		// Enable the PIE block
//	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;		// Enable in PIE Group 1 - INT5
//	IER |= M_INT1;							// Enable CPU INT1
//}
// End of example code--------------------------------------------------

static interrupt void spiTxFifoIsr (void);
static interrupt void spiRxFifoIsr (void);

Uint16 spiInit(spiMode mode, Uint32 baud, spiLpbk loopback, spiCPol cPol, spiCPha cPha) {
	// TODO: Enable TALK??
	float32 spiBrr = (float32)baud;
	Uint16 temp = 7;				/* Init the config control reg (SPICCR) value. Reset on, 8-bit char length */
	temp |= ((Uint16)cPol) << 6;	/* Add the clock polarity setting to the SPICCR value. */
	temp |= ((Uint16)loopback) << 4;/* Add the loop-back setting to the SPICCR value. */
	SpiaRegs.SPICCR.all = temp;		/* Save the SPICCR value to the reg. */

	temp = 0x01;					/* Init the operation control reg (SPICTL) value. Enable SPI interrupts. */
	temp |= ((Uint16)cPha) << 3;	/* Add the clock phase setting to the SPICTL value. */
	temp |= ((Uint16)mode) << 2;	/* Add the mode setting to the SPICTL value. */
	SpiaRegs.SPICTL.all = temp;		/* Save the SPICTL value to the reg. */

									/* Use the requested baud rate and LSPCLK frequency to calculate the
									 * SPI bit rate control value.
									 * Baud rate = (LSPCLK / [SPIBRR + 1]).
									 */
	spiBrr = (LOSPCLK_FREQ_SET / baud) - 1;

	if ((spiBrr < 4) || (spiBrr > 127))	/* Ensure SPIBRR value is within bounds */
			return VALUE_OOB;

	SpiaRegs.SPIBRR = (Uint16) spiBrr;	/* Save the |SPIBRR| value to the reg. */

									/* Set Tx FIFO. Reset channels, FIFOs enabled, FIFO in reset, clear any
									 * interrupt, enable interrupt and add interrupt level.
									 */
	SpiaRegs.SPIFFTX.all = 0x4060 | SPI_FFTX_INTLVL;
									/* Set Rx FIFO. Clear overflow, FIFO in reset, clear any interrupt, enable
									 * interrupt and add interrupt level.
									 */
	SpiaRegs.SPIFFRX.all = 0x4040 | SPI_FFRX_INTLVL;
	SpiaRegs.SPIFFCT.all = 0x00;	/* Set 0 TxFIFO delay. */
	SpiaRegs.SPIPRI.all = 0x00;		/* Set free-run and 4-wire mode. */

	//TODO: set up the interrupts in PIE/IER and assign ISRs

	return 0;
}

void spiTx(void) {

}

static interrupt void spiTxFifoIsr (void) {

}

static interrupt void spiRxFifoIsr (void) {

}

//void spiTx(Uint16 txData) {
//	// Transmit data on SPI, uses  polling till transmission is finished
//	Uint16 dummy;
//	SpiaRegs.SPICTL.bit.TALK = 1;				// Enable Tx path
//	SpiaRegs.SPITXBUF = txData;					// Copy data to Tx buffer
//	while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}// Wait until rx'd
//	dummy = SpiaRegs.SPIRXBUF;					// Clears junk data
//	dummy = dummy;
//}

/*interrupt void spiTxFifoIsr(void) {
	Uint16 i;
    for(i=0;i<SPI_TX_LENGTH;i++)
    {
 	   SpiaRegs.SPITXBUF=spiTxBuf[i];      // Send data
    }
    SpiaRegs.SPIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x20;  		// Issue PIE ACK
	//if transmission finished, disable Tx. 
}
*/
