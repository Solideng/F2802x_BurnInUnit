/*
 * MacroNets.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

Uint16	stopAllFlag = 0;	// TODO May be moved to SCPI device specific regs
Uint16	enableAllFlag = 0;

loadStageNets 		loadNets[numberOfLoads];	/* Load nets */
xfmrStageNets		xfmrNets;					/* Transformer stage nets */
acStageNets 		acNets;						/* AC stage nets */
loadStageSettings 	loadSettings[numberOfLoads];/* Load settings */
xfmrStageSettings	xfmrSettings;				/* Transformer stage nets */
acStageSettings 	acSettings;					/* AC stage settings */
extDeviceSettings 	extSettings;				/* External device settings */

#if (INCR_BUILD == 1)
	volatile int32 duty[NUM_CHNLS] = {0};	/* Open-loop duty setting for use in watch window */
#endif

static volatile int32 sgenSignNet = 0;		/* Net for sine generator sign output NET */


static void initSettings (void) {
	/* Initialise all settings structures with default values */
	/* FOR SOME REASON THIS *MUST* BE CALLED *AFTER* pwmMacroConfig(), NOT BEFORE */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].slewRate   = 16777;		/* 0.001 Q24 */
		loadSettings[i].slewTarget = 0;			/* 0 Q24 */
		loadSettings[i].ocpLevel   = 587202560;	/* 35A variable Q24 */
		loadSettings[i].otpLevel   = 19200;		/* 150 degree C Q7 */
		loadSettings[i].iScale     = _SQ14(LOAD_I_SCALE);/* Q14 */
		loadSettings[i].vScale     = _SQ14(LOAD_V_SCALE);
		loadSettings[i].enable     = FALSE;		/* Disabled */
	}

	xfmrSettings.ocpLevel 	 = 16777216;	/* Maximum Q24 */
	//xfmrSettings.midOvpLevel = 16777216;	/* Maximum Q24 */ //Fixed to 400 Vpk, is that correct?
	xfmrSettings.hvOvpLevel  = 16777216;	/* Maximum Q24 */
	xfmrSettings.otpLevel 	 = 19200;		/* 150 degree C Q7*/
	xfmrSettings.iMax	 	 = 15360;		/* 15 Amps RMS Q10 */
	xfmrSettings.iScale 	 = _SQ14(MID_I_SCALE);	/* Q14 */
	xfmrSettings.midVMax	 = 15360;		/* 15 Amps RMS Q10 */
	xfmrSettings.midVScale 	 = _SQ14(MID_V_SCALE);
	xfmrSettings.hvVMax  	 = 15360;		/* 15 Volts Q10 */
	xfmrSettings.hvVScale 	 = _SQ14(HV_V_SCALE);
	xfmrSettings.enable 	 = FALSE;		/* Disabled */

	acSettings.gainRate   = 16777;		/* 0.001 Q24 */
	acSettings.gainTarget = 0;			/* 0 Q24 */
	acSettings.ocpLevel   = 16777216;	/* Maximum Q24 */
	acSettings.ovpLevel   = 16777216;	/* Maximum Q24 */
	acSettings.otpLevel   = 19200;		/* 150 degree C Q7 */
	acSettings.iMaxRms    = 15360;		/* 15 amps (RMS) Q10 SQ10 */
	acSettings.vMaxRms    = 15360;		/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
	acSettings.iScale     = _SQ14(LOAD_I_SCALE);/* Q14 */
	acSettings.vScale     = 32767;		/* 1.0f volts-per-volt Q14 */
	acSettings.enable     = FALSE;		/* FALSE | TRUE */
	acSettings.mode       = masterUnit;	/* master | slave */
	acSettings.vScale     = _SQ14(AC_V_SCALE);
	acSettings.vGainLmt   = _SQ14(0.99);	/* 1.0 gain limit Q14 */

	extSettings.ext1OtpLevel = 19200;	/* 150 degree C Q7 */
	extSettings.ext2OtpLevel = 19200;	/* 150 degree C Q7 */
	extSettings.extFanEnable = FALSE;	/* Disabled */
	extSettings.extPsuEnable = FALSE;	/* Disabled */
}

