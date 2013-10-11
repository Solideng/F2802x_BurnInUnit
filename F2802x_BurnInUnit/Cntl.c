/*
 * Cntl.c
 *
 *  Created on: 28 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/*=============== GLOBAL VARS ==============*/
/* Declare closed loop feedback coefficient structure net nodes
 * The DATA_SECTION directive assigns the structures to the correct
 *  sections of dataRAM as mapped in the relevant .CMD file
 */
struct CNTL_2P2Z_CoefStruct loadICoefs [load4 + 1];
#pragma DATA_SECTION(loadICoefs, "CNTL_2P2Z_Coef")
struct CNTL_2P2Z_CoefStruct acICoefs;
#pragma DATA_SECTION(acICoefs, "CNTL_2P2Z_Coef");
struct CNTL_3P3Z_CoefStruct acVCoefs;
#pragma DATA_SECTION(acVCoefs, "CNTL_3P3Z_Coef")

#define INIT_SMIN	0			/* IQ24 0.0 */
#define INIT_SMAX	15099494	/* IQ24 0.9 */
#define INIT_B0		241969836	/* IQ26 3.60563154 */
#define INIT_B1		81334398	/* IQ26 1.21197699 */
#define INIT_A1		0			/* IQ26 0.0 */
#define INIT_B2 	-160635437	/* IQ26 -2.39365455 */
#define INIT_A2		67108864	/* IQ26 1.0 */
#define INIT_B3		0			/* IQ26 0.0 */
#define INIT_A3		0			/* IQ26 0.0 */

/*=============== LOCAL VARS ===============*/
										/*	 min,       max,       B0,      B1,      A1,      B2,      A2,      B3,      A3	 */
static int32 pCfs [NUM_CHNLS][cA3+ 1] = {	{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 1. */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 2. */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 3. */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 4. */
											{INIT_SMIN, 11744051,  INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* AC I CNTL (SMAX 0.7) */
										//	{INIT_SMIN, 16777216,  INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* DC STAGE (SMAX 1.0) */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3}};	/* AC V STAGE */

							/*	 min, max, B0,  B1,  A1,  B2,  A2,  B3,  A3	 */
float32 cfLmts[2][cA3 + 1] = {	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},	/* Coefficient minimums. */
								{1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};	/* Coefficient maximums. */

/*==== SETUP COEFFICIENT STRUCT MEMBERS ====*/
void cntlUpdateCoefs (void) {
	/* Should be called by one of the state machine tasks */
	Uint16 i = 0, j = 0;
	for (i = 0; i < numberOfLoads; i++) {
		j = 0;
		loadICoefs[i].min = pCfs[i][j++];
		loadICoefs[i].max = pCfs[i][j++];
		loadICoefs[i].b0  = pCfs[i][j++];
		loadICoefs[i].b1  = pCfs[i][j++];
		loadICoefs[i].a1  = pCfs[i][j++];
		loadICoefs[i].b2  = pCfs[i][j++];
		loadICoefs[i].a2  = pCfs[i][j++];
	}

	j = 0;
	acICoefs.min = pCfs[i][j++];
	acICoefs.max = pCfs[i][j++];
	acICoefs.b0  = pCfs[i][j++];
	acICoefs.b1  = pCfs[i][j++];
	acICoefs.a1  = pCfs[i][j++];
	acICoefs.b2  = pCfs[i][j++];
	acICoefs.a2  = pCfs[i][j++];
	i++;

	j = 0;
	acVCoefs.min = pCfs[i][j++];
	acVCoefs.max = pCfs[i][j++];
	acVCoefs.b0  = pCfs[i][j++];
	acVCoefs.b1  = pCfs[i][j++];
	acVCoefs.a1  = pCfs[i][j++];
	acVCoefs.b2  = pCfs[i][j++];
	acVCoefs.a2  = pCfs[i][j++];

	return;
}

static float32 cntlGetCoef (Uint16 i, Uint16 j) {
	if (j > cMax)
		return _IQ26toF(pCfs[i][j]);
	else
		return _IQ24toF(pCfs[i][j]);
}

static Uint16 cntlSetCoef (Uint16 i, Uint16 j, float32 val) {
	if ((val < cfLmts[cMin][j]) || (val > cfLmts[cMax][j]))
		return VALUE_OOB;
	if (j > cMax) {
		pCfs[i][j] = _IQ26(val);
	} else {
		pCfs[i][j] = _IQ24(val);
	}
	return 0;
}

Uint16 cntlGetLoadICoef (loadStage load, cfType coef, float32 *valDest) {
	/* Gets the specified coefficient for the specified load
	 * The coefficient reported is not the coefficient actually in use but the
	 * one set to be used after the next iteration of cntlUpdateCoefs().
	 */
	if ((load >= numberOfLoads) || (coef > cA2))
		return CHANNEL_OOB;
	*valDest = cntlGetCoef(load, coef);
	return 0;
}

Uint16 cntlGetAcICoef (cfType coef, float32 * valDest) {
	/* Gets the specified coefficient for the AC I IIR filter control law.
	 * The coefficient reported is not the coefficient actually in use but the
	 * one set to be used after the next iteration of cntlUpdateCoefs().
	 */
	if (coef > cA3)
		return CHANNEL_OOB;
	*valDest = cntlGetCoef(4, coef);
	return 0;
}

Uint16 cntlGetAcVCoef (cfType coef, float32 *valDest) {
	/* Gets the specified coefficient for the AC V IIR filter control law.
	 * The coefficient reported is not the coefficient actually in use but the
	 * one set to be used after the next iteration of cntlUpdateCoefs().
	 */
	if (coef > cA3)
		return CHANNEL_OOB;
	*valDest = cntlGetCoef(5, coef);
	return 0;
}

Uint16 cntlSetLoadICoef (loadStage load, cfType coef, float32 val) {
	/* Sets the specified coefficient on the specified load.
	 * Actual coefficient in use is not changed until cntlUpdatCoefs() is
	 * called as otherwise the DPL_ISR() could be run *as* the change is being
	 * made, also, the coefficient structure layout is defined by DPLib, cannot
	 * be redefined and does not allow programmatic indexing.
	 */
	if ((load >= numberOfLoads) || (coef > cA2))
		return CHANNEL_OOB;
	return cntlSetCoef(load, coef, val);
}

Uint16 cntlSetAcICoef (cfType coef, float32 val) {
	/* Sets the specified coefficient on the specified load.
	 * Actual coefficient in use is not changed until cntlUpdatCoefs() is
	 * called
	 */
	if (coef > cA3)
		return CHANNEL_OOB;
	return cntlSetCoef(4, coef, val);
}

Uint16 cntlSetAcVCoef (cfType coef, float32 val) {
	/* Sets the specified coefficient on the specified load.
	 * Actual coefficient in use is not changed until cntlUpdatCoefs() is
	 * called
	 */
	if (coef > cA3)
		return CHANNEL_OOB;
	return cntlSetCoef(5, coef, val);
}
