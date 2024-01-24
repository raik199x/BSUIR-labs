#pragma once

#include <cmath>
#include <mspl/std.hpp>
#include <mspl/msp.hpp>

#include <msp-exp/MSP-EXP430F5529_HAL/HAL_Dogs102x6.h>

#define gpcl_Initialize_Dogs102x6() \
	Dogs102x6_init(); \
	Dogs102x6_backlightInit(); \
	Dogs102x6_setBacklight(255); \
	Dogs102x6_clearScreen()

namespace Dogs102x6 {
	static u8 symbols[12][6] = {
		{0xff, 0x81, 0x81, 0x81, 0x81, 0xff}, // UPPER 0
		{0x08, 0x10, 0x20, 0x40, 0x80, 0xff}, // UPPER 1
		{0x9f, 0x91, 0x91, 0x91, 0x91, 0xf1}, // UPPER 2
		{0x91, 0x91, 0x91, 0x91, 0x91, 0xff}, // UPPER 3
		{0xf0, 0x10, 0x10, 0x10, 0x10, 0xff}, // UPPER 4
		{0xf1, 0x91, 0x91, 0x91, 0x91, 0x9f}, // UPPER 5
		{0xff, 0x91, 0x91, 0x91, 0x91, 0x9f}, // UPPER 6
		{0x80, 0x80, 0x80, 0x80, 0x80, 0xff}, // UPPER 7
		{0xff, 0x91, 0x91, 0x91, 0x91, 0xff}, // UPPER 8
		{0xf1, 0x91, 0x91, 0x91, 0x91, 0xff}, // UPPER 9
		{0x00, 0x08, 0x08, 0x3e, 0x08, 0x08}, // UPPER +
		{0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // UPPER -
	};
	inline usize getNumberLength(i16 number) {
		usize length = 0;
		number = std::abs(number);
		if(number == 0)
			return 1;
		while(number) {
			number /= 10;
			++length;
		}
		return length;
	}
	inline void partial_clear(void) {
		static u8 sData[] = { 0x00 };
		static constexpr const u8 pa = 0u;
		Dogs102x6_setAddress(pa, 0);
		for (u8 c = 0; c < 5; c++) {
			Dogs102x6_writeData(sData, 1);
		}
	}
	inline void printNumber(i16 number, const usize length) {
		static constexpr const u8 COL_ADR = 0;
		static constexpr const u8 pa = 0;
		Dogs102x6_setAddress(pa, COL_ADR);
		Dogs102x6_writeData(symbols[number < 0 ? 11 : 10], 6);
		i16 divider = std::pow(10, length - 1);
		number = std::abs(number);
		for (usize i = 1; i <= length; i++) {
			const i16 digit = number / divider;
			Dogs102x6_setAddress(pa, COL_ADR + (i * 2) + (i * 6));
			Dogs102x6_writeData(symbols[digit], 6);
			number %= divider;
			divider /= 10;
		}
	}
	inline void display(const i16 number, const usize length) {
		partial_clear();
		printNumber(number, length);
	}
}
