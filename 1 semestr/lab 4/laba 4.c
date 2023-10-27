//powered by me alexaaaander 050504

//#define scanf_s scanf // uncomment for linux

#include <stdio.h>
int main() {
	int mas[100], n, m = 0, k = 1, fact = 1; 
	printf("enter size of massiv ");
	scanf_s("%d", &n);
	printf("enter massiv ");
	for (int i = 0; i < n; i++) { 
		scanf_s("%d", &mas[i]); //вводим массив
		if (mas[i] < 0) { // проверяем является ли число < 0 (отрицательным)
			mas[i] = 0; // если да то приравниваем его 0
			m+=1; // запоминаем его существование
		}
	}
	while (k <= m) {
		fact *= k; 
		k+=1;
		}
	for (int i = 0; i < n; i++) {
		printf("%d ", mas[i]); // выводим наш новый массив (хоть по условию не надо, но мы же не просто так нули ставили)
	}
	printf("\n");
	if (m == 0) {
		printf("0"); //частный случай если отрицательных чисел не было изначально а факториал сразу = 1
	} else
	printf("%d", fact); //выводим факториал
}
