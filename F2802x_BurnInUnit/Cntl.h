/**
 * @file Cntl.h
 *
 * @brief DPLib CNTL Macro related helper functions.
 *
 */
#ifndef CNTL_H_
#define CNTL_H_

//#define SATMAX_MAX 0.9f	/**< The maximum allowable value for the IIR filter control law's maximum saturation. */

//#define INIT_SMIN	0			/* IQ24 0.0 */
//#define INIT_SMAX	15099494	/* IQ24 0.9 */
//#define INIT_B0		241969836	/* IQ26 3.60563154 */
//#define INIT_B1		81334398	/* IQ26 1.21197699 */
//#define INIT_A1		0			/* IQ26 0.0 */
//#define INIT_B2 	-160635437	/* IQ26 -2.39365455 */
//#define INIT_A2		67108864	/* IQ26 1.0 */
//#define INIT_B3		0			/* IQ26 0.0 */
//#define INIT_A3		0			/* IQ26 0.0 */

#define DFLT_LOAD_SMIN	0			/* IQ24 0.0 */
#define DFLT_LOAD_SMAX	15099494	/* IQ24 0.9 */
#define DFLT_LOAD_B0	241969836	/* IQ26 3.60563154 */
#define DFLT_LOAD_B1	81334398	/* IQ26 1.21197699 */
#define DFLT_LOAD_A1	0			/* IQ26 0.0 */
#define DFLT_LOAD_B2 	-160635437	/* IQ26 -2.39365455 */
#define DFLT_LOAD_A2	67108864	/* IQ26 1.0 */
#define DFLT_LOAD_B3	0			/* IQ26 0.0 */
#define DFLT_LOAD_A3	0			/* IQ26 0.0 */

#define DFLT_AC_ISMIN 	0			/* IQ24 0 */
#define DFLT_AC_ISMAX 	15099494	/* IQ24 0.9 */
#define DFLT_AC_IB0		34581200	/* IQ26 0.5153 */
#define DFLT_AC_IB1		15482015 	/* IQ26 0.2307 */
#define DFLT_AC_IB2		-19099182	/* IQ26 -0.2846 */
#define DFLT_AC_IA1		0			/* IQ26 0 */
#define DFLT_AC_IA2		67108864	/* IQ26 1 */

#define DFLT_AC_VSMIN 	-2130706432	/* IQ24 -127 */
#define DFLT_AC_VSMAX 	2130706432	/* IQ24 127 */
#define DFLT_AC_VB0		77423496	/* IQ26 1.1537 */
#define DFLT_AC_VB1		24870544 	/* IQ26 0.3706 */
#define DFLT_AC_VB2		-56780812	/* IQ26 -0.8461 */
#define DFLT_AC_VA1		0			/* IQ26 0 */
#define DFLT_AC_VA2		67108864	/* IQ26 1 */

#ifdef AC_V_3P3Z
	#define DFLT_AC_VB3 	0	/* IQ26 0 */
	#define DFLT_AC_VB3 	0	/* IQ26 0 */
#endif

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
#ifdef AC_V_3P3Z
	cB3,		/**< B3 coefficient reference. */
	cA3,		/**< A3 coefficient reference. */
#endif
	numCoefs
};
/** A type that allows a reference to a CNTL coefficient. */
typedef enum coefNum cfType;

extern struct CNTL_2P2Z_CoefStruct loadICoefs [numberOfLoads];	/**< Array of structures that hold the load 2-pole 2-zero IIR filter control law coefficients currently in use. */
extern struct CNTL_2P2Z_CoefStruct acICoefs;	/**< Structure that holds the AC I 2-pole 2-zero IIR filter control law coefficients currently in use. */
#ifdef AC_V_3P3Z
	extern struct CNTL_3P3Z_CoefStruct acVCoefs;	/**< Structure that holds the AC V 3-pole 3-zero IIR filter control law coefficients currently in use. */
#else
	extern struct CNTL_2P2Z_CoefStruct acVCoefs;	/**< Structure that holds the AC V 2-pole 2-zero IIR filter control law coefficients currently in use. */
#endif

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

#ifdef AC_V_3P3Z
	extern volatile int32 *CNTL_3P3Z_Coef1;	/**< AC stage V IIR filter control law coefficient terminal pointer. */
	extern volatile int32 *CNTL_3P3Z_Fdbk1;	/**< AC stage V IIR filter control law feedback terminal pointer. */
	extern volatile int32 *CNTL_3P3Z_Out1;	/**< AC stage V IIR filter control law output terminal pointer. */
	extern volatile int32 *CNTL_3P3Z_Ref1;	/**< AC stage V IIR filter control law reference terminal pointer. */
#else
	extern volatile int32 *CNTL_2P2Z_Coef6;	/**< AC stage V IIR filter control law coefficient terminal pointer. */
	extern volatile int32 *CNTL_2P2Z_Fdbk6;	/**< AC stage V IIR filter control law feedback terminal pointer. */
	extern volatile int32 *CNTL_2P2Z_Out6;	/**< AC stage V IIR filter control law output terminal pointer. */
	extern volatile int32 *CNTL_2P2Z_Ref6;	/**< AC stage V IIR filter control law reference terminal pointer. */
#endif

/** Initialises all IIR filter control law coefficients.
 * @warning This function must be called before the control macros are used.
 */
extern void initCoefs (void);

/*============== Load n ==============*/
/** Sets the specified IIR filter control law coefficient for a specified load's current control.
 * @param[in]	load	Specifies the load number the setting is to be applied to [0, NUM_CHNLS).
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	value	Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[][].
 * @return				Error status.
 */
extern Uint16 setLoadICoef (loadStage load, cfType coef, float32 value);

/** Queries the specified IIR filter control law coefficient for a specified load's current control.
 * @param[in]	load	Specifies the load on which the setting is to be queried [0, NUM_CHNLS).
 * @param[in]	coef	Specifies the coefficient to be queried [cMin, cA3).
 * @param[out]	value	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 getLoadICoef (loadStage load, cfType coef, float32 *value);

/*=============== AC I ===============*/
/** Sets the specified IIR filter control law coefficient for the AC current control.
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	val		Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[][].
 * @return				Error status.
 */
extern Uint16 cntlSetAcICoef (cfType coef, float32 val);

/** Queries the specified IIR filter control law coefficient for the AC current control.
 * @param[in]	coef	Specifies the coefficient to be queried [cMin, cA3).
 * @param[out]	value	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 getAcICoef (cfType coef, float32 *value);

/*=============== AC V ===============*/
/** Sets the specified IIR filter control law coefficient for the AC voltage control.
 * - The actual setting in use is not updated until AFTER cntlUpdateCoefs() has been called.
 * @param[in]	coef	Specifies the coefficient to be set [cMin, cA3).
 * @param[in]	value	Specifies the coefficient value to be applied. Should be between the minimum and maximum values for the specific coefficient as defined by cfLmts[][].
 * @return				Error status.
 */
extern Uint16 setAcVCoef (cfType coef, float32 value);

/** Queries the specified IIR filter control law coefficient for the AC voltage control.
 * @param[in]	coef	Specifies the coefficient to be queried [cMin, cA3).
 * @param[out]	value	Address of the memory location at which to place the query result.
 * @return				Error status.
 */
extern Uint16 getAcVCoef (cfType coef, float32 *value);

#endif /* CNTL_H_ */
