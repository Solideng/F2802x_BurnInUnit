/*
 * EnableCtrl.c
 *
 *  Created on: 1 Oct 2013
 *      Author: Toby
 */
#include "Common.h"

static uint16_t crntState;	/* Keeps track of the current state of the I/O Ex GPIO otherwise
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

uint16_t initEnableControl (void) {
	uint16_t err = 0;
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
	i2cMsgOutEC.msgBuffer[0] = ALL_DISABLED_WORD; /* GPIO: All outputs OFF */
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

void resetEnableControl (void) {
	/* Pulses the reset-bar line of the MCP23008 I/O Expander low */
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
	DELAY_US(1);		/* Hold low for 1uS, T_RSTL */
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
	DELAY_US(100000);	/* Allow PowerOn Reset time. SV_DD = 0.05 V/ms. V_DD = 5V => S = 100ms */
	initEnableControl();
}

uint16_t enableCircuit(circuitSection section) {
	uint16_t err = 0, msk = 1;
	if (section >= maxchan)	/* Ensure section is valid */
		return CHANNEL_OOB;
							/* Enable the related bit in the current GPIO state */
	if (ALL_DISABLED_WORD & (1 << section))
		msk = crntState & ~(msk << section);
	else
		msk = crntState | (msk << section);
							/* Send the new GPIO state */
	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPIO_ADDR);
	i2cMsgOutEC.msgBuffer[0] = msk;
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;
	crntState = msk;		/* Update the current GPIO state record */

	if (section <= chan4)	/* Set the settings enable value to allow macros to output and slews to ramp */
		loadSettings[section].enable = TRUE;
	else {
		switch (section) {
			case xfmrCct:
				xfmrSettings.enable = TRUE;
				break;
			case acCct:
				acSettings.enable = TRUE;
				break;
			case psu:
				extSettings.extPsuEnable = TRUE;
				break;
			case fan:
				extSettings.extFanEnable = TRUE;
				break;
			default:
				return CHANNEL_OOB;
		}
	}
	return 0;
}

uint16_t disableCircuit (circuitSection section) {
	uint16_t err = 0, msk = 1;
	if (section >= maxchan)	/* Ensure section is valid */
		return CHANNEL_OOB;
							/* Disable the related bit in the current GPIO state */
	if (ALL_DISABLED_WORD & (1 << section))
		msk = crntState | (msk << section);
	else
		msk = crntState & ~(msk << (section));
							/* Send the new GPIO state */
	i2cPopMsg(&i2cMsgOutEC, I2C_MSGSTAT_SEND_WITHSTOP, IOE_I2C_ADDR, 1, 1, 0, IOE_GPIO_ADDR);
	i2cMsgOutEC.msgBuffer[0] = msk;
	err = i2cWrite(&i2cMsgOutEC);
	if (err)
		return err;
	crntState = msk;		/* Update the current GPIO state record */

	if (section <= chan4)	/* Clear the settings enable value to prevent macro output and slew ramping */
			loadSettings[section].enable = FALSE;
	else {
		switch (section) {
			case xfmrCct:
				xfmrSettings.enable = FALSE;
				break;
			case acCct:
				acSettings.enable = FALSE;
				break;
			case psu:
				extSettings.extPsuEnable = FALSE;
				break;
			case fan:
				extSettings.extFanEnable = FALSE;
				break;
			default:
				return CHANNEL_OOB;
		}
	}
	return 0;
}

uint16_t getCurrentState (circuitSection section, uint16_t *state) {
	uint16_t msk = 0;
	if (section >= maxchan)	/* Ensure section is valid */
		return CHANNEL_OOB;
	msk = 1 << section;		/* Create mask for specified section */
							/* Check if the section's current state is the same as the
							 * section's disabled state and return accordingly
							 */
	return ((ALL_DISABLED_WORD & msk) == (crntState & msk)) ? 0 : 1;
}