static void initNets (void) {
	/* Clears all nets. */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadNets[i].iFdbkNet 	= 0;	/* Q24 (IQ24: -128 - +128) */
		loadNets[i].vFdbkNet 	= 0;	/* Q24 */
		loadNets[i].iRefNet 	= 0;	/* Q24 */
		loadNets[i].iFiltOutNet = 0;	/* Q24 */
	}

	acNets.iFdbkNet 	= 0;	/* Q24 */
	acNets.vFdbkNet 	= 0;	/* Q24 */
	acNets.vRefNet 		= 0;	/* Q24 */
	acNets.iRefNet 		= 0;	/* Q24 */
	acNets.iFiltOutNet 	= 0;	/* Q24 */

	xfmrNets.iSnsNet 	= 0;	/* Q24 */
	xfmrNets.midVSnsNet = 0;	/* Q24 */
	xfmrNets.hvVSnsNet 	= 0;	/* Q24 */
	xfmrNets.pwmDutyNet = _IQ24(0.5);	/* 50% Q24 */
}

static void connectLoadNets (void) {
				/* Load 1 */
	ADCDRV_1ch_Rlt1 = &loadNets[load1].iFdbkNet;	/* Load I Sns */
	ADCDRV_1ch_Rlt7 = &loadNets[load1].vFdbkNet;	/* Load V Sns */
	CNTL_2P2Z_Ref1 	= &loadNets[load1].iRefNet;		/* ICNTL reference */
	CNTL_2P2Z_Fdbk1 = &loadNets[load1].iFdbkNet;	/* ICNTL feedback */
	CNTL_2P2Z_Out1 	= &loadNets[load1].iFiltOutNet;	/* ICNTL out */
	CNTL_2P2Z_Coef1 = &loadICoefs[load1].b2;		/* ICNTL coefficients */
	PWMDRV_2ch_UpCnt_Duty1A = &loadNets[load1].iFiltOutNet;	/* Load PWM */
				/* Load 2 */
	ADCDRV_1ch_Rlt2 = &loadNets[load2].iFdbkNet;	/* Load I Sns */
	ADCDRV_1ch_Rlt8 = &loadNets[load2].vFdbkNet;	/* Load V Sns */
	CNTL_2P2Z_Ref2 	= &loadNets[load2].iRefNet;		/* ICNTL reference */
	CNTL_2P2Z_Fdbk2 = &loadNets[load2].iFdbkNet;	/* ICNTL feedback */
	CNTL_2P2Z_Out2 	= &loadNets[load2].iFiltOutNet;	/* ICNTL out */
	CNTL_2P2Z_Coef2 = &loadICoefs[load2].b2;		/* ICNTL coefficients */
	PWMDRV_2ch_UpCnt_Duty1B = &loadNets[load2].iFiltOutNet;	/* Load PWM */
				/* Load 3 */
	ADCDRV_1ch_Rlt3 = &loadNets[load3].iFdbkNet;	/* Load I Sns */
	ADCDRV_1ch_Rlt9 = &loadNets[load3].vFdbkNet;	/* Load V Sns */
	CNTL_2P2Z_Ref3 	= &loadNets[load3].iRefNet;		/* ICNTL reference */
	CNTL_2P2Z_Fdbk3 = &loadNets[load3].iFdbkNet;	/* ICNTL feedback */
	CNTL_2P2Z_Out3 	= &loadNets[load3].iFiltOutNet;	/* ICNTL out */
	CNTL_2P2Z_Coef3 = &loadICoefs[load3].b2;		/* ICNTL coefficients */
	PWMDRV_2ch_UpCnt_Duty2A = &loadNets[load3].iFiltOutNet;	/* Load PWM */
				/* Load 4 */
	ADCDRV_1ch_Rlt4  = &loadNets[load4].iFdbkNet;	/* Load I Sns */
	ADCDRV_1ch_Rlt10 = &loadNets[load4].vFdbkNet;	/* Load V Sns */
	CNTL_2P2Z_Ref4 	 = &loadNets[load4].iRefNet;	/* ICNTL reference */
	CNTL_2P2Z_Fdbk4  = &loadNets[load4].iFdbkNet;	/* ICNTL feedback */
	CNTL_2P2Z_Out4 	 = &loadNets[load4].iFiltOutNet;/* ICNTL out */
	CNTL_2P2Z_Coef4  = &loadICoefs[load4].b2;		/* ICNTL coefficients */
	PWMDRV_2ch_UpCnt_Duty2B = &loadNets[load4].iFiltOutNet;	/* Load PWM */
}

