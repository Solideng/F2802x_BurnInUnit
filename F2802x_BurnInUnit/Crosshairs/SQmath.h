#ifndef _SQMATH_H
#define _SQMATH_H

/* 
 * The original example from Texas Instruments uses QN-format for many of the variables
 * (16 bits QMath variables, QN: N{1:15})
 * These macros convert from the int16 QN to IQN with the target Q-value
 * Since there exists no typedefs for int16 based QMath (or QN as it is refered to in the IQMath library)
 */

// Ranges and precision for SQ0 to SQ15
//																Binary min			Binary Max
//Format	 	Min			Max					Precision		Sign;whole;decimal	Sign;whole;decimal
//_sq15			-1			0.999969482			0.000030518		1;;000000000000000 	0;;111111111111111
//_sq14 		-2 			1.999938965			0.000061035		1;0;00000000000000	0;1;11111111111111
//_sq13 		-4 			3.999877930			0.000122070		1;00;0000000000000	0;11;1111111111111	
//_sq12 		-8 			7.999755859			0.000244141 	1;000;000000000000	0;111;111111111111
//_sq11 		-16 		15.999511719		0.000488281		1;0000;00000000000	0;1111;11111111111
//_sq10 		-32 		31.999023437		0.000976563		1;00000;0000000000	0;11111;1111111111
//_sq9 			-64 		63.998046875		0.001953125		1;000000;000000000	0;111111;111111111
//_sq8 			-128 		127.996093750		0.003906250		1;0000000;00000000	0;1111111;11111111
//_sq7 			-256 		255.992187500		0.007812500		1;00000000;0000000	0;11111111;1111111
//_sq6 			-512 		511.984375000		0.015625000		1;000000000;000000	0;111111111;111111
//_sq5 			-1024 		1023.968750000		0.031250000		1;0000000000;00000	0;1111111111;11111
//_sq4 			-2048 		2047.937500000		0.062500000		1;00000000000;0000	0;11111111111;1111
//_sq3 			-4096 		4095.875000000		0.125000000		1;000000000000;000	0;111111111111;111
//_sq2 			-8192 		8191.750000000		0.250000000		1;0000000000000;00	0;1111111111111;11
//_sq1 			-16384 		16383.500000000		0.500000000		1;90000000000000;0	0;11111111111111;1
//_sq0 			-32768 		32767  				1.000000000		1;000000000000000;	0;111111111111111;
  
#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int             int16;
typedef long            int32;
typedef unsigned int    Uint16;
typedef unsigned long   Uint32;
typedef float           float32;
typedef long double     float64;
#endif

#include <limits.h>

#ifndef GLOBAL_SQ
#define GLOBAL_SQ 15
#endif /* GLOBAL_SQ */
//---------------------------------------------------------------------------


typedef int16 _sq;
typedef int16 _sq15;
typedef int16 _sq14;
typedef int16 _sq13;
typedef int16 _sq12;  
typedef int16 _sq11;
typedef int16 _sq10;
typedef int16 _sq9;
typedef int16 _sq8;
typedef int16 _sq7;
typedef int16 _sq6;
typedef int16 _sq5;
typedef int16 _sq4;
typedef int16 _sq3;
typedef int16 _sq2;
typedef int16 _sq1;
typedef int16 _sq0;

//---------------------------------------------------------------------------
#define   _SQ15(A)      (int16) ((A) * 32768.0L)
#define   _SQ14(A)      (int16) ((A) * 16384.0L)
#define   _SQ13(A)      (int16) ((A) * 8192.0L)
#define   _SQ12(A)      (int16) ((A) * 4096.0L)
#define   _SQ11(A)      (int16) ((A) * 2048.0L)
#define   _SQ10(A)      (int16) ((A) * 1024.0L)
#define   _SQ9(A)       (int16) ((A) * 512.0L)
#define   _SQ8(A)       (int16) ((A) * 256.0L)
#define   _SQ7(A)       (int16) ((A) * 128.0L)
#define   _SQ6(A)       (int16) ((A) * 64.0L)
#define   _SQ5(A)       (int16) ((A) * 32.0L)
#define   _SQ4(A)       (int16) ((A) * 16.0L)
#define   _SQ3(A)       (int16) ((A) * 8.0L)
#define   _SQ2(A)       (int16) ((A) * 4.0L)
#define   _SQ1(A)       (int16) ((A) * 2.0L)
#define   _SQ0(A)		(int16) ((A) * 1.0L)
//---------------------------------------------------------------------------
#define _SQmpy2(A)          ((A)<<1)
#define _SQmpy4(A)          ((A)<<2)
#define _SQmpy8(A)          ((A)<<3)
#define _SQmpy16(A)         ((A)<<4)
#define _SQmpy32(A)         ((A)<<5)
#define _SQmpy64(A)         ((A)<<6)

