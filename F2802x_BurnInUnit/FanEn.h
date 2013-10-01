/**
 * @file FanEn.h
 *
 * @brief Functions for enabling and disabling the external fans via I2C
 *
 * The fans are controlled via an external I/O expander
 * (MCP23008) that is connected to the I2C bus at
 * address 0100x-x-x where 'x-x-x' is dependent upon
 * the configuration of resistors R60 - 61 & R70 - R74.
 *
 * After fcInit() all fans default to disabled.
 *
 * @warning
 * Before any fan control functions can be used the I2C
 * peripheral MUST be initialised and EITHER fcInit() or
 * bcInit() must be run - fcInit() will require the
 * interrupts to be enabled globally.
 *
 * @sa i2cInit()
 * @sa bcInit()
 *
 */

#ifndef FANEN_H_
#define FANEN_H_

/*================== MACROS ===================*/
#ifndef I2C_H_
	#error "The fan control interface requires the file I2c.h to be included"
#endif

#ifndef BSTEN_H_
	#define IOE_I2C_ADDR 		0x20	/**< MCP23008 I/O expander I2C address (slave, 32d, 8-bit I/O expander) */

	#define	IOE_IODIR_ADDR		0x00	/**< MCP23008 I/O expander I/O direction register address */
	#define IOE_IPOL_ADDR		0x01	/**< MCP23008 I/O expander input polarity register address */
	#define IOE_GPINTEN_ADDR	0x02	/**< MCP23008 I/O expander interrupt on change enable register address */
	#define IOE_DEFVAL_ADDR		0x03	/**< MCP23008 I/O expander default value register address */
	#define IOE_INTCON_ADDR		0x04	/**< MCP23008 I/O expander interrupt on change control register address */
	#define IOE_IOCON_ADDR		0x05	/**< MCP23008 I/O expander configuration register address */
	#define IOE_GPPU_ADDR		0x06	/**< MCP23008 I/O expander pull-up resistor configuration register address */
	#define IOE_INTF_ADDR		0x07	/**< MCP23008 I/O expander interrupt flag register address */
	#define IOE_INTCAP_ADDR		0x08	/**< MCP23008 I/O expander interrupt capture register address */
	#define IOE_GPIO_ADDR		0x09	/**< MCP23008 I/O expander GPIO port register address */
	#define IOE_OLAT_ADDR		0x0A	/**< MCP23008 I/O expander output latch register address */
#endif

#define FAN_NUM_CHNL			0x04	/**< Number of fan channels */
#define FAN_CHNL_OFST			0x04	/**< Fan channel numbering offset */

/*============= GLOBAL FUNCTIONS ==============*/
/** Initialises the fan enable control interface.
 * The I2C peripheral must be initialised before this function is used.
 * @sa i2cInit()
 * @return				Error status
 */
extern Uint16 fcInit (void);

/** Enables the specified channel's fan
 * The I2C peripheral and the fan enable controller interface
 * MUST be initialised before this function is used.
 * @sa i2cInit()
 * @sa fcInit()
 * @param[in]	chnl	Specifies the channel fan that is to be enabled
 * @return				Error status
 */
extern Uint16 fcEnable(Uint16 chnl);

/** Disables the specified channel's fan
 * The I2C peripheral and the fan enable controller interface
 * MUST be initialised before this function is used.
 * @sa i2cInit()
 * @sa fcInit()
 * @param[in]	chnl	Specifies the channel fan that is to be disabled
 * @return				Error status
 */
extern Uint16 fcDisable (Uint16 chnl);

#endif /* FANCTRL_H_ */
