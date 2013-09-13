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
struct CNTL_2P2Z_CoefStruct coefs2 [NUM_ICTRL_CHNLS];
#pragma DATA_SECTION(coefs2, "CNTL_2P2Z_Coef")
struct CNTL_3P3Z_CoefStruct coefs3 [NUM_VCTRL_CHNLS];
#pragma DATA_SECTION(coefs3, "CNTL_3P3Z_Coef")

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
static int32 pCfs [NUM_CHNLS][cA3+ 1] = {	{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 0. */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 1. */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 2. */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* LOAD 3. */
											{INIT_SMIN, 11744051,  INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* AC I CNTL (SMAX 0.7) */
											{INIT_SMIN, 16777216,  INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3},	/* DC STAGE (SMAX 1.0) */
											{INIT_SMIN, INIT_SMAX, INIT_B0, INIT_B1, INIT_A1, INIT_B2, INIT_A2, INIT_B3, INIT_A3}};	/* AC STAGE */

/*==== SETUP COEFFICIENT STRUCT MEMBERS ====*/
void cntlUpdateCoefs (void) {
	/* Should be called by one of the state machine tasks */
	Uint16 i = 0, j = 0;
	for (i = 0; i < NUM_CHNLS; i++) {
			j = 0;
			if (channel[i].ctlMode == iCtrl) {
				coefs2[i].min = pCfs[i][j++];
				coefs2[i].max = pCfs[i][j++];
				coefs2[i].b0 = pCfs[i][j++];
				coefs2[i].b1 = pCfs[i][j++];
				coefs2[i].a1 = pCfs[i][j++];
				coefs2[i].b2 = pCfs[i][j++];
				coefs2[i].a2 = pCfs[i][j++];
			} else {
				coefs3[i - NUM_ICTRL_CHNLS].min = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].max = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].b0 = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].b1 = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].a1 = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].b2 = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].a2 = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].b3 = pCfs[i][j++];
				coefs3[i - NUM_ICTRL_CHNLS].a3 = pCfs[i][j++];
			}
	}
	return;
}

Uint16 cntlGetCoef (Uint16 chnl, cfType coef, float32 *valDest) {
	/* Gets the specified coef on the specified channel
	 * The coef reported is not the coef actually in use but the one set
	 * to be used after the next iteration of cntlUpdateCoefs()
	 */
	if ((chnl >= NUM_CHNLS) || (coef > cA3))
		return CHANNEL_OOB;
	if ((coef > cA2) && (channel[chnl].ctlMode == iCtrl))
		return CHANNEL_OOB;
	if (coef > cMax)
		*valDest = _IQ26toF(pCfs[chnl][coef]);
	else
		*valDest = _IQ24toF(pCfs[chnl][coef]);
	return 0;
}

Uint16 cntlSetCoef (Uint16 chnl, cfType coef, float32 val) {
	/* Sets the specified coef on the specified channel
	 * Actual coef in use is not changed until cntlUpdatCoefs() is called
	 * as otherwise the DPL_ISR() could be run *as* the change is being made
	 * + the coef structure layout is defined by DPLib, cannot be redefined
	 * and does not allow programmatic indexing.
	 */

	//TODO Need actual value	/*	 min, max, B0,  B1,  A1,  B2,  A2,  B3,  A3	 */
	float32 cfLmts[2][cA3 + 1] = {	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},	/* Coefficient minimums. */
									{1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};	/* Coefficient maximums. */

	if ((chnl >= NUM_CHNLS) || (coef > cA3))
		return CHANNEL_OOB;
	if ((coef > cA2) && (channel[chnl].ctlMode == iCtrl))
		return CHANNEL_OOB;
	if ((val < cfLmts[cMin][coef]) || (val > cfLmts[cMax][coef]))
		return VALUE_OOB;
	if (coef > cMax) {
		pCfs[chnl][coef] = _IQ26(val);
	} else {
		pCfs[chnl][coef] = _IQ24(val);
	}
	return 0;
}
