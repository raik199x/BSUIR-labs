#pragma once

#include <msp430.h>

#include <mspl/std.hpp>
#include <mspl/msp.hpp>

#define literal static constexpr const

/**
 * @brief Соответствие выводов акселерометра
 * 
 * |-------------|-------|-----------------------------------------------------|
 * | Линия схемы | Выход | Назначение                                          |
 * |-------------|-------|-----------------------------------------------------|
 * | ACCEL_PWR   | 3.6   | Напряжение питания                                  |
 * | ACCEL_SOMI  | 3.4   | Линия приема данных по интерфейсу SPI               |
 * | ACCEL_INT   | 2.5   | Сигнал прерывания                                   |
 * | ACCEL_CS    | 3.5   | Выбор устройства                                    |
 * | ACCEL_SIMO  | 3.3   | Линия передачи данных по интерфейсу SPI             |
 * | ACCEL_SCK   | 2.7   | Синхросигнал                                        |
 * |-------------|-------|-----------------------------------------------------|
 */

/**
 * @brief Регистры акселерометра
 * 
 * |------------|------|----|--------------------------------------------------|
 * | Регистр    | Addr | RW | Назначение                                       |
 * |------------|------|----|--------------------------------------------------|
 * | WHO_AM_I   | 0h   | R  | Идентификационный регистр                        |
 * | REVID      | 1h   | R  | Версия ASIC                                      |
 * | CTRL       | 2h   | RW | Регистр управления                               |
 * | STATUS     | 3h   | R  | Регистр состояния                                |
 * | RSTR       | 4h   | RW | Регистр сброса                                   |
 * | INT_STATUS | 5h   | R  | Регистр состояния прерывания                     |
 * | DOUTX      | 6h   | R  | Регистр данных канала X                          |
 * | DOUTY      | 7h   | R  | Регистр данных канала Y                          |
 * | DOUTZ      | 8h   | R  | Регистр данных канала Z                          |
 * | MDTHR      | 9h   | RW | Регистр порога ускорения в режиме обнаружения    |
 * |            |      |    | движения                                         |
 * | MDFFTMR    | Ah   | RW | Регистр порога времени в режимах обнаружения     |
 * |            |      |    | движения и свободного падения                    |
 * | FF_THR     | Bh   | RW | Регистр порога ускорения в режиме обнаружения    |
 * |            |      |    | свободного падения                               |
 * | I2C_ADDR   | Ch   | R  | Адрес устройства для протокола I2C               |
 * |------------|------|----|--------------------------------------------------|
 */

/**
 * @brief  Отдельные поля регистров акселерометра
 * 
 * |---------|------|-----------|----------------------------------------------|
 * | Регистр | Биты | Поле      | Назначение                                   |
 * |---------|------|-----------|----------------------------------------------|
 * | CTRL    | 7    | G_RANGE   | Диапазон. 0 — 8g, 1 - 2g                     |
 * |         | 6    | INT_LEVEL | Активный уровень сигнала прерывания:         |
 * |         |      |           | 0 - высокий, 1 - низкий                      |
 * |         | 5    | MDET_EXIT | Переход в режим измерения после обнаружения  |
 * |         |      |           | движения                                     |
 * |         | 4    | I2C_DIS   | Выбор интерфейса I2C:                        |
 * |         |      |           | 0 — разрешен, 1 - запрещен                   |
 * |         | 1-3  | MODE      | Режим:                                       |
 * |         |      | [2..0]    | 000 — отключено питание                      |
 * |         |      |           | 001 — измерение, 100 Гц                      |
 * |         |      |           | 010 — измерение, 400 Гц                      |
 * |         |      |           | 011 — измерение, 40 Гц                       |
 * |         |      |           | 100 — обнаружение движения, 10 Гц            |
 * |         |      |           | 101 — обнаружение свободного падения, 100 Гц |
 * |         |      |           | 110 — обнаружение свободного падения, 400 Гц |
 * |         |      |           | 111 — отключено питание                      |
 * |         | 0    | INT_DIS   | Запрещение прерывания (1 - отключен)         |
 * |---------|------|-----------|----------------------------------------------|
 */

