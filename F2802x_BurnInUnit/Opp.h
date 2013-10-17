/**
 * @file Opp.h
 *
 * @brief Over-power protection functions.
 *
 * These functions require that the relevant
 * relevant measurement scales be set before
 * use.
 *
 */

#ifndef OPP_H_
#define OPP_H_

#define LOAD1_OPP_TRIP 	1	/**< OPP flag register load 1 bit. */
#define LOAD2_OPP_TRIP 	2	/**< OPP flag register load 2 bit. */
#define LOAD3_OPP_TRIP 	4	/**< OPP flag register load 3 bit. */
#define LOAD4_OPP_TRIP 	8	/**< OPP flag register load 4 bit. */
#define DCMID_OPP_TRIP 	16	/**< OPP flag register DC MID bit. */
#define DCHV_OPP_TRIP 	32	/**< OPP flag register DC HV bit. */
#define AC_OPP_TRIP 	64	/**< OPP flag register AC bit. */

extern Uint16 oppFlagRegister;	/** < OPP flag register. Bits are set to indicate an OPP condition has been found. */

/*============== Load n ==============*/
/** Checks the power reading of the specified load against the load OPP limit.
 * Raises the load OPP flag if the reading is above the limit.
 * @param[in]	load	Specifies the load on which the reading is to be tested.
 * @return 				Error status
 */
extern Uint16 checkLoadOpp (loadStage load);

/** Queries the state of the OPP flag for the specified load.
 * @param[in]	load	Specifies the load on which to check the flag.
 * @return				True if OPP flag has been raised.
 */
extern Uint16 getLoadOppState (loadStage load);

/** Clears the OPP state for the specified load.
 * @param[in]	load	Specifies the load for which the OPP state is to be cleared.
 * @return				Error status.
 */
extern Uint16 clearLoadOpp (loadStage load);

/*================ AC ================*/
/** Checks the current reading of the AC stage against the AC OPP limit.
 * Raises the AC OPP flag if the reading is above the limit.
 * @return 				Error status
 */
extern Uint16 checkAcOpp (void);

/** Queries the state of the AC OPP flag.
 * @return	True if OPP flag has been raised.
 */
extern Uint16 getAcOppState (void);

/** Clears the AC stage OPP state.
 * @return				Error status.
 */
extern Uint16 clearAcOpp (void);

#endif /* OPP_H_ */
