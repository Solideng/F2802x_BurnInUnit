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

#define LOAD1_OVP_TRIP 	1	/**< OVP flag register load 1 bit. */
#define LOAD2_OVP_TRIP 	2	/**< OVP flag register load 2 bit. */
#define LOAD3_OVP_TRIP 	4	/**< OVP flag register load 3 bit. */
#define LOAD4_OVP_TRIP 	8	/**< OVP flag register load 4 bit. */
#define DCMID_OVP_TRIP 	16	/**< OVP flag register DC MID bit. */
#define DCHV_OVP_TRIP 	32	/**< OVP flag register DC HV bit. */
#define AC_OVP_TRIP 	64	/**< OVP flag register AC bit. */

extern Uint16 ovpFlagRegister;	/** < OVP flag register. Bits are set to indicate an OVP condition has been found. */

/*============== Load n ==============*/
extern Uint16 checkLoadOvp (loadStage load);

extern Uint16 getLoadOvpState (loadStage load);

extern Uint16 clearLoadOvp (loadStage load);

/*============== DC Mid ==============*/

/*=============== DC HV ==============*/
extern Uint16 setDcHvOvpLevel (float32 dcLevel);

extern Uint16 getDcHvOvpLevel (float32 *dcLevel);

extern Uint16 checkDcHvOvp (void);

extern Uint16 getDcHvOvpState (void);

extern Uint16 clearDcHvOvp (void);

/*================ AC ================*/
extern Uint16 setAcOvpLevel (float32 pkLevel);

extern Uint16 getAcOvpLevel (float32 *pkLevel);

extern Uint16 checkAcOvp (void);

extern Uint16 getAcOvpState (void);

extern Uint16 clearAcOvp (void);

#endif /* OVP_H_ */
