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

//#define LOG_SIN		/**< Logs the sine wave VOut and SignOut data into data-log buffers for debug graphing. */

//#define VTIMERS		/**< Enables virtul timer code for use */

#define DUAL_CNTL_AC	/**< Uses the dual CNTL AC control instead of single VCtrl. Cannot be used if PID is still in use. */

/*==============================================================
 * System Settings
 *============================================================*/

#define VSSA 		0l		/**< System VLOWREF (millivolts). */

#define VMID_R1		540.0	/**< Scaling voltage divider R1 resistor value for VMID ADC. */
#define VMID_R2		4.3		/**< Scaling voltage divider R2 resistor value for VMID ADC. */
#define VAC_R1 		540.0	/**< Scaling voltage divider R1 resistor value for VAC ADC. */
#define VAC_R2 		4.3		/**< Scaling voltage divider R2 resistor value for VAC ADC. */

#define NUM_ICTRL_CHNLS	5	/**< The number of current, or 2-pole 2-zero, IIR filter control law macros used. */
#define NUM_VCTRL_CHNLS 2	/**< The number of voltage, or 3-pole 3-zero, IIR filter control law macros used. */
#define NUM_CHNLS 	NUM_ICTRL_CHNLS + NUM_VCTRL_CHNLS	/**< Total number of IIR filter control law macros used (doesn't include VMID semi-channel). */

#define SQRT_2		1.41429	/**< Sqrt(2) constant used for RMS calculations. */
#define RECP_SQRT_2	0.70711	/**< 1/sqrt(2) constant used for RMS calculations. */
#define VDDA 3300l	/**< System VMAXREF (millivolts). */

#define	uSec100		6000	/**< 100us - System define. */

/*==============================================================
 * System Error Codes
 *=============================================================*/
/**@{*/
#define CHANNEL_OOB 		0x10	/**< Channel out of bounds error code. */
#define VALUE_OOB	 		0x11	/**< Value out of bounds error code. */
#define OCP_TRIP			0x12	/**< Over-current protection trip error code. */
#define OVP_TRIP			0x13	/**< Over-voltage protection trip error code. */
#define OTP_TRIP			0x14	/**< Over-temperature protection trip error code. */

#define I2C_READ_WRONG_MSG	0x20	/**< Incorrect type I2C message read error code. */
#define I2C_WRITE_WRONG_MSG	0x21	/**< Incorrect type I2C write message error code. */
#define I2C_STP_NOT_READY	0x22	/**< I2C stop bit was not yet received error code. */
#define I2C_BUS_BUSY		0x23	/**< I2C bus already busy error code. */
#define I2C_INVALID_ISRC	0x24	/**< Invalid I2C interrupt source error code. */
/**@}*/

#endif
