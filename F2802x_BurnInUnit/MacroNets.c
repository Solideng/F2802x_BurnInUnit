/*
 * MacroNets.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/*============= GLOBAL VARIABLES ==============*/
Uint16	stopAll = 0;
Uint16	enableAll = 0;

// TODO WHY IS IT +2?
#ifndef DUAL_CNTL_AC
	channelParameters channel[NUM_CHNLS + 1];	/* +1 is for VMid parameters which don't have a channel */
#else
	channelParameters channel[NUM_CHNLS + 2];	/* +2 is for VMid and AC 2nd stage parameters which dont have a channel */
#endif

#if (INCR_BUILD == 1)
	volatile int32 duty[NUM_CHNLS] = {0};	/* Open-loop duty setting for use in watch window */
#endif

/*============== LOCAL VARIABLES ==============*/
volatile int32 sgenSignNet = 0;		/* Net for sine generator sign output */


void mnSetupChannels (void) {
	/* FOR SOME REASON THIS *MUST* BE CALLED *AFTER* pwmMacroConfig(), NOT BEFORE */
	/* Initialise all channel structures with default values */
	Uint16 i = 0;
	for (i = 0; i< (NUM_CHNLS + 2); i++) {
		channel[i].refNet = 0;		/* Q24 (IQ24: -128 - +128) */
		channel[i].iFdbkNet = 0;	/* Q24 */
		channel[i].vFdbkNet = 0;	/* Q24 */
		channel[i].outNet = 0;		/* Q24 */ //<< IS IT BECAUSE OF THIS?? //TODO
		channel[i].ocp = 16777216;	/* maximum Q24 */
		channel[i].ovp = 16777216;	/* maximum Q24 */
		channel[i].target = 0;		/* Q24 */
		channel[i].slewRate = _IQ24(0.001);	/* Q24 */
		channel[i].otp = 19200;		/* 150 degree C OTP limit Q7 */
//		channel[i].sMax = _SQ14(satMax[i]);	/* Q14 SQ14 */
//		channel[i].sMin = 0;		/* Q15 */
		channel[i].iMaxRms = 15360;	/* 15 amps (RMS) Q10 SQ10 */
		channel[i].iMinRms = 0;		/* 0 amps (RMS) Q10 */
		channel[i].vMaxRms = 15360;	/* 15 volts (RMS) Q10 */ //TODO Test setting << need actual
		channel[i].vMinRms = 0;		/* 0 volts (RMS) Q10 */
		channel[i].iScale = 2048;	/* 0.125f amps-per-volt Q14 */
		channel[i].vScale = 32767;	/* 1.0f volts-per-volt Q14 */
		channel[i].vGainLmt = 16384;/* 1.0f gain Q14 */
		channel[i].pGain = pGain[i];/* Q14 */
		channel[i].iGain = iGain[i];/* Q14 */
		channel[i].dGain = dGain[i];/* Q14 */
		channel[i].opMode = dc;		/* dc | ac */
		channel[i].ctlMode = iCtrl; /* iCtrl | vCtrl */
		channel[i].chEnable = FALSE;/* FALSE | TRUE */
	}
	channel[DC_STAGE].ctlMode = vCtrl;
	channel[DC_STAGE].vScale = _SQ14(1.0); //TODO Need actual value

	channel[AC_STAGE].ctlMode = vCtrl;
	channel[AC_STAGE].vScale = _SQ14(VAC_R2 / (VAC_R1 + VAC_R2 + VAC_R2));
	channel[DC_STAGE].vGainLmt = _SQ14(0.9);
	channel[AC_STAGE].opMode = ac;

	channel[V_MID_CH].ctlMode = vCtrl;	/* VMid channel is used for OVP only */
	channel[V_MID_CH].vScale = _SQ14(VMID_R2 / (VMID_R1 + VMID_R2));

	#ifdef DUAL_CNTL_AC
		channel[AC_I_CNTL].ctlMode = iCtrl;	/* VMid channel is used for OVP only */
		// TODO anything else for default values of AC_I_CNTRL?
	#endif
}

void mnStopAll (void) {
	/* Disables and zeroes all references */
	Uint16 i = 0;
	for (i = 0; i < NUM_CHNLS; i++) {
		channel[i].chEnable = FALSE;
		channel[i].refNet = 0;
	}
	stopAll = 0;
}

void mnRunAll (void) {
	/* Enable all references */
	Uint16 i = 0;
	for (i = 0; i < NUM_CHNLS; i++) {
		channel[i].chEnable = TRUE;
	}
	enableAll = 0;
}

