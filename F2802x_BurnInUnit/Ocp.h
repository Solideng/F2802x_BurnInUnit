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

#define LOAD1_OCP_TRIP 	1	/**< OCP flag register load 1 bit. */
#define LOAD2_OCP_TRIP 	2	/**< OCP flag register load 2 bit. */
#define LOAD3_OCP_TRIP 	4	/**< OCP flag register load 3 bit. */
#define LOAD4_OCP_TRIP 	8	/**< OCP flag register load 4 bit. */
#define DCMID_OCP_TRIP 	16	/**< OCP flag register DC MID bit. */
#define DCHV_OCP_TRIP 	32	/**< OCP flag register DC HV bit. */
#define AC_OCP_TRIP 	64	/**< OCP flag register AC bit. */

extern Uint16 ocpFlagRegister;	/** < OCP flag register. Bits are set to indicate an OCP condition has been found. */

/*============== Load n ==============*/
/** Sets the over current protection level for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	dcLevel	Specifies the DC value to be applied (amps).
 * @return				Error status.
 */
extern Uint16 setLoadOcpLevel (loadStage load, float32 dcLevel);

/** Queries the over current protection level for the specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried.
 * @param[out]	dcLevel	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 getLoadOcpLevel (loadStage load, float32 * dcLevel);

/** Checks the current reading of the specified load against the load OCP limit.
 * Raises the load OCP flag if the reading is above the limit.
 * @param[in]	load	Specifies the load on which the reading is to be tested.
 * @return 				Error status
 */
extern Uint16 checkLoadOcp (loadStage load);

/** Queries the state of the OCP flag for the specified load.
 * @param[in]	load	Specifies the load on which to check the flag.
 * @return				True if OCP flag has been raised.
 */
extern Uint16 getLoadOcpState (loadStage load);

/** Clears the OCP state for the specified load.
 * @param[in]	load	Specifies the load for which the OCP state is to be cleared.
 * @return				Error status.
 */
extern Uint16 clearLoadOcp (loadStage load);


/*============== DC Mid ==============*/
/** Sets the over current protection limit for the DC stage.
 * @param[in]	dcLevel	Specifies the DC value to be applied (amps).
 * @return				Error status.
 */
extern Uint16 setDcMidOcpLevel (float32 dcLevel);

/** Queries the over current protection level for the DC stage.
 * @param[out]	dcLevel	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 getDcMidOcpLevel (float32 * dcLevel);

/** Checks the current reading of the DC stage against the DC OCP limit.
 * Raises the DC OCP flag if the reading is above the limit.
 * @return 				Error status
 */
extern Uint16 checkDcMidOcp (void);

/** Queries the state of the DC OCP flag.
 * @return	True if OCP flag has been raised.
 */
extern Uint16 getDcMidOcpState (void);

/** Clears the DC stage OCP state.
 * @return				Error status.
 */
extern Uint16 clearDcMidOcp (void);


/*================ AC ================*/
/** Sets the over current protection limit for the AC stage.
 * @param[in]	pkLevel	Specifies the peak value to be applied (amps).
 * @return				Error status.
 */
extern Uint16 setAcOcpLevel (float32 pkLevel);

/** Queries the over current protection level for the AC stage.
 * @param[out]	dcLevel	Pointer to location at which to place the query result (amps).
 * @return				Error status.
 */
extern Uint16 getAcOcpLevel (float32 * pkLevel);

/** Trips the AC OCP.
 * This provides an OCP function for the trip zone ISR to call when the AC comparator is triggered.
 * @return 				Error status
 */
extern Uint16 tripAcOcp (void);

/** Queries the state of the AC OCP flag.
 * @return	True if OCP flag has been raised.
 */
extern Uint16 getAcOcpState (void);

/** Clears the AC stage OCP state.
 * @return				Error status.
 */
extern Uint16 clearAcOcp (void);

#endif /* OCP_H_ */
