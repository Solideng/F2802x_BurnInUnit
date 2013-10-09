/*
 * MacroNets.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

void mnInitSettings (void);
void mnInitNets (void);
void mnConnectNets (void);

/*============= GLOBAL VARIABLES ==============*/
Uint16	stopAll = 0;	// TODO May be moved to SCPI device specific regs
Uint16	enableAll = 0;

loadStageNets 		loadNets[numberOfLoads];
loadStageSettings 	loadSettings[numberOfLoads];
acStageNets 		acNets;
acStageSettings 	acSettings;
xfmrStage 			xfmr;

//#ifndef DUAL_CNTL_AC
//	channelParameters channel[NUM_CHNLS + 1];	/* +1 is for VMid parameters which don't have a channel */
//#else
//	channelParameters channel[NUM_CHNLS + 3];	/* +3 is for VMid, Xfmr and AC 2nd stage parameters which don't have a channel */
//#endif

#if (INCR_BUILD == 1)
	volatile int32 duty[NUM_CHNLS] = {0};	/* Open-loop duty setting for use in watch window */
#endif

/*============== LOCAL VARIABLES ==============*/
static volatile int32 sgenSignNet = 0;		/* Net for sine generator sign output */

//
//void mnSetupChannels (void) {
//	/* FOR SOME REASON THIS *MUST* BE CALLED *AFTER* pwmMacroConfig(), NOT BEFORE */
//	/* Initialise all channel structures with default values */
//	Uint16 i = 0;
//	for (i = 0; i< (NUM_CHNLS + 2); i++) {
//		channel[i].refNet = 0;		/* Q24 (IQ24: -128 - +128) */
//		channel[i].iFdbkNet = 0;	/* Q24 */
//		channel[i].vFdbkNet = 0;	/* Q24 */
//		channel[i].outNet = 0;		/* Q24 */ //<< TODO IS IT BECAUSE OF THIS??
//		channel[i].ocp = 16777216;	/* maximum Q24 */
//		channel[i].ovp = 16777216;	/* maximum Q24 */
//		channel[i].target = 0;		/* Q24 */
//		channel[i].slewRate = _IQ24(0.001);	/* Q24 */
//		channel[i].otp = 19200;		/* 150 degree C OTP limit Q7 */
//		channel[i].iMaxRms = 15360;	/* 15 amps (RMS) Q10 SQ10 */
//		channel[i].iMinRms = 0;		/* 0 amps (RMS) Q10 */
//		channel[i].vMaxRms = 15360;	/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
//		channel[i].vMinRms = 0;		/* 0 volts (RMS) Q10 */
//		channel[i].iScale = 2048;	/* 0.125f amps-per-volt Q14 */
//		channel[i].vScale = 32767;	/* 1.0f volts-per-volt Q14 */
//		channel[i].vGainLmt = 16384;/* 1.0f gain Q14 */
//		channel[i].opMode = dc;		/* dc | ac */
//		channel[i].ctlMode = iCtrl; /* iCtrl | vCtrl */
//		channel[i].chEnable = FALSE;/* FALSE | TRUE */
//	}
//	channel[DC_STAGE].ctlMode = vCtrl;
//	channel[DC_STAGE].vScale = _SQ14(1.0); //TODO Need actual value
//
//	channel[AC_STAGE].ctlMode = vCtrl;
//	channel[AC_STAGE].vScale = _SQ14(VAC_R2 / (VAC_R1 + VAC_R2 + VAC_R2));
//	channel[DC_STAGE].vGainLmt = _SQ14(0.9);
//	channel[AC_STAGE].opMode = ac;
//
//	channel[V_MID_CH].ctlMode = vCtrl;	/* VMid channel is used for OVP only */
//	channel[V_MID_CH].vScale = _SQ14(VMID_R2 / (VMID_R1 + VMID_R2));
//
//	#ifdef DUAL_CNTL_AC
//		channel[AC_I_CNTL].ctlMode = iCtrl;	/* VMid channel is used for OVP only */
//		// TODO anything else for default values of AC_I_CNTRL?
//	#endif
//}

