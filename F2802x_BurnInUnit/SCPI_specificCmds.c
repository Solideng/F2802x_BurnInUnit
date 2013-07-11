#include "Common.h"

Uint16 nSel = 0;

Uint16 registerDeviceCommands (void) {
	/* String literals MUST be all upper case */
	Uint16 err = 0;
	err += registerChild ("CALIBRATION", "ROOT", false, false, pNone, &calAll);			/* Header with optional child. */
	err += registerChild ("ALL", "CAL", false, false, pNone, &calAll);					/* Optional child. */
	
	err += registerChild ("CONTROL", "ROOT", true, false, pNone, NULL);					/* Header only. */
	err += registerChild ("IDLE", "CONT", false, false, pNone, &contIdleInit);			/* Header with optional child. */
	err += registerChild ("INITIATE", "IDLE", false, false, pNone, &contIdleInit);		/* Optional child. */
	
	err += registerChild ("INSTRUMENT", "ROOT", false, true, pNumber, &instNsel);		/* Header with optional child. */
	err += registerChild ("CATALOG", "INST", true, true, pNone, &instCat);				/* Child, query only */
	err += registerChild ("NSELECT", "INST", false, true, pNumber, &instNsel);			/* Optional child. */
	err += registerChild ("STATE", "INST", false, true, pBool, &instStat);				/* child. */

	err += registerChild ("OUTPUT", "ROOT", false, true, pBool, &outpStat);				/* Header with optional child. */
	err += registerChild ("STATE", "OUTP", false, true, pBool, &outpStat);				/* Optional child. */
	
	err += registerChild ("INPUT", "ROOT", false, true, pBool, &inpStat);				/* Header with optional child. */
	err += registerChild ("STATE", "INP", false, true, pBool, &inpStat);				/* Optional child. */

	err += registerChild ("SOURCE", "ROOT", true, false, pNone, NULL);					/* Header only. */
	err += registerChild ("VOLTAGE", "SOUR", false, true, pNone, &sourVoltLev);			/* Header with optional child. */
	err += registerChild ("LEVEL", "VOLT", false, true, pNumber, &sourVoltLev);			/* Optional child. */
	err += registerChild ("LIMIT", "VOLT", true, true, pNone, &sourVoltLim);			/* Child, query only. */
	err += registerChild ("PROTECT", "VOLT", false, true, pNumber, &sourVoltProt);		/* Child. */
	err += registerChild ("RANGE", "VOLT", false, true, pNumber, &sourVoltRang);		/* Child. */
	err += registerChild ("SLEW", "VOLT", false, true, pNumber, &sourVoltSlew);			/* Child. */
	err += registerChild ("COEFFICIENT", "VOLT", false, true, pNumber, &sourVoltCoef); 	/* Child. */
	err += registerChild ("CURRENT", "SOUR", false, true, pNumber, &sourCurrLev);		/* Header with optional child. */
	err += registerChild ("LEVEL", "CURR", false, true, pNumber, &sourCurrLev);			/* Optional child. */
	err += registerChild ("LIMIT", "CURR", true, true, pNone, &sourCurrLim);			/* Child, query only. */
	err += registerChild ("PROTECT", "CURR", false, true, pNumber, &sourCurrProt);		/* Child. */
	err += registerChild ("RANGE", "CURR", false, true, pNumber, &sourCurrRang);		/* Child. */
	err += registerChild ("SLEW", "CURR", false, true, pNumber, &sourCurrSlew);			/* Child. */
	err += registerChild ("COEFFICIENT", "CURR", false, true, pNumber, &sourCurrCoef);	/* Child. */
	err += registerChild ("FREQUENCY", "SOUR", false, true, pNumber, &sourFreqFix);		/* Header with optional child. */
	err += registerChild ("FIXED", "FREQ", false, true, pNumber, &sourFreqFix);			/* Optional child. */
	err += registerChild ("GAIN", "FREQ", false, true, pNumber, &sourFreqGain);			/* Child. */
	err += registerChild ("OFFSET", "FREQ", false, true, pNumber, &sourFreqOffs);		/* Child. */
	err += registerChild ("TEMPERATURE", "SOUR", false, true, pNone, &sourTempProt);	/* Header with optional child. */
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
	// TODO write CAL cmd
	// Full system calibration
	return 0;
}

Uint16 contIdleInit (double * parameters, bool isQuery) {
	// Child
	// TODO write IDLE cmd
	// Returns the device to the idle state.
	return 0;
}

