/**
 * @file SCPI_buildTree.h
 *
 * This file includes the base functions and types
 * used to setup and create the SCPI command tree
 *
 * This file should not need to be edited by the user
 *
 */

#ifndef SCPI_BUILDTREE_H_
#define SCPI_BUILDTREE_H_

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef TREE_CHILD_LIMIT
	#error "The command tree size 'TREE_CHILD_LIMIT' has not been defined (should be in scpi.h)"
#endif

typedef enum pType {				/* Basic parameter specification type. */
	pNone = 0,
	pBool = 1,
	pNumber = 2,
	pBaud = 3,
	pQuery = 4
}pType;

typedef enum uType {				/* Parameter units type */
	unitDefault = 0,
	unitVolt,
	unitAmp,
	unitSec,
	unitMin,
	unitHr,
	unitHz,
	unitCel
}uType;

//typedef enum mType {				/* Parameter unit multipliers type */
//	multDefault = 0,
//	multEx = 18,
//	multPe = 15,
//	multT = 12,
//	multG = 9,
//	multMa = 6,						/* NOTE with Hz this is M and M for milli is disallowed */
//	multK = 3,
//	multM = -3,
//	multU = -6,
//	multN = -9,
//	multP = -12,
//	multF = -15,
//	multA = -18
//}mType;
									/* Application callback function pointer type */
typedef Uint16 (*deviceFuncPtr)(double * parameters, bool isQuery);

typedef struct treeNode { 			/* Structure that makes a tree node - 28 byte total size structure (TMS320C28x byte is 16 bits). */
	char longName [13];		/* 13 bytes. */
	char shortName [5];		/* 5 bytes. */
	char shortParent [5];	/* 5 bytes. */
	bool isHeaderOnly;		/* 1 byte */
	bool isQueryable;		/* 1 byte. */
	pType parameterType;	/* 1 byte. */
	deviceFuncPtr handler;	/* 2 bytes. */
}treeNode;

extern treeNode tree[TREE_CHILD_LIMIT]; /* Command mnemonic tree: array of tree node structures each of 28 bytes. */

extern Uint16 buildSCPITree (void); /* Creates the tree by registering root and then calling all included child node registrations. */

									/* Used to register a new command with the tree. */
extern Uint16 registerChild (char longName [], char parentName [], bool isHeaderOnly, bool isQueryable, pType parameterType, deviceFuncPtr handler);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /*SCPI_BUILDTREE_H_*/
