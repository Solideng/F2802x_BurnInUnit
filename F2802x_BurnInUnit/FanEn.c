/*
 * FanEn.c
 *
 *  Created on: 7 May 2013
 *      Author: Toby
 */

#include"Common.h"

#ifndef BSTEN_H_
	static Uint16 crntState;
	static i2cMsg i2cMsgOutFan = {	I2C_MSGSTAT_INACTIVE, 		/* Message status */
									IOE_I2C_ADDR,				/* Slave device I2C address (ADC) */
									1,							/* Number of data bytes to be sent */
									1,							/* Slave uses one command byte */
									0,							/* Slave command (high byte) */
									0,							/* Slave command (low byte) */
									0x00,						/* Data byte 1 */
									0x00,						/* Data byte 2 */
									0x00						/* Data byte 3 */
								};
#endif

Uint16 fcInit (void) {
	#ifdef BSTEN_H_
		return bcInit();
	#else
		Uint16 err = 0;
		crntState = 0;

		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_IOCON_ADDR);
		i2cMsgOutFan.msgBuffer[0] = 0x24;		/* IOCON: SEQOP off, DISSLW on, HAEN x, ODR on, INTPOL x */
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;

		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPINTEN_ADDR);
		i2cMsgOutFan.msgBuffer[0] = 0x00;		/* GPINTEN: Input interrupt on change is disabled */
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;

		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_IPOL_ADDR);
		i2cMsgOutFan.msgBuffer[0] = 0x00;		/* IPOL: GPIO register bits are not inverted */
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;

		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_OLAT_ADDR);
		i2cMsgOutFan.msgBuffer[0] = 0x00;		/* GPIO: All outputs OFF */
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;

		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_IODIR_ADDR);
		i2cMsgOutFan.msgBuffer[0] = 0x00;		/* IODIR: Set all GPIO pins as outputs */
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;
		return 0;
	#endif
}

Uint16 fcEnable(Uint16 chnl) {
	#ifdef BSTEN_H_
		return bcEnable(chnl + FAN_CHNL_OFST);
	#else
		Uint16 err = 0, msk = 1;
		i2cMsg i2cMsgOutFan;
							/* Ensure channel is valid */
		if (chnl > FAN_NUM_CHNL)
			return CHANNEL_OOB;
							/* Set the related GPIO bit in the current state of the I/O expander output control */
		msk = crntState | (msk << (chnl + FAN_CHNL_OFST));
							/* Send the new output control state */
		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPIO_ADDR);
		i2cMsgOutFan.msgBuffer[0] = msk;
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;
		crntState = msk;	/* Update the current output control state record */
		return 0;
	#endif
}

Uint16 fcDisable (Uint16 chnl) {
	#ifdef BSTEN_H_
		return bcDisable(chnl + FAN_CHNL_OFST);
	#else
		Uint16 err = 0, msk = 1;
							/* Ensure channel is valid */
		if (chnl > FAN_NUM_CHNL)
				return CHANNEL_OOB;
							/* Clear the related GPIO bit in the current state of the I/O expander output control */
		msk = crntState & ~(msk << (chnl + FAN_CHNL_OFST));
							/* Send the new output control state */
		i2cPopMsg(&i2cMsgOutFan, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPIO_ADDR);
		i2cMsgOutFan.msgBuffer[0] = msk;
		err = i2cWrite(&i2cMsgOutFan);
		if (err)
			return err;
		crntState = msk;	/* Update the current output control state record */
		return 0;
	#endif
}
