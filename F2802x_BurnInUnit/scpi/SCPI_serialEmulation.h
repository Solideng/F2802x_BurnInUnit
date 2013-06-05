/**
 * @file SCPI_serialEmulation.h
 *
 * This file provides functions to allow serial
 * emulation of GPIB as specified by IEEE 1174-2000
 *
 * Compliance with this standard is, however, dependent
 * on RS-232 hardware connections as set out in IEEE
 * 1174-2000 Class 1. As this library is intended for
 * use in an embedded environment, with simpler serial
 * protocols (such as I2C, SCI, SPI, etc) this emulation
 * only follows the standard outside of this requirement
 * nd so does not claim compliance.
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_SERIALEMULATION_H_
#define SCPI_SERIALEMULATION_H_

#ifdef __cplusplus
	extern "C" {
#endif

extern Uint16 registerEmulationCommands (void);

extern Uint16 SRQ (double * parameters, bool isQuery);	/* Requests service (asynchronous) - code appended with CRLF */
extern Uint16 ddd (double * parameters, bool isQuery);	/* Sends STB and RQS as three decimal digits (response to &POL) - code appended with CRLF */
extern Uint16 DCL (double * parameters, bool isQuery);	/* Acknowledges device clear completion (emulated by a break) - code appended with CRLF */
extern Uint16 GET (double * parameters, bool isQuery);	/* Emulates device trigger */
extern Uint16 GTL (double * parameters, bool isQuery);	/* Changes device R/L state to local */
extern Uint16 LLO (double * parameters, bool isQuery);	/* Changes device R/L state to lockout */
extern Uint16 POL (double * parameters, bool isQuery);	/* Performs serial poll (device responds with &ddd CRLF) */
extern Uint16 SFC (double * parameters, bool isQuery);	/* Enables software (XON/XOFF) flow control */
extern Uint16 HFC (double * parameters, bool isQuery);	/* Enables hard wired (RFR/CTS) flow control */
extern Uint16 DFC (double * parameters, bool isQuery);	/* Disables flow control */

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_SERIALEMULATION_H_*/
