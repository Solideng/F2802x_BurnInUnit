/*
 * Pwm.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

// TODO: For 130 kHz the period would need to be 60MHz / 130kHz = 461.538...
// WE CANT CHANGE THE FREQUENCY OF THE A PORTION OF A PWM CHANNEL BY ITSEF - THUS CHANGING INTBSTPWM (EPWM3A) TO 130 kHz WOULD
// REQUIRE CHANGING ACPWM (EPWM3B) AS WELL
static Uint16 period = PERIOD;		/* Initially 600 - with a SysClk of 60MHz the freq = 60MHz/600 = 100kHz */

void pwmMacroConfigure (void) {
	/* Configure each of the PWM macros to be used */
	PWM_2ch_UpCnt_CNF(1, period, 1, 0);
	PWM_2ch_UpCnt_CNF(2, period, 0, 2);
 	PWM_2ch_UpCnt_CNF(3, period, 0, 2);
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
