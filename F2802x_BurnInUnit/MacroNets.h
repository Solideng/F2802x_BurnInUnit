/**
 * @file MacroNets.h
 *
 * @brief DPLib macro net and value control functions.
 *
 */
#ifndef MACRONETS_H_
#define MACRONETS_H_

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
extern void SetupNets (slaveMode mode);

/** Disables and zeros all IIR filter control law reference inputs, thus causing their outputs to ramp down to zero. */
extern void stopAll (void);

/** Enables all IIR filter control law reference inputs. */
extern void runAll (void);

#if (INCR_BUILD == 1)
	extern volatile int32 duty[NUM_CHNLS];	/* Open-loop duty for GUI input (Replaces Duty:n). */
#endif

extern Uint16 stopAllFlag;		/**< Stop-all condition flag that allows status communication between the state machine tasks. */
extern Uint16 enableAllFlag;	/**< Enable-all condition flag that allows status communication between the state machine tasks. */

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
	int32 ext1OtpLevel;		/**< External 1 OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int32 ext2OtpLevel;		/**< External 2 OTP limit in @f$ ^\circ@f$ C (SQ7). */
	Uint16 extFanEnable;	/**< External fan enable status {TRUE | FALSE}. */
	Uint16 extPsuEnable;	/**< External PSU enable status {TRUE | FALSE}. */
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
	volatile int32 iFdbkNet;		/**< Current feedback net (IQ24). */
	volatile int32 vFdbkNet;		/**< Voltage feedback net (IQ24). */
	volatile int32 iRefNet;			/**< IIR filter control law current reference net (IQ24). */
	volatile int32 iFiltOutNet;		/**< IIR filter control law output net (IQ24). */
};

/** A type to allow use of the load stage nets structure. */
typedef struct loadStageNets loadStageNets;

/** A global array instance of the load nets structure that collects the nets for all 4 loads. */
extern loadStageNets loadNets[numberOfLoads];

/** A structure that contains the settings that pertain to a single load stage. */
struct loadStageSettings {
	int32 	slewRate;	/**< IIR filter control law reference slew rate (IQ24). */
	int32 	slewTarget;	/**< IIR filter control law reference slew target (IQ24). */
	int32 	ocpLevel;	/**< Normalised OCP limit (IQ24). */
	int32 	ovpLevel;	/**< Normalised OVP limit (IQ24). */
	int32 	oppLevel;	/**< Normalised OPP limit (IQ22). */
	int32 	otpLevel;	/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int16 	iMax;		/**< Maximum DC current setting limit (SQ10). */
	int16 	vMax;		/**< Maximum DC voltage setting limit (SQ10). */
	int16 	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16 	vScale;		/**< Voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	Uint16 	enable;		/**< Channel enable status {FALSE, TRUE}. */
};

/** A type to allow use of the load stage settings structure. */
typedef struct loadStageSettings loadStageSettings;

/** A global array instance of the load settings structure that collects the settings for all 4 loads. */
extern loadStageSettings loadSettings[numberOfLoads];

/** A structure that contains the nets used in the transformer stage. */
struct xfmrStageNets {
	volatile int32 iSnsNet;		/**< Current sense net (IQ24). */
	volatile int32 midVSnsNet;	/**< DC mid voltage sense net (IQ24). */
	volatile int32 hvVSnsNet;	/**< DC HV voltage sense net (IQ24). */
	volatile int32 pwmDutyNet;	/**< Transformer PWM duty net (IQ24). */
};

/** A type to allow use of the transformer stage nets structure. */
typedef struct xfmrStageNets xfmrStageNets;

/** A global instance of the transformer nets structure. */
extern xfmrStageNets xfmrNets;

/** A structure that contains the settings that pertain to the transformer stage. */
struct xfmrStageSettings {
	int32 	ocpLevel;	/**< Normalised OCP limit (IQ24). */
	int32 	midOvpLevel;/**< Normalised DC mid OVP limit (IQ24). */
	int32   hvOvpLevel;	/**< Normalised DC HV OVP limit (IQ24). */
	int32	otpLevel;	/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int16 	iMax;		/**< Maximum DC current setting limit (SQ10). */
	int16 	midVMax;	/**< Maximum DC mid voltage setting limit (SQ10). */
	int16 	hvVMax;		/**< Maximum DC HV voltage setting limit (SQ10). */
	int16 	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16 	midVScale;	/**< DC mid voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	int16 	hvVScale;	/**< DC HV voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	Uint16 	enable;		/**< Channel enable status {FALSE, TRUE}. */
};

/** A type to allow use of the AC stage settings structure. */
typedef struct xfmrStageSettings xfmrStageSettings;

/** A global instance of the transformer settings structure. */
extern xfmrStageSettings xfmrSettings;

/** A structure that contains the nets used in the AC stage. */
struct acStageNets {
	volatile int32 vRefNet;		/**< IIR filter control law voltage reference net (IQ24). */
	volatile int32 vFdbkNet;	/**< Voltage feedback net (IQ24). */
	volatile int32 iFdbkNet;	/**< Current feedback net (IQ24). */
	volatile int32 iRefNet;		/**< IIR filter control law current reference net (IQ24). */
	volatile int32 iFiltOutNet;	/**< Current IIR filter control law output net (IQ24). */
};

/** A type to allow use of the AC stage nets structure. */
typedef struct acStageNets acStageNets;

/** A global instance of the AC nets structure. */
extern acStageNets acNets;

/** A structure that contains the settings that pertain to the AC stage. */
struct acStageSettings {
	int32 	gainRate;	/**< Reference sine gain rate (IQ24). */
	int32 	gainTarget;	/**< Reference sine gain target (IQ24). */
	int32 	ocpLevel;	/**< Normalised OCP limit (IQ24). */
	int32 	ovpLevel;	/**< Normalised OVP limit (IQ24). */
	int32 	otpLevel;	/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int16 	iMaxRms;	/**< Maximum RMS current setting limit (SQ10). */
	int16 	vMaxRms;	/**< Maximum RMS voltage setting limit (SQ10). */
	int16 	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16 	vScale;		/**< Voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	int16	vGainLmt;	/**< Sine signal generator voltage gain limit (SQ14). */
	slaveMode 	mode;	/**< Master or slave mode select. */
	Uint16 	acFrequency;/**< Sine signal generator frequency setting (Hz). */
	Uint16 	enable;		/**< Channel enable status {FALSE, TRUE}. */
};

/** A type to allow use of the AC stage settings structure. */
typedef struct acStageSettings acStageSettings;

/** A global instance of the AC settings structure. */
extern acStageSettings acSettings;

#endif /* MACRONETS_H_ */
