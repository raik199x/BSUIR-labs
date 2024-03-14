#pragma once

#include <msp430.h>
#include <mspl/msp.hpp>

#define gpcl_StopTimer_WatchDog() \
	WDTCTL = WDTPW | WDTHOLD

#define gpcl_StartTimer_TA(number, clock, divisor, delay) \
	squash(TA, number, CCR0) = delay; /* Set Timer A_number delay to debounce delay */ \
	squash(TA, number, CTL) = clock | divisor | MC__UP | TACLR

#define gpcl_StopTimer_TA(number) \
	squash(TA, number, CTL) = MC__STOP

#define gpcl_Interrupt_Enable_TA(number) \
	squash(TA, number, CCTL0) |= CCIE

#define gpcl_Interrupt_Disable_TA(number) \
	squash(TA, number, CCTL0) &= ~CCIE
