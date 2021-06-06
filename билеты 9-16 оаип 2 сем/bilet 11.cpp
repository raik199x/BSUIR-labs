//bilet 11
// made by AlexanderXD
//team sessia na 8+

#include <stdio.h>
#include <Windows.h>

//############################################################################################################

//void file_add(FILE* bin) {
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
//void file_print(FILE* bin) {
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
//void bullshit_happens(FILE* first, FILE* second, FILE* third) {
//	//ситуация будет напоминать 10-ый билет, первое задние
//	//в файле first and second мы будем идти с конца и до нуля (значения fpos_t)
//	// ну, и просто записывать в третий файл, не то чтобы сложно, но дрючить эти 3 файла...
//	fpos_t end_first = 0, end_second = 0;
//	int temp;
//	rewind(first); rewind(second); rewind(third); //обновляем все три файла (кстати по факту третий можно было не трогать)
//	while (!feof(first)) //находим конец первого файла
//		fread(&temp, sizeof(int), 1, first);
//	while (!feof(second))   //находим конец второгой файла
//		fread(&temp, sizeof(int), 1, second);
//	//ну а теперь начинаем как-то записывать в 3-ий файл
//	fpos_t pos_first = end_first - sizeof(int), pos_second = end_second - sizeof(int); // берем наши последние значения
//																					  // возможно у тебя вопрос: зачем делать -sizeof(int)?
//																					 // ответ прост - мы стоим на eof поэтому мы двигуаем на одну позицию назад
//	int select[2];											   	 //переменная для хранения чисел
//	bool skip_first = false, skip_second = false;			    // флаг окончания файла
//	rewind(first); rewind(second);							   //обновляем файлы
//	fsetpos(first, &pos_first); fsetpos(second, &pos_second); //становимся на позиции
//	while (pos_first >= 0 || pos_second >= 0) {              //пока есть еще цифры для считывания - мы работаем
//		fsetpos(first, &pos_first); fsetpos(second, &pos_second);
//		if (pos_first >= 0)                          //пока остались цифры - мы их считываем
//			fread(&select[0], sizeof(int), 1, first);
//		else
//			skip_first = 1;                      // иначе ставим флаг пропуска
//		if (pos_second >= 0)
//			fread(&select[1], sizeof(int), 1, second);
//		else
//			skip_second = 1;
//		if (skip_first == 1) { //если флаг пропуска стоит
//			fwrite(&select[1], sizeof(int), 1, third);
//			pos_second -= sizeof(int);
//			continue;
//		}
//		else if (skip_second == 1) {    // если флаг пропуска стоит
//			fwrite(&select[0], sizeof(int), 1, third);
//			pos_first -= sizeof(int);
//			continue;
//		}
//		if (select[0] < select[1]) {     //просто сравниваем числа
//			fwrite(&select[0], sizeof(int), 1, third);
//			pos_first -= sizeof(int);
//		}
//		else {
//			fwrite(&select[1], sizeof(int), 1, third);
//			pos_second -= sizeof(int);
//		}
//	}
//}
//
//int main() {
//	FILE* first, * second, * third;
//	if (fopen_s(&first, "first.bin", "r+"))  //проверяем создан ли файлик
//		fopen_s(&first, "first.bin", "w+"); //если нет, то создаем
//	if (fopen_s(&second,"second.bin","r+"))
//		fopen_s(&second,"second.bin","w+");
//	if (fopen_s(&third, "third.bin", "r+"))
//		fopen_s(&third, "third,bin", "w+");
//	//тут такой момент, что тяжело воссоздать ситуацию с такими двумя бинарными файл
//	//поэтому я просто добавлю функцию изменения двух бинарок
//    //если вы напишите там залупу, виноват буду уже не я
//	int option = 0, proverka;
//		while (1) {   //меню, на экзе оно нахер будет нужно, но у нас оно будет
//			system("cls");
//			rewind(stdin);
//			printf("1. Add to file\n2. Reset all files\n3. Do the task\n4. output\n>");
//			// ставлю деньги, что по условию в файле уже будут какие-то данные
//		   //но мне не лень, добавлю функцию ввода
//			proverka = scanf_s("%d", &option);  // конечно же не забываем мои любимые проверочки
//			if (!proverka)
//				return 0;
//			switch (option){
//			case 1:  system("cls");
//					 rewind(stdin);
//					 printf("1. First binary file\n2. Second binary\n");
//					 proverka = scanf_s("%d", &option);
//					 switch (option) {
//						case 1:	file_add(first); break;
//						case 2: file_add(second); break;
//						default: break;
//			         }
//				     break;
//			case 2:  fclose(first); fopen_s(&first, "first.bin", "w+"); fclose(second); fopen_s(&second, "second.bin", "w+"); fclose(third); fopen_s(&third, "first.bin", "w+"); printf("reseted!\n"); system("pause"); break; //ну типо лол и так всё понятно
//			case 3:  bullshit_happens(first,second,third); break; //та самая функция
//			case 4: system("cls");
//					rewind(stdin);
//					printf("1. First binary file\n2. Second binary\n3. Third binary file\n");
//					proverka = scanf_s("%d", &option);
//					switch (option) {
//					case 1:	file_print(first); break;
//					case 2: file_print(second); break;
//					case 3: file_print(third); break;
//					default: break;
//					} break;
//			default: return 0;
//			}
//		}
//}

