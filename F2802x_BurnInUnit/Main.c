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

/* R2 H/W Issues:
 *  - 2x4 header cables should be ~50mm longer
 *  - CTRL PCB - U8 & U10 incorrect footprint.
 *  - CTRL PCB - J14 incorrect footprint for locating pegs.
 *  - CTRL PCB - J10, J11, J12, J13, J15, J16 placed to close together
 *  - CTRL PCB - C23 changed from 1nF to 10nF for noise reduction.
 *  - CTRL PCB - Added 1k pullups to I2C SCK & SDA
 *  - XFMR PCB - 12VIN not connected to 12V track - Added mod wire.
 *  - XFMR PCB - SGND not connected to PWR GND - Added mod wire.
 *  - AC PCB - SGND not connected to PGND - Added mod wire.
 *  - AC PCB - Gain setting resistor needed on U2 - Added 100R & 10k for gain ~100.
 *  - AC PCB - R19 & R20 changed from 0R to 100R.
 *  - AC PCB - C27 added with 100nF.
 *  - AC PCB - Added diodes D1, D2, D3, D4, D5 & D6 to improve OFF switching.
 *  - AC PCB - VSns track (ACVSNS net) has lots of noise due to routing under low side switches
 *  - IP PCB - J4-1 & J4-2 reversed - Requires new layout - Adjusted screws & added mod cable.
 *  - IP PCB - SGND & PWR GND not connected - Relied on connection through XFMR PCB GND.
 */

/*
 * If the TI C2000 LaunchPad is to be powered by a source
 * other than the USB connection the jumpers JP1, JP2 and
 * JP3 should be removed.
 *
 * Lantronix XPort Pro static raw IP: 169.254.11.150:10001
 *
 * Control the TI C2000 LaunchPad XL boot mode using
 * switch 1 (S1):
 * Position the three switches (S1-1, S1-2, S1-3) so that
 * all of them are on, then connect USB and use CCS IDE to
 * load the program. When the program is loaded, stop the
 * emulator and disconnect the USB. Then turn switch S1-3
 * to the off position. Ensure the terminal emulator is in
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

/*=====TEST VARS=====*/
#ifdef DEBUG
	float chan1Target = 1.0;
	float chan2Target = 1.0;
#endif
/*===================*/

