/*
 * slaveMode.h
 *
 *  Created on: 3 Oct 2013
 *      Author: Toby
 */

#ifndef SLAVEMODE_H_
#define SLAVEMODE_H_

enum slaveMode {
	slave = 0,
	master = 1
};

typedef enum slaveMode slaveMode;

extern Uint16 initSlaveModeDetect (void);

extern slaveMode getSlaveMode (void);

extern Uint16 changeMode (slaveMode mode);

#endif /* SLAVEMODE_H_ */
