/*
 * SCPI_respFmtr.c
 *
 *  Created on: 18 May 2013
 *      Author: Toby
 */

#include "scpi.h"

struct rspnsStruct rMsg;

static bool rstFlg = false;

void resetRespFmtr (void) {
	// reset for when either pon or dcas are TRUE
	// TODO add msgs clear/set
	rstFlg = true;
	rMsg.dataSize = 0;
	return;
}

Uint16 respond (void *data, rtype rspnsType, bool isRspnsEnd) {
	static Uint16 i = 0;
	static rtype typeChk = typeNotSet;
	
	if (rstFlg) {				/* If there is a reset pending, clear the local static context variables. */
		i = 0;
		typeChk = typeNotSet;
		rstFlg = false;
	}
								/* Check there is room available for another <RESPONSE DATA> in this
								 * <RESPONSE MESSAGE UNIT>.
								 */
	if (i >= MAX_NUM_RDATA) {
		i = 0;
		//TODO response error - should be handled by MEP (Unterminated action).
		return 1;
	}
								/* Ensure <RESPONSE DATA> type is only set once per <RESPONSE MESSAGE UNIT> and
								 * all <RESPONSE DATA> in a single <RESPONSE MESSAGE UNIT> is of the same type.
								 */
	if (typeChk == typeNotSet) {
		typeChk = rspnsType;
		rMsg.rspnsType = rspnsType;
	} else if (typeChk != rspnsType) {
		// TODO response error - change in msgs here should mean MEP interrupted action - queryError
		return 2;
	}

	switch (typeChk) {			/* Save the <RESPONSE DATA> to the <RESPONSE MESSAGE UNIT> data. */
		case Integer:
			rMsg.rUnit[i++].intRData = *((int16*)data);
			break;
		case Double:
			rMsg.rUnit[i++].dbleRData = *((double*)data);
			break;
		case String:
			rMsg.rUnit[i].strRData[0] = STR_NULL;
			if (strcspn((char*)data, rMsg.rUnit[i].strRData) >= MAX_RDATA_STRING_SIZE)
				return 2;
			strcpy(rMsg.rUnit[i++].strRData, (char*)data);
			break;
		default:
			//TODO response error
			return 3;
	}
	rMsg.dataSize = i;		/* Update <RESPONSE DATA> size record. */
	
	if (isRspnsEnd) {
		i = 0;					/* Reset <RESPONSE DATA> index. */
		typeChk = typeNotSet;	/* Reset <RESPONSE DATA> type check */
		//TODO do something to tell response formatter all data has been got. msgs?
	}
	return 0;
}

Uint16 respFmtr (void) {
	char buf[20] = {0}, delim[2] = "";
	Uint16 i = 0;
	
	while (i < rMsg.dataSize) {
								/* Convert each <RESPONSE DATA> in the <RESPONSE MESSAGE UNIT> data to string form. */
		switch (rMsg.rspnsType) {
			case Integer:
				itoa(rMsg.rUnit[i++].intRData, buf, 10);
				break;
			case Double:
				dtoa(rMsg.rUnit[i++].dbleRData, buf);
				break;
			case String:
				strcpy(buf, rMsg.rUnit[i++].strRData);
				break;
			default:
				return 1;
		}
		if (i < rMsg.dataSize) {
			*delim = DATA_SEP;
			strcat(buf, delim);	/* Delimit each <RESPONSE DATA> with a <RESPONSE DATA SEPARATOR>. */
		} else {
			*delim = UNIT_SEP;
			strcat(buf, delim);	/* Terminate the <RESPONSE MESSAGE UNIT> with a <RESPONSE MESSAGE UNIT SEPARTOR> */
		}
		if (msgs.oqFull) {
			msgs.rfBlocked = true;
			return 2;
		} else {
			msgs.rfBlocked = false;
			pushOQueue(buf); 	/* Send the <RESPONSE DATA> to the output queue */
		}
	}
	return 0;
}
