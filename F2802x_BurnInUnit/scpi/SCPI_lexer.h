/**
 * @file SCPI_lexer.h
 *
 * The lexer performs a lexical analysis of a
 * @f$ \langle@f$ PROGRMA MESSAGE @f$ \rangle@f$ , breaking
 * it into its constituent headers command and parameter
 * tokens and as such, forms the first stage of the parser.
 * For further details on the parser see SCPI_parser.h.
 *
 * This file should not need to be edited by the user.
 *
 */

#ifndef SCPI_LEXER_H_
#define SCPI_LEXER_H_

#ifdef __cplusplus
	extern "C" {
#endif

#define MAX_HEADERS		5
#define MAX_MSG_UNITS	30
#define MAX_PARAMS		5

#define UNIT_SEP 		0x3B /* <PROGRAM MESSAGE UNIT SEPARATOR> ";". */
#define DATA_SEP 		0x2C /* <PROGRAM DATA SEPARATOR> ",". */
#define MNEM_SEP 		0x3A /* <program mnemonic> separator ":". */
#define HEADER_SEP 		0x20 /* <PROGRAM HEADER SEPARATOR> " ". */
#define COMMON_IND 		0x2A /* <COMMON COMMAND PROGRAM HEADER> prefix "*". */
#define EMUCODE_IND 	0x26 /* Emulation code command indicator prefix "&". */
#define QUERY_IND 		0x3F /* <QUERY PROGRM HEADER> suffix "?". */

#define NL_MSG_TERM 	0x0A /* <PROGRAM MESSAGE TERMINATOR> "\n". */
#define END_MSG_TERM 	0x04 /* <PROGRAM MESSAGE TERMINATOR> EOT. */
#define TAB 			0x09 /* <white space> tab. */
#define SPACE			0x20 /* <white space> " ". */
#define STR_NULL		0x00 /* String null/terminator "\0". */

typedef enum msgType {		/* Possible message types. */
	notFound = 0,
	root = 1,
	relative = 2,
	common = 3,
	emuCode = 4
}msgType;

typedef struct lexedItem { 	/* Storage for lex'd tokens and other message information. */
	double pNums[MAX_PARAMS];	/* Lex'd parameter storage */
	char msgUnit[MAX_MSG_UNITS];/*  */
	char * headers[MAX_HEADERS];/* Lex'd msg unit program mnemonics */
	msgType type;				/* Found message type */
	pType parameterType;		/* Found parameter type */
}lexedItem;
extern lexedItem lRes;

/* Resets the lexer */
extern void resetLexer (void);

/* Lexically analyses the passed program message into its
 * constituent parts - message type, headers, command,
 * parameters and query status
 */
extern lexedItem * (lexProgramMsg) (lexedItem ** dest, char * programMsg);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_LEXER_H_*/
