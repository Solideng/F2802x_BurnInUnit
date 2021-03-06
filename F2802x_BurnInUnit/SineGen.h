/**
 * @file SineGen.h
 *
 * @brief Signal generator functions.
 *
 * sgInit() must be called before any other signal
 * generator functions are used.
 * Note that the frequency resolution is determined
 * by the maximum frequency and the step max.
 * For further details, see the signal generator library
 * documentation (Texas Instruments Signal Generator
 * Library Module user's Guide).
 *
 * @warning This file is included by the file ISR.asm.
 *
 */

// TODO: Add setSineGainStep();

#ifndef SINEGEN_H_
#define SINEGEN_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SIN_F_ISR 		60000000.0f/PWM_1_PRD	/**< Sampling frequency. I.e. the frequency at which the DPL ISR task runs. */
#define SIN_F_ISR_DVSR 	6		/**< Sampling frequency divisor. E.g. F_ISR = 130kHz, but the DPL ISR only runs on every
									 third trigger plus there are two alternating tasks so, 3 x 2 = 6 */
#define SIN_F_SPL 		((SIN_F_ISR)/SIN_F_ISR_DVSR)

#define SIN_F_MAX 		200u	/**< Maximum frequency setting (hertz). */
#define SIN_F_REQ 		50.0	/**< Required frequency setting (hertz). */

#define SIN_OFST		0		/**< Offset setting [-0.5, +0.5], IQ15. */
#define SIN_PHSE		0		/**< Initial phase setting [0, 360), IQ16. */
#define SIN_DFLT_GAIN	0.0		/**< Initial gain setting [0.0, 1.0]. */
#define SIN_DFLT_RCTFY	TRUE	/**< Initial rectification setting [TRUE | FALSE). */

#define SIN_CHANNEL	AC_STAGE 	/**< Defines which channel enable controls the generator output. */

extern volatile int32 *SGENTI_1ch_VOut;	/**< Voltage output terminal. */


#ifdef LOG_SIN
	#define LOG_SIZE 200		/* Arrays for logging data during debug. */
	extern volatile int16_t sine_abs[LOG_SIZE];
	extern volatile int16_t sine_sign[LOG_SIZE];
#endif

#ifdef DEBUG
	extern SGENTI_1 sigGen;		/* Makes structure available to watch during debug. */
#endif

/** Sets the initial generator values and disables the output.
 * This function MUST be called before any other signal
 * generator function.
 * @param[in]	enablePhaseOut	Determines if the AC phase signal is enabled
 * 								as an output or disabled and switched to an input.
 */
extern void initSine (uint16_t enablePhaseOut);

/** Updates the gain value to create a slow-start ramp.
 * This should be called at the same time and similarly to
 * the DC slew update.
 */
extern void updateSineGain (void);

/** Generates the next signal data point and loads it onto the VOut terminal.
 * If the point is positive the sign terminal is set, otherwise it is cleared.
 * If rectify is enabled, the VOut value produced will be an absolute value.
 * This is called by the DP_LIB asm ISR.
 */
extern void updateSineSignal (void);

/** Enables the use of the AC phase output signal on GPIO12. */
extern void enableSinePhaseOut (void);

/** Disables the use of the AC phase output signal on GPIO12. */
extern void disableSinePhaseOut (void);

/** Enables or disables the output of the generator onto the connected net
 * @param[in]	state	Output enable state {1:ON | 0:OFF}.
 * @return				Error status.
 */
extern uint16_t setSineState (uint16_t state);

/** Queries the current state of the generator output.
 * @param[out]	state	Pointer to location at which to place the query result  {1:ON | 0:OFF}.
 * @return				Error status.
 */
extern uint16_t getSineState (uint16_t *state);

/** Sets the target RMS voltage of the signal.
 * @param[in] 	rmsTarget	RMS voltage target value.
 * @return					Error status.
 */
extern uint16_t setSineRmsTarget (float rmsTarget);

/** Sets the target gain of the signal.
 * @param[in]	target	Gain target value [0.0, 1.0).
 * @return				Error status.
 */
extern uint16_t setSineGainTarget (float target);

/** Queries the current target gain setting.
 * @param[out] 	target	Pointer to location at which to place the query result.
 * @return				Error status.
 */
extern uint16_t getSineGainTarget (float *target);

/** Sets the gain step of the sine signal.
 * @param[in]	step	Specifies the value of the gain step to be applied.
 * @return				Error status.
 */
extern uint16_t setSineGainStep (float step);

/** Queries the current gain step setting.
 * @param[out]	step	Pointer to location at which to place the query result.
 * @return				Error status.
 */
extern uint16_t getSineGainStep (float *step);

/* Enables or disables the rectification of the generator output
 * @param[in]	rfy	Rectification enable state {1:ON | 0:OFF}.
 * @return			Error status.
 */
//extern Uint16 sgSetRectify (Uint16 rfy);

/* Sets the signal DC offset
 * @param[in]	ofst	DC offset value [-0.5, +0.5].
 * @return				Error status.
 */
//extern Uint16 sgSetOffset (float32 ofst);

/* Sets the signal initial phase value
 * @param[in]	phs	Initial phase value [0, 360) (degrees).
 * @return			Error status
 */
//extern Uint16 sgSetInitialPhase (float32 phs);

/* Sets the signal frequency.
 * @param[in]	frq	Frequency value [0, @f$ f_{max}@f$] (hertz).
 * @return			Error status.
 */
//extern Uint16 sgSetFreq (Uint16 frq);

/* Sets the signal generator maximum frequency setting value, @f$ f_{max}@f$ .
 * @param[in]	frq	Frequency value [0, @f$ f_{sample}@f$] (hertz).
 * @return			Error status.
 */
//extern Uint16 sgSetFMax (Uint16 frq);

/*Sets the signal generator step max setting value.
 * @param[in]	sMx	Step_max value [0, 32767).
 * @return			Error status.
 */
//extern Uint16 sgSetStepMax (Uint16 sMx);

/* Queries the current state of the signal generator rectification enable.
 * @param[out]	rfyDest	Address of the memory location at which to place the query result  {1:ON | 0:OF}.
 * @return				Error status.
 */
//extern Uint16 sgGetRectify (Uint16 *rfyDest);

/* Queries the current signal DC offset setting.
 * @param[out]	oftDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
//extern Uint16 sgGetOffset (float32 *oftDest);

/* Queries the current frequency setting.
 * @param[out]	frqDest	Address of the memory location at which to place the query result (hertz).
 * @return				Error status.
 */
//extern Uint16 sgGetFreq (Uint16 *frqDest);

/* Queries the current maximum frequecny setting.
 * @param[out]	frqDest	Address of the memory location at which to place the query result (hertz).
 * @return				Error ststus.
 */
//extern Uint16 sgGetFMax (Uint16 *frqDest);

/* Queries the current step_max setting.
 * @param[out]	sMxDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
//extern Uint16 sgGetStepMax (Uint16 *sMxDest);

/* Queries the current frequency resolution.
 * This is equal to @f$ f_{max}@f$ / step_max.
 * @param[out]	rslDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
//extern Uint16 sgGetResolution (float32 *rslDest);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SINEGEN_H_ */
