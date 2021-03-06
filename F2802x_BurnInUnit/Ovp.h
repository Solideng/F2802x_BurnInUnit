/**
 * @file Ovp.h
 *
 * @brief Over-voltage protection functions.
 *
 * These functions require that the relevant
 * relevant measurement scales be set before
 * use.
 *
 */

#ifndef OVP_H_
#define OVP_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LOAD1_OVP_TRIP 	1	/**< OVP flag register load 1 bit. */
#define LOAD2_OVP_TRIP 	2	/**< OVP flag register load 2 bit. */
#define LOAD3_OVP_TRIP 	4	/**< OVP flag register load 3 bit. */
#define LOAD4_OVP_TRIP 	8	/**< OVP flag register load 4 bit. */
#define DCMID_OVP_TRIP 	16	/**< OVP flag register DC MID bit. */
#define DCHV_OVP_TRIP 	32	/**< OVP flag register DC HV bit. */
#define AC_OVP_TRIP 	64	/**< OVP flag register AC bit. */

extern uint16_t ovpFlagRegister;	/** < OVP flag register. Bits are set to indicate an OVP condition has been found. */

/*============== Load n ==============*/
/** Checks the voltage reading of the specified load against the load OVP limit.
 * Raises the load OVP flag if the reading is above the limit.
 * @param[in]	load	Specifies the load on which the reading is to be tested.
 * @return 				Error status
 */
extern uint16_t checkLoadOvp (loadStage load);

/** Queries the state of the OVP flag for the specified load.
 * @param[in]	load	Specifies the load on which to check the flag.
 * @return				True if OVP flag has been raised.
 */
extern uint16_t getLoadOvpState (loadStage load);

/** Clears the OVP state for the specified load.
 * @param[in]	load	Specifies the load for which the OVP state is to be cleared.
 * @return				Error status.
 */
extern uint16_t clearLoadOvp (loadStage load);

/*============== DC Mid ==============*/
//** Sets the over voltage protection limit for the DC Mid stage.
// * @param[in]	dcLevel	Specifies the DC value to be applied (volts).
// * @return				Error status.
// */
//extern Uint16 setDcMidOvpLevel (float32 dcLevel);
//
//** Queries the over voltage protection level for the DC Mid stage.
// * @param[out]	dcLevel	Pointer to location at which to place the query result (volts).
// * @return				Error status.
// */
//extern Uint16 getDcMidOvpLevel (float32 *dcLevel);

/** Trips the DC Mid OVP.
 * This provides an OVP function for the trip zone ISR to call when the DC comparator is triggered.
 * @return 				Error status
 */
extern uint16_t tripDcMidOvp (void);

/** Queries the state of the DC Mid OVP flag.
 * @return	True if OVP flag has been raised.
 */
extern uint16_t getDcMidOvpState (void);

/** Clears the DC Mid stage OVP state.
 * @return				Error status.
 */
extern uint16_t clearDcMidOvp (void);

/*=============== DC HV ==============*/
/** Sets the over voltage protection limit for the DC HV stage.
 * @param[in]	dcLevel	Specifies the DC value to be applied (volts).
 * @return				Error status.
 */
extern uint16_t setDcHvOvpLevel (float dcLevel);

/** Queries the over voltage protection level for the DC HV stage.
 * @param[out]	dcLevel	Pointer to location at which to place the query result (volts).
 * @return				Error status.
 */
extern uint16_t getDcHvOvpLevel (float *dcLevel);

/** Checks the voltage reading of the DC HV stage against the DC HV OVP limit.
 * Raises the DC HV OVP flag if the reading is above the limit.
 * @return 				Error status
 */
extern uint16_t checkDcHvOvp (void);

/** Queries the state of the DC HV OVP flag.
 * @return	True if OVP flag has been raised.
 */
extern uint16_t getDcHvOvpState (void);

/** Clears the DC HV stage OVP state.
 * @return				Error status.
 */
extern uint16_t clearDcHvOvp (void);

/*================ AC ================*/
/** Sets the over voltage protection limit for the AC stage.
 * @param[in]	pkLevel	Specifies the peak value to be applied (volts).
 * @return				Error status.
 */
extern uint16_t setAcOvpLevel (float pkLevel);

/** Queries the over voltage protection level for the AC stage.
 * @param[out]	pkLevel	Pointer to location at which to place the query result (volts).
 * @return				Error status.
 */
extern uint16_t getAcOvpLevel (float *pkLevel);

/** Checks the voltage reading of the AC stage against the AC OVP limit.
 * Raises the AC OVP flag if the reading is above the limit.
 * @return 				Error status
 */
extern uint16_t checkAcOvp (void);

/** Queries the state of the AC OVP flag.
 * @return	True if OVP flag has been raised.
 */
extern uint16_t getAcOvpState (void);

/** Clears the AC stage OVP state.
 * @return				Error status.
 */
extern uint16_t clearAcOvp (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* OVP_H_ */
