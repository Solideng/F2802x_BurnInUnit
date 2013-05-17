/*
 * Pwm.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"
/*================ GLOBAL VARS ================*/
Uint16 comp1Trig = FALSE;	/* TODO Replace these with SCPI register bits when available */
Uint16 comp2Trig = FALSE;

/*================= LOCAL VARS ================*/
Uint16 period = PERIOD;		/* Initially 600 - with a SysClk of 60MHz the freq = 60MHz/600 = 100kHz */

/*============== LOCAL FUNCTIONS ==============*/
static interrupt void pwmTz_ISR (void);

/*============= TRIP ZONE SECTION =============*/
void pwmTzConfigure (void) {
	Uint16 i;
	EALLOW;
	for (i = 1; i <= 3; i++) {
		(*ePWM[i]).DCTRIPSEL.bit.DCAHCOMPSEL = 0x08;/* Select COMP1 as DC A high input */
		(*ePWM[i]).DCTRIPSEL.bit.DCALCOMPSEL = 0x09;/* Select COMP2 as DC A Low input */
		(*ePWM[i]).DCTRIPSEL.bit.DCBHCOMPSEL = 0x08;/* Select COMP1 as DC B high input */
		(*ePWM[i]).DCTRIPSEL.bit.DCBLCOMPSEL = 0x09;/* Select COMP2 as DC B low input */

		(*ePWM[i]).TZDCSEL.bit.DCAEVT1 = 0x2;		/* Select DCAH=Hi & DCAL=x (COMP1) for DCAEVT1 */
		(*ePWM[i]).TZDCSEL.bit.DCAEVT2 = 0x4;		/* Select DCAH=x & DCAL=Hi (COMP2) for DCAEVT2 */
		(*ePWM[i]).TZDCSEL.bit.DCBEVT1 = 0x2;		/* Select DCBH=Hi & DCBL=x (COMP1) for DCBEVT1 */
		(*ePWM[i]).TZDCSEL.bit.DCBEVT2 = 0x4;		/* Select DCBH=x & DCBL=Hi (COMP2) for DCBEVT2 */

		(*ePWM[i]).DCACTL.bit.EVT1SRCSEL = 0;		/* DCAEVT1 is not filtered */
		(*ePWM[i]).DCACTL.bit.EVT1FRCSYNCSEL = 1;	/* DCAEVT1 source is asynchronous */
		(*ePWM[i]).DCACTL.bit.EVT2SRCSEL = 0;		/* DCAEVT2 is not filtered */
		(*ePWM[i]).DCACTL.bit.EVT2FRCSYNCSEL = 1;	/* DCAEVT2 source is asynchronous */

		(*ePWM[i]).DCBCTL.bit.EVT1SRCSEL = 0;		/* DCBEVT1 is not filtered */
		(*ePWM[i]).DCBCTL.bit.EVT1FRCSYNCSEL = 1;	/* DCBEVT1 source is asynchronous */
		(*ePWM[i]).DCBCTL.bit.EVT2SRCSEL = 0;		/* DCBEVT2 is not filtered */
		(*ePWM[i]).DCBCTL.bit.EVT2FRCSYNCSEL = 1;	/* DCBEVT2 source is asynchronous */

		(*ePWM[i]).TZCTL.bit.DCAEVT1 = 0x2;			/* Set DCAEVT1 (COMP1) to force PWMA to a low state */
		(*ePWM[i]).TZCTL.bit.DCAEVT2 = 0x2;			/* Set DCAEVT2 (COMP2) to force PWMA to a low state */
		(*ePWM[i]).TZCTL.bit.DCBEVT1 = 0x2;			/* Set DCBEVT1 (COMP1) to force PWMB to a low state */
		(*ePWM[i]).TZCTL.bit.DCBEVT2 = 0x2;			/* Set DCBEVT2 (COMP2) to force PWMB to a low state */
	}

	EPwm3Regs.TZEINT.bit.DCAEVT1 = 1;		/* Set all DC events to raise an interrupt */
	EPwm3Regs.TZEINT.bit.DCAEVT2 = 1;		/* i.e. COMP1 & 2 on PWM A and COMP1 & 2 on PWMB */
	EPwm3Regs.TZEINT.bit.DCBEVT1 = 1;
	EPwm3Regs.TZEINT.bit.DCBEVT2 = 1;

	PieVectTable.EPWM3_TZINT = &pwmTz_ISR;	/* Map interrupt to ISR */
	EDIS;

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		/* Enable the PIE block */
	PieCtrlRegs.PIEIER2.bit.INTx3 = 1;		/* Enable in PIE group 2 - INT 3 */
	IER |= M_INT2;							/* Enable CPU INT 2 */
}

void pwmRstTz (void) {
	Uint16 i;	// TODO: write more for this reset function?
	EALLOW;
	for (i = 1; i <= 3; i++) {
		(*ePWM[i]).TZCLR.all = 0x3F;	/* Clear trip all zone flags */
	}
	EDIS;
	mnRunAll();
}

static interrupt void pwmTz_ISR (void) {
	mnStopAll();	/* Stop all channels, i.e. disable and zero their references */
					/* Check flags to see which COMP/SNS caused the interrupt */
	if ((EPwm3Regs.TZFLG.bit.DCAEVT1 == 1) || (EPwm3Regs.TZFLG.bit.DCBEVT1 == 1)) {
		comp1Trig = TRUE;
	}
	if ((EPwm3Regs.TZFLG.bit.DCAEVT2 == 1) || (EPwm3Regs.TZFLG.bit.DCBEVT2 == 1)) {
		comp2Trig = TRUE;
	}
	PieCtrlRegs.PIEACK.bit.ACK2 = 1;	/* Acknowledge this interrupt to allow more from group 2 */
	return;
}

/*================ PWM SECTION ================*/
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

Uint16 pwmSetFreq (Uint32 frq) {
	Uint32 clck = (((SysCtrlRegs.PLLCR.bit.DIV*10000000)/2) /((EPwm1Regs.TBCTL.bit.HSPCLKDIV + 1) * (EPwm1Regs.TBCTL.bit.CLKDIV + 1)));

	if (frq >= clck)
		return VALUE_OOB;

	period = (clck / frq);	/* T =  SysClk / f */
	EPwm1Regs.TBPRD = period - 1;
	EPwm2Regs.TBPRD = period - 1;
	EPwm3Regs.TBPRD = period - 1;
	return 0;
}

Uint16 pwmGetFreq (Uint32 *frqDest) {
	Uint32 clock = (((SysCtrlRegs.PLLCR.bit.DIV*10000000)/2) /((EPwm1Regs.TBCTL.bit.HSPCLKDIV + 1) * (EPwm1Regs.TBCTL.bit.CLKDIV + 1)));
	*frqDest = (clock / period); /* f = SysClk / T */
	return 0;
}
