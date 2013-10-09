/**
 * @file Cntl.h
 *
 * @brief DPLib CNTL Macro related helper functions.
 *
 */
#ifndef CNTL_H_
#define CNTL_H_

/*================== MACROS ===================*/
//#define SATMAX_MAX 0.9f	/**< The maximum allowable value for the IIR filter control law's maximum saturation. */

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

extern volatile int32 *CNTL_2P2Z_Coef1;	/**< Load 1 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef2;	/**< Load 2 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef3;	/**< Load 3 IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Coef4;	/**< Load 4 IIR filter control law coefficient terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Fdbk1;	/**< Load 1 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk2;	/**< Load 2 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk3;	/**< Load 3 IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk4;	/**< Load 4 IIR filter control law feedback terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Out1;	/**< Load 1 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out2;	/**< Load 2 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out3;	/**< Load 3 IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out4;	/**< Load 4 IIR filter control law output terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Ref1;	/**< Load 1 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref2;	/**< Load 2 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref3;	/**< Load 3 IIR filter control law reference terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref4;	/**< Load 4 IIR filter control law reference terminal pointer. */

extern volatile int32 *CNTL_2P2Z_Coef5;	/**< AC stage I IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Fdbk5;	/**< AC stage I IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Out5;	/**< AC stage I IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_2P2Z_Ref5;	/**< AC stage I IIR filter control law reference terminal pointer. */

extern volatile int32 *CNTL_3P3Z_Coef1;	/**< AC stage V IIR filter control law coefficient terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Fdbk1;	/**< AC stage V IIR filter control law feedback terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Out1;	/**< AC stage V IIR filter control law output terminal pointer. */
extern volatile int32 *CNTL_3P3Z_Ref1;	/**< AC stage V IIR filter control law reference terminal pointer. */

extern struct CNTL_2P2Z_CoefStruct loadICoefs [numberOfLoads];	/**< Array of structures that hold the load 2-pole 2-zero IIR filter control law coefficient currently in use. */
extern struct CNTL_2P2Z_CoefStruct acICoefs;	/**< Structure that holds the AC I 2-pole 2-zero IIR filter control law coefficient currently in use. */
extern struct CNTL_3P3Z_CoefStruct acVCoefs;	/**< Structure that holds the AC V 2-pole 2-zero IIR filter control law coefficient currently in use. */

/*============= GLOBAL FUNCTIONS ==============*/
/** Updates the IIR filter control law's coefficients that are being used to those values set by the use of the other functions within this file. */
extern void cntlUpdateCoefs (void);

/** Queries the specified IIR filter control law coefficient for a specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried [0, NUM_CHNLS).
 * @param[in]	coef	Specifies the coefficient to be queried [cMin, cA3).
 * @param[out]	valDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 cntlGetLoadICoef (loadStage load, cfType coef, float32 *valDest);

/** Queries the specified IIR filter control law coefficient for the AC I control.
 * @param[in]	coef	Specifies the coefficient to be queried [cMin, cA3).
 * @param[out]	valDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 cntlGetAcICoef (cfType coef, float32 *valDest);

/** Queries the specified IIR filter control law coefficient for the AC V control.
 * @param[in]	coef	Specifies the coefficient to be queried [cMin, cA3).
 * @param[out]	valDest	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 cntlGetAcVCoef (cfType coef, float32 *valDest);

/** Sets the specified IIR filter control law coefficient for a specified load.
 * - The actual setting in use is not updated until AFTER cntlUpdateCoefs() has been called.
 * @param[in]	load	Specifies the load number the setting is to be applied to [0, NUM_CHNLS).
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	val		Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[coef] and cfLmts[coef + cA3].
 * @return				Error status.
 */
extern Uint16 cntlSetLoadICoef (loadStage chnl, cfType coef, float32 val);

/** Sets the specified IIR filter control law coefficient for the AC I control.
 * - The actual setting in use is not updated until AFTER cntlUpdateCoefs() has been called.
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	val		Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[coef] and cfLmts[coef + cA3].
 * @return				Error status.
 */
extern Uint16 cntlSetAcICoef (cfType coef, float32 val);

/** Sets the specified IIR filter control law coefficient for the AC V control.
 * - The actual setting in use is not updated until AFTER cntlUpdateCoefs() has been called.
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	val		Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[coef] and cfLmts[coef + cA3].
 * @return				Error status.
 */
extern Uint16 cntlSetAcVCoef (cfType coef, float32 val);

#endif /* CNTL_H_ */
