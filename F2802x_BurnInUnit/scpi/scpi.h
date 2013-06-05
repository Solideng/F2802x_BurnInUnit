/**
 * @file scpi.h
 *
 * Include this header in your project to add the SCPI parser
 *
 * This file includes all other necessary files.
 *
 * In this file you should edit the lines marked as user
 * editable before compilation
 *
 */

#ifndef SCPICOMMON_H_
#define SCPICOMMON_H_

// TODO: try reduce mount of global and uninitialised variables to reduce size needed for .ebss.
// Tree and queues taking up quite a bit of space...

#ifdef __cplusplus
	extern "C" {
#endif

/*===================== USER EDITABLE VALUES ======================*/

#define MANUF 	"VOXPOW"		/**< Manufacturer for *IDN? response field 1, required. Should be identical for
 	 	 	 	 	 	 	 	 * all devices produced by a single company. User editable. Maximum length of 18
 	 	 	 	 	 	 	 	 * characters.
 	 	 	 	 	 	 	 	 */
#define MODEL 	"M003"			/**< Model for *IDN? response field 2, required. Should NOT contain the word
 	 	 	 	 	 	 	 	 * "MODEL". User editable. Maximum length of 18 characters.
 	 	 	 	 	 	 	 	 */
#define SERIAL 	"S001"			/**< Serial number for *IDN? response field 3, ASCII character 0 if not available,
								 * if unique serial numbers are used, then ASCII character 0 shall NOT be used.
								 * User editable. Maximum length of 18 characters.
								 */
#define FIRMW 	"F003"			/**< Firmware level or equivalent for *IDN command field 4, ASCII character 0 if
								 * not available. Should contain information about all separately revisable
								 * subsystems. This information can be contained in single or multiple revision codes.
								 *  User editable. Maximum length of 18 characters.
								 */
#define INBUFF_LENGTH 100		/**< Length of the input buffer in bytes where one byte is equal to one ASCII character. User editable. */
#define OQUEUE_LENGTH 100		/**< Length of the response output queue in bytes where one byte is equal to one ASCII character. User editable. */

#define TREE_CHILD_LIMIT 102	/**< Used to limit the command tree size. User editable.
								 * Each tree item consumes 28 bytes.
								 * Therefore the total memory required for the tree can be calculated to be equal to TREE_CHILD_LIMIT x 28 bytes
								 * E.G. 102 items x 28 bytes = 2.856 kB
								 * NOTE: TMS320C28x byte is 16 bits.
								 */

#define GPIB_EMULATION	1		/**< Used to select GPIB emulation mode. User editable.
								 * - 0: No emulation
 	 	 	 	 	 	 	 	 * - 1: Serial emulation according to IEEE 1174-2000 @sa SCPI_serialEmulation.h
 	 	 	 	 	 	 	 	 * - 2+: Not yet implemented
 	 	 	 	 	 	 	 	 */

#include "../Common.h"			/**< Add access to device specific functions. User editable. */

/*======================== STOP EDITING HERE ======================*/

#include "stdlib.h" /* Used for atof() */

#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL (void *)0
	#endif
#endif


#ifndef _Bool
	#define _Bool
	/** Defines a boolean type and its possible values */
	typedef enum bool {
		false = 0,	/**< false (0) */
		true		/**< true (1 or not-zero) */
	} bool;
#endif
/** Device specific definitions */
#include "DSP2802x_Device.h"

/** SCPI message exchange control */
#include "SCPI_msgEPCtrl.h"

/** Creates SCPI required registers */
#include "SCPI_regs.h"
/** Creates application specific SCPI related registers */
#include "SCPI_specificRegs.h"

/** Provides string manipulation functions */
#include "SCPI_string.h"
/** Sets up and creates the SCPI command tree */
#include "SCPI_buildTree.h"
/** SCPI common command functions and registration */
#include "SCPI_commonCmds.h"
/** SCPI required command functions and registration */
#include "SCPI_requiredCmds.h"
/** Application specific SCPI command functions and registration */
#include "SCPI_specificCmds.h"

#if GPIB_EMULATION != 0
	#if GPIB_EMULATION == 1
		/** IEE 1174-2000 serial GPIB emulation */
		#include "SCPI_serialEmulation.h"
	#else
		/** 2+ emulation modes not yet defined */
		#error "An invalid emulation mode has been specified"
	#endif
#endif

/** SCPI input buffer and management */
#include "SCPI_inputBuffer.h"
/** SCPI response output queue and management */
#include "SCPI_outputQueue.h"
/** SCPI lexer */
#include "SCPI_lexer.h"
/** SCPI parser */
#include "SCPI_parser.h"
/** SCPI execution control */
#include "SCPI_execCtrl.h"
/** SCPI response formatter */
#include "SCPI_respFmtr.h"

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPICOMMON_H_*/
