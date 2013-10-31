;-------------------------------------------------------------------------------
;	FILE:			ISR.asm
;
;	Description:	ISR for F2802x_BurnInUnit
;
;	Version: 		1.0
;
;   Target:  		TMS320F2802x (PiccoloA) families
;
;	Note:			Changes to this files will require a complete rebuild
;
;-------------------------------------------------------------------------------
;   Copyright Texas Instruments © 2004-2009
;-------------------------------------------------------------------------------
;   Revision History:
;-------------------------------------------------------------------------------
; 	Date		| Author	| Description
;-------------------------------------------------------------------------------
; 	2009 Dec 15	| DC       	| Created new file
; 	2010 Dec 05	| DC       	| Released file
;-------------------------------------------------------------------------------
		; Include C files that contain relevent symbols
	    .cdecls   C, LIST
	    %{
	    	//Gives peripheral addresses visibility in assembly
	    	#include "PeripheralHeaderIncludes.h"
	    	//Include C header file - sets INCR_BUILD (used in conditional builds)
	    	#include "Settings.h"
	    	//Include the signal generator module header
	    	#include "sgen.h"
	    	//Include the signal generator control header
	    	#include "SineGen.h"
    	%}

		; Identify symbols to be used that are defined elsewhere (e.g. in the included C headers)
		.ref _sgUpdate
		.ref _pcUpdate

		; Include asm files for the Power Library Macro's being used by the system
		.include "ADCDRV_1ch.asm"
		.include "CNTL_2P2Z.asm"
		.if $defined(AC_V_3P3Z)
			.include "CNTL_3P3Z.asm"
		.endif
		.include "PWMDRV_2ch_UpCnt.asm"

;**********************************************************************************
; Declare Public functions for External Reference
;**********************************************************************************
		; label to DP initialisation function
		.def _DPL_Init	

;**********************************************************************************
; Variable declaration
;**********************************************************************************	
; All Terminal modules initially point to the ZeroNet to ensure a known
; start state. Pad extra locations to accomodate unwanted ADC results.
		; dummy variable for pointer initialisation
ZeroNet	.usect "ZeroNet_Section",2,1,1	; output terminal 1
		; time slice counter variable
tsPtr	.usect "ISR_Section",1,1,1
		; sine gen update skip variable
sgntPtr	.usect "ISR_Section",1,1,1

				.text
;---------------------------------------------------------
; ISR Initialisation
;---------------------------------------------------------
_DPL_Init:

; Clear the ZeroNet
	MOVL 	XAR2,#ZeroNet
	RPT		#7 ; 8 times
	||MOV	*XAR2++, #0

	;---------------------------------------------------------
	.if(INCR_BUILD = 1) ; Open-Loop
		ADCDRV_1ch_INIT 1		; LOAD 1 ISNS
		ADCDRV_1ch_INIT 2		; LOAD 2 ISNS
		PWMDRV_2ch_UpCnt_INIT 1	; EPWM1AB - LOAD 2 & LOAD 2 PWM OUT

		ADCDRV_1ch_INIT 3		; LOAD 3 ISNS
		ADCDRV_1ch_INIT 4		; LOAD 4 ISNS
		PWMDRV_2ch_UpCnt_INIT 2	; EPWM2AB - LOAD 3 & LOAD 4 PWM OUT

		ADCDRV_1ch_INIT 5		; XFMR ISNS
		ADCDRV_1ch_INIT 6		; AC ISNS
		PWMDRV_2ch_UpCnt_INIT 3	; EPWM3 - XFMR & AC STAGE PWM OUT

		ADCDRV_1ch_INIT 7		; LOAD 1 VSNS
		ADCDRV_1ch_INIT 8		; LOAD 2 VSNS
		ADCDRV_1ch_INIT 9		; LOAD 3 VSNS
		ADCDRV_1ch_INIT 10		; LOAD 4 VSNS
		ADCDRV_1ch_INIT 11		; HV VSNS
		ADCDRV_1ch_INIT 12		; AC VSNS
		ADCDRV_1ch_INIT 13		; MID VSNS

	.endif
	;---------------------------------------------------------
	.if(INCR_BUILD = 2) ; Closed-Loop 2P2Z
		; Initialize the time slicer
		MOVW	DP, #tsPtr
		MOV 	@tsPtr,#1	
		MOVW 	DP, #sgntPtr
		MOV 	@sgntPtr, #1

		ADCDRV_1ch_INIT 1		; LOAD 1 ISNS
		CNTL_2P2Z_INIT 1		; LOAD 1 ICNTL
		ADCDRV_1ch_INIT 2		; LOAD 2 ISNS
		CNTL_2P2Z_INIT 2		; LOAD 2 ICNTL
		PWMDRV_2ch_UpCnt_INIT 1	; EPWM1AB - LOAD 1 & LOAD 2 PWM OUT

		ADCDRV_1ch_INIT 3		; LOAD 3 ISNS
		CNTL_2P2Z_INIT 3		; LOAD 3 ICNTL
		ADCDRV_1ch_INIT 4		; LOAD 4 ISNS
		CNTL_2P2Z_INIT 4		; LOAD 4 ICNTL
		PWMDRV_2ch_UpCnt_INIT 2	; EPWM2AB - LOAD 3 & LOAD 4 PWM OUT

		ADCDRV_1ch_INIT 5		; XFMR ISNS
		ADCDRV_1ch_INIT 6		; AC ISNS
		CNTL_2P2Z_INIT 5		; AC ICNTL
		.if $defined(AC_V_3P3Z)
			CNTL_3P3Z_INIT 1	; AC VCNTL
		.else
			CNTL_2P2Z_INIT 6	; AC VCNTL
		.endif
		PWMDRV_2ch_UpCnt_INIT 3	; EPWM3AB - XMFR & AC STAGE PWM OUT

		ADCDRV_1ch_INIT 7		; LOAD 1 VSNS
		ADCDRV_1ch_INIT 8		; LOAD 2 VSNS
		ADCDRV_1ch_INIT 9		; LOAD 3 VSNS
		ADCDRV_1ch_INIT 10		; LOAD 4 VSNS
		ADCDRV_1ch_INIT 11		; HV VSNS
		ADCDRV_1ch_INIT 12		; AC VSNS
		ADCDRV_1ch_INIT 13		; MID VSNS
	.endif
	;---------------------------------------------------------
	.if(INCR_BUILD = 3) ; Closed-Loop PID
	.endif
	;---------------------------------------------------------

	LRETR

		.sect "ramfuncs"
		; label to DP ISR Run function
		.def	_DPL_ISR

