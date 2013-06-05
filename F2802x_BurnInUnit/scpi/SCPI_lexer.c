#include "scpi.h"

lexedItem lRes = {0};

//static lexedItem clearResults = {0};
static lexedItem * resPtr = &lRes;

static void cleanPrg (char * msg);
static msgType getMsgType (char * msgUnit);
static tBool btof (char * boolStr);
static Uint16 lexPNums (char ** src, double * dest);
static bool isBaud(long number);
static clearResults (lexedItem * item);

/*
// Need good atof() before stdlib.h can be removed
int atoi( char* pStr ) {
	int iRetVal = 0;
	if (pStr) {
		while ( *pStr && *pStr <= '9' && *pStr >= '0' ) {
			iRetVal = (iRetVal * 10) + (*pStr - '0');
			pStr++;
		}
	}
	return iRetVal;
} 
*/

void resetLexer (void) {
	clearResults(&lRes);	/* 6.1.6.1.2 When the parser is reset, it shall interpret the next data byte it
	 	 	 	 	 	 	 * receives as the first byte of a <PROGRAM MESSAGE>.
	 	 	 	 	 	 	 */
	msgs.eom = false;		/* 6.1.6.2.2 The parser shall set the eom message FALSE when 1) it is reset. */
	msgs.query = false;		/* 6.1.6.2.3 The parser shall set query FALSE when it is reset. */
	msgs.pBlocked = false;	/* 6.1.6.2.5 the parser shall set p-blocked FALSE at all other times. */
}

static clearResults (lexedItem * item) {
	/* Clears a lexedItem structure ready for a new message. */
	Uint16 i;
	item->type = notFound;
	item->parameterType = pNone;
	for (i = 0; i < MAX_HEADERS; i++)
		item->headers[i] = 0;
	for(i = 0; i < MAX_MSG_UNITS; i++)
		item->msgUnit[i] = 0;
	for (i = 0; i < MAX_PARAMS; i++)
		item->pNums[i] = 0;
}

static bool isBaud (long number) {
	/* Check if the passed number is one of the valid standard Baud rates.
	 * Note that this will also return true for number thats are the same as standard Baud rates but which may not
	 * have been intended as Baud rates.
	 */
	switch(number) {
		case 110:
		case 300:
		case 600:
		case 1200:
		case 2400:
		case 4800:
		case 9600:
		case 14400:
		case 19200:
		case 28800:
		case 38400:
		case 56000:
		case 57600:
		case 115200:
		case 230400:
		case 460800:
		case 921600:
			return true;
		default:
			return false;
	}
}

