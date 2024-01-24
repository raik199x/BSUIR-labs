#include <msp430.h>

/// @brief Task
#include "task.hpp"

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/timers.hpp>
#include <mspl/buttons.hpp>
#include <mspl/leds.hpp>
#include <mspl/pads.hpp>

/// @brief Pads
#include "pads.hpp"

static constexpr const u16 RECHARGE_DELAY = 0xF000;
static constexpr const u16 COMPARE_DELAY = 0x1000;
static_assert(
	RECHARGE_DELAY > COMPARE_DELAY,
	"Recharge delay must be greater than compare delay"
);

int main(void) {
	// Stop WatchDog Timer
	StopTimer(WatchDog);
	// Initialize Pads (as Leds)
	Initialize(PadLed, 1);
	Initialize(PadLed, 2);
	disable_pads();
	// Enable interrupts from timers
	Interrupt(Enable, TA, 0);
	Interrupt(Enable, TA, 1);
	// Start Timers
	StartTimer(TA, 0, TASSEL__SMCLK, ID__8, RECHARGE_DELAY);
	// Announcements (Indication)
	TurnOff(PadLed, 1);
	TurnOff(PadLed, 2);
	// Global Interrupts Enable + CCStudio debug
	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();
}

enum class PadPositive {
	PAD1 = CBIPSEL_0,
	PAD2 = CBIPSEL_1,
	PAD3 = CBIPSEL_2,
	PAD4 = CBIPSEL_3,
	PAD5 = CBIPSEL_4,
};
enum class PadNegative {
	PAD1 = CBIMSEL_0,
	PAD2 = CBIMSEL_1,
	PAD3 = CBIMSEL_2,
	PAD4 = CBIMSEL_3,
	PAD5 = CBIMSEL_4,
};

bool compare(const PadPositive left, const PadNegative right) {
	/// @todo Compare left and right pads
	CBCTL0 = CBIPEN | ((u16)left) | CBIMEN | ((u16)right);
	CBCTL1 = CBON | CBF | CBFDLY_1;
	__delay_cycles(150);
	const bool result = CBCTL1 & CBOUT;
	CBCTL1 &= ~CBON;
	return result;
}

/// @brief Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR(void)
#else
#error Compiler not supported!
#endif
{
	__disable_interrupt();
	// Stop Timers A0 and A1
	StopTimer(TA, 0);
	StopTimer(TA, 1);
	// Recharge Pads (as buttons)
	charge_pads(PadNumber::PAD5, PadNumber::PAD4);
	// Start Timers A0 and A1 again
	StartTimer(TA, 0, TASSEL__SMCLK, ID__8, RECHARGE_DELAY);
	StartTimer(TA, 1, TASSEL__SMCLK, ID__8, COMPARE_DELAY);
	__enable_interrupt();
}

/// @brief Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR(void)
#else
#error Compiler not supported!
#endif
{
	__disable_interrupt();
	// Stop Timer A1
	StopTimer(TA, 1);
	// Configure pads
	configure_pads(PadNumber::PAD4, PadNumber::PAD5);
	/// @todo Compare Pads (as buttons)
	const bool greater = compare(PadPositive::PAD4, PadNegative::PAD5);
	const bool less = compare(PadPositive::PAD5, PadNegative::PAD4);
	// Indication
	TurnOff(PadLed, 1);
	TurnOff(PadLed, 2);
	if (greater) {
		TurnOn(PadLed, 1);
	}
	if (less) {
		TurnOn(PadLed, 2);
	}
	// Restore pads
	restore_pads(PadNumber::PAD4, PadNumber::PAD5);
	// Start Timer A1 again
	StartTimer(TA, 1, TASSEL__SMCLK, ID__8, COMPARE_DELAY);
	__enable_interrupt();
}
