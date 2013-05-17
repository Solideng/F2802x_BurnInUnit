/*==============================================================================
 *	FILE:			Main.c (F2802x_BurnInUnit)
 *
 *	Description:	Control of single burn in test unit.
 *					Based on TI basic framework for LED-ColorMix board example
 *
 *	Version: 		1.0
 *
 *  Target:  		TMS320F2802x (PiccoloA) family
 *
 *==============================================================================
 *  Copyright Vox Power Ltd © 2013
 *==============================================================================
 *  Revision History:
 *==============================================================================
 * 	Date		| Author	| Description
 *==============================================================================
 * 	2013 Mar 12	| TM       	| Created Project
 * 	2013 MON NN	| XX       	| Released file
 *==============================================================================
 * Code can be compiled with various build options (Incremental Builds INCR_BUILD),
 *  these options are selected in file "ProjectSettings.h".
 * Note: "Rebuild All" compile must be used if this file is modified.
 *============================================================================*/
#include "Common.h"
#include "DPlibEx.h"

float32 f32Tst;
Uint16 u16Tst, errTst;
Uint32 u32Tst;
cfType enTst;

/* FUNCTION PROTOTYPES */
void DeviceInit(void);
void MemCopy();
void InitCommros();
#ifdef FLASH		
	void InitFlash();
#endif

/* VARIABLE DECLARATIONS */
extern struct Commros commros;	/* Commros */
extern Uint16 *RamfuncsLoadStart, *RamfuncsLoadEnd, *RamfuncsRunStart; /* Used for running BackGround in flash, and ISR in RAM */
/*============================ MAIN CODE - starts here ===========================*/
void main(void)
{
	/* INITIALISATION - General */
	DeviceInit();			/* Device Life support & GPIO */
	//InitCommros();			/* Init Commros lib */

	#ifdef FLASH
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
							/* Copy time critical code and Flash setup code to RAM */
		InitFlash();		/* Call the flash wrapper init function */
	#endif

	smInit();				/* Setup state machine */

	adcSocCnf();			/* Configure Macro ADCs SOCs */
	pwmMacroConfigure();	/* Configure Macro PWMs */

	// TODO VVV !!!! NEEDS TO BE AFTER pwmMacroConfigure() FOR SOME REASON??!?!?!? (ELSE DPL_ISR WILL NOT WORK)
	mnSetupChannels();		/* Setup control loop coefficient values */

	pwmSocConfigure();		/* Configure PWM as SOC trigger */
	adcCompConfigure();		/* Configure comparators */
	pwmTzConfigure();		/* Configure trip zone (for comparator outputs) */

	i2cInit();				/* Initialise the I2C control */
	sgInit();				/* Initialise sine ref generator */

	DPL_Init();				/* DPL ASM ISR init */

	mnConnectNets();		/* Connect Macros to nets */
							/* Enable Peripheral, global Ints and higher priority real-time debug events: */
	pwmDPLTrigInit();		/* Init DPL_ISR trigger */

	EINT;   				/* Enable Global interrupt INTM */
	ERTM;   				/* Enable Global realtime interrupt DBGM */

	tmpInit();				/* Initialise the temperature sensing */
	bcInit();				/* Initialise the external boost converter enable control */

	//scSetStepAll(0.01);		/* Setup initial values */
	#ifdef TEST_SETGET
		// ------------ TEST ITEMS ------------
		// --- ADC Test Section ---
		f32Tst = 0.125;
		errTst = adcSetIScale(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = adcGetIScale(LOAD_0, &f32Tst);
		f32Tst = 1.0;
		errTst = adcSetVScale(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = adcGetVScale(LOAD_0, &f32Tst);
		f32Tst = 2.0;
		errTst = adcSetOcp(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = adcGetOcp(LOAD_0, &f32Tst);
		f32Tst = 2.0;
		errTst = adcSetOvp(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = adcGetOcp(LOAD_0, &f32Tst);
		f32Tst = 2.5;
		errTst = adcSetDac(DC_STAGE, f32Tst);
		f32Tst = 0;
		errTst = adcGetDac(DC_STAGE, &f32Tst);

		// --- CNTL TEST SECTION ---
		for (enTst = firstCoef; enTst < numCoefs; enTst++) {
			f32Tst = 1.0;
			errTst = cntlSetCoef(LOAD_0, enTst, f32Tst);
			f32Tst = 0;
			errTst = cntlGetCoef(LOAD_0, enTst, &f32Tst);
		}

		// --- PWM TEST SECTION ---
		u32Tst = 120000;
		errTst = pwmSetFreq(u32Tst);
		u32Tst = 0;
		errTst = pwmGetFreq(&u32Tst);
		u32Tst = u32Tst;

		// --- SIG GEN TEST SECTION ---
		errTst = sgSetState(TRUE);
		u16Tst = 0;
		errTst = sgGetState(&u16Tst);
		errTst = sgSetState(FALSE);
		u16Tst = 0;
		errTst = sgGetState(&u16Tst);
		errTst = sgSetRectify(TRUE);
		errTst = sgGetRectify(&u16Tst);
		f32Tst = 0.25;
		errTst = sgSetOffset(f32Tst);
		f32Tst = 0;
		errTst = sgGetOffset(&f32Tst);
		f32Tst = 180;
		errTst = sgSetInitialPhase(f32Tst);
		f32Tst = 0.5;
		errTst = sgSetGainTarget(f32Tst);
		f32Tst = 0;
		errTst = sgGetGainTarget(&f32Tst);
		u16Tst = 50;
		errTst = sgSetFreq(u16Tst);
		u16Tst = 0;
		errTst = sgGetFreq(&u16Tst);
		u16Tst = 1000;
		errTst = sgSetFMax(u16Tst);
		u16Tst = 0;
		errTst = sgGetFMax(&u16Tst);
		u16Tst = 3971;
		errTst = sgSetStepMax(u16Tst);
		u16Tst = 0;
		errTst = sgGetStepMax(&u16Tst);
		f32Tst = 0;
		errTst = sgGetResolution(&f32Tst);

		// --- SLEW CTRL TEST SECTION ---
		f32Tst = 1.0;
		errTst = scSetTarget(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = scGetTarget(LOAD_0, &f32Tst);
		f32Tst = 0.1;
		errTst = scSetStep(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = scGetStep(LOAD_0, &f32Tst);
		errTst = scSetState(LOAD_0, TRUE);
		u16Tst = 0;
		errTst = scGetState(LOAD_0, &u16Tst);
		errTst = scSetState(LOAD_0, FALSE);
		errTst = scSetStateAll(TRUE);
		errTst = scSetStateAll(FALSE);

		// --- TMP TEST SECTION ---
		f32Tst = 100;
		errTst = tmpSetOtp(LOAD_0, f32Tst);
		f32Tst = 0;
		errTst = tmpGetOtp(LOAD_0, &f32Tst);

		if (errTst)
			while (1) {}
		//----------- END TEST ITEMS ----------
	#endif

	for(;;)					/* BACKGROUND (BG) LOOP */
	{
		/*=========== State machine entry & exit point ============*/
		(*Alpha_State_Ptr)();	/* jump to an Alpha state (A0,B0,...) */
		/*=========================================================*/
		//ServiceRoutine(&commros);	/* Service the Commros lib */
	}
}
