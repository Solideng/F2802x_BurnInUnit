/**
 * @file SCPI_inputBuffer.h
 *
 * This file provides the SCPI input buffer and
 * the functions that are used to access and
 * manage it
 *
 * The input buffer stores DABs, GET messages, and END
 * messages. The input buffer then delivers these messages
 * to the parser in the order that they were received from
 * the I/O control.
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_INPUTBUFFER_H_
#define SCPI_INPUTBUFFER_H_

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef INBUFF_LENGTH
	#error "The SCPI input buffer length 'INBUFF_LENGTH' has not been defined (should be in scpi.h)"
#endif
extern void resetIBuff (void);			/* Resets the input buffer and any saved state */

extern void clearIBuff (Uint16 from);	/* Clears the input buffer from the point indicated, on to the end */

extern Uint16 pushIBuff (char * src);	/* Push a program onto the input buffer.
										 * This would normally be called by the application's
										 *  incoming communications handling functions and is
										 *  the main data input point for the SCPI parser
										 */

extern Uint16 popIBuff (char * dest);	/* Pop a program off of the input buffer */

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_INPUTBUFFER_H_*/
