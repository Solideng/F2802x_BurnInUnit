/*
 * Adc.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

static void adcSocConfig(void) {
	/* Configures ADC SOC for ADC macro
	 *  - SHOULD BE RUN BEFORE DPL_INIT() -
	 */
	int16 ChSel[16]={1,1,3,6,7,2,12,9,11,14,15,0,10,4,0,0};	/* ADC channel selections */
	int16 TrigSel[16]={5,5,5,5,5,5,5,5,5,5,5,5,5,0,0};		/* ADC trigger selections */
	int16 ACQPS[16]={6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};		/* ADC qualification window sizes */
	ADC_SOC_CNF(ChSel, TrigSel, ACQPS, 16, 0);
}

void configAllAdc (void) {
	/* This SHOULD be called after the PWMs have been configured (pwmMacroConfigure())
	 *  - SHOULD BE RUN AFTER pwmMacroConfigure() -
	 *  - SHOULD BE RUN BEFORE DPL_INIT() -
	 */
	adcSocConfig();			/* Configure Macro ADCs SOCs */
	pwmSocConfigure();		/* Configure PWM as SOC trigger */
}

/*============== Load n ==============*/
Uint16 getLoadCurrent (loadStage load, float32 * dcCurrent) {
	/* Reads the load current ADC reading. */
	float32 iMax = 0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;
	if (loadSettings[load].iScale == 0)
		return VALUE_OOB;
								/* Calculate maximum current */
	iMax = ((VDDA - VSSA) * 0.001) * (16384 / loadSettings[load].iScale);
								/* Scale and de-normalise the measured load current and convert to float */
	*dcCurrent = _IQ24toF(loadNets[load].iFdbkNet) * iMax;
	return 0;
}

Uint16 getLoadVoltage (loadStage load, float32 * dcVoltage) {
	/* Reads the load voltage ADC reading. */
	float32 vMax = 0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;
	if (loadSettings[load].vScale == 0)
		return VALUE_OOB;
								/* Calculate maximum voltage */
	vMax = ((VDDA - VSSA) * 0.001) * (16384 / loadSettings[load].vScale);
								/* Scale and de-normalise the measured load voltage and convert to float */
	*dcVoltage = _IQ24toF(loadNets[load].vFdbkNet) * vMax;
	return 0;
}

/*============== DC Mid ==============*/
Uint16 getDcMidCurrent (float32 * dcCurrent) {
	/* Reads the DC Mid current ADC reading. */
	float32 iMax = 0;
	if (xfmrSettings.iScale == 0)
		return VALUE_OOB;
								/* Calculate maximum current */
	iMax = ((VDDA - VSSA) * 0.001) * (16384 / xfmrSettings.iScale);
								/* Scale and de-normalise the measured load current and convert to float */
	*dcCurrent = _IQ24toF(xfmrNets.iSnsNet) * iMax;
	return 0;
}

Uint16 getDcMidVoltage (float32 * dcVoltage) {
	/* Reads the DC Mid voltage ADC reading. */
	float32 vMax = 0;
	if (xfmrSettings.midVScale == 0)
		return VALUE_OOB;
								/* Calculate maximum voltage */
	vMax = ((VDDA - VSSA) * 0.001) * (16384 / xfmrSettings.midVScale);
								/* Scale and de-normalise the measured load voltage and convert to float */
	*dcVoltage = _IQ24toF(xfmrNets.midVSnsNet) * vMax;
	return 0;
}

/*=============== DC HV ==============*/
Uint16 getDcHvVoltage (float32 * dcVoltage) {
	/* Reads the DC HV voltage ADC reading. */
	float32 vMax = 0;
	if (xfmrSettings.hvVScale == 0)
		return VALUE_OOB;
								/* Calculate maximum voltage */
	vMax = ((VDDA - VSSA) * 0.001) * (16384 / xfmrSettings.hvVScale);
								/* Scale and de-normalise the measured load voltage and convert to float */
	*dcVoltage = _IQ24toF(xfmrNets.hvVSnsNet) * vMax;
	return 0;
}

