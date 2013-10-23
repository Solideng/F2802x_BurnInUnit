/*
 * Pwm.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

// N.B. DACs do not currently ramp.

static Uint16 setDcDac (float32 level);

void initDcComparator (void) {
	/* Initialises the DC comparator (COMP 2) using an internal DAC at the inverting input.
	 *  - SHOULD BE CALLED AFTER adcSocCnf() -
	 *  - SHOULD BE CALLED BEFORE PWMS (SYNC) ARE STARTED -
	 *  - SHOULD BE CALLED BEFORE pwmTZConfigure() -
	 *  - midVScale should be set before use
	 */
	EALLOW;									/* COMPCTL & DACCTL are EALLOW protected */
	Comp2Regs.COMPCTL.bit.COMPDACEN = 1;	/* Enable COMP2 section */
	Comp2Regs.DACVAL.bit.DACVAL = 0x3FF;	/* Set the DAC value to the maximum value to begin with */
	Comp2Regs.COMPCTL.bit.COMPSOURCE = 0;	/* COMP2 inverting i/p is connected to internal DAC */
	Comp2Regs.COMPCTL.bit.CMPINV = 0; 		/* COMP2 o/p is not inverted */
	Comp2Regs.COMPCTL.bit.QUALSEL = 3;		/* COMP2 o/p is qualified to 3 clock period */
	Comp2Regs.COMPCTL.bit.SYNCSEL = 1;		/* Synchronous version of COMP2 o/p is used  */
	Comp2Regs.DACCTL.bit.DACSOURCE = 0;		/* Set DACVAL as DAC control source */
	EDIS;
	setDcDac (DCMID_VDCLVL_FIX);	/* Set the DC DAC to the fixed OVP_V_MID_PK value*/
}

static Uint16 setDcDac (float32 level) {
	/* Sets the parameters of the DAC for the DC comparator (COMP 2).
	 * level argument is expected in volts.
	 * - midVScale SHOULD BE SET BEFORE USE -
	 * - SHOULD BE CALLED AFTER initDcComparator() -
	 */
	if (level < 0 )		/* Check the requested level is within lower boundary */
		return VALUE_OOB;
												/* DACLVL = (level * scale * 1023) / 3.3 */
	level *= (1.0 / ((VDDA - VSSA) * 0.001));	/* level / 3v3 */
	level *= (float32) (xfmrSettings.midVScale >> 4);/* level * scale * 1023 */

	if (level > 1023.0)	/* Check the scaled level is within the upper DAC boundary */
		return VALUE_OOB;
						/* Round and cast the level into COMP 2 DAC VAL */
	Comp2Regs.DACVAL.all = (Uint16)(level + 0.5);
	return 0;
}

//Uint16 getDcDac (float32 * level) {
//															/* Get the raw data from COMP 1 DAC VAL * 3v3 */
//	float32 dacLvl = Comp2Regs.DACVAL.all * ((VDDA - VSSA) * 0.001);
//	*level = dacLvl * (1.0 / (xfmrSettings.midVScale >> 4));/* level / (scale * 1023) */
//	return 0;
//}


void initAcComparator (void) {
	/* Sets up the COMP 1 comparator using an internal DAC at the inverting input.
	 *  - SHOULD BE CALLED AFTER adcSocCnf() -
	 *  - SHOULD BE CALLED BEFORE PWMS (SYNC) ARE STARTED -
	 *  - SHOULD BE CALLED BEFORE pwmTZConfigure() -
	 */
	EALLOW;									/* COMPCTL & DACCTL are EALLOW protected */
	Comp1Regs.COMPCTL.bit.COMPDACEN = 1;	/* Enable COMP1 section */
	Comp1Regs.DACVAL.bit.DACVAL = 0x3FF;	/* Set the DAC Value to the maximum value to begin with */
	Comp1Regs.COMPCTL.bit.COMPSOURCE = 0;	/* COMP1 inverting i/p is connected to internal DAC */
	Comp1Regs.COMPCTL.bit.CMPINV = 0; 		/* COMP1 o/p is not inverted */
	Comp1Regs.COMPCTL.bit.QUALSEL = 3;		/* COMP1 o/p is qualified to 3 clock period */
	Comp1Regs.COMPCTL.bit.SYNCSEL = 1;		/* Synchronous version of COMP2 o/p is used */
	Comp1Regs.DACCTL.bit.DACSOURCE = 0;		/* Set DACVAL as DAC control source */
	EDIS;
}

