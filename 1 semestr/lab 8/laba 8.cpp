 // laba 8 (ultimate edition) 
//powered by AlexanderXD
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {
	char *mas, *mas1, temp = 's';
	int n = 1, i = 0, zhongler = 0;
	mas = (char*)malloc(n * sizeof(char)); //заранее выделяем память на 1 переменную
	mas1 = (char*)malloc(n * sizeof(char)); //заранее выделяем память на 1 переменную
	n = 0;
	//начинаем вводить
	while (1) {
		scanf_s("%c", &temp); // таможня для переменных
		if (temp == '\n') {  //если текущий символ является переходом на новую строку, то...
			break;          // ...покидаем помещение
		}
		//по скольку мы не знаем сколько памяти нам нужно, с каждым веденным символом мы будем жонглировать памятью
	  // а именно отдавать все переменны в мас1 или в мас.
		// Note from future (2023): those 2 if's might be replaced with realloc =)
		if (zhongler % 2 == 0) { 
			zhongler++; //"перекидываем в другую руку"
			mas[i] = temp; //забираем значение с таможни
			n++; //увеличиваем кол-во памяти
			i++; // переходим на следующий эллемент
			mas1 = (char*)malloc(n * sizeof(char)); //выделяем память для следующего жонглирования
			for (int j = 0; j < n; j++) {
				mas1[j] = mas[j];  //передаем данные
			}
		}
		//т.к. все действия аналогичны объяснять их не будем
		else {
			zhongler++; 
			mas1[i] = temp;
			n++;
			i++;
			mas = (char*)malloc(n * sizeof(char));
			for (int j = 0; j < n; j++) {
				mas[j] = mas1[j];
			}
		}
	}
	// для удобства я хочу пользоваться одним массивом, и если данные записаны в мас1 я переписываю их в мас
	if (zhongler % 2 == 0) {
		mas = (char*)malloc(n * sizeof(char));
		for (int j = 0; j < n; j++) {
			mas[j] = mas1[j];
		}
	}
  //теперь занимаемся поиском и выводом нужным нам слова
 // для этого нам нужно знать координаты слова (начало конец)
// длину текущего слова и длину наибольшего (на текущий момент) слова
	int koordi1 = 0, koordi2 = 0, dl_slova = 0, max_slovo = 0;
	for (i = 0; i < n; i++) {
		if (mas[i] != ' ' && mas[i] != '.') {
			dl_slova++; //замеряем слово
		}
		else  {
			if (dl_slova >= max_slovo) {
				max_slovo = dl_slova;      //если текущее слово больше предыдущего, то меняем все данные
				dl_slova = 0;             // не забываем сбрасывать счётчик букв
				koordi1 = i - max_slovo; //начало слова
				koordi2 = i;            //конец
			}
			else {
				dl_slova = 0;         // не забываем сбрасывать счётчик букв
			}
		}
		if(mas[i] == '.') {
			for (int j = koordi1; j < koordi2; j++) {
				printf("%c", mas[j]);
			}
			printf("\n");
			max_slovo = 0;   //переход на новую строчку
			dl_slova = 0;   // а значит нужно забыть прошлое...
			koordi1 = 0;   // ...и жить настоящим
			koordi2 = 0;
		}
	}
}

//пометка из будущего: всю систему *жонглирования* можно заменить командой realloc, не допускайте моей неопытности
