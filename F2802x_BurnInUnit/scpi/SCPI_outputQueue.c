#include "scpi.h"

static char outputQueue[OQUEUE_LENGTH] = {0};
static int pushQP = 0;
static int popQP = 0;
Uint16 (*IOTxCallback)(char data);

void resetOQueue (void) {
	popQP = 0;
	pushQP = 0;
	clearOQueue(0);
}

Uint16 setTxOQueue (Uint16 (*IOTx)(char data)) {
	if (IOTx == NULL)
		return 1;
	IOTxCallback = IOTx;
	return 0;
}

void clearOQueue (Uint16 from) {
	Uint16 i = from;
	while (i < OQUEUE_LENGTH)
		outputQueue[i++] = 0;
	ScpiRegs.Status.bit.MAV = 0;	/* 6.1.10.2.1 MAV shall be FALSE when the output queue is empty */
	msgs.oqFull = false;			/* 6.1.10.2.2 oq-full shall be FALSE at all other times */
	if (!from)
		msgs.oqEmpty = true;		/* 6.1.10.2.3 oq-empty shall be TRUE when when the output queue is empty */
}

bool dataOnOQueue (void) {
	/* Should not depend on STB.MAV as this may be used to update MAV */
	return ( pushQP == 0 ? false : true);
}

Uint16 pushOQueue (char * src) {
	Uint16 programStart = pushQP;
									/* If there is already a <RESPONSE MESSAGE> on the queue then overwrite its
									 * <RESPONSE MESSAGE TERMINATOR>. 
									 */
	if (pushQP > 0) {
		pushQP -= 2;
	}
									/* Copy data from src to the output queue until the src is fully copied or 
									 * the queue is full. 
									 */
	while ((pushQP < OQUEUE_LENGTH) && (*src != 0)) {
		outputQueue[pushQP++] = *src++;
		while ((*src == NL_MSG_TERM) || (*src == END_MSG_TERM)) {
			src++;					/* Skip any <RESPONSE MSSAGE UNIT SEPARTOR> in the source */
		}
	}
	
	if ((pushQP > 0) && (pushQP < OQUEUE_LENGTH)) {
									/* If there is space on the queue when the src is pushed add a 
									 * <RESPONSE MESSAGE TERMINATOR>. 
									 */
		outputQueue[pushQP++] = NL_MSG_TERM;
		outputQueue[pushQP++] = END_MSG_TERM;
		msgs.oqFull = false; 		/* 6.1.10.2.2 oq-full shall be FALSE at all other times */
	} else if (pushQP > 0) {		/* If the src filled the queue totally terminate it with a string terminator. */
		outputQueue[OQUEUE_LENGTH-1] = STR_NULL;
		if (*src != '\0') {			/* If the src was too long for queue reomve any part of src that may already
									 * be on the queue. 
									 */
			pushQP = programStart;
			clearOQueue(pushQP);
			return 1;				/* Calling function should check returned value and set appropriate error. */
		}
		msgs.oqFull = true;			/* 6.1.10.2.2 oq-full shall be TRUE when the output queue is full */
	}
	msgs.oqEmpty = false;			/* 6.1.10.2.3 The oq-Empty message shall be FALSE at all other times */
	ScpiRegs.Status.bit.MAV = 1;	/* 6.1.10.2.1 As long as the output queue contains one or more bytes, MAV shall be TRUE */
	return 0;
}

Uint16 popOQueue (void) {
	unsigned char err = 0;
	
	// TODO: Disable int that may pop data for this 'if' otherwise data popped after if but before Q-clear will be orphaned !!!
							/* Pop data from queue */
	if ((popQP < OQUEUE_LENGTH)) {	// TODO reorgamnise msgs here
		if (msgs.brq && !msgs.oqEmpty) {
							/* 6.1.4.1.1 When brq is TRUE and oq-empty is FALSE, the I/O control shall remove one
							 * byte from the output queue, set brq FALSE, and send the byte to the controller
							 */
			msgs.brq = false;
			msgs.rmtSent = false;	/* 6.1.4.2.7 Receipt of TRUE bav or brq messages shall cause the I/O control
									 * set the RMT-sent message FALSE.
									 */
			err = IOTxCallback(outputQueue[popQP]);
			if (err) {
				err = err; //TODO do something for the error.
			}
			else {
				popQP++;
			}
		}
	}
	if ((popQP >= OQUEUE_LENGTH) || (outputQueue[popQP] == STR_NULL)) {
		popQP = 0;			/* Full queue has been read */
		pushQP = 0;
		clearOQueue(0); 	/* Also clears STB.MAV */
		msgs.rmtSent = true;
		return 1;
	}
	return 0;
}
