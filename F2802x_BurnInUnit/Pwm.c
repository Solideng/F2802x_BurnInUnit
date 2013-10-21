/*
 * Pwm.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

void initPwm (void) {
	pwmMacroConfigure();
	pwmSocConfigure();
	pwmDPLTrigInit();
}

void pwmMacroConfigure (void) {
	/* Configure each of the PWM macros to be used */
	PWM_2ch_UpCnt_CNF(1, PWM_1_PRD, 1, 0);
	PWM_2ch_UpCnt_CNF(2, PWM_2_PRD, 0, 2);
 	PWM_2ch_UpCnt_CNF(3, PWM_3_PRD, 1, 0);
}

void pwmSocConfigure (void) {
	/* Configure ePWM1 (master PWM) to generate ADC SOC pulses */
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;					/* Enable ePWM1 SOCA pulse */
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;		/* SOCA from ePWM1 Zero event (With the PWMDRV_2ch_UpCnt thats the start of a pulse) */
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_3RD;			/* Trigger ePWM1 SOCA on every 3rd event (i.e. the start of every third pulse) */
}

void pwmDPLTrigInit (void) {
	/* Interrupt and ISR initialisation */
	EALLOW;
	PieVectTable.EPWM1_INT = &DPL_ISR;		/* Map Interrupt */
	EDIS;
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;		/* PIE level enable, Grp3 / Int1 */
	/* Configure ISR trigger */
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;/* INT on ePWM1 Period event (With the PWMDRV_2ch_UpCnt thats the end of a pulse) */
	EPwm1Regs.ETSEL.bit.INTEN = 1;			/* Enable INT */
	EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;		/* Generate ISR INT on every 3rd ePWM1 event (i.e. the end of every third pulse) */

	IER |= M_INT3;							/* Enable CPU INT3 connected to ePWM1-6 INTs: */
}

//Uint16 pwmSetFreq (Uint32 frq) {
//	Uint32 clck = (((SysCtrlRegs.PLLCR.bit.DIV*10000000)/2) /((EPwm1Regs.TBCTL.bit.HSPCLKDIV + 1) * (EPwm1Regs.TBCTL.bit.CLKDIV + 1)));
//
//	if (frq >= clck)
//		return VALUE_OOB;
//
//	period = (clck / frq);	/* T =  SysClk / f */
//	EPwm1Regs.TBPRD = period - 1;
//	EPwm2Regs.TBPRD = period - 1;
//	EPwm3Regs.TBPRD = period - 1;
//	return 0;
//}
//
//Uint16 pwmGetFreq (Uint32 *frqDest) {
//	Uint32 clock = (((SysCtrlRegs.PLLCR.bit.DIV*10000000)/2) /((EPwm1Regs.TBCTL.bit.HSPCLKDIV + 1) * (EPwm1Regs.TBCTL.bit.CLKDIV + 1)));
//	*frqDest = (clock / period); /* f = SysClk / T */
//	return 0;
//}
