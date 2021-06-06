// made by Alexander
// bilet 10
// team sessia na 8+

#include<stdio.h>
#include<Windows.h>

//#####################################################################################################################################

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
//// https://habr.com/ru/post/181271/
//void bullshit_happens(FILE* bin) {
//	// в билете дано 2 вида сортировок, но я реализую один, а именно - вставками
//	  //собственно, какая проблема возникает?
//	 // а вот такая - бинарка отказывается воспринимать отрицательный сдвиг (дохера захотел)
//	// поэтому мы кое как выкрутися
//	int swap[2]; //для переменных
//	fpos_t pos, pos_end;
//	rewind(bin);
//	//подготовка к сортировки
//	while (1){ //ищем конец файла
//		fread(&swap[0], sizeof(int), 1, bin);
//		if (feof(bin)) break;
//	}
//	fgetpos(bin, &pos_end); //еще не понял? будем идти с конца
//	rewind(bin); //возвращаемся на место
//	pos = pos_end - sizeof(int)*2; // допустим у нас комбинация 4 7 6, для моего способа нам нужно стать на 7
//	fsetpos(bin, &pos); //установили положение
//
//	//сортировка
//	while (pos >= 0){
//		fread(&swap[0], sizeof(int), 1, bin); //записываем 7
//		fread(&swap[1], sizeof(int), 1, bin); //записываем 6
//		while (swap[1] < swap[0] && pos < pos_end) { //если мы выловили, что наш порядок нарушен
//			fsetpos(bin, &pos);                        //устанавлиаем положение семерки
//			fwrite(&swap[1], sizeof(int), 1, bin);    //ставим на ее место 6
//			fpos_t temp; fgetpos(bin, &temp);        //запоминаем временную позицию второй щестерки ( 4 6 (6) )
//			fwrite(&swap[0], sizeof(int), 1, bin);  // перепесиываем ее семеркой (4 6 7)
//			fsetpos(bin, &temp);                   // возвращаемся на положение 6 чтобы проверить нарушена ли комбинация перед нами
//			fread(&swap[0], sizeof(int), 1, bin); // записываем значения
//			fread(&swap[1], sizeof(int), 1, bin);// все еще записываем
//			if (feof(bin))                      // если мы в конце файла, Значит всё уже отсортировано
//				break;                         //выход из цикла
//			pos += sizeof(int);               //ставим на позицию вперед
//		}
//		pos -= sizeof(int); //спускаемся к началу
//		fsetpos(bin, &pos); //устанавливаем позицию
//	}
//	printf("done!");
//	system("pause");
//	//в чем минус моего алгоритма?
//	//тратится больше времени из-за того что я сдвигаю указатель в файле вперед, тем самым на обработку попадается чуть больше значение (хоть они и отсортированы)
//}
//
//int main() {
//	FILE* bin; // мой указатель на файлик
//	//конечно мы могли сделать ввод от руки и всё такое, но нам же не это важно, а сама реализация, так что
//	if (fopen_s(&bin, "my_bin.bin", "r+"))  //проверяем создан ли файлик
//		fopen_s(&bin, "my_bin.bin", "w+"); //если нет, то создаем
//	int option = 0, proverka;
//	while (1) {   //меню, на экзе оно нахер будет нужно, но у нас оно будет
//		system("cls");
//		rewind(stdin);
//		printf("1. Add to file\n2. Reset file\n3. Do the task\n4. output\n>");
//		// ставлю деньги, что по условию в файле уже будут какие-то данные
//	   //но мне не лень, добавлю функцию ввода
//		proverka = scanf_s("%d", &option);  // конечно же не забываем мои любимые проверочки
//		if (!proverka)
//			return 0;
//		switch (option){
//		case 1:  file_add(bin); break;
//		case 2:  fclose(bin); fopen_s(&bin, "my_bin.bin", "w+"); printf("reseted!\n"); system("pause"); break; //ну типо лол и так всё понятно
//		case 3:  bullshit_happens(bin); break; //та самая функция
//		case 4:  file_print(bin); break;
//		default: return 0;
//		}
//	}
//}


//#####################################################################################################################################
//однонаправленая очердеь.... хмммм
struct query {
	int number;
	query* next;
};

void add(query** fucker) {
	system("cls");
	query* block;
	block = (query*)malloc(sizeof(query));
	printf("enter number: ");
	int proverka, number;
	proverka = scanf_s("%d", &number);
	while (!proverka)
		proverka = scanf_s("%d", &number);
	block->number = number;
	block->next = nullptr;
	if ((*fucker) == nullptr) {
		(*fucker) = block;
		return;
	}
	query *end = (*fucker);
	while (end->next != nullptr) {
		end = end->next;
	}
	end->next = block;
}

void print(query* fucker) {
	while (fucker) {
		printf("%d - ", fucker->number);
		fucker = fucker->next;
	}
	system("pause");
}

void deleter(query** fucker) {
	if (!(*fucker)) {
		printf("ti kto takoy 4tobi tak delat??\n");
		return;
	}
	//способ будет таков:
	int delimoe; //вводим номер делимого
	int proverka; // честно она уже заебала
	proverka = scanf_s("%d", &delimoe); 
	while (!proverka)
		proverka = scanf_s("%d", &delimoe);

	//самое удаление
	query* block = (*fucker), *past;  //нам нужен указатель удаляемого и прошлого
	past = (query*)malloc(sizeof(query));
	for (int i = 0; i < proverka; i++) {
		if (block == nullptr) {          //проверка на хуету
			printf("xyeta a ne nomer!");
			system("pause");
			return;
		}
		past = block;
		block = block->next;
	}
	if (block == nullptr) {   //вообще она тут нужна,  можно выпилить из цикла
		printf("xyeta a ne nomer!");
		system("pause");
		return;
	}
	if ((*fucker) == block) {  //если это единственный элемент то НАХУЙ ЕГО
		free(block);
		(*fucker) = nullptr;
		return;
	}
	past->next = block->next; //нужно сохранить последовательност
	free (block);            //очистка
}

int main() {
	query* fucker; //просто не спрашивай
	fucker = (query*)malloc(sizeof(query)); //заранее выделяю память на один элемент
	fucker = nullptr;
	int option, proverka;
	while (1) {   //меню, на экзе оно нахер будет нужно, но у нас оно будет
		system("cls");
		rewind(stdin);
		printf("1. Add\n2. delete\n3. print\n>");
		// ставлю деньги, что по условию в файле уже будут какие-то данные
	   //но мне не лень, добавлю функцию ввода
		proverka = scanf_s("%d", &option);  // конечно же не забываем мои любимые проверочки
		if (!proverka)
			return 0;
		switch (option){
		case 1:  add(&fucker); break;
		case 2:  deleter(&fucker); break; //та самая функция
		case 3:  print(fucker); break;
		default: return 0;
		}
	}
}

//#####################################################################################################################################