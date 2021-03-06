/**
 * @file Adc.h
 *
 * @brief ADC functions.
 *
 */

#ifndef ADC_H_
#define ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

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

//** Configures ADC SOC for ADC macro */
//extern void adcSocConfig (void);

/** Configures the ADC's SOCs then calls pwmSocConfigure().
 *  - SHOULD be run after initPWM().
 *  - SHOULD be run before DPL_INIT().
 */
extern void initAdc (void);

/*============== Load n ==============*/
/** Queries the most recent current reading from the specified load's associated ADC.
 * @param[in]	load		Specifies the load on which the reading is to be queried.
 * @param[out]	dcCurrent	Address of the memory location at which to place the query result (amps).
 * @return					Error status.
 */
extern uint16_t getLoadCurrent (loadStage load, float * dcCurrent);

/** Queries the most recent voltage reading from the specified load's associated ADC.
 * @param[in]	load		Specifies the load on which the reading is to be queried.
 * @param[out]	dcVoltage	Address of the memory location at which to place the query result (volts).
 * @return					Error status.
 */
extern uint16_t getLoadVoltage (loadStage load, float * dcVoltage);

/*============== DC Mid ==============*/
/** Queries the most recent current reading from the DC Mid's associated ADC.
 * @param[out]	dcCurrent	Address of the memory location at which to place the query result (amps).
 * @return					Error status.
 */
extern uint16_t getDcMidCurrent (float * dcCurrent);

/** Queries the most recent voltage reading from the DC Mid's associated ADC.
 * @param[out]	dcVoltage	Address of the memory location at which to place the query result (volts).
 * @return					Error status.
 */
extern uint16_t getDcMidVoltage (float * dcVoltage);

/*=============== DC HV ==============*/
/** Queries the most recent voltage reading from the DC HV's associated ADC.
 * @param[out]	dcVoltage	Address of the memory location at which to place the query result (volts).
 * @return					Error status.
 */
extern uint16_t getDcHvVoltage (float * dcVoltage);

/*================ AC ================*/


/*========= SYMBOLS DEFINED BY DP LIB =========*/
extern void ADC_SOC_CNF(int16 ChSel[], int16 Trigsel[], int16 ACQPS[], int16 IntChSel, int16 mode);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* ADC_H_ */
