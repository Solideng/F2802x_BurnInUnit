/**
 * @file SCPI_specificCmds.h
 *
 * This file includes the functions for registering the
 * application specific commands and the related
 * callback functions
 *
 * These commands and their associated callback functions
 * can be edited to suit the application in use, though
 * they should follow the guidelines and conventions
 * laid out in IEEE 488.2-1992 and SCPI-1999
 *
 * Please follow the information in the parser
 * documentation and the file SCPI_specificCmds.c on how
 * to add or change message headers, commands and
 * parameters
 *
 * To register a new command device specific tree node, add
 * a registerChild() call to the function
 * registerSpecificCommands() with the form
 * "err += registerChild(CHILD_LONG_NAME, PARENT_SHORT_NAME,
 * BOOL_IS_HEADER_ONLY, BOOL_IS_QUERY, CALLBACK_HANDLE);",
 * where:
 *
 * - CHILD_LONG_NAME is the long form of the name of the
 * child node to be registered.
 *
 * - PARENT_SHORT_NAME is the short form of the name of the
 * parent of the child node to be registered. This must be
 * an already existing node.
 *
 * - BOOL_IS_HEADER_ONLY is a boolean value that indicates if
 * the node being registered is a header only (i.e. is not
 * a command) or not.
 *
 * - BOOL_IS_QUERY is a boolean value that indicates if the
 * node being registered my be queried or not.
 *
 * - CALLBACK_HANDLE is a pointer to the function to be
 * associated with this child if it is a command, or NULL
 * if it is a header only.
 *
 * For further information on the operation of
 * registerChild() please see the function description.
 *
 * All string literals MUST be in upper case and enclosed
 * in quotation marks.
 *
 * Node long and short names must conform to the standard
 * as outlined by SCPI-99 in 6.2.1
 *
 * All tree nodes are children of at least "ROOT".
 * Changes to the number of node registered should be
 * reflected by the user in the number defined for
 * TREE_CHILD_LIMIT in scpi.h
 *
 */

#ifndef SCPI_SPECIFICCMDS_H_
#define SCPI_SPECIFICCMDS_H_

#ifdef __cplusplus
	extern "C" {
#endif

/*============= GLOBAL FUNCTIONS ==============*/
extern Uint16 registerDeviceCommands (void);
/*=========== CALIBRATION SUBSYSTEM ===========*/
//extern Uint16 calAll (double * parameters, bool isQuery);
///*============= CONTROL SUBSYSTEM =============*/
//extern Uint16 contIdleInit (double * parameters, bool isQuery);
///*=========== INSTRUMENT SUBSYSTEM ============*/
//extern Uint16 instCat (double * parameters, bool isQuery);
//extern Uint16 instNsel (double * parameters, bool isQuery);
//extern Uint16 instStat (double * parameters, bool isQuery);
///*============== INPUT SUBSYSTEM ==============*/
//extern Uint16 outpStat (double * parameters, bool isQuery);
///*============= OUTPUT SUBSYSTEM ==============*/
//extern Uint16 inpStat (double * parameters, bool isQuery);
///*============= SOURCE SUBSYSTEM ==============*/
//extern Uint16 sourVoltLev (double * parameters, bool isQuery);
//extern Uint16 sourVoltLim (double * parameters, bool isQuery);
//extern Uint16 sourVoltProt (double * parameters, bool isQuery);
//extern Uint16 sourVoltRang (double * parameters, bool isQuery);
//extern Uint16 sourVoltSlew (double * parameters, bool isQuery);
//extern Uint16 sourVoltCoef (double * parameters, bool isQuery);
//extern Uint16 sourCurrLev (double * parameters, bool isQuery);
//extern Uint16 sourCurrLim (double * parameters, bool isQuery);
//extern Uint16 sourCurrProt(double * parameters, bool isQuery);
//extern Uint16 sourCurrRang (double * parameters, bool isQuery);
//extern Uint16 sourCurrSlew (double * parameters, bool isQuery);
//extern Uint16 sourCurrCoef (double * parameters, bool isQuery);
//extern Uint16 sourFreqFix (double * parameters, bool isQuery);
//extern Uint16 sourFreqGain (double * parameters, bool isQuery);
//extern Uint16 sourFreqOffs (double * parameters, bool isQuery);
//extern Uint16 sourFreqRes (double * parameters, bool isQuery);
//extern Uint16 sourTempProt (double * parameters, bool isQuery);
///*============= MEASURE SUBSYSTEM =============*/
//extern Uint16 measVolt (double * parameters, bool isQuery);
//extern Uint16 measCurr (double * parameters, bool isQuery);
//extern Uint16 measTemp (double * parameters, bool isQuery);
///*============= STATUS SUBSYSTEM ==============*/
//extern Uint16 statQuesVoltEven (double * parameters, bool isQuery);
//extern Uint16 statQuesVoltCond (double * parameters, bool isQuery);
//extern Uint16 statQuesVoltEnab (double * parameters, bool isQuery);
//extern Uint16 statQuesCurrEven (double * parameters, bool isQuery);
//extern Uint16 statQuesCurrCond (double * parameters, bool isQuery);
//extern Uint16 statQuesCurrEnab (double * parameters, bool isQuery);
//extern Uint16 statQuesTempEven (double * parameters, bool isQuery);
//extern Uint16 statQuesTempCond (double * parameters, bool isQuery);
//extern Uint16 statQuesTempEnab (double * parameters, bool isQuery);
//extern Uint16 statQuesEthEven (double * parameters, bool isQuery);
//extern Uint16 statQuesEthCond (double * parameters, bool isQuery);
//extern Uint16 statQuesEthEnab (double * parameters, bool isQuery);
//extern Uint16 statQuesScinEven (double * parameters, bool isQuery);
//extern Uint16 statQuesScinCond (double * parameters, bool isQuery);
//extern Uint16 statQuesScinEnab (double * parameters, bool isQuery);
//extern Uint16 statQuesIicbEven (double * parameters, bool isQuery);
//extern Uint16 statQuesIicbCond (double * parameters, bool isQuery);
//extern Uint16 statQuesIicbEnab (double * parameters, bool isQuery);
///*============= SYSTEM SUBSYSTEM ==============*/
//extern Uint16 systCommScinSbit (double * parameters, bool isQuery);
//extern Uint16 systCommScinPar (double * parameters, bool isQuery);
//extern Uint16 systCommScinDbit (double * parameters, bool isQuery);
//extern Uint16 systCommScinBaud (double * parameters, bool isQuery);
//extern Uint16 systCommScinEcho (double * parameters, bool isQuery);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_SPECIFICCMDS_H_*/
