#pragma once

#include <msp430.h>

#include <mspl/std.hpp>
#include <mspl/msp.hpp>

#define CALADC12_15V_30C *((i16*)0x1A1A) // Temperature sensor calibration 1.5V 30C
#define CALADC12_15V_85C *((i16*)0x1A1C) // Temperature sensor calibration 1.5V 85C
#define CALADC12_20V_30C *((i16*)0x1A1E) // Temperature sensor calibration 2.0V 30C
#define CALADC12_20V_85C *((i16*)0x1A20) // Temperature sensor calibration 2.0V 85C
#define CALADC12_25V_30C *((i16*)0x1A22) // Temperature sensor calibration 2.5V 30C
#define CALADC12_25V_85C *((i16*)0x1A24) // Temperature sensor calibration 2.5V 85C

// Taken form:
// ECE2049: Embedded Systems
// E-Term 2016
// Electrical & Computer Engineering
// https://users.wpi.edu/~ndemarinis/ece2049/e16/index.html

static float degC_per_bit = 1;

inline void gpcl_Initialize_ADC12(void) {
	/* ***** Core configuration ***** */
	REFCTL0 &= ~REFMSTR;    // Reset REFMSTR to hand over control of
	                        // internal reference voltages to
	                        // ADC12_A control registers
	// ADC12CTL0:  Use default settings, turn on ADC and internal reference generator
	// ADC12REV2_5V = 0, so internal reference = 1.5V
	ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON;
	// ADC12CTL1:  Use default settings (just enable sample timer)
	ADC12CTL1 = ADC12SHP;
	/* ***** Channel configuration ***** */
	// Use MCTL0 for conversion, configure reference = V_REF (1.5V)
	// Input channel = 10 => Interlan temperature sensor
	ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense
	                                          // ACD12SREF_1 = internal ref = 1.5v
	__delay_cycles(100);                    // delay to allow Ref to settle
	ADC12CTL0 |= ADC12ENC;                  // Enable conversion
	// Use calibration data stored in info memory to compute slope
	degC_per_bit = ((float)(85.0 - 30.0))/((float)(CALADC12_15V_85C - CALADC12_15V_30C));
	ADC12IE = ADC12IE0;
}

namespace ADC12 {
	inline void launch(void) {
		ADC12CTL0 &= ~ADC12SC;      // Clear the start bit (precautionary)
		ADC12CTL0 |= ADC12SC;       // Start the conversion
	}
	inline i16 temperature(void) {
		const i16 in_temp = ADC12MEM0 & 0x0FFF;      // Read in results if conversion
		// Temperature in Celsius. See the Device Descriptor Table section in the
		// System Resets, Interrupts, and Operating Modes, System Control Module
		// chapter in the device user's guide for background information on the
		// used formula.
		return (float)(in_temp - CALADC12_15V_30C) * degC_per_bit + 30.0;
	}
}
