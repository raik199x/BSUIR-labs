#include <msp430.h>

/// @brief Task
#include "task.hpp"

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/timers.hpp>
#include <mspl/buttons.hpp>
#include <mspl/leds.hpp>
#include <mspl/pads.hpp>

/**
 * @category Timer1 interrupt modes
 * @details Timer1 can be used either to debounce the button press or trigger the selected LED
 */
static constexpr bool DEBOUNCING = true;
static constexpr bool TRIGGERING = false;

/**
 * @category Timer1 delay constants
 * @details Picked empirically, tested on MSP430F5529
 */
static constexpr u16 DEBOUNCE_DELAY = 0x0F00;
static constexpr u16 TRIGGER_DELAY = 0xA000;

/// @brief Utility variable to store button presses
volatile u8 buttonPressCounter = 0;
/// @brief Utility variable to store the current state of Timer1
volatile bool timerState = DEBOUNCING;

int main(void) {
	/// Stop watchdog timer, initialize button S1, Led1-3, Pad1-5
	StopTimer(WatchDog);
	Initialize(Button, S1);
	Initialize(Led, 1); TurnOff(Led, 1);
	Initialize(Led, 2); TurnOff(Led, 2);
	Initialize(Led, 3); TurnOff(Led, 3);
	Initialize(PadLed, 1); TurnOff(PadLed, 1);
	Initialize(PadLed, 2); TurnOff(PadLed, 2);
	Initialize(PadLed, 3); TurnOff(PadLed, 3);
	Initialize(PadLed, 4); TurnOff(PadLed, 4);
	Initialize(PadLed, 5); TurnOff(PadLed, 5);

	/**
	 * Set P1.7 interrupt to rising edge
	 * @details Alternatively, we can set it to falling edge using
	 * @code P1IES &= ~BIT7; @endcode
	 */
	P1IES |= BIT7;
	/// Enable P1.7 interrupt
	P1IE |= BIT7;

	/// Switch to LPM0 with interrupts enabled
	__bis_SR_register(LPM0_bits + GIE);
	/// Enable CCStudio debugger
	__no_operation();
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
	/// Clear P1.7 interrupt flag
	P1IFG &= ~BIT7;

	/// False-positive interrupt, ignore
	if (GetStateOf(Button, S1) != ActiveStateOf(Button, S1))
		return;

	/// @category Configure Timer1 to debounce mode
	timerState = DEBOUNCING;
	StartTimer(TA, 1, TASSEL__SMCLK, ID__8, DEBOUNCE_DELAY);
}

/// @brief Timer1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	/// Disable P1.7 interrupt
	P1IE &= ~BIT7;
	/// Stop Timer1
	StopTimer(TA, 1);

	/// In the debounce mode - Check if button is still pressed
	if ((timerState == DEBOUNCING) && (GetStateOf(Button, S1) == ActiveStateOf(Button, S1))) {
		/// The S1 button is most likely pressed here
		++buttonPressCounter;
	}

	/// In the trigger mode - Toggle the selected LED
	if (timerState == TRIGGERING) {
		switch (buttonPressCounter) {
			case 1: Toggle(Led, 1); break;
			case 2: Toggle(Led, 2); break;
			case 3: Toggle(Led, 3); break;
			case 4: Toggle(PadLed, 1); break;
			case 5: Toggle(PadLed, 2); break;
			case 6: Toggle(PadLed, 3); break;
			case 7: Toggle(PadLed, 4); break;
			case 8: Toggle(PadLed, 5); break;
			default:
				Toggle(Led, 1); Toggle(Led, 2); Toggle(Led, 3);
				Toggle(PadLed, 1); Toggle(PadLed, 2); Toggle(PadLed, 3);
				Toggle(PadLed, 4); Toggle(PadLed, 5);
				break;
		}
		buttonPressCounter = 0;
	}

	/// Check if counter is not zero to start or restart Triggering
	if (buttonPressCounter != 0) {
		/// @category Configure Timer1 to trigger mode
		/// Set Timer1 state to triggering
		timerState = TRIGGERING;
		StartTimer(TA, 1, TASSEL__SMCLK, ID__8, TRIGGER_DELAY);
	}

	/// Re-enable P1.7 interrupt
	P1IE |= BIT7;
}
