/**
 * @file MacroNets.h
 *
 * @brief DPLib macro net and value control functions.
 *
 */
#ifndef MACRONETS_H_
#define MACRONETS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
	#define AC 0.0f
	#define DC 1.0f
	extern float debugSettings[3];		/* Index 0: AC or DC Voltage mode for the AC stage. 0 = AC, 1=DC.
	 	 	 	 	 	 	 	 	 	 * Index 1: Voltage value, RMS if AC mode or fixed peak if DC mode.
	 	 	 	 	 	 	 	 	 	 * 			Sign of value indicates state of phase signal when DC mode is enabled.
	 	 	 	 	 	 	 	 	 	 * Index 2: I_limit value to set AC V Coeff max value with.
	 	 	 	 	 	 	 	 	 	 */
	extern void updateDebugSettings (void);
#endif

#define LOAD_0		0	/**< The index position for Load 0 settings. */
#define LOAD_1		1	/**< The index position for Load 1 settings. */
#define LOAD_2		2	/**< The index position for Load 2 settings. */
#define LOAD_3		3	/**< The index position for Load 3 settings. */
#define XFMR_STAGE	4	/**< The index position for transformer stage settings. */
#define AC_STAGE	5	/**< The index position for AC stage settings. */
#define EXT_1 		6	/**< External temperature 1 channel reference. */
#define EXT_2 		7	/**< External temperature 2 channel reference. */

/** Initialises macro settings and nets.
 * IIR control law coefficients are handled outside of this function.
 * @warning This MUST be called after DPL_INIT()
 * @warning This MUST be called after pwmMacroConfig()
 * @param[in] 	mode	Selects the test unit mode to allow correct macro net connection.
 */
extern void setupNets (slaveMode mode);

/** Disables all circuit sections, zeroes all IIR references and flags each stage as disabled. */
extern void stopAll (void);

/** Enables all IIR filter control law reference inputs.
 * @return	Error status.
 */
extern uint16_t runAll (void);

#if (INCR_BUILD == 1)
	extern volatile int32_t duty[NUM_CHNLS];	/* Open-loop duty for GUI input (Replaces Duty:n). */
#endif

extern uint16_t stopAllFlag;	/**< Stop-all condition flag that allows status communication between the state machine tasks. */
extern uint16_t enableAllFlag;	/**< Enable-all condition flag that allows status communication between the state machine tasks. */

/** The possible external sensor selections. */
enum extSelect {
	ext1 = 0,			/**< External sensor 1. */
	ext2 = 1,			/**< External sensor 2. */
	numberOfExts = 2
};

/**  A type that allow specification of an external sensor. */
typedef enum extSelect extSelect;

/** A structure that contains the settings that pertain to external devices. */
struct extDeviceSettings {
	int32_t ext1OtpLevel;	/**< External 1 OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int32_t ext2OtpLevel;	/**< External 2 OTP limit in @f$ ^\circ@f$ C (SQ7). */
	uint16_t extFanEnable;	/**< External fan enable status {TRUE | FALSE}. */
	uint16_t extPsuEnable;	/**< External PSU enable status {TRUE | FALSE}. */
};

/** A type to allow use of the external device settings structure. */
typedef struct extDeviceSettings extDeviceSettings;

/** A global instance of the external device settings structure. */
extern extDeviceSettings extSettings;

/** The possible load stage selections. */
enum loadStage {
	load1 = 0,			/**< Load 1. */
	load2 = 1,			/**< Load 2. */
	load3 = 2,			/**< Load 3. */
	load4 = 3,			/**< Load 4. */
	numberOfLoads = 4
};

/**  A type that allow specification of a load. */
typedef enum loadStage loadStage;

/** A structure that contains the nets used in a single load stage. */
struct loadStageNets {
	volatile int32_t iFdbkNet;		/**< Current feedback net (IQ24). */
	volatile int32_t vFdbkNet;		/**< Voltage feedback net (IQ24). */
	volatile int32_t iRefNet;		/**< IIR filter control law current reference net (IQ24). */
	volatile int32_t iFiltOutNet;	/**< IIR filter control law output net (IQ24). */
};

/** A type to allow use of the load stage nets structure. */
typedef struct loadStageNets loadStageNets;

/** A global array instance of the load nets structure that collects the nets for all 4 loads. */
extern loadStageNets loadNets[numberOfLoads];

