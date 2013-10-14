/*
 * Adc.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/*============= COMP/DAC SECTION ==============*/
void adcCompConfigure (void) {
	/* Sets up the COMP 1 & 2 comparators using the internal DACs at inverting inputs
	 *  - SHOULD BE CALLED AFTER adcSocCnf() -
	 *  - SHOULD BE CALLED BEFORE PWMS (SYNC) ARE STARTED -
	 *  - SHOULD BE CALLED BEFORE pwmTZConfigure() -
	 */
	EALLOW;									/* COMPCTL & DACCTL are EALLOW protected */
	Comp1Regs.COMPCTL.bit.COMPDACEN = 1;	/* Enable COMP1 section */
	Comp1Regs.DACVAL.bit.DACVAL = 0x3FF;	/* Set the DAC Value to the maximum value to begin with */
	Comp1Regs.COMPCTL.bit.COMPSOURCE = 0;	/* COMP1 inverting i/p is connected to internal DAC */
	Comp1Regs.COMPCTL.bit.CMPINV = 0; 		/* COMP1 o/p is not inverted */
	Comp1Regs.COMPCTL.bit.QUALSEL = 3;		/* COMP1 o/p has no qualification window length */
	Comp1Regs.COMPCTL.bit.SYNCSEL = 1;		/* COMP1 o/p is not synchronised to the SysClk */
	Comp1Regs.DACCTL.bit.DACSOURCE = 0;		/* Set DACVAL as DAC control source */

	Comp2Regs.COMPCTL.bit.COMPDACEN = 1;	/* Do the same for COMP2 */
	Comp2Regs.DACVAL.bit.DACVAL = 0x3FF;
	Comp2Regs.COMPCTL.bit.COMPSOURCE = 0;
	Comp2Regs.COMPCTL.bit.CMPINV = 0;
	Comp2Regs.COMPCTL.bit.QUALSEL = 3;
	Comp2Regs.COMPCTL.bit.SYNCSEL = 1;
	Comp2Regs.DACCTL.bit.DACSOURCE = 0;
	EDIS;
}

Uint16 adcSetDac (Uint16 chnl, float32 dacLvl) {
	// TODO: Rewrite to two separate functions, one for AC stage ISns and one for Xfmr stage VSns
	//TODO ramp ADC?
	/* Sets the parameters of the DAC
	 * dacLvl is expected in amps or volts
	 *  - I & V SCALE MUST BE SET PREVIOUSLY -
	 *  - SHOULD BE CALLED AFTER adcInitComp() -
	 */
	if (dacLvl < 0)			/* Check requested voltage peak is within lower boundary */
		return VALUE_OOB;
							/* Scale and calculate DAC level setting */
							/* DACVAL = (Lvl * scale * 1023) / 3.3, but here scale is SQ14 so we can just rsh 4 instead of *1023 */
	if (chnl > NUM_CHNLS)	/* Check channel is valid */
			return CHANNEL_OOB;
							/* Normalise */
	dacLvl *= (1.0 / ((VDDA - VSSA) * 0.001));
							/* Perform the scaling based on the channel control type */
	if (channel[chnl].ctlMode == iCtrl)
		dacLvl *= (float32)(channel[chnl].vScale >> 4);
	else
		dacLvl *= (float32)(channel[chnl].iScale >> 4);

	if (dacLvl > 1023.0)	/* Check requested value is within upper boundary */
		return VALUE_OOB;

	if (chnl == DC_STAGE) {	/* Round and cast the level into relevant COMP DACVAL */
		Comp1Regs.DACVAL.all = (Uint16)(dacLvl + 0.5);
	} else if (chnl == AC_STAGE) {
		Comp2Regs.DACVAL.all = (Uint16)(dacLvl + 0.5);
	} else {
		return CHANNEL_OOB;
	}
	return 0;
}

