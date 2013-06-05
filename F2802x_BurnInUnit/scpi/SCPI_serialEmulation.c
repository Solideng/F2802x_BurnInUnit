
#include "scpi.h"

Uint16 registerEmulationCommands (void) {
	Uint16 err = 0;
	err += registerChild ("&GET", "ROOT", false, false, pNone, &GET);
	err += registerChild ("&GTL", "ROOT", false, false, pNone, &GTL);
	err += registerChild ("&LLO", "ROOT", false, false, pNone, &LLO);
	err += registerChild ("&POL", "ROOT", false, false, pNone, &POL);
	err += registerChild ("&SFC", "ROOT", false, false, pNone, &SFC);
	err += registerChild ("&HFC", "ROOT", false, false, pNone, &HFC);
	err += registerChild ("&DFC", "ROOT", false, false, pNone, &DFC);
	return err;
}

Uint16 SRQ (double * parameters, bool isQuery) {
	return 0;
}

Uint16 ddd (double * parameters, bool isQuery) {
	return 0;
}

Uint16 DCL (double * parameters, bool isQuery) {
	msgs.dcas = true;
	return 0;
}

Uint16 GET (double * parameters, bool isQuery) {
	msgs.get = true;	/* 6.1.6.2.2 The parser shall set eom true when 2) it parses a GET message */
	//printf("&GET!\n");
	return 0;
}

Uint16 GTL (double * parameters, bool isQuery) {
	//printf("&GTL!\n");
	return 0;
}

Uint16 LLO (double * parameters, bool isQuery) {
	//printf("&LLO!\n");
	return 0;
}

Uint16 POL (double * parameters, bool isQuery) {
	//printf("&POL!\n");
	return 0;
}

Uint16 SFC (double * parameters, bool isQuery) {
	//printf("&SFC!\n");
	return 0;
}

Uint16 HFC (double * parameters, bool isQuery) {
	//printf("&HFC!\n");
	return 0;
}

Uint16 DFC (double * parameters, bool isQuery) {
	//printf("&DFC!\n");
	return 0;
}
