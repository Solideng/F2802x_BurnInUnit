/*
 * Opp.c
 *
 *  Created on: 17 Oct 2013
 *      Author: Toby
 */
#include "Common.h"

Uint16 oppFlagRegister = 0;

/*============== Load n ==============*/
Uint16 checkLoadOpp (loadStage load) {
	/* Load over-power protection
	 *  - vScale AND iScale SHOULD BE SET BEFORE USE -
	 */
	Uint16 err = 0;
	float32 iLimDyn = 0, iMeas = 0, vMeas = 0;

	if (load >= numberOfLoads)
		return CHANNEL_OOB;
								/* Get current and voltage readings */
	err = getLoadCurrent(load, &iMeas);
	if (err)					/* Check reading completed OK*/
		 return err;
	err = getLoadVoltage(load, &vMeas);
	if (err)					/* Check reading completed OK*/
		return err;
								/* Divide real voltage value into the power limit value to get the dynamic
								 * current limit.
								 */
	iLimDyn = LOAD_PWRLVL_FIX / vMeas;

	if (iMeas > iLimDyn) {		/* Check the measured current value is below the dynamic current limit value. */
		stopAll();
		oppFlagRegister |= (1 << load);
		return OPP_TRIP;
	}
	return 0;
}

Uint16 getLoadOppState (loadStage load) {
	return (load < numberOfLoads) ? ((oppFlagRegister & (1 << load)) > 0) : CHANNEL_OOB;
}

Uint16 clearLoadOpp (loadStage load) {
	oppFlagRegister &= (~(1 << load));
	if (oppFlagRegister)
		return OPP_TRIP;
	runAll();
	return 0;
}

/*================ AC ================*/
Uint16 checkAcOpp (void) {
	/* AC stage over-power protection
	 *  - vScale AND iScale SHOULD BE SET BEFORE USE -
	 */
	int32 iMeas = 0, vMeas = 0, pMeas = 0;
	float32 pMeasF;
	// TODO: THESE ARE WRONG! THEY HAVE NOT BEEN UN-NORMALISED!
	vMeas = _IQmpy(acNets.vFdbkNet, _Q14toIQ(acSettings.vScale));	/* Scale voltage reading (IQ24) */
	iMeas = _IQmpy(acNets.iFdbkNet, _Q14toIQ(acSettings.iScale));	/* Scale current reading (IQ24) */
	pMeas = _IQmpy(vMeas, iMeas);	/* Get power value from measured current and voltage (IQ24) */
	pMeasF = _IQ24toF(pMeas);		/* Convert power value from IQ24 to float */

	if (pMeasF > AC_PWRLVL_FIX) {	/* Check the measured power is below the fixed power maximum */
		stopAll();
		oppFlagRegister |= AC_OPP_TRIP;
		return OPP_TRIP;
	}
	return 0;
}

Uint16 getAcOppState (void) {
	return ((oppFlagRegister & AC_OPP_TRIP) > 0);
}

Uint16 clearAcOpp (void) {
	oppFlagRegister &= (~AC_OPP_TRIP);	/* Clear flag */
	if (oppFlagRegister)				/* Check if there are any other OPP flags still raised */
		return OPP_TRIP;
	runAll();							/* Run macros */
	return 0;
}