void mnConnectNets (void) {
	/* Connect macro terminals to nets
	 *  - SHOULD BE RUN AFTER DPL_INIT()
	 */
	#if (INCR_BUILD == 1) 			/* Open loop */
		ADCDRV_1ch_Rlt1 = &channel[LOAD_0].iFdbkNet;	/* Channel 1 connections */
		ADCDRV_1ch_Rlt7 = &channel[LOAD_0].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty1A = &duty[LOAD_0];

		ADCDRV_1ch_Rlt2 = &channel[LOAD_1].iFdbkNet;	/* Channel 2 connections */
		ADCDRV_1ch_Rlt8 = &channel[LOAD_1].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty1B = &duty[LOAD_1];

		ADCDRV_1ch_Rlt3 = &channel[LOAD_2].iFdbkNet;	/* Channel 3 connections */
		ADCDRV_1ch_Rlt9 = &channel[LOAD_2].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty2A = &duty[LOAD_2];

		ADCDRV_1ch_Rlt4 = &channel[LOAD_3].iFdbkNet;	/* Channel 4 connections */
		ADCDRV_1ch_Rlt10 = &channel[LOAD_3].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty2B = &duty[LOAD_3];

		ADCDRV_1ch_Rlt5 = &channel[DC_STAGE].iFdbkNet;	/* Interboost connections */
		ADCDRV_1ch_Rlt11 = &channel[DC_STAGE].vFdbkNet;
		ADCDRV_1ch_Rlt13 = &channel[V_MID_CH].vFdbkNet;
		PWMDRV_2ch_UpCnt_Duty3A = &duty[DC_STAGE];

		ADCDRV_1ch_Rlt6 = &channel[AC_STAGE].iFdbkNet;/* AC stage connections */ //??
		ADCDRV_1ch_Rlt12 = &channel[AC_STAGE].vFdbkNet; //??
		PWMDRV_2ch_UpCnt_Duty3B = &duty[AC_STAGE];
	#endif

	#if (INCR_BUILD == 2)		/* Closed loop */
		/*============================ DC INPUTS ====================================*/
					/* Channel 0 (1) */
		ADCDRV_1ch_Rlt1 = &channel[LOAD_0].iFdbkNet;	/* Channel I Sns */
		ADCDRV_1ch_Rlt7 = &channel[LOAD_0].vFdbkNet;	/* Channel V Sns */
		CNTL_2P2Z_Ref1 = &channel[LOAD_0].refNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk1 = &channel[LOAD_0].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Coef1 = &coefs2[LOAD_0].b2;			/* CNTL coefficients */
		CNTL_2P2Z_Out1 = &channel[LOAD_0].outNet;		/* CNTL out */
		PWMDRV_2ch_UpCnt_Duty1A = &channel[LOAD_0].outNet;/* Channel PWM */

					/* Channel 1 (2) */
		ADCDRV_1ch_Rlt2 = &channel[LOAD_1].iFdbkNet;	/* Channel I Sns */
		ADCDRV_1ch_Rlt8 = &channel[LOAD_1].vFdbkNet;	/* Channel V Sns */
		CNTL_2P2Z_Ref2 = &channel[LOAD_1].refNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk2 = &channel[LOAD_1].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Coef2 = &coefs2[LOAD_1].b2;			/* CNTL coefficients */
		CNTL_2P2Z_Out2 = &channel[LOAD_1].outNet;		/* CNTL out */
		PWMDRV_2ch_UpCnt_Duty1B = &channel[LOAD_1].outNet;/* Channel PWM */

					/* Channel 2 (3) */
		ADCDRV_1ch_Rlt3 = &channel[LOAD_2].iFdbkNet;	/* Channel I Sns */
		ADCDRV_1ch_Rlt9 = &channel[LOAD_2].vFdbkNet;	/* Channel V Sns */
		CNTL_2P2Z_Ref3 = &channel[LOAD_2].refNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk3 = &channel[LOAD_2].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Coef3 = &coefs2[LOAD_2].b2;			/* CNTL coefficients */
		CNTL_2P2Z_Out3 = &channel[LOAD_2].outNet;		/* CNTL out */
		PWMDRV_2ch_UpCnt_Duty2A = &channel[LOAD_2].outNet;/* Channel PWM */

					/* Channel 3 (4) */
		ADCDRV_1ch_Rlt4 = &channel[LOAD_3].iFdbkNet;	/* Channel I Sns */
		ADCDRV_1ch_Rlt10 = &channel[LOAD_3].vFdbkNet;	/* Channel V Sns */
		CNTL_2P2Z_Ref4 = &channel[LOAD_3].refNet;		/* CNTL reference */
		CNTL_2P2Z_Fdbk4 = &channel[LOAD_3].iFdbkNet;	/* CNTL feedback */
		CNTL_2P2Z_Coef4 = &coefs2[LOAD_3].b2;			/* CNTL coefficients */
		CNTL_2P2Z_Out4 = &channel[LOAD_3].outNet;		/* CNTL out */
		PWMDRV_2ch_UpCnt_Duty2B = &channel[LOAD_3].outNet;/* Channel PWM */

		/*=========================== INTERBOOST ====================================*/
		ADCDRV_1ch_Rlt5 = &channel[DC_STAGE].iFdbkNet;	/* DC Mid I Sns */
		ADCDRV_1ch_Rlt11 = &channel[DC_STAGE].vFdbkNet;	/* DC HV V sns */
		ADCDRV_1ch_Rlt13 = &channel[V_MID_CH].vFdbkNet;	/* DC Mid V sns */
		#ifdef USE_PID
			CNTL_2P2Z_Ref5 = &channel[DC_STAGE].refNet;		/* CNTL reference */
			CNTL_2P2Z_Fdbk5 = &channel[DC_STAGE].vFdbkNet;	/* CNTL feedback */
			CNTL_2P2Z_Coef5 = &coefs2[DC_STAGE].b2;			/* CNTL coefficients */
			CNTL_2P2Z_Out5 = &channel[DC_STAGE].outNet;		/* CNTL out */
		#else
			CNTL_3P3Z_Ref1 = &channel[DC_STAGE].refNet;					/* CNTL reference */
			CNTL_3P3Z_Fdbk1 = &channel[DC_STAGE].vFdbkNet;				/* CNTL feedback */
			CNTL_3P3Z_Coef1 = &coefs3[DC_STAGE - NUM_ICTRL_CHNLS].b3;	/* CNTL coefficients */
			CNTL_3P3Z_Out1 = &channel[DC_STAGE].outNet;					/* CNTL out */
		#endif
		PWMDRV_2ch_UpCnt_Duty3A = &channel[DC_STAGE].outNet;/* Intbst PWM */

		/*============================ AC STAGE =====================================*/
		#ifndef DUAL_CNTL_AC
			ADCDRV_1ch_Rlt6 = &channel[AC_STAGE].iFdbkNet;	/* AC I Sns */
			ADCDRV_1ch_Rlt12 = &channel[AC_STAGE].vFdbkNet;	/* AC V Sns */
			SGENTI_1ch_VOut = &channel[AC_STAGE].refNet;	/* Rectified sine gen out */
			SGENTI_1ch_Sign = &sgenSignNet;					/* Sine sign */
			#ifdef USE_PID
				CNTL_2P2Z_Ref6 = &channel[AC_STAGE].refNet;		/* CNTL reference */
				CNTL_2P2Z_Fdbk6 = &channel[AC_STAGE].vFdbkNet;	/* CNTL feedback */
				CNTL_2P2Z_Coef6 = &coefs2[AC_STAGE].b2;			/* CNTL coefficients */
				CNTL_2P2Z_Out6 = &channel[AC_STAGE].outNet;		/* CNTL out */
			#else
				CNTL_3P3Z_Ref2 = &channel[AC_STAGE].refNet;					/* CNTL reference */
				CNTL_3P3Z_Fdbk2 = &channel[AC_STAGE].vFdbkNet;				/* CNTL feedback */
				CNTL_3P3Z_Coef2 = &coefs3[AC_STAGE - NUM_ICTRL_CHNLS].b3;	/* CNTL coefficients */
				CNTL_3P3Z_Out2 = &channel[AC_STAGE].outNet;					/* CNTL out */
			#endif
			PHASE_CTRL_In = &sgenSignNet;					/* AC F B Phase */
			#ifndef AC_STAGE_OPEN
				PWMDRV_2ch_UpCnt_Duty3B = &channel[AC_STAGE].outNet;/* AC F B PWM */
			#else
				PWMDRV_2ch_UpCnt_Duty3B = &channel[AC_STAGE].sigGenNet;
			#endif
		#else
			ADCDRV_1ch_Rlt6 = &channel[AC_I_CNTL].iFdbkNet;	/* AC I Sns */
			ADCDRV_1ch_Rlt12 = &channel[AC_STAGE].vFdbkNet;	/* AC V Sns */
			SGENTI_1ch_VOut = &channel[AC_STAGE].refNet;	/* Rectified sine gen out */
			SGENTI_1ch_Sign = &sgenSignNet;					/* Sine sign */

			CNTL_3P3Z_Ref2 = &channel[AC_STAGE].refNet;		/* CNTL reference */
			CNTL_3P3Z_Fdbk2 = &channel[AC_STAGE].vFdbkNet;	/* CNTL feedback */
			CNTL_3P3Z_Coef2 = &coefs3[AC_STAGE - NUM_ICTRL_CHNLS].b3;	/* CNTL coefficients */
			CNTL_3P3Z_Out2 = &channel[AC_I_CNTL].refNet;	/* CNTL out */

			CNTL_2P2Z_Ref6 = &channel[AC_I_CNTL].refNet;	/* CNTL reference */
			CNTL_2P2Z_Fdbk6 = &channel[AC_I_CNTL].iFdbkNet;	/* CNTL feedback */
			CNTL_2P2Z_Coef6 = &coefs2[AC_I_CNTL].b2;			/* CNTL coefficients */
			CNTL_2P2Z_Out6 = &channel[AC_I_CNTL].outNet;		/* CNTL out */

			PWMDRV_2ch_UpCnt_Duty3B = &channel[AC_I_CNTL].outNet;/* AC F B PWM */
			PHASE_CTRL_In = &sgenSignNet;					/* AC F B Phase */
		#endif
	#endif
}
