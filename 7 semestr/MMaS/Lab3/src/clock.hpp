#pragma once

#include <msp430.h>

#include <mspl/std.hpp>

static constexpr const u16 DEFAULT_SELS = SELS__REFOCLK;

inline void ConfigureClockDCO(void) {
	// Configure DCOCLK ( 0.9 MHz )

	// DCOCLK = FLLREFCLK / FLLREFDIV · (FLLN + 1 ) · FLLD
	UCSCTL1 = DCORSEL_0;
	UCSCTL2 = FLLD__1 | (26 * FLLN0);
	UCSCTL3 = SELREF__REFOCLK | FLLREFDIV__1;

	// Configure Main, Sub-Main and Auxilary clocks' sources
	UCSCTL4 = SELM__DCOCLK | DEFAULT_SELS | SELA__DCOCLK;
}