void mnSetupNets (void) {
	mnInitSettings();
	mnInitNets();
	mnConnectNets();
}

void mnStopAll (void) {
	/* Disables and zeroes all references */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = FALSE;
		loadNets[i].iRefNet = 0;
	}
	acSettings.enable = FALSE;
	acNets.vRefNet = 0;
	acNets.vFiltOutNet = 0;
	xfmr.enable = FALSE;
	xfmr.pwmDutyNet = 0;
	stopAll = 0;
}

void mnRunAll (void) {
	/* Enable all references */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = TRUE;
	}
	acSettings.enable = TRUE;
	xfmr.enable = TRUE;
	enableAll = 0;
}

void mnInitSettings (void) {
	/* Initialise all settings structures with default values */
	/* FOR SOME REASON THIS *MUST* BE CALLED *AFTER* pwmMacroConfig(), NOT BEFORE */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].slewRate = _IQ24(0.001);/* Q24 */
		loadSettings[i].slewTarget = 0;			/* Q24 */
		loadSettings[i].ocpLevel   = 16777216;	/* Maximum Q24 */
		loadSettings[i].ovpLevel   = 16777216;	/* Maximum Q24 */
		loadSettings[i].otpLevel   = 19200;		/* 150 degree C OTP limit Q7 */
		loadSettings[i].iMaxRms    = 15360;		/* 15 amps (RMS) Q10 SQ10 */
		loadSettings[i].iMinRms    = 0;			/* 0 amps (RMS) Q10 */
		loadSettings[i].vMaxRms    = 15360;		/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
		loadSettings[i].vMinRms    = 0;			/* 0 volts (RMS) Q10 */
		loadSettings[i].iScale     = 2048;		/* 0.125f amps-per-volt Q14 */
		loadSettings[i].vScale     = 32767;		/* 1.0f volts-per-volt Q14 */
		loadSettings[i].enable = FALSE;			/* FALSE | TRUE */
	}

	acSettings.gainRate = _IQ24(0.001);	/* Q24 */
	acSettings.gainTarget = 0;			/* Q24 */
	acSettings.ocpLevel   = 16777216;	/* Maximum Q24 */
	acSettings.ovpLevel   = 16777216;	/* Maximum Q24 */
	acSettings.otpLevel   = 19200;		/* 150 degree C OTP limit Q7 */
	acSettings.iMaxRms    = 15360;		/* 15 amps (RMS) Q10 SQ10 */
	acSettings.iMinRms    = 0;			/* 0 amps (RMS) Q10 */
	acSettings.vMaxRms    = 15360;		/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
	acSettings.vMinRms    = 0;			/* 0 volts (RMS) Q10 */
	acSettings.iScale     = 2048;		/* 0.125f amps-per-volt Q14 */
	acSettings.vScale     = 32767;		/* 1.0f volts-per-volt Q14 */
	acSettings.enable     = FALSE;		/* FALSE | TRUE */
	acSettings.mode       = master;		/* master | slave */
	acSettings.vScale     = _SQ14(VAC_R2 / (VAC_R1 + VAC_R2 + VAC_R2));
	acSettings.vGainLmt   = _SQ14(0.9);	/* 0.9 gain limit Q14 */

	xfmr.ocpLevel = 16777216;
	xfmr.midOvpLevel = 16777216;
	xfmr.hvOvpLevel = 16777216;
	xfmr.otpLevel = 19200;
	xfmr.iScale = 0;
	xfmr.midVScale = _SQ14(VMID_R2 / (VMID_R1 + VMID_R2));
	xfmr.hvVScale = _SQ14(1.0);		// TODO: Need actual value
	xfmr.enable = FALSE;
}

void mnInitNets (void) {
	/* Clears all nets. */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadNets[i].iFdbkNet = 0;	/* Q24 (IQ24: -128 - +128) */
		loadNets[i].vFdbkNet = 0;	/* Q24 */
		loadNets[i].iRefNet = 0;	/* Q24 */
		loadNets[i].iFiltOutNet = 0;/* Q24 */
	}

	acNets.iFdbkNet = 0;	/* Q24 */
	acNets.vFdbkNet = 0;	/* Q24 */
	acNets.vRefNet = 0;		/* Q24 */
	acNets.vFiltOutNet = 0;	/* Q24 */
	acNets.iFiltOutNet = 0;	/* Q24 */

	xfmr.iSnsNet = 0;		/* Q24 */
	xfmr.midVSnsNet = 0;	/* Q24 */
	xfmr.hvVSnsNet = 0;		/* Q24 */
}

