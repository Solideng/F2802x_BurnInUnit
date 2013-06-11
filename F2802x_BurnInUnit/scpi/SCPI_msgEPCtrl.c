#include "scpi.h"

static void clearMsgs (void);
static void unterminatedAction (void);
static void interruptedAction (void);
static void resetScpi (void);
//union MEP_MSGS_REG MepMsgs = {0};
msg msgs = {false};

// TODO Functions that receive data in (E.g. SCI Rx), should set msgs.bav when a byte is received
typedef enum states {
	idle = 0,
	read,
	query,
	send,
	response,
	done,
	deadlock
} states;
static states mecState = idle;
static bool resetDone = false;
static Uint16 execResult = 0;
static treeNode * treeLeaf = NULL;
static lexedItem * lexMsgUnit = NULL;
static char programMsg[INBUFF_LENGTH] = {0};
static bool err = false;

static void clearMsgs (void) {
	/* Use with caution, this function clears all messages,
	 * care must be taken to reassign non-false defaults after use
	 */
	msgs.brq = false;
	msgs.bav = false;
	msgs.get = false;
	msgs.dcas = false;
	msgs.rmtSent = false;
	msgs.ibEmpty = false;
	msgs.ibFull = false;
	msgs.eom = false;
	msgs.query = false;
	msgs.pIdle = false;
	msgs.pBlocked = false;
	msgs.ecIdle = false;
	msgs.ecBlocked = false;
	msgs.pon = false;
	msgs.rfBlocked = false;
	msgs.oqFull = false;
	msgs.oqEmpty = false;
//	msgs.ibBlocked = false;
	msgs.reqt = false;
	msgs.reqf = false;
	msgs.ibEom = false;
}

Uint16 scpiInit (Uint16 (*IOTx)(char data)) {
	if (IOTx == NULL)
		return 1;
	if (setTxOQueue(IOTx) != 0)
		return 2;
	resetScpi();
	buildSCPITree();
	// Need to clear STB.PON bit?
	mecState = idle;
	return 0;
}

static void resetScpi (void) {
	Uint16 i;
	clearMsgs();
	resetIBuff();
	resetOQueue();
	resetParser();
	resetExecCtrl();
	resetRespFmtr();
	for(i = 0; i < INBUFF_LENGTH; i++)
		programMsg[i] = 0;
	lexMsgUnit = NULL;
	treeLeaf = NULL;
	execResult = 0;
	resetDone = true;
}

static void unterminatedAction (void) {
	/* 6.3.2.2 	Controller attempts to read a <RESPONSE MESSAGE> from the device without having first sent a
	 * 			complete query message to the device.
	 * 			I.E. Addressed to talk with nothing to say.
	 */
	queryError();
	resetParser();
	msgs.brq = false;
}

static void interruptedAction (void) {
	/* 6.3.2.3	Device interrupted by a new <PROGRAM MESSAGE> before it finishes sending a <RESPONSE MESSAGE>
	 */
	queryError();
	resetOQueue();
	resetExecCtrl();
	resetRespFmtr();
	lexMsgUnit = NULL;
	treeLeaf = NULL;
	execResult = 0;
	msgs.brq = false;
	msgs.eom = false;
	msgs.query = false;
}

