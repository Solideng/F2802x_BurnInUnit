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
#define AC_I_CNTL	4	/**< The index position for AC I control settings. */
#define AC_STAGE	5	/**< The index position for AC stage settings. */
#define DC_STAGE	6	/**< The index position for DC stage  settings. */
#define V_MID_CH	7	/**< The index position for VMid settings. */
#define EXT_1 		8	/**< External temperature 1 channel reference. */
#define EXT_2 		9	/**< External temperature 2 channel reference. */

extern void mnSetupNets (void);

//** Initialises all channel settings structures with their default values.
// * @warning This MUST be called AFTER pwmMacroConfigure()
// */
//extern void mnSetupChannels (void);
//
//** Connects the macro terminals to the relevant nets.
// * This SHOULD be called AFTER DPL_Init()*/
//extern void mnConnectNets (void);

/** Disables and zeros all IIR filter control law reference inputs, thus causing their outputs to ramp down to zero. */
extern void mnStopAll (void);

/** Enables all IIR filter control law reference inputs. */
extern void mnRunAll (void);


#if (INCR_BUILD == 1)
	extern volatile int32 duty[NUM_CHNLS];	/* Open-loop duty for GUI input (Replaces Duty:n). */
#endif

extern Uint16 stopAll;		/**< Stop-all condition flag that allows status communication between the state machine tasks. */
extern Uint16 enableAll;	/**< Enable-all condition flag that allows status communication between the state machine tasks. */

enum extSelect {
	ext1 = 0,
	ext2 = 1,
	numberOfExts = 2
};

typedef enum extSelect extSelect;

struct extDeviceSettings {
	int32 ext1OtpLevel;		/**< External 1 OTP limit in @f$ ^\circ@f$ C (SQ7). */
	int32 ext2OtpLevel;		/**< External 2 OTP limit in @f$ ^\circ@f$ C (SQ7). */
	Uint16 extFanEnable;	/**< External fan enable status {TRUE | FALSE}. */
	Uint16 extPsuEnable;	/**< External PSU enable status {TRUE | FALSE}. */
};

typedef struct extDeviceSettings extDeviceSettings;

extern extDeviceSettings extSettings;

enum loadStage {
	load1 = 0,
	load2 = 1,
	load3 = 2,
	load4 = 3,
	numberOfLoads = 4
};

typedef enum loadStage loadStage;

struct loadStageNets {
	volatile int32 iFdbkNet;		/**< Current feedback net (IQ24). */
	volatile int32 vFdbkNet;		/**< Voltage feedback net (IQ24). */
	volatile int32 iRefNet;			/**< IIR filter control law current reference net (IQ24). */
	volatile int32 iFiltOutNet;		/**< IIR filter control law output net (IQ24). */
};

typedef struct loadStageNets loadStageNets;

extern loadStageNets loadNets[numberOfLoads];

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

typedef struct loadStageSettings loadStageSettings;

extern loadStageSettings loadSettings[numberOfLoads];

struct acStageNets {
	volatile int32 vRefNet;		/**< IIR filter control law voltage reference net (IQ24). */
	volatile int32 vFdbkNet;	/**< Voltage feedback net (IQ24). */
	volatile int32 iFdbkNet;	/**< Current feedback net (IQ24). */
	volatile int32 vFiltOutNet;	/**< Voltage IIR filter control law output net (IQ24). */
	volatile int32 iFiltOutNet;	/**< Current IIR filter control law output net (IQ24). */
};

typedef struct acStageNets acStageNets;

extern acStageNets acNets;

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

typedef struct acStageSettings acStageSettings;

extern acStageSettings acSettings;

struct xfmrStageNets {
	volatile int32 iSnsNet;
	volatile int32 midVSnsNet;
	volatile int32 hvVSnsNet;
	volatile int32 pwmDutyNet;
};

typedef struct xfmrStageNets xfmrStageNets;

extern xfmrStageNets xfmrNets;

struct xfmrStageSettings {
	int32 	ocpLevel;
	int32 	midOvpLevel;
	int32   hvOvpLevel;
	int32	otpLevel;
	int16 	iMax;
	int16 	midVMax;
	int16 	hvVMax;
	int16 	iScale;
	int16 	midVScale;
	int16 	hvVScale;
	Uint16 	enable;
};

typedef struct xfmrStageSettings xfmrStageSettings;

extern xfmrStageSettings xfmrSettings;

//** A structure used to represent the collection of settings pertaining
// * to a particular channel or stage.
// * Note that DPLib CNTL coefficient structures are handled separately to
// * reduce complexity as DPLib expects them to be arranged in a certain
// * manner in memory.
// */
//struct channelParameters {


#endif /* MACRONETS_H_ */
