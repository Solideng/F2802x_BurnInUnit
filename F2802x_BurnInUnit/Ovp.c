/*
 * Ovp.c
 *
 *  Created on: 15 Oct 2013
 *      Author: Toby
 */
#include "Common.h"

Uint16 ovpFlagRegister = 0;

/*============== Load n ==============*/
Uint16 checkLoadOvp (loadStage load) {
	/* Load over-voltage protection
	 *  - vScale AND ovpLevel SHOULD BE SET BEFORE USE -
	 */
	Uint16 err = 0;
	float32 vMeas = 0;
					/* Read voltage */
	err = getLoadVoltage(load, &vMeas);
	if (err)		/* Check reading completed OK */
		return err;
					/* Compare the scaled measured load voltage value to the fixed OVP level */
	if (vMeas > LOAD_VDCLVL_FIX) {
		stopAll();
		ocpFlagRegister |= (1 << load);
		return OVP_TRIP;
	}
	return 0;
}

Uint16 getLoadOvpState (loadStage load) {
	return (load < numberOfLoads) ? ((ovpFlagRegister & (1 << load)) > 0) : CHANNEL_OOB;
}

Uint16 clearLoadOvp (loadStage load) {
	ovpFlagRegister &= (~(1 << load));
	if (ovpFlagRegister)
		return OVP_TRIP;
	runAll();
	return 0;
}

/*============== DC Mid ==============*/
// THESE ARE COMMENTED OUT BECAUSE DC_MID IS USING THE 400 V_PK FIXED LEVEL
// BUT NOT SURE IF THAT IS CORRECT. IF IT IS, THESE CAN BE DELETED.

//Uint16 setDcMidOvpLevel (float32 dcLevel) {
//	/* Sets the OVP level for the DC Mid VSns
//	 * dcLevel is expected in volts
//	 * - vScale SHOULD BE SET BEFORE USE -
//	 */
//	float32 vMax = 0;
//	int32 vSt = 0;
//
//	if (xfmrSettings.midVScale == 0)	/* Check vScale is set, to avoid div-by-0 exception */
//		return VALUE_OOB;
//
//	vSt = _IQ10(dcLevel);				/* Convert level to Q10 and check result is in range */
//	if ((vSt <= 0) && (vSt > xfmrSettings.midVMax))
//		return VALUE_OOB;
//
//	vMax = _IQ14toF((int32) xfmrSettings.midVScale);	/* Convert scale from SQ to float */
//	vMax = ((VDDA - VSSA) * 0.001) * (1.0 / vMax);		/* Calculate maximum V */
//	xfmrSettings.midOvpLevel = _IQ24(dcLevel / vMax);	/* Normalise and save */
//	setDcDac(dcLevel);					/* Set the related DAC level */
//	return 0;
//}
//
//Uint16 getDcMidOvpLevel (float32 *dcLevel) {
//	/* Returns current OVP limit for the DC Mid VSns,
//	 *  based on actual OVP and vScale
//	 *  - vScale SHOULD BE SET BEFORE USE -
//	 */
//	float32 vMax = 0;
//	if (xfmrSettings.midVScale == 0)	/* Check vScale is set, to avoid div-by-0 exception */
//		return VALUE_OOB;
//
//	vMax = ((VDDA - VSSA) * 0.001) * (16384.0 / xfmrSettings.midVScale);/* Calculate maximum V */
//	*dcLevel = ((_IQ24toF(xfmrSettings.midOvpLevel)) * vMax);			/* De-normalise */
//	return 0;
//}

Uint16 tripDcMidOvp (void) {
	stopAll();
	ovpFlagRegister |= DCMID_OVP_TRIP;
	return OVP_TRIP;
}

Uint16 getDcMidOvpState (void) {return ((ovpFlagRegister & DCMID_OVP_TRIP) > 0);}

Uint16 clearDcMidOvp (void) {
	ovpFlagRegister &= (~DCMID_OVP_TRIP);
	rstDcTripzone();
	if (ovpFlagRegister)
		return OVP_TRIP;
	runAll();
	return 0;
}

