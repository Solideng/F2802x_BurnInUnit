/**
 * @file SCPI_specificRegs.h
 *
 * This file creates the application specific
 * questionable status registers and provides the
 * functions for managing them
 *
 * These registers and their bits can be changed by
 * by the user to suit the application in use, though
 * they should follow the guidelines and conventions
 * laid out in IEEE 488.2-1992 and SCPI-1999
 *
 * Please follow the information in the parser
 * documentation and the file SCPI_specificRegs.c
 * on how to add or change questionable event status
 * registers
 *
 */

#ifndef SCPI_SPECIFICREGS_H_
#define SCPI_SPECIFICREGS_H_

#ifdef __cplusplus
	extern "C" {
#endif

struct VOLT_BITS {
	Uint16 	VSOR:1;	// Setting out of range
	Uint16 	VROL:1;	// Reading over limit
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
	Uint16 	rsvd10:1;
	Uint16 	rsvd11:1;
	Uint16 	rsvd12:1;
	Uint16 	rsvd13:1;
	Uint16 	rsvd14:1;
};

union VOLT_REG {
	Uint16 			all;
	struct VOLT_BITS bit;
};
extern union VOLT_REG VoltageEventStatus;

struct CURR_BITS {
	Uint16 	ISOR:1;	// Setting out of range
	Uint16 	IROL:1;	// Reading over limit
	Uint16 	IEOL:1;	// External over limit condition (interrupt)
	Uint16 	ODOR:1;	// OCP duty setting out of range
	Uint16 	OLOR:1;	// OCP level setting out of range
	Uint16 	OLOU:1;	// OCP level setting out of user range
	Uint16 	OCOR:1;	// OCP conversion factor out of range
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
};

union CURR_REG {
	Uint16 			all;
	struct CURR_BITS bit;
};
extern union CURR_REG CurrentEventStatus;

struct PULS_BITS {
	Uint16 	OSOR:1;	// OCT Setting out of range
	Uint16 	DSOR:1;	// DAC Setting out of range
	Uint16 	FSOR:1;	// Frequency setting out of range
	Uint16 	POHM:1; // Pulse width over device hardware maximum
	Uint16 	PBTI:1;	// Pulse width below T_inh
	Uint16 	POPM:1;	// Pulse width over period maximum
	Uint16 	PODM:1;	// Pulse width over duty maximum
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
};

union PULS_REG {
	Uint16 			all;
	struct PULS_BITS bit;
};
extern union PULS_REG PulseEventStatus;

struct TEMP_BITS {
	Uint16 	TSOR:1;	// Setting out of range
	Uint16 	TROL:1;	// Reading over limit
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
	Uint16 	rsvd10:1;
	Uint16 	rsvd11:1;
	Uint16 	rsvd12:1;
	Uint16 	rsvd13:1;
	Uint16 	rsvd14:1;
};

union TEMP_REG {
	Uint16 			all;
	struct TEMP_BITS bit;
};
extern union TEMP_REG TemperatureEventStatus;

struct SPI_BITS {
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
	Uint16 	rsvd10:1;
	Uint16 	rsvd11:1;
	Uint16 	rsvd12:1;
	Uint16 	rsvd13:1;
	Uint16 	rsvd14:1;
	Uint16 	rsvd15:1;
	Uint16 	rsvd16:1;
};

union SPI_REG {
	Uint16 			all;
	struct SPI_BITS bit;
};
extern union SPI_REG SpiEventStatus;

struct SCI_BITS {
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
	Uint16 	rsvd10:1;
	Uint16 	rsvd11:1;
	Uint16 	rsvd12:1;
	Uint16 	rsvd13:1;
	Uint16 	rsvd14:1;
	Uint16 	rsvd15:1;
	Uint16 	rsvd16:1;
};

union SCI_REG {
	Uint16 			all;
	struct SCI_BITS bit;
};
extern union SCI_REG SciEventStatus;

struct IIC_BITS {
	Uint16 	ALOS:1;	// Arbitration Lost
	Uint16 	NACK:1;	// NACK received
	Uint16 	BBSY:1;	// Bus Busy
	Uint16 	SNR:1;	// Stop not ready
	Uint16 	NFL:1;	// No flags
	Uint16 	MBSY:1;	// Message busy
	Uint16 	RWM:1;	// Read wrong message
	Uint16 	WWM:1;	// Write wrong message
	Uint16 	IIS:1;	// Invalid interrupt source
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
};

union IIC_REG {
	Uint16 			all;
	struct SCI_BITS bit;
};
extern union IIC_REG IicEventStatus;

struct ETH_BITS {
	Uint16 	rsvd1:1;
	Uint16 	rsvd2:1;
	Uint16 	rsvd3:1;
	Uint16 	rsvd4:1;
	Uint16 	rsvd5:1;
	Uint16 	rsvd6:1;
	Uint16 	rsvd7:1;
	Uint16 	rsvd8:1;
	Uint16 	rsvd9:1;
	Uint16 	rsvd10:1;
	Uint16 	rsvd11:1;
	Uint16 	rsvd12:1;
	Uint16 	rsvd13:1;
	Uint16 	rsvd14:1;
	Uint16 	rsvd15:1;
	Uint16 	rsvd16:1;
};

union ETH_REG {
	Uint16 			all;
	struct ETH_BITS bit;
};
extern union ETH_REG EthernetEventStatus;
extern Uint16 registerQuestionableRegs(void);

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /* SCPI_SPECIFICREGS_H_ */