static void connectXfmrNets (void) {
	ADCDRV_1ch_Rlt5  = &xfmrNets.iSnsNet;			/* DC Mid I Sns */
	ADCDRV_1ch_Rlt11 = &xfmrNets.hvVSnsNet;			/* DC HV V Sns */
	ADCDRV_1ch_Rlt13 = &xfmrNets.midVSnsNet;		/* DC Mid V Sns */
	PWMDRV_2ch_UpCnt_Duty3A = &xfmrNets.pwmDutyNet;	/* Xfmr PWM */
}

static void connectAcNets (slaveMode mode) {
	#ifndef DUAL_CNTL_AC
		ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;	/* AC V Sns */
		SGENTI_1ch_VOut  = &acNets.vRefNet;		/* Sine gen out */
		CNTL_3P3Z_Ref1 	 = &acNets.vRefNet;		/* VCNTL reference */
		CNTL_3P3Z_Fdbk1  = &acNets.vFdbkNet;	/* VCNTL feedback */
		CNTL_3P3Z_Out1   = &acNets.iRefNet;		/* VCNTL out */
		CNTL_3P3Z_Coef1  = &acVCoefs.b3;		/* VCNTL coefficients */
		ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;	/* AC I Sns */
		#ifndef AC_STAGE_OPEN
			PWMDRV_2ch_UpCnt_Duty3B = &acNets.iRefNet;/* AC F B PWM */
		#else
			PWMDRV_2ch_UpCnt_Duty3B = &acNets.vRefNet;
		#endif
	#else
		if (mode != slaveUnit) {
			ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;/* AC V Sns */
			SGENTI_1ch_VOut  = &acNets.vRefNet;	/* Sine gen out */

			CNTL_3P3Z_Ref1   = &acNets.vRefNet;	/* VCNTL reference */
			CNTL_3P3Z_Fdbk1  = &acNets.vFdbkNet;/* VCNTL feedback */
			CNTL_3P3Z_Out1   = &acNets.iRefNet;	/* VCNTL out/CNTLI in */
			CNTL_3P3Z_Coef1  = &acVCoefs.b3;	/* VCNTL coefficients */
		} else {
			ADCDRV_1ch_Rlt12 = &acNets.iRefNet;	/* AC Ext I Sns */
		}

		ADCDRV_1ch_Rlt6 = &acNets.iFdbkNet;		/* AC I Sns */
		CNTL_2P2Z_Ref5  = &acNets.iRefNet;		/* ICNTL reference */
		CNTL_2P2Z_Fdbk5 = &acNets.iFdbkNet;		/* ICNTL feedback */
		CNTL_2P2Z_Out5  = &acNets.iFiltOutNet;	/* ICNTL out */
		CNTL_2P2Z_Coef5 = &acICoefs.b2;			/* ICNTL coefficients */
		PWMDRV_2ch_UpCnt_Duty3B = &acNets.iFiltOutNet;/* AC F B PWM */
	#endif
}

static connectAllNets (slaveMode mode) {
	/* Connect all macro terminals to nets.
	 *  - SHOULD BE RUN AFTER DPL_INIT()
	 */
	#if (INCR_BUILD == 2)
		connectLoadNets();
		connectXfmrNets();
		connectAcNets(mode);
	#elif (INCR_BUILD == 1)
			/* Load 1 connections */
		ADCDRV_1ch_Rlt1 = &loadNets[load1].iFdbkNet;
		ADCDRV_1ch_Rlt7 = &loadNets[load1].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty1A = &duty[load1];
			/* Load 2 connections */
		ADCDRV_1ch_Rlt2 = &loadNets[load2].iFdbkNet;
		ADCDRV_1ch_Rlt8 = &loadNets[load2].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty1B = &duty[load2];
			/* Load 3 connections */
		ADCDRV_1ch_Rlt3 = &loadNets[load3].iFdbkNet;
		ADCDRV_1ch_Rlt9 = &loadNets[load3].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty2A = &duty[load3];
			/* Load 4 connections */
		ADCDRV_1ch_Rlt4  = &loadNets[load4].iFdbkNet;
		ADCDRV_1ch_Rlt10 = &loadNets[load4].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty2B = &duty[load4];
			/* Xfmr connections */
		ADCDRV_1ch_Rlt5  = &xfmrNets.iSnsNet;
		ADCDRV_1ch_Rlt11 = &xfmrNets.hvVSnsNet;
		ADCDRV_1ch_Rlt13 = &xfmrNets.midVSnsNet;
		PWMDRV_2ch_UpCnt_Duty3A = &duty[DC_STAGE];
			/* AC stage connections */
		ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;
		ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty3B = &duty[AC_STAGE];
	#endif
}

