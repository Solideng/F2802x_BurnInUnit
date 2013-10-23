/**
 * @file Comparator.h
 *
 * @brief Comparator, DAC and trip zone functions.
 *
 * Requires the modification of the COMP_REGS struct of the
 * DSP2802x_GlobalVariableDefs in the file DSP2802x_Comp.h,
 * to allow use of the DACCTL and ramp-related register
 * unions. Use the equivalent file from the f2903x includes
 * for reference.
 *
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_

/** Configures PWM trip zones for use.
 * Requires the comparator and DAC to be configured
 * @sa adc.h
 */
extern void initTripzone (void);

/** Initialises the DC comparator (COMP 2) using an internal DAC at the inverting input.
 *  - SHOULD be called AFTER adcSocCnf().
 *  - SHOULD be called BEFORE PWMS (SYNC) are started.
 *  - SHOULD be called BEFORE pwmTZConfigure().
 *  - midVScale should be set before use.
 */
extern void initDcComparator (void);

//** Sets the output level of the DAC on the inverting input of the DC comparator.
// * The voltage and vScale MUST be set before use.
// * @param[in]	level	Specifies the value of the level setting to be applied (volts).
// * @return				Error status.
// */
//extern Uint16 setDcDac (float32 level);
//
//** Queries the output level setting of the DAC on the inverting input of the DC comparator.
// * vScale MUST be set before use.
// * @param[out]	level	Pointer to location at which to place the query result (volts).
// * @return				Error status.
// */
//extern Uint16 getDcDac (float32 *level);

/** Resets the DC trip zone after an DC comparator event. */
extern void rstDcTripzone (void);

/** Initialises the AC comparator (COMP 1) using an internal DAC at the inverting input.
 *  - SHOULD be called AFTER adcSocCnf().
 *  - SHOULD be called BEFORE PWMS (SYNC) are started.
 *  - SHOULD be called BEFORE pwmTZConfigure().
 */
extern void initAcComparator (void);

/** Sets the output level of the DAC on the inverting input of the AC comparator.
 * The voltage and iScale MUST be set before use.
 * @param[in]	level	Specifies the value of the level setting to be applied (amps).
 * @return				Error status.
 */
extern Uint16 setAcDac (float32 level);

/** Queries the output level setting of the DAC on the inverting input of the DC comparator.
 * iScale MUST be set before use.
 * @param[out]	level	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 getAcDac (float32 *level);

/** Resets the AC trip zone after an AC comparator event. */
extern void rstAcTripzone (void);

#endif /* COMPARATOR_H_ */
