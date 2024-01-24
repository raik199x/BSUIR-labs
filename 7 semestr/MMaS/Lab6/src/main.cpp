#include <msp430.h>

/// @brief Task
#include "task.hpp"

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/timers.hpp>
#include <mspl/buttons.hpp>
#include <mspl/leds.hpp>
#include <mspl/pads.hpp>

#include <msp-exp/FatFs/ff.h>

#include "error.hpp"
#include "ADC12.hpp"
#include "Cma3000.hpp"
#include "Dogs102x6.hpp"

/// @brief Timers
static constexpr const struct {
	f32 FREQ_XT1_HZ = 32768;
	f32 FREQ_40_HZ = 40;
	u16 PERIOD = FREQ_XT1_HZ / FREQ_40_HZ;
	u16 DIVIDER = ID__1;
} TA_40_HZ;
static constexpr const u16 ITERATION_DELAY = 0x0800;
static constexpr const i16 THRESHOLD = 250;

/// @brief Buffer
static constexpr const usize BUFFER_SIZE = 10;
static i16 buffer[BUFFER_SIZE] = {};
static u8 idx = 0;

/// @brief Drawing
static constexpr const u8 K = 8;

/// @brief Flags
static enum {
	MEMORY,
	SD_CARD,
} MODE = MEMORY;
static struct {
	bool measure_mode = true;
	bool full_buffer = false;
	bool redraw = false;
} flags;

static constexpr const auto FILENAME = "buffer.bin";

u16 Get_Draw_Value(const u8 idx)
{
	const i16 data_range = 200;
	const i16 value = buffer[idx] + (data_range / 2);
	u16 draw_value = (u16)((float)DOGS102x6_X_SIZE * (float)(value) / (float)(data_range));
	return draw_value;
}

void InfiniteLoop(void) {
	bool pressed = false, released = true;
	while (true) {
		// Start Timer A1 again
		StartTimer(TA, 1, TASSEL__SMCLK, ID__8, ITERATION_DELAY);
		// Global Interrupts Enable + Enter LPM3
		__bis_SR_register(GIE | LPM3_bits);
		// CMA300 state
		GetStateOf(Cma3000);
		const i16 x = Cma3000::decode(Cma3000_xAccel);
		if (released && x > +THRESHOLD) { pressed = true; released = false; }
		if (pressed  && x < -THRESHOLD) { released = true; }
		if (pressed  && released) {
			flags.measure_mode = !flags.measure_mode;
			if (flags.measure_mode) {
				TurnOff(Led, 1);
				MODE = MEMORY;
				Interrupt(Enable, TA, 2);
			} else {
				TurnOn(Led, 1);
				MODE = SD_CARD;
				flags.redraw = true;
				Interrupt(Disable, TA, 2);
			}
			pressed = false;
		}
		i16 draw_buffer[BUFFER_SIZE];
		for (usize i = 0; i < BUFFER_SIZE; i++) {
			draw_buffer[i] = buffer[i];
		}
		if (MODE == MEMORY && flags.full_buffer) {
			Toggle(PadLed, 1);
			idx = 0;
			flags.full_buffer = false;
			flags.redraw = true;
			// save to the file
			FIL file;
			UINT bw = 0;
			f_open(&file, FILENAME, FA_WRITE | FA_CREATE_ALWAYS);
			f_write(&file, (void*)draw_buffer, BUFFER_SIZE, &bw);
			f_close(&file);
		}
		if (flags.redraw) {
			TurnOn(Led, 3);
			if (MODE == SD_CARD) {
				FIL file;
				UINT bw = 0;
				f_open(&file, FILENAME, FA_READ);
				f_read(&file, (void*)draw_buffer, BUFFER_SIZE, &bw);
				f_close(&file);
			}
			for (usize u = 0; u < BUFFER_SIZE; ++u) {
				u16 dv = Get_Draw_Value(u);
				u8 xp = u * K;
				Dogs102x6_clearRow(xp / 8);
				Dogs102x6_pixelDraw(dv, xp + 0, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 1, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 2, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 3, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 4, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 5, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 6, DOGS102x6_DRAW_NORMAL);
				Dogs102x6_pixelDraw(dv, xp + 7, DOGS102x6_DRAW_NORMAL);
			}
			TurnOff(Led, 3);
			flags.redraw = false;
		}
	}
}

int main(void) {
	// Stop WatchDog Timer
	StopTimer(WatchDog);
	// Initialize Buttons
	Initialize(Button, S1);
	// Initialize Leds
	Initialize(Led, 1); TurnOff(Led, 1);
	Initialize(Led, 2); TurnOff(Led, 2);
	Initialize(Led, 3); TurnOff(Led, 3);
	// Initialize Pads (as leds)
	Initialize(PadLed, 1); TurnOff(PadLed, 1);
	Initialize(PadLed, 2); TurnOff(PadLed, 2);
	Initialize(PadLed, 3); TurnOff(PadLed, 3);
	Initialize(PadLed, 4); TurnOff(PadLed, 4);
	Initialize(PadLed, 5); TurnOff(PadLed, 5);
	// Initialize ADC12
	Initialize(ADC12);
	// Initialize Cma3000
	Initialize(Cma3000);
	// Initialize Dogs102x6
	Initialize(Dogs102x6);
	// Enable interrupts from timers
	Interrupt(Enable, TA, 1);
	Interrupt(Enable, TA, 2);
	// Configure Main, Sub-Main and Auxilary clocks' sources
	UCSCTL4 = SELM__REFOCLK | SELS__REFOCLK | SELA__XT1CLK;
	// Start Timer A2
	StartTimer(TA, 2, TASSEL__ACLK, TA_40_HZ.DIVIDER, TA_40_HZ.PERIOD);
	// Enter infinite loop
	InfiniteLoop();
}

/// @brief Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	// Stop Timer A1
	StopTimer(TA, 1);
	// Exit LPM3
	LPM3_EXIT;
}

/// @brief Timer A2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) TIMER2_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	// Stop Timer A2
	StopTimer(TA, 2);
	ADC12::launch();
}

/// @brief ADC12 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(ADC12IV, ADC12IV_ADC12IFG15)) {
	case ADC12IV_ADC12IFG0: {
		// Measure temperature
		const i16 temperature = ADC12::temperature();
		buffer[idx] = temperature;
		if (idx + 1 == BUFFER_SIZE) {
			flags.full_buffer = true;
		}
		idx = (idx + 1) % BUFFER_SIZE;
		// Launch Timer A2 again
		StartTimer(TA, 2, TASSEL__ACLK, TA_40_HZ.DIVIDER, TA_40_HZ.PERIOD);
	} break;
	default:
		raise(ERR_ADC12);
		break;
	};
}
