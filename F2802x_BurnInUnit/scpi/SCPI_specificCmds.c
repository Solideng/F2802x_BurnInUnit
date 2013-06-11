#include "scpi.h"
// TODO: Don't think <coefSpecifier> parameter will work

Uint16 nSel = 0;

Uint16 registerSpecificCommands (void) {
	/* String literals MUST be all upper case */
	Uint16 err = 0;
	err += registerChild ("CALIBRATION", "ROOT", false, false, pNone, &calAll);			/* Header with optional child. */
	err += registerChild ("ALL", "CAL", false, false, pNone, &calAll);					/* Optional child. */
	
	err += registerChild ("CONTROL", "ROOT", true, false, pNone, NULL);					/* Header only. */
	err += registerChild ("IDLE", "CONT", false, false, pNone, &contIdleInit);			/* Header with optional child. */
	err += registerChild ("INITIATE", "IDLE", false, false, pNone, &contIdleInit);		/* Optional child. */
	
	err += registerChild ("INSTRUMENT", "ROOT", false, false, pNumber, &instNsel);		/* Header with optional child. */
	err += registerChild ("CATALOG", "INST", true, true, pNone, &instCat);				/* Child, query only */
	err += registerChild ("NSELECT", "INST", false, true, pNumber, &instNsel);			/* Optional child. */
	err += registerChild ("STATE", "INST", false, true, pBool, &instStat);				/* child. */

	err += registerChild ("OUTPUT", "ROOT", false, false, pBool, &outpStat);			/* Header with optional child. */
	err += registerChild ("STATE", "OUTP", false, true, pBool, &outpStat);				/* Optional child. */
	
	err += registerChild ("INPUT", "ROOT", false, false, pBool, &inpStat);				/* Header with optional child. */
	err += registerChild ("STATE", "INP", false, true, pBool, &inpStat);				/* Optional child. */

	err += registerChild ("SOURCE", "ROOT", true, false, pNone, NULL);					/* Header only. */
	err += registerChild ("VOLTAGE", "SOUR", false, false, pNone, &sourVoltLev);		/* Header with optional child. */
	err += registerChild ("LEVEL", "VOLT", false, true, pNumber, &sourVoltLev);			/* Optional child. */
	err += registerChild ("LIMIT", "VOLT", true, true, pNone, &sourVoltLim);			/* Child, query only. */
	err += registerChild ("PROTECT", "VOLT", false, true, pNumber, &sourVoltProt);		/* Child. */
	err += registerChild ("RANGE", "VOLT", false, true, pNumber, &sourVoltRang);		/* Child. */
	err += registerChild ("SLEW", "VOLT", false, true, pNumber, &sourVoltSlew);			/* Child. */
	err += registerChild ("COEFFICIENT", "VOLT", false, true, pNumber, &sourVoltCoef); 	/* Child. */
	err += registerChild ("CURRENT", "SOUR", false, false, pNumber, &sourCurrLev);		/* Header with optional child. */
	err += registerChild ("LEVEL", "CURR", false, true, pNumber, &sourCurrLev);			/* Optional child. */
	err += registerChild ("LIMIT", "CURR", true, true, pNone, &sourCurrLim);			/* Child, query only. */
	err += registerChild ("PROTECT", "CURR", false, true, pNumber, &sourCurrProt);		/* Child. */
	err += registerChild ("RANGE", "CURR", false, true, pNumber, &sourCurrRang);		/* Child. */
	err += registerChild ("SLEW", "CURR", false, true, pNumber, &sourCurrSlew);			/* Child. */
	err += registerChild ("COEFFICIENT", "CURR", false, true, pNumber, &sourCurrCoef);	/* Child. */
	err += registerChild ("FREQUENCY", "SOUR", false, false, pNumber, &sourFreqFix);	/* Header with optional child. */
	err += registerChild ("FIXED", "FREQ", false, true, pNumber, &sourFreqFix);			/* Optional child. */
	err += registerChild ("GAIN", "FREQ", false, true, pNumber, &sourFreqGain);			/* Child. */
	err += registerChild ("OFFSET", "FREQ", false, true, pNumber, &sourFreqOffs);		/* Child. */
	err += registerChild ("RESOLUTION", "FREQ", false, true, pNumber, &sourFreqRes);	/* Child. */
	err += registerChild ("TEMPERATURE", "SOUR", false, false, pNone, &sourTempProt);	/* Header with optional child. */
	err += registerChild ("PROTECT", "TEMP", false, true, pNumber, &sourTempProt);		/* Optional child. */
	
	err += registerChild ("MEASURE", "ROOT", true, false, pNone, NULL);					/* Header only. */
	err += registerChild ("VOLTAGE", "MEAS", true, true, pNone, &measVolt);				/* Child, query only. */
	err += registerChild ("CURRENT", "MEAS", true, true, pNone, &measCurr);				/* Child, query only. */
	err += registerChild ("TEMPERATURE", "MEAS", true, true, pNone, &measTemp);			/* Child, query only. */
	
	err += registerChild ("VOLTAGE", "QUES", true, true, pNone, &statQuesVoltEven);		/* Header with optional child. */
	err += registerChild ("EVENT", "VOLT", true, true, pNone, &statQuesVoltEven);		/* Optional child, query only. */
	err += registerChild ("CONDITION", "VOLT", true, true, pNone, &statQuesVoltCond);	/* Child, query only. */
	err += registerChild ("ENABLE", "VOLT", false, true, pNone, &statQuesVoltEnab);		/* Child. */
	err += registerChild ("CURRENT", "QUES", true, true, pNone, &statQuesCurrEven);		/* Header with optional child. */
	err += registerChild ("EVENT", "CURR", true, true, pNone, &statQuesCurrEven);		/* Optional child, query only. */
	err += registerChild ("CONDITION", "CURR", true, true, pNone, &statQuesCurrCond);	/* Child. */
	err += registerChild ("TEMPERATURE", "QUES", true, true, pNone, &statQuesTempEven);	/* Header with optional child. */
	err += registerChild ("EVENT", "TEMP", true, true, pNone, &statQuesTempEven);		/* Optional child, query only. */
	err += registerChild ("CONDITION", "TEMP", true, true, pNone, &statQuesTempCond);	/* Child, query only. */
	err += registerChild ("ENABLE", "TEMP", false, true, pNone, &statQuesTempEnab);		/* Child. */
	err += registerChild ("ETHERNET", "QUES", true, true, pNone, &statQuesEthEven);		/* Header with optional child. */
	err += registerChild ("EVENT", "ETH", true, true, pNone, &statQuesEthEven);			/* Optional child, query only. */
	err += registerChild ("CONDITION", "ETH", true, true, pNone, &statQuesEthCond);		/* Child, query only. */
	err += registerChild ("ENABLE", "ETH", false, true, pNone, &statQuesEthEnab);		/* Child. */
	err += registerChild ("SCINTERFACE", "QUES", true, true, pNone, &statQuesScinEven);	/* Header with optional child. */
	err += registerChild ("EVENT", "SCIN", true, true, pNone, &statQuesScinEven);		/* Optional child, query only. */
	err += registerChild ("CONDITION", "SCIN", true, true, pNone, &statQuesScinCond);	/* Child query only. */
	err += registerChild ("ENABLE", "SCIN", false, true, pNone, &statQuesScinEnab);		/* Child. */
	err += registerChild ("IICBUS", "QUES", true, true, pNone, &statQuesIicbEven);		/* Header with optional child. */
	err += registerChild ("EVENT", "IICB", true, true, pNone, &statQuesIicbEven);		/* Optional child, query only. */
	err += registerChild ("CONDITION", "IICB", true, true, pNone, &statQuesIicbCond);	/* Child, query only. */
	err += registerChild ("ENABLE", "IICB", false, true, pNone, &statQuesIicbEnab);		/* Child. */

	err += registerChild ("COMMS", "SYST", true, false, pNone, NULL);					/* Header only. */
	err += registerChild ("SCINTERFACE", "COMM", true, false, pNone, NULL);				/* Header only. */
	err += registerChild ("SBITS", "SCIN", false, true, pNumber, &systCommScinSbit);	/* Child. */
	err += registerChild ("PARITY", "SCIN", false, true, pNumber, &systCommScinPar);	/* Child. */
	err += registerChild ("DBITS", "SCIN", false, true, pNumber, &systCommScinDbit);	/* Child. */
	err += registerChild ("BAUD", "SCIN", false, true, pBaud, &systCommScinBaud);		/* Child. */
	err += registerChild ("ECHO", "SCIN", false, true, pBool, &systCommScinEcho);		/* Child. */
	return err;
}

