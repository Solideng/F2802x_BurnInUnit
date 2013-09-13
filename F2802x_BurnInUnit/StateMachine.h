/**
 * @file StateMachine.h
 *
 * @brief State machine functions.
 *
 */

#ifndef F2802X_BURNINUNIT_STATEMACHINE_H_
#define F2802X_BURNINUNIT_STATEMACHINE_H_

/*============= GLOBAL FUNCTIONS ==============*/
/** Sets up the state machine (incl. timers) ready for use. */
extern void smInit (void);

/** Runs the next iteration of the state machine.
 * Should be called from the main super-loop.
 */
extern void (*Alpha_State_Ptr)(void);

#endif /* F2802X_BURNINUNIT_STATEMACHINE_H_ */
