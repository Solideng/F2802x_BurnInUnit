/*
 * StateMachine.c
 *
 *  Created on: 15 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

#ifdef DEBUG
	circuitSection enableSection = maxchan; /* Set to 0-7 for one state machine iteration to enable a section, set >= maxchan for no action  */
	circuitSection disableSection = maxchan; /* Set to 0-7 for one state machine iteration to disable a section, set >= maxchan for no action */
#endif

/*================ LOCAL VARS =================*/
static void (*A_Task_Ptr)(void);	/* State pointer A branch */
static void (*B_Task_Ptr)(void);	/* State pointer B branch */
static void (*C_Task_Ptr)(void);	/* State pointer C branch */

/*============== LOCAL FUNCTIONS ==============*/
static void loopASync(void);	/* State B0 */
static void loopBSync(void);	/* State B0 */
static void loopCSync(void);	/* State C0 */
static void loopATask1(void);	/* State A1 */
static void loopATask2(void);	/* State A2 */
static void loopBTask1(void);	/* State B1 */
static void loopBTask2(void);	/* State B2 */
static void loopCTask1(void);	/* State C1 */
static void loopCTask2(void);	/* State C2 */

/*============== GLOBAL FUNCTIONS =============*/
void (*Alpha_State_Ptr)(void);

void initStateMachine (void) {
	#ifdef VTIMERS
		timersSetupVirtual();	/* Setup (clear) virtual timer arrays */
	#endif
	timersSetupReal();			/* Timing sync for background loops */

	Alpha_State_Ptr = &loopASync;	/* Set state machine alpha task, i.e., first task for next machine iteration */
	A_Task_Ptr = &loopATask1;			/* Set first A state */
	B_Task_Ptr = &loopBTask1;			/* Set first B state */
	C_Task_Ptr = &loopCTask1;			/* Set first C state */
}

void loopASync (void) {
	/* loop rate synchroniser for A-tasks */
	if(CpuTimer0Regs.TCR.bit.TIF == 1) {
		CpuTimer0Regs.TCR.bit.TIF = 1;	/* clear flag */
		(*A_Task_Ptr)();		/* jump to an A Task (A1,A2,A3,...) */
		#ifdef VTIMERS
			VTimer0[0]++;		/* virtual timer 0, instance 0 (spare) */
		#endif
	}
	Alpha_State_Ptr = &loopBSync;	/* Comment out to allow only A tasks */
}

void loopATask1 (void) {
	/* Over-current protection, all channel On/Off control */
	/* Check for over-current */
//	checkLoadOcp(load1);	//TODO: Commented out for test usage.
//	checkLoadOcp(load2);
//	checkLoadOcp(load3);
//	checkLoadOcp(load4);
	checkDcMidOcp();

	/* Check for over-voltage */
//	checkLoadOvp(load1);
//	checkLoadOvp(load2);
//	checkLoadOvp(load3);
//	checkLoadOvp(load4);
	checkDcHvOvp();
	if (slaveModeStatus != slaveUnit)
		checkAcOvp();

	/* Check for over-power */
//	checkLoadOpp(load1);
//	checkLoadOpp(load2);
//	checkLoadOpp(load3);
//	checkLoadOpp(load4);
	checkAcOpp();

	A_Task_Ptr = &loopATask2;		/* Set pointer to next A task */
}

void loopATask2 (void) {
	/* OTP, slew and gain update */
	/* Check for over-temperature */
//	checkLoadOtp(load1);	//TODO: Commented out for test usage.
//	checkLoadOtp(load2);
//	checkLoadOtp(load3);
//	checkLoadOtp(load4);
	checkDcOtp();
	checkAcOtp();
//	checkExtOtp(ext1);
//	checkExtOtp(ext2);

	updateLoadSlew();		/* Step the slew values for load channels */
	updateSineGain();		/*  and for AC stage */

	A_Task_Ptr = &loopATask1;		/* Set pointer to next A task */
}

void loopBSync (void) {
	/* loop rate synchroniser for B-tasks */
	if(CpuTimer1Regs.TCR.bit.TIF == 1) {
		CpuTimer1Regs.TCR.bit.TIF = 1;	/* clear flag */
		(*B_Task_Ptr)();		/* jump to a B Task (B1,B2,B3,...) */
		#ifdef VTIMERS
			VTimer1[0]++;		/* virtual timer 1, instance 0 (used to control SPI LEDs) */
		#endif
	}
	Alpha_State_Ptr = &loopCSync;
}

void loopBTask1 (void) {
	/* Current dash-board measurements */
	#ifdef DEBUG_ADC
		adcGui();
	#endif
	B_Task_Ptr = &loopBTask2;		/* Set pointer to next B task */
}

void loopBTask2 (void) {
	B_Task_Ptr = &loopBTask1;		/* Set pointer to next B task */
}

void loopCSync(void) {
	/* loop rate synchroniser for C-tasks */
	if(CpuTimer2Regs.TCR.bit.TIF == 1) {
		CpuTimer2Regs.TCR.bit.TIF = 1;	/* clear flag */
		(*C_Task_Ptr)();		/* jump to a C Task (C1,C2,C3,...) */
		#ifdef VTIMERS
			VTimer2[0]++;		/* virtual timer 2, instance 0 (spare) */
		#endif
	}
	Alpha_State_Ptr = &loopASync;	/* Back to State A0 */
}

void loopCTask1 (void) {
	/* Spare task */

	/* Allows circuit enable state to be changed in watch window during debug */
	#ifdef DEBUG
		if (enableSection < maxchan)
			enableCircuit(enableSection);
		if (disableSection < maxchan)
			disableCircuit(disableSection);
	#endif
	C_Task_Ptr = &loopCTask2;			/* Set pointer to next C task */
}

void loopCTask2 (void) {
	/* Spare task */
	C_Task_Ptr = &loopCTask1;			/* Set pointer to next C task */
}
