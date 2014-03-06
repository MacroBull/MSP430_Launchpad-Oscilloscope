#include "uart.h"
/* bench @ 1M & 9600bps */

/* float version
 * 
int UART0_init(float freq, unsigned long int baud, char ie){ //freq in MHz, ie= enable interrupt , // ~8481 CLKs;
	UART0_XD_enable;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 = UCSWRST + UCSSEL_2; //  ACLK? kidding me ?
	
	freq = 1e6 * freq / baud;
	UCA0BR1= (int)(freq / 256);
	freq -= UCA0BR1 * 256;
	UCA0BR0= (int)freq;
	UCA0MCTL = _UCBR[ (int)((freq - UCA0BR0) * 8 + 0.5) ];
	
	UCA0CTL1 &= ~UCSWRST; 
	if (ie) UART0_enable_int;  
}
*/

int (*UART0_RX_ISR_ptr)(char c);

uint16_t UART0_init(uint32_t freq, uint32_t baud, int (*RX_ISR)(char c)){ //freq in Hz, ie= enable interrupt 
	UART0_XD_enable;
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 = UCSWRST + UCSSEL_2;
	
	freq = ((freq << 3) + 4) / baud;
	UCA0BR1= (char)(freq >> 11); //BR1 <10
	freq -= (UCA0BR1 << 11);
	UCA0BR0= freq >> 3;
	UCA0MCTL = _UCBR[ freq - (UCA0BR0 << 3) ];
	
	UCA0CTL1 &= ~UCSWRST; 
	if (RX_ISR != NULL) {
		UART0_enable_int;
		UART0_set_RX_ISR(RX_ISR);
	}
	else {
		UART0_set_RX_ISR(NULL);
	}
}



inline uint16_t UART0_sendChar(char c){ // 927\939 CLKs;
	UART0_TXLED_on;
	UART0_TX_wait; 
	UCA0TXBUF=c;
	UART0_TXLED_off;
	return 1;
}

inline uint16_t UART0_sendInt(int n){ // ~1981 CLKs;
	UART0_TXLED_on;	
	UART0_TX_wait;
	UCA0TXBUF=(char)(n>>8);
	UART0_TX_wait;
	UCA0TXBUF=(char)n;
	UART0_TXLED_off;
	return 2;
}

uint16_t UART0_printInt(int n){ // ~5119 CLKs
	uint8_t c;
	c=0;
	if (n<0) {
		c+=UART0_sendChar('-');
		n = -n;
	};
	if (n>=10) c+=UART0_printInt(n/10);
	UART0_sendChar((char)(48+(n % 10)));
	
	return c+1;
}

uint16_t UART0_printStr(char s[]){
	char *st;
	st=s;
	while (*s){
		UART0_sendChar(*s);
		s++;
	}
	return s-st;
}


void UART0_ljustInt(int n, uint16_t w){ // cold: 8250 CLKs, hot: 8270 CLKs;
	w-=UART0_printInt(n);
	while(w--) UART0_sendChar(' ');
}


void UART0_ljustFloat(float f, uint16_t fd, uint16_t w){
	w-=UART0_printInt((int)(f));
	UART0_sendChar('.');
	if (f<0) f=-f;
	while (fd--) UART0_sendChar(48+(int)(f*=10));
	w-=1+fd;
	while(w--) UART0_sendChar(' ');
}


void UART0_set_RX_ISR(int (*ISR_ptr)(char c)){
	UART0_RX_ISR_ptr = ISR_ptr;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART0_RX_ISR(){
	int exit_code;
	if(UART0_RX_ISR_ptr != NULL) {
		UART0_RXLED_on;
		exit_code = (UART0_RX_ISR_ptr)(UCA0RXBUF);
		UART0_RXLED_off;
		if (exit_code>0)
			__bis_SR_register_on_exit(exit_code);
		else if (exit_code<0)
			__bic_SR_register_on_exit(-exit_code);
	}
}


/* UART1 */
#if defined(UCA1CTL0) || defined(UCA1CTL0_)
/* float version hard to debug...
 * 
int UART1_init(float freq, unsigned long int baud, char ie){ //freq in MHz, ie= enable interrupt , // ~8481 CLKs;
	UART1_XD_enable;
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 = UCSWRST + UCSSEL_2;
	
	freq = 1e6 * freq / baud;
	UCA1BR1= (int)(freq / 256);
	freq -= UCA1BR1 * 256;
	UCA1BR0= (int)freq;
	UCA1MCTL = _UCBR[ (int)((freq - UCA1BR0) * 8 + 0.5) ];
	
	UCA1CTL1 &= ~UCSWRST; 
	if (ie) UART1_enable_int;
}
*/
uint16_t UART1_init(uint32_t freq, uint32_t baud, uint8_t ie){ //freq in Hz, ie= enable interrupt 
	UART1_XD_enable;
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 = UCSWRST + UCSSEL_2;
	
	freq = ((freq << 3) + 4) / baud;
	UCA1BR1= (char)(freq >> 11); //BR1 <10
	freq -= (UCA1BR1 << 11);
	UCA1BR0= freq >> 3;
	UCA1MCTL = _UCBR[ freq - (UCA1BR0 << 3) ];
	
	UCA1CTL1 &= ~UCSWRST; 
	if (ie) UART1_enable_int;
}


inline uint16_t UART1_sendChar(char c){ // 927\939 CLKs;
	UART1_TXLED_on;
	UART1_TX_wait; 
	UCA1TXBUF=c;
	UART1_TXLED_off;
	return 1;
}

inline uint16_t UART1_sendInt(int n){ // ~1981 CLKs;
	UART1_TXLED_on;	
	UART1_TX_wait;
	UCA1TXBUF=(char)(n>>8);
	UART1_TX_wait;
	UCA1TXBUF=(char)n;
	UART1_TXLED_off;
	return 2;
}

uint16_t UART1_printInt(int n){ // ~5119 CLKs
	uint8_t c;
	c=0;
	if (n<0) {
		c+=UART1_sendChar('-');
		n = -n;
	};
	if (n>=10) c+=UART1_printInt(n/10);
	UART1_sendChar((char)(48+(n % 10)));
	
	return c+1;
}

uint16_t UART1_printStr(char s[]){
	char *st;
	st=s;
	while (*s){
		UART1_sendChar(*s);
		s++;
	}
	return s-st;
}


void UART1_ljustInt(int n, uint16_t w){ // cold: 8250 CLKs, hot: 8270 CLKs;
	w-=UART1_printInt(n);
	while(w--) UART1_sendChar(' ');
}


void UART1_ljustFloat(float f, uint16_t fd, uint16_t w){
	w-=UART1_printInt((int)(f));
	UART1_sendChar('.');
	if (f<0) f=-f;
	while (fd--) UART1_sendChar(48+(int)(f*=10));
	w-=1+fd;
	while(w--) UART1_sendChar(' ');
}
#endif