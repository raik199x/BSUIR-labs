//bilet 13
//made by AlexanderXD
//team sessia na 8+

#include <stdio.h>
#include <Windows.h>

//############################################################################################################

//void add_bin(FILE* bin) {
//	system("cls");
//	printf("entering number\nprintf letter to stop\n");
//	int temp, number[1];
//	fseek(bin, 0, 2);
//	rewind(stdin);
//	while (1) {
//		temp = scanf_s("%d", &number[0]);
//		if (!temp) {
//			rewind(stdin);
//			break;
//		}
//		fwrite(number, sizeof(int), 1, bin);
//	}
//	printf("entreing is done\n");
//	system("pause");
//}
//
//void print_bin(FILE* bin) {
//	system("cls");
//	int j;
//	rewind(bin);
//	do {
//		fread(&j, sizeof(int), 1, bin);
//		if (feof(bin))  break;
//		printf("%d ", j);
//	} while (1);
//	puts("");
//	system("pause");
//}
//
//void add_txt(FILE* txt) {
//	system("cls");
//	printf("entering number\nprintf letter to stop\n");
//	fseek(txt, 0, 2);
//	int temp, number;
//	while (1) {
//		temp = scanf_s("%d", &number);
//		if (!temp) {
//			rewind(stdin);
//			break;
//		}
//		fprintf(txt, "%d ", number);
//	}
//	puts("entering done!");
//	system("pause");
//}
//
//void print_txt(FILE *txt) {
//	rewind(txt);
//	system("cls");
//	int number = 1000;
//	while (!feof(txt)) {
//		fscanf_s(txt, "%d", &number);
//		if (feof(txt)) {
//			rewind(stdin);
//			break;
//		}
//		printf("%d ", number);
//	}
//	puts("");
//	system("pause");
//}
//
//void do_the_task(FILE* txt, FILE* bin) {
//	//что? снова ни слова про доп массив? ну....
//	int* massiv,temp,counter = 0;
//	massiv = (int*)malloc(sizeof(int));
//	rewind(bin); rewind(txt);
//	//забираем значения из бинарки
//	while (1) {
//		fread(&temp, sizeof(int), 1, bin);
//		if (feof(bin))
//			break;
//		massiv[counter] = temp;
//		counter++;
//		massiv = (int*)realloc(massiv,(counter + 1) * sizeof(int));
//	}
//
//	//ищем лишние значения
//	while (1) {
//		fscanf_s(txt, "%d", &temp);
//		if (feof(txt))
//			break;
//		for (int i = 0; i < counter; i++)
//			if (temp == massiv[i]) {           //если находим
//				for (int j = i; j < counter - 1; j++)  
//					massiv[j] = massiv[j + 1];			//все смещаем
//				counter--;								//уменьшаем длину
//				massiv = (int*)realloc(massiv, (counter + 1) * sizeof(int));
//			}
//	}
//
//	fclose(bin); fopen_s(&bin, "binary.bin", "w+"); //ресетаем бин
//	//записываем все
//	for (int i = 0; i < counter; i++)
//		fwrite((massiv + i), sizeof(int), 1, bin);
//
//	free(massiv);
//	puts("done!");
//	system("pause");
//}
//
//int main() {
//	int option, proverka;
//	FILE* bin, * txt;
//	if (fopen_s(&bin, "binary.bin", "r+"))
//		fopen_s(&bin, "binary.bin", "w+");
//	if (fopen_s(&txt, "text.txt", "r+"))
//		fopen_s(&txt, "text.txt", "w+");
//	while (1) {
//		system("cls");
//		printf("1. Add to bin\n2. Add to txt\n3. output bin\n4. output txt\n5. do the task\n6. Regenerate files\n>");
//		proverka = scanf_s("%d", &option);
//		while (!proverka)
//			proverka = scanf_s("%d", &option);
//		switch (option){
//		case 1: add_bin(bin); break;
//		case 2: add_txt(txt); break;
//		case 3: print_bin(bin); break;
//		case 4: print_txt(txt); break;
//		case 5: do_the_task(txt, bin); break;
//		case 6: fclose(bin); fclose(txt); fopen_s(&bin, "binary.bin", "w+"); fopen_s(&txt, "text.txt", "w+"); printf("done!\n"); system("pause"); break;
//		default: return 0;
//		}
//	}
//}

