#include "scpi.h"

treeNode tree[TREE_CHILD_LIMIT] = {0};			/* Command tree. */

static Uint16 registerRoot (void);
static char * shortenName(char * shortName, char * longName);
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
	static char lastTreeNode = 0; 			/* Keeps track of the last node on the tree. */
	char tempName[5] = {0};				 	/* A temporary variable for node shortName strings. */
	if (lastTreeNode <= TREE_CHILD_LIMIT) {	/* Check there is room on the tree array. */
		lastTreeNode++;
	} else {
		return 1;
	}
											/* Check a node of the same name is not already registered. */
	if	(alreadyRegistered(longName, parentName)) {
		return 1;
	} else {
		strcpy(tree[lastTreeNode].longName, longName);	/* Set the node's long name. */
											/* Shorten the long name and set the result as the node's short name. */
		strcpy(tree[lastTreeNode].shortName, shortenName(tempName, tree[lastTreeNode].longName));
											/* Shorten the long parent name and set the result as the node's
											 * parent node name.
											 */
		strcpy(tree[lastTreeNode].shortParent, shortenName(tempName, parentName));
											/* Set whether the node maybe used without optional children. */
		tree[lastTreeNode].isHeaderOnly = isHeaderOnly;
											/* Set whether the node may be queried. */
		tree[lastTreeNode].isQueryable = isQueryable;
											/* Set the node's parameter type, e.g., pNone, pBool, pBaud or pNumber. */
		tree[lastTreeNode].parameterType = parameterType;
											/* Set the device specific call-back function handle for the node. */
		tree[lastTreeNode].handler = handler;
	}
	return 0;
}

static Uint16 registerRoot (void) {
	/* Creates the special root tree node. */
	strcpy(tree[0].longName, "ROOT");
	strcpy(tree[0].shortName, "ROOT");
	strcpy(tree[0].shortParent, "ROOT");
	tree[0].isHeaderOnly = true;
	tree[0].isQueryable = false;
	tree[0].parameterType = pNone;
	tree[0].handler = NULL;
	return 0;
}

static Uint16 alreadyRegistered(char longName [], char parentName []) {
	/* Searches the tree to check if a node with the same long name and parent name is already registered. */
	Uint16 i = 0;
	char tempName[5] = {0}; 	/* A temporary variable for node shortName strings. */
	char prntStr [13] = {0};
	char chldStr [5] = {0};
	
	strcpy(prntStr, shortenName(tempName, parentName));	/* Gets the short version of the node's parent name. */
	strcpy(chldStr, shortenName(tempName, longName));		/* Gets the short version of the node's name. */

	while ((i <= TREE_CHILD_LIMIT) && (tree[i++].shortParent != 0)) {	/* Scans the tree. */
		if (!strcmp(tree[i].shortParent, prntStr)) {	/* Check if the node's short parent name matches */
									/* Check if the node with a matching parent short name has the same long name. */
			if (!strcmp(tree[i].longName, longName))
				return 1;
									/* Check if the node with a matching parent short name has the same short name. */
			if (!strcmp(tree[i].shortName, chldStr))
				return 2;
		}
	}
	return 0;
}

static char * shortenName(char * shortName, char * longName) {
	/* Shortens a long SCPI header from the long version according to the rules in SCPI-99 6.2.1.
	 * Header long versions are expected to be in all upper case already and obey the same rules.
	 */
	Uint16 i = 0;
	char * ptr;
	ptr = shortName;

	if ((*longName == '*') || (*longName == '&')) {	/* Copy common and emulation longNames as-is. */
		longName++;								/* Skip star or ampersand character. */
		while (*ptr++ = *longName++);
		return shortName;
	} else {
		while ((longName[i] != '\0') && (i <= 4))
			*ptr++ = longName[i++];			/* Copy first 0-4 letters of long longName. */
		ptr--;
		if (i <= 4) {
			ptr++;							/* If longName is already exactly four letters or less. */
			*ptr = 0;
			return shortName;
		} else {
			ptr--;
			if ((*ptr == 'A') || (*ptr == 'E') || (*ptr == 'I') || (*ptr == 'O') || (*ptr == 'U')) {
				*ptr = '\0';				/* If letter 4 is a vowel overwrite previous letter with \0. */
				return shortName;
			} else {						/* If letter 4 is not a vowel overwrite with \0. */
				*++ptr = '\0';
				return shortName;
			}
		}
	}
}
