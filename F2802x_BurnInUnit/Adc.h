/**
 * @file Adc.h
 *
 * @brief ADC functions.
 *
 */

#ifndef ADC_H_
#define ADC_H_

/*============= GLOBAL FUNCTIONS ==============*/
/** Configures ADC SOC for ADC macro */
extern void adcSocCnf (void);

/** Configures the ADC's SOCs then calls pwmSocConfigure().
 *  - SHOULD be run after pwmMacroConfigure().
 *  - SHOULD be run before DPL_INIT().
 */
extern void adcMacroConfigure (void);

/** Checks the current voltage sense ADC readings against the OVP limits.
 * @return Error status
 */
extern Uint16 adcCheckOvp (void);


/** Sets the current scaling for the specified load.
 * @param[in]	load 			Specifies the load the setting is to be applied to.
 * @param[in]	scaleSetting	Specifies the value of the scaling setting to be applied (amps-per-volts).
 * @return						Error status.
 */
extern Uint16 adcSetLoadIScale (loadStage load, float32 scaleSetting);

/** Sets the voltage scaling for the specified load.
 * @param[in]	load 			Specifies the load the setting is to be applied to.
 * @param[in]	scaleSetting	Specifies the value of the scaling setting to be applied (volts-per-volts).
 * @return						Error status.
 */
extern Uint16 adcSetLoadVScale (loadStage load, float32 scaleSetting);

/** Sets the over voltage protection limit for the specified load.
 * @param[in]	load 		Specifies the load the setting is to be applied to.
 * @param[in]	ovpSetting	Specifies the value of the limit to be applied (volts).
 * @return					Error status.
 */
extern Uint16 adcSetLoadOvp (loadStage load, float32 ovpSetting);

/** Queries the current current scaling setting of the specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried.
 * @param[out]	sclDest	Address of the memory location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 adcGetLoadIScale (loadStage load, float32 * sclDest);

/** Queries the current voltage scaling setting of the specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried.
 * @param[out]	sclDest	Address of the memory location at which to place the query result (volts).
 * @return				Error status.
 */
extern Uint16 adcGetLoadVScale (loadStage load, float32 * sclDest);

/** Queries the most recent voltage reading from the specified load's associated ADC.
 * @param[in]	load	Specifies the load on which the reading is to be queried.
 * @param[out]	vDest	Address of the memory location at which to place the query result (volts).
 * @return				Error status.
 */
extern Uint16 adcGetLoadVoltage (loadStage load, float32 * vDest);

/** Queries the most recent current reading from the specified load's associated ADC.
 * @param[in]	load	Specifies the load on which the reading is to be queried.
 * @param[out]	iDest	Address of the memory location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 adcGetLoadCurrent (loadStage load, float32 * iDest);

/** Queries the over current protection setting for the specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried.
 * @param[out]	ovpDest	Address of the memory location at which to place the query result (volts).
 * @return				Error status.
 */
extern Uint16 adcGetLoadOvp (loadStage load, float32 * ovpDest);

/*================ GLOBAL VARS ================*/
/*============= TERMINAL POINTERS =============*/
extern volatile int32 *ADCDRV_1ch_Rlt1;	/**< Channel 0 current sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt2;	/**< Channel 1 current sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt3;	/**< Channel 2 current sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt4;	/**< Channel 3 current sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt5;	/**< Interboost current sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt6;	/**< AC stage current sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt7;	/**< Channel 0 voltage sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt8;	/**< Channel 1 voltage sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt9;	/**< Channel 2 voltage sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt10;/**< Channel 3 voltage sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt11;/**< Interboost voltage sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt12;/**< AC stage voltage sense ADC terminal pointer. */
extern volatile int32 *ADCDRV_1ch_Rlt13;/**< VMid voltage sense ADC terminal pointer. */

/*========= SYMBOLS DEFINED BY DP LIB =========*/
extern void ADC_SOC_CNF(int16 ChSel[], int16 Trigsel[], int16 ACQPS[], int16 IntChSel, int16 mode);

#endif /* ADC_H_ */