void setupNets (slaveMode mode) {
	/*  - SHOULD BE RUN AFTER DPL_INIT() */
	initSettings();
	initNets();
	connectAllNets(mode);
}

void stopAll (void) {
	Uint16 i = 0;
	resetEnableControl();	/* Disable all stages */ //TODO: OR DOES THIS NEED TO BE ORDERED??
	disableCircuit(chan1);
	disableCircuit(chan2);
	disableCircuit(chan3);
	disableCircuit(chan4);
	disableCircuit(acCct);
	disableCircuit(xfmrCct);
	disableCircuit(psu);
	disableCircuit(fan);
							/* Zero all IIR references and flag each stage as disabled */
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = FALSE;
		loadNets[i].iRefNet = 0;
	}
	xfmrSettings.enable = FALSE;
	xfmrNets.pwmDutyNet = 0;
	acSettings.enable = FALSE;
	acNets.vRefNet = 0;
	acNets.iRefNet = 0;
	extSettings.extPsuEnable = FALSE;
	extSettings.extFanEnable = FALSE;
	stopAllFlag = 0;
}

Uint16 runAll (void) {
	Uint16 i = 0;
	float32 meas = 0;

	// TODO: Can move disable section to stopALL() in place off resetEnableControl() then just call stopAll() ??
	disableCircuit(chan1);	/* Ensure channel 1 is disabled */
	disableCircuit(chan2);	/* Ensure channel 2 is disabled */
	disableCircuit(chan3);	/* Ensure channel 3 is disabled */
	disableCircuit(chan4);	/* Ensure channel 4 is disabled */
	disableCircuit(psu);	/* Ensure external PSU is disabled */
	disableCircuit(xfmrCct);/* Ensure transformer stage is disabled */
	disableCircuit(acCct);	/* Ensure AC stage is disabled */
	disableCircuit(fan);	/* Ensure fan is disabled */

	if ((ocpFlagRegister) || (ovpFlagRegister) || (oppFlagRegister) || (otpFlagRegister))
		return TRIP_ALERT;

	getDcMidVoltage(&meas);	/* Check the DC Mid voltage level is low enough to turn on the transformer stage */
	while (meas > DCMIDV_STRTUP_LMT) {
		DELAY_US(1000000);	/* Wait 1 second */
		getDcMidVoltage(&meas);	/* Check the DC Mid voltage level again */
	}

	enableCircuit(fan);		/* Enable the fan */
	enableCircuit(xfmrCct);	/* Enable the transformer stage */
	enableCircuit(psu);		/* Enable the external PSU */

	getDcHvVoltage(&meas);	/* Check the DC HV voltage level is high enough to turn on the AC stage */
	if (meas < DCHV_UVLCK_LMT) {
		stopAll();
		return UVLKT_TRIP;
	}

	enableCircuit(acCct);	/* Enable the AC stage */
	enableCircuit(chan1);	/* Enable on the load channel circuits */
	enableCircuit(chan2);
	enableCircuit(chan3);
	enableCircuit(chan4);
							/* Flag each stage as enabled. */
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = TRUE;
	}
	acSettings.enable = TRUE;
	xfmrSettings.enable = TRUE;
	extSettings.extPsuEnable = TRUE;
	extSettings.extFanEnable = TRUE;
	enableAllFlag = 0;
	return 0;
}
