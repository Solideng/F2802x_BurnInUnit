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
	int32 vMeas = 0, iMeas = 0;
	float32 iLimDyn = 0, iMeasF = 0;

	if (load >= numberOfLoads)
		return CHANNEL_OOB;
								/* Get the voltage reading for the given load and multiply by
								 * the related vScale to get the actual value (IQ24).
								 */
	vMeas = _IQmpy(loadNets[load].vFdbkNet, _Q14toIQ(loadSettings[load].vScale));
	iLimDyn = _IQ24toF(vMeas); 	/* Convert real voltage value from IQ24 */
								/* Divide real voltage value into the power limit value to get the dynamic
								 * current limit.
								 */
	iLimDyn = LOAD_PWRLVL_FIX / iLimDyn;
								/* Get the most recent current reading for the given load and multiply by the
								 * related current scale to get the real value (IQ24).
								 */
	iMeas = _IQmpy(loadNets[load].iFdbkNet, _Q14toIQ(loadSettings[load].iScale));
	iMeasF = _IQ24toF(iMeas);

	if (iMeasF > iLimDyn) {		/* Check the measured current value is below the dynamic current limit value. */
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
