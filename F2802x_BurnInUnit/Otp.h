/**
 * @file Otp.h
 *
 * @brief Over-temperature protection functions.
 *
 */

#ifndef OTP_H_
#define OTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LOAD1_OTP_TRIP 	1	/**< OTP flag register load 1 bit. */
#define LOAD2_OTP_TRIP 	2	/**< OTP flag register load 2 bit. */
#define LOAD3_OTP_TRIP 	4	/**< OTP flag register load 3 bit. */
#define LOAD4_OTP_TRIP 	8	/**< OTP flag register load 4 bit. */
#define DC_OTP_TRIP 	16	/**< OTP flag register DC bit. */
#define RSRVD_OTP_TRIP	32	/**< Reserved bit. */
#define AC_OTP_TRIP 	64	/**< OTP flag register AC bit. */
#define EXT1_OTP_TRIP 	128	/**< OTP flag register external 1 bit. */
#define EXT2_OTP_TRIP 	256	/**< OTP flag register external 2 bit. */

extern uint16_t otpFlagRegister;	/** < OTP flag register. Bits are set to indicate an OTP condition has been found. */

/*============== Load n ==============*/
/** Sets the over temperature protection level for the specified load.
 * @param[in]	load	Specifies the load the setting is to be applied to.
 * @param[in]	level	Specifies the value to be applied (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t setLoadOtpLevel (loadStage load, float level);

/** Queries the over temperature protection level for the specified load.
 * @param[in]	load	Specifies the load on which the setting is to be queried.
 * @param[out]	level	Pointer to location at which to place the query result (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t getLoadOtpLevel (loadStage load, float *level);

/** Checks the temperature reading of the specified load against the load OTP limit.
 * Raises the load OTP flag if the reading is above the limit.
 * @param[in]	load	Specifies the load on which the reading is to be tested.
 * @return 				Error status
 */
extern uint16_t checkLoadOtp (loadStage load);

/** Queries the state of the OTP flag for the specified load.
 * @param[in]	load	Specifies the load on which to check the flag.
 * @return				True if OTP flag has been raised.
 */
extern uint16_t getLoadOtpState (loadStage load);

/** Clears the OTP state for the specified load.
 * @param[in]	load	Specifies the load for which the OTP state is to be cleared.
 * @return				Error status.
 */
extern uint16_t clearLoadOtp (loadStage load);

/*================ DC ===============*/
/** Sets the over temperature protection limit for the DC stage.
 * @param[in]	level	Specifies the value to be applied (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t setDcOtpLevel (float level);

/** Queries the over current protection level for the DC stage.
 * @param[out]	level	Pointer to location at which to place the query result (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t getDcOtpLevel (float *level);

/** Checks the temperature reading of the DC stage against the DC OTP limit.
 * Raises the DC OTP flag if the reading is above the limit.
 * @return 				Error status
 */
extern uint16_t checkDcOtp (void);

/** Queries the state of the DC OTP flag.
 * @return	True if OTP flag has been raised.
 */
extern uint16_t getDcOtpState (void);

/** Clears the DC stage OTP state.
 * @return				Error status.
 */
extern uint16_t clearDcOtp (void);

/*================ AC ================*/
/** Sets the over current protection limit for the AC stage.
 * @param[in]	level	Specifies the value to be applied (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t setAcOtpLevel (float level);

/** Queries the over temperature protection level for the AC stage.
 * @param[out]	level	Pointer to location at which to place the query result (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t getAcOtpLevel (float *level);

/** Checks the temperature reading of the AC stage against the AC OTP limit.
 * Raises the AC OTP flag if the reading is above the limit.
 * @return 				Error status
 */
extern uint16_t checkAcOtp (void);

/** Queries the state of the AC OTP flag.
 * @return	True if OTP flag has been raised.
 */
extern uint16_t getAcOtpState (void);

/** Clears the AC stage OTP state.
 * @return				Error status.
 */
extern uint16_t clearAcOtp (void);

/*================ Ext ===============*/
/** Sets the over temperature protection level for the specified external sensor.
 * @param[in]	ext		Specifies the sensor the setting is to be applied to.
 * @param[in]	level	Specifies the value to be applied (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t setExtOtpLevel (extSelect ext, float level);

/** Queries the over temperature protection level for the specified external sensor.
 * @param[in]	ext		Specifies the external sensor on which the setting is to be queried.
 * @param[out]	level	Pointer to location at which to place the query result (@f$ ^\circ@f$ C).
 * @return				Error status.
 */
extern uint16_t getExtOtpLevel (extSelect ext, float *level);

/** Checks the temperature reading of the specified external sensor against the sensor OTP limit.
 * Raises the load OTP flag if the reading is above the limit.
 * @param[in]	ext		Specifies the sensor on which the setting is to be queried.
 * @return 				Error status
 */
extern uint16_t checkExtOtp (extSelect ext);

/** Queries the state of the OTP flag for the specified external sensor.
 * @param[in]	ext	Specifies the sensor on which to check the flag.
 * @return				True if OTP flag has been raised.
 */
extern uint16_t getExtOtpState (extSelect ext);

/** Clears the OTP state for the specified external sensor.
 * @param[in]	ext	Specifies the external sensor for which the OTP state is to be cleared.
 * @return			Error status.
 */
extern uint16_t clearExtOtp (extSelect ext);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* OTP_H_ */