Uint16 calAll (double * parameters, bool isQuery) {
	// Optional child
	// TODO write cmd
	// Full system calibration
	return 0;
}

Uint16 contIdleInit (double * parameters, bool isQuery) {
	// Child
	// TODO write cmd
	// Returns the device to the idle state.
	return 0;
}

Uint16 instCat (double * parameters, bool isQuery) {
	/* Child. Queries the available selection of logical instruments. */
	Uint16 err = 0;
	Uint16 i = 0;

	if (!isQuery)
		return 1; /* Query only */

	for (i = 0; i <= NUM_CHNLS; i++) {
		 if (i != NUM_CHNLS)
			 err += respond(&i, Integer, false);
		 else
			 err += respond(&i, Integer, true);
	}
	return err;
}

Uint16 instNsel (double * parameters, bool isQuery) {
	/* Optional child. Sets or queries the currently selected logical instrument. */
	if (isQuery) {
		return respond (&nSel, Integer, true);
	}

	if ((*parameters < 0) || (*parameters >= NUM_CHNLS+1)) 	/* +1 accounts for the case where the parameter may be a double that is in range but has a trailing mantissa. */
		return 1;
	nSel = *parameters;
	return 0;
}

Uint16 instStat (double * parameters, bool isQuery) {
	// Child
	// TODO write cmd
	// Turns the selected logical instrument on or off.
	return 0;
}

