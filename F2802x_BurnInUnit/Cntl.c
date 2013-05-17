/*
 * Cntl.c
 *
 *  Created on: 28 Mar 2013
 *      Author: Toby
 */
#include "Common.h"

/*=============== GLOBAL VARS ==============*/
/* Declare closed loop feedback coefficient struct nodes
 * The DATA_SECTION directive assigns the structs to the correct
 *  sections of dataRAM as mapped in the relevant .CMD file
 */

#ifdef USE_PID
	struct CNTL_2P2Z_CoefStruct coefs2 [NUM_CHNLS];
	#pragma DATA_SECTION(coefs2, "CNTL_2P2Z_Coef")
#else
	struct CNTL_2P2Z_CoefStruct coefs2 [NUM_ICTRL_CHNLS];
	#pragma DATA_SECTION(coefs2, "CNTL_2P2Z_Coef")
	struct CNTL_3P3Z_CoefStruct coefs3 [NUM_VCTRL_CHNLS];
	#pragma DATA_SECTION(coefs3, "CNTL_3P3Z_Coef")

#endif

/*=============== LOCAL VARS ===============*/
	// REMOVE FOR RELEASE
#ifdef USE_PID
	/* Default PID settings  */
	int16 pGain[NUM_CHNLS] = {50, 50, 50, 50, 50, 10000, 5000}; /* Arrays for PID for use during debug (SQ8) */
	int16 iGain[NUM_CHNLS] = {1, 1, 1, 1, 1, 20, 1000};
	int16 dGain[NUM_CHNLS] = {0, 0, 0, 0, 0, 5000, 1000};
	float32 satMax[NUM_CHNLS] = {0.9, 0.9, 0.9, 0.9, 0.9, 0.7, 1.0};
	//int32 satMax[NUM_CHNLS] = {15099494, 15099494, 15099494, 15099494, 11744051, 16777216}; /* 4 x 0.9, 0.7, 1.0 */
#endif
	//TODO Need actual value
static float32 cfLmts[2][cA3 + 1] = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
static int32 pCfs [NUM_CHNLS][cA3+ 1] = {0};

/*==== SETUP COEFFICIENT STRUCT MEMBERS ====*/
void cntlUpdateCoefs (void) {
	/* Should be called by one of the state machine tasks */
	#ifdef USE_PID
		int32 temp = 0;
	#else
		Uint16 j = 0;
	#endif
	// TODO: Rewrite cntlUpdateCoefs() for final use
	Uint16 i = 0;
	for (i = 0; i < NUM_CHNLS; i++) {
		#ifdef USE_PID
			if (channel[i].ctlMode == iCtrl) {
				coefs2[i].b2 = _IQ26(-2.39365455);	/* IQ26 */
				coefs2[i].b1 = _IQ26(1.21197699);	/* IQ26 */
				coefs2[i].b0 = _IQ26(3.60563154);	/* IQ26 */
				coefs2[i].a2 = _IQ26(1.0);			/* IQ26 */
				coefs2[i].a1 = 0;					/* IQ26 */
				coefs2[i].max = _IQ24(satMax[i]);	/* IQ24 */
				coefs2[i].min = 0;					/* IQ24 */

				/* Update the channel struct PID values */
				temp = coefs2[i].b0 - coefs2[i].b1;				/* b0 - b1 = 2p */
				temp = temp >> 1;								/* (b0 - b1) / 2 = p */
				channel[i].pGain = (int16)(temp >> 18);			/* Convert to SQ8 */
				temp = coefs2[i].b0 + coefs2[i].b1;				/* b0 + b1 = 2i */
				temp = temp >> 1; 								/* (b0 + b1) / 2 = i */
				channel[i].iGain = (int16)(temp >> 18);			/* Convert to SQ8 */
				channel[i].dGain = (int16)(coefs2[i].b2 >> 18);	/* Convert to SQ8 */


			} else {
				coefs2[i].b2 = _IQ16(channel[i].dGain);
				coefs2[i].b1 = _IQ16(channel[i].iGain - channel[i].pGain - channel[i].dGain - channel[i].dGain);
				coefs2[i].b0 = _IQ16(channel[i].pGain + channel[i].iGain + channel[i].dGain);
				coefs2[i].a2 = 0;
				coefs2[i].a1 = _IQ26(1.0);
				coefs2[i].max = _IQ24(satMax[i]);
				coefs2[i].min = 0;
			}
		#else
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
		#endif
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
	 * and does not allow programmatic indexing
	 */
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