Uint16 instCat (double * parameters, bool isQuery) {
	/* Child. Queries the available selection of logical instruments. */
	Uint16 err = 0;
	Uint16 i = 0;
	for (i = 0; i < NUM_CHNLS; i++) {
		 if (i != (NUM_CHNLS - 1))
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

	if ((*parameters > 0) && (*parameters < NUM_CHNLS)) {
		nSel = *parameters;
		return 0;
	}
	return 1;
}

Uint16 instStat (double * parameters, bool isQuery) {
	// Child
	// TODO write cmd
	// Turns the selected logical instrument on or off.
	return 0;
}

Uint16 outpStat (double * parameters, bool isQuery) {
	// Optional child
	// TODO write cmd - outpStat
	// Turns the output of the selected logical instrument on or off.
	Uint16 err = 0;
	if (isQuery) {

	} else {

	}
	return err;
}

Uint16 inpStat (double * parameters, bool isQuery) {
	// Optional child
	// TODO write cmd - inpStat
	// Turns the input of the selected logical instrument on or off.
	return 0;
}

Uint16 sourVoltLev (double * parameters, bool isQuery) {
	/* Optional child. Sets or queries the voltage level setting. */
	Uint16 err = 0;
	float32 buf = 0;
	if (channel[nSel].ctlMode != vCtrl)	/* Make sure selected instrument is voltage controlled. */
		return 1;
	if (isQuery) {
		err += scGetTarget (nSel, &buf);
		err += respond (&buf, Double, true); // TODO test if Double works OK for float32
	} else {
		err += scSetTarget (nSel, *parameters);
	}
	return err;
}

Uint16 sourVoltLim (double * parameters, bool isQuery) {
	/* Child, query only. Queries the voltage setting limit, response is RMS. */
	float32 buf = 0;
					/* Convert value from SQ10 format to double. */
	buf = _IQ10toF((int32)channel[nSel].vMaxRms);
	return respond(&buf, Double, true);
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
	/* Child. Sets or queries the voltage scaling factor, thus adjusting the available range. */
	Uint16 err = 0;
	float32 buf = 0;
	if (isQuery) {
		err += adcGetVScale(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += adcSetVScale(nSel, *parameters);
	}
	return err;
}

Uint16 sourVoltSlew (double * parameters, bool isQuery) {
	/* Child. Sets or queries the voltage slew step. */
	Uint16 err = 0;
	float32 buf = 0;
	if (channel[nSel].ctlMode != vCtrl)	/* Make sure selected instrument is voltage controlled. */
		return 1;
	if (isQuery) {
		err += scGetStep(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += scSetStep(nSel, *parameters);
	}
	return err;
}

Uint16 sourVoltCoef (double * parameters, bool isQuery) {
	/* Child. Sets or queries a IIR filter control law coefficient. */
	Uint16 err = 0;
	float32 buf = 0;
	cfType coefSelect = firstCoef;

	if (channel[nSel].ctlMode != vCtrl)			/* Make sure selected instrument is voltage controlled. */
		return 1;

	coefSelect = (cfType) (int16) *parameters;	/* Convert first parameter from double to coefficient selector type. */

	if (isQuery) {
		err += cntlGetCoef(nSel, coefSelect, &buf);
		err += respond(&buf, Double, true);
	} else {
		parameters++;							/* Look at second parameter. */
		err += cntlSetCoef(nSel, coefSelect, *parameters);
	}
	return err;
}

Uint16 sourCurrLev (double * parameters, bool isQuery) {
	/* Optional child. Sets or queries the current level setting. */
	Uint16 err = 0;
	float32 buf = 0;
	if (channel[nSel].ctlMode != iCtrl)	/* Make sure selected instrument is current controlled. */
		return 1;
	if (isQuery) {
		err += scGetTarget (nSel, &buf);
		err += respond (&buf, Double, true);
	} else {
		err += scSetTarget (nSel, *parameters);
	}
	return err;
}

Uint16 sourCurrLim (double * parameters, bool isQuery) {
	/* Child, query only. Queries the current setting limit. Response is RMS. */
	float32 buf = 0;
					/* Convert value from SQ10 format to double. */
	buf = _IQ10toF((int32)channel[nSel].iMaxRms);
	return respond(&buf, Double, true);
}

Uint16 sourCurrProt(double * parameters, bool isQuery) {
	/* Child. Sets or queries the current protection level. */
	Uint16 err = 0;
	float32 buf = 0;

	if (isQuery) {
		err += adcGetOcp(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += adcSetOcp(nSel, *parameters);
	}
	return err;
}

Uint16 sourCurrRang (double * parameters, bool isQuery) {
	/* Child. Sets or queries the current scaling factor, thus adjusting the available range. */
	Uint16 err = 0;
	float32 buf = 0;
	if (isQuery) {
		err += adcGetIScale(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += adcSetIScale(nSel, *parameters);
	}
	return err;
}

Uint16 sourCurrSlew (double * parameters, bool isQuery) {
	/* Child. Sets or queries the current slew step. */
	Uint16 err = 0;
	float32 buf = 0;
	if (channel[nSel].ctlMode != iCtrl)	/* Make sure selected instrument is current controlled. */
			return 1;
	if (isQuery) {
		err += scGetStep(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += scSetStep(nSel, *parameters);
	}
	return err;
}

Uint16 sourCurrCoef (double * parameters, bool isQuery) {
	/* Child. Sets or queries a IIR filter control law coefficient. */
	Uint16 err = 0;
	float32 buf = 0;
	cfType coefSelect = firstCoef;
	if (channel[nSel].ctlMode != iCtrl)			/* Make sure selected instrument is current controlled. */
		return 1;
	coefSelect = (cfType) (int16) (*parameters);/* Convert first parameter from double to coefficient selector type. */
	if (isQuery) {
		err += cntlGetCoef(nSel, coefSelect, &buf);
		err += respond(&buf, Double, true);
	} else {
		parameters++;							/* Look at second parameter. */
		err += cntlSetCoef(nSel, coefSelect, *parameters);
	}
	return err;
}

Uint16 sourFreqFix (double * parameters, bool isQuery) {
	/* Optional child. Sets or queries the frequency of the sine wave produced by the AC stage. */
	Uint16 err = 0;
	Uint16 buf = 0;
	if (nSel != AC_STAGE)
		return 1;
	if (isQuery) {
		err += sgGetFreq(&buf);
		err += respond(&buf, Integer, true);
	} else {
		buf = (Uint16) (*parameters);
		err += sgSetFreq(buf);
	}
	return err;
}

Uint16 sourFreqGain (double * parameters, bool isQuery) {
	/* Child. Sets or queries the target gain of the sine wave produced by the AC stage. */
	Uint16 err = 0;
	float32 buf = 0;
	if (nSel != AC_STAGE)
		return 1;
	if (isQuery) {
		err += sgGetGainTarget(&buf);
		err += respond(&buf, Double, true);
	} else {
		err += sgSetGainTarget(*parameters);
	}
	return err;
}

Uint16 sourFreqOffs (double * parameters, bool isQuery) {
	/* Child. Sets or queries the offset of the sine wave produced by the AC stage. */
	Uint16 err = 0;
	float32 buf = 0;
	if (nSel != AC_STAGE)
		return 1;
	if (isQuery) {
		err += sgGetOffset(&buf);
		err += respond(&buf, Double, true);
	} else {
		err += sgSetOffset(*parameters);
	}
	return err;
}

Uint16 sourTempProt (double * parameters, bool isQuery) {
	/* Optional child. Sets or queries the over temperature protection level setting. */
	Uint16 err = 0;
	float32 buf = 0;

	if (isQuery) {
		err += tmpGetOtp(nSel, &buf);
		err += respond(&buf, Double, true);
	} else {
		err += tmpSetOtp(nSel, *parameters);
	}
	return err;
}

Uint16 measVolt (double * parameters, bool isQuery) {
	/* Child, query only. Measures the voltage of the selected channel */
	Uint16 err = 0;
	float32 buf = 0;
	err += adcGetVoltage(nSel, &buf);
	err += respond(&buf, Double, true);
	return err;
}

Uint16 measCurr (double * parameters, bool isQuery) {
	/* Child. Measures the current of the selected channel */
	Uint16 err = 0;
	float32 buf = 0;
	err += adcGetCurrent(nSel, &buf);
	err += respond(&buf, Double, true);
	return err;
}

Uint16 measTemp (double * parameters, bool isQuery) {
	/* Child. Measures the temperature of the selected input channel. */
	Uint16 err = 0;
	float32 buf= 0;
	err += tmpRead(nSel, &buf);
	err += respond(&buf, Double, true);
	return err;
}

Uint16 statQuesVoltEven (double * parameters, bool isQuery) {
	// Optional child
	// TODO CMDS FOR ALL SPECIFIC STATUS QUERY
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
