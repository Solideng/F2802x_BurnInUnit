/**
 * @file PhaseCtrl.h
 *
 * @brief Signal generator phase (ACFBPHASE) control function.
 *
 * @warning
 * This file is included by the file ISR.asm and thus any dependencies this file has
 * should also be included there (e.g. PeripheralHeaderIncludes.h).
 *
 */
#ifndef PHASECTRL_H_
#define PHASECTRL_H_

/** Updates GPIO19 based on state of *PHASE_CTRL_In terminal.
 * Expects 0 (GPIO19 set) or non-zero (GPIO19 cleared).
 * This is generally called by the DPL_ISR.asm
 */
extern void pcUpdate (void);

extern volatile int32 *PHASE_CTRL_In; /**< Phase control module signal input terminal. */

#endif /* PHASECTRL_H_ */
