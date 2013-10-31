/*
 * Cntl.c
 *
 *  Created on: 28 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/* Declare closed loop feedback coefficient structure net nodes
 * The DATA_SECTION directive assigns the structures to the correct
 *  sections of dataRAM as mapped in the relevant .CMD file
 */
struct CNTL_2P2Z_CoefStruct loadICoefs [load4 + 1];
#pragma DATA_SECTION(loadICoefs, "CNTL_2P2Z_Coef")
struct CNTL_2P2Z_CoefStruct acICoefs;
#pragma DATA_SECTION(acICoefs, "CNTL_2P2Z_Coef");

#ifdef AC_V_3P3Z
	struct CNTL_3P3Z_CoefStruct acVCoefs;
	#pragma DATA_SECTION(acVCoefs, "CNTL_3P3Z_Coef")
#else
	struct CNTL_2P2Z_CoefStruct acVCoefs;
	#pragma DATA_SECTION(acVCoefs, "CNTL_2P2Z_Coef")
#endif

#ifdef AC_V_3P3Z
							/*	 min, max, B0,  B1,  A1,  B2,  A2,  B3,  A3	 */
float32 cfLimits[2][cA3 + 1] = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},	/* Coefficient minimums. */
								{1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};	/* Coefficient maximums. */
#else
							/*	 min, max, B0,  B1,  A1,  B2,  A2  */
float32 cfLimits[2][numCoefs] = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},	/* Coefficient minimums. */
								{1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0}};	/* Coefficient maximums. */
#endif

extern void initCoefs (void) {
	/* Initialises all coefficients.
	 *  - MUST be called before IIR CNTL macros are used.
	 */
	Uint16 i = 0;
	for (i = 0; i < numberOfLoads; i++) {
		loadICoefs[i].min 	= DFLT_LOAD_SMIN;
		loadICoefs[i].max 	= DFLT_LOAD_SMAX;
		loadICoefs[i].b0 	= DFLT_LOAD_B0;
		loadICoefs[i].b1 	= DFLT_LOAD_B1;
		loadICoefs[i].a1 	= DFLT_LOAD_A1;
		loadICoefs[i].b2 	= DFLT_LOAD_B2;
		loadICoefs[i].a2 	= DFLT_LOAD_A2;
	}

	acICoefs.min 	= DFLT_AC_ISMIN;
	acICoefs.max 	= DFLT_AC_ISMAX;
	acICoefs.b0 	= DFLT_AC_IB0;
	acICoefs.b1 	= DFLT_AC_IB1;
	acICoefs.a1 	= DFLT_AC_IA1;
	acICoefs.b2 	= DFLT_AC_IB2;
	acICoefs.a2 	= DFLT_AC_IA2;

	acVCoefs.min 	= DFLT_AC_VSMIN;
	acVCoefs.max 	= DFLT_AC_VSMAX;
	acVCoefs.b0 	= DFLT_AC_VB0;
	acVCoefs.b1 	= DFLT_AC_VB1;
	acVCoefs.a1 	= DFLT_AC_VA1;
	acVCoefs.b2 	= DFLT_AC_VB2;
	acVCoefs.a2 	= DFLT_AC_VA2;
#ifdef AC_V_3P3Z
	acVCoefs.b3 	= DFLT_AC_VB3;
	acVCoefs.a3 	= DFLT_AC_VA3;
#endif
}

/*============== Load n ==============*/
static Uint16 getLoadICoefAddress(loadStage load, cfType coef, int32 *address) {
	/* Sets the specified coefficient for the specified load current control.
	 * The CNTL macro expects the coefficient structure layout as defined by
	 * DPLib, so cannot be redefined and does not allow programmatic indexing.
	 * The load specifier is NOT checked for validity.
	 */
	switch (coef) {
		case cMin:
			*address = (int32) &loadICoefs[load].min;
			break;
		case cMax:
			*address = (int32) &loadICoefs[load].max;
			break;
		case cB0:
			*address = (int32) &loadICoefs[load].b0;
			break;
		case cB1:
			*address = (int32) &loadICoefs[load].b1;
			break;
		case cA1:
			*address = (int32) &loadICoefs[load].a1;
			break;
		case cB2:
			*address = (int32) &loadICoefs[load].b2;
			break;
		case cA2:
			*address = (int32) &loadICoefs[load].a2;
			break;
		default:
			return CHANNEL_OOB;
	}
	return 0;
}

Uint16 setLoadICoef (loadStage load, cfType coef, float32 value) {
	/* Sets the specified coefficient for the specified load current control */
	Uint16 err = 0;
	int32 cfAddr = 0;
	if ((load >= numberOfLoads) || (coef > cA2))	/* Check load and coefficient specifiers are valid */
		return CHANNEL_OOB;
													/* Check the new value is within the coefficient boundaries */
	if ((value < cfLimits[cMin][coef]) || (value > cfLimits[cMax][coef]))
		return VALUE_OOB;

	err = getLoadICoefAddress(load, coef, &cfAddr);	/* Get the address of the specified coefficient on the specified load */
	if (err)										/* Check that the address was found without error */
		return err;
	if (coef > cMax)								/* Save the value to the found address */
		*((long *) cfAddr) = _IQ26(value);
	else
		*((long *) cfAddr) = _IQ24(value);
	return 0;
}

