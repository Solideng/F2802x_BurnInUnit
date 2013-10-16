/**
 * @file Otp.h
 *
 * @brief Over-temperature protection functions.
 *
 */

#ifndef OTP_H_
#define OTP_H_

#define LOAD1_OTP_TRIP 	1
#define LOAD2_OTP_TRIP 	2
#define LOAD3_OTP_TRIP 	4
#define LOAD4_OTP_TRIP 	8
#define DC_OTP_TRIP 	16
#define RSRVD_OTP_TRIP	32
#define AC_OTP_TRIP 	64
#define EXT1_OTP_TRIP 	128
#define EXT2_OTP_TRIP 	256

extern Uint16 otpFlagRegister;

/*============== Load n ==============*/
extern Uint16 setLoadOtp (loadStage load, float32 level);

extern Uint16 getLoadOtp (loadStage load, float32 *level);

extern Uint16 checkLoadOtp (loadStage load);

extern Uint16 getLoadOtpState (loadStage load);

extern Uint16 clearLoadOtp (loadStage load);

/*================ DC ===============*/
extern Uint16 setDcOtp (float32 level);

extern Uint16 getDcOtp (float32 *level);

extern Uint16 checkDcOtp (void);

extern Uint16 getDcOtpState (void);

extern Uint16 clearDcOtp (void);

/*================ AC ================*/
extern Uint16 setAcOtp (float32 level);

extern Uint16 getAcOtp (float32 *level);

extern Uint16 checkAcOtp (void);

extern Uint16 getAcOtpState (void);

extern Uint16 clearAcOtp (void);

/*================ Ext ===============*/
extern Uint16 setExtOtp (extSelect ext, float32 level);

extern Uint16 getExtOtp (extSelect ext, float32 *level);

extern Uint16 checkExtOtp (extSelect ext);

extern Uint16 getExtOtpState (extSelect ext);

extern Uint16 clearExtOtp (extSelect ext);

#endif /* OTP_H_ */
