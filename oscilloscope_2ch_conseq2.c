#include	<msp430.h>

#include "uart.h"
#include "adc10.h"

#define CH1 5
#define CH2 4

#define ZERO 0x20
#define MAXLEN 108
#define TVLO 12000

#ifdef debug
	#define debugS
	#define debugMEM
	#define debugTime
#endif

char len, cnt, cntacc; 
unsigned int ch1[MAXLEN]; //@678/2a6 maxlen=80
unsigned int ch2[MAXLEN]; //@516/204 maxlen=80


int gotRequest(char c){
	char tmp;
	
	tmp=c - 0x30; // join the requring count
	if ((tmp>=0)&&(tmp<=9)) cntacc = cntacc * 10 + tmp;
	else if ((cntacc>0)&(cntacc<=len)){
		cnt = cntacc;
		cntacc=0;
	}
	
	//weego;
	return 0;
}


int ADC_done(void){ 
	return -CPUOFF; // CPU on
}


inline void filter(int a[]){
	// calc cnt from len
	char i, j, l;
	unsigned int acc;
	unsigned long s;
	i = j = acc =0;
	while (i<len){
		s = 0;
		l = i;
		acc+=len;
		
		weego;
		while (i*cnt<acc) s+=a[i++];
		//a[j++]= s / (i-l);
		a[j++]= (s + ((i-l) >> 1)) / (i-l); // round
	}
}

void initTimer(){
	//Using VLO = 12kHz as timeStamper
	BCSCTL1 &= ~XTS;
	BCSCTL1 |= XT2OFF + DIVA_0;
	BCSCTL3 |= LFXT1S_2;
	
	TACCR0 = TVLO;                   
	TACCTL0 |= CCIE;  
	TACTL = TASSEL_1 + MC_1; //continuous 
	
}

void triggerADC(char ch, int *addr){
	//start ADC MSC
	ADC10CTL0 &= ~ENC;
	while (ADC10_busy);
	ADC10CTL1 &= ~INCH_15;
	ADC10CTL1 |= INCH0 * ch;
	
	ADC10DTC1 = len;
	ADC10SA = addr;
	
	ADC10_start;
}

void enablePullRes(){
	
	P1REN |= BIT4;
	P1OUT &= ~BIT4;
	//P1OUT |= BIT4;
	
	P2DIR &= ~(BIT0 + BIT1);
	P2REN |= BIT0 + BIT1;
	P2OUT |= BIT0;
	P2OUT &= ~BIT1;
}


void main(void)

{
	char i;
	unsigned int t0, t1, t2;
	
	WDT_disable; // Disable WDT
	BC16MSET; // Set clock to 16MHz
	len=MAXLEN;
	cnt=4; //initial 4 period to average
	cntacc = 0;
	UART0_XLED_enable;
	UART0_init(16000000,115200, gotRequest); // 16MHz, 115200bps, enable interrupt
	
	ADC10_init(CH1, 0, 25, 4, 0, 2, ADC_done); // multisample, ref=2.5v, 
	
	initTimer();
	enablePullRes();
	
	while (1){	
		
		weego;
		t0 = TAR;
		triggerADC(CH1,ch1);
		__bis_SR_register(CPUOFF);
		t1 = TAR;
		
		weego;
		printStr("AS");ljustInt(t0, 6); // channel1 start
		
		filter(ch1);
		for (i=0;i<cnt;i++){ 
			weego;
			sendChar('A');//channel1 data
			ljustInt(ch1[i] -ZERO, 3);
		};
		
		printStr("AE");ljustInt(t1, 6); // channel1 end
		weego;
		t0 = TAR;
		triggerADC(CH2,ch2);// channel2 start ADC
		__bis_SR_register(CPUOFF);
		t1 = TAR;
		weego;
		printStr("BS");ljustInt(t0, 6);// channel2 start
		
		filter(ch2);
		for (i=0;i<cnt;i++){ 
			weego;
			sendChar('B'); // channel2 data
			ljustInt(ch2[i] -ZERO, 3);
		};
		
		printStr("BE");ljustInt(t1, 6); // channel2 end
		
	}
}

/*
#ifdef debugMEM
		
		//delayms(5000);
		ljustInt(len, 8);
		ljustInt(cnt, 8);
		printStr("______orig______");
		for (i=0;i<len;i++){ 
				ljustInt(ch1[i], 8);
			};
		//weego;
		printStr("______filter____");
#endif

#ifdef debugS
		filter(ch1);
		for (i=0;i<cnt;i++){ 
			ljustInt(ch1[i], 8);
		};
		
#else
		*/