;---------------------------------------------------------
; ISR Run
;---------------------------------------------------------
_DPL_ISR:	;(13 cycles to get to here from ISR trigger)
	;CONTEXT_SAVE
	PUSH AR1H:AR0H ; 32-bit	; Pushes low halfs of aux reg 0 & 1 ; 1 CYC :
	PUSH XAR2 ; 32-bit		; Pushes aux reg	; 1 CYC :
	PUSH XAR3 ; 32-bit		; Pushes aux reg	; 1 CYC :
	PUSH XAR4 ; 32-bit		; Pushes aux reg	; 1 CYC :
;-- Comment these to save cycles --------
	PUSH XAR5 ; 32-bit		; Pushes aux reg	; 1 CYC :
	PUSH XAR6 ; 32-bit		; Pushes aux reg	; 1 CYC :
	PUSH XAR7 ; 32-bit		; Pushes aux reg	; 1 CYC :
;----------------------------------------
	PUSH XT	  ; 32-bit		; Pushes multiplicand reg	; 1 CYC :
;	SPM   	0          				; set C28 mode
;	CLRC  	AMODE       
;	CLRC  	PAGE0,OVM 
	NOP											; 1 CYC :

	;---------------------------------------------------------
	.if(INCR_BUILD = 1) ; Open-Loop
		ADCDRV_1ch 1		; LOAD 1 ISNS
		ADCDRV_1ch 2		; LOAD 2 ISNS
		PWMDRV_2ch_UpCnt 1	; EPWM1AB - LOAD 1 & LOAD 2 PWM OUT

		ADCDRV_1ch 3		; LOAD 3 ISNS
		ADCDRV_1ch 4		; LOAD 4 ISNS
		PWMDRV_2ch_UpCnt 2	; EPWM2AB - LOAD 3 & LOAD 4 PWM OUT

		ADCDRV_1ch 5		; XFMR ISNS
		ADCDRV_1ch 6		; AC ISNS
		PWMDRV_2ch_UpCnt 3	; EPWM3AB - XFMR & AC STAGE PWM OUT

		ADCDRV_1ch 7		; LOAD 1 VSNS
		ADCDRV_1ch 8		; LOAD 2 VSNS
		ADCDRV_1ch 9		; LOAD 3 VSNS
		ADCDRV_1ch 10		; LOAD 4 VSNS
		ADCDRV_1ch 11		; HV VSNS
		ADCDRV_1ch 12		; AC VSNS
		ADCDRV_1ch 13		; MID VSNS
	.endif
	;---------------------------------------------------------
	.if(INCR_BUILD = 2) ; Closed-Loop 2P2Z
		MOVW	DP, #tsPtr	; Load the data page pointer with the page that contains 'tsPtr'	; 1 CYC :
		CMP		@tsPtr, #2	; Compare the data pointed to by 'tsPtr' to '2'	; 1 CYC :
		B		TS2, EQ		; Branch to TS2 if the CMP was equal	; 7 CYC :
