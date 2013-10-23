/*
 * SineGen.c
 *
 *  Created on: 2 Apr 2013
 *      Author: Toby
 */

#include "Common.h"


volatile int32 *SGENTI_1ch_VOut = 0;	/* Voltage signal output terminal*/

#ifdef DEBUG
	SGENTI_1 sigGen = SGENTI_1_DEFAULTS;	// TODO: can be private when testing finished
	#pragma DATA_SECTION (sigGen, "SGENTI_1ch_Struct")
#else
	static SGENTI_1 sigGen = SGENTI_1_DEFAULTS;
	#pragma DATA_SECTION (sigGen, "SGENTI_1ch_Struct")
#endif

#ifdef LOG_SIN
	volatile int16 sine_abs[LOG_SIZE] = {0};
	volatile int16 sine_sign[LOG_SIZE] = {0};
#endif

static Uint16 rectifyMode = SIN_DFLT_RCTFY;	/* Selects whether signal is recitified or not */
static Uint16 fMax = SIN_DFLT_F_MAX;

void initSine (void) {
	/* Set signal generator default values
	 * These values can be altered by changing
	 *  the values #define'd in SineGen.h
	 */
	fMax = SIN_DFLT_F_MAX;
	rectifyMode = SIN_DFLT_RCTFY;

	sigGen.offset = SIN_DFLT_OFST;		/* DC offset, Uint16 Q15 */
	sigGen.alpha = SIN_DFLT_PHSE; 		/* Alpha = [phase / (2 x pi)] x 2^16, Uint16, Q16 */
								//(Uint16) (((SIN_DFLT_PHSE / 360.0) * 65536) + 0.5);

	sigGen.gain = _IQ15(SIN_DFLT_GAIN);	/* Gain, 0x7fff is full gain of 1, int16 Q15 */

	sigGen.step_max = (Uint16)(((fMax * 65536.0) / SIN_F_SPL) + 0.5);
										/* Step_max = (f_max x 2^16) / f_sampling
										 *  		= (1kHz x 65,536) / 16.5kHz = 3971.9
										 * Frequency resolution = f_max/step_max,
										 * hence step_max should be above 100 for good resolution
										 * Uint16 Q0
										 */
	sigGen.freq = (Uint16)((SIN_DFLT_F / SIN_DFLT_F_MAX) * 32768) + 0.5;
										/* Freq = (f_req / f_max) x 2^15
										 *  	= (50Hz / 1kHz) x 32768 = 1638
										 * Uint16 Q15
										 */
	acSettings.enable = 0; 				/* Ensure sine channel output is 0 until enabled */
}

void updateSineGain (void) {
	int32 targetPoint = 0, error = 0, ref = 0;

	if (!acSettings.enable) {
		targetPoint = 0;					/* Set target to 0 if channel disabled */
	} else {								/*  else use the currently set target */
		targetPoint = acSettings.gainTarget;
	}

	ref = _IQ15toIQ(sigGen.gain);

	error = (ref - targetPoint);			/* Calculate error = ref - targetPoint */
	if (error > acSettings.gainRate)		/* If error greater than a positive step, reduce the ref by one step */
		ref = ref - acSettings.gainRate;
	else if (error < (-acSettings.gainRate))/* Else if error greater than a negative step, increase the ref by one step */
		ref = ref + acSettings.gainRate;
	else									/* Else error is less than or equal to step so set ref = targetPoint */
		ref = targetPoint;

	sigGen.gain = (Uint16)(ref >> 9);		/* Convert to unsigned Q15 */
}

void updateSineSignal (void) {
	/* Generates the next sine wave point using the settings in the sigGen structure */
	#ifdef LOG_SIN
		static Uint16 i = 0;
		static Uint16 j = 0;

		if (i >= LOG_SIZE)		/* If i (which is static) has reached the log limit, go back to the start of the log arrays */
			i = 0;
	#endif

	if(!acSettings.enable) {	/* If channel disabled output all zeroes */
		*SGENTI_1ch_VOut = 0;
		return;
	}
	sigGen.calc(&sigGen);		/* Call the sine lib function, passing the settings struct */

	if (slaveModeStatus != slaveUnit) {
		if (sigGen.out < 0) {	/* Switch GPIO12 to indicate the phase */
			GpioDataRegs.GPASET.bit.GPIO12 = 1;
		} else {
			GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
		}
	}

	if (rectifyMode) {			/* Load the sine gen result to the net connected to the VOut module terminal */
		*SGENTI_1ch_VOut = _IQ15toIQ(sigGen.out * ((sigGen.out > 0) - (sigGen.out < 0)));
	} else {
		*SGENTI_1ch_VOut = _IQ15toIQ(sigGen.out);
	}

	#ifdef LOG_SIN
		sine_sign[i] = (sigGen.out > 0);/* Save the sign value to the sign log array */
		sine_abs[i] = (int16)(*SGENTI_1ch_VOut >> 9);
		if (j == 0) { 					/* Only save every other sample */
			i++;
			j++;
		} else {
			j = 0;
		}
	#endif
}

Uint16 setSineState (Uint16 state) {
	acSettings.enable = (state > 0);
	return 0;
}

Uint16 getSineState (Uint16 *state) {
	*state = (acSettings.enable > 0);
	return 0;
}

