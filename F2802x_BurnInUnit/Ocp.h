/**
 * @file Ocp.h
 *
 * @brief Over-current protection functions.
 *
 * These functions require that the relevant
 * relevant measurement scales be set before
 * use.
 *
 */

#ifndef OCP_H_
#define OCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LOAD1_OCP_TRIP 	1	/**< OCP flag register load 1 bit. */
#define LOAD2_OCP_TRIP 	2	/**< OCP flag register load 2 bit. */
#define LOAD3_OCP_TRIP 	4	/**< OCP flag register load 3 bit. */
#define LOAD4_OCP_TRIP 	8	/**< OCP flag register load 4 bit. */
#define DCMID_OCP_TRIP 	16	/**< OCP flag register DC MID bit. */
#define DCHV_OCP_TRIP 	32	/**< OCP flag register DC HV bit. */
#define AC_OCP_TRIP 	64	/**< OCP flag register AC bit. */

extern uint16_t ocpFlagRegister;	/** < OCP flag register. Bits are set to indicate an OCP condition has been found. */

/*============== Load n ==============*/
/** Sets the over current protection level for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	dcLevel	Specifies the DC value to be applied (amps).
 * @return				Error status.
 */
extern uint16_t setLoadOcpLevel (loadStage load, float dcLevel);

/** Queries the over current protection level for the specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried.
 * @param[out]	dcLevel	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern uint16_t getLoadOcpLevel (loadStage load, float * dcLevel);

/** Checks the current reading of the specified load against the load OCP limit.
 * Raises the load OCP flag if the reading is above the limit.
 * @param[in]	load	Specifies the load on which the reading is to be tested.
 * @return 				Error status
 */
extern uint16_t checkLoadOcp (loadStage load);

/** Queries the state of the OCP flag for the specified load.
 * @param[in]	load	Specifies the load on which to check the flag.
 * @return				True if OCP flag has been raised.
 */
extern uint16_t getLoadOcpState (loadStage load);

/** Clears the OCP state for the specified load.
 * @param[in]	load	Specifies the load for which the OCP state is to be cleared.
 * @return				Error status.
 */
extern uint16_t clearLoadOcp (loadStage load);


/*============== DC Mid ==============*/
/** Sets the over current protection limit for the DC stage.
 * @param[in]	dcLevel	Specifies the DC value to be applied (amps).
 * @return				Error status.
 */
extern uint16_t setDcMidOcpLevel (float dcLevel);

/** Queries the over current protection level for the DC stage.
 * @param[out]	dcLevel	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern uint16_t getDcMidOcpLevel (float * dcLevel);

/** Checks the current reading of the DC stage against the DC OCP limit.
 * Raises the DC OCP flag if the reading is above the limit.
 * @return 				Error status
 */
extern uint16_t checkDcMidOcp (void);

/** Queries the state of the DC OCP flag.
 * @return	True if OCP flag has been raised.
 */
extern uint16_t getDcMidOcpState (void);

/** Clears the DC stage OCP state.
 * @return				Error status.
 */
extern uint16_t clearDcMidOcp (void);


/*================ AC ================*/
/** Sets the over current protection limit for the AC stage.
 * @param[in]	pkLevel	Specifies the peak value to be applied (amps).
 * @return				Error status.
 */
extern uint16_t setAcOcpLevel (float pkLevel);

/** Queries the over current protection level for the AC stage.
 * @param[out]	pkLevel	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern uint16_t getAcOcpLevel (float * pkLevel);

/** Trips the AC OCP.
 * This provides an OCP function for the trip zone ISR to call when the AC comparator is triggered.
 * @return 				Error status
 */
extern uint16_t tripAcOcp (void);

/** Queries the state of the AC OCP flag.
 * @return	True if OCP flag has been raised.
 */
extern uint16_t getAcOcpState (void);

/** Clears the AC stage OCP state.
 * @return				Error status.
 */
extern uint16_t clearAcOcp (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* OCP_H_ */
