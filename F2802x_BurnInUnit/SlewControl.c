/*
 * SlewControl.c
 *
 *  Created on: 19 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

void scSlewUpdate (void) {
	/* This code ensures that each channel ramps at the
	 *  given slew rate until it reaches the given target
	 * Derivation shown in attached Excel Spreadsheet
	 */
	int32 targetPoint = 0, error = 0;
	Uint16 i = 0;

	for (i = 0; i < (NUM_CHNLS - 1); i++) {		/* -1 as last is sin slew and handled by sgGainUpdate() */
		if (i != AC_I_CNTL) {
			if (!channel[i].chEnable)			/* If the channel is disabled set the targetPoint to zero */
				targetPoint = 0;
			else
				targetPoint = channel[i].target;/* If the channel is enabled set the targetPoint to the target */

			error = (channel[i].refNet - targetPoint);	/* Calculate error = ref - targetPoint */
			if (error > channel[i].slewRate)			/* If error greater than a positive step, reduce the ref by one step */
				channel[i].refNet = channel[i].refNet - channel[i].slewRate;
			else if (error < (-channel[i].slewRate))	/* Else if error greater than a negative step, increase the ref by one step */
				channel[i].refNet = channel[i].refNet + channel[i].slewRate;
			else										/* Else error is less than or equal to step so set ref = targetPoint */
				channel[i].refNet = targetPoint;
		}
	}
}

Uint16 scSetTarget (Uint16 chnl, float32 trgt) {
	/* Changes an enabled channel's target value
	 * scTarget is expected in amps or volts
	 */
	float32 scale = 0.0, gain = 1.0, max = 0.0;

	if ((chnl > (NUM_CHNLS - 1)) && (chnl != AC_I_CNTL))
		return CHANNEL_OOB;					/* Check channel is valid, -1 as the last is sin slew and handled by sgSetGainTarget() */

	if (channel[chnl].ctlMode == iCtrl) { 	/* Select which scaling to use, V or I */
		scale = _IQ14toF((int32)channel[chnl].iScale);
	} else {
		scale = _IQ14toF((int32)channel[chnl].vScale);
	}

	max = ((VDDA - VSSA) / 1000.0) / scale;	/* Calculate the target maximum with the current scaling */
	if (trgt > max)							/* Check target is valid */
		return VALUE_OOB;
											/* For current controlled channels this is 1 by default anyway (and should never be changed) */
	gain = _IQ14toF((int32)channel[chnl].vGainLmt);
											/* Apply gain, normalise and save as Q value */
	channel[chnl].target =  _IQ24(trgt * gain * (1.0 / max));
	return 0;
}

Uint16 scSetStep (Uint16 chnl, float32 stp) {
	/* Changes a channels slew step value
	 * scStep is expected in amps or volts
	 */
	float32 scale = 0.0, max = 0.0;
	int32 qStp = 0;

	if ((chnl > NUM_CHNLS) && (chnl != AC_I_CNTL))
		return CHANNEL_OOB;					/* Check channel is valid */

	if (channel[chnl].ctlMode == iCtrl) { 	/* Select which scaling to use, V or I*/
		scale = _IQ14toF((int32)channel[chnl].iScale);
	} else {
		scale = _IQ14toF((int32)channel[chnl].vScale);
	}

	max = ((VDDA - VSSA) / 1000.0) / scale;	/* Calculate the step maximum with the current scaling */
	if (stp > max)							/* Check step smaller than scale maximum */
		return VALUE_OOB;

	qStp = _IQ24(stp * (1.0 / max));		/* Normalise and convert to Q */
	if (qStp > channel[chnl].target)		/* Check step is smaller than target */
		return VALUE_OOB;
	channel[chnl].slewRate = qStp;			/* Save to struct member */
	return 0;
}

Uint16 scSetState (Uint16 chnl, Uint16 stt) {
	/* onOrOff should be zero (OFF) or non-zero (ON) */
	if ((chnl >= NUM_CHNLS) && (chnl != AC_I_CNTL))
		return CHANNEL_OOB;					/* Check channel is valid */
	channel[chnl].chEnable = (stt > 0);
	return 0;
}

Uint16 scSetTargetAll (float32 trgt) {
	/* Set all enabled channels to the same target value given by scTarget
	 * scTarget expected in amps or volts
	 */
	Uint16 i = 0, err = 0;
	for (i = 0; i < (NUM_CHNLS - 1); i++) {
		err = scSetTarget(i, trgt);
		if (err != 0)
			return err;
	}
	return 0;
}

Uint16 scSetStepAll (float32 stp) {
	/* Set all channels to the same step value given by slewStep
	 * scStep is expected in amps or volts
	 */
	Uint16 i = 0, err = 0;
	for (i = 0; i < (NUM_CHNLS - 1); i++) {
		err = scSetStep(i, stp);
		if (err != 0)
			return err;
	}
	return 0;
}

Uint16 scSetStateAll (Uint16 stt) {
	/* onOrOff should be zero (OFF) or non-zero (ON) */
	Uint16 i = 0;
	stt = (stt > 0);
	for (i = 0; i < NUM_CHNLS; i++) {
		channel[i].chEnable = stt;
	}
	return 0;
}

Uint16 scGetTarget (Uint16 chnl, float32 * trgtDest) {
	float32 gain = 1.0, max = 0.0, scale = 0.0;

	if ((chnl > (NUM_CHNLS - 1)) && (chnl != AC_I_CNTL))
		return CHANNEL_OOB;					/* Check channel is valid. -1 ad the last is sin slew and handled by sgGetGainTarget() */

	if (channel[chnl].ctlMode == iCtrl) { 	/* Select which scaling to use, V or I */
		scale = _IQ14toF((int32)channel[chnl].iScale);
	} else {
		scale = _IQ14toF((int32)channel[chnl].vScale);
	}
											/* For current controlled channels this is 1 by default anyway (and should never be changed) */
	gain = _IQ14toF((int32)channel[chnl].vGainLmt);

	max = ((VDDA - VSSA) / 1000.0) / scale;	/* Calculate the target maximum with the current scaling */
											/* Convert from IQ24, de-gain and de-normalise */
	*trgtDest = (_IQ24toF(channel[chnl].target)) * (1.0 / gain) * max;
	return 0;
}

Uint16 scGetStep (Uint16 chnl, float32 * stpDest) {
	float32 scale = 0.0, max = 0.0;

	if ((chnl > NUM_CHNLS) && (chnl != AC_I_CNTL))
		return CHANNEL_OOB;						/* Check channel is valid */

	if (channel[chnl].ctlMode == iCtrl) { 	/* Select which scaling to use, V or I */
		scale = _IQ14toF((int32)channel[chnl].iScale);
	} else {
		scale = _IQ14toF((int32)channel[chnl].vScale);
	}

	max = ((VDDA - VSSA) / 1000.0) / scale;	/* Calculate the step maximum with the current scaling */
											/* Convert from IQ24 and de-normalise */
	*stpDest = (_IQ24toF(channel[chnl].slewRate)) * max;
	return 0;
}

Uint16 scGetState (Uint16 chnl, Uint16 * sttDest) {
	if (chnl >= NUM_CHNLS)		/* Check channel is valid */
		return CHANNEL_OOB;
	*sttDest = (channel[chnl].chEnable > 0);
	return 0;
}
