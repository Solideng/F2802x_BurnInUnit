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



#endif /* OVP_H_ */