lexedItem * (lexProgramMsg) (lexedItem ** dest, char * programMsg) {
	char *pCmd, * params[MAX_PARAMS], * msgUnit = 0, delims[] = {0, 0};
	static char *lastsPtr = NULL;
	Uint16 i = 0;
	//double pNums[MAX_PARAMS] = {0.0};
	
	clearResults(&lRes);		/* Clear any previous results */
	
	if (programMsg == 0) {		/* If <PROGRAM MESSAGE> is NULL, this is not the first iteration of the lexer for
								 * this <PROGRAM MESSAGE>.
								 */
		if (lastsPtr != NULL) {	/* There is still data available in the current <PROGRAM MESSAGE> */
			delims[0] = UNIT_SEP;
			msgUnit = strtok_r(NULL, delims, &lastsPtr);/* Get the next validly terminated <PROGRAM MESSAGE UNIT>
			 	 	 	 	 	 	 	 	 	 	 	 * from the <PROGRAM MESSAGE>.
			 	 	 	 	 	 	 	 	 	 	 	 */
			msgs.eom = false;	/* 6.1.6.2.2 The parser shall set the eom message FALSE when 2) it receives any
								 * other DAB or GET messages from the input buffer.
								 */
		}
		if (msgUnit == 0) { 	/* No data left in the current <PROGRAM MESSAGE>. */
			lastsPtr = NULL;
			msgs.eom = true;	/* 6.1.6.2.2 The parser shall set eom true when 1) it receives an END message or
								 * a sequence of data bytes making up a <PROGRAM MESSAGE TERMINATOR>.
								 */
			*dest = NULL;
			return NULL;
		}

	} else {					/* This is the first iteration of the lexer for this <PROGRAM MESSAGE>. */
		cleanPrg(programMsg);	/* Clean the <PROGRAM MESSAGE> string to make sure all characters are legal
		 	 	 	 	 	 	 * and converts all alpha chars to upper-case, as expected from this point on.
		 	 	 	 	 	 	 */
		delims[0] = UNIT_SEP;
		msgUnit = strtok_r(programMsg, delims, &lastsPtr);	/* Get the first <PROGRAM MESSAGE UNIT> from the
		 	 	 	 	 	 	 	 	 	 	 	 	 	 * <PROGRAM MESSAGE>.
		 	 	 	 	 	 	 	 	 	 	 	 	 	 */
		msgs.eom = false;		/* 6.1.6.2.2 The parser shall set the eom message FALSE when 2) it receives any
								 * other DAB or GET messages from the input buffer.
								 */
		msgs.query = false;		/* 6.1.6.2.3 The parser shall set query FALSE when receives any other DAB or GET
					 	 	 	 * messages from the input buffer.
					 	 	 	 */
		if (msgUnit == 0){ 		/* There was no validly terminated <PROGRAM MESSAGE UNIT> in the <PROGRAM MESSAGE>. */
			lastsPtr = NULL;
			*dest = NULL;
			return NULL;
		}
	}
	strcpy(lRes.msgUnit, msgUnit);						/* Save the found <PROGRAM MESSAGE UNIT> from the passed
														 * <PROGRAM MESSAGE> string to the result structure
														 * message unit member.
														 */
	lRes.type = getMsgType(lRes.msgUnit);				/* Find what type of <COMMAND/QUERY PROGRAM HEADER> the
	 	 	 	 	 	 	 	 	 	 	 	 	 	 * <PROGRAM MESSAGE UNIT> has, root, relative common,
	 	 	 	 	 	 	 	 	 	 	 	 	 	 * or emulation code.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	delims[0] = MNEM_SEP;
	breakString(lRes.msgUnit, lRes.headers, delims);	/* Break the <COMMAND/QUERY PROGRAM HEADER> into <program
														 *  mnemonic>s, i.e. at any <program mnemonic> separators.
														 */
	while (lRes.headers[i] != NULL) {					/* Go to the start of the last <program mnemonic> as this
														 * will be the leaf tree node mnemonic.
														 */
		i++;
	}
	i--;
	
	pCmd = lRes.headers[i];		/* Look at the last character in the last program header, if this is a valid
								 * <QUERY PROGRAM HEADER> this should be a <QUERY PROGRAM HEADER> suffix.
								 */
	while (*pCmd++ != 0);
	pCmd-=2;
	
	if (*pCmd == QUERY_IND) {	/* If the program header is a valid <QUERY PROGRAM HEADER> then there is no need
								 * to check for parameters so the parameter type is set to the 'system-use-only'
								 * pQuery type.
								 */
		lRes.parameterType = pQuery;
		*pCmd = 0;
	} else {
		pCmd = lRes.headers[i];  
								/* This doesn't have a valid <QUERY PROGRAM HEADER> suffix so treat it as a
								 * <COMMAND PROGRAM HEADER>.
								 */
		while ((*pCmd != HEADER_SEP) && (*pCmd != 0))
			pCmd++;				/* Scan the last <program mnemonic> and look for a valid <PROGRAM HEADER SEPARATOR>. */

		if (*pCmd == 0) {		/* If a valid <PROGRAM HEADER SEPARATOR> wasn't found, there is no <PROGRAM DATA>,
							 	 * i.e. there are no parameters, so its just a command.
							 	 */
			lRes.parameterType = pNone;
		} else {				/* A valid <PROGRAM HEADER SEPARATOR> was found. */
			*pCmd = 0;			/* Replace the <PROGRAM HEADER SEPARATOR> with a string terminator. */
			pCmd++;				/* Move to start of the  <PROGRAM DATA>. */
			delims[0] = DATA_SEP;
			breakString(pCmd, params, delims);			/* Separate any <PROGRAM DATA> delineated by a valid
														 * <PROGRAM DATA SEPARATOR>.
														 */
			if (lexPNums(params, lRes.pNums) == 1) {	/* Lex the <PROGRAM DATA>. */
				commandError();							/* The <PROGRAM DATA> could not be lex'd correctly. */
				*dest = NULL;
				return NULL;							/* Halt any further lex'ing. */
			}
		}
	}
	
	msgs.eom = true;			/* 6.1.6.2.2 The parser shall set eom true when 1) it receives an END message or
								 * a sequence of data bytes making up a <PROGRAM MESSAGE TERMINATOR>.
								 */
	*dest = resPtr;
	return resPtr;				/* Return pointer for next <PROGRAM MESSAGE UNIT> position in the current <PROGRAM MESSAGE>. */
}

static msgType getMsgType (char * msgUnit) {
	/* Reads the first char of the <PROGRAM MESSAGE UNIT> argument and uses
	 * it to determine and return the type of the <PROGRAM MESSAGE UNIT>.
	 */
	switch (msgUnit[0]) {
		case MNEM_SEP:
			return root;
		case COMMON_IND:
			return common;
		case EMUCODE_IND:
			return emuCode;
		default:
			return relative;
	}
}

static tBool btof (char * boolStr) {
	/* Check for "ON" or "OFF" and convert to boolean double values.
	 * Strings are expected to be all upper-case.
	 */
	if (strcmp(boolStr, "ON") == 0) {
		return tTrue;	/* True. */
	} else if (strcmp(boolStr, "OFF") == 0) {
		return tFalse;	/* False. */
	} else {
		return tNAB;	/* Not a boolean. */
	}
}

