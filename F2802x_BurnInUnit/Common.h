/**
 * @file Common.h
 *
 * @brief Common include file for the project.
 *
 * All other header files used should be included within this file and this
 * file should then be used to include them in the required source files.
 *
 */
#ifndef COMMON_H_
#define COMMON_H_

/* Core device definitions and functions */
#include "Settings.h"
#include "PeripheralHeaderIncludes.h"
#include "DSP2802x_EPWM_defines.h"
/* Additional function libraries */
#include "IQMathLib.h"
#include "SQMath.h"
#include "DPlib.h"
#include "sgen.h"
/* Application includes */
/* State machine */
#include "StateMachine.h"
/* Communications */
#include "I2c.h"
#include "Sci.h"
#include "Spi.h"
/* Macro connection */
#include "MacroNets.h"
/* Macro terminals & function control */
#include "slaveMode.h"
#include "Timers.h"
#include "Adc.h"
#include "Pwm.h"
#include "Cntl.h"
#include "SlewControl.h"
#include "SineGen.h"
#include "PhaseCtrl.h"
#include "tmp.h"
#include "EnableCtrl.h"

#include "../../SCPI_Build/SCPI_Build/scpi/scpi.h"
#include "SCPI_specificCmds.h"

/*================== MACROS ===================*/
#define	FALSE	0
#define	TRUE	1

/*============================================================================
 * DO NOT MODIFY THE FOLLOWING LINES!
 *
 * These create the accurate DELAY_US() macro that calls the DSP28x_usDelay()
 *  assembly function in DSP2802x_usDelay.asm
 *===========================================================================*/
void DSP28x_usDelay(Uint32 Count);	/* ASM micro second delay called by DELAY_US() macro. */
#define CPU_RATE   16.667L   		/* Set CPU clock speed (SYSCLKOUT) of 60MHz for macro use. */
									/* Macro accounts for DSP28x_usDelay() cycle overhead. */
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)
/*============================================================================
 *===========================================================================*/

#endif /* F2802X_BURNINUNIT_COMMON_H_ */