void mnConnectNets (void) {
	/* Connect macro terminals to nets
	 *  - SHOULD BE RUN AFTER DPL_INIT()
	 */
	#if (INCR_BUILD == 1) 			/* Open loop */
		ADCDRV_1ch_Rlt1 = &loadNets[load1].iFdbkNet;	/* Load 1 connections */
		ADCDRV_1ch_Rlt7 = &loadNets[load1].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty1A = &duty[load1];

		ADCDRV_1ch_Rlt2 = &loadNets[load2].iFdbkNet;	/* Load 2 connections */
		ADCDRV_1ch_Rlt8 = &loadNets[load2].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty1B = &duty[load2];

		ADCDRV_1ch_Rlt3 = &loadNets[load3].iFdbkNet;	/* Load 3 connections */
		ADCDRV_1ch_Rlt9 = &loadNets[load3].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty2A = &duty[load3];

		ADCDRV_1ch_Rlt4  = &loadNets[load4].iFdbkNet;	/* Load 4 connections */
		ADCDRV_1ch_Rlt10 = &loadNets[load4].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty2B = &duty[load4];

		ADCDRV_1ch_Rlt5 = &xfmr.iSnsNet;				/* Xfmr connections */
		ADCDRV_1ch_Rlt11 = &xfmr.hvVSnsNet;
		ADCDRV_1ch_Rlt13 = &xfmr.midVSnsNet;
		PWMDRV_2ch_UpCnt_Duty3A = &duty[DC_STAGE];

		ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;			/* AC stage connections */
		ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty3B = &duty[AC_STAGE];
	#endif

	#if (INCR_BUILD == 2)		/* Closed loop */
		/*============================ DC INPUTS ====================================*/
					/* Load 1 */
		ADCDRV_1ch_Rlt1 = &loadNets[load1].iFdbkNet;	/* Load I Sns */
		ADCDRV_1ch_Rlt7 = &loadNets[load1].vFdbkNet;	/* Load V Sns */
		CNTL_2P2Z_Ref1 	= &loadNets[load1].iRefNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk1 = &loadNets[load1].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Out1 	= &loadNets[load1].iFiltOutNet;	/* CNTL out */
		CNTL_2P2Z_Coef1 = &loadICoefs[load1].b2;		/* CNTL coefficients */
		PWMDRV_2ch_UpCnt_Duty1A = &loadNets[load1].iFiltOutNet;	/* Load PWM */

					/* Load 2 */
		ADCDRV_1ch_Rlt2 = &loadNets[load2].iFdbkNet;	/* Load I Sns */
		ADCDRV_1ch_Rlt8 = &loadNets[load2].vFdbkNet;	/* Load V Sns */
		CNTL_2P2Z_Ref2 	= &loadNets[load2].iRefNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk2 = &loadNets[load2].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Out2 	= &loadNets[load2].iFiltOutNet;	/* CNTL out */
		CNTL_2P2Z_Coef2 = &loadICoefs[load2].b2;		/* CNTL coefficients */
		PWMDRV_2ch_UpCnt_Duty1B = &loadNets[load2].iFiltOutNet;	/* Load PWM */

					/* Load 3 */
		ADCDRV_1ch_Rlt3 = &loadNets[load3].iFdbkNet;	/* Load I Sns */
		ADCDRV_1ch_Rlt9 = &loadNets[load3].vFdbkNet;	/* Load V Sns */
		CNTL_2P2Z_Ref3 	= &loadNets[load3].iRefNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk3 = &loadNets[load3].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Out3 	= &loadNets[load3].iFiltOutNet;	/* CNTL out */
		CNTL_2P2Z_Coef3 = &loadICoefs[load3].b2;		/* CNTL coefficients */
		PWMDRV_2ch_UpCnt_Duty2A = &loadNets[load3].iFiltOutNet;	/* Load PWM */

					/* Load 4 */
		ADCDRV_1ch_Rlt4  = &loadNets[load4].iFdbkNet;	/* Load I Sns */
		ADCDRV_1ch_Rlt10 = &loadNets[load4].vFdbkNet;	/* Load V Sns */
		CNTL_2P2Z_Ref4 	 = &loadNets[load4].iRefNet;	/* CNTL reference */
		CNTL_2P2Z_Fdbk4  = &loadNets[load4].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Out4 	 = &loadNets[load4].iFiltOutNet;/* CNTL out */
		CNTL_2P2Z_Coef4  = &loadICoefs[load4].b2;		/* CNTL coefficients */
		PWMDRV_2ch_UpCnt_Duty2B = &loadNets[load4].iFiltOutNet;	/* Load PWM */

		/*============================ AC STAGE =====================================*/
		#ifndef DUAL_CNTL_AC
			ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;	/* AC I Sns */
			ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;	/* AC V Sns */
			SGENTI_1ch_VOut  = &acNets.vRefNet;		/* Rectified sine gen out */
			CNTL_3P3Z_Ref2 	 = &acNets.vRefNet;		/* CNTL reference */
			CNTL_3P3Z_Fdbk2  = &acNets.vFdbkNet;	/* CNTL feedback */
			CNTL_3P3Z_Out2   = &acNets.vFiltOutNet;	/* CNTL out */
			CNTL_3P3Z_Coef2  = &acVCoefs.b3;		/* CNTL coefficients */
			SGENTI_1ch_Sign  = &sgenSignNet;		/* Sine sign */
			PHASE_CTRL_In 	 = &sgenSignNet;		/* AC F B Phase */
			#ifndef AC_STAGE_OPEN
				PWMDRV_2ch_UpCnt_Duty3B = &acNets.vFiltOutNet;/* AC F B PWM */
			#else
				PWMDRV_2ch_UpCnt_Duty3B = &acNets.vRefNet;
			#endif
		#else
			ADCDRV_1ch_Rlt6  = &acNets.iFdbkNet;	/* AC I Sns */
			ADCDRV_1ch_Rlt12 = &acNets.vFdbkNet;	/* AC V Sns */
			SGENTI_1ch_VOut  = &acNets.vRefNet;		/* Rectified sine gen out */
			SGENTI_1ch_Sign  = &sgenSignNet;		/* Sine sign */

			CNTL_3P3Z_Ref1  = &acNets.vRefNet;		/* CNTL reference */
			CNTL_3P3Z_Fdbk1 = &acNets.vFdbkNet;		/* CNTL feedback */
			CNTL_3P3Z_Out1  = &acNets.vFiltOutNet;	/* CNTL out */
			CNTL_3P3Z_Coef1 = &acVCoefs.b3;			/* CNTL coefficients */

			CNTL_2P2Z_Ref5  = &acNets.vFiltOutNet;	/* CNTL reference */
			CNTL_2P2Z_Fdbk5 = &acNets.iFdbkNet;		/* CNTL feedback */
			CNTL_2P2Z_Out5  = &acNets.iFiltOutNet;	/* CNTL out */
			CNTL_2P2Z_Coef5 = &acICoefs.b2;			/* CNTL coefficients */

			PWMDRV_2ch_UpCnt_Duty3B = &acNets.iFiltOutNet;/* AC F B PWM */
			PHASE_CTRL_In = &sgenSignNet;			/* AC F B Phase */
		#endif

		/*============================== XFMR =======================================*/
		ADCDRV_1ch_Rlt5  = &xfmr.iSnsNet;			/* DC Mid I Sns */
		ADCDRV_1ch_Rlt11 = &xfmr.hvVSnsNet;			/* DC HV V Sns */
		ADCDRV_1ch_Rlt13 = &xfmr.midVSnsNet;		/* DC Mid V Sns */
		PWMDRV_2ch_UpCnt_Duty3A = &xfmr.pwmDutyNet;	/* Xfmr PWM */
	#endif
}
