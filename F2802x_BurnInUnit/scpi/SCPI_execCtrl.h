/**
 * @file SCPI_execCtrl.h
 *
 * TODO: Currently the calling function should call
 * operationComplete() when execCtrl() has finished all
 * commands.
 *
 * The execution control block deals with coupled
 * parameters and queries that require a device action
 * before they can generate a @f$ \langle@f$ RESPONSE
 * MESSAGE @f$ \rangle@f$ . The execution control
 * determines when the device has gathered enough
 * information to execute a command. When the execution
 * control has gathered enough information to initiate a
 * device action, it sends an executable message element to
 * the device functions. The execution control will wait
 * for the resulting device actions to complete (sequential
 * command). A @f$ \langle@f$ PROGRM MESSAGE UNIT
 * @f$ \rangle@f$ is considered executed when it has been
 * parsed and all corresponding device operations have been
 * either completed or initiated.
 *
 * This file should not need to be edited by the user.
 *
 */

#ifndef SCPI_EXECCTRL_H_
#define SCPI_EXECCTRL_H_

#ifdef __cplusplus
	extern "C" {
#endif

/** Resets the execution controller. */
extern void resetExecCtrl(void);

/** Handles the calling of device specific functions
 * @param[in]	fnd		The SCPI command tree node found by the parser
 * @param[in]	reqstd	The lex'd and parsed components of the program message
 * @return				Error status
 */
extern Uint16 execCtrl(treeNode * fnd, lexedItem * reqstd);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_EXECCTRL_H_*/
