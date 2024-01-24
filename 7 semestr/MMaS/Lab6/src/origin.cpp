#include <cmath>

#include <msp430.h>

#include <msp-exp/CTS/CTS_HAL.h>
#include <msp-exp/CTS/CTS_Layer.h>
#include <msp-exp/CTS/structure.h>

#include <msp-exp/FatFs/diskio.h>
#include <msp-exp/FatFs/ff.h>
#include <msp-exp/FatFs/ffconf.h>
#include <msp-exp/FatFs/integer.h>
#include <msp-exp/FatFs/mmc.h>

#include <msp-exp/MSP-EXP430F5529_HAL/HAL_Cma3000.h>
#include <msp-exp/MSP-EXP430F5529_HAL/HAL_Dogs102x6.h>
#include <msp-exp/MSP-EXP430F5529_HAL/HAL_SDCard.h>

#define CALADC12_15V_30C *((signed int*)0x1A1A) // Temperature sensor calibration 1.5V 30C
#define CALADC12_15V_85C *((signed int*)0x1A1C) // Temperature sensor calibration 1.5V 85C
#define CALADC12_20V_30C *((signed int*)0x1A1E) // Temperature sensor calibration 2.0V 30C
#define CALADC12_20V_85C *((signed int*)0x1A20) // Temperature sensor calibration 2.0V 85C
#define CALADC12_25V_30C *((signed int*)0x1A22) // Temperature sensor calibration 2.5V 30C
#define CALADC12_25V_85C *((signed int*)0x1A24) // Temperature sensor calibration 2.5V 85C

// Данные и позиции для рисования дополнительной информации
#define DRAW_TEXT_ROW 7
static char *SD_TEXT = (char *)"SD_DRAW_MODE";

#define LINE_Y 41

// Имя файла для хранения буфера
#define FILE_NAME "buffer.bin"

// Буфер
#define BUFFER_SIZE 10
#define BUFFER_COUNT (BUFFER_SIZE / 2)
volatile uint16_t buffer[BUFFER_COUNT];
volatile uint8_t idx = 0;

unsigned int temp;
int MAPPING_VALUES[] = {4571, 2286, 1141, 571, 286, 143, 71};
uint8_t BITx[] = {BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0};

void set_timers() {
	/*  800 ACLK/1 циклов or 800*1/32Khz = 25ms  */
	TA2CCR0 = 800 - 1; // Период в 62,500 цикла, от 0 до 62,499.
	TA2CTL = TIMER_ACLK | TIMER_SOURCE_DIV_0;
	TA2CTL |= (TACLR + MC__UP);
}

long int termo_sensor_value = 0;
// получаем данные термодатчика
void start_adc(void) {
	ADC12CTL0 = ADC12ON;
	ADC12CTL1 = ADC12SHP | ADC12SSEL_0;
	// switch
	// case 0x0040
	ADC12CTL0 |= ADC12SHT0_12 | ADC12REFON | ADC12REF2_5V;
	ADC12CTL1 |= ADC12CSTARTADD_2;
	ADC12CTL2 &= ~ADC12TCOFF;
	ADC12IE = ADC12IE2;
	// end
	ADC12CTL0 |= ADC12ENC;
	ADC12CTL0 |= ADC12SC;
	__bis_SR_register(LPM0_bits | GIE);
}

signed int temperature_adc;
void calc_temp(void) {
	termo_sensor_value = (signed char)((temperature_adc - CALADC12_25V_30C) * (85.0 - 30.0) / (CALADC12_25V_85C - CALADC12_25V_30C) + 30.0);
}

void setupADC(void) {
	ADC12CTL2 = ADC12RES_2;
	REFCTL0 &= ~REFMSTR;
	ADC12CTL2 |= ADC12TCOFF;
	ADC12MCTL2 = ADC12INCH_10 | ADC12SREF_1;
	ADC12IFG = 0;
}

