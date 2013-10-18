/*
 * Adc.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

void adcMacroConfig (void) {
	/* This SHOULD be called after the PWMs have been configured (pwmMacroConfigure())
	 *  - SHOULD BE RUN AFTER pwmMacroConfigure() -
	 *  - SHOULD BE RUN BEFORE DPL_INIT() -
	 */
	adcSocConfig();			/* Configure Macro ADCs SOCs */
	pwmSocConfigure();		/* Configure PWM as SOC trigger */
}

void adcSocConfig(void) {
	/* Configures ADC SOC for ADC macro
	 *  - SHOULD BE RUN BEFORE DPL_INIT() -
	 */
	int16 ChSel[16]={1,1,3,6,7,2,12,9,11,14,15,0,10,4,0,0};	/* ADC channel selections */
	int16 TrigSel[16]={5,5,5,5,5,5,5,5,5,5,5,5,5,0,0};		/* ADC trigger selections */
	int16 ACQPS[16]={6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};		/* ADC qualification window sizes */
	ADC_SOC_CNF(ChSel, TrigSel, ACQPS, 16, 0);
}

Uint16 getLoadVoltage (loadStage load, float32 * vDest) {
	/* Reads the Voltage ADC reading. */
	float32 vltg = 0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;
	vltg = _IQ24toF(loadNets[load].vFdbkNet);					/* Get the most recent reading. */
	*vDest = vltg * _IQ14toF((int32) loadSettings[load].vScale);/* Multiply reading by scaling factor */
	return 0;
}

Uint16 getLoadCurrent (loadStage load, float32 * iDest) {
	/* Reads the Current ADC reading. */
	float32 curr = 0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;
	curr = _IQ24toF(loadNets[load].iFdbkNet);					/* Get the most recent reading. */
	*iDest = curr * _IQ14toF((int32) loadSettings[load].iScale);/* Multiply reading by scaling factor */
	return 0;
}

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
