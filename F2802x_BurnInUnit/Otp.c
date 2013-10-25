/*
 * Otp.c
 *
 *  Created on: 16 Oct 2013
 *      Author: Toby
 */

#include "Common.h"

Uint16 otpFlagRegister;

/*============== Load n ==============*/
Uint16 setLoadOtpLevel (loadStage load, float32 level){
	if (load >= numberOfLoads)					/* Check channel is valid */
		return CHANNEL_OOB;
	if ((level < TMP_OTP_MIN) || (level > TMP_OTP_MAX))
		return VALUE_OOB;						/* Check temperature is valid */
	loadSettings[load].otpLevel = _SQ7(level);	/* Convert to SQ7 and Set OTP */
	return 0;
}

Uint16 getLoadOtpLevel (loadStage load, float32 *level){
	if (load >= numberOfLoads)	/* Check channel is valid */
		return CHANNEL_OOB;
								/* Convert from SQ7 and save data to destination */
	*level = ((float32)loadSettings[load].otpLevel) * (1.0 / 128);
	return 0;
}

Uint16 checkLoadOtp (loadStage load){
	/* Read the specified load's temperature and compare
	 * the reading to the OTP level
	 */
	Uint16 err = 0, sqTmp;
	float32 tmp = 0;
	if (load >= numberOfLoads)	/* Check channel is valid */
		return CHANNEL_OOB;

	err = readTemperature(load, &tmp);	/* Read temperature of the load */
	if (err)					/* Check the read finished without error */
		return err;
	sqTmp = _SQ7(tmp);			/* Convert temperature to SQ7 to match OTP level */
								/* Check temperature against OTP level */
	if (sqTmp >= loadSettings[load].otpLevel) {
		stopAll();
		otpFlagRegister |= (1 << load);
		return OTP_TRIP;
	}
	return 0;	/* If we've made it this far then there were no errors */
}

Uint16 getLoadOtpState (loadStage load){
	return (load < numberOfLoads) ? ((otpFlagRegister & (1 << load)) > 0) : CHANNEL_OOB;
}

Uint16 clearLoadOtp (loadStage load){
	otpFlagRegister &= (~(1 << load));
	if (otpFlagRegister)
		return OTP_TRIP;
	runAll();
	return 0;
}

/*================ DC ===============*/
Uint16 setDcOtpLevel (float32 level){
	if ((level < TMP_OTP_MIN) || (level > TMP_OTP_MAX))
		return VALUE_OOB;				/* Check temperature is valid */
	xfmrSettings.otpLevel = _SQ7(level);/* Convert to SQ7 and Set OTP */
	return 0;
}

Uint16 getDcOtpLevel (float32 *level){
	*level = ((float32) xfmrSettings.otpLevel) * (1.0 / 128);	/* Convert from SQ7 and save data to destination */
	return 0;
}

Uint16 checkDcOtp (void){
	/* Read the DC stage's temperature and compare
	 * the reading to the OTP level
	 */
	Uint16 err = 0, sqTmp;
	float32 tmp = 0;

	err = readTemperature(XFMR_STAGE, &tmp);/* Read temperature of the DC stage */
	if (err)						/* Check the read finished without error */
		return err;
	sqTmp = _SQ7(tmp);				/* Convert temperature to SQ7 to match OTP level */
									/* Check temperature against OTP level */
	if (sqTmp >= xfmrSettings.otpLevel) {
		stopAll();
		otpFlagRegister |= DC_OTP_TRIP;
		return OTP_TRIP;
	}
	return 0;	/* If we've made it this far then there were no errors */
}

Uint16 getDcOtpState (void){return ((otpFlagRegister & DC_OTP_TRIP) > 0);}

Uint16 clearDcOtp (void){
	otpFlagRegister &= (~DC_OTP_TRIP);
	if (otpFlagRegister)
		return OTP_TRIP;
	runAll();
	return 0;
}

/*================ AC ================*/
Uint16 setAcOtpLevel (float32 level){
	if ((level < TMP_OTP_MIN) || (level > TMP_OTP_MAX))
		return VALUE_OOB;					/* Check temperature is valid */
	acSettings.otpLevel = _SQ7(level);/* Convert to SQ7 and Set OTP */
	return 0;
}

