/*
 * MacroNets.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/*============= GLOBAL VARIABLES ==============*/
Uint16	stopAll = 0;	// TODO May be moved to SCPI device specific regs
Uint16	enableAll = 0;

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

/*============== LOCAL VARIABLES ==============*/
static volatile int32 sgenSignNet = 0;		/* Net for sine generator sign output NET */


static void initSettings (void) {
	/* Initialise all settings structures with default values */
	/* FOR SOME REASON THIS *MUST* BE CALLED *AFTER* pwmMacroConfig(), NOT BEFORE */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].slewRate   = 16777;		/* 0.001 Q24 */
		loadSettings[i].slewTarget = 0;			/* 0 Q24 */
		loadSettings[i].ocpLevel   = 587202560;	/* 35A variable Q24 */
		//loadSettings[i].ovpLevel   = 1006632960;/* 60V fixed Q24 */
		//loadSettings[i].oppLevel   = 0;			/* 200W fixed Q24 */ 	//TODO: 200 doesnt fit in IQ24!!!
		loadSettings[i].otpLevel   = 19200;		/* 150 degree C Q7 */
		//loadSettings[i].iMax 	   = 0;			/* 35 amps (RMS) Q10 SQ10 */	// TODO: 35 doesnt fit in SQ10!!! use LOAD_OCPLVL_MAX
		//loadSettings[i].vMax 	   = 15360;		/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
		loadSettings[i].iScale     = 2048;		/* 0.125f amps-per-volt Q14 */
		loadSettings[i].vScale     = 32767;		/* 1.0f volts-per-volt Q14 */
		loadSettings[i].enable     = FALSE;		/* Disabled */
	}

	acSettings.gainRate   = 16777;		/* 0.001 Q24 */
	acSettings.gainTarget = 0;			/* 0 Q24 */
	acSettings.ocpLevel   = 16777216;	/* Maximum Q24 */
	acSettings.ovpLevel   = 16777216;	/* Maximum Q24 */
	acSettings.otpLevel   = 19200;		/* 150 degree C Q7 */
	acSettings.iMaxRms    = 15360;		/* 15 amps (RMS) Q10 SQ10 */
	acSettings.vMaxRms    = 15360;		/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
	acSettings.iScale     = 2048;		/* 0.125f amps-per-volt Q14 */
	acSettings.vScale     = 32767;		/* 1.0f volts-per-volt Q14 */
	acSettings.enable     = FALSE;		/* FALSE | TRUE */
	acSettings.mode       = master;		/* master | slave */
	acSettings.vScale     = _SQ14(VAC_R2 / (VAC_R1 + VAC_R2 + VAC_R2));
	acSettings.vGainLmt   = _SQ14(0.9);	/* 0.9 gain limit Q14 */

	xfmrSettings.ocpLevel 	 = 16777216;	/* Maximum Q24 */
	xfmrSettings.midOvpLevel = 16777216;	/* Maximum Q24 */
	xfmrSettings.hvOvpLevel  = 16777216;	/* Maximum Q24 */
	xfmrSettings.otpLevel 	 = 19200;		/* 150 degree C Q7*/
	xfmrSettings.iMax	 	 = 15360;		/* 15 Amps RMS Q10 */
	xfmrSettings.iScale 	 = 0;			/* Q14 */
	xfmrSettings.midVMax	 = 15360;		/* 15 Amps RMS Q10 */
	xfmrSettings.midVScale 	 = _SQ14(VMID_R2 / (VMID_R1 + VMID_R2));
	xfmrSettings.hvVMax  	 = 15360;		/* 15 Amps Q10 */
	xfmrSettings.hvVScale 	 = _SQ14(1.0);	// TODO: Need actual value
	xfmrSettings.enable 	 = FALSE;		/* Disabled */

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
		ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;	/* AC I Sns */
		ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;	/* AC V Sns */
		SGENTI_1ch_VOut  = &acNets.vRefNet;		/* Rectified sine gen out */
		CNTL_3P3Z_Ref2 	 = &acNets.vRefNet;		/* VCNTL reference */
		CNTL_3P3Z_Fdbk2  = &acNets.vFdbkNet;	/* VCNTL feedback */
		CNTL_3P3Z_Out2   = &acNets.iRefNet;		/* VCNTL out */
		CNTL_3P3Z_Coef2  = &acVCoefs.b3;		/* VCNTL coefficients */
		SGENTI_1ch_Sign  = &sgenSignNet;		/* Sine sign */
		PHASE_CTRL_In 	 = &sgenSignNet;		/* AC F B Phase */
		#ifndef AC_STAGE_OPEN
			PWMDRV_2ch_UpCnt_Duty3B = &acNets.vFiltOutNet;/* AC F B PWM */
		#else
			PWMDRV_2ch_UpCnt_Duty3B = &acNets.vRefNet;
		#endif
	#else
		if (mode != slave) {
			ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;/* AC V Sns */
			SGENTI_1ch_VOut  = &acNets.vRefNet;	/* Rectified sine gen out */
			SGENTI_1ch_Sign  = &sgenSignNet;	/* Sine sign */
			PHASE_CTRL_In 	 = &sgenSignNet;	/* AC F B Phase */

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
	initSettings();
	initNets();
	connectAllNets(mode);
}

