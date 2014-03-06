#include <msp430.h>
#include <stdint.h>
#include "dev.h"
#include "misc.h"

#ifndef _UART_H
#define _UART_H


#if defined(UCA0CTL0) || defined(UCA0CTL0_)


	 
	#if defined(UART0_RXLED_DIR) && defined(UART0_TXLED_DIR)
		#define UART0_XLED_enable {UART0_RXLED_DIR|= UART0_RXLED; UART0_TXLED_DIR|= UART0_TXLED;}
	#endif
	
	#ifdef UART0_TXLED_OUT
		#define UART0_TXLED_on {UART0_TXLED_OUT |= UART0_TXLED;}
		#define UART0_TXLED_off {UART0_TXLED_OUT &= ~UART0_TXLED;}
	#else
		#define UART0_TXLED_on {}
		#define UART0_TXLED_off {}
	#endif
	
	#ifdef UART0_RXLED_OUT
		#define UART0_RXLED_on {UART0_RXLED_OUT |= UART0_RXLED;}
		#define UART0_RXLED_off {UART0_RXLED_OUT &= ~UART0_RXLED;}
	#else
		#define UART0_RXLED_on {}
		#define UART0_RXLED_off {}
	#endif
	
	
	#ifdef UCA0IE
		#ifdef UART0_XD_SEL
			#warning dev_board macro detected! (SEL1)
			#define UART0_XD_enable {UART0_XD_SEL = UART0_TXD + UART0_RXD;}
		#endif
		#define UART0_busy (UCA0STAT&UCBUSY)
		#define UART0_TX_wait {while (!(UCA0IFG & UCTXIFG)) ;}
		#define UART0_RX_wait {while (!(UCA0IFG & UCRXIFG)) ;}
		#define UART0_enable_int {UCA0IE |= UCRXIE;} // receive only
	#else
		#ifdef UART0_XD_SEL
			#warning dev_board macro detected! (SEL2)
			#define UART0_XD_enable {UART0_XD_SEL = UART0_XD_SEL2 = UART0_TXD + UART0_RXD;}
		#endif
		#define UART0_busy (IFG2&UCA0TXIFG)
		#define UART0_TX_wait {while (!(IFG2&UCA0TXIFG));}
		#define UART0_RX_wait {while (!(IFG2&UCA0RXIFG));}
		#define UART0_enable_int {IE2 |= UCA0RXIE;}
	#endif
	
	#ifndef UART0_XD_enable
		#define UART0_XD_enable {}
	#endif
	
#endif

#if defined(UCA1CTL0) || defined(UCA1CTL0_)
	
	#ifdef UART1_TXLED_DIR
		#define UART1_TXLED_on {UART1_TXLED_OUT |= UART1_TXLED;}
		#define UART1_TXLED_off {UART1_TXLED_OUT &= ~UART1_TXLED;};
		#ifdef UART1_RXLED_DIR
			#define UART1_XLED_enable {UART1_RXLED_DIR|= UART1_RXLED; UART1_TXLED_DIR|= UART1_TXLED;}
		#endif
	#endif
	
	#ifdef UCA1IE
		#ifdef UART1_XD_SEL
			#warning dev_board macro detected! (SEL1)
			#define UART1_XD_enable {UART1_XD_SEL = UART1_TXD + UART1_RXD;}
		#endif
		#define UART1_busy (UCA1STAT&UCBUSY)
		#define UART1_TX_wait {while (!(UCA1IFG & UCTXIFG)) ;}
		#define UART1_RX_wait {while (!(UCA1IFG & UCRXIFG)) ;}
		#define UART1_enable_int {UCA1IE |= UCRXIE;} // receive only
	#else
		#ifdef UART1_XD_SEL
			#warning dev_board macro detected! (SEL2)
			#define UART1_XD_enable {UART1_XD_SEL = UART1_XD_SEL2 = UART1_TXD + UART1_RXD;}
		#endif
		#define UART1_busy (IFG2&UCA1TXIFG)
		#define UART1_TX_wait {while (!(IFG2&UCA1TXIFG));}
		#define UART1_RX_wait {while (!(IFG2&UCA1RXIFG));}
		#define UART1_enable_int {IE2 |= UCA1RXIE;}
	#endif
	
	#ifndef UART1_XD_enable 
		#warning  no dev_board macro detected!
		#define UART1_XD_enable {} 
	#endif
	#ifndef UART1_TXLED_on 
		#define UART1_TXLED_on {}
	#endif
	#ifndef UART1_TXLED_off 
		#define UART1_TXLED_off {}
	#endif
	
#endif




#define UCBR0 0b00000000
#define UCBR1 0b00000010
#define UCBR2 0b00100010
#define UCBR3 0b00101010
#define UCBR4 0b10101010
#define UCBR5 0b10101110
#define UCBR6 0b11101110
#define UCBR7 0b11111110

#ifndef _UCBR
	static const uint8_t _UCBR[] = {
		UCBR0, UCBR1, UCBR2, UCBR3, UCBR4, UCBR5, UCBR6, UCBR7
	};
#endif

#if defined(UCA0CTL0) || defined(UCA0CTL0_)
	//extern uint16_t UART0_init(float freq, unsigned long int baud, uint8_t ie);
	extern uint16_t UART0_init(uint32_t freq, uint32_t baud, int (*RX_ISR)(char c));
	extern inline uint16_t UART0_sendChar(char c);
	extern inline uint16_t UART0_sendInt(int n);
	extern uint16_t UART0_printInt(int n);
	extern uint16_t UART0_printStr(char s[]);
	extern void UART0_ljustInt(int n, uint16_t w);
	extern void UART0_ljustFloat(float f, uint16_t fd, uint16_t w);
	extern void UART0_set_RX_ISR(int (*ISR_ptr)(char c));

	#define UART_init UART0_init
	#define sendChar UART0_sendChar
	#define sendInt UART0_sendInt
	#define printInt UART0_printInt
	#define printStr UART0_printStr
	#define ljustInt UART0_ljustInt
	#define ljustFloat UART0_ljustFloat
	
#endif

#if defined(UCA1CTL0) || defined(UCA1CTL0_)
	extern uint16_t UART1_init(uint32_t freq, uint32_t baud, uint8_t ie);
	extern inline uint16_t UART1_sendChar(char c);
	extern inline uint16_t UART1_sendInt(int n);
	extern uint16_t UART1_printInt(int n);
	extern uint16_t UART1_printStr(char s[]);
	extern void UART1_ljustInt(int n, uint16_t w);
	extern void UART1_ljustFloat(float f, uint16_t fd, uint16_t w);
	extern void UART1_set_RX_ISR(int (*ISR_ptr)(char c));
	
#endif
	
#endif