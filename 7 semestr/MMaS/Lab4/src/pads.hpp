#pragma once

#include <msp430.h>

#include <mspl/std.hpp>
#include <mspl/msp.hpp>

static constexpr const u16 CHARGE_DELAY = 1000;

enum class PadNumber {
	PAD1 = BIT0,
	PAD2 = BIT1,
	PAD3 = BIT2,
	PAD4 = BIT3,
	PAD5 = BIT4,
};

inline auto disable_pads(void) -> void {
	P6DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4);
	P6SEL &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4);
	P6OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4);
}

inline auto charge_pads(const PadNumber left, const PadNumber right) -> void {
	const u16 LEFT_BIT = static_cast<u16>(left);
	const u16 RIGHT_BIT = static_cast<u16>(right);
	// 1. Зарядить емкость
	// a. Перевести линию 6.x в I/O, подать высокий уровень
	P6DIR |= (LEFT_BIT | RIGHT_BIT);
	P6SEL &= ~(LEFT_BIT | RIGHT_BIT);
	P6OUT |= (LEFT_BIT | RIGHT_BIT);
	// b. На линию 1.6 завести землю
	P1DIR |= BIT6;
	P1SEL |= BIT6;
	P1OUT &= ~BIT6;
	// 2. Небольшая задержка (__delay_cycles(n))
	__delay_cycles(CHARGE_DELAY);
	// 3. Перевести линию 6.х на вход с разрешением прерываний
	P6DIR &= ~(LEFT_BIT | RIGHT_BIT);
	// 4. Запустить таймер (время таймера выбирается для каждого варианта экспериментально)
}

inline auto configure_pads(const PadNumber left, const PadNumber right) -> void {
	const u16 LEFT_BIT = static_cast<u16>(left);
	const u16 RIGHT_BIT = static_cast<u16>(right);
	// 5. По прерыванию таймера произвести измерения (перевести линию 6.х для работы c периферией)
	P6DIR |= (LEFT_BIT | RIGHT_BIT);
	P6SEL |= (LEFT_BIT | RIGHT_BIT);
}

inline auto restore_pads(const PadNumber left, const PadNumber right) -> void {
	const u16 LEFT_BIT = static_cast<u16>(left);
	const u16 RIGHT_BIT = static_cast<u16>(right);
	// +. Restore
	P6DIR &= ~(LEFT_BIT | RIGHT_BIT);
	P6SEL &= ~(LEFT_BIT | RIGHT_BIT);
}
