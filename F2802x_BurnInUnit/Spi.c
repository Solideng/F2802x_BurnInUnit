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
static interrupt void startSpiRxFifoIsr (void);

// TODO change sciEchoEnable to be a SCPI register bit
static Uint16 spiEchoEnable = 0;		/* Sets the SPI to echo any input instead of doing anything else with it. */

Uint16 spiInit(spiMode mode, Uint32 baud, spiLpbk loopback, transPol cPol, spiCPha cPha) {
	// TODO: Enable TALK??
	float32 spiBrr = (float32)baud;
	Uint16 temp = 7;				/* Init the config control reg (SPICCR) value. Reset on, 8-bit char length */
	temp |= ((Uint16) cPol) << 6;	/* Add the clock polarity setting to the SPICCR value. */
	temp |= ((Uint16) loopback) << 4;/* Add the loop-back setting to the SPICCR value. */
	SpiaRegs.SPICCR.all = temp;		/* Save the SPICCR value to the register. */

	temp = 0x01;					/* Init the operation control reg (SPICTL) value. Enable SPI interrupts. */
	temp |= ((Uint16) cPha) << 3;	/* Add the clock phase setting to the SPICTL value. */
	temp |= ((Uint16) mode) << 2;	/* Add the mode setting to the SPICTL value. */
	SpiaRegs.SPICTL.all = temp;		/* Save the SPICTL value to the register. */

									/* Use the requested baud rate and LSPCLK frequency to calculate the
									 * SPI bit rate control value.
									 * Baud rate = (LSPCLK / [SPIBRR + 1]).
									 */
	spiBrr = (LOSPCLK_FREQ_SET / baud) - 1;

	if ((spiBrr < 4) || (spiBrr > 127))	/* Ensure SPIBRR value is within bounds */
			return VALUE_OOB;

	SpiaRegs.SPIBRR = (Uint16) spiBrr;	/* Save the |SPIBRR| value to the register. */

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

	EALLOW;
	PieVectTable.SPIRXINTA = &spiRxFifoIsr;
	PieVectTable.SPITXINTA = &spiTxFifoIsr;
	EDIS;

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	/* Enable the PIE block. */
	PieCtrlRegs.PIEIER6.bit.INTx1 = 1;	/* PIE Group 6, INT1. */
	PieCtrlRegs.PIEIER6.bit.INTx2 = 1;	/* PIE Group 6, INT2. */

	if (mode == spiMaster) {
		EALLOW;
									/* Set GPIO6 as input. */
		GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;
									/* Enable pull up for slave SRQ (GPIO6) signal. */
		GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;
									/* Select GPIO6 as external interrupt 2. */
		GpioIntRegs.GPIOXINT2SEL.all = 0x06;
									/* Map interrupts to ISR functions. */
		PieVectTable.XINT2 = &startSpiRxFifoIsr;
		EDIS;

									/* Set external interrupt 2 as falling edge activated. */
		XIntruptRegs.XINT2CR.bit.POLARITY = (Uint16) fallingEdge;
									/* Enable external interrupt 2. */
		XIntruptRegs.XINT2CR.bit.ENABLE = 1;
									/* PIE Group 1, INT5. */
		PieCtrlRegs.PIEIER1.bit.INTx5 = 1;
		IER |= M_INT1;				/* Enable INT 1 group in IER. */

	} else {
		EALLOW;
									/* Set GPIO6 as output. */
		GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;
									/* Set GPIO6 output HIGH initially. */
		GpioDataRegs.GPASET.bit.GPIO6 = 1;
		EDIS;

	}


	IER |= M_INT6;						/* Enable INT 6 group in IER. */

	/* Relinquish SPI and FIFOs from reset. */
	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;
	SpiaRegs.SPIFFTX.bit.TXFIFO = 1;
	SpiaRegs.SPIFFTX.bit.SPIRST = 1;
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;
	return 0;
}

void spiTx(void) {
	/* Pop data from the SCPI output queue and start transmitting it. */
	Uint16 popResult = 0;
	char dataByte = 0;

	SpiaRegs.SPICTL.bit.TALK = 1;		/* Enable talk. */
										/* Fill the FIFO until it is full or the SCPI output queue is empty. */
	while ((SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL) && (popResult == 0)) {
		msgs.flag.brq = 1;				/* Make sure brq is set, as if the FIFO takes more than 1 byte the
										 * pop is called without first running the response state code.
										 */
		popResult = popOQueue(&dataByte);
		if (popResult == 0)
			SpiaRegs.SPITXBUF = ((Uint16) dataByte) << 8;
	}
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;/* Clear SPIA FIFO transmit interrupt flag. */
}

static interrupt void spiTxFifoIsr (void) {
	/* SPI interrupt (SPITXINTA) indicating the SPI is ready to accept more data. */
	SpiaRegs.SPICTL.bit.TALK = 0;		/* Disable talk. */
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;	/* Acknowledge interrupt in PIE. */
}

static interrupt void startSpiRxFifoIsr (void) {

	// TODO: .... check e2e.ti for how to start SPI 'read'

	PieCtrlRegs.PIEACK.bit.ACK1 = 1;	/* Acknowledge interrupt in PIE. */
}

static interrupt void spiRxFifoIsr (void) {
	/* SPI interrupt (SPIRXINTA) indicating the SPI has received some data. */
    Uint16 pushResult = 0;
    char buf[2] = {0};

    if (!spiEchoEnable) {				/* Check that SPI is not set to echo. */
										/* Check if there is data remaining on the receive FIFO and that the last
										 * push did not result in an error.
										 */
		while ((SpiaRegs.SPIFFRX.bit.RXFFST > 0) && (pushResult == 0)) {
										/* Cast the next word on the receive buffer to a character. */
			buf[0] = (char) SpiaRegs.SPIRXBUF;
			pushResult = pushIBuff(buf);/* Push the character onto the SCPI input buffer. */
		}
    } else {							/* Check if there is data remaining in the RX FIFO and there is space
										 * remaining in the TX FIFO.
										 */
    	while ((SpiaRegs.SPIFFRX.bit.RXFFST > 0) && (SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL))
    									/* Copy the words from the RX FIFO into the TX FIFO. */
    		SpiaRegs.SPITXBUF = SpiaRegs.SPIRXBUF;
    }
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1;	/* Clear Interrupt flag. */
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;	/* Acknowledge interrupt in PIE. */
}
