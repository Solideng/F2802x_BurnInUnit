/**
 * @file Pwm.h
 *
 * @brief PWM and related functions.
 *
 */

#ifndef PWM_H_
#define PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*================== MACROS ===================*/
/* E.G. 60MHz / 600prd = 100kHz. */
#define PWM_1_PRD 	462	/**< Defines the period setting for PWM1. 60MHz / 462 = 130kHz. */
#define PWM_2_PRD 	462	/**< Defines the period setting for PWM1. 60MHz / 462 = 130kHz. */
#define PWM_3_PRD 	462	/**< Defines the period setting for PWM1. 60MHz / 462 ~ 130kHz. */

/*============= TERMINAL POINTERS =============*/
extern volatile int32_t *PWMDRV_2ch_UpCnt_Duty1A;	/**< Channel 0 PWM terminal pointer. */
extern volatile int32_t *PWMDRV_2ch_UpCnt_Duty1B;	/**< Channel 1 PWM terminal pointers. */
extern volatile int32_t *PWMDRV_2ch_UpCnt_Duty2A;	/**< Channel 2 PWM terminal pointer. */
extern volatile int32_t *PWMDRV_2ch_UpCnt_Duty2B;	/**< Channel 3 PWM terminal pointer. */
extern volatile int32_t *PWMDRV_2ch_UpCnt_Duty3A;	/**< Interboost PWM terminal pointer. */
extern volatile int32_t *PWMDRV_2ch_UpCnt_Duty3B;	/**< AC stage PWM terminal pointer. */

/*============= GLOBAL FUNCTIONS ==============*/

/** Initialises PWM macros, SOCs and ISR (DPL trigger). */
extern void initPwm (void);

//** Configures each of the PWM macros for use. */
//extern void pwmMacroConfigure (void);
//
//** Configures PWM1 (master) to generate ADC SOC start for ADC macro - configure before initialisation. */
//extern void pwmSocConfigure (void);
//
//** Initialises and enables PWM1 (master) to trigger the DPL ISR. */
//extern void pwmDPLTrigInit (void);

//** Sets the frequency of the PWMs.
// * @param[in]	frq	Specifies the required frequency (hertz).
// * @return 			Error status.
// */
//extern Uint16 pwmSetFreq (Uint32 frq);
//
//** Queries the current PWM frequency setting.
// * @param[out]	frqDest	Address of the memory location at which to place the query result (hertz).
// * @return 				Error status.
// */
//extern Uint16 pwmGetFreq (Uint32 *frqDest);

/*========= SYMBOLS DEFINED BY DP LIB =========*/
extern volatile struct EPWM_REGS *ePWM[];
extern void PWM_2ch_UpCnt_CNF(int16 n, int16 period, int16 mode, int16 phase);

/** Digital power control loop interrupt service routine
 * Located in the assembly file BurnInUnit_ISR.asm
 */
extern interrupt void DPL_ISR(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* PWM_H_ */