Uint16 adcGetDac (Uint16 chnl, float32 *dacDest) {
	// TODO: Rewrite to two separate functions, one for AC stage ISns and one for Xfmr stage VSns
	float32 dacLvl = 0;

	if (chnl > NUM_CHNLS)	/* Check channel is valid */
		return CHANNEL_OOB;

	if (chnl == DC_STAGE) {	/* Get the raw data from the relevant comparator peripheral and de-normalise */
		dacLvl = Comp1Regs.DACVAL.all * ((VDDA - VSSA) * 0.001);
	} else if (chnl == AC_STAGE) {
		dacLvl = Comp2Regs.DACVAL.all * ((VDDA - VSSA) * 0.001);
	} else {
		return CHANNEL_OOB;
	}
							/* Perform the scaling based on the channel control type */
	if (channel[chnl].ctlMode == iCtrl)
		*dacDest = dacLvl * (1.0 / (channel[chnl].vScale >> 4));
	else
		*dacDest = dacLvl * (1.0 / (channel[chnl].iScale >> 4));
	return 0;
}

/*================ ADC SECTION ================*/
void adcMacroConfigure (void) {
	/* This SHOULD be called after the PWMs have been configured (pwmMacroConfigure())
	 *  - SHOULD BE RUN AFTER pwmMacroConfigure() -
	 *  - SHOULD BE RUN BEFORE DPL_INIT() -
	 */
	adcSocCnf();			/* Configure Macro ADCs SOCs */
	pwmSocConfigure();		/* Configure PWM as SOC trigger */
}

void adcSocCnf(void) {
	/* Configures ADC SOC for ADC macro
	 *  - SHOULD BE RUN BEFORE DPL_INIT() -
	 */
	int16 ChSel[16]={1,1,3,6,7,2,12,9,11,14,15,0,10,4,0,0};	/* ADC channel selections */
	int16 TrigSel[16]={5,5,5,5,5,5,5,5,5,5,5,5,5,0,0};		/* ADC trigger selections */
	int16 ACQPS[16]={6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};		/* ADC qualification window sizes */
	ADC_SOC_CNF(ChSel, TrigSel, ACQPS, 16, 0);
}

Uint16 adcCheckOcp (void) {
	/* Over-current protection
	 *  - iScale AND ocp SHOULD BE SET BEFORE USE -
	 */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {			/* Compare all load ISns ADC values to their OCP limits */
		if (loadNets[i].iFdbkNet > loadSettings[i].ocpLevel) {
			mnStopAll();
			return OCP_TRIP;
		}
	}
	if (xfmrNets.iSnsNet > xfmrSettings.ocpLevel) {	/* Compare the DC Mid ISns ADC value to its OCP limit */
		mnStopAll();
		return OCP_TRIP;
	}
	return 0;
}

Uint16 adcCheckOvp (void) {
	/* Over-voltage protection
	 *  - vScale AND OVP SHOULD BE SET BEFORE USE -
	 */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {				/* Compare all load VSns ADC values to their OVP limits */
		if (loadNets[i].vFdbkNet > loadSettings[i].ovpLevel) {
			mnStopAll();
			return OVP_TRIP;
		}
	}
	if (xfmrNets.hvVSnsNet > xfmrSettings.hvOvpLevel){	/* Compare the DC HV VSns ADC value to its OVP limit */
		mnStopAll();
		return OVP_TRIP;
	}
	if (getSlaveMode() == master) {						/* Check if the system is in master mode */
		if (acNets.vFdbkNet > acSettings.ovpLevel) {	/* Compare the AC VSns ADC value to its OVP limit */
			mnStopAll ();
			return OVP_TRIP;
		}
	}
	return 0;
}

