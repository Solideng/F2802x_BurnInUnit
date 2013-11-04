/**
 * @file slaveMode.h
 *
 * @brief Functions and variables used in detection of the mode of the unit.
 *
 * These functions determine the mode of the burn in unit and should be run
 * after system start up. The detection is managed using GPIO6 and GPIO7.
 * A unit may be in one of four modes: a single unit, a master unit, a slave
 * unit or alternatively a unit may be in the undetected mode which is the
 * initial mode of a unit upon startup.
 *
 * @warning For the mode to be correctly detected the two units should be
 * connected while powered off, then the slave unit should be powered on first,
 * followed by powering on the master unit second. Once detected at startup,
 * any further changes to the connected mode will not be detected and thus
 * the detected mode reported will not reflect such changes to the connection
 * mode of the unit. To change the connection mode of a unit, it, and any
 * slave or master unit must be powered off again.
 *
 * Also note that, as mentioned, the mode detection algorithm uses GPIO6.
 * However the during initialisation and use of the SPI peripheral GPIO6 is
 * used as a slave service request line. Thus even if the unit mode is not
 * used in conjunction with the SPI functions they should still only be used
 * after the slave mode has been detected.
 *
 */

#ifndef SLAVEMODE_H_
#define SLAVEMODE_H_

#ifdef __cplusplus
extern "C" {
#endif

// TODO:...
// IF SLAVE SCPI CTRL SHOULD CHANGE INST NUMBERS... EG FROM 5-8 to 1-4
// IF MASTER WITH SLAVE, ALL SCPI DEVICE FUNCTIONS (COMMON, REQUIRED, SPECIFIC) WILL NEED TO CALL MASTER TO SLAVE SPECIFIC FUNCTIONS
//  THEN WAIT FOR A CMD FINISHED STATUS RETURN OR A RESPONSE RETURN IF CMD IS QUERY.
// THE MASTER TO SLAVE SPECIFIC FUNCTIONS ARE WHERE THE SPI USAGE SHOULD TAKE PLACE - THEY SHOULD WAIT ON A RESPONSE BY INTERRUPT
//  **BUT THEY SHOUD NOT BLOCK** IE THE SCPI CNTL SHOULD RETURN FOR THE NEXT ITERATION AFTER THE COMMAND IS SENT, BUT SHOULD NOT DO
//  ANYTHING MORE IN FURTHER ITERATIONS UNTIL A RESPONSE OR STATUS IS RECEIVED BACK ON SPI.

// MAYBE THE MASTER TO SLAVE SPECIFIC FUNCTIONS COULD ALL BE MANAGED BY ONE OR TWO WRAPPER FUNCTIONS THT SIMPLY HANDLE THE CMD
//  PASS-THROUGH AND RESPONSE??

/** The possible unit modes */
enum slaveMode {
	slaveUnit = 0,
	masterUnit = 1,
	singleUnit = 2,
	undetected = 10
};

/** A type to allow specification of the unit mode */
typedef enum slaveMode slaveMode;

extern slaveMode slaveModeStatus;	/**< A global variable that holds the detected mode of the unit, allowing the mode to be queried. */

/** Detects the unit mode.
 * This should only be run once, at startup, to allow the
 * mode to be provided to any other function initialisations
 * that may rely on it.
 *
 * When a slave unit is powered on the state of GPIO7 is read.
 * If the reading is LOW, this indicates that the unit is a slave
 * and subsequently GPIO6 is set HIGH.
 * If the GPIO7 reading indicated that the unit is a master unit
 * the state of GPIO6 is then read and if it is LOW it determines
 * that the unit is a single unit.
 *
 * @return	The detected slave mode. If the mode has already been
 * 			detected the state will be available by referencing
 * 			the slaveModeStatus variable instead.
 */
extern slaveMode detectSlaveMode (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SLAVEMODE_H_ */
