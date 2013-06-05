#include "scpi.h"


Uint16 registerCommonCommands (void) {
	/* Registers the 10 IEEE 488.2 required common commands on the command tree.
	 * String literals MUST be all upper case.
	 */
	Uint16 err = 0;
	/* 4.5.1 Required internal operation common commands. */
	err += registerChild ("*IDN", "ROOT", true, true, pNone, &IDN);
	err += registerChild ("*RST", "ROOT", false, false, pNone, &RST);
	err += registerChild ("*TST", "ROOT", false, false, pNone, &TST);
	/* 4.6 Required synchronisation common commands. */
	err += registerChild ("*OPC", "ROOT", false, true, pNone, &OPC);
	err += registerChild ("*WAI", "ROOT", false, false, pNone, &WAI);
	/* 4.4.1 Required status reporting common commands. */
	err += registerChild ("*CLS", "ROOT", false, false, pNone, &CLS);
	err += registerChild ("*ESE", "ROOT", false, true, pNumber, &ESE);
	err += registerChild ("*ESR", "ROOT", true, true, pNone, &ESR);
	err += registerChild ("*SRE", "ROOT", false, true, pNumber, &SRE);
	err += registerChild ("*STB", "ROOT", true, true, pNone, &STB);
	return err;
}

Uint16 IDN (double * parameters, bool isQuery) {
	/* 10.14 Identification query
	 * For unique identification of devices over the system interface.
	 * The response is organised into four fields separated by commas:
	 * "Manufacturer, model, serial, firmware level"
	 */
	Uint16 err = 0;
	char buf[18] = "";
	if (!isQuery)
		return 1;				/* Query only. */
	strcpy(buf, MANUF);			/* Manufacturer field response. */
	err += respond(buf, String, false);
	strcpy(buf, MODEL);			/* Model field response. */
	err += respond(buf, String, false);
	strcpy(buf, SERIAL);		/* Serial field response. */
	err += respond(buf, String, false);
	strcpy(buf, FIRMW);			/* Firmware field response. */
	err += respond(buf, String, true);
	return err;
}

Uint16 RST (double * parameters, bool isQuery) {
	/* 10.32 Reset command.
	 * Performs a device reset.
	 */

	// TODO Write RST function
	return 0;
}

Uint16 TST (double * parameters, bool isQuery) {
	/* 10.38 Self Test query.
	 * Causes an internal self-test and places a response into the
	 * output queue indicating whether or not the device completed
	 * the self-test without any detected errors.
	 */

	//self-test, then rst or return to previous settings??
	
	//TODO Write TST function
	//Return 0 for no errors
	//Return not equal to zero for tst not completed 
	// or completed but errors found
	return 0;
}

Uint16 OPC (double * parameters, bool isQuery) {
	/* 10.18 & 10.19 Operation Complete command and query.
	 * Causes the device to generate the operation complete message
	 * in the SES when all pending selected device operations have
	 * been finished.
	 * The query places a "1" into the devices output queue when
	 * operations are complete.
	 */
	char buf = 1;
	if (isQuery)
		return respond(&buf, Integer, true);
							/* There are only sequential commands in this system
							 * so a query just needs to respond with a "1".
							 */
	ScpiRegs.StandardEventStatus.bit.OPC = 1;
							/* There are only sequential commands so a command
							 * just need to set the OPC bit.
							 */
	return 0;
}

Uint16 WAI (double * parameters, bool isQuery) {
	/* 10.39 Wait-to-Continue command.
	 * Prevents the device from executing any further commands or queries until
	 * the no-operation-pending flag is true.
	 * In a device that implements only sequential commands, the no-operation-
	 * pending flag is always true.
	 */

	/* There are only sequential commands so this is a No-Op */
	return 0;
}

Uint16 CLS (double * parameters, bool isQuery) {	
	/* 10.3 Clear Status Command.
	 * Clears status data structures and forces the device to the idle state
	 * (except output queue and STB.MAV)
	 */
	Uint16 mav;
	mav = ScpiRegs.Status.bit.MAV;			/* Preserve MAV. */
	ScpiRegs.Questionable.all = 0;			/* Clear all questionable status data structures. */
	ScpiRegs.Operation.all = 0;				/* Clear operation status data structure. */
	ScpiRegs.StandardEventStatus.all = 0;	/* Clear all standard event data structure. */
	ScpiRegs.Status.all = 0;				/* Clear status byte data structure. */
	clearSpecificRegs();					/* Clear all device specific status data structures. */
	ScpiRegs.Status.bit.MAV = mav;			/* Reinstate MAV value. */
	return 0;
}

Uint16 ESE (double * parameters, bool isQuery) {
	/* 10.10 & 10.11 Standard Event Status Enable command and query.
	 * Sets and queries the standard event status enable register bits.
	 */
	if (isQuery)	/* Query responds with the value of the ESE. */
		return respond(&ScpiRegs.StandardEventStatusEnable.all, Integer, true);

	if ((*parameters < 0.0) || (*parameters > 255.0))
		return 1;	/* Execution error - parameter out of range. */
					/* Set ESE register to parameter. */
	ScpiRegs.StandardEventStatusEnable.all = ((Uint16) (*parameters + 0.5));
	return 0;
}

Uint16 ESR (double * parameters, bool isQuery) {
	/* 10.12 Standard event status register query.
	 * Determines the current contents of the standard event status register.
	 * Reading the standard event status register clears it.
	 */
	Uint16 err = 0;
	if (!isQuery)
		return 1;							/* Execution error - query only. */
											/* Query responds with value of the SES. */
	err = respond(&ScpiRegs.StandardEventStatus.all, Integer, true);
	ScpiRegs.StandardEventStatus.all = 0;	/* ESR read is destructive. */
	return err;
}

Uint16 SRE (double * parameters, bool isQuery) {
	/* 10.34 & 10.35 Service Request Enable command and query.
	 * Sets and queries the SRE register bits as defined in 11.3.2
	 */
	if (isQuery)	/* Query responds with the value of the SRE register. */
		return respond(&ScpiRegs.ServiceRequestEnable.all, Integer, true);
	
	if ((*parameters < 0.0) || (*parameters > 255.0))
		return 1;	/* Execution error - Parameter out of range. */
					/* Set SRE register to parameter. */
	ScpiRegs.ServiceRequestEnable.all = ((Uint16) (*parameters + 0.5)) & (~(1 << 6));
	return 0;
}

Uint16 STB (double * parameters, bool isQuery) {
	/* 10.36 Read Status Byte query.
	 * Queries the status byte and master summary status bit.
	 */
	Uint16 stb, mss, mask = 64;	/* Create a mask for RQS bit of status byte register. */

	if (!isQuery)
		return 1;				/* Query only. */

	mss = updateMSS();			/* Get MSS based on current status byte register contents. */
	stb = ScpiRegs.Status.all;	/* Read current status byte register contents. */
								/* If MSS is TRUE, set the 6th bit, else clear it. */
	stb = mss > 0 ? stb | mask : stb & (~mask);
								/* Query responds with status byte register value. */
	return respond(&stb, Integer, true);
}
