//powered by Alexander
// все изменение массива проводились тактикой: сдвинуть в правл - удалить
#include <stdio.h>
#include <malloc.h>
int main() {
	int n,flag=0, flag2 = 1,m=0,k=0;
	char *mas, z;
	printf("enter length:\n");
	scanf_s("%d", &n);
	n += 1;
	mas = (char*)malloc(sizeof(char)*n); //динамическое выделение
	printf("type message:\n");
	//ввод
	for (int i = 0; i < n; i++) {
		scanf_s("%c", &mas[i]);
	}
	//находит пробелы в начале
	if (mas[1] == ' ') {
		for (int i = 1; i < n; i++) {
			if (mas[i] == ' ') {
				flag += 1;
			} else
			if (mas[i] != ' ') {
				break;
			}
		}
	}
 //(если они есть) убираем пробелы в начале
	if (mas[1] == ' ') {
		for (int i = 1; i < n; i++) {
			mas[i] = mas[i + flag];
		}
		n -= flag;
	}
	// удаление пробелов в конце
	while (mas[n - 1] == ' ') {
		n--;
	}
	//убираем двойные пробелы
	while (flag2 == 1) {
		k = 0;
		//цикл удаления
		for (int i = 0; i < n; i++) {
			if (mas[i] == ' ') {
				m += 1;
			}
			else {
				m = 0;
			}
			if (m >= 2) {
				flag = i;
				for (; flag-m < n; flag++) {
					mas[flag] = mas[flag + 1]; //сдвиул вправо
				}
				n--; //удалид
			}
		}
		// цикл проверки
		for (int i = 0; i < n; i++) {
			if (mas[i] == ' ') {
				k += 1;
			}
			else {
				k = 0;
			}
			if (k >= 2) {
				flag2 = 1;
				break; //ну как бы очевидно что есть лишний пробел значит пробегаем назад циткл
			}
			else {
				flag = 0; //обнуляем флаг
			}
		}
		if (k == 0) { // если лишних пробелов нету = навыход
			break;
		}
	}
	//результат
	for (int i = 0; i < n; i++) {
		printf("%c", mas[i]);
	}
}