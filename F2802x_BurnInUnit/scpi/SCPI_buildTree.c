#include "scpi.h"

tBool tTrue = 1.0;
tBool tFalse = 0.0;
tBool tNAB = 10.0;
treeNode treeChild[TREE_CHILD_LIMIT] = {0};

static const treeNode defaultValues = {"\0", "\0", "ROOT", true, false, pNone, NULL}; /* Vales for initialising a new tree node */
static const treeNode rootItemValues = {"ROOT\0", "ROOT\0", "ROOT\0", true, false, pNone, NULL}; /* Values for initialising the tree root node */
static char tempName[SHORTNAME_LIMIT] = {0}; 	/*  */
static char lastTreeNode = 0; 					/*  */

static Uint16 registerRoot (void);
static char * shortenName(char * name);
static Uint16 alreadyRegistered(char longName [], char parentName []);

Uint16 buildSCPITree (void) {
	Uint16 err = 0;
	err += registerRoot();
	err += registerCommonCommands();
	err += registerRequiredCommands();
	#if GPIB_EMULATION != 0
		err += registerEmulationCommands();
	#endif
	err += registerSpecificCommands();
	err += registerQuestionableRegs();
	ScpiRegs.StandardEventStatus.bit.PON = 1;
	return err;
}

Uint16 registerChild (char longName [], char parentName [], bool isHeaderOnly, bool isQueryable,  pType parameterType, deviceFuncPtr handler) {
	/* Registers a new SCPI node (header or command) on the tree.
	 * Essentially this means validating all the passed arguments and copying them into the appropriate places in
	 * an empty structure in the tree array.
	 */
	if (lastTreeNode <= TREE_CHILD_LIMIT) {	/* Check there is room on the tree array. */
		lastTreeNode++;
	} else {
		return 1;
	}
											/* Check a node of the same name is not already registered. */
	if	(alreadyRegistered(longName, parentName)) {
		return 1;
	} else {								/* Ensure the structure is clear. */
		treeChild[lastTreeNode] = defaultValues;
		strcpy(treeChild[lastTreeNode].longName, longName);	/* Set the node's long name. */
											/* Shorten the long name and set the result as the node's short name. */
		strcpy(treeChild[lastTreeNode].shortName, shortenName(treeChild[lastTreeNode].longName));
											/* Shorten the long parent name and set the result as the node's
											 * parent node name.
											 */
		strcpy(treeChild[lastTreeNode].shortParent, shortenName(parentName));
											/* Set whether the node maybe used without optional children. */
		treeChild[lastTreeNode].isHeaderOnly = isHeaderOnly;
											/* Set whether the node may be queried. */
		treeChild[lastTreeNode].isQueryable = isQueryable;
											/* Set the node's parameter type, e.g., pNone, pBool, pBaud or pNumber. */
		treeChild[lastTreeNode].parameterType = parameterType;
											/* Set the device specific call-back function handle for the node. */
		treeChild[lastTreeNode].handler = handler;
	}
	return 0;
}

static Uint16 registerRoot (void) {
	/* Creates the special root tree node. */
	treeChild[0] = defaultValues;	/* Ensures the root node is clear. */
	treeChild[0] = rootItemValues;	/* Sets the root node values. */
	return 0;
}

static Uint16 alreadyRegistered(char longName [], char parentName []) {
	/* Searches the tree to check if a node with the same long name and parent name is already registered. */
	Uint16 i = 0;
	char prntStr [13] = {0};
	char chldStr [5] = {0};
	
	strcpy(prntStr, shortenName(parentName));	/* Gets the short version of the node's parent name. */
	strcpy(chldStr, shortenName(longName));		/* Gets the short version of the node's name. */

	while ((i <= TREE_CHILD_LIMIT) && (treeChild[i++].shortParent != 0)) {	/* Scans the tree. */
		if (!strcmp(treeChild[i].shortParent, prntStr)) {	/* Check if the node's short parent name matches */
									/* Check if the node with a matching parent short name has the same long name. */
			if (!strcmp(treeChild[i].longName, longName))
				return 1;
									/* Check if the node with a matching parent short name has the same short name. */
			if (!strcmp(treeChild[i].shortName, chldStr))
				return 2;
		}
	}
	return 0;
}

static char * shortenName(char * name) {
	/* Shortens a long SCPI header from the long version according to the rules in SCPI-99 6.2.1.
	 * Header long versions are expected to be in all upper case already and obey the same rules.
	 */
	Uint16 i = 0;
	char * ptr = tempName;

	if ((*name == '*') || (*name == '&')) {	/* Copy common and emulation names as is. */
		name++;								/* Skip star or ampersand character. */
		while (*ptr++ = *name++);
		return tempName;
	} else {
		while ((name[i] != '\0') && (i <= 4))
			*ptr++ = name[i++];				/* Copy first 0-4 letters of long name. */
		ptr--;
		if (i <= 4) {
			ptr++;							/* If name is already exactly four letters or less. */
			*ptr = 0;
			return tempName;
		} else {
			ptr--;
			if ((*ptr == 'A') || (*ptr == 'E') || (*ptr == 'I') || (*ptr == 'O') || (*ptr == 'U')) {
				*ptr = '\0';				/* If letter 4 is a vowel overwrite previous letter with \0. */
				return tempName;
			} else {						/* If letter 4 is not a vowel overwrite with \0. */
				*++ptr = '\0';
				return tempName;
			}
		}
	}
}