namespace CMA3000::constants {
	literal u8 ZERO    = 0x00;
	literal u8 REVID   = 0x01;
	literal u8 CTRL    = 0x02;
	literal u8 RSTR    = 0x04;
	literal u8 MDFFTMR = 0x0A;
	namespace CTRL_BITS {
		literal u8 G_RANGE__2G      = 0b001 << 7;
		literal u8 G_RANGE__8G      = 0b000 << 7;
		literal u8 INT_LEVEL__HIGH  = 0b000 << 6;
		literal u8 INT_LEVEL__LOW   = 0b001 << 6;
		literal u8 MDET_EXIT        = 0b001 << 5;
		literal u8 I2C_DIS          = 0b001 << 4;
		literal u8 MODE__OFF0       = 0b000 << 1;
		literal u8 MODE__MEASURE100 = 0b001 << 1;
		literal u8 MODE__MEASURE400 = 0b010 << 1;
		literal u8 MODE__MEASURE040 = 0b011 << 1;
		literal u8 MODE__MOVE010    = 0b100 << 1;
		literal u8 MODE__FALL100    = 0b101 << 1;
		literal u8 MODE__FALL400    = 0b110 << 1;
		literal u8 MODE__OFF1       = 0b111 << 1;
		literal u8 INT_DIS          = 0b001 << 0;
	}
	literal u8 RX = 0x06;
	literal u8 RY = 0x07;
	literal u8 RZ = 0x08;

	literal i16 WEIGHTS0[] = { 1142, 571 , 286 , 143, 71 , 36 , 18 };
	literal i16 WEIGHTS1[] = { 4571, 2286, 1141, 571, 286, 143, 71 };
	literal u8 BITx[] = { BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };
	#undef literal
}

