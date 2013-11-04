/**
 * @file Timers.h
 *
 * @brief Real and virtual timer functions.
 *
 * These functions should be run as part of the state machine setup.
 *
 * @sa StateMachine.h
 *
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============= GLOBAL FUNCTIONS ==============*/
#ifdef VTIMERS
	extern int16 VTimer0[4];	/**< First set of virtual timers. */
	extern int16 VTimer1[4];	/**< Second set of virtual timers. */
	extern int16 VTimer2[4];	/**< Third set of virtual timers. */

	/** Sets up the virtual timers for use in the state machine.
	 * This should be called as part of the state machine initialisation
	 * and before the real timers are setup.
	 * @sa smInit()
	 */
	extern void timersSetupVirtual (void);
#endif

/** Sets up the real timers that run the state machine
 * This should be called as part of the state machine initialisation.
 * @sa smInit()
 */
extern void timersSetupReal (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* TIMERS_H_ */