static void remWhitSpace(char *src) {
	/* Remove ALL white space from the passed string. */
	char * ptr;
	ptr = src;
	while (*ptr != 0) {
		if ((*ptr != TAB) && (*ptr != SPACE)) {
			*src = *ptr;
			src++;
		}
		ptr++;
	}
	*src = 0;
}

static Uint16 lexPNums (char ** src, double * dest) {
	while (*src != 0) {			/* Scan each parameter. */
		remWhitSpace(*src);		/* Remove all white space. */
		if (**src == 0x4F) {	/* If parameter starts with 'O' read as boolean. */
			*dest = btof(*src);	/*  so attempt to convert to true or false. */
			if (*dest == tNAB) {/* Bad parameter - Not a Boolean value. */
				return 1;
			} else {
								/* Set the result parameter type to boolean. */
				lRes.parameterType = pBool;
			}
		} else {				/* If the parameter starts with a number conver.t */
			*dest = atof(*src);	/*  to a double */
								/* Set the result parameter type to number. */
			lRes.parameterType = pNumber;
								/* Check if the number is a standard Baud value. */
			if(isBaud((long)(*dest))) {
								/* Set the result parameter type to Baud. */
				lRes.parameterType = pBaud;
            }
    	}
		dest++;
		src++;
	}
	return 0;
}

static void cleanPrg (char * msg) {
	/* Clean the string, msg - Converts all alpha chars to upper case, removes any non printable chars (except
	 * ^END), removes any non-semantic white space and reduces any white space that is semantic to once space at
	 * each occurrence.
	 */
	Uint16 i = 0, j = 0, x = 0;//, y = 1;
	
	while ((msg[j] != NL_MSG_TERM) && (msg[j] != END_MSG_TERM) && (msg[j] != STR_NULL)) {
		if ((msg[j] >= 0x21) && (msg[j] <= 0x7E)) {	/* Standard ASCII printable non-space characters. */
													/* Convert any lower case a-z to upper and copy printable
													 * char to self.
													 */
			if ((msg[j] >= 0x61) && (msg[j] <= 0x7A)) {
				msg[i] = msg[j] - 0x20;
			} else {
				msg[i] = msg[j];
			}
			i++; j++;								/* Increment position. */
		} else if ((msg[j] == TAB) || (msg[j] == SPACE)) {
													/* If the first characters are spaces,
													 * pass over the space until some non-space chars are found.
													 */
			if (j == 0) {
				while ((msg[j] == TAB) || (msg[j] == SPACE)) {
					j++;
				}
			} else 									/* If there is space characters...
													 * If preceded by delimiter, pass over the space.
													 */
			if ((msg[j-1] == MNEM_SEP) || (msg[j-1] == UNIT_SEP) ||(msg[j-1] == DATA_SEP) ||(msg[j-1] == COMMON_IND) ||(msg[j-1] == QUERY_IND)) {
				while ((msg[j] == TAB) || (msg[j] == SPACE)) {
					j++;
				}
			} else {
				x = 0;								/* Else pass over the white space. */
				while ((msg[j] == TAB) || (msg[j] == SPACE)) {
					j++; x++;
				}
													/* If not followed by delimiter, copy one space in
													 * as it may be a malformed header or just a header separator.
													 */
				if ((msg[j] != MNEM_SEP) && (msg[j] != UNIT_SEP) && (msg[j] != DATA_SEP) &&(msg[j] != COMMON_IND) &&(msg[j] != QUERY_IND)) {
					msg[i++] = SPACE;
					/*	// Copies in ALL the white space as is, instead of just one space
					j -= x;
					while ((msg[j] == TAB) || (msg[j] == SPACE)) {
						msg[i] = msg[j];
						i++; j++;
					}
					*/
				}
			}
		} else {
			j++;
		}
	}
	msg[i] = '\0';
}

/*
int lexSuffixes(char ** src, char ** dest) {
			// Step through each parameter
	char ** tmp;
	int i = 0;
	
	tmp = dest;
	while (*src != 0) {
		if (btof(*src) == tNAB) {
			printf("A %c.", **src);
			while ((**src < 0x41) || (**src > 0x5A) && (**src != 0)) {
				*src[i++];
				printf("while1 %c.", *src[i]);
			}
			
			if (**src != 0) {
				printf("if not 0 %c.", **src);
				// Find first non exponential-indicatro letter
				if (**src == 'E') {
					printf("if E %c.", **src);
					*src++;
					if ((**src < 0x41) && (**src > 0x5A) && (**src != 0)) {
						while ((**src < 0x41) && (**src > 0x5A) && (**src != 0)) {
							*src++;
						}
						printf("if %c.", **src);
					} else {
						*src--;
						printf("else %c.", **src);
					}
				}
//				// Match suffix's
//				while (**src != 0) {
//					printf("%c.", **src);
//					**dest = **src;
//					*dest++; 
//					*src++;
//				}
//				**dest == 0;
//				printf("suffix: %s\n", *dest);

			}
		}
		
		src++;
	}
	return 0;
} */
