#include "scpi.h"

static char inputBuffer[INBUFF_LENGTH] = {0}; /* Input buffer string */
static int pushPos = 0;	/* Current push position on buffer */
static int popPos = 0;	/* Current pop position on buffer */

void resetIBuff (void) {
	popPos = 0;
	pushPos = 0;
	clearIBuff(0);
}

void clearIBuff (Uint16 from) {
	Uint16 i = from;
							/* Set every character in the buffer, from the position indicated to the end, to 0 */
	while (i < INBUFF_LENGTH)
		inputBuffer[i++] = 0;
	msgs.ibFull = false;	/* 6.1.5.2.2 ib-full shall be FALSE at all other times */
	if (!from)
		msgs.ibEmpty = true;/* 6.1.5.2.1 ib-empty shall be TRUE when the parser attempts to remove an item from
		 	 	 	 	 	 * the input buffer and the input buffer is empty
		 	 	 	 	 	 */
}

Uint16 pushIBuff (char * src) {
	/* Pushes the ASCII string, located at *src, onto the input buffer until the string is fully pushed or the
	 * queue is full
	 */
	Uint16 programStart = pushPos;
	static char nwMsgFlg = true;

	msgs.bav = true;
			/* 6.1.4.1.2 When bav is TRUE and ib-full is FALSE, the I/O control shall set bav FALSE and then
			 * place the associated DAB and any accompanying END message into the the input buffer.
			 */
	if (msgs.bav && !msgs.ibFull) {
		msgs.rmtSent = false;	/* 6.1.4.2.7 Receipt of TRUE bav or brq messages shall cause the I/O control set
								 * the RMT-sent message FALSE.
								 */
		while ((pushPos < INBUFF_LENGTH) && (*src != 0)) {
			if ((*src != NL_MSG_TERM) && (*src != END_MSG_TERM)) {
				nwMsgFlg = true;
				inputBuffer[pushPos++] = *src++;	/* While there is unread data at src and space in the buffer,
													 * push data onto queue
													 */
			} else if (nwMsgFlg && ((*src == NL_MSG_TERM) || (*src == END_MSG_TERM))) {
													/* If a 'LF' or 'EOT' is recognised, make sure it is placed
													 *  as 'LF'
													 */
				inputBuffer[pushPos++] = NL_MSG_TERM;
				src++;
				msgs.ibEom = true;
				nwMsgFlg = false;
			} else {
				src++;				// I/O probably pushed more <PROGRAM MESSAGE TERMINATOR> characters after the ibEom was already set
				return 0;
			}
		}
		
		if ((pushPos > 0) && (pushPos < INBUFF_LENGTH)) {
											/* Terminate the pushed data with an 'LF' */
			msgs.ibFull = false;			/* 6.1.5.2.2 ib-full shall be FALSE at all other times */
		} else if (pushPos > 0) {			/* Queue full */
											/* Make sure the last character is a 0 */
			inputBuffer[INBUFF_LENGTH-1] = STR_NULL;

			if (*src != '\0') {				/* If the end of src hasn't been reached yet then src is too long for
			 	 	 	 	 	 	 	 	 * the buffer
			 	 	 	 	 	 	 	 	 */
				pushPos = programStart;		/* Clear all of this src off of the buffer without discarding any
											 * previously pushed data - allows a shorter src to be tried
											 */
				clearIBuff(pushPos);
				return 1;					/* Calling function should check this returned value and set an
											 * appropriate error
											 */
			}
			msgs.ibFull = true;				/* 6.1.5.2.2 ib-full shall be TRUE when the input buffer is full */
		}
		msgs.ibEmpty = false;				/* 6.1.5.2.1 ib-empty shall be FALSE at all other times */
	}
	return 0;
}

Uint16 popIBuff (char * dest) {
	/* Pops a program off of the buffer to *dest location */
	unsigned char i = 0;
	if (popPos != 0)
		popPos++;
		
	// TODO: Disable int that may pop data for this 'if' otherwise data
	// popped after if but before Q-clear will be orphaned !!! (May be taken care of by messages?)
	if ((popPos >= INBUFF_LENGTH) || (inputBuffer[popPos] == STR_NULL)) {
		resetIBuff();	/* Full buffer has been read so reset the buffer */
		return 1;
	}
						/* While there is data on the buffer and a 'LF' has not been detected, pop data from the
						 * buffer to dest
						 */
	while ((popPos < INBUFF_LENGTH) && (inputBuffer[popPos] != NL_MSG_TERM)) {
		*dest++ = inputBuffer[popPos++];
	}
	i = popPos;
						/* Check if there is any more data in the buffer that isnt just <MESSAGE TERMINATOR>s. */
	while ((i < INBUFF_LENGTH) && ((inputBuffer[i] == NL_MSG_TERM) || (inputBuffer[i] == END_MSG_TERM))) {
		i++;
	}
	if ((i >= INBUFF_LENGTH) || (inputBuffer[i] == STR_NULL)) {
		resetIBuff();	/* No more data in buffer so reset the buffer */
	}
	
	*dest = STR_NULL;	/* Terminate the popped data */
	return 0;
}
