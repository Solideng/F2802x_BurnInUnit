/**
 * @file Settings.h
 *
 * @brief Major build definitions and settings for the project.
 *
 * @warning
 * This file is included and referenced by ISR.asm, main() and mnConnectNets().
 *
 * @warning
 * When changes are made to this file please use rebuild all.
 *
 */

#ifndef _PROJSETTINGS_H
#define _PROJSETTINGS_H

/*==============================================================
 * Build options
 *============================================================*/
#define INCR_BUILD 2	/**< Alters the digital power control loop between closed or open.
 	 	 	 	 	 	 * Open-Loop: 1.
 	 	 	 	 	 	 * Closed-loop: 2.
 	 	 	 	 	 	 */

//#define AC_STAGE_OPEN	/**< Sets AC stage PWM duty with SIN output instead of IIR filter control law output. */

#define DEBUG			/**< Includes and makes functions and variables public that are used only for debugging purposes. */

#define LOG_SIN		/**< Logs the sine wave VOut and SignOut data into data-log buffers for debug graphing. */

//#define VTIMERS		/**< Enables virtual timer code for use */

//#define DUAL_CNTL_AC	/**< Uses the dual CNTL AC control instead of single VCtrl. Cannot be used if PID is still in use. */

#ifdef DEBUG
	circuitSection enableSection = 10; /* Set to 0-7 for one state machine iteration to enable a section, set >7 for no action  */
	circuitSection disableSection = 10; /* Set to 0-7 for one state machine iteration to disable a section, set >7 for no action */
#endif
/*==============================================================
 * System Settings
 *============================================================*/

#define VSSA 				0l		/**< System VLOWREF (millivolts). */
#define VDDA 				3300l	/**< System VMAXREF (millivolts). */

#define LOAD_I_SCALE		0.125f	/**< Scaling factor for load current ADC. */ // TODO: Get correct value!!
#define LOAD_V_R1 			18.0f	/**< Scaling voltage divider R1 resistor value for load voltage ADC. */
#define LOAD_V_R2 			1.0f	/**< Scaling voltage divider R2 resistor value for load voltage ADC. */

#define HV_V_R1 			540.0f	/**< Scaling voltage divider R1 resistor value for HV voltage ADC. */
#define HV_V_R2 			4.3f	/**< Scaling voltage divider R2 resistor value for HV voltage ADC. */

#define MID_I_SCALE 		1.0f	/**< Scaling factor for MID current ADC. */ // TODO: Get correct value!!
#define MID_V_R1			540.0f	/**< Scaling voltage divider R1 resistor value for MID voltage ADC. */
#define MID_V_R2			4.3f	/**< Scaling voltage divider R2 resistor value for MID voltage ADC. */

#define AC_I_SCALE 			1.0f	/**< Scaling factor for AC current ADC. */ // TODO: Get correct value!!
#define AC_V_R1				540.0f	/**< Scaling voltage divider R1 resistor value for AC voltage ADC. */
#define AC_V_R2				4.3f	/**< Scaling voltage divider R2 resistor value for AC voltage ADC. */

// TODO: Remove NUM_CHNLS. Use numberOfLoads etc in specificCmds.
//#define NUM_CHNLS 	6		/**< Total number of IIR filter control law macros used (doesn't include VMID semi-channel). */

#define LOAD_IDCLVL_MAX 	35		/**< The maximum allowable value, in amps, for the load current levels. */
#define LOAD_VDCLVL_FIX 	60		/**< The fixed maximum value, in volts, for the load voltage levels. */
#define LOAD_PWRLVL_FIX 	200		/**< The fixed maximum value, in watts, for the load power levels. */

#define DCMID_VDCLVL_FIX 	400		/**< The fixed maximum value, in volts, for the DC mid voltage level. */
#define DCMIDV_STRTUP_LMT 	10.0f	/**< The maximum limit for DC MID voltage at to allow transformer stage startup. */

#define DCHV_UVLCK_LMT 		330.0f	/**< The DC HV under-voltage lockout limit level. */

#define AC_IRMSLVL_MAX 		10		/**< The maximum allowable value, in amps (RMS), for the AC current level. */
#define AC_VRMSLVL_MAX 		250		/**< The maximum allowable value, in volts (RMS), for the AC voltage level. */
#define AC_PWRLVL_FIX 		800		/**< The fixed value, in watts, for the AC OPP level. */

#define XFMR_PWMF_FIX 		130		/**< The fixed frequency (kHz) setting for the transformer PWM (at 50% duty). */

#define SQRT_2				1.41429	/**< Sqrt(2) constant used for RMS calculations. */
#define RECP_SQRT_2			0.70711	/**< 1/sqrt(2) constant used for RMS calculations. */

// TODO: Not used??
//#define	uSec100				6000	/**< 100us - System define. */

/*==============================================================
 * System Error Codes
 *=============================================================*/
/**@{*/
#define CHANNEL_OOB 		0x10	/**< Channel out of bounds error code. */
#define VALUE_OOB	 		0x11	/**< Value out of bounds error code. */
#define OCP_TRIP			0x12	/**< Over-current protection trip error code. */
#define OVP_TRIP			0x13	/**< Over-voltage protection trip error code. */
#define OPP_TRIP			0x14	/**< Over-power protection trip error code. */
#define OTP_TRIP			0x15	/**< Over-temperature protection trip error code. */
#define UVLKT_TRIP 			0x16	/**< Under-voltage lock-out trip error code. */
#define TRIP_ALERT 			0x17	/**< General purpose alert trip error code. */

#define I2C_READ_WRONG_MSG	0x20	/**< Incorrect type I2C message read error code. */
#define I2C_WRITE_WRONG_MSG	0x21	/**< Incorrect type I2C write message error code. */
#define I2C_STP_NOT_READY	0x22	/**< I2C stop bit was not yet received error code. */
#define I2C_BUS_BUSY		0x23	/**< I2C bus already busy error code. */
#define I2C_INVALID_ISRC	0x24	/**< Invalid I2C interrupt source error code. */
/**@}*/

#endif
