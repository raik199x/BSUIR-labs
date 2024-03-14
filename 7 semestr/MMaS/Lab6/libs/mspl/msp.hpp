#pragma once

#include <gpcl/gpcl.h>

#define Initialize(device, ...)        gpcl(Initialize, device)(__VA_ARGS__)
#define TurnOff(device, ...)           gpcl(TurnOff, device)(__VA_ARGS__)
#define TurnOn(device, ...)            gpcl(TurnOn, device)(__VA_ARGS__)
#define Toggle(device, ...)            gpcl(Toggle, device)(__VA_ARGS__)
#define GetStateOf(device, ...)        gpcl(GetStateOf, device)(__VA_ARGS__)
#define ActiveStateOf(device, ...)     gpcl(ActiveStateOf, device)(__VA_ARGS__)
#define StopTimer(device, ...)         gpcl(StopTimer, device)(__VA_ARGS__)
#define StartTimer(device, ...)        gpcl(StartTimer, device)(__VA_ARGS__)
#define Interrupt(action, device, ...) gpcl(Interrupt, action, device)(__VA_ARGS__)

#define gpcl_squash(a, b, c) a##b##c
#define squash(a, b, c) gpcl(squash)(a, b, c)

#define msp_bit(bit) BIT##bit

// CLang Error Suppression
#if defined(__clang__)
	#define __bic_SR_register_on_exit(bits) __no_operation()
	#define __bis_SR_register_on_exit(bits) __no_operation()
	#define __delay_cycles(cycles)          __no_operation()
#endif

static constexpr const bool ON_PRESS   = true;
static constexpr const bool ON_RELEASE = false;
