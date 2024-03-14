#pragma once

#include <mspl/msp.hpp>
#include <mspl/traits/buttons.hpp>

#define gpcl_Initialize_Button(name) \
	gpcl(Button, name, dir) |= gpcl(Button, name, bit); \
	gpcl(Button, name, ren) |= gpcl(Button, name, bit); \
	gpcl(Button, name, out) |= gpcl(Button, name, bit)

#define gpcl_GetStateOf_Button(name) \
	(gpcl(Button, name, in) & gpcl(Button, name, bit))

#define gpcl_ActiveStateOf_Button(name) \
	gpcl(Button, name, abs)

#define gpcl_Interrupt_Enable_Button(name, condition) \
	if (condition == ON_PRESS)   gpcl(Button, name, ies) |= gpcl(Button, name, bit); \
	if (condition == ON_RELEASE) gpcl(Button, name, ies) &= ~gpcl(Button, name, bit); \
	gpcl(Button, name, ie) |= gpcl(Button, name, bit)

#define gpcl_Interrupt_Disable_Button(name) \
	gpcl(Button, name, ie) &= ~gpcl(Button, name, bit)

#define gpcl_Interrupt_ClearFlag_Button(name) \
	gpcl(Button, name, ifg) &= ~gpcl(Button, name, bit)