Uint16 setAcDac (float32 level) {
	/* Sets the parameters of the DAC for the AC comparator (COMP 1).
	 * level argument is expected in amps.
	 * - iScale SHOULD BE SET BEFORE USE -
	 * - SHOULD BE CALLED AFTER initAcComparator() -
	 */
	if (level < 0 )		/* Check the requested level is within lower boundary */
		return VALUE_OOB;
												/* DACLVL = (level * scale * 1023) / 3.3 */
	level *= (1.0 / ((VDDA - VSSA) * 0.001));	/* level / 3 */
	level *= (float32) (acSettings.vScale >> 4);/* level * scale * 1023 */

	if (level > 1023.0)	/* Check the scaled level is within the upper DAC boundary */
		return VALUE_OOB;
						/* Round and cast the level into COMP 1 DAC VAL */
	Comp1Regs.DACVAL.all = (Uint16)(level + 0.5);
	return 0;
}

Uint16 getAcDac (float32 * level) {
														/* Get the raw data from COMP 1 DAC VAL * 3v3 */
	float32 dacLvl = Comp1Regs.DACVAL.all * ((VDDA - VSSA) * 0.001);
	*level = dacLvl * (1.0 / (acSettings.vScale >> 4));	/* level / (scale * 1023) */
	return 0;
}

static interrupt void tripzone_ISR (void) {
	Uint16 i = 0;
	stopAll();		/* Stop all channels */
					/* Check flags to see which COMP/SNS caused the interrupt */
	if ((EPwm3Regs.TZFLG.bit.DCAEVT1 == 1) || (EPwm3Regs.TZFLG.bit.DCBEVT1 == 1)) {
		/* AC comparator (COMP1) triggered */
		tripAcOcp();
	}
	if ((EPwm3Regs.TZFLG.bit.DCAEVT2 == 1) || (EPwm3Regs.TZFLG.bit.DCBEVT2 == 1)) {
		/* DC comparator (COMP2) triggered */
		tripDcMidOvp();
	}

	for (i = 1; i <= 3; i++)
		(*ePWM[i]).TZCLR.bit.INT = 1;	/* Clear trip zone interrupt flag, does not clear event flags */

	PieCtrlRegs.PIEACK.bit.ACK2 = 1;	/* Acknowledge this interrupt to allow more from group 2 */
	return;
}

static void initDcTripzone (void) {
	Uint16 i = 0;
	EALLOW;
	for (i = 1; i <= 3; i++) {
			/* Digital compare trip select */
		(*ePWM[i]).DCTRIPSEL.bit.DCALCOMPSEL 	= 0x09;	/* Select COMP2 as digital compare A Low input */
		(*ePWM[i]).DCTRIPSEL.bit.DCBLCOMPSEL 	= 0x09;	/* Select COMP2 as digital compare B Low input */
			/* Digital compare event A qualification */
		(*ePWM[i]).DCACTL.bit.EVT2SRCSEL 		= 0;	/* DCAEVT2 is not filtered */
		(*ePWM[i]).DCACTL.bit.EVT2FRCSYNCSEL 	= 1;	/* DCAEVT2 source is asynchronous */
			/* Digital compare event B qualification */
		(*ePWM[i]).DCBCTL.bit.EVT2SRCSEL 		= 0;	/* DCBEVT2 is not filtered */
		(*ePWM[i]).DCBCTL.bit.EVT2FRCSYNCSEL 	= 1;	/* DCBEVT2 source is asynchronous */
			/* Trip zone digital compare select */
		(*ePWM[i]).TZDCSEL.bit.DCAEVT2 			= 0x04;	/* Select DCAH=x & DCAL=Hi (COMP2) for DCAEVT2 */
		(*ePWM[i]).TZDCSEL.bit.DCBEVT2 			= 0x04;	/* Select DCBH=x & DCBL=Hi (COMP2) for DCBEVT2 */
			/* Trip zone trip event action setting */
		(*ePWM[i]).TZCTL.bit.DCAEVT2 			= 0x02;	/* Set DCAEVT2 (COMP2) to force PWMA to a low state */
		(*ePWM[i]).TZCTL.bit.DCBEVT2 			= 0x02;	/* Set DCBEVT2 (COMP2) to force PWMB to a low state */
	}
	EDIS;
}

