#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>

// Interface for DOS functions for MSVS/Qt/VSCode/etc.
#ifdef _WIN64
#include "../../dos.h"
#endif
/**
 * @brief Terms of reference:
 * 
 * 1. Запись байтов команды должна выполняться только после проверки незанятости входного регистра контроллера клавиатуры.
 * Проверка осуществляется считыванием и анализом регистра состояния контроллера клавиатуры. 
 * 2. Для каждого байта команды необходимо считывать и анализировать код возврата. В случае считывания кода возврата,
 * требующего повторить передачу байта, необходимо повторно, при необходимости – несколько раз, выполнить передачу
 * байта. При этом повторная передача данных не исключает выполнения всех оставшихся условий.
 * 3. Для определения момента получения кода возврата необходимо использовать аппаратное прерывания от клавиатуры.
 * 4. Все коды возврата должны быть выведены на экран в шестнадцатеричной форме.
 */
typedef unsigned int byte_t;
typedef unsigned int kbrd_port;
#define false 0
#define true 1
typedef unsigned char bool;

enum KBRD_BITS
{
	KBRD_NONE = 0x00,
	KBRD_SCR_LCK_BIT = 0x01,
	KBRD_NUM_LCK_BIT = 0x02,
	KBRD_CPS_LCK_BIT = 0x04,
};
enum
{
	kbrd_controle_register_port = 0x60,
	kbrd_condition_register_port = 0x64, 
};

byte_t kbrd_get(const kbrd_port port)
{
	const byte_t result = inp(port);
	printf("0x%02X port's content: 0x%02X\n", port, result);
	return result;
}

void kbrd_check_if_busy(void)
{
	//while(true) if (kbrd_get(kbrd_condition_register_port) & 0x02 == 0x00) break;
	//printf("Input buffer is free!\n");
}

void kbrd_send(const kbrd_port port, const byte_t command)
{
	for (unsigned short int i = 0u; i <=3; ++i)
	{
		if (i == 3)
		{
			printf("Aborted: Failed to write byte 0x%02X 3 times!\n");
			exit(-1);
		}
		outp(port, command);
		byte_t result = kbrd_get(kbrd_controle_register_port);
		if (result == 0xFA) break;
		if (result == 0xFE) printf("Failed to write byte 0x%02X to port 0x%02X", command, port);
	}
	outp(port, command);
	if (command != 0xED)
		printf("Sending 0x%02X to port 0x%02X succeeded.\n", command, port);
}

void kbrd_wait(void)
{
	for(unsigned int i = 0u; i <= 50000u; ++i)
	{
		if (kbrd_get(kbrd_condition_register_port) & 0x02 != 0x00) continue;
		//printf("Kbrd is free to write...\n");
		return;
	}
	printf("Aborted: Waiting too long...\n");
	exit(-1);
}

int main(void)
{
	unsigned int kbrd_bits_array[10] = { KBRD_NONE, KBRD_SCR_LCK_BIT, KBRD_NONE, KBRD_SCR_LCK_BIT | KBRD_NUM_LCK_BIT,
									   KBRD_NONE, KBRD_NUM_LCK_BIT, KBRD_NONE, KBRD_NUM_LCK_BIT | KBRD_CPS_LCK_BIT,
									   KBRD_NONE, KBRD_SCR_LCK_BIT | KBRD_NUM_LCK_BIT | KBRD_CPS_LCK_BIT };
	for(unsigned int i = 0u; i <= 9u; ++i)
	{
		kbrd_send(kbrd_controle_register_port, 0xED);
		kbrd_wait();
		kbrd_send(kbrd_controle_register_port, kbrd_bits_array[i]);
		sleep(1);
	}
	printf("Enter 'Esc' to quit:\n");
	while (true)
	{
		const char c = getch();
		if (kbrd_get(kbrd_controle_register_port) == 0x01) break;
	}
    kbrd_send(kbrd_controle_register_port, 0xED);
	kbrd_wait();
    kbrd_send(kbrd_controle_register_port, KBRD_NONE);
	exit(0);
}
