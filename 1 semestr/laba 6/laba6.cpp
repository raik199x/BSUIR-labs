// laba 6 powered by alexander
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
int main() {
	int *m, *m2, n, max, kordi[2], flag = 0, n2;
	printf("welcom user!\nenter n size of matrix\n");
	scanf_s("%d", &n); // вод размера
	n2 = n - 1;
	m = (int*)malloc(sizeof(int)*n*n); // инициализация динамического двумерного массива
	system("CLS"); // очистка консоли
	printf("dear user, enter the whole matrix\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf_s("%d", m + i * n + j); // по стандарту вводим
		}
	}
	system("CLS"); // очистка консоли
	max = *(m + 0 * n + 0); //берем 1 число
	//ищем наибольшее число по модулю
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(max) < abs(*(m + i * n + j)) || abs(max) == abs(*(m + i * n + j))) { // здесь добавленая еще одна проверка
				max = *(m + i * n + j);                                                // мол, если числа получились одинаковые, то запоминаем его корды
				kordi[0] = i; //запоминаем корды
				kordi[1] = j; // тоже корды
			}
		}
	}
	//время гениального мозга (или вывод ответа)
	 // я не стал создавать отдельный массив, а просто аккуратно вывел уже существующий
	for (int i = 0; i < n; i++) {
		if (i == kordi[0]) {  //у нас же появляется строка которая должна полностью исчезнуть, чтобы не создавался лишний пробел мы поставим метку
			flag = 1; //вот собственно и она
		}
		for (int j = 0; j < n; j++) {
			if (i == kordi[0] || j == kordi[1]) { // если тригер на корды сработал просто скипаем их
				continue;
			}
			printf("%d ", *(m + i * n + j));
		}
		if (flag == 1) { //по итогу метка срабатывает, и мы пропускаем пробел (который бы выглядел как пустая линия)
			flag = 0; //убераем тригер
		}
		else {
			printf("\n");
		}
	}
}