//############################################################################################################
//честно, из условия я не понимаю как должен жить и существовать стек, должна ли быть просто его структура или в нем что-то задержиться
//с этими вопросами я живу и по сей день, поэтому я просто воссоздаю его структуру
// в дальнейшем не будет никакой итерации со стеком, я просто рили хуй знает что с этим делать
struct stack {
	int num;
	stack* next;
};

struct football_teams {
	char* team_name;
	stack check;
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
	char temp, *bufer;
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
	football_teams* probeg = (*circle), * end = (*circle), *remember; //поэтому я создаю 3 указателя на прошлый, текущий элемент и точку остановки
	do{ //ищем конец
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

void do_the_task(football_teams** circle) {
	if ((*circle) == nullptr) { //проверочка
		return;
	}
	//к самому заданию: опять же, непонятно итерация со стеком, поэтому я просто устрою псевдо перезапись
	//основная задача найти и уничтожить, что же, начинаеи искать
	int calculating = 0, max = 0; //будем искать


	//снова заводим переменную пробега и конца (и прошлого)
	football_teams* current = (*circle), * end = (*circle), * past = current, *deleter = nullptr;
	do {
		char temp = '1';    
		while (temp != '\0') {      //будем учитывать длину строки с учетом нуль символа
			temp = current->team_name[calculating];
			calculating++;
		}
		if (calculating > max) {       //запоминаем значение если надо
			max = calculating;
			deleter = current;        //и удаляемый объект
		}
		calculating = 0;
		current = current->right;
	} while (current != end); 

	//просто так мы удалить не можем, нам еще надо найти предыдущий элемент от удляемого, ищем
	//но есть одно исключение
	if (deleter == (*circle) && (*circle)->right == (*circle)) { // если элемент является единственным в кольце
		free(deleter->team_name);
		free(deleter);
		(*circle) = nullptr;
		return;
	}
	current = (*circle);
	while (deleter != current) { //ищем предыдущий элемент
		past = current;
		current = current->right;
	}

	//теперь когда мы всё нашли можем приступить
	past->right = deleter->right;
	deleter->right->check = deleter->check; //вот типо тот самый стэк
	free(deleter->team_name);
	free(deleter);
	return;
}

int main() {
	football_teams* circle;
	circle = nullptr;
	while (1) {
		system("cls");
		printf("1. Make circle\n2. Output\n3. Do the task\n>");
		rewind(stdin);
		int option;
		//print_menu();
		checker_num(option);
		switch (option) {
		case 1: make_circle(&circle); break;
		case 2: output(circle); system("pause"); break;
		case 3: do_the_task(&circle); break;
		default: return 0;
		}
	}
}