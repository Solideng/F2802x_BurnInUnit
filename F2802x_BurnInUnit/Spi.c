//============================================================================
// FILE:	PSU_Control-spi.c
// TITLE:	SPI functions for PSU_Control
// Version: 3.0
// Date: 	03 May 12 - TM
//============================================================================
#include "Common.h"			// Include shared definitions and declarations

void spiInit(void) {
	// Initialise SPI for 3-wire use
	SpiaRegs.SPICCR.all = 0x000A;	// Reset on, rising edge, 11-bit char bits
	SpiaRegs.SPICTL.all = 0x000C;	// Enable master mode and delayed phase.
	SpiaRegs.SPIBRR = 0x007B;		// Set bit rate to 123, Baud = LSPCLK/(123+1) = 120968 Bd
	SpiaRegs.SPIPRI.bit.TRIWIRE = 1;// Set SPI as 3-wire.
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;	// Relinquish SPI from Reset
    SpiaRegs.SPIPRI.bit.FREE = 1;	// Set so breakpoints don't disturb xmission
}

void spiTx(Uint16 txData) {
	// Transmit data on SPI, uses  polling till transmission is finished 
	Uint16 dummy;
	SpiaRegs.SPICTL.bit.TALK = 1;				// Enable Tx path
	SpiaRegs.SPITXBUF = txData;					// Copy data to Tx buffer
	while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}// Wait until rx'd
	dummy = SpiaRegs.SPIRXBUF;					// Clears junk data
	dummy = dummy;
}

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

interrupt void spiTxFifoIsr(void) {
    asm("     ESTOP0");	// Stop execution! Function Not Written yet!!
    for (;;);	
}
*/
