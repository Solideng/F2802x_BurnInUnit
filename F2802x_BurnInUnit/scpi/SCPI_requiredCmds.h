/**
 * @file SCPI_requiredCmds.h
 *
 * This file includes the functions for registering
 * the SCPI required commands and the related
 * callback functions
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_REQUIREDCMDS_H_
#define SCPI_REQUIREDCMDS_H_

#ifdef __cplusplus
	extern "C" {
#endif

extern Uint16 registerRequiredCommands (void);
extern Uint16 statOperEven (double * parameters, bool isQuery);
extern Uint16 statOperCond (double * parameters, bool isQuery);
extern Uint16 statOperEnab (double * parameters, bool isQuery);
extern Uint16 statPres (double * parameters, bool isQuery);
extern Uint16 statQuesEven (double * parameters, bool isQuery);
extern Uint16 statQuesCond (double * parameters, bool isQuery);
extern Uint16 statQuesEnab (double * parameters, bool isQuery);
extern Uint16 systErrNext (double * parameters, bool isQuery);
extern Uint16 systVers (double * parameters, bool isQuery);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_REQUIREDCMDS_H_*/
