#include "scpi.h"

static bool rstFlg = false;
static Uint16 parse(char ** hToks, Uint16 relLevel);

void resetParser (void) {
	resetLexer();		/* 6.1.6.1.2 When the parser is reset, it shall interpret the next data byte it receives
	 	 	 	 	 	 * as the first byte of a <PROGRAM MESSAGE>.
	 	 	 	 	 	 */
	rstFlg = true;
	msgs.eom = false;	/* 6.1.6.2.2 The parser shall set the eom message FALSE when 1) it is reset. */
	msgs.query = false;	/* 6.1.6.2.3 The parser shall set query FALSE when it is reset. */
	msgs.pBlocked = false;	/* 6.1.6.2.5 the parser shall set p-blocked FALSE at all other times. */
}

treeNode * (parseMsgUnit) (lexedItem * msgUnit) {
	/* Call parse() in  manner determined by the message type found by the lexer,
	 *  i.e. Let parse() know what level to start searching at: root or relative.
	 */
	static Uint16 leaf = 0;
	if (rstFlg == true) {
		leaf = 0;
		rstFlg = false;
	}
	if ((msgUnit->type == root) || (msgUnit->type == common) || (msgUnit->type == emuCode)) {
		leaf = parse(msgUnit->headers, 0);
		if (leaf == 0) return NULL;	/* if parse() failed, return NULL. */
	} else if (msgUnit->type == relative) {
		leaf = parse(msgUnit->headers, leaf);
		if (leaf == 0) return NULL;	/* if parse() failed, return NULL. */
	} else {
		return NULL;				/* Unknown message type. */
	}
	return &treeChild[leaf];		/* Return the tree leaf node found by parse() if it returned successfully. */
}

static Uint16 parse(char ** hToks, Uint16 relLevel) {
	/* Attempts to sequentially match the <header-path> tokens (the <program mnemonic>s) to a route through
	 * registered tree nodes.
	 */
	bool nodeFound = false;
	char parent[5] = {0};
	Uint16 node = 0, i = 0;
							/* Setup the child at the same starting level as the first parent. */
	strcpy(parent, treeChild[relLevel].shortParent);
	while (*hToks != 0) {	/* Step through each <program mnemonic> token. */
		nodeFound = false;
							/* Check all tree nodes at the current level to see if any compare to the current token. */
							/* Scan the command tree until either the correct node is found or the end is reached */
		while ((i < TREE_CHILD_LIMIT) && (nodeFound == false)) {
							/* Check if the long and short names of this node match the current token */
			if((strcmp(*hToks, treeChild[i].longName) == 0) || (strcmp(*hToks, treeChild[i].shortName) == 0)) {
							/* If node matches, use it to move to the next level until the final child node that
							 * matches the final, trailing <program mnemonic>, token is found.
							 */
							/* Check if the previous <program mnemonic> matches the the current node's parent name. */
				if (strcmp(parent, treeChild[i].shortParent) == 0) {
					node = i;			/* Save the tree array index of this node. */
					nodeFound = true;	/* Let the outer loop know that a node was found. */
					strcpy(parent, treeChild[i].shortName);	/* Save this node's name as the parent name for the next level. */
				} else {
					nodeFound = false;	/* This node did not match. */
				}
			}
			i++;
		}
		if (nodeFound == false) {
			commandError(); 
			return 0;
		}
		i = 0;
		hToks++;
	}
	return node; 			/* Return the array index of the tree leaf node that matches the final, trailing
							 * <program mnemonic>, token.
							 */
}