namespace CMA3000 {
	inline u8 writeCommand(const u8 first, const u8 second) {
		P3OUT &= ~BIT5;              // ACCEL_CS => enable CMA300 SPI data transfer
		u8 indata = UCA0RXBUF;       // reads the receive buffer to clear any pending data
		while(!(UCA0IFG & UCTXIFG)); // waits until the transmit buffer is ready for sending data
		UCA0TXBUF = first;           // sends the first command byte by writing it to the transmit buffer
		while(!(UCA0IFG & UCRXIFG)); // waits until the receive buffer has received data
		indata = UCA0RXBUF;          // reads the receive buffer to store the received data
		while(!(UCA0IFG & UCTXIFG)); // waits until the transmit buffer is ready again
		UCA0TXBUF = second;          // sends the second command byte
		while(!(UCA0IFG & UCRXIFG)); // waits until the receive buffer has received data
		indata = UCA0RXBUF;          // reads the receive buffer to store the received data
		while(UCA0STAT & UCBUSY);    // waits until the SPI bus is no longer busy
		P3OUT |=  BIT5;              // ACCEL_CS => disable CMA300 SPI data transfer
		return indata;
	}
	inline u8 readRegister(const u8 address) {
		const u8 addr = address << 2;// calculate 'the address' + 'R/W' + '0'
		P3OUT &= ~BIT5;              // ACCEL_CS => enable CMA300 SPI data transfer
		u8 result = UCA0RXBUF;       // reads the receive buffer to clear any pending data
		while(!(UCA0IFG & UCTXIFG)); // waits until the transmit buffer is ready for sending data
		UCA0TXBUF = addr;            // sends the register address by writing it to the transmit buffer
		while(!(UCA0IFG & UCRXIFG)); // waits until the receive buffer has received data
		result = UCA0RXBUF;          // reads the receive buffer to store the received data
		while(!(UCA0IFG & UCTXIFG)); // waits until the transmit buffer is ready again
		UCA0TXBUF = 0;               // sends a dummy byte to initiate the read operation
		while(!(UCA0IFG & UCRXIFG)); // waits until the receive buffer has received data
		result = UCA0RXBUF;          // reads the receive buffer to store the received data
		while(UCA0STAT & UCBUSY);    // waits until the SPI bus is no longer busy
		P3OUT |=  BIT5;              // ACCEL_CS => disable CMA300 SPI data transfer
		return result;
	}
	inline void configure(void) {
		// Конфигурирование линии прерывания от CMA300
		P2DIR &= ~BIT5; // ACCEL_INT => mode: input
		P2IES &= ~BIT5; // ACCEL_INT => process on interrupt's front
		P2IFG &= ~BIT5; // ACCEL_INT => clear interrupt flag
		// P2IE  |=  BIT5; // ACCEL_INT => interrupt enable
		P2IE  &= ~BIT5; // ACCEL_INT => interrupt disable

		// Запрещение CMA300 (вывод CSB)
		P3DIR |= BIT5; // ACCEL_CS => mode: output
		P3OUT |= BIT5; // ACCEL_CS => disable CMA300 SPI data transfer

		// Подключение порта USCI_A0
		UCA0CTL1  =  UCSSEL__SMCLK | UCSWRST;                    // USCI_A0 => set clock source as SMCLK, state: start reset
		UCA0BR1   =  0x0; UCA0BR0   =  0x30;                     // USCI_A0 => set baud rate divisor to 0x0030
		UCA0CTL0  =  UCCKPH | UCMSB | UCMST | UCSYNC | UCMODE_0; // USCI_A0 => *
		UCA0CTL1 &= ~UCSWRST;                                    // USCI_A0 => state: stop reset
		// * = clock phase to be captured on the first edge and propagated on the following edge.
		//     the most significant bit (MSB) first during data transmission.
		//     configures the USCI_A0 module as the master for the serial communication.
		//     enables synchronous mode.
		//     sets the USCI_A0 module to 3-pin SPI mode.

		// Конфигурирование и запуск SPI интерфейса
		P2DIR |=  BIT7; // ACCEL_SCK => mode: output
		P2SEL |=  BIT7; // ACCEL_SCK => set peripheral function as USCI_A0CLK
		P3DIR |=  BIT3; // ACCEL_SIMO => mode: output
		P3SEL |=  BIT3; // ACCEL_SIMO => set peripheral function as UCA0SIMO
		P3DIR &= ~BIT4; // ACCEL_SOMI => mode: input
		P3SEL |=  BIT4; // ACCEL_SOMI => set peripheral function as UCA0SOMI
		P3DIR |=  BIT6; // ACCEL_PWR => mode: output
		P3OUT |=  BIT6; // ACCEL_PWR => power CMA3000 on

		using namespace constants;
		using namespace constants::CTRL_BITS;
		// Чтение регистра REVID (Unnecessary, but still...)
		readRegister(REVID);
		// Сброс CMA3000
		writeCommand(RSTR, 0x02);
		writeCommand(RSTR, 0x0A);
		writeCommand(RSTR, 0x04);
		// Инициализация CMA3000 (2g / 400 Hz)
		writeCommand(MDFFTMR, BIT4 | BIT2);
		writeCommand(CTRL, G_RANGE__8G | I2C_DIS | MODE__MEASURE400);
	}
	inline i16 decode(const u8 bits, const u8 G_RANGE, const u8 mode) {
		using namespace constants;
		i16 value = 0;
		int negative = bits & BIT7;
		const i16* WEIGHTS;
		if (
			G_RANGE == CTRL_BITS::G_RANGE__2G      &&
			mode    != CTRL_BITS::MODE__MEASURE040 &&
			mode    != CTRL_BITS::MODE__MOVE010
		) { WEIGHTS = WEIGHTS0; } else { WEIGHTS = WEIGHTS1; }
		for (u8 i = 0; i < 7; ++i) {
			if (negative) {
				value += (constants::BITx[i] & bits) ? 0 : WEIGHTS[i];
			} else {
				value += (constants::BITx[i] & bits) ? WEIGHTS[i] : 0;
			}
		}
		value *= negative ? (-1) : (+1);
		return value;
	}
}