Uint16 setSineGainTarget (float32 target) {
	/* Sets the gain
	 * 0.0 - 1.0
	 */
	float32 uprLmt = 0.0;
	uprLmt = _IQ14toF((int32)acSettings.vGainLmt);
	if ((target <= 0) || (target > uprLmt))	/* Check gain is between 0-1 */
		return VALUE_OOB;
	target = target * 16777216;				/* Convert to Q24 and round */
	acSettings.gainTarget = (int32) target;	/* Cast and set */
	return 0;
}

Uint16 getSineGainTarget (float32 *target) {
	*target = _IQ24toF(acSettings.gainTarget);
	return 0;
}


//Uint16 sgSetFreq (Uint16 frq) {
//	/* Sets the frequency of the SIN signal
//	 * 0 - fMax (max. 7FFF)
//	 */
//	float32 temp = 0.0;
//	if (frq > fMax)		/* Check frequency is less than maximum frequency */
//		return VALUE_OOB;
//	temp = (float32)frq / fMax;
//	sigGen.freq = (Uint16)((temp * 32768.0) + 0.5);
//	return 0;
//}
//
//Uint16 setRectify (Uint16 rfy) {
//	rectifyMode = (rfy > 0);
//	return 0;
//}
//
//Uint16 sgSetOffset (float32 ofst) {
//	/* DC offset in the sine
//	 * -0.5 - +0.5 dec
//	 */
//	if ((ofst < -0.5) || (ofst > 0.5))
//		return VALUE_OOB;
//	sigGen.gain = _SQ15(ofst);
//	return 0;
//}
//
//Uint16 sgSetInitialPhase (float32 phs) {
//	/* Sets the initial phase
//	 * 0 - 2pi (360deg)
//	 */
//	if ((phs < 0) || (phs > 360))	/* Check angle is within 0-360 degrees */
//		return VALUE_OOB;
//	if (phs == 0) {					/* Take a shortcut if phase is 0 */
//		sigGen.alpha = 0;
//		return 0;
//	}
//	phs = ((phs / 360.0) * 65536) + 0.5;	/* Normalise, convert to Q16 and round */
//	sigGen.alpha = (Uint16)phs;		/* Cast and set */
//	return 0;
//}
//
//Uint16 sgSetResolution (float32 rsl) {
//	Uint16 currentFreq;
//	float32 newFMax;
//
//	if (rsl <= 0)
//		return VALUE_OOB;
//
//	newFMax = rsl * sigGen.step_max;
//	sgGetFreq(&currentFreq);
//
//	if ((newFMax > SIN_F_SPL) || (currentFreq < newFMax))
//		return VALUE_OOB;
//
//	fMax = newFMax;
//	sgSetFreq(currentFreq);
//
//	return 0;
//}
//
//Uint16 sgSetFMax (Uint16 frq) {
//	/* Sets fMax (instead of using sgSetStepMax())
//	 * also affects f, step max and resolution
//	 * 0 - Sampling frequency (SIN_F_SPL)
//	 */
//	Uint16 fOld = 0, err = 0;
//	(sgGetFreq(&fOld));		/* Calculate the current frequency value using the current fMax */
//
//	if ((frq <= 0) || (frq > SIN_F_SPL) || (frq < fOld))
//		return VALUE_OOB;	/* Check the new fMax value is within allowable ranges */
//
//	fMax = frq;				/* Update fMax with new value */
//	err = sgSetFreq(fOld);	/* Update freq with new fMax */
//	if (err != 0)
//		return VALUE_OOB;
//							/* Update step_max with new fMax */
//	sigGen.step_max = (Uint16)(((fMax * 65536.0) / SIN_F_SPL) + 0.5);
//	return 0;
//}
//
//Uint16 sgSetStepMax (Uint16 sMx) {
//	/* Sets the step_max
//	 * also affects fMax and resolution
//	 * 0 - 7FFF
//	 */
//	float32 fmNew = 0, freqInHz = 0;
//	Uint16 err = 0;
//	fmNew = sMx * (SIN_F_SPL / 65536.0);	/* Calculate the new f_max with the new step_max*/
//	freqInHz = _IQ15toF((int32)sigGen.freq) * fMax;/* Calculate the current f_req setting */
//
//	if (freqInHz > fmNew)					/* f_req should be less than the new f_max */
//		return VALUE_OOB;
//
//	fMax = (Uint16) (fmNew + 0.5);			/* Update fMax */
//	err = sgSetFreq (freqInHz);				/* Update freq with the new fMax */
//	if (err != 0)
//		return VALUE_OOB;
//											/* Update step_max */
//	sigGen.step_max = sMx;
//	return 0;
//}
//
//Uint16 sgGetRectify (Uint16 *rfyDest) {
//	*rfyDest = rectifyMode > 0;
//	return 0;
//}
//
//Uint16 sgGetOffset (float32 *oftDest) {
//	*oftDest = _IQ15toF(sigGen.gain);
//	return 0;
//}
//
//Uint16 sgGetFreq (Uint16 *frqDest) {
//	*frqDest = (Uint16)((sigGen.freq * (1.0 / 32768) * fMax) + 0.5);
//	return 0;
//}
//
//Uint16 sgGetFMax (Uint16 *frqDest) {
//	*frqDest = fMax;
//	return 0;
//}
//
//Uint16 sgGetStepMax (Uint16 *sMxDest) {
//	*sMxDest = sigGen.step_max;
//	return 0;
//}
//
//Uint16 sgGetResolution (float32 *rslDest) {
//	if (sigGen.step_max == 0)
//		return VALUE_OOB;
//	*rslDest = ( fMax / ((float32) sigGen.step_max));
//	return 0;
//}
