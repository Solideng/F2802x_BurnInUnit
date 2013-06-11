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
	//Comp1Regs.COMPCTL.bit.QUALSEL = 0;		/* COMP1 o/p has no qualification window length */
	Comp1Regs.COMPCTL.bit.QUALSEL = 3;		/* COMP1 o/p has no qualification window length */
	//Comp1Regs.COMPCTL.bit.SYNCSEL = 0;		/* COMP1 o/p is not synchronised to the SysClk */
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
	for (i = 0; i < NUM_CHNLS; i++) {	/* Compare all current ADC values to OCP limit */
		if (channel[i].iFdbkNet > channel[i].ocp) {
			mnStopAll();
			return OCP_TRIP;
		}
	}
	return 0;
}

Uint16 adcCheckOvp (void) {
	/* Over-voltage protection
	 *  - vScale and ovp SHOULD BE SET BEFORE USE -
	 */
	Uint16 i = 0;
	for (i = 0; i < (NUM_CHNLS + 1); i++) {	/* Compare all voltage ADC values to OVP limit */
		if (channel[i].vFdbkNet > channel[i].ovp) {
			mnStopAll();
			return OVP_TRIP;
		}
	}
	return 0;
}

Uint16 adcSetIScale (Uint16 chnl, float32 scaleSetting) {
	/* Sets iScale value for the specified channel
	 *  scaleSetting is expected in amps-per-volt
	 */
	volatile float32 iMaxRms  = 0;		// TODO check is volatile needed here?
	if (chnl > NUM_CHNLS)							/* Check channel is valid */
		return CHANNEL_OOB;
	if ((scaleSetting <= 0) || (scaleSetting > 1))  /* Check scale is within normal limits */
		return VALUE_OOB;
													/* Check max (from scale) is within application limits */
	iMaxRms = (1.0 / ((VDDA - VSSA) * 0.001)) * scaleSetting * RECP_SQRT_2;
	if ((iMaxRms <= (_IQ10toF((int32)channel[chnl].iMinRms))) || (iMaxRms > (_IQ10toF((int32)channel[chnl].iMaxRms))))
		return VALUE_OOB;

	channel[chnl].iScale = _SQ14(scaleSetting);		/* Save as Q format*/
	return 0;
}

Uint16 adcSetVScale (Uint16 chnl, float32 scaleSetting) {
	/* Sets vScale value for the specified channel
	 *  scaleSetting is expected in volts-per-volt
	 */
	float32 vMaxRms  = 0;
	if (chnl > (NUM_CHNLS + 1))						/* Check channel is valid */
		return CHANNEL_OOB;
	if ((scaleSetting <= 0.0) || (scaleSetting > 1))/* Check scale is within normal limits */
		return VALUE_OOB;
													/* Check max (from scale) is within application limits */
	vMaxRms = ( 1.0 / ((VDDA - VSSA) * 0.001)) * scaleSetting * RECP_SQRT_2;
	if ((vMaxRms <= (_IQ10toF((int32)channel[chnl].vMinRms))) || (vMaxRms > (_IQ10toF((int32)channel[chnl].vMaxRms))))
		return VALUE_OOB;

	channel[chnl].vScale = _SQ14(scaleSetting);		/* Save as Q format*/
	return 0;
}

Uint16 adcSetOcp (Uint16 chnl, float32 ocpSetting) {
	/* Sets fOcp and ocp values for the specified channel
	 *  ocpSetting is expected in amps
	 *  - iScale SHOULD BE SET BEFORE OCP -
	 */
	float32 iMax = 0;
	int32 iStRms = 0;

	if (chnl > NUM_CHNLS)					/* Check channel is valid */
			return CHANNEL_OOB;
	if (channel[chnl].iScale == 0)			/* Check iScale is set, to avoid div-by-0 exception */
			return VALUE_OOB;

	iMax = _IQ14toF((int32)channel[chnl].iScale);	/* Convert scale from SQ to float */

	iMax = ((VDDA - VSSA) * 0.001) * (1.0 / iMax); 	/* Calculate maximum I */

	iStRms = _IQ10(ocpSetting * RECP_SQRT_2);		/* Convert setting to RMS Q10 and compare check result is in range */
	if ((iStRms <= channel[chnl].iMinRms) && (iStRms > channel[chnl].iMaxRms))
		return VALUE_OOB;
											/* Apply gain and normalise */
	channel[chnl].ocp = _IQ24(ocpSetting / iMax);
	return 0;
}

