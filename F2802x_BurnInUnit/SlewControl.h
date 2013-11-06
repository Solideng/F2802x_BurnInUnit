/**
 * @file SlewControl.h
 *
 * @brief Slew control functions.
 *
 */

#ifndef SLEWCONTROL_H_
#define SLEWCONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============= GLOBAL FUNCTIONS ==============*/
/** Advances the slew ramps for all loads. */
extern void updateLoadSlew (void);

/** Sets the slew target for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	target	Specifies the value of the slew target to be applied (amps).
 * @return				Error status.
 */
extern uint16_t setLoadSlewTarget (loadStage load, float target);

/** Sets the slew step size for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	step	Specifies the value of the slew step size to be applied (amps).
 * @return				Error status.
 */
extern uint16_t setLoadSlewStep (loadStage load, float step);

/** Sets the reference net enable state for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	state	Specifies the reference net state to be applied (0:OFF | non-zero:ON).
 * @return				Error Status.
 */
extern uint16_t setLoadState (loadStage load, uint16_t state);

//** Sets all channels' slew target
// * @param[in]	trgt	Specifies the value of the slew target to be applied (amps or volts).
// * @return				Error status.
// */
//extern Uint16 setLoadSlewTargetAll (float32 trgt);
//
//** Sets all channels' slew step size.
// * @param[in]	stp		Specifies the value of the slew step size to be applied (amps or volts).
// * @return				Error status.
// */
//extern Uint16 setLoadSlewStepAll (float32 stp);

/** Sets all channels' reference net enable state.
 * @param[in]	state	Specifies the refernce net state to be applied (0:OFF | non-zero:ON).
 * @return				Error status.
 */
extern uint16_t scSetLoadStateAll (uint16_t state);

/** Queries the current slew target setting for the specified channel.
 * @param[in]	load		Specifies the load the setting is to be read from.
 * @param[out]	target		Pointer to location at which to place the query result (amps).
 * @return					Error status.
 */
extern uint16_t getLoadSlewTarget (loadStage load, float * target);

/** Queries the current slew step size of the specified channel.
 * @param[in]	load	Specifies the load the setting is to be read from.
 * @param[out]	step	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern uint16_t getLoadSlewStep (loadStage load, float * step);

/** Queries the current reference net enable state for the specified channel.
 * @param[in]	load	Specifies the load the setting is to be read from.
 * @param[out]	state	Pointer to location at which to place the query result (0:OFF | non-zero:ON).
 * @return				Error status.
 */
extern uint16_t getLoadState (loadStage load, uint16_t * state);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SLEWCONTROL_H_ */