Uint16 outpStat (double * parameters, bool isQuery) {
	// Optional child
	// TODO write cmd
	// Turns the output of the selected logical instrument on or off.
	return 0;
}

Uint16 inpStat (double * parameters, bool isQuery) {
	// Optional child
	// TODO write cmd
	// Turns the input of the selected logical instrument on or off.
	return 0;
}

Uint16 sourVoltLev (double * parameters, bool isQuery) {
	/* Optional child. Sets or queries the voltage setting for the currently selected logical instrument. */
	Uint16 err = 0;
	float32 buf = 0;
	// TODO currently there is no check that the channel is voltage controlled to say this "channel cannot have voltage set, only current".
	// for this or other channel level related settings.
	if (isQuery) {
		err += scGetTarget (nSel, &buf);    // TODO what about vGain setting? Will this cover it? Or should it wait till slew/window redesign?
		err += respond (&buf, Double, true); // TODO test if Double works OK for float32 i.e. is float32 same as double on this system?
	} else {
		err += scSetTarget (nSel, *parameters); // TODO Will the function accept a double in place of a float 32? see the to-do below.
	}
	return err;
}

Uint16 sourVoltLim (double * parameters, bool isQuery) {
	/* Child. Queries the voltage setting limit, response is RMS. */
	Uint16 err = 0;
	float32 buf = 0;

	if (isQuery)
		return 1;	/* Query only. */

	buf = _IQ10toF((int32)channel[nSel].vMaxRms);
	err += respond(&buf, Double, true);
	return err;
}

Uint16 sourVoltProt (double * parameters, bool isQuery) {
	/* Child. Sets or queries the voltage protection level. */
	Uint16 err = 0;
	float32 buf = 0;

	if (isQuery) {
		err += adcGetOvp(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += adcSetOvp(nSel, *parameters);
	}
	return err;
}

Uint16 sourVoltRang (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourVoltSlew (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourVoltCoef (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourCurrLev (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 sourCurrLim (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourCurrProt(double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourCurrRang (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourCurrSlew (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourCurrCoef (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourFreqFix (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 sourFreqGain (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourFreqOffs (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourFreqRes (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 sourTempProt (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 measVolt (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 measCurr (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 measTemp (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesVoltEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesVoltCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesVoltEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesCurrEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesCurrCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesCurrEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesTempEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesTempCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesTempEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesEthEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesEthCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesEthEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesScinEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesScinCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesScinEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesIicbEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesIicbCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesIicbEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 systCommScinSbit (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 systCommScinPar (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 systCommScinDbit (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 systCommScinBaud (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 systCommScinEcho (double * parameters, bool isQuery) {
	// Child
	return 0;
}