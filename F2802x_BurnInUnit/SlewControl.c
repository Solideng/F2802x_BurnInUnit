/*
 * SlewControl.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

void updateLoadSlew (void) {
	/* This code ensures that each load ramps at the
	 *  given slew rate until it reaches the given target
	 * Derivation shown in attached Excel Spreadsheet
	 */
	int32 targetPoint = 0, error = 0;
	Uint16 i = 0;

	for (i = 0; i < numberOfLoads; i++) {		/* Loop through each load */
		if (!loadSettings[i].enable)			/* If the channel is disabled set the targetPoint to zero */
			targetPoint = 0;
		else
			targetPoint = loadSettings[i].slewTarget;	/* If the channel is enabled set the targetPoint to the target */

		error = (loadNets[i].iRefNet - targetPoint);	/* Calculate error = ref - targetPoint */
		if (error > loadSettings[i].slewRate)			/* If error greater than a positive step, reduce the ref by one step */
			loadNets[i].iRefNet = loadNets[i].iRefNet - loadSettings[i].slewRate;
		else if (error < (-loadSettings[i].slewRate))	/* Else if error greater than a negative step, increase the ref by one step */
			loadNets[i].iRefNet = loadNets[i].iRefNet + loadSettings[i].slewRate;
		else											/* Else error is less than or equal to step so set ref = targetPoint */
			loadNets[i].iRefNet = targetPoint;
	}
}

Uint16 setLoadSlewTarget (loadStage load, float32 target) {
	/* Changes an enabled channel's target value
	 * scTarget is expected in amps or volts
	 */
	float32 max = 0.0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid */
	if (target > LOAD_IDCLVL_MAX)			/* Check target is less than the allowed maximum. */
		return CHANNEL_OOB;

	max = _IQ14toF((int32) loadSettings[load].iScale);
	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the target maximum with the current scaling */
	if (target > max)						/* Check target is less than maximum allowed by scaling */
		return VALUE_OOB;
											/* Normalise and save as Q value */
	loadSettings[load].slewTarget =  _IQ24(target * (1.0 / max));
	return 0;
}

Uint16 setLoadSlewStep (loadStage load, float32 step) {
	/* Changes a load's slew step value
	 * scStep is expected in amps //or volts
	 */
	float32 max = 0.0;
	int32 qStp = 0;

	if (load >= numberOfLoads)
		return CHANNEL_OOB;						/* Check channel is valid */

	max = _IQ14toF((int32) loadSettings[load].iScale);
	max = ((VDDA - VSSA) / 1000.0) / max;		/* Calculate the step maximum with the current scaling */
	if (step > max)								/* Check step smaller than scale maximum */
		return VALUE_OOB;

	qStp = _IQ24(step * (1.0 / max));			/* Normalise and convert to Q */
	if (qStp > loadSettings[load].slewTarget)	/* Check step is smaller than target */
		return VALUE_OOB;

	loadSettings[load].slewRate = qStp;			/* Save to load settings */
	return 0;
}

// TODO: SHOULD NOT BE IN SLEW CONTROL...
Uint16 setLoadState (loadStage load, Uint16 state) {
	/* onOrOff should be zero (OFF) or non-zero (ON) */
	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid */
	loadSettings[load].enable = (state > 0);
	return 0;
}

//Uint16 setLoadSlewTargetAll (float32 trgt) {
//	/* Set all enabled channels to the same target value given by scTarget
//	 * scTarget expected in amps or volts
//	 */
//	Uint16 i = 0, err = 0;
//	for (i = 0; i < numberOfLoads; i++) {
//		err = scSetLoadSlewTarget((loadStage)i, trgt);
//		if (err != 0)
//			return err;
//	}
//	return 0;
//}
//
//Uint16 setLoadSlewStepAll (float32 stp) {
//	/* Set all channels to the same step value given by slewStep
//	 * scStep is expected in amps or volts
//	 */
//	Uint16 i = 0, err = 0;
//	for (i = 0; i < numberOfLoads; i++) {
//		err = scSetLoadSlewStep((loadStage)i, stp);
//		if (err != 0)
//			return err;
//	}
//	return 0;
//}

// TODO: SHOULD NOT BE IN SLEW CONTROL...
Uint16 setLoadStateAll (Uint16 state) {
	/* onOrOff should be zero (OFF) or non-zero (ON) */
	Uint16 i = 0;
	state = (state > 0);
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = state;
	}
	return 0;
}

Uint16 getLoadSlewTarget (loadStage load, float32 * target) {
	float32 max = 0.0;
	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid */

	max = _IQ14toF((int32) loadSettings[load].iScale);
	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the target maximum with the current scaling */
											/* Convert from IQ24 and de-normalise */
	*target = (_IQ24toF(loadSettings[load].slewTarget)) * max;
	return 0;
}

Uint16 getLoadSlewStep (loadStage load, float32 * step) {
	float32 max = 0.0;
	if (load > numberOfLoads)
		return CHANNEL_OOB;						/* Check channel is valid */

	max = _IQ14toF((int32) loadSettings[load].iScale);
	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the step maximum with the current scaling */
											/* Convert from IQ24 and de-normalise */
	*step = (_IQ24toF(loadSettings[load].slewRate)) * max;
	return 0;
}

// TODO: SHOULD NOT BE IN SLEW CONTROL...
Uint16 getLoadState (loadStage load, Uint16 * state) {
	if (load >= numberOfLoads)		/* Check channel is valid */
		return CHANNEL_OOB;
	*state = (loadSettings[load].enable > 0);
	return 0;
}