/* VARIABLE DECLARATIONS */
extern Uint16 *RamfuncsLoadStart, *RamfuncsLoadEnd, *RamfuncsRunStart; /* Used for running BackGround in flash, and ISR in RAM */
/*============================ MAIN CODE - starts here ===========================*/
void main (void) {
	DeviceInit();			/* Device life support & GPIO */
	#ifdef FLASH
							/* Copy time critical code and Flash setup code to RAM */
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		InitFlash();		/* Initialise the flash wrapper function */
	#endif

	detectSlaveMode();		/* Detect if the unit is master, slave or single */

	initI2c();				/* Initialise the I2C control to external devices (ADC, I/O expander) */

		/* Enable Peripheral, global interrupts and higher priority real-time debug events
		 * This block with enabled interrupts allows things, like the enable control, to be
		 * initialised before code that starts output, such as initPwm(), are initialised.
		 */
	EINT;   				/* Enable global interrupt INTM */
	ERTM;   				/* Enable global real-time interrupt DBGM */
	initTemperature();		/* Initialise the temperature sensing. Uses I2C */
							/* Initialise the external circuit enable control
							 * - makes sure everything is disabled. Uses I2C
							 */
	initEnableControl();
		/* Disable interrupts again */
	DRTM;					/* Disable global real-time interrupt DBGM */
	DINT;					/* Disable global interrupt INTM */

		/* Initialises SPI, SCI and SCPI with different configurations depending on the unit's slave mode */
	if (slaveModeStatus == singleUnit) {
		EALLOW;				/* Disable clock to SPI-A peripheral */
		SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;
		EDIS;
		sciInit(9600);		/* Initialise SCI with 9600 Baud setting for LAN server communications. */
							/* Initialise SCPI with SCI I/O. */
		scpiInit(&registerDeviceCommands, &sciTx);

	} else if (slaveModeStatus == masterUnit) {
		sciInit(9600);		/* Initialise SCI with 9600 baud and SPI as master */
		//spiInit(spiMaster, SPI_DFLT_BAUD, disabled, (transPol)SPI_DFLT_CPOL, (spiCPha)SPI_DFLT_CPHA);
							/* Initialise SCPI with SCI I/O */
		scpiInit(&registerDeviceCommands, &sciTx);
	} else if (slaveModeStatus == slaveUnit) {
		EALLOW;				/* Disable clock to SCI-A peripheral */
		SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;
		EDIS;
							/* Initialise SPI as slave */
		//spiInit(spiSlave, SPI_DFLT_BAUD, disabled, (transPol)SPI_DFLT_CPOL, (spiCPha)SPI_DFLT_CPHA);
							/* Initialise SCPI with SPI I/O */
		//scpiInit(&registerDeviceCommands, &spiTx);
	} else {
		/* Unit type has somehow not been determined */
		// TODO: Put an error in SCPI error queue.
	}

	initStateMachine();		/* Initialise device state machine and timers */
		/* Setup macros and the hardware they use */
	initPwm();				/* Initialise PWM macros */
	initAdc();				/* Initialise the ADCs macros */
	initSine(slaveModeStatus);	/* Initialise the sine generator macro */
	initDcComparator();		/* Initialise the comparators */
	initAcComparator();
	DPL_Init();				/* Initialise the used macros with the DPL ASM ISR */
	initCoefs();			/* Initialise the IIR control loop coefficient values */
	setupNets(slaveModeStatus);	/* Setup macro nets and settings according to the unit mode */
		/* Enable Peripheral, global Ints and higher priority real-time debug events: */
	EINT;   				/* Enable Global interrupt INTM */
	ERTM;   				/* Enable Global real-time interrupt DBGM */

	#ifdef DEBUG
		//=============== TEST CODE ===============
//		setSineRmsTarget(10.0);
		// Setup load channels' targets, slew rates, OCP levels and OTP levels.
		// Coefficients are left as default.
		setLoadSlewTarget(load1, 1.0);
		setLoadSlewStep(load1, 0.001);
		setLoadOcpLevel(load1, 30.0);
		setLoadOtpLevel(load1, 100.0);

		setLoadSlewTarget(load2, 1.0);
		setLoadSlewStep(load2, 0.001);
		setLoadOcpLevel(load2, 30.0);
		setLoadOtpLevel(load2, 100.0);

//		setLoadSlewTarget(load3, 1.0);
//		setLoadSlewStep(load3, 0.001);
//		setLoadOcpLevel(load3, 30.0);
//		setLoadOtpLevel(load3, 100.0);
//
//		setLoadSlewTarget(load4, 1.0);
//		setLoadSlewStep(load4, 0.001);
//		setLoadOcpLevel(load4, 30.0);
//		setLoadOtpLevel(load4, 100.0);

		// Setup xfmr stage OCP, OVP and OTP levels.
//		setDcMidOcpLevel(15.0);
//		setDcHvOvpLevel(15.0);	// TODO: Max limit needs correct value.
		setDcOtpLevel(100.0);

		// Setup AC stage gain target, gain rate, OCP level, OVP level and OTP level.
		// Coefficients are left as default.
//		setSineGainTarget(0.04);
//		setSineGainStep(0.001);
//		setAcOcpLevel(15.0);	// TODO: Max limit needs correct value.
//		setAcOvpLevel(15.0);	// TODO: Max limit needs correct value.
		setAcOtpLevel(60.0);


		enableCircuit(xfmrCct);
		enableCircuit(chan1);
		enableCircuit(chan2);
//		enableCircuit(acCct);
		// Enable all stages in sequence
//		runAll();
		//============= END TEST CODE =============
	#endif

	for(;;)					/* BACKGROUND (BG) LOOP */
	{
		/*=========== State machine entry & exit point ============*/
		(*Alpha_State_Ptr)();	/* jump to an Alpha state (A0,B0,...) */
		/*=========================================================*/
		scpi();	/* Service the SCPI parser */
	}
}