Uint16 adcCheckOpp (void) {
	/* Over-power protection
	 *  - vScale AND iScale SHOULD BE SET BEFORE USE -
	 */
	Uint16 i = 0;
	int32 vMeas = 0, iMeas = 0;
	float32 iLimDyn = 0;

	for (i = 0; i < numberOfLoads; i++) {

		/* Get the most recent vSns reading for the given load and multiply by the related vScale to get the real value (IQ24). */
		vMeas = _IQmpy(loadNets[i].vFdbkNet, _Q14toIQ(loadSettings[i].vScale));

		// TODO: CONVERT iLIM to IQ24 or CONVERT vMeas AND iMeas TO FLOAT.
		iLimDyn = LOAD_OPPLVL_FIX / vMeas;	/* Divide real V value into the power limit value to get the dynamic I limit. */

		/* Get the most recent iSns reading for the given load and multiply by the related iScale to get the real value (IQ24). */
		iMeas = _IQmpy(loadNets[i].iFdbkNet, _Q14toIQ(loadSettings[i].iScale));


		if (iMeas > iLimDyn) {	/* Check the measured current value is below the dynamic current limit value. */
			mnStopAll();
			return OPP_TRIP;
		}
	}

	// TODO: ADD OPP CHECK FOR XFMR AND AC

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

Uint16 adcSetLoadOcp (loadStage load, float32 ocpSetting) {
	/* Sets OCP value for the specified load
	 *  ocpSetting is expected in amps
	 *  - iScale SHOULD BE SET BEFORE OCP -
	 */
	float32 iMax = 0;
	int32 iStRms = 0;

	if (load >= numberOfLoads)				/* Check channel is valid */
		return CHANNEL_OOB;
	if (loadSettings[load].iScale == 0)		/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;
													/* Convert scale from SQ to float */
	iMax = _IQ14toF((int32) loadSettings[load].iScale);
	iMax = ((VDDA - VSSA) * 0.001) * (1.0 / iMax); 	/* Calculate maximum I */
	iStRms = _IQ10(ocpSetting * RECP_SQRT_2);		/* Convert setting to RMS Q10 and check result is in range */
	if ((iStRms <= loadSettings[load].iMinRms) && (iStRms > loadSettings[load].iMaxRms))
		return VALUE_OOB;
													/* Normalise and save */
	loadSettings[load].ocpLevel = _IQ24(ocpSetting / iMax);
	return 0;
}

Uint16 adcSetMidOcp (float32 ocpSetting) {
	/* Sets the OCP value for the DC MID ISns
	 * ocpSetting is expected in amps
	 */
	float32 iMax = 0;
	int32 iStRms = 0;
	iMax = _IQ14toF((int32) xfmrSettings.iScale);	/* Convert scale from SQ to float */
	iMax = ((VDDA - VSSA) * 0.001) * (1.0 / iMax);	/* Calculate maximum I */
	iStRms = _IQ10(ocpSetting * RECP_SQRT_2);		/* Convert setting to RMS Q10 and check result is in range */
	if ((iStRms <= xfmrSettings.iMinRms) && (iStRms > xfmrSettings.iMaxRms))
		return VALUE_OOB;
	xfmrSettings.ocpLevel = _IQ24(ocpSetting / iMax);/* Normalise and save */
	return 0;
}

Uint16 adcSetAcOcp (float32 ocpSetting) {
	/* Sets the OCP value for the AC ISns
	 * ocpSetting expected in amps
	 */
	float32 iMax = 0;
	int32 iStRms = 0;
	iMax = _IQ14toF((int32) acSettings.iScale);		/* Convert scale from SQ to float */
	iMax = ((VDDA - VSSA) * 0.001) * (1.0 / iMax);	/* Calculate maximum I */
	iStRms = _IQ10(ocpSetting * RECP_SQRT_2);		/* Convert setting to RMS Q10 and check result is in range */
	if ((iStRms <= acSettings.iMinRms) && (iStRms > acSettings.iMaxRms))
		return VALUE_OOB;
	acSettings.ocpLevel = _IQ24(ocpSetting / iMax);	/* Normalise and save */

	//TODO: Update AC OCP ISns DAC value?

	return 0;
}

Uint16 adcSetLoadOvp (loadStage load, float32 ovpSetting) {
	/* Sets OVP value for the specified load
	 *  ovpSetting is expected in volts
	 *  - vScale SHOULD BE SET BEFORE OVP -
	 */
	float32 vMax = 0;
	int32 vStRms = 0;

	if (load >= numberOfLoads)				/* Check channel is valid */
		return CHANNEL_OOB;
	if (loadSettings[load].vScale == 0)		/* Check vScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	vMax = _IQ14toF((int32) loadSettings[load].vScale);		/* Convert scale from SQ to float */
	vMax = ((VDDA - VSSA) * 0.001) * (1.0 / vMax);	/* Calculate maximum V */
	vStRms = _IQ10(ovpSetting * RECP_SQRT_2);		/* Convert setting to RMS Q10 and check result is in range */
	if ((vStRms <= loadSettings[load].vMinRms) && (vStRms > loadSettings[load].vMaxRms))
		return VALUE_OOB;

	loadSettings[load].ovpLevel = _IQ24(ovpSetting / vMax);	/* Normalise */
	return 0;
}

Uint16 adcSetMidOvp (float32 ovpSetting) {
	/* Sets OVP value for the DC Mid VSns
	 * ovpSetting is expected in volts
	 */
	float32 vMax = 0;
	int32 vStRms = 0;
	vMax = _IQ14toF((int32) xfmrSettings.midVScale);	/* Convert scale from SQ to float */
	vMax = ((VDDA - VSSA) * 0.001) * (1.0 / vMax);		/* Calculate maximum V */
	vStRms = _IQ10(ovpSetting * RECP_SQRT_2);			/* Convert setting to RMS Q10 and check result is in range */
	if ((vStRms <= xfmrSettings.midVMinRms) && (vStRms > xfmrSettings.midVMaxRms))
		return VALUE_OOB;
	xfmrSettings.midOvpLevel = _IQ24(ovpSetting / vMax);/* Normalise */

	//TODO: Update Mid OVP VSns DAC value?

	return 0;
}

Uint16 adcSetHvOvp (float32 ovpSetting) {
	/* Sets OVP value for the DC HV VSns
	 * ovpSetting is expected in volts
	 */
	float32 vMax = 0;
	int32 vStRms = 0;
	vMax = _IQ14toF((int32) xfmrSettings.hvVScale);		/* Convert scale from SQ to float */
	vMax = ((VDDA - VSSA) * 0.001) * (1.0 / vMax);		/* Calculate maximum V */
	vStRms = _IQ10(ovpSetting * RECP_SQRT_2);			/* Convert setting to RMS Q10 and check result is in range */
	if ((vStRms <= xfmrSettings.hvVMinRms) && (vStRms > xfmrSettings.hvVMaxRms))
		return VALUE_OOB;
	xfmrSettings.hvOvpLevel = _IQ24(ovpSetting / vMax);	/* Normalise */
	return 0;
}

Uint16 adcSetAcOvp (float32 ovpSetting) {
	if (getSlaveMode() == master)
		return CHANNEL_OOB;

	// TODO: ...
	return 0;
}

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

Uint16 adcGetLoadVoltage (loadStage load, float32 * vDest) {
	/* Reads the Voltage ADC reading. */
	float32 vltg = 0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;
	vltg = _IQ24toF(loadNets[load].vFdbkNet);					/* Get the most recent reading. */
	*vDest = vltg * _IQ14toF((int32) loadSettings[load].vScale);/* Multiply reading by scaling factor */
	return 0;
}

Uint16 adcGetLoadCurrent (loadStage load, float32 * iDest) {
	/* Reads the Current ADC reading. */
	float32 curr = 0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;
	curr = _IQ24toF(loadNets[load].iFdbkNet);					/* Get the most recent reading. */
	*iDest = curr * _IQ14toF((int32) loadSettings[load].iScale);/* Multiply reading by scaling factor */
	return 0;
}

Uint16 adcGetLoadOcp (loadStage load, float32 * ocpDest) {
	/* Returns current OCP limit, for the specified load,
	 *  based on actual OCP and iScale
	 *  - iScale SHOULD BE SET BEFORE OCP USE -
	 */
	float32 iMax = 0;
	if (load >= numberOfLoads)				/* Check channel is valid */
		return CHANNEL_OOB;
	if (loadSettings[load].iScale == 0)		/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;
	iMax = ((VDDA - VSSA) * 0.001) * (16384.0 / loadSettings[load].iScale); /* Calculate maximum I */
	*ocpDest = ((_IQ24toF(loadSettings[load].ocpLevel)) * iMax);			/* De-normalise */
	return 0;
}

Uint16 adcGetLoadOvp (loadStage load, float32 * ovpDest) {
	/* Returns current OVP limit, for the specified load,
	 *  based on actual OVP and vScale
	 *  - vScale SHOULD BE SET BEFORE OVP USE -
	 */
	float32 vMax = 0;
	if (load >= numberOfLoads)				/* Check channel is valid */
		return CHANNEL_OOB;
	if (loadSettings[load].vScale == 0)		/* Check vScale is set, to avoid divide-by-0 exception */
		return VALUE_OOB;
	vMax = ((VDDA - VSSA) * 0.001) * (16384.0 / loadSettings[load].vScale); 	/* Calculate maximum V */
	*ovpDest = ((_IQ24toF(loadSettings[load].ovp)) * vMax);		/* De-normalise */

	//*ovpDest = (((_IQ24toF(channel[chnl].ovp)) * vMax) * (16384.0 / channel[chnl].vGainLmt));	/* De-normalise and de-gain */
	return 0;
}