//############################################################################################################
//сортировка кольца само по себе ебнутое задание, а когда оно однонаправленное, становится очень не приятно
struct football_teams {
	char* team_name;
	football_teams* right;
};

void checker_num(int& n) {  //checks correcting written number
	int temp = scanf_s("%d", &n);
	while (temp != 1 || n <= 0 || n > 9999999) {
		if (temp != 1) {
			printf("detected letters, do not use them\n");
		}
		else if (n == 0) {
			printf("0 pointer\n");
		}
		else if (n < 0) {
			printf("detected negative number, unacceptable\n");
		}
		else if (n > 999999) {
			printf("we are sorry! our programm dont work with number higher than 999999\n");
		}
		rewind(stdin);
		temp = scanf_s("%d", &n);
	}
}

char* entering_team_name() {
	char temp, * bufer;
	int i = 0, n = 1;
	bufer = (char*)malloc(1 * sizeof(char));
	rewind(stdin);                                                      //free console bufer
	while (1) {
		scanf_s("%c", &temp);
		if (i == 0 && temp == ' ' || i == 0 && temp == '\n') {     // exceptions
			printf("name cannot start with ' ' or dont have any name\n");
			continue;
		}
		if (temp == '\n') {                                    // if this is the end = leave
			bufer[i] = '\0';
			break;
		}
		bufer[i] = temp;                                   // writting letter
		i++; n++;                                         //increase for space
		bufer = (char*)realloc(bufer, n * sizeof(char)); //increase space for name
	}
	return bufer; //sent back pointer on team name
}

void make_circle(football_teams** circle) {
	system("cls");
	int numbers;
	football_teams* new_block;
	new_block = (football_teams*)malloc(sizeof(football_teams));
	printf("enter team name: ");
	//start to add infromation**************************************************
	new_block->team_name = entering_team_name();
	//end to fill up information block********************************************

	if ((*circle) == nullptr) {
		printf("cricle succesfully created!\n");
		(*circle) = new_block;
		(*circle)->right = (*circle);
		system("pause");
		return;
	}
	//из-за того что кольцо однонаправленное тяжело добавить что-то в конец
	football_teams* probeg = (*circle), * end = (*circle), * remember; //поэтому я создаю 3 указателя на прошлый, текущий элемент и точку остановки
	do { //ищем конец
		remember = probeg;
		probeg = probeg->right;
	} while (probeg != end);

	new_block->right = (*circle);
	remember->right = new_block;
	printf("cricle succesfully created!\n");
	system("pause");
}

void output(football_teams* teams) {
	if (!teams) {
		puts("queu is empty");
		return;
	}
	football_teams* end = teams;
	do {
		printf("%s\n", teams->team_name);
		teams = teams->right;
	} while (teams != end);
}

void swap(football_teams* first, football_teams* second) {
	football_teams* change;
	change = (football_teams*)malloc(sizeof(football_teams));
	change->team_name = second->team_name;
	second->team_name = first->team_name;
	first->team_name = change->team_name;

	return;
}
//https://habr.com/ru/post/181271/
void do_the_task(football_teams** circle) {
	if (*circle == nullptr)
		return;
	//именно эта сортировка будет очень простой т.к. мы сортируем по названиям структуры, нам нужно просто поменять названия
	//ну а еще вы должны помнить сортировку вставками, лично я немного ее искорякою, но в целом должно быть норм
	football_teams* current = (*circle), * start = (*circle), * end = (*circle); //текущий - будет идти до конца, начало нужно будет для поиска места замены, конец очевидно
	do {
		while (current != start) { 
			if (strcmp(current->team_name, start->team_name) == -1)
				swap(current, start);
			else
				start = start->right;
		}
		start = (*circle);
		current = current->right;
	} while (current != end);
	return;
	//я думаю что функция понятно, так что сори, комментов минимум
}

int main() {
	football_teams* circle;
	circle = nullptr;
	while (1) {
		system("cls");
		printf("1. Make circle\n2. Output\n3. Do the task\n>");
		rewind(stdin);
		int option;
		checker_num(option);
		switch (option) {
		case 1: make_circle(&circle); break;
		case 2: output(circle); system("pause"); break;
		case 3: do_the_task(&circle); break;
		default: return 0;
		}
	}
}

//############################################################################################################