/*================ AC ================*/
// HOW TO GET THE PEAK VALUE? ADC VALUE COULD BE FROM ANYWHERE IN THE SINE WAVE - EVEN 0...
//Uint16 getDcMidCurrent (float32 * pkCurrent) {
//	/* Reads the AC current ADC reading. */
//	float32 iMax = 0;
//	if (xfmrSettings.iScale == 0)
//		return VALUE_OOB;
//								/* Calculate maximum current */
//	iMax = ((VDDA - VSSA) * 0.001) * (16384 / xfmrSettings.iScale);
//								/* Scale and de-normalise the measured load current and convert to float */
//	*dcCurrent = _IQ24toF(xfmrNets.iSnsNet) * iMax;
//	return 0;
//}
//
//Uint16 getDcMidVoltage (float32 * pkVoltage) {
//	/* Reads the AC voltage ADC reading. */
//	float32 vMax = 0;
//	if (xfmrSettings.midVScale == 0)
//		return VALUE_OOB;
//								/* Calculate maximum voltage */
//	vMax = ((VDDA - VSSA) * 0.001) * (16384 / xfmrSettings.midVScale);
//								/* Scale and de-normalise the measured load voltage and convert to float */
//	*dcVoltage = _IQ24toF(xfmrNets.midVSnsNet) * vMax;
//	return 0;
//}

//Uint16 adcSetLoadIScale (loadStage load, float32 scaleSetting) {
//	/* Sets iScale value for the specified load
//	 *  scaleSetting is expected in amps-per-volt
//	 */
//	float32 iMaxRms  = 0;
//	if (load >= numberOfLoads)						/* Check channel is valid */
//		return CHANNEL_OOB;
//	if ((scaleSetting <= 0) || (scaleSetting > 1))  /* Check scale is within normal limits */
//		return VALUE_OOB;
//													/* Check max (from scale) is within application limits */
//	iMaxRms = (1.0 / ((VDDA - VSSA) * 0.001)) * scaleSetting * RECP_SQRT_2;
//	if ((iMaxRms <= (_IQ10toF((int32) loadSettings[load].iMinRms))) || (iMaxRms > (_IQ10toF((int32) loadSettings[load].iMaxRms))))
//		return VALUE_OOB;
//
//	loadSettings[load].iScale = _SQ14(scaleSetting);/* Save as Q format*/
//	return 0;
//}
//
//Uint16 adcSetLoadVScale (loadStage load, float32 scaleSetting) {
//	/* Sets vScale value for the specified load
//	 *  scaleSetting is expected in volts-per-volt
//	 */
//	float32 vMaxRms  = 0;
//	if (load >= numberOfLoads)						/* Check channel is valid */
//		return CHANNEL_OOB;
//	if ((scaleSetting <= 0.0) || (scaleSetting > 1))/* Check scale is within normal limits */
//		return VALUE_OOB;
//													/* Check max (from scale) is within application limits */
//	vMaxRms = ( 1.0 / ((VDDA - VSSA) * 0.001)) * scaleSetting * RECP_SQRT_2;
//	if ((vMaxRms <= (_IQ10toF((int32) loadSettings[load].vMinRms))) || (vMaxRms > (_IQ10toF((int32) loadSettings[load].vMaxRms))))
//		return VALUE_OOB;
//
//	loadSettings[load].vScale = _SQ14(scaleSetting);/* Save as Q format*/
//	return 0;
//}
//
//Uint16 adcGetLoadIScale (loadStage load, float32 * sclDest) {
//	/* Returns the current iScale value (amps-per-volt) for the specified load */
//	if (load >= numberOfLoads)
//		return CHANNEL_OOB;
//	*sclDest = _IQ14toF((int32)loadSettings[load].iScale);
//	return 0;
//}
//
//Uint16 adcGetLoadVScale (loadStage load, float32 * sclDest) {
//	/* Returns the current vScale value (volts-per-volt) for the specified load */
//	if (load >= numberOfLoads)
//		return CHANNEL_OOB;
//	*sclDest = _IQ14toF((int32) loadSettings[load].vScale);
//	return 0;
//}
