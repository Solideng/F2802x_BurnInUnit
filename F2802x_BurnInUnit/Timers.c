/*
 * Timers.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

#ifdef VTIMERS
	int16_t VTimer0[4] = {0};
	int16_t VTimer1[4] = {0};
	int16_t VTimer2[4] = {0};

	void timersSetupVirtual (void) {
		uint16_t i = 0;
		for (i = 0; i < 4; i++) {
			VTimer0[i]=0;		// Virtual Timers slaved off CPU Timer 0 (A events)
			VTimer1[i]=0; 		// Virtual Timers slaved off CPU Timer 1 (B events)
			VTimer2[i]=0; 		// Virtual Timers slaved off CPU Timer 2 (C events)
		}
	}
#endif

void timersSetupReal (void) {
	CpuTimer0Regs.PRD.all =  mSec1;			// A tasks
	CpuTimer1Regs.PRD.all =  mSec10;		// B tasks
	CpuTimer2Regs.PRD.all =  mSec50;		// C tasks
}