/*=============== DC HV ==============*/
Uint16 setDcHvOvpLevel (float32 dcLevel) {
	/* Sets OVP value for the DC HV VSns
	 * level is expected in volts
	 */
	float32 vMax = 0;
	int32 vSt = 0;

	//vSt = _IQ10(dcLevel * RECP_SQRT_2);			/* Convert setting to RMS Q10 and check result is in range */
	vSt = _IQ10(dcLevel);	/* Convert setting to Q10 and check result is in range */
	if ((vSt <= 0) && (vSt > xfmrSettings.hvVMax))
		return VALUE_OOB;

	vMax = _IQ14toF((int32) xfmrSettings.hvVScale);	/* Convert scale from SQ to float */
	vMax = ((VDDA - VSSA) * 0.001) * (1.0 / vMax);	/* Calculate maximum V */
	xfmrSettings.hvOvpLevel = _IQ24(dcLevel / vMax);/* Normalise */
	return 0;
}

Uint16 getDcHvOvpLevel (float32 * dcLevel) {
	/* Returns current OVP limit for the DC HV VSns,
	 *  based on actual OVP and vScale
	 *  - vScale SHOULD BE SET BEFORE USE -
	 */
	float32 vMax = 0;
	if (xfmrSettings.hvVScale == 0)	/* Check vScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	vMax = ((VDDA - VSSA) * 0.001) * (16384 / xfmrSettings.hvVScale);	/* Calculate maximum V */
	*dcLevel = ((_IQ24toF(xfmrSettings.hvOvpLevel)) * vMax);			/* De-normalise */
	return 0;
}

Uint16 checkDcHvOvp (void) {
	/* DC HV over-voltage protection
	 *  - vScale AND ovpLevel SHOULD BE SET BEFORE USE -
	 */
	if (xfmrNets.hvVSnsNet > xfmrSettings.hvOvpLevel){	/* Compare the DC HV VSns ADC value to its OVP limit */
		stopAll();
		ovpFlagRegister |= DCHV_OVP_TRIP;	/* Set flag */
		return OVP_TRIP;
	}
	return 0;
}

Uint16 getDcHvOvpState (void) {return ((ovpFlagRegister & DCHV_OVP_TRIP) > 0);}

Uint16 clearDcHvOvp (void) {
	ovpFlagRegister &= (~DCHV_OVP_TRIP);
	if (ovpFlagRegister)
		return OVP_TRIP;
	runAll();
	return 0;
}

/*================ AC ================*/
Uint16 setAcOvpLevel (float32 pkLevel) {
	/* Sets the OVP value for the AC VSns
	 * pkLevel expected in amps
	 */
	float32 vPkMax = 0;
	int32 vStRms = 0;

	vStRms = _IQ10(pkLevel * RECP_SQRT_2);	/* Convert setting to RMS Q10 and check result is in range */
	if ((vStRms <= 0) && (vStRms > acSettings.vMaxRms))
		return VALUE_OOB;

	vPkMax = _IQ14toF((int32) acSettings.vScale);		/* Convert scale from SQ to float */
	vPkMax = ((VDDA - VSSA) * 0.001) * (1.0 / vPkMax);	/* Calculate maximum Vpk */
	acSettings.ovpLevel = _IQ24(pkLevel / vPkMax);		/* Normalise and save */
	//TODO: Include gain??
	return 0;
}

Uint16 getAcOvpLevel (float32 * pkLevel) {
	/* Returns current OVP limit for the AC VSns,
	 *  based on actual OVP and vScale
	 *  - vScale SHOULD BE SET BEFORE USE -
	 */
	float32 vPkMax = 0;
	if (acSettings.vScale == 0)	/* Check vScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	vPkMax = ((VDDA - VSSA) * 0.001) * (16384 / acSettings.vScale);	/* Calculate maximum Vpk */
	*pkLevel = ((_IQ24toF(acSettings.ovpLevel)) * vPkMax);			/* De-normalise */
	return 0;
}

Uint16 checkAcOvp (void)  {
	/* AC over-voltage protection
	 *  - vScale AND ovpLevel SHOULD BE SET BEFORE USE -
	 */
	if (acNets.vFdbkNet > acSettings.ovpLevel) {	/* Compare the AC VSns ADC value to its OVP limit */
		stopAll();
		ovpFlagRegister |= AC_OVP_TRIP;
		return OVP_TRIP;
	}
	return 0;
}

Uint16 getAcOvpState (void) {return ((ovpFlagRegister & AC_OVP_TRIP) > 0);}

Uint16 clearAcOvp (void) {
	ovpFlagRegister &= (~AC_OVP_TRIP);
	if (ovpFlagRegister)
		return OVP_TRIP;
	runAll();
	return 0;
}
