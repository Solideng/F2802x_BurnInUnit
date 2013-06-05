#include "scpi.h"

Uint16 MSS;
//Uint16 GET;
//Uint16 DAB;
//Uint16 END;

struct SCPI_REGS ScpiRegs;

//TODO clear error function and use for dcas/pon reset

void commandError (void) {
	ScpiRegs.StandardEventStatus.bit.CME = 1;
	return;
}

void executionError (void) {
	ScpiRegs.StandardEventStatus.bit.EXE = 1;
	return;
}

void deviceError (void) {
	ScpiRegs.StandardEventStatus.bit.DDE = 1;
	return;
}

void queryError (void) {
	ScpiRegs.StandardEventStatus.bit.QYE = 1;
	return;
}

void requestControl (void) {
	ScpiRegs.StandardEventStatus.bit.RQC = 1;
	return;
}

void operationComplete (void) {
	ScpiRegs.StandardEventStatus.bit.OPC = 1;
	return;
}

Uint16 registerSummary (Uint16 ** summaryBitPtr,  Uint16 * eventAll) {
	/* Point summary bit in standard registers at "all" part of union of device specific registers */
	if (eventAll != NULL) {
		*summaryBitPtr = eventAll;
		return 0;
	} else {
		return 1;
	}
}

Uint16 deregisterSummary (Uint16 ** summaryBitPtr) {
	*summaryBitPtr = NULL;
	return 0;
}

Uint16 updateQUES (void) {
	updateQuesReg();
	if (ScpiRegs.Questionable.all & ScpiRegs.QuestionableEnable.all) {
		ScpiRegs.Status.bit.QUES = 1;
		if (ScpiRegs.ServiceRequestEnable.bit.QUES == 1)
			msgs.reqt = true;
	} else {
		ScpiRegs.Status.bit.QUES = 0;
	}
	return 0;
}

Uint16 updateMAV (void) {
	if (!dataOnOQueue())
		ScpiRegs.Status.bit.MAV = 1;
	else
		ScpiRegs.Status.bit.MAV = 0;
	return 0;
}

Uint16 updateESB (void) {
	if (ScpiRegs.StandardEventStatus.all & ScpiRegs.StandardEventStatusEnable.all)
		ScpiRegs.Status.bit.ESB = 1;
	else
		ScpiRegs.Status.bit.ESB = 0;
	return 0;
}

Uint16 updateOPER (void) {
	updateOperReg();
	if (ScpiRegs.Operation.all & ScpiRegs.OperationEnable.all)
		ScpiRegs.Status.bit.OPER = 1;
	else
		ScpiRegs.Status.bit.OPER = 0;
	return 0;
}

Uint16 updateSTB (void) {
	updateOPER();
	updateESB();
	updateMAV();
	updateQUES();
	return 0;
}

Uint16 updateMSS (void) {
	Uint16 mask = 64;	// Create a mask for RQS bit of status byte
	Uint16 prevMSS = MSS;	// Save the current state of the MSS
	updateSTB();		// Update the status byte register to show current state
						// Create MSS from status byte register AND'd with service request enable register (ignoring the RQS bit)
	MSS = (ScpiRegs.Status.all & ScpiRegs.ServiceRequestEnable.all) & (~mask);
	if (!MSS) {
		msgs.reqf = true;	// Update reqf message based on new MSS state
		if (prevMSS)
			msgs.reqt = true;// Update reqt message based on old and new MSS states
	}
	return MSS;
}

void clearSpecificRegs (void) {
	clearQuesSpecificRegs();
	clearOperSpecificRegs();
	return;
}

void clearQuesSpecificRegs (void) {
	*ScpiRegs.QuestionablePointer.bit.usrb0 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb1 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb2 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb3 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb4 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb5 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb6 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb7 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb8 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb9 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb10 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb11 = 0;
	*ScpiRegs.QuestionablePointer.bit.usrb12 = 0;
	*ScpiRegs.QuestionablePointer.bit.INST = 0;
	*ScpiRegs.QuestionablePointer.bit.CW = 0;
	return;
}