void stopAll (void) {
	/* Disables and zeroes all references */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = FALSE;
		loadNets[i].iRefNet = 0;
	}
	acSettings.enable = FALSE;
	acNets.vRefNet = 0;
	acNets.iRefNet = 0;
	xfmrSettings.enable = FALSE;
	xfmrNets.pwmDutyNet = 0;
	extSettings.extPsuEnable = FALSE;
	extSettings.extFanEnable = FALSE;
	stopAll = 0;
}

void runAll (void) {
	/* Enable all references */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = TRUE;
	}
	acSettings.enable = TRUE;
	xfmrSettings.enable = TRUE;
	extSettings.extPsuEnable = TRUE;
	enableAll = 0;
}

//void mnConnectNets (void) {
//	/* Connect macro terminals to nets
//	 *  - SHOULD BE RUN AFTER DPL_INIT()
//	 */
//	#if (INCR_BUILD == 1) 			/* Open loop */
//		ADCDRV_1ch_Rlt1 = &loadNets[load1].iFdbkNet;	/* Load 1 connections */
//		ADCDRV_1ch_Rlt7 = &loadNets[load1].vFdbkNet;
//		PWMDRV_2ch_UpCnt_Duty1A = &duty[load1];
//
//		ADCDRV_1ch_Rlt2 = &loadNets[load2].iFdbkNet;	/* Load 2 connections */
//		ADCDRV_1ch_Rlt8 = &loadNets[load2].vFdbkNet;
//		PWMDRV_2ch_UpCnt_Duty1B = &duty[load2];
//
//		ADCDRV_1ch_Rlt3 = &loadNets[load3].iFdbkNet;	/* Load 3 connections */
//		ADCDRV_1ch_Rlt9 = &loadNets[load3].vFdbkNet;
//		PWMDRV_2ch_UpCnt_Duty2A = &duty[load3];
//
//		ADCDRV_1ch_Rlt4  = &loadNets[load4].iFdbkNet;	/* Load 4 connections */
//		ADCDRV_1ch_Rlt10 = &loadNets[load4].vFdbkNet;
//		PWMDRV_2ch_UpCnt_Duty2B = &duty[load4];
//
//		ADCDRV_1ch_Rlt5  = &xfmrNets.iSnsNet;			/* Xfmr connections */
//		ADCDRV_1ch_Rlt11 = &xfmrNets.hvVSnsNet;
//		ADCDRV_1ch_Rlt13 = &xfmrNets.midVSnsNet;
//		PWMDRV_2ch_UpCnt_Duty3A = &duty[DC_STAGE];
//
//		ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;			/* AC stage connections */
//		ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;
//		PWMDRV_2ch_UpCnt_Duty3B = &duty[AC_STAGE];
//	#endif
//
//	#if (INCR_BUILD == 2)		/* Closed loop */
//		/*============================ DC INPUTS ====================================*/
//					/* Load 1 */
//		ADCDRV_1ch_Rlt1 = &loadNets[load1].iFdbkNet;	/* Load I Sns */
//		ADCDRV_1ch_Rlt7 = &loadNets[load1].vFdbkNet;	/* Load V Sns */
//		CNTL_2P2Z_Ref1 	= &loadNets[load1].iRefNet;		/* CNTL reference */
//		CNTL_2P2Z_Fdbk1 = &loadNets[load1].iFdbkNet;	/* CNTL feedback */
//		CNTL_2P2Z_Out1 	= &loadNets[load1].iFiltOutNet;	/* CNTL out */
//		CNTL_2P2Z_Coef1 = &loadICoefs[load1].b2;		/* CNTL coefficients */
//		PWMDRV_2ch_UpCnt_Duty1A = &loadNets[load1].iFiltOutNet;	/* Load PWM */
//
//					/* Load 2 */
//		ADCDRV_1ch_Rlt2 = &loadNets[load2].iFdbkNet;	/* Load I Sns */
//		ADCDRV_1ch_Rlt8 = &loadNets[load2].vFdbkNet;	/* Load V Sns */
//		CNTL_2P2Z_Ref2 	= &loadNets[load2].iRefNet;		/* CNTL reference */
//		CNTL_2P2Z_Fdbk2 = &loadNets[load2].iFdbkNet;	/* CNTL feedback */
//		CNTL_2P2Z_Out2 	= &loadNets[load2].iFiltOutNet;	/* CNTL out */
//		CNTL_2P2Z_Coef2 = &loadICoefs[load2].b2;		/* CNTL coefficients */
//		PWMDRV_2ch_UpCnt_Duty1B = &loadNets[load2].iFiltOutNet;	/* Load PWM */
//
//					/* Load 3 */
//		ADCDRV_1ch_Rlt3 = &loadNets[load3].iFdbkNet;	/* Load I Sns */
//		ADCDRV_1ch_Rlt9 = &loadNets[load3].vFdbkNet;	/* Load V Sns */
//		CNTL_2P2Z_Ref3 	= &loadNets[load3].iRefNet;		/* CNTL reference */
//		CNTL_2P2Z_Fdbk3 = &loadNets[load3].iFdbkNet;	/* CNTL feedback */
//		CNTL_2P2Z_Out3 	= &loadNets[load3].iFiltOutNet;	/* CNTL out */
//		CNTL_2P2Z_Coef3 = &loadICoefs[load3].b2;		/* CNTL coefficients */
//		PWMDRV_2ch_UpCnt_Duty2A = &loadNets[load3].iFiltOutNet;	/* Load PWM */
//
//					/* Load 4 */
//		ADCDRV_1ch_Rlt4  = &loadNets[load4].iFdbkNet;	/* Load I Sns */
//		ADCDRV_1ch_Rlt10 = &loadNets[load4].vFdbkNet;	/* Load V Sns */
//		CNTL_2P2Z_Ref4 	 = &loadNets[load4].iRefNet;	/* CNTL reference */
//		CNTL_2P2Z_Fdbk4  = &loadNets[load4].iFdbkNet;	/* CNTL feedback */
//		CNTL_2P2Z_Out4 	 = &loadNets[load4].iFiltOutNet;/* CNTL out */
//		CNTL_2P2Z_Coef4  = &loadICoefs[load4].b2;		/* CNTL coefficients */
//		PWMDRV_2ch_UpCnt_Duty2B = &loadNets[load4].iFiltOutNet;	/* Load PWM */
//
//		/*============================ AC STAGE =====================================*/
//		#ifndef DUAL_CNTL_AC
//			ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;	/* AC I Sns */
//			ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;	/* AC V Sns */
//			SGENTI_1ch_VOut  = &acNets.vRefNet;		/* Rectified sine gen out */
//			CNTL_3P3Z_Ref2 	 = &acNets.vRefNet;		/* CNTL reference */
//			CNTL_3P3Z_Fdbk2  = &acNets.vFdbkNet;	/* CNTL feedback */
//			CNTL_3P3Z_Out2   = &acNets.vFiltOutNet;	/* CNTL out */
//			CNTL_3P3Z_Coef2  = &acVCoefs.b3;		/* CNTL coefficients */
//			SGENTI_1ch_Sign  = &sgenSignNet;		/* Sine sign */
//			PHASE_CTRL_In 	 = &sgenSignNet;		/* AC F B Phase */
//			#ifndef AC_STAGE_OPEN
//				PWMDRV_2ch_UpCnt_Duty3B = &acNets.vFiltOutNet;/* AC F B PWM */
//			#else
//				PWMDRV_2ch_UpCnt_Duty3B = &acNets.vRefNet;
//			#endif
//		#else
//			ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;	/* AC I Sns */
//			ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;	/* AC V Sns */
//			SGENTI_1ch_VOut  = &acNets.vRefNet;		/* Rectified sine gen out */
//			SGENTI_1ch_Sign  = &sgenSignNet;		/* Sine sign */
//
//			CNTL_3P3Z_Ref1  = &acNets.vRefNet;		/* CNTL reference */
//			CNTL_3P3Z_Fdbk1 = &acNets.vFdbkNet;		/* CNTL feedback */
//			CNTL_3P3Z_Out1  = &acNets.vFiltOutNet;	/* CNTL out */
//			CNTL_3P3Z_Coef1 = &acVCoefs.b3;			/* CNTL coefficients */
//
//			CNTL_2P2Z_Ref5  = &acNets.vFiltOutNet;	/* CNTL reference */
//			CNTL_2P2Z_Fdbk5 = &acNets.iFdbkNet;		/* CNTL feedback */
//			CNTL_2P2Z_Out5  = &acNets.iFiltOutNet;	/* CNTL out */
//			CNTL_2P2Z_Coef5 = &acICoefs.b2;			/* CNTL coefficients */
//
//			PWMDRV_2ch_UpCnt_Duty3B = &acNets.iFiltOutNet;/* AC F B PWM */
//			PHASE_CTRL_In = &sgenSignNet;			/* AC F B Phase */
//		#endif
//
//		/*============================== XFMR =======================================*/
//		ADCDRV_1ch_Rlt5  = &xfmrNets.iSnsNet;			/* DC Mid I Sns */
//		ADCDRV_1ch_Rlt11 = &xfmrNets.hvVSnsNet;			/* DC HV V Sns */
//		ADCDRV_1ch_Rlt13 = &xfmrNets.midVSnsNet;		/* DC Mid V Sns */
//		PWMDRV_2ch_UpCnt_Duty3A = &xfmrNets.pwmDutyNet;	/* Xfmr PWM */
//	#endif
//}
