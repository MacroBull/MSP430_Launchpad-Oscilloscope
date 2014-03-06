
#ifndef _MISC_H
	#define _MISC_H

	#define NULL 0L
	
	/*DCO quick settings*/
	#define BC1MSET {BCSCTL1 = CALBC1_1MHZ; DCOCTL = CALDCO_1MHZ;}
	#define BC8MSET {BCSCTL1 = CALBC1_8MHZ; DCOCTL = CALDCO_8MHZ;}
	#define BC12MSET {BCSCTL1 = CALBC1_12MHZ; DCOCTL = CALDCO_12MHZ;}
	#define BC16MSET {BCSCTL1 = CALBC1_16MHZ; DCOCTL = CALDCO_16MHZ;}
	
	/*WDT */
	#define WDT_disable {WDTCTL = WDTPW | WDTHOLD;}
	#define weego {WDTCTL =WDTPW + WDTCNTCL;}
	
	/*UCA0 */
	#define UCA0_BUSY (IFG2&UCA0TXIFG)
	#define UCA0_wait {while (!(IFG2&UCA0TXIFG));}
	
#endif