long int parseProjectionByte(uint8_t projectionByte) {
	int i = 0;
	long int projectionValue = 0;
	int isNegative = projectionByte & BIT7;
	for (; i < 7; i++) {
		if (isNegative) {
			projectionValue +=
				(BITx[i] & projectionByte) ? 0 : MAPPING_VALUES[i];
		} else {
			projectionValue +=
				(BITx[i] & projectionByte) ? MAPPING_VALUES[i] : 0;
		}
	}
	projectionValue *= isNegative ? -1 : 1;
	return projectionValue;
}

void str_from_number(long int val, uint8_t *string) {
	uint8_t i = 0;
	if (val >= 0) {
		string[i++] = '+';
	} else {
		string[i++] = '-';
		val *= -1;
	}
	if (val == 0)
		string[i++] = '0';
	while (val > 0) {
		string[i++] = val % 10 + '0';
		val /= 10;
	}
	//   string[i] = '\0';
	string[i] = ' ';
	i--;
	uint8_t j = 1;
	for (; i >= j; i--) {
		uint8_t temp = string[i];
		string[i] = string[j];
		string[j] = temp;
		j++;
	}
}

// Вычисления значения для рисования
uint16_t Get_Draw_Value(uint8_t idx) {
	uint16_t data_range = 85;
	int16_t value = buffer[idx];
	if (value < 0) {
		value = 0;
	}
	uint16_t draw_value =
		(uint16_t)((float)DOGS102x6_X_SIZE * (float)value / (float)data_range);
	return draw_value;
}

void setupPins() {
	// led 1 on p1.0
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
}

void enLed1(int f) {
	switch (f) {
	case 0:
		P1OUT &= ~BIT0;
		break;
	case 1:
		P1OUT |= BIT0;
		break;
	default:
		P1OUT &= ~BIT0;
		break;
	}
}

