#include "adc.h"

int (*ACD10_ISR_ptr)();

inline void ADC_waitingRefSettle(){
	
	__delay_cycles(40);
}

inline void ADC10_startMSC(unsigned int data[], unsigned char len){
	ADC10DTC1 = len;  
	//__delay_cycles(40);
	ADC10SA = (unsigned int)data;
    ADC10CTL0 |= ENC + ADC10SC;
}

void ADC10_init(char port, char refN, char refP, char sht, char timing_source, char convMode, int (*ADC10_done_ISR)()) { // ~387~414 CLKs;
	
	char temp_char;
	
	temp_char = ADC10CTL0 & REFON;
	
	ADC10CTL1 = INCH0 * port;
	
	//defualt clock 
	ADC10CTL0 = ADC10ON + ADC10SSEL_0 + ADC10DIV_0 ; 
	
	//timing
	ADC10CTL1 |= SHS0 * timing_source;
	
	//MSC & CONSEQ_0  + DTC
	if (convMode) {
		ADC10CTL0 |= MSC;
		ADC10CTL1 |= CONSEQ0 * convMode;
	}
	
	if ((convMode & CONV_MCH)){
		ADC10AE0  = ((1 << (port+1)) - 1) & 0xff;
		/* only for F22xx
		ADC10AE1  = (1 << (port-8)) - 1; 
		*/
	}
	else{
		ADC10AE0  = (1 << port) & 0xff;
		/* only for F22xx
		ADC10AE1  = 1 << (port-8); 
		*/
	}
	//SHT
	temp_char = 0;
	sht -= 1;
	temp_char += (sht>>=2)>0;
	temp_char += (sht>>=1)>0;
	temp_char += (sht>>=1)>0;
	ADC10CTL0 |= temp_char * ADC10SHT0; 
	
	//reference on
	if (refP==REF2_5) ADC10CTL0 |= REF2_5V;
	if (refP==REFE) ADC10CTL0 |= REFOUT;
	if ((refN==REFN) || (refP==REF1_5) || (refP==REF2_5)) {
		ADC10CTL0 |= REFON;
		//if (temp_char == 0) ADC_waitingRefSettle(); //40 cycles for ref startup
	}
	//REFVCC==0
	if ((refP==REF1_5) || (refP==REF2_5)) ADC10CTL0 |= SREF0;
	if (refP==REFE) ADC10CTL0 |= SREF1;
	if (refN) ADC10CTL0 |= SREF2;
	
	//enable interrupt
	if (ADC10_done_ISR != NULL){
		ADC10CTL0 |= ADC10IE; 
		__bis_SR_register(GIE);
		ADC10_set_ISR(ADC10_done_ISR);
	}
	else{
		ADC10_set_ISR(NULL);
	}
    //ADC10CTL0|= ENC;
	
} // ADC_init

/*
float ADC10_toDegC_f(int v){
	return (v * ADC10_15P - 0.986) / 0.00355;
}
*/

int ADC10_toDegC (int v){
	return ((v - 673) * 423) / 1024;
}

int ADC10_result(){
	while ((ADC10CTL1 & ADC10BUSY) ); 
	return ADC10MEM;
}

int ADC10_result_2s(){
	while ((ADC10CTL1 & ADC10BUSY) ); 
	return ADC10MEM >> 6;
}


inline void avgFilter(int a[],char len, char cnt){ 
	// 813 min ~ 34547 @ 120,120
	// calc cnt from len
	char i, j, l;
	unsigned int acc;
	unsigned long s;
	i = j = acc =0;
	while (i<len){
		s = 0;
		l = i;
		acc+=len;
/*
#ifdef WDT_ENABLE
		weego;
#endif
*/
		while (i*cnt<acc) s+=a[i++];
		//a[j++]= s / (i-l);
		a[j++]= (s + ((i-l) >> 1)) / (i-l); //5/4 op
	}
}

void ADC10_set_ISR(int (*ISR_ptr)()){
	ACD10_ISR_ptr = ISR_ptr;
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(){
	int exit_code;
	if(ACD10_ISR_ptr != NULL) {
		exit_code = (ACD10_ISR_ptr)();
		if (exit_code>0)
			__bis_SR_register_on_exit(exit_code);
		else if (exit_code<0)
			__bic_SR_register_on_exit(-exit_code);
	}
}
