/**
 * @file Tmp.h
 *
 * @brief Temperature sensor functions.
 *
 * The temperature sensor (MCP9701) output is read via
 * an external ADC (ADS7830) that is connected to the
 * I2C bus at address 10010xx where 'xx' is dependent
 * upon the configuration of resistors R75 - R78.
 * All temperatures are in degrees Celcius.
 *
 * @warning
 * Before any temperature functions can be used the I2C
 * peripheral MUST be initialised and tmpInit() must be
 * run - tmpInit() will require the interrupts to be
 * enabled globally.
 *
 * @sa i2cInit()
 *
 */

#ifndef TMP_H_
#define TMP_H_

#ifndef I2C_H_
	#error "The temperature sensor interface requires the file I2c.h to be included"
#endif

#define TMP_OTP_MIN		0x00	/**< Minimum over temperature limit setting (0@f$ ^\circ@f$ C). */  /* 0 degree C */
#define TMP_OTP_MAX		0x96	/**< Maximum over temperature limit setting (150@f$ ^\circ@f$ C). */ /* 150 degree C */

#define ADC_I2C_ADDR 	0x48	/**< Slave I2C address (ADS7830 8-channel ADC - A0 = 0, A1 = 0). */
#define ADC_NUM_CHNL	0x08	/**< Number of temperature channels.
								 * The program expects a 50/50 split with first half for the on-board temperature channels. */

#define ADC_VREF		5.0		/**< ADS7830 ADC reference voltage (volts). */
#define ADC_STPS		256		/**< Number of ADS7830 ADC steps. */

#define TMP_V0C_OFST	0.4		/**< MCP9701 Temperature sensor @f$ V_{0{^\circ}C}@f$ (volts). */
#define TMP_SCL_OFST	TMP_V0C_OFST * ADC_STPS / ADC_VREF	/**< Scaled temperature offset. */

#define TMP_T_COLD		-15.0	/**< MCP9701 Lowest operating temperature (@f$ ^\circ@f$ C). */ /* degree C */
#define TMP_T_HOT		125.0	/**< MCP9701 Highest operating temperature (@f$ ^\circ@f$ C). */ /* degree C */
#define	TMP_E_T_COLD	1.5		/**< MCP9701 Error at lowest operating temperature (@f$ ^\circ@f$ C), calculated as shown in Microchip AN1001. */
#define TMP_EC1			1E-3	/**< First order MCP9701 temperature error coefficient (@f$ ^\circ@f$ C/@f$ ^\circ C^2@f$), calculated as shown in Microchip AN1001. */ /* degree C / degree C^2 */
#define TMP_EC2			-200E-6	/**< Second order MCP9701 temperature error coefficient (@f$ ^\circ@f$ C/@f$ ^\circ C^2@f$), calculated as shown in Microchip AN1001. */ /* degree C / degree C^2 */

/** Initialises the system for temperature readings.
 * The I2C peripheral must be initialised before this function is used
 * @sa i2cInit().
 * @return				Error status.
 */
extern Uint16 tmpInit (void);

/** Sets the on-board over temperature limit for the specified channel.
 * The I2C peripheral and temperature reading interface
 * MUST be initialised before this function is used.
 * @param[in]	chnl	Specifies the channel the setting is to be applied to.
 * @param[in]	tmp		Specifies the value of the limit to be applied (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern Uint16 tmpSetOtp (Uint16 chnl, float32 tmp);

/** Queries the on-board over temperature limit for the specified channel.
 * The I2C peripheral and temperature reading interface
 * MUST be initialised before this function is used.
 * @param[in]	chnl	Specifies the channel the setting is to be read from.
 * @param[out]	tmpDest	Address of the memory location at which to place the query result (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern Uint16 tmpGetOtp (Uint16 chnl, float32 *tmpDest);

/** Tests the current on-board temperature sensor readings against the OTP limits.
 * @return				Error status.
 */
extern Uint16 tmpCheckOtp (void);

/** Queries the current on-board temperature of the specified channel.
 * @param[in]	chnl	Specifies the channel the temperature is to be read from.
 * @param[out]	tmpDest	Address of the memory location at which to place the query result (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern Uint16 tmpRead (Uint16 chnl, float32 *tmpDest);

/* The above functions use only on-board temperature sensors, the following
 * functions will need to be implemented to allow of-board sensors to be added
 */
/* extern Uint16 tmpSetExtOtp (Uint16 chnl, float32 tmp); */
/* extern Uint16 tmpGetExtOtp (Uint16 chnl, float32 *tmpDest); */
/* extern Uint16 tmpCheckExtOtp (void); */

#endif /* TMP_H_ */