void clearOperSpecificRegs (void) {	
	*ScpiRegs.OperationPointer.bit.usrb0 = 0;
	*ScpiRegs.OperationPointer.bit.usrb1 = 0;
	*ScpiRegs.OperationPointer.bit.usrb2 = 0;
	*ScpiRegs.OperationPointer.bit.usrb3 = 0;
	*ScpiRegs.OperationPointer.bit.usrb4 = 0;
	*ScpiRegs.OperationPointer.bit.usrb5 = 0;
	*ScpiRegs.OperationPointer.bit.usrb6 = 0;
	*ScpiRegs.OperationPointer.bit.usrb7 = 0;
	*ScpiRegs.OperationPointer.bit.usrb8 = 0;
	*ScpiRegs.OperationPointer.bit.usrb9 = 0;
	*ScpiRegs.OperationPointer.bit.usrb10 = 0;
	*ScpiRegs.OperationPointer.bit.usrb11 = 0;
	*ScpiRegs.OperationPointer.bit.usrb12 = 0;
	*ScpiRegs.OperationPointer.bit.INST = 0;
	*ScpiRegs.OperationPointer.bit.usrb13 = 0;
	return;
}

void clearQuesPtrReg (void) {
	ScpiRegs.QuestionablePointer.bit.usrb0 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb1 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb2 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb3 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb4 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb5 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb6 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb7 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb8 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb9 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb10 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb11 = NULL;
	ScpiRegs.QuestionablePointer.bit.usrb12 = NULL;
	ScpiRegs.QuestionablePointer.bit.INST = NULL;
	ScpiRegs.QuestionablePointer.bit.CW = NULL;
	return;
}

void clearSpecificPtrs (void) {
	clearOperPtrReg();
	clearQuesPtrReg();
	return;
}

void clearOperPtrReg (void) {
	ScpiRegs.OperationPointer.bit.usrb0 = NULL;
	ScpiRegs.OperationPointer.bit.usrb1 = NULL;
	ScpiRegs.OperationPointer.bit.usrb2 = NULL;
	ScpiRegs.OperationPointer.bit.usrb3 = NULL;
	ScpiRegs.OperationPointer.bit.usrb4 = NULL;
	ScpiRegs.OperationPointer.bit.usrb5 = NULL;
	ScpiRegs.OperationPointer.bit.usrb6 = NULL;
	ScpiRegs.OperationPointer.bit.usrb7 = NULL;
	ScpiRegs.OperationPointer.bit.usrb8 = NULL;
	ScpiRegs.OperationPointer.bit.usrb9 = NULL;
	ScpiRegs.OperationPointer.bit.usrb10 = NULL;
	ScpiRegs.OperationPointer.bit.usrb11 = NULL;
	ScpiRegs.OperationPointer.bit.usrb12 = NULL;
	ScpiRegs.OperationPointer.bit.INST = NULL;
	ScpiRegs.OperationPointer.bit.usrb13 = NULL;
	return;
}