/** A structure that contains the settings that pertain to a single load stage. */
struct loadStageSettings {
	int32_t 	slewRate;	/**< IIR filter control law reference slew rate (IQ24). */
	int32_t 	slewTarget;	/**< IIR filter control law reference slew target (IQ24). */
	int32_t 	ocpLevel;	/**< Normalised OCP limit (IQ24). */
	int32_t 	ovpLevel;	/**< Normalised OVP limit (IQ24). */
	int32_t 	oppLevel;	/**< Normalised OPP limit (IQ24). */
	int32_t 	otpLevel;	/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int16_t 	iMax;		/**< Maximum DC current setting limit (SQ10). */
	int16_t 	vMax;		/**< Maximum DC voltage setting limit (SQ10). */
	int16_t 	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16_t 	vScale;		/**< Voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	uint16_t 	enable;		/**< Channel enable status {FALSE, TRUE}. */
};

/** A type to allow use of the load stage settings structure. */
typedef struct loadStageSettings loadStageSettings;

/** A global array instance of the load settings structure that collects the settings for all 4 loads. */
extern loadStageSettings loadSettings[numberOfLoads];

/** A structure that contains the nets used in the transformer stage. */
struct xfmrStageNets {
	volatile int32_t iSnsNet;		/**< Current sense net (IQ24). */
	volatile int32_t midVSnsNet;	/**< DC mid voltage sense net (IQ24). */
	volatile int32_t hvVSnsNet;	/**< DC HV voltage sense net (IQ24). */
	volatile int32_t pwmDutyNet;	/**< Transformer PWM duty net (IQ24). */
};

/** A type to allow use of the transformer stage nets structure. */
typedef struct xfmrStageNets xfmrStageNets;

/** A global instance of the transformer nets structure. */
extern xfmrStageNets xfmrNets;

/** A structure that contains the settings that pertain to the transformer stage. */
struct xfmrStageSettings {
	int32_t 	ocpLevel;	/**< Normalised OCP limit (IQ24). */
//	int32_t 	midOvpLevel;/**< Normalised DC mid OVP limit (IQ24). */
	int32_t 	hvOvpLevel;	/**< Normalised DC HV OVP limit (IQ24). */
	int32_t		otpLevel;	/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int16_t 	iMax;		/**< Maximum DC current setting limit (SQ10). */
	int16_t 	midVMax;	/**< Maximum DC mid voltage setting limit (SQ10). */
	int16_t 	hvVMax;		/**< Maximum DC HV voltage setting limit (SQ10). */
	int16_t 	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16_t 	midVScale;	/**< DC mid voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	int16_t 	hvVScale;	/**< DC HV voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	uint16_t 	enable;		/**< Channel enable status {FALSE, TRUE}. */
};

/** A type to allow use of the AC stage settings structure. */
typedef struct xfmrStageSettings xfmrStageSettings;

/** A global instance of the transformer settings structure. */
extern xfmrStageSettings xfmrSettings;

/** A structure that contains the nets used in the AC stage. */
struct acStageNets {
	volatile int32_t vRefNet;		/**< IIR filter control law voltage reference net (IQ24). */
	volatile int32_t vFdbkNet;		/**< Voltage feedback net (IQ24). */
	volatile int32_t iFdbkNet;		/**< Current feedback net (IQ24). */
	volatile int32_t iRefNet;		/**< IIR filter control law current reference net (IQ24). */
	volatile int32_t iFiltOutNet;	/**< Current IIR filter control law output net (IQ24). */
};

/** A type to allow use of the AC stage nets structure. */
typedef struct acStageNets acStageNets;

/** A global instance of the AC nets structure. */
extern acStageNets acNets;

/** A structure that contains the settings that pertain to the AC stage. */
struct acStageSettings {
	int32_t 	gainRate;	/**< Reference sine gain rate (IQ24). */
	int32_t 	gainTarget;	/**< Reference sine gain target (IQ24). */
	int32_t 	ocpLevel;	/**< Normalised OCP limit (IQ24). */
	int32_t 	ovpLevel;	/**< Normalised OVP limit (IQ24). */
	int32_t 	otpLevel;	/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int16_t 	iMaxRms;	/**< Maximum RMS current setting limit (SQ10). */
	int16_t 	vMaxRms;	/**< Maximum RMS voltage setting limit (SQ10). */
	int16_t 	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16_t 	vScale;		/**< Voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	int16_t		vGainLmt;	/**< Sine signal generator voltage gain limit (SQ14). */
	slaveMode 	mode;		/**< Master or slave mode select. */
	uint16_t 	acFrequency;/**< Sine signal generator frequency setting (Hz). */
	uint16_t 	enable;		/**< Channel enable status {FALSE, TRUE}. */
};

/** A type to allow use of the AC stage settings structure. */
typedef struct acStageSettings acStageSettings;

/** A global instance of the AC settings structure. */
extern acStageSettings acSettings;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MACRONETS_H_ */
