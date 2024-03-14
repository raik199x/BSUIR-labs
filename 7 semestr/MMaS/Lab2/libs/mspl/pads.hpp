#pragma once

#include <mspl/msp.hpp>
#include <mspl/traits/pads.hpp>

#define gpcl_Initialize_PadLed(number) \
	gpcl(Pad, number, dir) |= gpcl(Pad, number, bit)

#define gpcl_TurnOff_PadLed(number) \
	gpcl(Pad, number, out) &= ~gpcl(Pad, number, bit)

#define gpcl_TurnOn_PadLed(number) \
	gpcl(Pad, number, out) |= gpcl(Pad, number, bit)

#define gpcl_Toggle_PadLed(number) \
	gpcl(Pad, number, out) ^= gpcl(Pad, number, bit)
