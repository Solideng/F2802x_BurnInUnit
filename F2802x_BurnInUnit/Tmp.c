/*
 * Tmp.c
 *
 *  Created on: 2 May 2013
 *      Author: Toby
 */

#include "Common.h"

static i2cMsg i2cMsgOutTmp = {	I2C_MSGSTAT_INACTIVE, 		/* Message status */
								ADC_I2C_ADDR,				/* Slave device I2C address (ADC) */
								0,							/* Number of data bytes to be sent */
								1,							/* Slave uses one command byte */
								0,							/* Slave command (high byte) */
								0,							/* Slave command (low byte) */
								0x00,						/* Data byte 1 */
								0x00,						/* Data byte 2 */
								0x00						/* Data byte 3 */
							};

static uint16_t getTmpChnCode (uint16_t chn) {
	/* Calculates the channel number of the external ADC based on the passed boost channel number. */
	if ((chn % 2) == 0)		/* Check if the argument is even or odd */
		return (chn / 2);	/* Half the argument */
	else					/* Add the argument to the highest code, then half the result. */
		return (((ADC_NUM_CHNL - 1) + chn) / 2);
}

static void tmpCompensation (float tmp, float *tCmpDest) {
	/* Apply temperature sensor error compensation */
	float error2ndOrder = 0.0;		/* Calculate the 2nd order error using the 2nd order polynomial equation from Microchip AN1001 */
	error2ndOrder = (TMP_EC2 * (TMP_T_HOT - tmp) * (tmp - TMP_T_COLD)) + (TMP_EC1 * (tmp - TMP_T_COLD) + TMP_E_T_COLD);
	*tCmpDest = tmp - error2ndOrder;	/* Apply the error to the temperature reading */
}

uint16_t initTemperature (void) {
	/* Do an initial write to all ADC channels to set
	 * power and reference settings as the reference
	 * change needs settling time
	 */
	uint16_t i = 0, err = 0;

	for (i = 0; i < ADC_NUM_CHNL; i++) {
										/* Setup the message */
		i2cPopMsg(&i2cMsgOutTmp, I2C_MSGSTAT_SEND_WITHSTOP, ADC_I2C_ADDR, 1, 0, 0, 0);
		i2cMsgOutTmp.msgBuffer[0] = (0x84 | (getTmpChnCode(i) << 4));
		err = i2cWrite(&i2cMsgOutTmp);	/* Start sending the message */
		if (err)						/* Check write initiation finished OK */
			return err;
	}
	return 0;
}

uint16_t readTemperature (uint16_t chnl, float *tmpDest) {
	/* Read the temperature of the specified channel */
	uint16_t err = 0;
	float ofst = TMP_SCL_OFST, rawTmp = 0.0;
	i2cMsg i2cMsgInTmp;

	if (chnl >= ADC_NUM_CHNL)			/* Check channel is valid */
		return CHANNEL_OOB;
										/* Setup a read message */
	i2cPopMsg(&i2cMsgInTmp, I2C_MSGSTAT_SEND_NOSTOP, ADC_I2C_ADDR, 1, 1, 0, (0x84 | (getTmpChnCode(chnl) << 4)));
	err = i2cRead(&i2cMsgInTmp);		/* Start temperature I2C read */
										/* Wait for I2C to finish */
	while (i2cMsgInTmp.msgStatus != I2C_MSGSTAT_INACTIVE) {}
	if (err)							/* Check the read initiation reported no errors */
		return err;
										/* Remove sensor 0 degree C offset */
	rawTmp = (i2cMsgInTmp.msgBuffer[0] - ((int)ofst));
	tmpCompensation(rawTmp, tmpDest);	/* Apply sensor error compensation */
	return 0;
}
