#include <msp430.h>

/// @brief Task and results
#include "task.hpp"
#include "results.hpp"

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/timers.hpp>
#include <mspl/buttons.hpp>
#include <mspl/leds.hpp>
#include <mspl/pads.hpp>

#include "clock.hpp"
#include "voltage.hpp"

static bool activeMode = true;
static bool fastMode = true;
static constexpr const bool BLINKING_MODE = true;
static constexpr const u16 TRIGGER_DELAY = 0x0A00;
static constexpr const u16 DEBOUNCE_DELAY = 0x00A0;

struct {
	u16 delay;
	u16 divisor;
	void adjust(void) {
		delay = (fastMode ? 3 : 1) * TRIGGER_DELAY;
		divisor = fastMode ? ID__8 : ID__1;
	}
} flexible;

void SwitchFV(void) {
	enum {
		F1V1,
		F1V2,
		F2V2,
		F2V1,
		LOOP
	};
	static u8 fvMode = F1V1;
	bool toF1 = fvMode == F2V1;
	bool toF2 = fvMode == F1V2;
	bool toV1 = fvMode == F2V2;
	bool toV2 = fvMode == F1V1;
	if (toF1) {
		fastMode = true;
		UCSCTL4 = SELM__DCOCLK | DEFAULT_SELS | SELA__DCOCLK;
	} else if (toF2) {
		fastMode = false;
		UCSCTL4 = SELM__VLOCLK | DEFAULT_SELS | SELA__VLOCLK;
	} else if (toV1) {
		TurnOff(PadLed, 2);
		// Switch to High Voltage
		SetVCoreUp(Voltage::LEVEL1);
		SetVCoreUp(Voltage::LEVEL2);
		SetVCoreUp(Voltage::LEVEL3);
		TurnOn(PadLed, 1);
	} else if (toV2) {
		TurnOff(PadLed, 1);
		// Switch to Low Voltage
		SetVCoreDown(Voltage::LEVEL2);
		SetVCoreDown(Voltage::LEVEL1);
		SetVCoreDown(Voltage::LEVEL0);
		TurnOn(PadLed, 2);
	}
	if (toF1 || toF2) {
		// Stop Timer A0
		StopTimer(TA, 0);
		// Start Timer A0 again
		flexible.adjust();
		StartTimer(TA, 0, TASSEL__ACLK, flexible.divisor, flexible.delay);
	}
	++fvMode;
	if (fvMode >= LOOP) fvMode = F1V1;
}

int main(void) {
	// Stop WatchDog Timer
	StopTimer(WatchDog);
	// Initialize Leds
	Initialize(Led, 1); TurnOff(Led, 1);
	Initialize(Led, 3); TurnOff(Led, 3);
	// Initialize Pads
	Initialize(PadLed, 1); TurnOff(PadLed, 1);
	Initialize(PadLed, 2); TurnOff(PadLed, 2);
	Initialize(PadLed, 5); TurnOff(PadLed, 5);
	// Initialize Buttons
	Initialize(Button, S1);
	Initialize(Button, S2);
	// Enable interrupts from buttons
	Interrupt(Enable, Button, S1, ON_PRESS);
	Interrupt(Enable, Button, S2, ON_PRESS);

	// Configure DCO Clock
	ConfigureClockDCO();
	// Enable interrupts from timers
	Interrupt(Enable, TA, 0);
	Interrupt(Enable, TA, 1);
	Interrupt(Enable, TA, 2);
	// Start Timer A0
	flexible.adjust();
	StartTimer(TA, 0, TASSEL__ACLK, flexible.divisor, flexible.delay);

	// Configure Port 7.7 for peripherals
	P7DIR |= BIT7;
	P7SEL |= BIT7;

	// Announcements (Indication)
	TurnOn(Led, 1); TurnOff(Led, 3);
	TurnOn(PadLed, 1); TurnOff(PadLed, 2);

	// Make sure we start in Voltage Level 3
	SetVCoreUp(Voltage::LEVEL1);
	SetVCoreUp(Voltage::LEVEL2);
	SetVCoreUp(Voltage::LEVEL3);

	// Global Interrupts Enable + CCStudio debug
	__bis_SR_register(GIE);
	__no_operation();
}

/// @brief Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	// Stop Timer A0
	StopTimer(TA, 0);
	// If not blinking - stop
	if (!BLINKING_MODE)
		return;
	// Toggle Pad5
	Toggle(PadLed, 5);
	// Start Timer A0 again
	StartTimer(TA, 0, TASSEL__ACLK, flexible.divisor, flexible.delay);
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
	StartTimer(TA, 1, TASSEL__SMCLK, ID__8, DEBOUNCE_DELAY);
}

/// @brief Port2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) PORT2_ISR (void)
#else
#error Compiler not supported!
#endif
{
	// Clear P2.2 interrupt flag
	Interrupt(ClearFlag, Button, S2);

	// False-positive interrupt, ignore
	if (GetStateOf(Button, S2) != ActiveStateOf(Button, S2))
		return;

	// Debouncing
	StartTimer(TA, 2, TASSEL__SMCLK, ID__8, DEBOUNCE_DELAY);
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
	if (GetStateOf(Button, S1) != ActiveStateOf(Button, S1))
		// Button was not actually pressed
		return;
	
	// Button was pressed
	// Switch F1/F2 and V1/V2
	SwitchFV();
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

	// Check if Button S2 is still pressed
	if (GetStateOf(Button, S2) != ActiveStateOf(Button, S2))
		// Button was not actually pressed
		return;
	
	// Button was pressed
	// Switch ACTIVE/LPM
	if (activeMode) {
		// Switch to LPM
		TurnOn(Led, 3); TurnOff(Led, 1);
		__bis_SR_register_on_exit(LPM4_bits);
	} else {
		// Switch to ACTIVE
		TurnOn(Led, 1); TurnOff(Led, 3);
		__bic_SR_register_on_exit(LPM4_bits);
	}
	activeMode = !activeMode;
}
