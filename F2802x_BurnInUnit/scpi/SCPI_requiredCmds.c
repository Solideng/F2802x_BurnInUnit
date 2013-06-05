#include "scpi.h"

Uint16 registerRequiredCommands (void) {
	/* String literals MUST be all upper case */
	Uint16 err = 0;
	err += registerChild ("STATUS", "ROOT",true, false, pNone, NULL);				/* Header only, no query */
	err += registerChild ("OPERATION", "STAT", true, true, pNone, &statOperEven);	/* Header with optional child */
	err += registerChild ("EVENT", "OPER", true, true, pNone, &statOperEven);		/* Optional child, query only */
	err += registerChild ("CONDITION", "OPER", true, true, pNone, &statOperCond);	/* Child, query only */
	err += registerChild ("ENABLE", "OPER", false, true, pNumber, &statOperEnab);	/* Child */
	err += registerChild ("PRESET", "STAT", false, false, pNone, &statPres);		/* Child */
	err += registerChild ("QUESTIONABLE", "STAT", true, true, pNone, &statQuesEven);/* Header with optional child */
	err += registerChild ("EVENT", "QUES", true, true, pNone, &statQuesEven);		/* Optional child, query only */
	err += registerChild ("CONDITION", "QUES", true, true, pNone, &statQuesCond);	/* Child, query only */
	err += registerChild ("ENABLE", "QUES", false, true, pNone, &statQuesEnab);		/* Child */

	err += registerChild ("SYSTEM", "ROOT", true, false, pNone, NULL);				/* Header only, no query */
	err += registerChild ("ERROR", "SYST", true, true, pNone, &systErrNext);		/* Header with optional child, query only */
	err += registerChild ("NEXT", "ERR", true, true, pNone, &systErrNext);			/* Optional child, query only */
	err += registerChild ("VERSION", "SYST", true, true, pNone, &systVers);			/* Child, query only */
	return err;
}

Uint16 statOperEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statOperCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statOperEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statPres (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesEven (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 statQuesCond (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 statQuesEnab (double * parameters, bool isQuery) {
	// Child
	return 0;
}

Uint16 systErrNext (double * parameters, bool isQuery) {
	// Optional child
	return 0;
}

Uint16 systVers (double * parameters, bool isQuery) {
	// Child
	return 0;
}
