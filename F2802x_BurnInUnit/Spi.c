/*
 * Sci.c
 *
 *  Created on: 1 Oct 2013
 *      Author: Toby
 */

#include "Common.h"

static interrupt void spiTxFifoIsr_Master (void);
static interrupt void startSpiRxFifoIsr_Master (void);
static interrupt void spiRxFifoIsr_Master (void);

static interrupt void spiTxFifoIsr_Slave (void);
static interrupt void spiRxFifoIsr_Slave (void);

//static interrupt void doNothingIsr (void);
//static void disableXInt2 (void);

static uint16_t txMsgLen = 0;
static uint16_t txMsgPos = 0;
static uint16_t rxMsgPos = 0;
static char * txMsgLoc = 0;
static char * rxMsgLoc = 0;

// TODO change sciEchoEnable to be a SCPI register bit
static uint16_t spiEchoEnable = 0;		/* Sets the SPI to echo any input instead of doing anything else with it. */

uint16_t spiInit(spiMode mode, uint32_t baud, spiLpbk loopback, transPol cPol, spiCPha cPha) {
	// TODO: Enable TALK??
	float spiBrr = (float)baud;
	uint16_t temp = 7;				/* Init the config control reg (SPICCR) value. Reset on, 8-bit char length */
	temp |= ((uint16_t) cPol) << 6;	/* Add the clock polarity setting to the SPICCR value. */
	temp |= ((uint16_t) loopback) << 4;/* Add the loop-back setting to the SPICCR value. */
	SpiaRegs.SPICCR.all = temp;		/* Save the SPICCR value to the register. */

	temp = 0x03;					/* Init the operation control reg (SPICTL) value. Enable SPI interrupts and TALK. */
	temp |= ((uint16_t) cPha) << 3;	/* Add the clock phase setting to the SPICTL value. */
	temp |= ((uint16_t) mode) << 2;	/* Add the mode setting to the SPICTL value. */
	SpiaRegs.SPICTL.all = temp;		/* Save the SPICTL value to the register. */

									/* Use the requested baud rate and LSPCLK frequency to calculate the
									 * SPI bit rate control value.
									 * Baud rate = (LSPCLK / [SPIBRR + 1]).
									 */
	spiBrr = (LOSPCLK_FREQ_SET / spiBrr) - 1;

	if ((spiBrr < 4) || (spiBrr > 127))	/* Ensure SPIBRR value is within bounds */
			return VALUE_OOB;

	SpiaRegs.SPIBRR = (uint16_t) spiBrr;	/* Save the |SPIBRR| value to the register. */

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

		XIntruptRegs.XINT2CR.bit.POLARITY = (uint16_t) fallingEdge;	/* Set external interrupt 2 as falling edge activated */
		XIntruptRegs.XINT2CR.bit.ENABLE = 1;	/* Enable external interrupt 2. */

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

	txMsgLen = 0; /* Ensure message global variables are clear */
	txMsgPos = 0;
	rxMsgPos = 0;
	txMsgLoc = 0;
	rxMsgLoc = 0;

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

void spiTx_Master (uint16_t length, char * txMessage, char * rxMessage) {
	/* Begins an SPI transmission by a master unit to a slave unit */
	txMsgLoc = txMessage;
	rxMsgLoc = rxMessage;
	txMsgLen = length;
	txMsgPos = 0;
										/* Fill FIFO */
	while ((SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL) && (txMsgPos < length))
		SpiaRegs.SPITXBUF = ((uint16_t) txMsgLoc[txMsgPos++]) << 8;

	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;/* Make sure the SPIA FIFO Tx interrupt flag is clear */
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;	/* Make sure PIE interrupt flag is clear */

	// TODO: If response required wait for it to be completed here...
}

static interrupt void spiTxFifoIsr_Master (void) {
	/* Continues on SPI transition by a master unit to a slave unit */
	if (GpioDataRegs.GPADAT.bit.GPIO6 == 0) { /* Check if SRQ is active */
		if (txMsgPos < txMsgLen) {
												/* Fill FIFO */
			while ((SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL) && (txMsgPos < txMsgLen))
				SpiaRegs.SPITXBUF = ((uint16_t) txMsgLoc[txMsgPos++]) << 8;
			SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;/* Clear SPIA FIFO Tx interrupt flag */
		}
	} else {
		/* If the master is reading it needs to transmit a dummy byte for each byte it wants to receive */

		// TODO: check here if last received byte was end of message?
		// If it was EOM then SRQ would be inactive??? then if would have run instead of else...

		SpiaRegs.SPITXBUF = 0;	/* Place a dummy byte in the Tx buffer */
		SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;/* Clear SPIA FIFO Tx interrupt flag */
	}
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;		/* Acknowledge interrupt in PIE */
}

static interrupt void startSpiRxFifoIsr_Master (void) {
	/* Begins a master unit reading from a slave unit (When SRQ has been activated) */
	rxMsgPos = 0;

	// TODO:...Master function to start a read from a slave

	if (GpioDataRegs.GPADAT.bit.GPIO6)
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;	/* Acknowledge interrupt in PIE */
}

static interrupt void spiRxFifoIsr_Master (void) {
	/* Continues a master unit reading from a slave unit */
	uint16_t pushResult = 0;
							/* Check if there is more data in the Rx FIFO and the previous data was not the end of the message */
	while ((SpiaRegs.SPIFFRX.bit.RXFFST > 0) && (pushResult == 0)) {
		rxMsgLoc[rxMsgPos] = (char) SpiaRegs.SPIRXBUF;	/* Move byte from the Rx FIFO to the response location */
		if (rxMsgLoc[rxMsgPos] == END_MSG_TERM)	/* Check if the byte indicates the end of the message */
			pushResult = 1;	/* Indicate that it is the end of the message */
		else
			rxMsgPos++;	/* Move to the next position on the response location */
	}
	if (pushResult == 0) {	/* Check if an end of message was not detected */
		if (GpioDataRegs.GPADAT.bit.GPIO6 == 0)		/* Check if the SRQ is still active */
			while (SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL) /* Check there is space in the Tx FIFO */
				SpiaRegs.SPITXBUF = END_MSG_TERM;	/* Fill the Tx FIFO with dummy data */
	}
}

void spiTx_Slave (void) {
	/* Begins an SPI transmission by a slave unit to a master unit */
	uint16_t popResult = 0;
	char dataByte = 0;
							/* Fill the FIFO until it is full or the SCPI output queue is empty */
	while ((SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL) && (popResult == 0)) {
		msgs.flag.brq = 1;	/* Make sure brq is set, as if the FIFO takes more than 1 byte the
							 * pop is called without first running the response state code
							 */
		popResult = popOQueue(&dataByte);	/* Pop a byte from the SCPI output queue */
		if (popResult == 0)					/* Check the pop ran OK */
			SpiaRegs.SPITXBUF = ((uint16_t) dataByte) << 8;	/* Left-align the byte and place on the SPI Tx FIFO buffer */
	}
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;	/* Clear SPIA FIFO transmit interrupt flag */
	GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;	/* Activate the SRQ line to the master */
}

static interrupt void spiTxFifoIsr_Slave (void) {
	/* Continues an SPI transmission by a slave unit to a master unit */

	// TODO:... Slave function to place next data in Tx buf or make SRQ inactive if EOM
	/* SPI interrupt (SPITXINTA) indicating the SPI is ready to accept more data */

	SpiaRegs.SPICTL.bit.TALK = 0;		/* Disable talk */
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;	/* Acknowledge interrupt in PIE */
}

static interrupt void spiRxFifoIsr_Slave (void) {
	/* Reads data sent by a master unit for a slave unit */
	uint16_t pushResult = 0;
	char buf[2] = {0};					/* pushIBuff requires a string input */
	if (GpioDataRegs.GPADAT.bit.GPIO6) {/* Check SRQ is not activated as otherwise the received data is just dummy data */
		if (!spiEchoEnable) {			/* Check that SPI is not set to echo */
			while ((SpiaRegs.SPIFFRX.bit.RXFFST > 0) && (pushResult == 0)) {
										/* Read the data from the SPI Rx buffer and push the lower byte to the SCPI input buffer */
				buf[0] = (char) SpiaRegs.SPIRXBUF;
				pushResult = pushIBuff(buf);
				if (buf[0] == END_MSG_TERM)
					pushResult = 1;		/* If the end of the message is detected while inside the loop break out of the loop */
			}
		} else {
			while ((SpiaRegs.SPIFFRX.bit.RXFFST > 0) && (SpiaRegs.SPIFFTX.bit.TXFFST < SPI_FFTX_FILLVL)) {
										/* Copy the received data from SPI Rx buffer into the SPI Tx buffer to create loop */
				SpiaRegs.SPITXBUF = SpiaRegs.SPIRXBUF;
			}
		}
	} else {
		while (SpiaRegs.SPIFFRX.bit.RXFFST > 0)
			buf[0] == (char) SpiaRegs.SPIRXBUF;	/* Read dummy data from buffer but don't do anything with it */
	}
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;	/* Clear SPIA FIFO receive interrupt flag */
	PieCtrlRegs.PIEACK.bit.ACK6 = 1;		/* Acknowledge the interrupt in PIE */
}




void oldSpiRxIsr (void) {
	// TODO: ...
	// Rx needs to know to ignore dummy data from master during a slave transmit -- could ignore Rx data if SRQ is activated?
	/* SPI interrupt (SPIRXINTA) indicating the SPI has received some data. */
    uint16_t pushResult = 0;
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
