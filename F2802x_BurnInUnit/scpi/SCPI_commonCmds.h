/**
 * @file SCPI_commonCmds.h
 *
 * This file includes the functions for registering
 * the SCPI common commands and the related callback
 * functions
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_COMMONCMDS_H_
#define SCPI_COMMONCMDS_H_

#ifdef __cplusplus
	extern "C" {
#endif

extern Uint16 registerCommonCommands (void); /* Registers the common command children on the command tree */

/*  Callback functions for the required common commands
 *  as specified by IEEE 488.2-1992 and SCPI-1999
 */
extern Uint16 IDN (double * parameters, bool isQuery);
extern Uint16 RST (double * parameters, bool isQuery);
extern Uint16 TST (double * parameters, bool isQuery);
extern Uint16 OPC (double * parameters, bool isQuery);
extern Uint16 WAI (double * parameters, bool isQuery);
extern Uint16 CLS (double * parameters, bool isQuery);
extern Uint16 ESE (double * parameters, bool isQuery);
extern Uint16 ESR (double * parameters, bool isQuery);
extern Uint16 SRE (double * parameters, bool isQuery);
extern Uint16 STB (double * parameters, bool isQuery);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_COMMONCMDS_H_*/
