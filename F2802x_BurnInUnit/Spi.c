/*
 * Sci.c
 *
 *  Created on: 1 Oct 2013
 *      Author: Toby
 */

#include "Common.h"

// xint on gpio7 to detect if slave or master mode
// master if HIGH, slave if LOW
// mode may be selected before or after startup/reset
//  maybe have the int falling edge activated,
//  but also run a check on startup for preset mode level
//  could also have the ISR change the int settings to rising edge
//  to detect if slave mode is removed (changed back to master).

static interrupt void spiTxFifoIsr (void);
static interrupt void spiRxFifoIsr_Slave (void);
static interrupt void startSpiRxFifoIsr_Master (void);
//static interrupt void doNothingIsr (void);
//static void disableXInt2 (void);

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


	if (mode == spiMaster) {
		EALLOW;
		GpioCtrlRegs.GPADIR.bit.GPIO6 	= 0;	/* Set GPIO6 as input */
		GpioCtrlRegs.GPAPUD.bit.GPIO6 	= 1;	/* Enable pull up for GPIO6 */
		GpioIntRegs.GPIOXINT2SEL.all 	= 0x06;	/* Select GPIO6 as external interrupt 2 */

		PieVectTable.SPIRXINTA = &spiRxFifoIsr_Master;	/* Set ISR for SPI Rx interrupt */
		PieVectTable.SPITXINTA = &spiTxFifoIsr_Master;	/* Set ISR for SPI Tx interrupt */
		PieVectTable.XINT2 = &startSpiRxFifoIsr_Master;	/* Set ISR for XINT2 (Slave SRQ) */
		EDIS;

		XIntruptRegs.XINT2CR.bit.POLARITY = (Uint16) fallingEdge;	/* Set external interrupt 2 as falling edge activated */
		XIntruptRegs.XINT2CR.bit.Enable = 1;	/* Enable external interrupt 2. */

		PieCtrlRegs.PIECTRL.bit.ENPIE  = 1;	/* Enable the PIE block */
		PieCtrlRegs.PIEIER1.bit.INTx5  = 1;	/* Enable PIE group 1, INT 5, XINT2 */
		IER  |= M_INT1;	/* Enable group 1 in IER */
	} else {
		EALLOW;
		GpioCtrlRegs.GPADIR.bit.GPIO6 	= 1;	/* Set GPIO6 as output */
		GpioDataRegs.GPASET.bit.GPIO6 	= 1;	/* Set GPIO6 to output HIGH initially */

		PieVectTable.SPIRXINTA = &spiRxFifoIsr_Slave;	/* Set ISR for SPI Rx interrupt */
		PieVectTable.SPITXINTA = &spiTxFifoIsr_Slave;	/* Set ISR for SPI Tx interrupt */
		EDIS;
		PieCtrlRegs.PIECTRL.bit.ENPIE  = 1;	/* Enable the PIE block */
	}
	PieCtrlRegs.PIEIER6.bit.INTx1  = 1;	/* Enable PIE group 6, INT 1, SPIRXINTA */
	PieCtrlRegs.PIEIER6.bit.INTx2  = 1;	/* Enable PIE group 6, INT 2, SPITXINTA */
	IER  |= M_INT6;	/* Enable group 6 in IER */

	/* Relinquish SPI and FIFOs from reset. */
	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;
	SpiaRegs.SPIFFTX.bit.TXFIFO = 1;
	SpiaRegs.SPIFFTX.bit.SPIRST = 1;
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;
	return 0;
}

//static void disableXInt2(void) {
//	/* Disable previously setup or enabled interrupt on external interrupt 2. */
//	/* DISCARDS ANY PENDING INTERRUPTS */
//	DINT;									/* Disable global interrupts (INTM = 1). */
//	EALLOW;									/* Set EALLOW. */
//	PieVectTable.XINT2 = &doNothingIsr;		/* Map interrupt to empty ISR. */
//	XIntruptRegs.XINT2CR.bit.ENABLE = 0;	/* Disable external interrupt 2. */
//	EINT;									/* Enable global interrupts (INTM = 0). */
//	DELAY_US(2);							/* Allow time for any pending interrupt to be serviced
//											 *  by the doNothingIsr.
//											 */
//	DINT;									/* Disable global interrupts (INTM = 1). */
//	PieVectTable.XINT2 = &startSpiRxFifoIsr_Master;/* Map interrupt back to correct ISR. */
//	EDIS;									/* Clear EALLOW. */
//	PieCtrlRegs.PIEIER1.bit.INTx5 = 0;		/* Disable in PIE Group 1, INT5. */
//	PieCtrlRegs.PIEIFR1.bit.INTx5 = 0;		/* Ensure interrupt flag is clear. */
//	PieCtrlRegs.PIEACK.bit.ACK1 = 1;		/* Clear the PIE ACK for this group. */
//	EINT;									/* Enable global interrupts (INTM = 0). */
//}
//
//static interrupt void doNothingIsr (void) {
//	/* Used to sink spurious interrupts when disabling an interrupt. */
//	return;
//}

void spiTx_Master (Uint16 length, char * message) {
	Uint16 popResult = 0;
	SpiaRegs.SPICTL.bit.TALK = 1;	/* Enable talk */

	// as this is the first time Tx-ing for this message...
	// find message length in SCPI OQueue
	// put length into fisrt byte in spiaRegs.SPITXBUF

	while ((SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLLVL) && (popResult == 0)) {
											/* Set BRQ as if the FIFO takes more than one byte the pop will
											 * be called without first running the response state code */
		msgs.flag.brq = 1;
		popResult = popOQueue(&dataByte);	/* Pop a data byte from the SCPI output queue */
		if (!popResult)						/* Check the pop completed correctly */
			SpiaRegs.SPITXBUF = ((Uint16) dataByte) << 8;	/* Add data byte to FIFO buffer */
	}
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;	/* Clear SPIA FIFO Tx interrupt flag */
}

void spiTx_Slave(void) {
	// TODO:....
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
	// TODO:...
	/* SPI interrupt (SPITXINTA) indicating the SPI is ready to accept more data. */
	SpiaRegs.SPICTL.bit.TALK = 0;		/* Disable talk. */
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;	/* Acknowledge interrupt in PIE. */
}

static interrupt void startSpiRxFifoIsr_Master (void) {
	// TODO:...
	// TODO: .... check e2e.ti for how to start SPI 'read'
	// TALK bit?
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;	/* Acknowledge interrupt in PIE. */
}

static interrupt void spiRxFifoIsr_Slave (void) {
	// TODO: ...
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
