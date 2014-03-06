#include	<msp430.h>
#include	<msp430.h>
#include	<stdint.h>
#include	"misc.h"

#ifndef _ADC_H
#define _ADC_H

#define	REFVSS	0
#define	REFVCC	50
#define	REF1_5	15
#define	REF2_5	25
#define	REFN	1
#define	REFE	2
//#define	REFNP	REFN
//#define	REFEP	10

#define	CONV_SCH	0
#define	CONV_MCH	1
#define	CONV_SCONV	0
#define	CONV_MCONV	2

#define	INCH_TEMP INCH_10

#if defined(ADC10CTL0_)
	#define ADC10_15P	1.5/1024
	#define ADC10_25P	2.5/1024
	#define ADC10_start	(ADC10CTL0 |= ENC + ADC10SC)
	#define ADC10_busy	(ADC10CTL1 & ADC10BUSY)
	#define ADC10_wait	{while(!(ADC10CTL1 & ADC10BUSY));}
#endif

extern inline void ADC_waitingRefSettle();

#if defined(ADC10CTL0_)
	//#warning ADC10!
	extern inline void ADC10_startMSC(unsigned int data[], unsigned char len);
	extern void ADC10_init(char port, char refN, char refP, char sht, char timing_source, char convMode, int (*ADC10_done_ISR)());
	extern float ADC10_toDegC_f(int v);
	extern int ADC10_toDegC (int v);
	extern int ADC10_result();
	extern int ADC10_result_2s();
	extern inline void avgFilter(int a[],char len, char cnt);
	extern void ADC10_set_ISR(int (*ISR_ptr)());
	//#pragma vector=ADC10_VECTOR
#endif
#endif