/*
 * I2c.c
 *
 *  Created on: 2 May 2013
 *      Author: Toby
 */

#include "Common.h"

// TODO fix ISR error reporting

/*============== LOCAL FUNCTIONS ==============*/
static interrupt void i2c_isr(void);

/*================ LOCAL VARS ================*/
static i2cMsg *currentMsgPtr;               /* Used in interrupts */
//Uint16 PassCount;
//Uint16 FailCount;
Uint16 i2cIsrErr = 0;						// TODO Make SCPI reg status bit - nothing else done with it

/*=============== GLOBAL VARS =================*/
i2cMsg i2cMsgBlank = {	I2C_MSGSTAT_INACTIVE, 	/* Message status */
						0,						/* Slave device I2C address */
						0,						/* Number of data bytes */
						0,						/* Slave uses one pointer byte */
						0,						/* Slave register pointer (high byte) */
						0x00,					/* Slave register pointer (low byte) */
						0x00,					/* Data byte 1 */
						0x00,					/* Data byte 2 */
						0x00,					/* Data byte 3 */
						0x00					/* Data byte 4 */
						};


void i2cPopMsg(i2cMsg *msg, Uint16 msgStatus, Uint16 slaveAddr, Uint16 numDataBytes, Uint16 numSlavePtrBytes, Uint16 slavePtrAddrHi, Uint16 slavePtrAddrLo) {
	/* Populate the passed msg struct with the passed arguments
	 *  Saves having to set individual struct members one by
	 *  one in calling function for each communication
	 */
	Uint16 x;
	while (currentMsgPtr->msgStatus != I2C_MSGSTAT_INACTIVE) {} /* Check previous message has finished */
	msg->msgStatus = msgStatus;
	msg->slaveAddress = slaveAddr;
	
	if (numSlavePtrBytes > I2C_MAX_PTR_SIZE)	/* Ensure slave pointer is a valid size */
		msg->numSlavePtrBytes = I2C_MAX_PTR_SIZE; 
	else
		msg->numSlavePtrBytes = numSlavePtrBytes;
	
	if (msgStatus != I2C_MSGSTAT_SEND_NOSTOP)
		x = numSlavePtrBytes;
	else
		x = 0;
												/* Ensure message length is not too long */
	if (numDataBytes > (I2C_MAX_BUFFER_SIZE - x))
		msg->numOfBytes = I2C_MAX_BUFFER_SIZE - x;
	else
		msg->numOfBytes = numDataBytes;

	msg->slavePtrAddrHigh = slavePtrAddrHi;
	msg->slavePtrAddrLow = slavePtrAddrLo;
}

void i2cInit(void) {
	/* Initialize I2C peripheral an interrupts */
	Uint16 address = 0;
	
	currentMsgPtr = &i2cMsgBlank;		/* Set initial values for I2C message */
	
	I2caRegs.I2CSAR = address;			/* Slave address */
	I2caRegs.I2CPSC.all = 0x05;			/* Prescalar - Module clock must be 7-12 MHz */
										/* Module clk = SYSCLKOUT/(IPSC+1) = 10MHz */
										/* Master clk = 1/(((IPSC+1) [(ICCL+5)+(ICCH+5)])/SYSCLKOUT) = 80kHz */
	I2caRegs.I2CCLKL = 0x3A;			/* 6.3uS NOTE: must be non zero */
	I2caRegs.I2CCLKH = 0x39;			/* 6.2uS NOTE: must be non zero */
	I2caRegs.I2CIER.all = 0x24;			/* Enable SCD & ARDY interrupts */
	I2caRegs.I2CMDR.all = 0x20;			/* Release I2C from reset */
										/* Stop I2C when suspended */
	I2caRegs.I2CFFTX.all = 0x6000;		/* Enable FIFO mode and TXFIFO */
	I2caRegs.I2CFFRX.all = 0x2040;		/* Enable RXFIFO, clear RXFFINT */
	EALLOW;
	PieVectTable.I2CINT1A = &i2c_isr;	/* Map interrupt to ISR */
	EDIS;
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	/* Enable the PIE block */
	PieCtrlRegs.PIEIER8.bit.INTx1 = 1;	/* Enable in PIE Group 8 - INT1 */
	IER |= M_INT8;						/* Enable CPU interrupt 8 */
}

Uint16 i2cRead (i2cMsg *msg) {
	/* Start an I2C read */
	Uint16 i;
	
	for (i = 0; i < (I2C_MAX_BUFFER_SIZE); i++) 		/* Make sure received data buffer is fully empty */
		msg->msgBuffer[i] = 0x0000;
	
	while (currentMsgPtr->msgStatus != I2C_MSGSTAT_INACTIVE) {} /* Check previous message has finished */
	if (msg->msgStatus != I2C_MSGSTAT_SEND_NOSTOP) {
		return I2C_READ_WRONG_MSG;						/* Check message is of correct type */
	} else if (I2caRegs.I2CMDR.bit.STP == TRUE) {		/* Check STOP bit is clear */
		return I2C_STP_NOT_READY;
	} else if (I2caRegs.I2CSTR.bit.BB == TRUE) {		/* Check bus is not busy */
		return I2C_BUS_BUSY;
	} else {
		I2caRegs.I2CSAR = msg->slaveAddress;			/* Setup slave address */
		I2caRegs.I2CCNT = msg->numSlavePtrBytes;		/* Setup number of slave pointer bytes to be sent */
		if (msg->numSlavePtrBytes == I2C_MAX_PTR_SIZE)
			I2caRegs.I2CDXR = msg->slavePtrAddrHigh;	/* Setup high slave pointer byte */

		if (msg->numSlavePtrBytes > 0) 
			I2caRegs.I2CDXR = msg->slavePtrAddrLow;		/* Setup low slave pointer byte */

		msg->msgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;	/* Set status as busy */
		I2caRegs.I2CMDR.all = 0x2620;					/* Start transmission of slave pointer bytes */
		currentMsgPtr = msg;							/* Setup message pointer for interrupt */
	}
	return 0;
}