void updateQuesReg (void) {
	if (ScpiRegs.QuestionablePointer.bit.usrb0 != NULL)
		ScpiRegs.Questionable.bit.usrb0 = (*(ScpiRegs.QuestionablePointer.bit.usrb0) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb1 != NULL)
		ScpiRegs.Questionable.bit.usrb1 = (*(ScpiRegs.QuestionablePointer.bit.usrb1) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb2 != NULL)
		ScpiRegs.Questionable.bit.usrb2 = (*(ScpiRegs.QuestionablePointer.bit.usrb2) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb3 != NULL)
		ScpiRegs.Questionable.bit.usrb3 = (*(ScpiRegs.QuestionablePointer.bit.usrb3) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb4 != NULL)
		ScpiRegs.Questionable.bit.usrb4 = (*(ScpiRegs.QuestionablePointer.bit.usrb4) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb5 != NULL)
		ScpiRegs.Questionable.bit.usrb5 = (*(ScpiRegs.QuestionablePointer.bit.usrb5) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb6 != NULL)
		ScpiRegs.Questionable.bit.usrb6 = (*(ScpiRegs.QuestionablePointer.bit.usrb6) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb7 != NULL)
		ScpiRegs.Questionable.bit.usrb7 = (*(ScpiRegs.QuestionablePointer.bit.usrb7) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb8 != NULL)
		ScpiRegs.Questionable.bit.usrb8 = (*(ScpiRegs.QuestionablePointer.bit.usrb8) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb9 != NULL)
		ScpiRegs.Questionable.bit.usrb9 = (*(ScpiRegs.QuestionablePointer.bit.usrb9) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb10 != NULL)
		ScpiRegs.Questionable.bit.usrb10 = (*(ScpiRegs.QuestionablePointer.bit.usrb10) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb11 != NULL)
		ScpiRegs.Questionable.bit.usrb11 = (*(ScpiRegs.QuestionablePointer.bit.usrb11) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.usrb12 != NULL)
		ScpiRegs.Questionable.bit.usrb12 = (*(ScpiRegs.QuestionablePointer.bit.usrb12) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.INST != NULL)
		ScpiRegs.Questionable.bit.INST = (*(ScpiRegs.QuestionablePointer.bit.INST) > 0) ? 1 : 0;
	if (ScpiRegs.QuestionablePointer.bit.CW != NULL)
		ScpiRegs.Questionable.bit.CW = (*(ScpiRegs.QuestionablePointer.bit.CW) > 0) ? 1 : 0;
	return;	
}

void updateOperReg (void) {
	if (ScpiRegs.OperationPointer.bit.usrb0 != NULL)
		ScpiRegs.Operation.bit.usrb0 = (*(ScpiRegs.OperationPointer.bit.usrb0) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb1 != NULL)
		ScpiRegs.Operation.bit.usrb1 = (*(ScpiRegs.OperationPointer.bit.usrb1) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb2 != NULL)
		ScpiRegs.Operation.bit.usrb2 = (*(ScpiRegs.OperationPointer.bit.usrb2) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb3 != NULL)
		ScpiRegs.Operation.bit.usrb3 = (*(ScpiRegs.OperationPointer.bit.usrb3) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb4 != NULL)
		ScpiRegs.Operation.bit.usrb4 = (*(ScpiRegs.OperationPointer.bit.usrb4) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb5 != NULL)
		ScpiRegs.Operation.bit.usrb5 = (*(ScpiRegs.OperationPointer.bit.usrb5) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb6 != NULL)
		ScpiRegs.Operation.bit.usrb6 = (*(ScpiRegs.OperationPointer.bit.usrb6) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb7 != NULL)
		ScpiRegs.Operation.bit.usrb7 = (*(ScpiRegs.OperationPointer.bit.usrb7) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb8 != NULL)
		ScpiRegs.Operation.bit.usrb8 = (*(ScpiRegs.OperationPointer.bit.usrb8) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb9 != NULL)
		ScpiRegs.Operation.bit.usrb9 = (*(ScpiRegs.OperationPointer.bit.usrb9) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb10 != NULL)
		ScpiRegs.Operation.bit.usrb10 = (*(ScpiRegs.OperationPointer.bit.usrb10) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb11 != NULL)
		ScpiRegs.Operation.bit.usrb11 = (*(ScpiRegs.OperationPointer.bit.usrb11) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb12 != NULL)
		ScpiRegs.Operation.bit.usrb12 = (*(ScpiRegs.OperationPointer.bit.usrb12) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.INST != NULL)
		ScpiRegs.Operation.bit.INST = (*(ScpiRegs.OperationPointer.bit.INST) > 0) ? 1 : 0;
	if (ScpiRegs.OperationPointer.bit.usrb13 != NULL)
		ScpiRegs.Operation.bit.usrb13 = (*(ScpiRegs.OperationPointer.bit.usrb13) > 0) ? 1 : 0;
	return;	
}
