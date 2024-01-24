#pragma once

#include <msp430.h>

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/pads.hpp>

#define Error static constexpr const u8
[[maybe_unused]] Error ERR_UNIMPLEMENTED = 0b00001;
[[maybe_unused]] Error ERR_FILESYSTEM    = 0b00010;
[[maybe_unused]] Error ERR_ADC12         = 0b00100;
#undef Error

inline void raise(const u8 error) {
	const bool bit1 = error & 0b00001;
	const bool bit2 = error & 0b00010;
	const bool bit3 = error & 0b00100;
	const bool bit4 = error & 0b01000;
	const bool bit5 = error & 0b10000;
	const bool over = error > 0b11111;
	if (bit1 || over) { TurnOn(PadLed, 5); }
	if (bit2 || over) { TurnOn(PadLed, 4); }
	if (bit3 || over) { TurnOn(PadLed, 3); }
	if (bit4 || over) { TurnOn(PadLed, 2); }
	if (bit5 || over) { TurnOn(PadLed, 1); }
}
