// powered by alexander 050504
#include <stdio.h>

//#define scanf_s scanf // uncomment for linux

int main() {
	int mas[100][100], n, vector[100], k = 0, whyistillhere = 0; //говорящие названия
	printf("enter size of massiv ");
	scanf_s("%d", &n);
	for (int i = 0; i < n; i++) { // ввод массива
		scanf_s("%d", &vector[i]);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) { // по начала массив идет (к примеру) 1 2 3 4, но потом он должен идти 2 3 4 1 
			if (j + k >= n) { // тогда надо к j добавлять k чтобы сделать данный переход
				for (int m = 0; m < n - j; m++) { //но в один момент k+j > n и тогда нужно сделать переход, переход для ввода старых переменных
					mas[i][j+whyistillhere] = vector[m]; //делает m
					whyistillhere += 1; // чтобы разные переменные не сохранялись в одну и ту же ячейку мы каждый раз будем прибавлять это
				} //значение (и постоянно увеличивать)
				whyistillhere = 0; //в конечном итоге мы ёё обнулим чтобы провернуть все махинации заново
				break; //т.к. столбцы уже заполнены мы переходим к новой строчке
			} // profit
			mas[i][j] = vector[j+k];
		}
		k++;
	}
	printf("output\n"); //аккуратно выводим
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", mas[i][j]);
		}
		printf("\n");
	}
}