void scpi (void) {

	if (msgs.dcas || msgs.pon) {
		if (!resetDone)
			resetScpi();					/* INITIALIZE action */
		mecState = idle;
		
	} else if (mecState == idle) {
		// When comms Rx is ready to push IB data call pushIBuff() and set bav TRUE //6.1.4.2.4
									/* Check messages to see if state change is needed, else do nothing */
		if (msgs.bav || msgs.get || !msgs.ibEmpty) {
			mecState = read; 		/* If data is available, move to read state */
		} else if (msgs.brq && !msgs.bav && !msgs.get && msgs.ibEmpty) {
			unterminatedAction();	/* Asked to talk with nothing to say. */
			operationComplete();
			mecState = idle;
		}

	} else if (mecState == read) {
		if (msgs.ibEom) {								/* The input buffer has detected that a full message has 
														 * now been pushed.
														 */
			err = false;
			msgs.ibEom = false;
			msgs.bav = false;		// Used differently from spec as need to allow MEC to move from idle to read.
			if (popIBuff(programMsg) == 0) {
														/* If there isn't already a popped <PROGRAM MESSAGE> then pop 
														 * one from the input buffer. 
														 */
				msgs.pIdle = false;						/* 6.1.6.2.4 The parser shall set p-idle FALSE at all 
														 * other times. 
														 */
				if (lexProgramMsg(&lexMsgUnit, programMsg) != 0) {
														/* Lex the first <PROGRAM MESSAGE UNIT> from the popped
														 * <PROGRAM MESSAGE>.
														 */
					if (lexMsgUnit->parameterType == pQuery)
						msgs.query = true;				/* 6.1.6.2.3 set query TRUE when it receives a valid 
														 * <QUERY MESSAGE UNIT> 
														 */
					treeLeaf = parseMsgUnit(lexMsgUnit);/* Parse the lex'd  <PROGRAM MESSAGE UNIT>. */
					msgs.pIdle = true;					/* 6.1.6.2.4 The parser shall set p-idle TRUE when the 
														 * parser has parsed all prior messages 
														 */
					if (treeLeaf != NULL) {				/* Check if the parser found a tree leaf. */
						msgs.pBlocked = true;			/* 6.1.6.2.5 the parser shall set p-blocked TRUE when the
														 * parser is waiting for the execution control to finish.
														 */
						msgs.ecIdle = false;			/* 6.1.1.7.2.2 */
						execResult = execCtrl(treeLeaf, lexMsgUnit); /* Execute the found tree leaf command. */
						msgs.ecIdle = true;				/* 6.1.1.7.2.2 */
						msgs.ecBlocked = false;			/* 6.1.7.2.3 */
						msgs.pBlocked = false;			/* 6.1.6.2.5 the parser shall set p-blocked FALSE at all 
														 * other times 
														 */
					}
				} else {
					msgs.pIdle = true;					/* 6.1.6.2.4 The parser shall set p-idle TRUE when the 
														 * parser has parsed all prior messages 
														 */
				}
				
				if ((msgs.query) && (lexMsgUnit != NULL)) {	/* If there is a query check there are no more PMUs in the PM ***** */
				 	if (lexProgramMsg(&lexMsgUnit, 0) != NULL)
						err = true; 					/* There is message data after a query. */
				} else {
					msgs.query = false;
														/* Lex subsequent <PROGRAM MESSAGE UNIT> from the popped 
														 * <PROGRAM MESSAGE>. 
														 */
					while ((!err) && (lexProgramMsg(&lexMsgUnit, 0) != NULL)) {
						if (lexMsgUnit->parameterType == pQuery)
							msgs.query = true;			/* 6.1.6.2.3 The parser shall set query FALSE when
														 * receives any other DAB or GET messages from the
														 * input buffer.
											 			 */
						treeLeaf = parseMsgUnit(lexMsgUnit);/* Parse the lex'd  <PROGRAM MESSAGE UNIT>. */
						msgs.pIdle = true;				/* 6.1.6.2.4 The parser shall set p-idle TRUE when the 
														 * parser has parsed all prior messages 
														 */
						if (treeLeaf != NULL) {			/* Check if the parser found a tree leaf. */
							msgs.pBlocked = true;		/* 6.1.6.2.5 the parser shall set p-blocked TRUE when the
														 * parser is waiting for the execution control to finish
														 */
							msgs.ecIdle = false;		/* 6.1.1.7.2.2 */
							execResult = execCtrl(treeLeaf, lexMsgUnit); /* Execute the found tree leaf command. */
							msgs.ecIdle = true;			/* 6.1.1.7.2.2 */
							msgs.ecBlocked = false;		/* 6.1.7.2.3 */
							msgs.pBlocked = false;		/* 6.1.6.2.5 the parser shall set p-blocked FALSE at all
														 * other times 
														 */
						}
						if ((msgs.query) && (lexMsgUnit != NULL)) {
				 			if (lexProgramMsg(&lexMsgUnit, 0) != NULL)
								err = true;				/* There is message data after a query. */
						}
					}
				}
				if (execResult != 0)
					msgs.query = false;
			}
		}
		if (msgs.eom && !msgs.query) {				/* End of message detected. */
			msgs.eom = false;
			mecState = idle;
			operationComplete();
		} else if (msgs.query) {					/* Query detected. */
			mecState = query;
		} else if (msgs.brq && !msgs.bav && !msgs.get && msgs.ibEmpty) {
			unterminatedAction();					/* Addressed to talk with nothing to say. */
			mecState = idle;
		}

	} else if (mecState == query) {
		respFmtr();									/* Format the device function response. */
		if (msgs.eom && !msgs.bav && !msgs.get && msgs.ibEmpty) {
			mecState = response;					/* End of message and query (get). */
			msgs.query = false;
		//} else if (msgs.brq && !msgs.eom) {		/* Addressed to talk and not end of message yet. */
		//	mecState = send;
		} else if (msgs.eom && (msgs.bav || msgs.get || !msgs.ibEmpty)) {
			interruptedAction();						/* New message before curent one done. */
			mecState = read;
		} else if ((msgs.bav || msgs.get) && msgs.ibFull && msgs.pBlocked && msgs.ecBlocked && msgs.rfBlocked && msgs.oqFull) {
			queryError();			/* New command while all functions are full - DEADLOCK action error */
			msgs.brq = false;
			mecState = deadlock;
			// clear opQ & reset rspFmtr
		}

	} else if (mecState == deadlock) {
		if (msgs.eom) {								/* End of message detected. */
			msgs.brq = false;
			mecState = idle;
			operationComplete();
		}

	} else if (mecState == send) {
		// TODO
		//-///////// SEND STATE NOT USED, CHANGE MSGS ACCORDINGLY /////////////
		if (msgs.eom && !msgs.bav && !msgs.get && msgs.ibEmpty) {
			mecState = response;					/* End of message and no more data. */
		} else if (msgs.bav || msgs.get || (msgs.eom && msgs.ibEmpty)) {
			interruptedAction();					/* New message before current one done. */
			mecState = read;
		} else if (!msgs.eom && !msgs.bav && !msgs.get && msgs.ibEmpty) {
			unterminatedAction();					/* Addressed to talk with nothing to say. */
			operationComplete();
			mecState = idle;
		}

	} else if (mecState == response) {
		msgs.brq = true;
		popOQueue();
		if (msgs.rmtSent) {							/* Finished sending response. */
			mecState = done;
		} else if (msgs.bav || msgs.get) {			/* New message before current one done. */
			interruptedAction();
			mecState = read;
		}

	} else if (mecState == done) {
		operationComplete();
		if (msgs.bav || msgs.get) {
			msgs.brq = false;
			mecState = read;
		}

	} else {
		// move to deadlock?
	}

	// when ready to pop OQ data set brq TRUE //6.1.4.2.3

	// if dcas TRUE clear IB + OQ, reset parser, exec Ctrl + RFmttr, go to IDLE state, set dcas FALSE

	// if pon TRUE clear IB + OQ, reset parser // 6.1.5.3, 6.1.6.1.2,

	//pop pq until ib-empty	//6.1.5.2.1

	// if parser return error (cmd error)  discard any further input //6.1.6.1.1

	// p-idle TRUE when parser fin //6.1.6.2.4


	return;
}
