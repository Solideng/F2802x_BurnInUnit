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
extern Uint16 checkLoadOpp (loadStage load);

extern Uint16 getLoadOppState (loadStage load);

extern Uint16 clearLoadOpp (loadStage load);

/*================ AC ================*/
extern Uint16 checkAcOpp (void);

extern Uint16 getAcOppState (void);

extern Uint16 clearAcOpp (void);

#endif /* OPP_H_ */