Uint16 i2cWrite (i2cMsg *msg) {
	/* start an I2C write */
	Uint16 i;
	
	while (currentMsgPtr->msgStatus != I2C_MSGSTAT_INACTIVE) {} /* Check previous message has finished */
	if (msg->msgStatus != I2C_MSGSTAT_SEND_WITHSTOP) {
		return I2C_WRITE_WRONG_MSG;					/* Check message is of correct type */
	} else if (I2caRegs.I2CMDR.bit.STP == TRUE) {	/* Check STP bit is clear */
		return I2C_STP_NOT_READY;
	} else if (I2caRegs.I2CSTR.bit.BB == TRUE) {	/* Check bus is not busy */
		return I2C_BUS_BUSY;
	} else {
		I2caRegs.I2CSAR = msg->slaveAddress;		/* Setup slave address */
													/* Setup total number of bytes to be sent */
		I2caRegs.I2CCNT = msg->numOfBytes + msg->numSlavePtrBytes;
		if (msg->numSlavePtrBytes == I2C_MAX_PTR_SIZE) 
			I2caRegs.I2CDXR = msg->slavePtrAddrHigh;/* Setup high slave pointer byte */

		if (msg->numSlavePtrBytes > 0) 
			I2caRegs.I2CDXR = msg->slavePtrAddrLow;	/*Setup low slave pointer byte */

		for (i = 0; i < msg->numOfBytes; i++) 		/* Setup data bytes */
			I2caRegs.I2CDXR = *(msg->msgBuffer+i);

		msg->msgStatus = I2C_MSGSTAT_WRITE_BUSY;	/* Set status as busy */
		currentMsgPtr = msg;						/* Setup message pointer for interrupt */
		I2caRegs.I2CMDR.all = 0x6E20;				/* Start transmission */
		
		DELAY_US(600);								/* Wait until transmission finished before returning */
													/* other wise a next write may interrupt this one */
		// TODO if a NACK is received then this may not be long enough to retry but is to long for 2 byte messages
		return 0;
	}
}

static interrupt void i2c_isr (void) {
	Uint16 intSource, i;

	intSource = I2caRegs.I2CISRC.all;								/* Get interrupt source */

	if (intSource == I2C_SCD_ISRC) {								/* Stop condition detected i.e. read or write finished */
		if (currentMsgPtr->msgStatus == I2C_MSGSTAT_WRITE_BUSY) {
			currentMsgPtr->msgStatus = I2C_MSGSTAT_INACTIVE;		/* If write completed, set message as finished */
			currentMsgPtr = &i2cMsgBlank;
		} else {
			if (currentMsgPtr->msgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY) {
				currentMsgPtr->msgStatus = I2C_MSGSTAT_SEND_NOSTOP;
																	/* If a write portion of a read has failed with a NACK,
																	 *  the code below commands a STP so that the code here
																	 *  can set the message to try again*/
				i2cRead(currentMsgPtr);
			} else if (currentMsgPtr->msgStatus == I2C_MSGSTAT_READ_BUSY) {	/* Read finished */
				for (i = 0; i < currentMsgPtr->numOfBytes; i++) 	/* Copy data to message data buffer */
					currentMsgPtr->msgBuffer[i] = I2caRegs.I2CDRR;

				currentMsgPtr->msgStatus = I2C_MSGSTAT_INACTIVE;	/* Set message as finished */
				currentMsgPtr = &i2cMsgBlank;
			}
		}
	} else if (intSource == I2C_ARDY_ISRC) {						/* Register access ready detected i.e. write section of read completed */
		if (I2caRegs.I2CSTR.bit.NACK == TRUE) {						/* NACK detected - write section failed */
			I2caRegs.I2CMDR.bit.STP = 1;							/* Command a stop so the code above can set the message to try again */
			I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;					/* Clear NACK */
		} else if  (currentMsgPtr->msgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY) {
																	/* If the write section of the read has completed successfully */
			I2caRegs.I2CCNT = currentMsgPtr->numOfBytes;
			currentMsgPtr->msgStatus = I2C_MSGSTAT_READ_BUSY;		/* Set message to read */
			I2caRegs.I2CMDR.all = 0x2C20;							/* Start read */
		}
	} else {
		i2cIsrErr = I2C_INVALID_ISRC;								/* Invalid or unhandled interrupt */
	}
	PieCtrlRegs.PIEACK.bit.ACK8 = 1;								/* Acknowledge interrupt in PIE */
}
