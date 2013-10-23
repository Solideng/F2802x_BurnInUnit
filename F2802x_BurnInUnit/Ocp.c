/*
 * ocp.c
 *
 *  Created on: 14 Oct 2013
 *      Author: Toby
 */

#include "Common.h"

Uint16 ocpFlagRegister = 0;

/*============== Load n ==============*/
Uint16 setLoadOcpLevel (loadStage load, float32 dcLevel) {
	/* Sets OCP level for the specified load
	 *  dcLevel is expected in amps
	 *  - iScale SHOULD BE SET BEFORE USE -
	 */
	float32 iMax = 0;
//	int32 iSt = 0;

	if (load >= numberOfLoads)			/* Check channel is valid */
		return CHANNEL_OOB;
	if (dcLevel > LOAD_IDCLVL_MAX)		/* Check level is less than the allowed maximum */
		return VALUE_OOB;
	if (loadSettings[load].iScale == 0)	/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	iMax = _IQ14toF((int32) loadSettings[load].iScale);	/* Convert scale from SQ to float */
	iMax = ((VDDA - VSSA) * 0.001) * (1.0 / iMax); 		/* Calculate maximum I */
	loadSettings[load].ocpLevel = _IQ24(dcLevel / iMax);/* Normalise and save */
	return 0;
}

Uint16 getLoadOcpLevel (loadStage load, float32 * dcLevel) {
	/* Returns current OCP level, for the specified load,
	 *  based on actual OCP and iScale
	 *  - iScale SHOULD BE SET BEFORE USE -
	 */
	float32 iMax = 0;
	if (load >= numberOfLoads)			/* Check channel is valid */
		return CHANNEL_OOB;
	if (loadSettings[load].iScale == 0)	/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;
	iMax = ((VDDA - VSSA) * 0.001) * (16384.0 / loadSettings[load].iScale); /* Calculate maximum I */
	*dcLevel = ((_IQ24toF(loadSettings[load].ocpLevel)) * iMax);			/* De-normalise */
	return 0;
}

Uint16 checkLoadOcp (loadStage load) {
	/* Load over-current protection
	 *  - iScale AND ocpLevel SHOULD BE SET BEFORE USE -
	 */
	if (load >= numberOfLoads)	/* Check channel is valid */
		return CHANNEL_OOB;
								/* Compare the load ISns ADC value to its OCP limit */
	if (loadNets[load].iFdbkNet > loadSettings[load].ocpLevel) {
		stopAll();
		ocpFlagRegister |= (1 << load);
		return OCP_TRIP;
	}
	return 0;
}

Uint16 getLoadOcpState (loadStage load) {
	return (load < numberOfLoads) ? ((ocpFlagRegister & (1 << load)) > 0) : CHANNEL_OOB;
}

Uint16 clearLoadOcp (loadStage load) {
	ocpFlagRegister &= (~(1 << load));
	if (ocpFlagRegister)
		return OCP_TRIP;
	runAll();
	return 0;
}

/*============== DC Mid ==============*/
Uint16 setDcMidOcpLevel (float32 dcLevel) {
	/* Sets the OCP level for the DC Mid ISns
	 * dcLevel is expected in amps
	 * - iScale SHOULD BE SET BEFORE USE -
	 */
	float32 iMax = 0;
	int32 iSt = 0;

	if (xfmrSettings.iScale == 0)	/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	iSt = _IQ10(dcLevel);			/* Convert level to Q10 and check result is in range */
	if ((iSt <= 0) && (iSt > xfmrSettings.iMax))
		return VALUE_OOB;

	iMax = _IQ14toF((int32) xfmrSettings.iScale);	/* Convert scale from SQ to float */
	iMax = ((VDDA - VSSA) * 0.001) * (1.0 / iMax);	/* Calculate maximum I */
	xfmrSettings.ocpLevel = _IQ24(dcLevel / iMax);	/* Normalise and save */
	return 0;
}

Uint16 getDcMidOcpLevel (float32 * dcLevel) {
	/* Returns current OCP limit for the DC Mid ISns,
	 *  based on actual OCP and iScale
	 *  - iScale SHOULD BE SET BEFORE USE -
	 */
	float32 iMax = 0;
	if (xfmrSettings.iScale == 0)	/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	iMax = ((VDDA - VSSA) * 0.001) * (16384.0 / xfmrSettings.iScale);	/* Calculate maximum I */
	*dcLevel = ((_IQ24toF(xfmrSettings.ocpLevel)) * iMax);				/* De-normalise */
	return 0;
}

Uint16 checkDcMidOcp (void) {
	/* DC Mid over-current protection
	 *  - iScale AND ocpLevel SHOULD BE SET BEFORE USE -
	 */
	if (xfmrNets.iSnsNet > xfmrSettings.ocpLevel) {	/* Compare the DC Mid ISns ADC value to its OCP limit */
		stopAll();
		ocpFlagRegister |= DCMID_OCP_TRIP;	/* Set flag */
		return OCP_TRIP;
	}
	return 0;
}

Uint16 getDcMidOcpState (void) {return ((ocpFlagRegister & DCMID_OCP_TRIP) > 0);}

Uint16 clearDcMidOcp (void) {
	ocpFlagRegister &= (~DCMID_OCP_TRIP);
	if (ocpFlagRegister)
		return OCP_TRIP;
	runAll();
	return 0;
}


/*================ AC ================*/
Uint16 setAcOcpLevel (float32 pkLevel) {
	/* Sets the OCP value for the AC ISns
	 * pkLevel expected in amps
	 */
	float32 iPkMax = 0;
	int32 iStRms = 0;

	iStRms = _IQ10(pkLevel * RECP_SQRT_2);	/* Convert setting to RMS Q10 and check result is in range */
	if ((iStRms <= 0) && (iStRms > acSettings.iMaxRms))
		return VALUE_OOB;

	iPkMax = _IQ14toF((int32) acSettings.iScale);		/* Convert scale from SQ to float */
	iPkMax = ((VDDA - VSSA) * 0.001) * (1.0 / iPkMax);	/* Calculate maximum Ipk */
	acSettings.ocpLevel = _IQ24(pkLevel / iPkMax);		/* Normalise and save */
	//TODO: Include gain??
	setAcDac(pkLevel);						/* Set the related DAC level */
	return 0;
}

Uint16 getAcOcpLevel (float32 * pkLevel) {
	/* Returns current OCP limit for the AC ISns,
	 *  based on actual OCP and iScale
	 *  - iScale SHOULD BE SET BEFORE USE -
	 */
	float32 iPkMax = 0;
	if (acSettings.iScale == 0)	/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	iPkMax = ((VDDA - VSSA) * 0.001) * (16384 / acSettings.iScale);	/* Calculate maximum Ipk */
	*pkLevel = ((_IQ24toF(acSettings.ocpLevel)) * iPkMax);			/* De-normalise */
	return 0;
}

Uint16 tripAcOcp (void) {
	stopAll();
	ocpFlagRegister |= AC_OCP_TRIP;
	return OCP_TRIP;
}

Uint16 getAcOcpState (void) {return ((ocpFlagRegister & AC_OCP_TRIP) > 0);}

Uint16 clearAcOcp (void) {
	ocpFlagRegister &= (~AC_OCP_TRIP);	/* Clear flag */
	rstAcTripzone();					/* Reset trip zone */
	if (ocpFlagRegister)				/* Check if there are any other flags still raised */
		return OCP_TRIP;
	runAll();							/* Run macros */
	return 0;
}
