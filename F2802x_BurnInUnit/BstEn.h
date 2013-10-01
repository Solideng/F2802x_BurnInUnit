/**
 * @file BstEn.h
 *
 * @brief Functions for enabling and disabling the boost converter stages via I2C.
 *
 * The converters are controlled via an external I/O
 * expander (MCP23008) that is connected to the I2C bus
 * at address 0100x-x-x where 'x-x-x' is dependent upon
 * the configuration of resistors R60 - 61 & R70 - R74.
 *
 * After bcInit() all converters default to disabled.
 *
 * @warning
 * Before any converter control functions can be used the
 * I2C peripheral MUST be initialised and EITHER bcInit()
 * or fcInit() MUST be run - bcInit() will require the
 * interrupts to be enabled globally.
 *
 * @sa i2cInit()
 * @sa fcInit()
 *
 */

#ifndef BSTEN_H_
#define BSTEN_H_

/*================== MACROS ===================*/
#ifndef I2C_H_
	#error "The fan control interface requires the file I2c.h to be included"
#endif

#define IOE_I2C_ADDR 		0x20	/**< MCP23008 I/O expander I2C address (slave, 32d, 8-bit I/O expander). */

#define	IOE_IODIR_ADDR		0x00	/**< MCP23008 I/O expander I/O direction register address. */
#define IOE_IPOL_ADDR		0x01	/**< MCP23008 I/O expander input polarity register address. */
#define IOE_GPINTEN_ADDR	0x02	/**< MCP23008 I/O expander interrupt on change enable register address. */
#define IOE_DEFVAL_ADDR		0x03	/**< MCP23008 I/O expander default value register address. */
#define IOE_INTCON_ADDR		0x04	/**< MCP23008 I/O expander interrupt on change control register address. */
#define IOE_IOCON_ADDR		0x05	/**< MCP23008 I/O expander configuration register address. */
#define IOE_GPPU_ADDR		0x06	/**< MCP23008 I/O expander pull-up resistor configuration register address. */
#define IOE_INTF_ADDR		0x07	/**< MCP23008 I/O expander interrupt flag register address. */
#define IOE_INTCAP_ADDR		0x08	/**< MCP23008 I/O expander interrupt capture register address. */
#define IOE_GPIO_ADDR		0x09	/**< MCP23008 I/O expander GPIO port register address. */
#define IOE_OLAT_ADDR		0x0A	/**< MCP23008 I/O expander output latch register address. */

#define BST_NUM_CHNL		0x04	/**< Number of boost converter channels. */

#ifdef FAN_NUM_CHNL
	#define IOE_NUM_CHNL BST_NUM_CHNL + FAN_NUM_CHNL	/**< Total number of MCP I/O expander channels. */
#else
	#define IOE_NUM_CHNL BST_NUM_CHNL	/**< Total number of MCP I/O expander channels. */
#endif

/*============= GLOBAL FUNCTIONS ==============*/
/** Initialises the boost converter enable control interface.
 * The I2C peripheral MUST be initialised before this function is used.
 * @sa i2cInit()
 * @return				Error status.
 */
extern Uint16 bcInit (void);

/** Enables the specified channel's boost converter.
 * The I2C peripheral and the boost converter enable controller
 * interface MUST be initialised before this function is used.
 * @sa i2cInit()
 * @sa bcInit()
 * @param[in]	chnl	Specifies the channel boost that is to be enabled.
 * @return				Error status.
 */
extern Uint16 bcEnable (Uint16 chnl);

/** Disables the specified channel's boost converter.
 * The I2C peripheral and the boost converter enable controller
 * interface MUST be initialised before this function is used.
 * @sa i2cInit()
 * @sa bcInit()
 * @param[in]	chnl	Specifies the channel boost that is to be disabled.
 * @return				Error status.
 */
extern Uint16 bcDisable (Uint16 chnl);

#endif /* BSTEN_H_ */
