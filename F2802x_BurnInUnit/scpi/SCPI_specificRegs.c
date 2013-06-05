#include "scpi.h"

//volatile struct SCPI_REGS ScpiRegs;

union VOLT_REG VoltageEventStatus;
union CURR_REG CurrentEventStatus;
union TEMP_REG TemperatureEventStatus;
union PULS_REG PulseEventStatus;
union SPI_REG SpiEventStatus;
union SCI_REG SciEventStatus;
union IIC_REG IicEventStatus;
union ETH_REG EthernetEventStatus;

Uint16 registerQuestionableRegs(void) {
	/* Register each device specific QUEStionable evnt and OPERation status register and
	 *  enable the relevant summary bit to allow that summary bit to request service
	 */
	Uint16 err = 0;
	
	clearQuesPtrReg();
	clearOperPtrReg();
	
	/* Register QUEStionable fan-out registers */
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb0, &VoltageEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb0 = 1;
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb1, &CurrentEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb1 = 1;
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb2, &PulseEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb2 = 1;
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb3, &TemperatureEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb3 = 1;
	/* QUES.4 - QUES.8 not used - available to designer */
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb9, &EthernetEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb9 = 1;
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb10, &SpiEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb10 = 1;
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb11, &SciEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb11 = 1;
	err += registerSummary(&ScpiRegs.QuestionablePointer.bit.usrb12, &IicEventStatus.all);
	ScpiRegs.QuestionableEnable.bit.usrb12 = 1;
	/* QUES.13 - QUES.15 are already determined by SCPI-1999.0 */
	
	// Register OPERation fan-out registers here
	
	
	/* OPER.13 - OPER.15 are already determined by SCPI-1999.0 */
	
	return err;
}
