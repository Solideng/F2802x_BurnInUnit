/**
 * @file SCPI_parser.h
 *
 * The parser is the logical portion of a device that takes
 * DABs, END messages and GET messages from the input
 * buffer and analyses them by separating out the various
 * IEEE488.2 syntactic elements into an internal
 * representation which is sent to the execution control.
 * The parser also generates the eom and query messages
 * when it recognises these syntactic element.
 *
 * A @f$ \langle@f$ PROGRAM MESSAGE @f$ \rangle@f$ or
 * @f$ \langle@f$ PROGRAM MESSAGE UNIT @f$ \rangle@f$ is
 * considered "parsed" when it has been analysed by the
 * parser and the parser is ready to continue analysing
 * other @f$ \langle@f$ PROGRAM MESSAGE UNIT @f$ \rangle@f$
 * elements.
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_PARSER_H_
#define SCPI_PARSER_H_

#ifdef __cplusplus
	extern "C" {
#endif

/* Resets the parser (including the lexer) */
extern void resetParser (void);

/* Attempts to sequentially match the <header-path> tokens (the <program mnemonic>s) to a route through
 * registered tree nodes.
 */
extern treeNode * (parseMsgUnit) (lexedItem * msgUnit);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_PARSER_H_*/
