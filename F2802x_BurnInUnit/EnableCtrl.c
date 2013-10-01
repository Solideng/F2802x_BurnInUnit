/*
 * EnableCtrl.c
 *
 *  Created on: 1 Oct 2013
 *      Author: Toby
 */
#include "Common.h"

static Uint16 crntState;	/* Keeps track of the current state of the I/O Ex GPIO otherwise
 	 	 	 	 	 	 	 * we would need to read it before every write to the GPIO_ADDR
 	 	 	 	 	 	 	 */
static i2cMsg i2cMsgOutEC = { I2C_MSGSTAT_INACTIVE, 		/* Message status */
								IOE_I2C_ADDR,				/* Slave device I2C address */
								1,							/* Number of data bytes to be sent */
								1,							/* Slave uses one command byte */
								0,							/* Slave command (high byte) */
								0,							/* Slave command (low byte) */
								0x00,						/* Data byte 1 */
								0x00,						/* Data byte 2 */
								0x00						/* Data byte 3 */
							};

void ecReset (void) {
	/* Pulses the reset-bar line of the MCP23008 I/O Expander low */
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
	DELAY_US(1);		/* Hold low for 1uS, T_RSTL */
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
	DELAY_US(100000);	/* Allow PowerOn Reset time. SV_DD = 0.05 V/ms. V_DD = 5V => S = 100ms */
}

Uint16 ecInit (void) {
	Uint16 err = 0;
	crntState = 0;							/* Clear current GPIO state record. */

	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_IOCON_ADDR);
	i2cMsgOutEC.msgBuffer[0] = 0x24;		/* IOCON: SEQOP off, DISSLW on, HAEN x, ODR on, INTPOL x */
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;

	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPINTEN_ADDR);
	i2cMsgOutEC.msgBuffer[0] = 0x00;		/* GPINTEN: Input interrupt on change is disabled */
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;

	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_IPOL_ADDR);
	i2cMsgOutEC.msgBuffer[0] = 0x00;		/* IPOL: GPIO register bits are not inverted */
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;

	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_OLAT_ADDR);
	i2cMsgOutEC.msgBuffer[0] = 0x00;		/* GPIO: All outputs OFF */
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;

	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_IODIR_ADDR);
	i2cMsgOutEC.msgBuffer[0] = 0x00;		/* IODIR: Set all GPIO pins as outputs */
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;

	return 0;
}

Uint16 ecEnable(circuitSection section) {
	Uint16 err = 0, msk = 1;
						/* Ensure channel is valid */
	if (section >= maxchan)
		return CHANNEL_OOB;
						/* Set the related bit in the current GPIO state */
	msk = crntState | (msk << (section));
						/* Send the new GPIO state */
	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPIO_ADDR);
	i2cMsgOutEC.msgBuffer[0] = msk;
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;
	crntState = msk;	/* Update the current GPIO state record */
	return 0;
}

Uint16 ecDisable (circuitSection section) {
	Uint16 err = 0, msk = 1;
						/* Ensure channel is valid */
	if (section >= maxchan)
			return CHANNEL_OOB;
						/* Clear the related bit in the current GPIO state */
	msk = crntState & ~(msk << (section));
						/* Send the new GPIO state */
	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPIO_ADDR);
	i2cMsgOutEC.msgBuffer[0] = msk;
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;
	crntState = msk;	/* Update the current GPIO state record */
	return 0;
}