int main(void) {
	// Остановка сторожевого таймера
	WDTCTL = WDTPW + WDTHOLD;
	setupADC();
	setupPins();
	FATFS fs;
	FIL file;
	int k = 8;
	// Инициализация экрана
	Dogs102x6_init();
	Dogs102x6_backlightInit();
	Dogs102x6_setBacklight(255);
	Dogs102x6_clearScreen();
	Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1, LINE_Y,
								 DOGS102x6_DRAW_NORMAL);
	Cma3000_init();
	// Монтирование диска
	FRESULT res = f_mount(0, &fs);
	if (res == FR_NO_FILESYSTEM) {
		f_mkfs(0, 0, 512);
	}
	// Работа DMA по прерыванию TA2CCR0 CCIFG(из даташита)
	DMACTL0 = DMA0TSEL_5;
	// Одиночная пересылка, включение DMA, разрешение прерываний
	// нет инкремента dst, src, размер данных 16 бит.
	DMA0CTL = DMADT_0 + DMAEN + DMAIE;
	// Размер = 1
	DMA0SZ = 1;
	// Источник - значение счетчика TA1.
	__data16_write_addr((unsigned short)&DMA0SA,
						(unsigned long)&termo_sensor_value);
	// Назначение - в элемент массива.
	__data16_write_addr((unsigned short)&DMA0DA, (unsigned long)&buffer[idx]);
	set_timers();
	uint8_t first_press_PAD = 0;
	uint8_t no_press_PAD = 0;
	uint8_t file_draw = 0;
	UINT bw = 0;
	__enable_interrupt();
	while (1) {
		// Вход в режим LPMO с разрешением прерываний
		//__bis_SR_register(LPM0_bits+GIE);
		__enable_interrupt();
		start_adc();
		calc_temp();
		Cma3000_readAccel();
		volatile long int xAxisProjection = parseProjectionByte(Cma3000_xAccel);
		// Рисуем значение на экран
		if (file_draw == 0) {
			if (idx == BUFFER_COUNT - 1) {
				int u;
				for (u = 0; u < BUFFER_COUNT; u++) {
					uint16_t draw_value = Get_Draw_Value(u);
					uint8_t x_pos = u * k;
					if (x_pos % 8 == 0) {
						if (x_pos == 40) {
							Dogs102x6_clearRow(x_pos / 8);
							Dogs102x6_horizontalLineDraw(
								0, DOGS102x6_X_SIZE - 1, LINE_Y,
								DOGS102x6_DRAW_NORMAL);
						} else {
							Dogs102x6_clearRow(x_pos / 8);
						}
					}
					Dogs102x6_pixelDraw(draw_value, x_pos,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 1,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 2,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 3,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 4,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 5,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 6,
										DOGS102x6_DRAW_NORMAL);
					Dogs102x6_pixelDraw(draw_value, x_pos + 7,
										DOGS102x6_DRAW_NORMAL);
				}
				// запись буфера в файл
				f_open(&file, FILE_NAME, FA_WRITE | FA_CREATE_ALWAYS);
				f_write(&file, (void *)buffer, BUFFER_SIZE, &bw);
				f_close(&file);
			}
			uint8_t string1[128] = {"      Temperature\0"};
			uint8_t string2[128] = {"      Acceleromet\0"};
			str_from_number(buffer[idx], string1);
			str_from_number(xAxisProjection, string2);
			Dogs102x6_clearRow(DRAW_TEXT_ROW);
			Dogs102x6_clearRow(DRAW_TEXT_ROW - 1);
			Dogs102x6_stringDraw(DRAW_TEXT_ROW, 0, (char *)string1,
								 DOGS102x6_DRAW_NORMAL);
			Dogs102x6_stringDraw(DRAW_TEXT_ROW - 1, 0, (char *)string2,
								 DOGS102x6_DRAW_NORMAL);
		}
		// Считываем значения буфера из файл по нажатию кнопки
		int keypressed = xAxisProjection <= -2000 || xAxisProjection >= 4000;
		if (keypressed == 0) {
			no_press_PAD = 0;
		}
		if (keypressed && no_press_PAD == 0) {
			if (first_press_PAD == 0) {
				first_press_PAD = 1;
			} else if (first_press_PAD == 1) {
				enLed1(1);
				no_press_PAD = 1;
				first_press_PAD = 0;
				file_draw ^= 1;
				if (file_draw) {
					f_open(&file, FILE_NAME, FA_READ);
					f_read(&file, (void *)buffer, BUFFER_SIZE, &bw);
					Dogs102x6_clearScreen();
					Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1,
												 LINE_Y, DOGS102x6_DRAW_NORMAL);
					Dogs102x6_stringDraw(DRAW_TEXT_ROW, 0, SD_TEXT,
										 DOGS102x6_DRAW_NORMAL);
					f_close(&file);
					uint16_t i = 0;
					for (i = 0; i < BUFFER_COUNT; i++) {
						uint16_t draw_value = Get_Draw_Value(i);
						Dogs102x6_pixelDraw(draw_value, k * i,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 1,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 2,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 3,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 4,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 5,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 6,
											DOGS102x6_DRAW_NORMAL);
						Dogs102x6_pixelDraw(draw_value, k * i + 7,
											DOGS102x6_DRAW_NORMAL);
					}
				}
				enLed1(0);
			}
		}
		idx++;
		if (idx == BUFFER_COUNT) {
			idx = 0;
		}
		// Обновляем адрес назначения DMA
		__data16_write_addr((unsigned short)&DMA0DA,
							(unsigned long)&buffer[idx]);
		// Запуск DMA
		DMA0CTL |= DMAEN;
		// Таймер A1 используется для измерения релаксационных циклов сенсора,
		// которые подключены к TACLK.
		TA2CTL |= (TACLR + MC__UP);
	}
}

/// @brief Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(DMA_VECTOR))) DMA_ISR(void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(DMAIV, 16)) {
	// Прерывание DMA0IFG
	case 2:
		// Остановка таймеров
		// TA1CTL &= ~MC__CONTINUOUS;
		TA2CTL &= ~MC__UP;
		// Выход из LPM0
		_bic_SR_register_on_exit(LPM0_bits);
		break;
	default:
		break;
	}
}

/// @brief ADC12 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(ADC12IV, ADC12IV_ADC12IFG15)) {
		case ADC12IV_ADC12IFG2: // MEM2 data ready (temperature)
		temperature_adc = ADC12MEM2;
		ADC12CTL2 |= ADC12TCOFF;
		ADC12CTL0 &= ~ADC12REFON;
		ADC12CTL0 &= ~ADC12ON;
		LPM0_EXIT;
		break;
	default:
		break;
	}
}
