#pragma once

#include <mspl/msp.hpp>
#include <mspl/traits/leds.hpp>

#define gpcl_Initialize_Led(number) \
	gpcl(Led, number, dir) |= gpcl(Led, number, bit)

#define gpcl_TurnOff_Led(number) \
	gpcl(Led, number, out) &= ~gpcl(Led, number, bit)

#define gpcl_TurnOn_Led(number) \
	gpcl(Led, number, out) |= gpcl(Led, number, bit)

#define gpcl_Toggle_Led(number) \
	gpcl(Led, number, out) ^= gpcl(Led, number, bit)