void initAcTripzone (void) {
	Uint16 i = 0;
	EALLOW;
	for (i = 1; i <= 3; i++) {
			/* Digital compare trip select */
		(*ePWM[i]).DCTRIPSEL.bit.DCAHCOMPSEL 	= 0x08;	/* Select COMP1 as digital compare A high input */
		(*ePWM[i]).DCTRIPSEL.bit.DCBHCOMPSEL 	= 0x08;	/* Select COMP1 as digital compare B high input */
			/* Digital compare event A qualification */
		(*ePWM[i]).DCACTL.bit.EVT1SRCSEL 		= 0;	/* DCAEVT1 is not filtered */
		(*ePWM[i]).DCACTL.bit.EVT1FRCSYNCSEL 	= 1;	/* DCAEVT1 source is asynchronous */
			/* Digital compare event B qualification */
		(*ePWM[i]).DCBCTL.bit.EVT1SRCSEL 		= 0;	/* DCBEVT1 is not filtered */
		(*ePWM[i]).DCBCTL.bit.EVT1FRCSYNCSEL 	= 1;	/* DCBEVT1 source is asynchronous */
			/* Trip zone digital compare select */
		(*ePWM[i]).TZDCSEL.bit.DCAEVT1 			= 0x02;	/* Select DCAH=Hi & DCAL=x (COMP1) for DCAEVT1 */
		(*ePWM[i]).TZDCSEL.bit.DCBEVT1 			= 0x02;	/* Select DCBH=Hi & DCBL=x (COMP1) for DCBEVT1 */
			/* Trip zone trip event action setting */
		(*ePWM[i]).TZCTL.bit.DCAEVT1 			= 0x02;	/* Set DCAEVT1 (COMP1) to force PWMA to a low state */
		(*ePWM[i]).TZCTL.bit.DCBEVT1 			= 0x02;	/* Set DCBEVT1 (COMP1) to force PWMB to a low state */
	}
	EDIS;
}

static void initTripzoneIsr (void) {
	EALLOW;
		/* Trip zone DC event interrupt enable */
	EPwm3Regs.TZEINT.bit.DCAEVT2 = 1;	/* Set DCAEVT2 to raise an interrupt */
	EPwm3Regs.TZEINT.bit.DCBEVT2 = 1;	/* Set DCBEVT2 to raise an interrupt */
		/* Trip zone AC event interrupt enable */
	EPwm3Regs.TZEINT.bit.DCAEVT1 = 1;	/* Set DCAEVT1 to raise an interrupt */
	EPwm3Regs.TZEINT.bit.DCBEVT1 = 1;	/* Set DCBEVT1 to raise an interrupt */

	EPwm3Regs.TZCLR.all = 0xFFFF;		/* Ensure all flags are clear */

	PieVectTable.EPWM3_TZINT = &tripzone_ISR;	/* Map interrupt to ISR */
	EDIS;

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		/* Enable the PIE block */
	PieCtrlRegs.PIEIER2.bit.INTx3 = 1;		/* Enable in PIE group 2 - INT 3 */
	IER |= M_INT2;							/* Enable CPU INT 2 */
}

void initTripZone (void) {
	initDcTripzone();
	initAcTripzone();
	initTripzoneIsr();
}

//static void rstTripZone (void) {
//	Uint16 i;
//	EALLOW;
//	for (i = 1; i <= 3; i++) {
//		(*ePWM[i]).TZCLR.all = 0x3F;	/* Clear all trip zone flags, restarts PWMs */
//	}
//	EDIS;
//	runAll();
//}

void rstDcTripzone (void) {
	Uint16 i;
	EALLOW;
	for (i = 1; i <= 3; i++) {
		(*ePWM[i]).TZCLR.bit.DCAEVT2 = 1;	/* Clear event trip flags */
		(*ePWM[i]).TZCLR.bit.DCBEVT2 = 1;
	}
	EDIS;
	runAll();
}

void rstAcTripzone (void) {
	Uint16 i;
	EALLOW;
	for (i = 1; i <= 3; i++) {
		(*ePWM[i]).TZCLR.bit.DCAEVT1 = 1;	/* Clear event trip flags */
		(*ePWM[i]).TZCLR.bit.DCBEVT1 = 1;
	}
	EDIS;
	runAll();
}
