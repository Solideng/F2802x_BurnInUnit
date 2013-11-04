/**
 * @file StateMachine.h
 *
 * @brief State machine functions.
 *
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
	extern circuitSection enableSection; /* Set to 0-7 for one state machine iteration to enable a section, set >7 for no action  */
	extern circuitSection disableSection; /* Set to 0-7 for one state machine iteration to disable a section, set >7 for no action */
#endif

/*============= GLOBAL FUNCTIONS ==============*/
/** Sets up the state machine (including timers) ready for use. */
extern void initStateMachine(void);

/** Runs the next iteration of the state machine.
 * Should be called from the main super-loop.
 */
extern void (*Alpha_State_Ptr)(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* STATEMACHINE_H_ */
