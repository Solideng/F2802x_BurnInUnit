/*
 * PhaseCtrl.c
 *
 *  Created on: 9 Apr 2013
 *      Author: Toby
 */

#include "Common.h"

volatile int32 *PHASE_CTRL_In = 0;

void pcUpdate (void) {
	if ((*PHASE_CTRL_In != 0) && (channel[AC_STAGE].chEnable)) {
		GpioDataRegs.GPASET.bit.GPIO19 = 1;	/* Phase before rectification was negative */
		return;
	}
	GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;	/* Phase before rectification was positive */
}
