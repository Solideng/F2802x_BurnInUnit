/**
 * @file SlewControl.h
 *
 * @brief Slew control functions.
 *
 */

#ifndef SLEWCONTROL_H_
#define SLEWCONTROL_H_

/*============= GLOBAL FUNCTIONS ==============*/
/** Advances the slew ramps for all loads. */
extern void scLoadSlewUpdate (void);

/** Sets the slew target for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	trgt	Specifies the value of the slew target to be applied (amps or volts).
 * @return				Error status.
 */
extern Uint16 scSetLoadSlewTarget (loadStage load, float32 trgt);

/** Sets the slew step size for the specified load.
 * @param[in]	chnl	Specifies the channel the setting is to be applied to.
 * @param[in]	stp		Specifies the value of the slew step size to be applied (amps or volts).
 * @return				Error status.
 */
extern Uint16 scSetLoadSlewStep (loadStage load, float32 stp);

/** Sets the reference net enable state for the specified load.
 * @param[in]	chnl	Specifies the channel the setting is to be applied to.
 * @param[in]	stt		Specifies the reference net state to be applied (0:OFF | non-zero:ON).
 * @return				Error Status.
 */
extern Uint16 scSetLoadState (loadStage load, Uint16 stt);

/** Sets all channels' slew target
 * @param[in]	trgt	Specifies the value of the slew target to be applied (amps or volts).
 * @return				Error status.
 */
extern Uint16 scSetLoadSlewTargetAll (float32 trgt);

/** Sets all channels' slew step size.
 * @param[in]	stp		Specifies the value of the slew step size to be applied (amps or volts).
 * @return				Error status.
 */
extern Uint16 scSetLoadSlewStepAll (float32 stp);

/** Sets all channels' reference net enable state.
 * @param[in]	stt		Specifies the refernce net state to be applied (0:OFF | non-zero:ON).
 * @return				Error status.
 */
extern Uint16 scSetLoadStateAll (Uint16 stt);

/** Queries the current slew target setting for the specified channel.
 * @param[in]	chnl		Specifies the channel the setting is to be read from.
 * @param[out]	trgtDest	Address of the memory location at which to place the query result (amps or volts).
 * @return					Error status.
 */
extern Uint16 scGetLoadSlewTarget (loadStage load, float32 * trgtDest);

/** Queries the current slew step size of the specified channel.
 * @param[in]	chnl	Specifies the channel the setting is to be read from.
 * @param[out]	stpDest	Address of the memory location at which to place the query result (amps or volts).
 * @return				Error status.
 */
extern Uint16 scGetLoadSlewStep (loadStage load, float32 * stpDest);

/** Queries the current reference net enable state for the specified channel.
 * @param[in]	chnl	Specifies the channel the setting is to be read from.
 * @param[out]	sttDest	Address of the memory location at which to place the query result (0:OFF | non-zero:ON).
 * @return				Error status.
 */
extern Uint16 scGetLoadState (loadStage load, Uint16 * sttDest);

#endif /* SLEWCONTROL_H_ */

