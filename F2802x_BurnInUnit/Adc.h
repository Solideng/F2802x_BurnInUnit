/**
 * @file Adc.h
 *
 * @brief ADC, DAC, comparator and related functions.
 *
 */

#ifndef ADC_H_
#define ADC_H_

/*============= GLOBAL FUNCTIONS ==============*/
#ifdef DEBUG_ADC
	extern void adcGUI (void);
#endif

extern void adcSocCnf (void);

/** Configures the ADC's SOCs then calls pwmSocConfigure().
 *  - SHOULD be run after pwmMacroConfigure().
 *  - SHOULD be run before DPL_INIT().
 */
extern void adcMacroConfigure (void);

/** Configures the COMP 1 & 2 comparators using the internal DACs at inverting inputs.
 *  - SHOULD be called AFTER adcSocCnf().
 *  - SHOULD be called BEFORE PWMS (SYNC) are started.
 *  - SHOULD be called BEFORE pwmTZConfigure().
 */
extern void adcCompConfigure (void);

/** Checks the current current sense ADC readings against the OCP limits.
 * @return Error status
 */
extern Uint16 adcCheckOcp (void);

/** Checks the current voltage sense ADC readings against the OVP limits.
 * @return Error status
 */
extern Uint16 adcCheckOvp (void);

/** Sets the output levels of the DACs on the inverting input of the comparators.
 * The function will determine the scaling to be applied by testing the ctrlMode setting of the channel specified.
 * The respective channel's current or voltage MUST be set previously.
 * @param[in]	chnl	Specifies the channel number the setting is to be applied to.
 * @param[in]	dacLvl	Specifies the value of the level setting to be applied (volts or amps).
 * @return				Error status.
 */
extern Uint16 adcSetDac (Uint16 chnl, float32 dacLvl);

/** Sets the current scaling for the specified channel.
 * @param[in]	chnl 			Specifies the channel number the setting is to be applied to.
 * @param[in]	scaleSetting	Specifies the value of the scaling setting to be applied (amps/volts).
 * @return						Error status.
 */
extern Uint16 adcSetIScale (Uint16 chnl, float32 scaleSetting);

/** Sets the voltage scaling for the specified channel.
 * @param[in]	chnl 			Specifies the channel number the setting is to be applied to.
 * @param[in]	scaleSetting	Specifies the value of the scaling setting to be applied (volts/volts).
 * @return						Error status.
 */
extern Uint16 adcSetVScale (Uint16 chnl, float32 scaleSetting);

/** Sets the over current protection limit for the specified channel.
 * @param[in]	chnl		Specifies the channel number the setting is to be applied to.
 * @param[in]	ocpSetting	Specifies the value of the limit to be applied (Amps).
 * @return					Error status.
 */
extern Uint16 adcSetOcp (Uint16 chnl, float32 ocpSetting);

/** Sets the over voltage protection limit for the specified channel.
 * @param[in]	chnl 		Specifies the channel number the setting is to be applied to.
 * @param[in]	ovpSetting	Specifies the value of the limit to be applied (volts).
 * @return					Error status.
 */
extern Uint16 adcSetOvp (Uint16 chnl, float32 ovpSetting);

/** Queries the output level setting of the DAC on the inverting input of the comparators.
 * @param[in]	chnl	Specifies the channel number on which the setting is to be queried.
 * @param[out]	dacDest	Address of the memory location at which to place the query result (volts or amps).
 * @return				Error status.
 */
extern Uint16 adcGetDac (Uint16 chnl, float32 *dacDest);

/** Queries the current current scaling setting of the specified channel.
 * @param[in]	chnl	Specifies the channel number on which the setting is to be queried.
 * @param[out]	sclDest	Address of the memory location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 adcGetIScale (Uint16 chnl, float32 * sclDest);

/** Queries the current voltage scaling setting of the specified channel.
 * @param[in]	chnl	Specifies the channel number on which the setting is to be queried.
 * @param[out]	sclDest	Address of the memory location at which to place the query result (volts).
 * @return				Error status.
 */
extern Uint16 adcGetVScale (Uint16 chnl, float32 * sclDest);

/** Queries the over current protection setting for the specified channel.
 * @param[in]	chnl	Specifies the channel number on which the setting is to be queried.
 * @param[out]	ocpDest	Address of the memory location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 adcGetOcp (Uint16 chnl, float32 * ocpDest);

/** Queries the over current protection setting for the specified channel.
 * @param[in]	chnl	Specifies the channel number on which the setting is to be queried.
 * @param[out]	ovpDest	Address of the memory location at which to place the query result (volts).
 * @return				Error status.
 */
extern Uint16 adcGetOvp (Uint16 chnl, float32 * ovpDest);

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

#ifdef DEBUG_ADC
	extern volatile float32 guiIRead[NUM_CHNLS + 1];
	extern volatile float32 guiVRead[NUM_CHNLS + 1];
#endif

/*========= SYMBOLS DEFINED BY DP LIB =========*/
extern void ADC_SOC_CNF(int16 ChSel[], int16 Trigsel[], int16 ACQPS[], int16 IntChSel, int16 mode);

#endif /* ADC_H_ */
