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

/*
 * If the TI C2000 LaunchPad is to be powered by a source
 * other than the USB connection the jumpers JP1, JP2 and
 * JP3 should be removed.
 *
 * Lantronix XPort Pro static raw IP: 169.254.11.150:10001
 *
 * Control the TI C2000 LaunchPad XL boot mode using
 * switch 1 (S1):
 * Position the three switches so that all of them are on,
 * then connect USB and use CCS IDE to load the program.
 * When the program is loaded, stop the emulator and
 * disconnect the USB. Then turn switch 3 to the off
 * position. Ensure the terminal emulator is in
 * disconnected mode, then reconnect the USB. Change the
 * terminal emulator mode to connected and press the reset
 * button on the PCB. The program should now be running and
 * the terminal emulator should be able to show received
 * data.
 * | Switch	| Function	|
 * |:------:|:---------:|
 * | 1		| GPIO34	|
 * | 2		| GPIO37	|
 * | 3		| TRSTn		|
 *
 * Project requires modification of the included file
 * DPlib.h, of the digital power application library, by
 * moving *ePwm[] and DLOG_BuffInit() into an external file
 * named DPlibEx.h at the same path location as DPlib.h.
 *
 */
#include "Common.h"
#include "DPlibEx.h"

/* FUNCTION PROTOTYPES */
void DeviceInit(void);
void MemCopy();
#ifdef FLASH		
	void InitFlash();
#endif
// TODO PWM Current isolation circuit may require a calibration functionality - dependent upon circuit design

/* VARIABLE DECLARATIONS */
extern Uint16 *RamfuncsLoadStart, *RamfuncsLoadEnd, *RamfuncsRunStart; /* Used for running BackGround in flash, and ISR in RAM */
/*============================ MAIN CODE - starts here ===========================*/
void main (void) {

	DeviceInit();			/* Device life support & GPIO */
	#ifdef FLASH
							/* Copy time critical code and Flash setup code to RAM */
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		InitFlash();		/* Call the flash wrapper init function */
	#endif

	detectSlaveMode();		/* Detect if the unit is master, single or single */

	initI2c();				/* Initialise the I2C control to external devices */

	if (slaveModeStatus == singleUnit) {
		EALLOW;				/* Disable clock to SPI-A peripheral */
		SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;
		EDIS;
		sciInit(9600);		/* Initialise SCI with 9600 Baud setting for LAN server communications. */
							/* Initialise SCPI with SCI I/O. */
		scpiInit(&registerDeviceCommands, &sciTx);

	} else if (slaveModeStatus == masterUnit) {
		sciInit(9600);		/* Initialise SCI with 9600 baud and SPI as master */
		spiInit(spiMaster, SPI_DFLT_BAUD, disabled, (transPol)SPI_DFLT_CPOL, (spiCPha)SPI_DFLT_CPHA);
							/* Initialise SCPI with SCI I/O */
		scpiInit(&registerDeviceCommands, &sciTx);
	} else if (slaveModeStatus == slaveUnit) {
		EALLOW;				/* Disable clock to SCI-A peripheral */
		SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;
		EDIS;
							/* Initialise SPI as slave */
		spiInit(spiSlave, SPI_DFLT_BAUD, disabled, (transPol)SPI_DFLT_CPOL, (spiCPha)SPI_DFLT_CPHA);
							/* Initialise SCPI with SPI I/O */
		scpiInit(&registerDeviceCommands, &spiTx);
	} else {
		/* Unit type has somehow not been determined */
		// TODO: Put an error in SCPI error queue.
	}

	initStateMachine();		/* Initialise device state machine and timers */

							/* Setup macros and the hardware they use */
	initPwm();				/* Initialise PWM macros */
	initAdc();				/* Initialise the ADCs macros */
	initSine();				/* Initialise the sine generator macro */
	initCoefs();			/* Initialise the IIR control loop coefficient values */
	initDcComparator();		/* Initialise the comparators */
	initAcComparator();
	initTripzone();			/* Initialise trip zone (for comparator outputs) */

	DPL_Init();				/* Initialise the used macros with the DPL ASM ISR */
	setupNets(slaveModeStatus);	/* Setup macro nets and settings according to the unit mode */

							/* Enable Peripheral, global Ints and higher priority real-time debug events: */
	EINT;   				/* Enable Global interrupt INTM */
	ERTM;   				/* Enable Global real-time interrupt DBGM */

							/* Initialise items that required interrupts to initialise - e.g. items on I2C */
	initTemperature();		/* Initialise the temperature sensing */
	initEnableControl();	/* Initialise the external boost converter enable control */

	for(;;)					/* BACKGROUND (BG) LOOP */
	{
		/*=========== State machine entry & exit point ============*/
		(*Alpha_State_Ptr)();	/* jump to an Alpha state (A0,B0,...) */
		/*=========================================================*/
		scpi();	/* Service the SCPI parser */
	}
}
