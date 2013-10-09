/*
 * SlewControl.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

void scLoadSlewUpdate (void) {
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

Uint16 scSetLoadSlewTarget (loadStage load, float32 trgt) {
	/* Changes an enabled channel's target value
	 * scTarget is expected in amps or volts
	 */
	float32 gain = 1.0, max = 0.0;

	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid */

	max = _IQ14toF((int32) loadSettings[load].iScale);
//		max = _IQ14toF((int32)channel[load].vScale);

	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the target maximum with the current scaling */
	if (trgt > max)							/* Check target is valid */
		return VALUE_OOB;

	// TODO remove GAIN from calc and ensure is present in other uses...
											/* Apply gain, normalise and save as Q value */
	loadSettings[load].slewTarget =  _IQ24(trgt * gain * (1.0 / max));
	return 0;
}

Uint16 scSetLoadSlewStep (loadStage load, float32 stp) {
	/* Changes a load's slew step value
	 * scStep is expected in amps //or volts
	 */
	float32 max = 0.0;
	int32 qStp = 0;

	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid */

	max = _IQ14toF((int32) loadSettings[load].iScale);
//		max = _IQ14toF((int32)channel[load].vScale);

	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the step maximum with the current scaling */
	if (stp > max)							/* Check step smaller than scale maximum */
		return VALUE_OOB;

	qStp = _IQ24(stp * (1.0 / max));			/* Normalise and convert to Q */
	if (qStp > loadSettings[load].slewTarget)	/* Check step is smaller than target */
		return VALUE_OOB;
	loadSettings[load].slewRate = qStp;		/* Save to load settings */
	return 0;
}

Uint16 scSetLoadState (loadStage load, Uint16 stt) {
	/* onOrOff should be zero (OFF) or non-zero (ON) */
	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid */
	loadSettings[load].enable = (stt > 0);
	return 0;
}

Uint16 scSetLoadSlewTargetAll (float32 trgt) {
	/* Set all enabled channels to the same target value given by scTarget
	 * scTarget expected in amps or volts
	 */
	Uint16 i = 0, err = 0;
	for (i = 0; i < numberOfLoads; i++) {
		err = scSetLoadSlewTarget((loadStage)i, trgt);
		if (err != 0)
			return err;
	}
	return 0;
}

Uint16 scSetLoadSlewStepAll (float32 stp) {
	/* Set all channels to the same step value given by slewStep
	 * scStep is expected in amps or volts
	 */
	Uint16 i = 0, err = 0;
	for (i = 0; i < numberOfLoads; i++) {
		err = scSetLoadSlewStep((loadStage)i, stp);
		if (err != 0)
			return err;
	}
	return 0;
}

Uint16 scSetLoadStateAll (Uint16 stt) {
	/* onOrOff should be zero (OFF) or non-zero (ON) */
	Uint16 i = 0;
	stt = (stt > 0);
	for (i = 0; i < numberOfLoads; i++) {
		loadSettings[i].enable = stt;
	}
	return 0;
}

Uint16 scGetLoadSlewTarget (loadStage load, float32 * trgtDest) {
	float32 gain = 1.0, max = 0.0;

	if (load >= numberOfLoads)
		return CHANNEL_OOB;					/* Check channel is valid. -1 ad the last is sin slew and handled by sgGetGainTarget() */

	max = _IQ14toF((int32) loadSettings[load].iScale);
		//max = _IQ14toF((int32)channel[chnl].vScale);

	// TODO remove GAIN from calc and ensure is present in other uses...
											/* For current controlled channels this is 1 by default anyway (and should never be changed) */
	//gain = _IQ14toF((int32) loadSettings[load].vGainLmt);

	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the target maximum with the current scaling */

											/* Convert from IQ24, de-gain and de-normalise */
	*trgtDest = (_IQ24toF(loadSettings[load].slewTarget)) * (1.0 / gain) * max;
	return 0;
}

Uint16 scGetLoadSlewStep (loadStage load, float32 * stpDest) {
	float32 max = 0.0;

	if (chnl > numberOfLoads)
		return CHANNEL_OOB;						/* Check channel is valid */

	if (channel[chnl].ctlMode == iCtrl) { 	/* Select which scaling to use, V or I */
		max = _IQ14toF((int32)channel[chnl].iScale);
	} else {
		max = _IQ14toF((int32)channel[chnl].vScale);
	}

	max = ((VDDA - VSSA) / 1000.0) / max;	/* Calculate the step maximum with the current scaling */
											/* Convert from IQ24 and de-normalise */
	*stpDest = (_IQ24toF(channel[chnl].slewRate)) * max;
	return 0;
}

Uint16 scGetLoadState (loadStage load, Uint16 * sttDest) {
	if (chnl >= NUM_CHNLS)		/* Check channel is valid */
		return CHANNEL_OOB;
	*sttDest = (channel[chnl].chEnable > 0);
	return 0;
}
