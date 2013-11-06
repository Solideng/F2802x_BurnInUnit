/**
 * @file I2c.h
 *
 * @brief Inter-integrated circuit communications functions.
 *
 * @warning
 * The following bridge tracks should be cut on the TI
 * C2000 LaunchPad XL PCB before I2C (or SPI) may be used:
 * |Bridge to Cut	| GPIO 	|PCB Pin|
 * |:--------------:|:-----:|:-----:|
 * | JP5			| GPIO32| J2-6	|
 * | JP7			| GPIO33| J2-7	|
 * | JP8			| GPIO16| J6-7	|
 * | JP10			| GPIO17| J6-8	|
 *
 * This should result in the following functionality:
 * |Function	| GPIO 	|PCB Pin|
 * |:----------:|:-----:|:-----:|
 * | I2C_SDAA	| GPIO32| J6-7	|
 * | I2C_SCLA	| GPIO33| J6-8	|
 * | SPI_MOSI	| GPIO16| J2-6	|
 * | SPI_MISO	| GPIO17| J2-7	|
 *
 * @warning
 * The function i2cInit() MUST be called before any other
 * public I2C function is used. This will clear any values
 * already in the I2C registers.
 *
 * @warning
 * Interrupts MUST be globally enabled for the functions i2cWrite()
 * and i2cRead() to operate correctly.
 *
 * @sa EnableCtrl.h
 * @sa Tmp.h
 *
 */

#ifndef I2C_H_
#define I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/*================== MACROS ===================*/
#define I2C_MAX_BUFFER_SIZE		0x04	/**< Maximum I2C message buffer size in bytes, including slave register pointer bytes. */
#define I2C_MAX_PTR_SIZE 		0x02	/**< Maximum number of slave register pointer bytes. */

/** @{ I2C Status Clear Bits */
#define I2C_CLR_AL_BIT          0x0001	/**< Arbitration lost status clear bit. */
#define I2C_CLR_NACK_BIT        0x0002	/**< NACK status clear bit. */
#define I2C_CLR_ARDY_BIT        0x0004	/**< Register access ready status clear bit. */
#define I2C_CLR_RRDY_BIT        0x0008	/**< Receive data ready status clear bit. */
#define I2C_CLR_SCD_BIT         0x0020	/**< Stop detected status clear bit. */
/**@}*/

/** @{ I2C Interrupt Sources */
//#define I2C_NO_ISRC             0x0000	/**< No I2C interrupt source. */
//#define I2C_ARB_ISRC            0x0001	/**< Arbitration lost condition I2C interrupt source. */
//#define I2C_NACK_ISRC           0x0002	/**< NACK response from slave during transmit condition I2C interrupt source. */
#define I2C_ARDY_ISRC           0x0003		/**< Register access ready condition I2C interrupt source. */
//#define I2C_RX_ISRC             0x0004	/**< Receive data ready condition I2C interrupt source. */
//#define I2C_TX_ISRC             0x0005	/**< Transmit data ready condition I2C interrupt source. */
#define I2C_SCD_ISRC            0x0006		/**< Stop detected condition I2C interrupt source. */
//#define I2C_AAS_ISRC            0x0007	/**< Addressed as a slave condition I2C interrupt source. */
/**@}*/

/** @{ I2C Message States */
#define I2C_MSGSTAT_INACTIVE          0x0000	/**< Inactive I2C message state. */
#define I2C_MSGSTAT_SEND_WITHSTOP     0x0010	/**< Transmit a write with stop I2C message state. */
#define I2C_MSGSTAT_WRITE_BUSY        0x0011	/**< State indicating the I2C is busy with a write with a stop. */
#define I2C_MSGSTAT_SEND_NOSTOP       0x0020	/**< Transmit a write with no stop. */
#define I2C_MSGSTAT_SEND_NOSTOP_BUSY  0x0021	/**< State indicating the I2C is busy with a write with no stop. */
#define I2C_MSGSTAT_RESTART           0x0022	/**< Transmit a master read with a restart. */
#define I2C_MSGSTAT_READ_BUSY         0x0023	/**< State indicating the I2C is busy with a read. */
/**@}*/

/*=============== GLOBAL VARS =================*/
/** The structure used to contain all settings and values relevant to a particular I2C message. */
typedef struct {
	volatile uint16_t msgStatus;		/**< Indicates which state the message is in. */
	uint16_t slaveAddress;			/**< The slave device I2C address this message is intended for. */
	uint16_t numOfBytes;				/**< The number of valid bytes in (or to be put in msgBuffer). */
	uint16_t numSlavePtrBytes;		/**< The number of slave register pointer address bytes. */
	uint16_t slavePtrAddrHigh;		/**< The slave register pointer high byte. */
	uint16_t slavePtrAddrLow;			/**< The slave register pointer low byte. */
	uint16_t msgBuffer[I2C_MAX_BUFFER_SIZE];	/**< A buffer array for message data. The maximum buffer size, MAX_BUFFER_SIZE, is 4 due to the FIFO's size. */
} i2cMsg;

/*============= GLOBAL FUNCTIONS ==============*/
/** Initialises the I2C-A peripheral and relevant interrupts.
 * @warning This function will clear any values already in the I2C peripheral registers.
 * @warning This function MUST be called before any other public I2C function.
 */
extern void initI2c (void);

/** This function can be used to validate and populate the specified settings and values into the specified I2C
 * message structure.
 * @param[out]	msg					The I2C message structure.
 * @param[in]	msgStatus			The initial I2C message status.
 * @param[in]	slaveAddr			The slave address.
 * @param[in]	numDataBytes		The number, if any, of data bytes, above any slave register pointer bytes, in
 * 									the message.
 * @param[in]	numSlavePtrBytes	The number, if any, of slave register pointer bytes.
 * @param[in]	slavePtrAddrHi		The slave register pointer high byte. If only one byte, or none, (as
 * 									indicated by numSlavePtrbytes) is to be used leave this at zero.
 * @param[in]	slavePtrAddrLo		The slave register pointer low byte. If no pointer bytes (as indicated by
 * 									numSlavePtrbytes) are used leave this at zero.
 */
extern void i2cPopMsg (i2cMsg *msg, uint16_t msgStatus, uint16_t slaveAddr, uint16_t numDataBytes, uint16_t numSlavePtrBytes, uint16_t slavePtrAddrHi, uint16_t slavePtrAddrLo);

/** Starts an I2C-A write using the settings and values	specified.
 * @param[in]	msg	The I2C message structure.
 * @return			Error Status.
 */
extern uint16_t i2cWrite (i2cMsg *msg);

/** Starts an I2C-A read using the settings specified.
 * Read bytes are saved to the buffer msg.msgBuffer[].
 * @param[in]	msg	The I2C message struct.
 * @return			Error status.
 */
extern uint16_t i2cRead (i2cMsg *msg);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* I2C_H_ */