#define _SQdiv2(A)          ((A)>>1)
#define _SQdiv4(A)          ((A)>>2)
#define _SQdiv8(A)          ((A)>>3)
#define _SQdiv16(A)         ((A)>>4)
#define _SQdiv32(A)         ((A)>>5)
#define _SQdiv64(A)         ((A)>>6)
//---------------------------------------------------------------------------
#if GLOBAL_SQ == 15
#define   _SQ(A)  _SQ15(A)
#endif
#if GLOBAL_SQ == 14
#define   _SQ(A)  _SQ14(A)
#endif
#if GLOBAL_SQ == 13
#define   _SQ(A)  _SQ13(A)
#endif
#if GLOBAL_SQ == 12
#define   _SQ(A)  _SQ12(A)
#endif
#if GLOBAL_SQ == 11
#define   _SQ(A)  _SQ11(A)
#endif
#if GLOBAL_SQ == 10
#define   _SQ(A)  _SQ10(A)
#endif
#if GLOBAL_SQ == 9
#define   _SQ(A)  _SQ9(A)
#endif
#if GLOBAL_SQ == 8
#define   _SQ(A)  _SQ8(A)
#endif
#if GLOBAL_SQ == 7
#define   _SQ(A)  _SQ7(A)
#endif
#if GLOBAL_SQ == 6
#define   _SQ(A)  _SQ6(A)
#endif
#if GLOBAL_SQ == 5
#define   _SQ(A)  _SQ5(A)
#endif
#if GLOBAL_SQ == 4
#define   _SQ(A)  _SQ4(A)
#endif
#if GLOBAL_SQ == 3
#define   _SQ(A)  _SQ3(A)
#endif
#if GLOBAL_SQ == 2
#define   _SQ(A)  _SQ2(A)
#endif
#if GLOBAL_SQ == 1
#define   _SQ(A)  _SQ1(A)
#endif

//---------------------------------------------------------------------------
// Convert SQ to IQ (With GlobalQ)
//---------------------------------------------------------------------------
#define _SQ15toIQ(A) _IQ15toIQ(A)
#define _SQ14toIQ(A) _IQ14toIQ(A)
#define _SQ13toIQ(A) _IQ13toIQ(A)
#define _SQ12toIQ(A) _IQ12toIQ(A)
#define _SQ11toIQ(A) _IQ11toIQ(A)
#define _SQ10toIQ(A) _IQ10toIQ(A)
#define _SQ9toIQ(A) _IQ9toIQ(A)
#define _SQ8toIQ(A) _IQ8toIQ(A)
#define _SQ7toIQ(A) _IQ7toIQ(A)
#define _SQ6toIQ(A) _IQ6toIQ(A)
#define _SQ5toIQ(A) _IQ5toIQ(A)
#define _SQ4toIQ(A) _IQ4toIQ(A)
#define _SQ3toIQ(A) _IQ3toIQ(A)
#define _SQ2toIQ(A) _IQ2toIQ(A)
#define _SQ1toIQ(A) _IQ1toIQ(A)
//---------------------------------------------------------------------------
// Convert IQ (With GlobalQ) to SQ
//---------------------------------------------------------------------------
#define _IQtoSQ15(A) _IQToIQ15(A)
#define _IQtoSQ14(A) _IQToIQ14(A)
#define _IQtoSQ13(A) _IQToIQ13(A)
#define _IQtoSQ12(A) _IQToIQ12(A)
#define _IQtoSQ11(A) _IQToIQ11(A)
#define _IQtoSQ10(A) _IQToIQ10(A)
#define _IQtoSQ9(A) _IQToIQ9(A)
#define _IQtoSQ8(A) _IQToIQ8(A)
#define _IQtoSQ7(A) _IQToIQ7(A)
#define _IQtoSQ6(A) _IQToIQ6(A)
#define _IQtoSQ5(A) _IQToIQ5(A)
#define _IQtoSQ4(A) _IQToIQ4(A)
#define _IQtoSQ3(A) _IQToIQ3(A)
#define _IQtoSQ2(A) _IQToIQ2(A)
#define _IQtoSQ1(A) _IQToIQ1(A)

#endif /* _SQMATH_H */

