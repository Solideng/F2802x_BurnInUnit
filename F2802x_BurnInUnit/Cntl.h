/**
 * @file Cntl.h
 *
 * @brief DPLib CNTL Macro related helper functions.
 *
 */
#ifndef CNTL_H_
#define CNTL_H_

/*================== MACROS ===================*/
#define SATMAX_MAX 0.9f	/**< The maximum allowable value for the IIR filter control law's maximum saturation. */

/*================ GLOBAL VARS ================*/
/** CNTL Coefficient references */
enum coefNum {
	firstCoef = 0,
	cMin = firstCoef,	/**< Saturation minimum reference. */
	cMax,		/**< Saturation maximum reference. */
	cB0,		/**< B0 coefficient reference. */
	cB1,		/**< B1 coefficient reference. */
	cA1,		/**< A1 coefficient reference. */
	cB2,		/**< B2 coefficient reference. */
	cA2,		/**< A2 coefficient reference. */
	cB3,		/**< B3 coefficient reference. */
	cA3,		/**< A3 coefficient reference. */
	numCoefs
};
/** A type that allows a reference to a CNTL coefficient. */
typedef enum coefNum cfType;

extern volatile int32 *CNTL_2P2Z_Coef1;	/**< Channel 0 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef2;	/**< Channel 1 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef3;	/**< Channel 2 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef4;	/**< Channel 3 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef5;	/**< Channel 4 IIR filter control law coefficient terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Fdbk1;	/**< Channel 0 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk2;	/**< Channel 1 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk3;	/**< Channel 2 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk4;	/**< Channel 3 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk5;	/**< Channel 4 IIR filter control law feedback terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Out1;	/**< Channel 0 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out2;	/**< Channel 1 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out3;	/**< Channel 2 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out4;	/**< Channel 3 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out5;	/**< Channel 4 IIR filter control law output terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Ref1;	/**< Channel 0 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref2;	/**< Channel 1 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref3;	/**< Channel 2 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref4;	/**< Channel 3 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref5;	/**< Channel 4 IIR filter control law reference terminal pointer. */

extern volatile int32 *CNTL_3P3Z_Coef1;	/**< Interboost IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Coef2;	/**< AC stage IIR filter control law coefficient terminal pointer. */

extern volatile int32 *CNTL_3P3Z_Fdbk1;	/**< Interboost IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Fdbk2;	/**< AC stage IIR filter control law feedback terminal pointer. */

extern volatile int32 *CNTL_3P3Z_Out1;	/**< Interboost IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Out2;	/**< AC stage IIR filter control law output terminal pointer. */

extern volatile int32 *CNTL_3P3Z_Ref1;	/**< Interboost IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Ref2;	/**< AC stage IIR filter control law reference terminal pointer. */

extern struct CNTL_2P2Z_CoefStruct coefs2 [NUM_ICTRL_CHNLS];	/**< Array of structures that hold the 2-pole 2-zero IIR filter control law coefficient currently in use. */
extern struct CNTL_3P3Z_CoefStruct coefs3 [NUM_VCTRL_CHNLS];	/**< Array of structures that hold the 3-pole 3-zero IIR filter control law coefficient currently in use. */

/*============= GLOBAL FUNCTIONS ==============*/
/** Updates the IIR filter control law's coefficients that are being used to those values set by the use of the other functions within this file. */
extern void cntlUpdateCoefs (void);

/** Queries the specified IIR filter control law coefficient for the specified channel.
 * @param[in]	chnl	Specifies the channel number on which the setting is to be queried.
 * @param[in]	coef	Specifies the coefficient to be queried.
 * @param[out]	valDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 cntlGetCoef (Uint16 chnl, cfType coef, float32 *valDest);

/** Sets the specified IIR filter control law coefficient for the specified channel.
 * - The actual setting in use is not updated until AFTER cntlUpdateCoefs() has been called.
 * @param[in]	chnl	Specifies the channel number the setting is to be applied to [0, NUM_CHNLS).
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	val		Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[coef] and cfLmts[coef + cA3].
 * @return				Error status.
 */
extern Uint16 cntlSetCoef (Uint16 chnl, cfType coef, float32 val);

#endif /* CNTL_H_ */
