#pragma once

#include <cmath>

#include <msp430.h>

#include <mspl/std.hpp>
#include <mspl/msp.hpp>

#define literal static constexpr const

/**
 * @brief Соответствие выводов ЖКИ экрана
 * 
 * |-------------|-------|-----------------------------------------------------|
 * | Линия схемы | Выход | Назначение                                          |
 * |-------------|-------|-----------------------------------------------------|
 * | LCD_RST     | 5.7   | Сброс (= 0)                                         |
 * | SIMO        | 4.1   | SIMO данные                                         |
 * | SCLK        | 4.3   | Синхросигнал                                        |
 * | LCD_D/C     | 5.6   | Режим: 0 — команда, 1 — данные                      |
 * | LCD_CS      | 7.4   | Выбор устройства (= 0)                              |
 * | LCD_BL_EN   | 7.6   | Питание подсветки                                   |
 * |-------------|-------|-----------------------------------------------------|
 */

namespace DOGS102W6::constants {
	namespace SET {
		literal u8 COLUMN_ADDRESS_LSB       = 0x00;
		literal u8 COLUMN_ADDRESS_MSB       = 0x10;
		literal u8 PAGE_ADDRESS             = 0xB0;
		literal u8 SEG_DIRECTION            = 0xA0;
		literal u8 COM_DIRECTION            = 0xC0;
		literal u8 POWER_CONTROL            = 0x2F;
		literal u8 SCROLL_LINE              = 0x40;
		literal u8 VLCD_RESISTOR_RATIO      = 0x27;
		literal u8 ELECTRONIC_VOLUME_MSB    = 0x81;
		literal u8 ELECTRONIC_VOLUME_LSB    = 0x0F;
		literal u8 ALL_PIXEL_ON             = 0xA4;
		literal u8 INVERSE_DISPLAY          = 0xA6;
		literal u8 DISPLAY_ENABLE           = 0xAF;
		literal u8 LCD_BIAS_RATIO           = 0xA2;
		literal u8 ADV_PROGRAM_CONTROL0_MSB = 0xFA;
		literal u8 ADV_PROGRAM_CONTROL0_LSB = 0x90;
	}
	literal u8 INIT[] = {
		SET::SCROLL_LINE,
		SET::SEG_DIRECTION,
		SET::COM_DIRECTION,
		SET::ALL_PIXEL_ON,
		SET::INVERSE_DISPLAY,
		SET::LCD_BIAS_RATIO,
		SET::POWER_CONTROL,
		SET::VLCD_RESISTOR_RATIO,
		SET::ELECTRONIC_VOLUME_MSB,
		SET::ELECTRONIC_VOLUME_LSB,
		SET::ADV_PROGRAM_CONTROL0_MSB,
		SET::ADV_PROGRAM_CONTROL0_LSB,
		SET::DISPLAY_ENABLE,
		SET::PAGE_ADDRESS,
		SET::COLUMN_ADDRESS_MSB,
		SET::COLUMN_ADDRESS_LSB,
	};
	literal u8 symbols[12][6] = {
		{0xff, 0x81, 0x81, 0x81, 0x81, 0xff}, // UPPER 0
		{0x08, 0x10, 0x20, 0x40, 0x80, 0xff}, // UPPER 1
		{0x9f, 0x91, 0x91, 0x91, 0x91, 0xf1}, // UPPER 2
		{0x91, 0x91, 0x91, 0x91, 0x91, 0xff}, // UPPER 3
		{0xf0, 0x10, 0x10, 0x10, 0x10, 0xff}, // UPPER 4
		{0xf1, 0x91, 0x91, 0x91, 0x91, 0x9f}, // UPPER 5
		{0xff, 0x91, 0x91, 0x91, 0x91, 0x9f}, // UPPER 6
		{0x80, 0x80, 0x80, 0x80, 0x80, 0xff}, // UPPER 7
		{0xff, 0x91, 0x91, 0x91, 0x91, 0xff}, // UPPER 8
		{0xf1, 0x91, 0x91, 0x91, 0x91, 0xff}, // UPPER 9
		{0x00, 0x08, 0x08, 0x3e, 0x08, 0x08}, // UPPER +
		{0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // UPPER -
	};
}

namespace DOGS102W6 {
	using namespace constants;
	inline void writeCommand(const u8* const command, const usize size) {
		usize i = size;
		const u8* cmd = command;
		P7OUT &= ~BIT4;                   // LCD_CS => enable DOGS102W6 SPI data transfer
		P5OUT &= ~BIT6;                   // LCD_D/C => switch to COMMAND LCD MODE
		while (i) {
			while (!(UCB1IFG & UCTXIFG)); // waits until the transfer buffer has transferred data
			UCB1TXBUF = *cmd;             // sends the selected command by writing it to the transmit buffer
			++cmd;                        // iterate to the next command
			--i;                          // decrement iteration counter
		}
		while (UCB1STAT & UCBUSY);        // waits until the SPI bus is no longer busy
		UCB1RXBUF;
		P7OUT |= BIT4;                    // LCD_CS => LCD_CS => disable DOGS102W6 SPI data transfer
	}
	inline void initialize(void) {
		P5DIR |=  BIT7; // LCD_RST => mode: output
		P5OUT &=  BIT7; // LCD_RST => state: start reset
		P5OUT |=  BIT7; // LCD_RST => state: stop reset
		P7DIR |=  BIT4; // LCD_CS => mode: output
		P5DIR |=  BIT6; // LCD_D/C => mode: output
		P5OUT &= ~BIT6; // LCD_D/C => switch to COMMAND LCD MODE
		P4DIR |=  BIT1; // SIMO => mode: output
		P4SEL |=  BIT1; // SIMO => set peripheral function as UCA1SIMO
		P4DIR |=  BIT3; // SCLK => mode: output
		P4SEL |=  BIT3; // SCLK => set peripheral function as USCI_A1CLK
		UCB1CTL1  = UCSSEL__SMCLK | UCSWRST;                     // USCI_A1 => set clock source as SMCLK, state: start reset
		UCB1BR1   =  0; UCB1BR0 = 0x02;                          // USCI_A1 => set baud rate divisor to 0x0002
		UCB1CTL0  =  UCCKPH | UCMSB | UCMST | UCSYNC | UCMODE_0; // USCI_A1 => *
		UCB1CTL1 &= ~UCSWRST;                                    // USCI_A1 => state: stop reset
		UCB1IFG  &= ~UCRXIFG;                                    // USCI_A1 => clear interrupt flag
		// * = clock phase to be captured on the first edge and propagated on the following edge.
		//     the most significant bit (MSB) first during data transmission.
		//     configures the USCI_A0 module as the master for the serial communication.
		//     enables synchronous mode.
		//     sets the USCI_A0 module to 3-pin SPI mode.
		writeCommand(INIT, sizeof(INIT)/sizeof(*INIT));
	}
	inline void backlight(void) {
		P7DIR |=  BIT6; // LCD_BL_EN => mode: output
		P7OUT |=  BIT6; // LCD_BL_EN => power LCD BACKLIGHT on
		P7SEL &= ~BIT6; // LCD_BL_EN => clear peripheral function configuration
	}
	inline void setAddress(u8 pa, u8 ca){
		u8 cmd[1];
		if (pa > 7) pa = 7;
		if (ca > 131) ca = 131;
		cmd[0] = SET::PAGE_ADDRESS + pa;
		u8 H = 0x00;
		u8 L = 0x00;
		u8 ColumnAddress[] = { SET::COLUMN_ADDRESS_MSB, SET::COLUMN_ADDRESS_LSB };
		L = (ca & 0x0F);
		H = (ca & 0xF0);
		H = (H >> 4);
		ColumnAddress[0] = SET::COLUMN_ADDRESS_LSB + L;
		ColumnAddress[1] = SET::COLUMN_ADDRESS_MSB + H;
		writeCommand(cmd, 1);
		writeCommand(ColumnAddress, 2);
	}
	inline void writeData(const u8* const data, const usize size){
		usize i = size;
		const u8* dp = data;
		P7OUT &= ~BIT4;
		P5OUT |=  BIT6;
		while (i){
			while (!(UCB1IFG & UCTXIFG));
			UCB1TXBUF = *dp;
			++dp;
			--i;
		}
		while (UCB1STAT & UCBUSY);
		UCB1RXBUF;
		P7OUT |= BIT4;
	}
	inline void full_clear(void) {
		literal u8 LcdData[] = { 0x00 };
		for (u8 p = 0; p < 8; p++) {
			setAddress(p, 0);
			for (u8 c = 0; c < 132; c++) {
				writeData(LcdData, 1);
			}
		}
	}
	inline void partial_clear(void) {
		literal u8 LcdData[] = { 0x00 };
		literal u8 p = 0u;
		setAddress(p, 0);
		for (u8 c = 0; c < 132; c++) {
			writeData(LcdData, 1);
		}
	}
	static u8 MirrorColMode = 0;
	inline void setMirrorColDisplay(void) {
		u8 cmd[] = {SET::COM_DIRECTION};
		if(MirrorColMode == 1) {
			cmd[0] = SET::COM_DIRECTION + 0x08;
		} else {
			cmd[0] = SET::COM_DIRECTION;
		}
		writeCommand(cmd, 1);
	}
	static u8 MirrorSegMode = 0;
	inline void setMirrorSegDisplay(void) {
		u8 cmd[] = {SET::SEG_DIRECTION};
		if(MirrorSegMode == 1) {
			cmd[0] = SET::SEG_DIRECTION + 0x01;
		} else {
			cmd[0] = SET::SEG_DIRECTION;
		}
		writeCommand(cmd, 1);
	}
	inline usize getNumberLength(i16 number) {
		usize length = 0;
		number = std::abs(number);
		if(number == 0)
			return 1;
		while(number) {
			number /= 10;
			++length;
		}
		return length;
	}
	static u8 COLUMN_START_ADDRESS = 30;
	inline void printNumber(i16 number, const usize length) {
		u8 COL_ADR = COLUMN_START_ADDRESS;
		literal u8 pa = 0;
		if(number < 0) {
			setAddress(pa, COL_ADR);
			writeData(symbols[11], 6);
		} else {
			setAddress(pa, COL_ADR);
			writeData(symbols[10], 6);
		}
		i16 divider = std::pow(10, length - 1);
		number = std::abs(number);
		for (usize i = 1; i <= length; i++) {
			i16 digit = number / divider;
			setAddress(pa, COL_ADR + (i * 2) + (i * 6));
			writeData(symbols[digit], 6);
			number %= divider;
			divider /= 10;
		}
	}
	inline void display(const i16 number, const bool mirrored, const usize length) {
		if (!mirrored)
			partial_clear();
		printNumber(number, length);
	}
	inline void mirror(void) {
		partial_clear();
		setMirrorColDisplay();
		setMirrorSegDisplay();
	}
	inline void configure(void) {
		initialize();
		backlight();
		mirror();
		full_clear();
	}
}
