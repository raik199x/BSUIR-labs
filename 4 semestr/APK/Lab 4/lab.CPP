	#include <stdio.h>
	#include <conio.h>
    #include <dos.h>

	// Массив частот для мелодии
	int mary[] = 
	{
	  349, 392, 440, 349, 440, 440, 392,
	  349, 392, 0
	};
	
	// Массив длительностей
	int del[] = 
	{
	  600, 300, 600,  300, 300, 300,  300,
	  300, 300, 300, 300
	};

	void tm_sound(int freq, int time) 
	{
	  int cnt;
	
	  // Задаем режим канала 2 таймера
	  outp(0x43, 0xb6);
	
	  // Вычисляем задержку для загрузки в
	  // регистр счетчика таймера
	  cnt = (int)(1193180L / freq);
	
	  // Загружаем регистр счетчика таймера - сначала
	  // младший, затем старший байты
	  outp(0x42, cnt & 0x00ff);
	  outp(0x42, (cnt &0xff00) >> 8);
	
	  // Включаем громкоговоритель. Сигнал от
	  // канала 2 таймера теперь будет проходить
	  // на вход громкоговорителя
	  outp(0x61, inp(0x61) | 3);
	
	  // Выполняем задержку.
      delay(time);
	
	  // Выключаем громкоговоритель.
	  outp(0x61, inp(0x61) & 0xfc);
	}

    

    void show_status(){
      unsigned i;	
	  printf("\n\nChannel 0\n-------\n");
	
	  // Читаем слово состояния канала,
	  // команда 0xe2 = 11100010B
	  outp(0x43, 0xe2);
	
	  printf("\nWord status channel: %02.2X",
	    inp(0x40));
	
	  // Читаем текущее состояние регистра счетчика
	  // канала. Для этого вначале выдаем команду CLC
	  // для канала 0. Код этой команды - 0x00
	  outp(0x43, 0x00);
	
	  // Вводим младший и старший байты счетчика
	  // и отображаем его.
	  i = inp(0x40);
	  i = (inp(0x40) << 8) + i;
	
	  printf("\nRegister counter:       %04.4X",i);
	
	  // Повторяем те же действия для 1 и 2 каналов.
	  printf("\n\nChannel 1\n-------\n");
	  outp(0x43, 0xe4);
	  printf("\nWord status channel: %02.2X",inp(0x41));
	  outp(0x43, 0x40);
	  i = inp(0x41);
	  i = (inp(0x41) << 8) + i;
	  printf("\nRegister counter:       %04.4X",i);
	
	  printf("\n\nChannel 2\n-------\n");
	  outp(0x43, 0xe8);
	  printf("\nWord status channel: %02.2X",inp(0x42));
	  outp(0x43, 0x80);
	  i = inp(0x42);
	  i = (inp(0x42) << 8) + i;
	  printf("\nRegister counter:       %04.4X",i);
      printf("\n\n");
    }

	int main() {
        char letter;
        while(1){
            printf("1. show status\n2. Play music\n3.End program\n> ");
            scanf("%c",&letter);
            getchar();
            switch(letter){
                case '1': show_status(); break;
                case '2':{
                        	int i;
	                        for(i=0 ;mary[i] != 0 ;i++)
	                            tm_sound(mary[i], del[i]);
                            break;
                }
                case '3': return 0;
            }
        }
        return 0;
	}