Uint16 adcSetOvp (Uint16 chnl, float32 ovpSetting) {
	/* Sets fOvp and ovp values for the specified channel
	 *  ovpSetting is expected in volts
	 *  - vScale SHOULD BE SET BEFORE OVP -
	 */
	float32 vMax = 0, vGn = 0;
	int32 vStRms = 0;

	if (chnl > (NUM_CHNLS + 1))				/* Check channel is valid */
		return CHANNEL_OOB;
	if (channel[chnl].vScale == 0)			/* Check vScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;

	vMax = _IQ14toF((int32)channel[chnl].vScale);	/* Convert scale and gain from SQ to float */
	vGn = _IQ14toF((int32)channel[chnl].vGainLmt);

	vMax = ((VDDA - VSSA) * 0.001) * (1.0 / vMax);	/* Calculate maximum V */

	vStRms = _IQ10(ovpSetting * RECP_SQRT_2);		/* Convert setting to Q10 and compare check result is in range */
	if ((vStRms <= channel[chnl].vMinRms) && (vStRms > channel[chnl].vMaxRms))
		return VALUE_OOB;
											/* Apply gain and normalise */
	channel[chnl].ovp = _IQ24((ovpSetting * vGn) / vMax);
	return 0;
}

Uint16 adcGetIScale (Uint16 chnl, float32 * sclDest) {
	/* Returns the current iScale value (amps-per-volt) for the specified channel */
	if (chnl > NUM_CHNLS)
		return CHANNEL_OOB;
	*sclDest = _IQ14toF((int32)channel[chnl].iScale);
	return 0;
}

Uint16 adcGetVScale (Uint16 chnl, float32 * sclDest) {
	/* Returns the current vScale value (volts-per-volt) for the specified channel */
	if (chnl > (NUM_CHNLS + 1))
		return CHANNEL_OOB;
	*sclDest = _IQ14toF((int32)channel[chnl].vScale);
	return 0;
}

Uint16 adcGetOcp (Uint16 chnl, float32 * ocpDest) {
	/* Returns current OCP limit, for the specified channel,
	 *  based on actual ocp and iScale
	 *  - iScale SHOULD BE SET BEFORE OCP USE -
	 */
	float32 iMax = 0;
	if (chnl > NUM_CHNLS)								/* Check channel is valid */
		return CHANNEL_OOB;
	if (channel[chnl].iScale == 0)						/* Check iScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;
	iMax = ((VDDA - VSSA) * 0.001) * (16384.0 / channel[chnl].iScale); 	/* Calculate maximum I */
	*ocpDest = ((_IQ24toF(channel[chnl].ocp)) * iMax);	/* De-normalise */
	return 0;
}

Uint16 adcGetOvp (Uint16 chnl, float32 * ovpDest) {
	/* Returns current OVP limit, for the specified channel,
	 *  based on actual ovp and vScale
	 *  - vScale SHOULD BE SET BEFORE OVP USE -
	 */
	float32 vMax = 0;
	if (chnl > (NUM_CHNLS + 1))							/* Check channel is valid */
		return CHANNEL_OOB;
	if (channel[chnl].vScale == 0)						/* Check vScale is set, to avoid div-by-0 exception */
		return VALUE_OOB;
	vMax = ((VDDA - VSSA) * 0.001) * (16384.0 / channel[chnl].vScale); 	/* Calculate maximum V */
	*ovpDest = (((_IQ24toF(channel[chnl].ovp)) * vMax) * (16384.0 / channel[chnl].vGainLmt));	/* De-normalise and de-gain */
	return 0;
}