TS1:
		; DO LOOPS FOR LOADS 1-4. CALLED EVERY SECOND TIME. PWMCLK/6
		INC		@tsPtr		; Increment the data pointed to by 'tsPtr' (from TS1 to TS2)	; 1 CYC :
							; Run these macros...
		ADCDRV_1ch 1		; LOAD 1 ISNS
		CNTL_2P2Z 1			; LOAD 1 ICNTL
		ADCDRV_1ch 2		; LOAD 2 ISNS
		CNTL_2P2Z 2			; LOAD 2 ICNTL
		PWMDRV_2ch_UpCnt 1	; EPWM1AB - LOAD 1 & LOAD 2 PWM OUT

		ADCDRV_1ch 3		; LOAD 3 ISNS
		CNTL_2P2Z 3			; LOAD 3 ICNTL
		ADCDRV_1ch 4		; LOAD 4 ISNS
		CNTL_2P2Z 4			; LOAD 4 ICNTL
		PWMDRV_2ch_UpCnt 2	; EPWM2AB - LOAD 3 & LOAD 4 PWM OUT
		LB 		TS_END		; Long branch to TS_END	; 4 CYC :
TS2:
		;DO LOOPS FOR XFMR & AC STAGE. CALLED EVERY SECOND TIME. PWMCLK/6
							; Run these macros...
		ADCDRV_1ch 5		; XFMR ISNS
							; Sine gen runs at PWMCLK/12
		MOVW DP, #sgntPtr	; Load the data page pointer with the page that contains 'sgntPtr'
		CMP @sgntPtr, #1	; Compare the data pointed to by 'sgntPtr' to '1'
		B SGNTSKP, EQ		; Branch to SGNTSKP if the CMP was equal so as to skip the following two instructions
		MOV @sgntPtr, #0	; Set 'sgntPtr' to '0'
		LCR _updateSineSignal	; Run the function 'updateSineSignal()'

SGNTSKP:
		ADCDRV_1ch 6		; AC ISNS
		CNTL_2P2Z 5			; AC ICNTL
		.if $defined(AC_V_3P3Z)
			CNTL_3P3Z 1		; AC VCNTL
		.else
			CNTL_2P2Z 6		; AC VCTNL
		.endif
		PWMDRV_2ch_UpCnt 3	; EPWM3AB - XFMR AND AC STAGE PWM OUT

		MOVW	DP, #tsPtr	; Load the data page pointer with the page that contains 'tsPtr'		; 1 CYC :
		MOV 	@tsPtr,#1	; Move 1 into 'tsPtr' (change from TS2 to TS1)	; 1 CYC :
		MOVW DP, #sgntPtr	; Load the data page pointer with the page that contains 'sgntPtr'		; 1 CYC :
		INC 	@sgntPtr	; Increment 'sgntPtr'
TS_END:	
		ADCDRV_1ch 7		; LOAD 1 VSNS
		ADCDRV_1ch 8		; LOAD 2 VSNS
		ADCDRV_1ch 9		; LOAD 3 VSNS
		ADCDRV_1ch 10		; LOAD 4 VSNS
		ADCDRV_1ch 11		; HV VSNS
		ADCDRV_1ch 12		; AC VSNS
		ADCDRV_1ch 13		; MID VSNS
	.endif
	;---------------------------------------------------------
	.if(INCR_BUILD = 3) ; Closed-Loop PID
	.endif
	;---------------------------------------------------------

;===================================
EXIT_ISR
;===================================
; Interrupt management before exit
	MOVW 	DP,#_EPwm1Regs.ETCLR		; Load the data page pointer with the page that contains ETCLR				; 1 CYC :
	MOV 	@_EPwm1Regs.ETCLR,#0x01		; Clear EPWM1 Int flag	; Move 1 into the address pointed to by ETCLR	; 1 CYC :
	MOVW 	DP,#_PieCtrlRegs.PIEACK		; Load the data page pointer with tthe page that contains PIEACK value		; 1 CYC :
	MOV 	@_PieCtrlRegs.PIEACK, #0x4	; Acknowledge PIE interrupt Group 3	; Move 4 into the address pointed to by PIEACK	; 1 CYC :

; Restore context & return
	POP XT					; Pop multiplicand reg	; 1 CYC :
;-- Comment these to save cycles ---
	POP XAR7				; Pop aux reg	; 1 CYC :
	POP XAR6				; Pop aux reg	; 1 CYC :
	POP XAR5				; Pop aux reg	; 1 CYC :
;-----------------------------------
	POP XAR4				; Pop aux reg	; 1 CYC :
	POP XAR3				; Pop aux reg	; 1 CYC :
	POP XAR2				; Pop aux reg	; 1 CYC :
	POP AR1H:AR0H			; Pop low halfs of aux reg 0 & 1	; 1 CYC :

	IRET					; Return from interrupt	; 8 CYC :