Uint16 getAcOtpLevel (float32 *level){
	*level = ((float32) acSettings.otpLevel) * (1.0 / 128);	/* Convert from SQ7 and save data to destination */
	return 0;
}

Uint16 checkAcOtp (void){
	/* Read the AC stage's temperature and compare
	 * the reading to the OTP level
	 */
	Uint16 err = 0, sqTmp;
	float32 tmp = 0;

	err = readTemperature(AC_STAGE, &tmp);	/* Read temperature of the AC stage */
	if (err)						/* Check the read finished without error */
		return err;
	sqTmp = _SQ7(tmp);				/* Convert temperature to SQ7 to match OTP level */
									/* Check temperature against OTP level */
	if (sqTmp >= xfmrSettings.otpLevel) {
		stopAll();
		otpFlagRegister |= AC_OTP_TRIP;
		return OTP_TRIP;
	}
	return 0;	/* If we've made it this far then there were no errors */
}

Uint16 getAcOtpState (void){return ((otpFlagRegister & DC_OTP_TRIP) > 0);}

Uint16 clearAcOtp (void){
	otpFlagRegister &= (~AC_OTP_TRIP);
	if (otpFlagRegister)
		return OTP_TRIP;
	runAll();
	return 0;
}

/*================ Ext ===============*/
Uint16 setExtOtpLevel (extSelect ext, float32 level){
	if (ext >= numberOfExts)	/* Check channel is valid */
		return CHANNEL_OOB;
	if ((level < TMP_OTP_MIN) || (level > TMP_OTP_MAX))
		return VALUE_OOB;		/* Check temperature is valid */
	if (ext == ext1)			/* Convert to SQ7 and Set OTP */
		extSettings.ext1OtpLevel = _SQ7(level);
	else
		extSettings.ext2OtpLevel = _SQ7(level);
	return 0;
}

Uint16 getExtOtpLevel (extSelect ext, float32 *level){
	if (ext >= numberOfExts)	/* Check channel is valid */
		return CHANNEL_OOB;

	if (ext == ext1)			/* Convert from SQ7 and save data to destination */
		*level = ((float32)extSettings.ext1OtpLevel) * (1.0 / 128);
	else
		*level = ((float32)extSettings.ext2OtpLevel) * (1.0 / 128);
	return 0;
}

Uint16 checkExtOtp (extSelect ext){
	/* Read the DC stage's temperature and compare
	 * the reading to the OTP level
	 */
	Uint16 err = 0, sqTmp;
	float32 tmp = 0;

	if (ext >= numberOfExts)		/* Check ext selection is valid */
		return CHANNEL_OOB;

	if (ext == ext1)
		err = readTemperature(EXT_1, &tmp);	/* Read temperature of the Ext1 sensor */
	else
		err = readTemperature(EXT_2, &tmp);	/* Read temperature of the Ext2 sensor */

	if (err)						/* Check the read finished without error */
		return err;
	sqTmp = _SQ7(tmp);				/* Convert temperature to SQ7 to match OTP level */
									/* Check temperature against OTP level */
	if (((ext == ext1) && (sqTmp >= extSettings.ext1OtpLevel)) ||
			((ext == ext2) &&(sqTmp >= extSettings.ext2OtpLevel))) {
		stopAll();
		otpFlagRegister |= (EXT1_OTP_TRIP << ext);
		return OTP_TRIP;
	}
	return 0;	/* If we've made it this far then there were no errors */
}

Uint16 getExtOtpState (extSelect ext){
	if (ext >= numberOfExts)
		return CHANNEL_OOB;
	return ((otpFlagRegister & (EXT1_OTP_TRIP << ext)) > 0);
}

Uint16 clearExtOtp (extSelect ext){
	otpFlagRegister &= (~(EXT1_OTP_TRIP << ext));
	if (otpFlagRegister)
		return OTP_TRIP;
	runAll();
	return 0;
}