Uint16 getLoadICoef (loadStage load, cfType coef, float32 *value) {
	/* Gets the specified coefficient for the specified load current control */
	Uint16 err = 0;
	int32 cfAddr = 0;
	if ((load >= numberOfLoads) || (coef > cA2))	/* Check load and coefficient specifiers are valid */
		return CHANNEL_OOB;
	err = getLoadICoefAddress(load, coef, &cfAddr);	/* Get the address of the specified coefficient on the specified load */
	if (err)										/* Check that the address was found without error */
		return err;
	if (coef > cMax)								/* Save the value from the found address */
		*value = _IQ26toF(*((long *) cfAddr));
	else
		*value = _IQ24toF(*((long *) cfAddr));
	return 0;
}

/*=============== AC I ===============*/
static Uint16 getAcICoefAddress (cfType coef, int32 *address) {
	/* Gets the address for the specified coefficient for the AC current control */
	switch (coef) {
		case cMin:
			*address = (int32) &acICoefs.min;
			break;
		case cMax:
			*address = (int32) &acICoefs.max;
			break;
		case cB0:
			*address = (int32) &acICoefs.b0;
			break;
		case cB1:
			*address = (int32) &acICoefs.b1;
			break;
		case cA1:
			*address = (int32) &acICoefs.a1;
			break;
		case cB2:
			*address = (int32) &acICoefs.b2;
			break;
		case cA2:
			*address = (int32) &acICoefs.a2;
			break;
		default:
			return CHANNEL_OOB;
	}
	return 0;
}

Uint16 setAcICoef (cfType coef, float32 value) {
	/* Sets the specified coefficient for the AC stage current control */
	Uint16 err = 0;
	int32 cfAddr = 0;
	if (coef > cA2)							/* Check the coefficient specifier is valid */
		return CHANNEL_OOB;
											/* Check the new value is within the coefficient boundaries */
	if ((value < cfLimits[cMin][coef]) || (value > cfLimits[cMax][coef]))
		return VALUE_OOB;

	err = getAcICoefAddress(coef, &cfAddr);	/* Get the address of the specified coefficient */
	if (err)								/* Check that the address was found without error */
		return err;
	if (coef > cMax)						/* Save the value to the found address */
		*((long *) cfAddr) = _IQ26(value);
	else
		*((long *) cfAddr) = _IQ24(value);
	return 0;
}

Uint16 getAcICoef (cfType coef, float32 *value) {
	/* Gets the specified coefficient for the AC stage current control */
	Uint16 err = 0;
	int32 cfAddr = 0;
	if (coef > cA2)							/* Check the coefficient specifier is valid */
		return CHANNEL_OOB;
	err = getAcICoefAddress(coef, &cfAddr);	/* Get the address of the specified coefficient */
	if (err)								/* Check that the address was found without error */
		return err;
	if (coef > cMax)						/* Save the value from the found address */
		*value = _IQ26toF(*((long *) cfAddr));
	else
		*value = _IQ24toF(*((long *) cfAddr));
	return 0;
}

/*=============== AC V ===============*/
static Uint16 getAcVCoefAddress (cfType coef, int32 *address) {
	/* Gets the address for the specified coefficient for the AC voltage control */
	switch (coef) {
		case cMin:
			*address = (int32) &acVCoefs.min;
			break;
		case cMax:
			*address = (int32) &acVCoefs.max;
			break;
		case cB0:
			*address = (int32) &acVCoefs.b0;
			break;
		case cB1:
			*address = (int32) &acVCoefs.b1;
			break;
		case cA1:
			*address = (int32) &acVCoefs.a1;
			break;
		case cB2:
			*address = (int32) &acVCoefs.b2;
			break;
		case cA2:
			*address = (int32) &acVCoefs.a2;
			break;
		#ifdef AC_V_3P3Z
			case cB3:
				*address = (int32) &acVCoefs.b3;
				break;
			case cA3:
				*address = (int32) &acVCoefs.a3;
				break;
		#endif
		default:
			return CHANNEL_OOB;
	}
	return 0;
}

Uint16 setAcVCoef (cfType coef, float32 value) {
	/* Sets the specified coefficient for the AC stage voltage control */
	Uint16 err = 0;
	int32 cfAddr = 0;
	if (coef >= numCoefs)							/* Check the coefficient specifier is valid */
		return CHANNEL_OOB;
											/* Check the new value is within the coefficient boundaries */
	if ((value < cfLimits[cMin][coef]) || (value > cfLimits[cMax][coef]))
		return VALUE_OOB;

	err = getAcVCoefAddress(coef, &cfAddr);	/* Get the address of the specified coefficient */
	if (err)								/* Check that the address was found without error */
		return err;
	if (coef > cMax)						/* Save the value to the found address */
		*((long *) cfAddr) = _IQ26(value);
	else
		*((long *) cfAddr) = _IQ24(value);
	return 0;
}

Uint16 getAcVCoef (cfType coef, float32 *value) {
	/* Gets the specified coefficient for the AC stage voltage control */
	Uint16 err = 0;
	int32 cfAddr = 0;
	if (coef >= numCoefs)							/* Check the coefficient specifier is valid */
		return CHANNEL_OOB;
	err = getAcVCoefAddress(coef, &cfAddr);	/* Get the address of the specified coefficient */
	if (err)								/* Check that the address was found without error */
		return err;
	if (coef > cMax)						/* Save the value from the found address */
		*value = _IQ26toF(*((long *) cfAddr));
	else
		*value = _IQ24toF(*((long *) cfAddr));
	return 0;
}
