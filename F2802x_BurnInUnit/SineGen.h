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

#ifndef SINEGEN_H_
#define SINEGEN_H_

/*================== MACROS ===================*/
/*======== INITIAL SIN PARAMETER VALUES =======*/
#define SIN_DFLT_RCTFY	TRUE	/**< Initial rectification setting [TRUE | FALSE). */
#define SIN_DFLT_OFST	0		/**< Initial offset setting [-0.5, +0.5], IQ15. */
#define SIN_DFLT_PHSE	0		/**< Initial initial phase setting [0, 360), IQ16. */
#define SIN_DFLT_GAIN	0.9		/**< Initial gain setting [0.0, 1.0]. */
#define SIN_DFLT_F 		50.0	/**< Initial frequency setting (hertz). */
#define SIN_DFLT_F_MAX 	1000u	/**< Initial maximum frequency setting (hertz). */

#define SIN_CHANNEL	AC_STAGE 	/**< Defines which channel enable controls the generator output. */
#define SIN_F_SPL 	8250u		/**< Signal sampling frequency, i.e. the frequency that sgen.calc()
 	 	 	 	 	 	 	 	 * is called at.
								 * This is dependent on ISR frequency, currently 1/4 of f_ISR,
								 * full ISR speed is 33,000Hz.
								 */

extern volatile int32 *SGENTI_1ch_VOut;	/**< Voltage output terminal. */
extern volatile int32 *SGENTI_1ch_Sign;	/**< Voltage sign (pre-rectification) output terminal. */
extern volatile int32 *PHASE_CTRL_In; /**< Phase control module signal input terminal. */

#ifdef LOG_SIN
	#define LOG_SIZE 100		/* Arrays for logging data during debug. */
	extern volatile int16 sine_abs[LOG_SIZE];
	extern volatile int16 sine_sign[LOG_SIZE];
#endif

#ifdef DEBUG
	extern SGENTI_1 sigGen;		/* Makes struct available to watch during debug. */
#endif

/** Updates GPIO19 based on state of *PHASE_CTRL_In terminal.
 * Expects 0 (GPIO19 set) or non-zero (GPIO19 cleared).
 * This is generally called by the DPL_ISR.asm
 */
extern void pcUpdate (void);

/** Sets the initial generator values and disables
 * the output.
 * This function MUST be called before any other signal
 * generator function.
 */
extern void sgInit (void);

/** Generates the next signal data point
 * and loads it onto the VOut terminal.
 * If the point is positive the sign terminal is set,
 * otherwise it is cleared.
 * If rectify is enabled, the value produced will be
 * an absolute value.
 */
extern void sgUpdate (void);

/** Updates the gain value to create a slow-start ramp.
 * This should be called at the same time and similarly to
 * the DC slew update.
 * @sa scSlewUpdate()
 */
extern void sgGainUpdate (void);

/** Enables or disables the output of the generator onto the connected net
 * @param[in]	stt	Output enable state {1:ON | 0:OFF}.
 * @return			Error status.
 */
extern Uint16 sgSetState (Uint16 stt);

/** Enables or disables the rectification of the generator output
 * @param[in]	rfy	Rectification enable state {1:ON | 0:OFF}.
 * @return			Error status.
 */
extern Uint16 sgSetRectify (Uint16 rfy);

/** Sets the signal DC offset
 * @param[in]	ofst	DC offset value [-0.5, +0.5].
 * @return				Error status.
 */
extern Uint16 sgSetOffset (float32 ofst);

/** Sets the signal initial phase value
 * @param[in]	phs	Initial phase value [0, 360) (degrees).
 * @return			Error status
 */
extern Uint16 sgSetInitialPhase (float32 phs);

/** Sets the target gain of the signal.
 * @param[in]	gnt	Gain target value [0.0, 1.0).
 * @return		Error status.
 */
extern Uint16 sgSetGainTarget (float32 gnt);

/** Sets the signal frequency.
 * @param[in]	frq	Frequency value [0, @f$ f_{max}@f$] (hertz).
 * @return			Error status.
 */
extern Uint16 sgSetFreq (Uint16 frq);

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

/** Queries the current state of the generator output.
 * @param[out]	sttDest	Address of the memory location at which to place the query result  {1:ON | 0:OFF}.
 * @return				Error status.
 */
extern Uint16 sgGetState (Uint16 *sttDest);

/** Queries the current state of the signal generator rectification enable.
 * @param[out]	rfyDest	Address of the memory location at which to place the query result  {1:ON | 0:OF}.
 * @return				Error status.
 */
extern Uint16 sgGetRectify (Uint16 *rfyDest);

/** Queries the current signal DC offset setting.
 * @param[out]	oftDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 sgGetOffset (float32 *oftDest);

/** Queries the current target gain setting.
 * @param[out] gntDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 sgGetGainTarget (float32 *gntDest);

/** Queries the current frequency setting.
 * @param[out]	frqDest	Address of the memory location at which to place the query result (hertz).
 * @return				Error status.
 */
extern Uint16 sgGetFreq (Uint16 *frqDest);

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

/** Queries the current frequency resolution.
 * This is equal to @f$ f_{max}@f$ / step_max.
 * @param[out]	rslDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 sgGetResolution (float32 *rslDest);

#endif /* SINEGEN_H_ */
