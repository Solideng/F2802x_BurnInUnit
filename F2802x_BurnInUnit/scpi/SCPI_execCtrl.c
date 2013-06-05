#include "scpi.h"

// TODO: Calling function should call operationComplete() when execCtrl() has finished all commands

void resetExecCtrl(void) {
	msgs.pBlocked = false;	/* 6.1.6.2.5 the parser shall set p-blocked FALSE at all other times */
	msgs.ecIdle = true;		/* 6.1.1.7.2.2 */
}
 
Uint16 execCtrl(treeNode * fnd, lexedItem * reqstd) {
	bool queryState = false;
	Uint16 err = 0;
							/* If the message is not a query check that the tree item is allowed to be called
							 *  as an unqueried command
							 */
	if ((fnd->isHeaderOnly != false) && (reqstd->parameterType != pQuery)) {
					/* Not allowed as an unqueried command, i.e. header & query use only allowed for this item*/
		commandError();
		return 1;
	}
							/* If the message is a query check that the tree item is allowed to be queried */
	if (reqstd->parameterType == pQuery) {
		if (fnd->isQueryable == false) {
			commandError();
			return 1;		/* Not allowed to query this item */
		} 
		queryState = true;	/* Save valid query state. */
							/* Check that the message's parameter type matches the parameter type of
							 * the tree item
							 */
	} else if (fnd->parameterType != reqstd->parameterType) {
							/* Parameter types do not match */
							/* Check that a Baud value hasn't been accidentally detected as a Number value */
			if (!((fnd->parameterType == pNumber) && (reqstd->parameterType == pBaud))) {
				commandError();
				return 1;	/* Parameter value type match failed */
			}
	}
							/* If we have made it this far use the handle to call the command callback function */
	err = (*fnd->handler)(reqstd->pNums, queryState);
							/* Check the callback's return value to see if the function executed correctly */
	if (err) {
		// TODO: or Device Specific Error (DDE)?
		executionError();
		return 1;
    }
	return 0;
}
