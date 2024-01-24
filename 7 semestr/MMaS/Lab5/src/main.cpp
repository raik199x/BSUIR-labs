#include <msp430.h>

/// @brief Task
#include "task.hpp"

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/timers.hpp>
#include <mspl/buttons.hpp>
#include <mspl/leds.hpp>
#include <mspl/pads.hpp>

#include "CMA3000.hpp"
#include "DOGS102W6.hpp"

static constexpr const u16 DRAWING_DELAY = 0x4000;
static constexpr const u16 DEBOUNCE_DELAY = 0x0800;
static bool confirm_debouncing = false;

static bool mirror = false;

void InfiniteLoop(void) {
	while (true) {
		// Start Timer A2 again
		StartTimer(TA, 2, TASSEL__SMCLK, ID__8, DRAWING_DELAY);
		// Global Interrupts Enable + Enter LPM3
		__bis_SR_register(GIE | LPM3_bits);
		// Redraw
		using namespace CMA3000::constants;
		const bool mirrored = mirror;
		const i16 number = CMA3000::decode(
			CMA3000::readRegister(RY),
			CTRL_BITS::G_RANGE__8G,
			CTRL_BITS::MODE__MEASURE400
		);
		const usize length = DOGS102W6::getNumberLength(number);
		if (mirrored) {
			mirror = false;
			DOGS102W6::MirrorSegMode ^= 1;
			if (DOGS102W6::MirrorSegMode == 0) DOGS102W6::COLUMN_START_ADDRESS = 30;
			else DOGS102W6::COLUMN_START_ADDRESS = 0;
			DOGS102W6::mirror();
		}
		DOGS102W6::display(number, mirrored, length);
	}
}

int main(void) {
	// Stop WatchDog Timer
	StopTimer(WatchDog);
	// Initialize Buttons
	Initialize(Button, S1);
	// Initialize Pads (as leds)
	Initialize(PadLed, 1); TurnOff(PadLed, 1);
	Initialize(PadLed, 2); TurnOff(PadLed, 2);
	Initialize(PadLed, 3); TurnOff(PadLed, 3);
	Initialize(PadLed, 4); TurnOff(PadLed, 4);
	Initialize(PadLed, 5); TurnOff(PadLed, 5);
	// Enable interrupts from buttons
	Interrupt(Enable, Button, S1, ON_PRESS);
	// Enable interrupts from timers
	Interrupt(Enable, TA, 1);
	Interrupt(Enable, TA, 2);
	// Configure CMA3000
	CMA3000::configure();
	// Configure DOGS102W6
	DOGS102W6::configure();
	// Enter infinite loop
	InfiniteLoop();
}

/// @brief Port1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) PORT1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	// Clear P1.7 interrupt flag
	Interrupt(ClearFlag, Button, S1);

	// False-positive interrupt, ignore
	if (GetStateOf(Button, S1) != ActiveStateOf(Button, S1))
		return;

	// Debouncing
	confirm_debouncing = false;
	StartTimer(TA, 1, TASSEL__SMCLK, ID__8, DEBOUNCE_DELAY);
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

	// Check if Button S1 is still pressed
	if (GetStateOf(Button, S1) != ActiveStateOf(Button, S1)) {
		// Button was not actually pressed
		confirm_debouncing = false;
		return;
	}

	// Button was pressed
	if (confirm_debouncing) {
		Toggle(PadLed, 1);
		mirror = !mirror;
		// Stop Timer A2
		StopTimer(TA, 2);
		// Exit LPM3 to mirror the screen
		LPM3_EXIT;
		return;
	}

	// Needs confirmation
	confirm_debouncing = true;
	StartTimer(TA, 1, TASSEL__SMCLK, ID__8, DEBOUNCE_DELAY);
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
	// Exit LPM3 to redraw the number
	LPM3_EXIT;
}
