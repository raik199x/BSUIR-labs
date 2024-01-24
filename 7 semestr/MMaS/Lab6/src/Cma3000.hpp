#pragma once

#include <msp430.h>

#include <mspl/std.hpp>
#include <mspl/msp.hpp>

#include <msp-exp/MSP-EXP430F5529_HAL/HAL_Cma3000.h>

#define gpcl_Initialize_Cma3000() \
	Cma3000_init()

#define gpcl_GetStateOf_Cma3000() \
	Cma3000_readAccel()

namespace Cma3000::constants {
	#define literal static constexpr const
	literal i16 WEIGHTS0[] = { 1142, 571 , 286 , 143, 71 , 36 , 18 };
	literal i16 WEIGHTS1[] = { 4571, 2286, 1141, 571, 286, 143, 71 };
	literal u8 BITx[] = { BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };
	#undef literal
}

namespace Cma3000 {
	inline i16 decode(i8 bits) {
		i16 value = 0;
		const u8 negative = bits & BIT7;
		bits *= negative ? (-1) : (+1);
		const i16* const WEIGHTS = constants::WEIGHTS0;
		for (u8 i = 0; i < 7; ++i) {
			value += (constants::BITx[i] & bits) ? WEIGHTS[i] : 0;
		}
		value *= negative ? (-1) : (+1);
		return value;
	}
}
