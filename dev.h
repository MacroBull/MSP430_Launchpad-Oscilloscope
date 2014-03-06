
#ifndef _DEV_H
#define _DEV_H

#ifdef _DEV_LAUNCHPAD
	#define UART0_XD_SEL P1SEL
	#define UART0_XD_SEL2 P1SEL2
	#define UART0_TXD BIT2
	#define UART0_RXD BIT1
	
	#define TXD UART0_TXD
	#define RXD UART0_RXD
	

	#ifndef TXLED
	
		#define UART0_TXLED_DIR P1DIR
		#define UART0_RXLED_DIR P1DIR
		#define UART0_TXLED_OUT P1OUT
		#define UART0_RXLED_OUT P1OUT
		#define UART0_TXLED BIT0
		#define UART0_RXLED BIT6
		
		#define TXLED UART0_TXLED
		#define RXLED UART0_RXLED
		
	#endif

#endif

#ifdef _DEV_5529EXP
	#define UART0_XD_SEL P3SEL
	#define UART0_TXD BIT3
	#define UART0_RXD BIT4
	
	#define UART1_XD_SEL P4SEL
	#define UART1_TXD BIT4
	#define UART1_RXD BIT5
	
	#ifndef TXLED
	
		#define UART0_TXLED_DIR P1DIR
		#define UART0_RXLED_DIR P1DIR
		#define UART0_TXLED_OUT P1OUT
		#define UART0_RXLED_OUT P1OUT
		#define UART0_TXLED BIT1
		#define UART0_RXLED BIT2
		
		#define UART1_TXLED_DIR P1DIR
		#define UART1_RXLED_DIR P1DIR
		#define UART1_TXLED_OUT P1OUT
		#define UART1_RXLED_OUT P1OUT
		#define UART1_TXLED BIT3
		#define UART1_RXLED BIT4
		
		#define TXLED UART1_TXLED
		#define RXLED UART1_RXLED
		
	#endif
	
	#define TXD UART1_TXD
	#define RXD UART1_RXD

#endif

#endif