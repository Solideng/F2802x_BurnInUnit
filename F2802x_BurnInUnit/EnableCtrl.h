/**
 * @file EnableCtrl.h
 *
 * @brief Functions for enabling and disabling circuit sections via I2C
 *
 * Circuit sections are controlled via an external I/O
 * expander (MCP23008) that is connected to the I2C bus at
 * address 0100xx0 where 'xx' is determined by the state
 * of switch 4 (SW4) on the Ctrl PCB, which the user should
 * record in the macros IOE_SW4A_STATE and IOE_SW4B_STATE.
 *
 * After initialisation or reset all sections default to disabled.
 *
 * @warning
 * Before any enable control functions can be used the I2C
 * peripheral MUST be initialised, followed by enable control
 * initialisation, both of which will require global interrupts
 * to be enabled.
 *
 * @sa initI2c()
 *
 */

#ifndef ENABLECTRL_H_
#define ENABLECTRL_H_

#ifndef I2C_H_						/* Check that I2C functionality is included in build. */
	#error "The enable control interface requires the file I2c.h to be included"
#endif

#define IOE_SW4A_STATE 		0x00	/**< The state of switch 4a; ON = 0x01, OFF = 0x00. */
#define IOE_SW4B_STATE 		0x00	/**< The state of switch 4b; ON = 0x01, OFF = 0x00. */
#define IOE_I2C_BASE_ADDR	0x20	/**< MCP23008 I/O expander base I2C address. */

#if !defined IOE_I2C_BASE_ADDR || !defined IOE_SW4A_STATE || !defined IOE_SW4B_STATE	/* Check the device address has been set properly. */
	#error "The enable control (MCP23008) I2C address has not been correctly specified."
#else
	#define IOE_I2C_ADDR 	(IOE_I2C_BASE_ADDR | (IOE_SW4A_STATE << 2)) | (IOE_SW4B_STATE << 3)	/**< MCP23008 I/O expander complete I2C address. */
#endif

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
#define IOE_MAX_CHNL 		0x08	/**< MCP23008 I/O expander number of channels (0 - 7). */

/** The possible enable control circuit section selections. */
enum ecSection {
	chan1 = 0,
	chan2 = 1,
	chan3 = 2,
	chan4 = 3,
	hvCct = 4,
	acCct = 5,
	psu = 6,
	fan = 7,
	maxchan = IOE_MAX_CHNL
};

/**  A type that allow specification of an enable control circuit section. */
typedef enum ecSection circuitSection;

/** Initialises the enable control interface.
 * The I2C peripheral MUST be initialised before this function is used.
 * @sa i2cInit()
 * @return				Error status.
 */
extern Uint16 initEnableControl (void);

/** Resets and reinitialises the MCP23008 I/O Expander device. */
extern void resetEnableControl (void);

/** Enables the specified circuit section enable signal.
 * @param[in]	section	Specifies the circuit section that is to be enabled.
 * @return				Error status.
 */
extern Uint16 enableCircuit (circuitSection section);

/** Disables the specified circuit section enable signal.
 * @param[in]	section	Specifies the circuit section that is to be disabled.
 * @return				Error status.
 */
extern Uint16 disableCircuit (circuitSection section);

#endif /* ENABLECTRL_H_ */
