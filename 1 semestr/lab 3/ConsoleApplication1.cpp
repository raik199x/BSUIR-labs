#include <stdio.h>
int main() {
	int mas[100], n, kolvo = 0,suma = 0;
	printf("enter amount ");
	scanf_s("%d", &n); // вводим кол-во элементов
	for (int i = 0; i < n; i++) {
		scanf_s("%d", &mas[i]); // заполняем массив
		if (mas[i] % 5 == 0 && mas[i] % 7 != 0) { //делаем проверку делимость на 5 и неделимости на 7
			kolvo = kolvo + 1; // в случае успеха увилечиваем количество на 1
			suma = suma + mas[i]; // и суммируем
		}
	}
	printf("kolichestvo:%d\nsumma:%d", kolvo, suma); // выводим ответ
}
