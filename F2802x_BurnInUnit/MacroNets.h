/**
 * @file MacroNets.h
 *
 * @brief DPLib macro net and value control functions.
 *
 */
#ifndef MACRONETS_H_
#define MACRONETS_H_

/*============= CHANNEL NUMBERING =============*/
#define LOAD_0		0	/**< The index position for Load 0 settings. */
#define LOAD_1		1	/**< The index position for Load 1 settings. */
#define LOAD_2		2	/**< The index position for Load 2 settings. */
#define LOAD_3		3	/**< The index position for Load 3 settings. */
#define AC_I_CNTL	4	/**< The index position for AC I control settings. */
#define DC_STAGE	5	/**< The index position for DC stage  settings. */
#define AC_STAGE	6	/**< The index position for AC stage settings. */
#define V_MID_CH	7	/**< The index position for VMid settings. */

/*============= GLOBAL FUNCTIONS ==============*/
/** Initialises all channel settings structures with their default values.
 * @warning This MUST be called AFTER pwmMacroConfigure()
 */
extern void mnSetupChannels (void);

/** Connects the macro terminals to the relevant nets.
 * This SHOULD be called AFTER DPL_Init()*/
extern void mnConnectNets (void);

/** Disables and zeros all IIR filter control law reference inputs, thus causing their outputs to ramp down to zero. */
extern void mnStopAll (void);

/** Enables all IIR filter control law reference inputs. */
extern void mnRunAll (void);

/*============= GLOBAL VARIABLES ==============*/
/** The possible settings for channel output settings. */
enum acOrDc {
	dc = 0, /**< DC channel setting (0). */
	ac = 1	/**< AC channel setting (1 or not-zero). */
};
/**  A type that allow specification of a channel's output mode setting. */
typedef enum acOrDc opType;

/** The possible settings for channel control setting. */
enum iOrVCtl {
	iCtrl = 0,	/**< Current control setting (0). */
	vCtrl = 1	/**< Voltage control setting (1 or not-zero). */
};
/**  A type that allow specification of a channel's control mode setting. */
typedef enum iOrVCtl ctlType;

/** A structure used to represent the collection of settings pertaining
 * to a particular channel or stage.
 * Note that DPLib CNTL coefficient structures are handled separately to
 * reduce complexity as DPLib expects them to be arranged in a certain
 * manner in memory.
 */
struct channelParameters {
	volatile int32 refNet;	/**< Net for CNTL reference (IQ24). */
	volatile int32 iFdbkNet;/**< Current feednack net (IQ24). */
	volatile int32 vFdbkNet;/**< Voltage feedback net (IQ24). */
	volatile int32 outNet;	/**< IIR filter control law output net (IQ24). */
	int32	ocp;		/**< Normalised OCP limit (IQ24). */
	int32	ovp;		/**< Normalised OVP limit (IQ24). */
	int32	target;		/**< IIR filter control law reference slew target (IQ24). */
	int32	slewRate; 	/**< IIR filter control law reference slew rate (IQ24). */
	int16	otp;		/**< OTP limit in @f$ ^\circ@f$ C (SQ7). */
//	int16	sMax;		/**< IIR filter control law maximum saturation setting (SQ14). */
//	int16	sMin;		/**< IIR filter control law minimum saturation setting (SQ14). */
	int16	iMaxRms;	/**< Maximum RMS current setting limit (SQ10). */
	int16	iMinRms;	/**< Minimum RMS current setting limit (SQ10). */
	int16	vMaxRms;	/**< Maximum RMS voltage setting limit (SQ10). */
	int16	vMinRms;	/**< Minimum RMS voltage setting limit (SQ10). */
	int16	iScale;		/**< Current scaling setting in volts-per-amp for scaling between a voltage level measured by an ADC to a real current value (SQ14). */
	int16	vScale;		/**< Voltage scaling setting in volts-per-volts for scaling between a voltage level measured by an ADC to a real voltage value (SQ14). */
	int16	vGainLmt;	/**< Sine signal generator voltage gain limit (SQ14). */
	int16	pGain; 		/**< CNTL P Gain (SQ8). */ /* these will need to be changed to b3-b0 & a3-a1. Maybe with a compile switch? */
	int16	iGain;		/**< CNTL I Gain (SQ8). */ /* also will need to be changed to floats, with conversion to IQ in assignment function. */
	int16	dGain;		/**< CNTL D Gain (SQ8). */
	opType	opMode;		/**< Output mode setting [dc | ac). */
	ctlType	ctlMode;	/**< Control mode setting [iCtrl | vCtrl). */
	Uint16	acFrequency;/**< Sine signal generator frequency setting (Hz). */
	Uint16	chEnable;	/**< Channel enable status [FALSE | TRUE). */
};
typedef struct channelParameters channelParameters;

#ifndef DUAL_CNTL_AC
	extern channelParameters channel[NUM_CHNLS + 1];	/**< A collection of the individual channel structures. */
	/* +1 is for VMid parameters which isn't a full channel. */
#else
	extern channelParameters channel[NUM_CHNLS + 1];	/**< A collection of the individual channel structures. */
	/* +1 is for VMid parameters which isn't a full channel. */
#endif

extern Uint16 stopAll;		/**< Stop-all condition flag that allows status communication between the state machine tasks. */
extern Uint16 enableAll;	/**< Enable-all condition flag that allows status communication between the state machine tasks. */

#if (INCR_BUILD == 1)
	extern volatile int32 duty[NUM_CHNLS];	/* Open-loop duty for Gui input (Replaces Duty:n). */
#endif

#endif /* MACRONETS_H_ */
