/**
 * @file SCPI_regs.h
 *
 * This file creates the SCPI required registers
 * such as STB and Questionable and provides the
 * functions for managing them
 *
 * Some bits of the status byte, questionable and
 * operation registers are available to the application
 * designer. See the IEEE Std 488.2 and SCPI-99 for
 * details on the operation of these registers.
 *
 * This file should not need to be edited by the user.
 *
 * @sa SCPI_specificRegs.h
 */

#ifndef SCPI_REGS_H_
#define SCPI_REGS_H_

#ifdef __cplusplus
	extern "C" {
#endif

struct STB_BITS {
	Uint16 	usrb1:1;	/* Available to designer */
	Uint16 	usrb2:1;	/* Available to designer */
	Uint16 	EQUEUE:1;	/* Error/Event Queue or available to designer */
	Uint16 	QUES:1;		/* Questionable register summary bit */
	Uint16 	MAV:1;		/* Message Available */
	Uint16 	ESB:1;		/* Standard Event Status */
	Uint16 	RQS:1;		/* Request Service */
	Uint16 	OPER:1;		/* Operation register summary bit */
};

union STB_REG {
	Uint16 			all;
	struct STB_BITS bit;
}; 

union SRE_REG {
	Uint16			all;	
	struct STB_BITS bit;
};

struct SES_BITS {
	Uint16 	OPC:1;	/* Operation Complete */
	Uint16 	RQC:1;	/* Request Control */
	Uint16 	QYE:1;	/* Query Error */
	Uint16 	DDE:1;	/* Device Dependent Error */
	Uint16 	EXE:1;	/* Execution Error */
	Uint16 	CME:1;	/* Command Error */
	Uint16 	URQ:1;	/* User Request */
	Uint16 	PON:1;	/* Power On */
	Uint16 	rsvd1:1;/* Reserved by IEEE */
	Uint16 	rsvd2:1;/* Reserved by IEEE */
	Uint16 	rsvd3:1;/* Reserved by IEEE */
	Uint16 	rsvd4:1;/* Reserved by IEEE */
	Uint16 	rsvd5:1;/* Reserved by IEEE */
	Uint16 	rsvd6:1;/* Reserved by IEEE */
	Uint16 	rsvd7:1;/* Reserved by IEEE */
	Uint16 	rsvd8:1;/* Reserved by IEEE */
};

union SES_REG {
	Uint16 			all;
	struct SES_BITS bit;
};

union SEE_REG {
	Uint16			all;
	struct SES_BITS bit;
};

struct QUES_BITS {
	Uint16 	usrb0:1;	/* Available to designer - Voltage */
	Uint16 	usrb1:1;	/* Available to designer - Current */
	Uint16 	usrb2:1;	/* Available to designer - Time - Was POW */
	Uint16 	usrb3:1;	/* Available to designer - Pulse */
	Uint16 	usrb4:1;	/* Available to designer - Temperature */
	Uint16 	usrb5:1;	/* Available to designer - Was FREQ */
	Uint16 	usrb6:1;	/* Available to designer - Was PHAS */
	Uint16 	usrb7:1;	/* Available to designer - Was MOD */
	Uint16 	usrb8:1;	/* Available to designer - Calibration */
	Uint16 	usrb9:1;	/* Available to designer - Ethernet */
	Uint16 	usrb10:1;	/* Available to designer - SPInterface */
	Uint16 	usrb11:1;	/* Available to designer - SCInterface */
	Uint16 	usrb12:1;	/* Available to designer - I2C Bus */
	Uint16 	INST:1;		/* Available to designer - Instrument summary */
	Uint16 	CW:1;		/* Available to designer - Command Warning */
	Uint16 	rsvd1:1;	/* NOT USED */
};

union QUES_REG {
	Uint16 			all;
	struct QUES_BITS bit;
}; 

union QUEE_REG {
	Uint16 			all;
	struct QUES_BITS bit;
}; 

struct QUEP_BITS {
	Uint16 * 	usrb0;
	Uint16 * 	usrb1;
	Uint16 * 	usrb2;
	Uint16 * 	usrb3;
	Uint16 * 	usrb4;
	Uint16 * 	usrb5;
	Uint16 * 	usrb6;
	Uint16 * 	usrb7;
	Uint16 * 	usrb8;
	Uint16 * 	usrb9;
	Uint16 * 	usrb10;
	Uint16 * 	usrb11;
	Uint16 * 	usrb12;
	Uint16 * 	INST;
	Uint16 * 	CW;
	Uint16 * 	rsvd1;
};

union QUEP_REG {
	//Uint16 			all;
	struct QUEP_BITS bit;
}; 

struct OPER_BITS {
	Uint16 	usrb0:1;	/* Available to designer - Calibrating */
	Uint16 	usrb1:1;	/* Available to designer - Setting */
	Uint16 	usrb2:1;	/* Available to designer - Ranging */
	Uint16 	usrb3:1;	/* Available to designer - Sweeping */
	Uint16 	usrb4:1;	/* Available to designer - Measuring */
	Uint16 	usrb5:1;	/* Available to designer - Waiting for Trigger summary */
	Uint16 	usrb6:1;	/* Available to designer - Waiting for Arm summary */
	Uint16 	usrb7:1;	/* Available to designer - Correcting */
	Uint16 	usrb8:1;	/* Available to designer */
	Uint16 	usrb9:1;	/* Available to designer */
	Uint16 	usrb10:1;	/* Available to designer */
	Uint16 	usrb11:1;	/* Available to designer */
	Uint16 	usrb12:1;	/* Available to designer */
	Uint16 	INST:1;		/* Available to designer -  Instrument summary */
	Uint16 	usrb13:1;	/* Available to designer - Program running */
	Uint16 	rsvd1:1;	/* NOT USED */
};

union OPER_REG {
	Uint16 			all;
	struct OPER_BITS bit;
}; 

union OPEE_REG {
	Uint16 			all;
	struct OPER_BITS bit;
}; 

struct OPEP_BITS {
	Uint16 * 	usrb0;
	Uint16 * 	usrb1;
	Uint16 * 	usrb2;
	Uint16 * 	usrb3;
	Uint16 * 	usrb4;
	Uint16 * 	usrb5;
	Uint16 *  	usrb6;
	Uint16 * 	usrb7;
	Uint16 * 	usrb8;
	Uint16 * 	usrb9;
	Uint16 * 	usrb10;
	Uint16 * 	usrb11;
	Uint16 * 	usrb12;
	Uint16 * 	INST;
	Uint16 * 	usrb13;
	Uint16 * 	rsvd1;
};

union OPEP_REG {
	//Uint16 			all;
	struct OPEP_BITS bit;
}; 

struct SCPI_REGS {
	union STB_REG 	Status;
	union SRE_REG 	ServiceRequestEnable;
	union QUES_REG 	Questionable;
	union QUEE_REG 	QuestionableEnable;
	union QUEP_REG 	QuestionablePointer;
	union OPER_REG 	Operation;
	union OPEE_REG 	OperationEnable;
	union OPEP_REG 	OperationPointer;
	union SES_REG 	StandardEventStatus;
	union SEE_REG 	StandardEventStatusEnable;
};

extern struct SCPI_REGS ScpiRegs;

extern void commandError (void);
extern void executionError (void);
extern void deviceError (void);
extern void queryError (void);
extern void requestControl (void);
extern void operationComplete (void);
extern Uint16 registerSummary (Uint16 ** summaryBitPtr,  Uint16 * eventAll);
extern Uint16 deregisterSummary (Uint16 ** summaryBitPtr);
extern Uint16 updateQUES (void);
extern Uint16 updateESB (void);
extern Uint16 updateOPER (void);
extern Uint16 updateSTB (void);
extern Uint16 updateMSS (void);
extern void updateQuesReg (void);
extern void updateOperReg (void);
extern void clearSpecificRegs (void);
extern void clearQuesSpecificRegs (void);
extern void clearOperSpecificRegs (void);
extern void clearSpecificPtrs (void);
extern void clearQuesPtrReg (void);
extern void clearOperPtrReg (void); 

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_REGS_H_*/
