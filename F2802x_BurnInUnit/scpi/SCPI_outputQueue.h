/**
 * @file SCPI_outputQueue.h
 *
 * This file provides the SCPI response output queue
 * and the functions that are used to access and
 * manage it
 *
 * The output queue stores device-to-controller messages
 * until the controller reads then. The response formatter
 * places DAB and END messages into the output queue in
 * response to query commands. These bytes are removed from
 * the output queue as they are read by the controller.
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_OUTPUTQUEUE_H_
#define SCPI_OUTPUTQUEUE_H_

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef OQUEUE_LENGTH	/* Ensures user has set the output queue length define in scpi.h */
	#error "The response output queue length 'OQUEUE_LENGTH' has not been defined (should be in scpi.h)"
#endif
// TODO document below
extern Uint16 setTxOQueue (Uint16 (*IOTx)(char data));
extern void resetOQueue (void);

/** Clears the output program queue of any data from the point indicated, onwards to the end
 * @param[in]	from	The point at which to start clearing from, onto the end of the queue
 */
extern void clearOQueue (Uint16 from);

/** Determines if there is any data in the output queue
 * @return	Boolean value indicating if there is data on the output queue or not
 */
extern bool dataOnOQueue (void);

/** Pushes a response onto the output program queue
 * @param[in]	src		Location of the response string to be placed on output queue
 * @return				Error status
 */
extern Uint16 pushOQueue (char * src);

/** Pops a response off of the output queue
 * This would normally be called by the application's outgoing communications
 * handling functions and is the main data output point for the SCPI parser
 * //@param[out]  dest	Location for the popped data to be written to
 * @return				Error status
 */
extern Uint16 popOQueue (void);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_OUTPUTQUEUE_H_*/
