/*
 * Pwm.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

// N.B. DACs do not currently ramp.

void initDcComparator (void) {
	/* Initialises the DC comparator (COMP 2) using an internal DAC at the inverting input.
	 *  - SHOULD BE CALLED AFTER adcSocCnf() -
	 *  - SHOULD BE CALLED BEFORE PWMS (SYNC) ARE STARTED -
	 *  - SHOULD BE CALLED BEFORE pwmTZConfigure() -
	 */
	EALLOW;									/* COMPCTL & DACCTL are EALLOW protected */
	Comp2Regs.COMPCTL.bit.COMPDACEN = 1;	/* Enable COMP1 section */
	Comp2Regs.DACVAL.bit.DACVAL = 0x3FF;	/* Set the DAC value to the maximum value to begin with */
	Comp2Regs.COMPCTL.bit.COMPSOURCE = 0;	/* COMP2 inverting i/p is connected to internal DAC */
	Comp2Regs.COMPCTL.bit.CMPINV = 0; 		/* COMP2 o/p is not inverted */
	Comp2Regs.COMPCTL.bit.QUALSEL = 3;		/* COMP2 o/p has no qualification window length */
	Comp2Regs.COMPCTL.bit.SYNCSEL = 1;		/* COMP2 o/p is not synchronised to the SysClk */
	Comp2Regs.DACCTL.bit.DACSOURCE = 0;		/* Set DACVAL as DAC control source */
	EDIS;
}

Uint16 setDcDac (float32 level) {
	/* Sets the parameters of the DAC for the DC comparator (COMP 2).
	 * level argument is expected in volts.
	 * - iScale SHOULD BE SET BEFORE USE -
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

Uint16 getDcDac (float32 * level) {
															/* Get the raw data from COMP 1 DAC VAL * 3v3 */
	float32 dacLvl = Comp2Regs.DACVAL.all * ((VDDA - VSSA) * 0.001);
	*level = dacLvl * (1.0 / (xfmrSettings.midVScale >> 4));/* level / (scale * 1023) */
	return 0;
}


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
	Comp1Regs.COMPCTL.bit.QUALSEL = 3;		/* COMP1 o/p has no qualification window length */
	Comp1Regs.COMPCTL.bit.SYNCSEL = 1;		/* COMP1 o/p is not synchronised to the SysClk */
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
