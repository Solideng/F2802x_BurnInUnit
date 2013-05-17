/**
 * @file Pwm.h
 *
 * @brief PWM and related functions.
 *
 */

#ifndef PWM_H_
#define PWM_H_

extern Uint16 comp1Trig;	/* Replace these with SCPI register bits when available. */
extern Uint16 comp2Trig;

#define PERIOD 600		/**< Defines the initial PWM period setting = 60MHz / 600 = 100. */

/*============= GLOBAL FUNCTIONS ==============*/
/** Configures PWM trip zones for use.
 * Requires the comparator and DAC to be configured
 * @sa adc.h
 */
extern void pwmTzConfigure (void);

/** Resets the trip zone after a comparator event. */
extern void pwmRstTz (void);

/** Configures each of the PWM macros for use. */
extern void pwmMacroConfigure (void);

/** Configures PWM1 (master) to generate ADC SOC start for ADC macro - configure before initialisation. */
extern void pwmSocConfigure (void);

/** Initialises and enables PWM1 (master) to trigger the DPL ISR. */
extern void pwmDPLTrigInit (void);

/** Sets the frequency of the PWMs.
 * @param[in]	frq	Specifies the required frequency (hertz).
 * @return 			Error status.
 */
extern Uint16 pwmSetFreq (Uint32 frq);

/** Queries the current PWM frequency setting.
 * @param[out]	frqDest	Address of the memory location at which to place the query result (hertz).
 * @return 				Error status.
 */
extern Uint16 pwmGetFreq (Uint32 *frqDest);

/*================ GLOBAL VARS ================*/
/*============= TERMINAL POINTERS =============*/
extern volatile int32 *PWMDRV_2ch_UpCnt_Duty1A;	/**< Channel 0 PWM terminal pointer. */
extern volatile int32 *PWMDRV_2ch_UpCnt_Duty1B;	/**< Channel 1 PWM terminal pointers. */
extern volatile int32 *PWMDRV_2ch_UpCnt_Duty2A;	/**< Channel 2 PWM terminal pointer. */
extern volatile int32 *PWMDRV_2ch_UpCnt_Duty2B;	/**< Channel 3 PWM terminal pointer. */
extern volatile int32 *PWMDRV_2ch_UpCnt_Duty3A;	/**< Interboost PWM terminal pointer. */
extern volatile int32 *PWMDRV_2ch_UpCnt_Duty3B;	/**< AC stage PWM terminal pointer. */

/*========= SYMBOLS DEFINED BY DP LIB =========*/
extern volatile struct EPWM_REGS *ePWM[];
extern void PWM_2ch_UpCnt_CNF(int16 n, int16 period, int16 mode, int16 phase);
extern interrupt void DPL_ISR(void);

#endif /* PWM_H_ */
