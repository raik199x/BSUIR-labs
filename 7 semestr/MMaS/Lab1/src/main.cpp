#include <msp430.h>

/// @brief Task
#include "task.hpp"

#include <mspl/std.hpp>
#include <mspl/msp.hpp>
#include <mspl/timers.hpp>
#include <mspl/buttons.hpp>
#include <mspl/leds.hpp>

//! \brief Loop limit for LED blinking
static const u16 BLINKING_LIMIT = 10000;
static const u16 DEBOUNCING_LIMIT = 15;

enum class ButtonSelector {
	S1,
	S2,
};

void ActivePause(u16 loopLimit) {
	for (volatile u16 i = 0; i < loopLimit; ++i) {
		__no_operation();
	}
}

bool GetButtonsState(ButtonSelector button) {
	bool initial_state = false, final_state = false;
	switch (button) {
	case ButtonSelector::S1:
		initial_state = (GetStateOf(Button, S1) == ActiveStateOf(Button, S1));
		break;
	case ButtonSelector::S2:
		initial_state = (GetStateOf(Button, S2) == ActiveStateOf(Button, S2));
		break;
	}
	ActivePause(DEBOUNCING_LIMIT);
	switch (button) {
	case ButtonSelector::S1:
		final_state = (GetStateOf(Button, S1) == ActiveStateOf(Button, S1));
		break;
	case ButtonSelector::S2:
		final_state = (GetStateOf(Button, S2) == ActiveStateOf(Button, S2));
		break;
	}
	return initial_state && final_state;
}

//! \brief Infinite loop
void InfiniteLoop() {
	struct {
		bool b1, b2;
	} currentStatus  = { false, false },
		previousStatus = { false,  false  };
	bool blinking = false;
	#define EventFront(button) (currentStatus.button && !previousStatus.button)

	while (true) {
		//! \details Get current buttons' statuses
		currentStatus.b1 = GetButtonsState(ButtonSelector::S1);
		currentStatus.b2 = GetButtonsState(ButtonSelector::S2);

		//! \details Check if buttons' statuses changed from 0 to 1
		if (EventFront(b1)) {
			blinking = !blinking;
		}
		if (EventFront(b2)) {
			blinking = !blinking;
		}

		//! \details Save current buttons' statuses as previous ones
		previousStatus = currentStatus;

		if (blinking)
			Toggle(Led, 2);
		blinking = false;
		ActivePause(BLINKING_LIMIT);
	}
	#undef EventFront
}

int main(void) {
	/// Stop watchdog timer
	StopTimer(WatchDog);
	/// Initialize Buttons S1-2 and Led2
	Initialize(Button, S1);
	Initialize(Button, S2);
	Initialize(Led, 2); TurnOff(Led, 2);
	/// Enter infinite loop
	InfiniteLoop();
}
