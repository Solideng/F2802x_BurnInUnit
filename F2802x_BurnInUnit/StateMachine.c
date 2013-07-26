/*
 * StateMachine.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/*================ LOCAL VARS =================*/
static void (*A_Task_Ptr)(void);	/* State pointer A branch */
static void (*B_Task_Ptr)(void);	/* State pointer B branch */
static void (*C_Task_Ptr)(void);	/* State pointer C branch */

/*============== LOCAL FUNCTIONS ==============*/
static void smA0(void);	/* State B0 */
static void smB0(void);	/* State B0 */
static void smC0(void);	/* State C0 */
static void smA1(void);	/* State A1 */
static void smA2(void);	/* State A2 */
static void smB1(void);	/* State B1 */
static void smB2(void);	/* State B2 */
static void smC1(void);	/* State C1 */
static void smC2(void);	/* State C2 */

/*============== GLOBAL FUNCTIONS =============*/
void (*Alpha_State_Ptr)(void);

void smInit (void) {
	#ifdef VTIMERS
		timersSetupVirtual();	/* Setup (clear) virtual timer arrays */
	#endif
	timersSetupReal();			/* Timing sync for background loops */

	Alpha_State_Ptr = &smA0;	/* Set state machine alpha task, i.e., first task for next machine iteration */
	A_Task_Ptr = &smA1;			/* Set first A state */
	B_Task_Ptr = &smB1;			/* Set first B state */
	C_Task_Ptr = &smC1;			/* Set first C state */
}

void smA0(void) {
	/* loop rate synchroniser for A-tasks */
	if(CpuTimer0Regs.TCR.bit.TIF == 1) {
		CpuTimer0Regs.TCR.bit.TIF = 1;	/* clear flag */
		(*A_Task_Ptr)();		/* jump to an A Task (A1,A2,A3,...) */
		#ifdef VTIMERS
			VTimer0[0]++;		/* virtual timer 0, instance 0 (spare) */
		#endif
	}
	Alpha_State_Ptr = &smB0;	/* Comment out to allow only A tasks */
}

void smA1(void) {
	/* Over-current protection, all channel On/Off control */
	adcCheckOcp();

	if(stopAll == 1)		/* All Channel shut-down control (no sequencing) */
		mnStopAll();

	if (enableAll == 1)		/* All Channel enable control */
		mnRunAll();

	A_Task_Ptr = &smA2;		/* Set pointer to next A task */
}

void smA2(void) /* SCI GUI, Slew control, LED current control */
{
//	scSetTarget(DC_STAGE, vMidOut);
//	scSetTarget(AC_STAGE, vMidOut);
	scSlewUpdate();			/* Step the slew values for load channels */
	sgGainUpdate();			/*  and for AC stage */

	A_Task_Ptr = &smA1;		/* Set pointer to next A task */
}

void smB0(void) {
	/* loop rate synchroniser for B-tasks */
	if(CpuTimer1Regs.TCR.bit.TIF == 1) {
		CpuTimer1Regs.TCR.bit.TIF = 1;	/* clear flag */
		(*B_Task_Ptr)();		/* jump to a B Task (B1,B2,B3,...) */
		#ifdef VTIMERS
			VTimer1[0]++;		/* virtual timer 1, instance 0 (used to control SPI LEDs) */
		#endif
	}
	Alpha_State_Ptr = &smC0;
}

void smB1(void) {
	/* Current dash-board measurements */
	#ifdef DEBUG_ADC
		adcGui();
	#endif
	B_Task_Ptr = &smB2;		/* Set pointer to next B task */
}

void smB2(void) {
	cntlUpdateCoefs();		/* Make sure CNTL coefficients are up to date */
	B_Task_Ptr = &smB1;		/* Set pointer to next B task */
}

void smC0(void) {
	/* loop rate synchroniser for C-tasks */
	if(CpuTimer2Regs.TCR.bit.TIF == 1) {
		CpuTimer2Regs.TCR.bit.TIF = 1;	/* clear flag */
		(*C_Task_Ptr)();		/* jump to a C Task (C1,C2,C3,...) */
		#ifdef VTIMERS
			VTimer2[0]++;		/* virtual timer 2, instance 0 (spare) */
		#endif
	}
	Alpha_State_Ptr = &smA0;	/* Back to State A0 */
}

void smC1(void) {
	/* Spare task */
	C_Task_Ptr = &smC2;			/* Set pointer to next C task */
}

void smC2(void) {
	/* Spare task */
	C_Task_Ptr = &smC1;			/* Set pointer to next C task */
}
