//bilet 14
//made by AlexanderXD
//team sessia na 8+

#include <stdio.h>
#include <Windows.h>

//############################################################################################################
////опять же здесь нужно воссоздать ситуацию, поэтому я оставил обычную функцию добавления
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
//void do_the_task(FILE* bin) {
//	//для начала хотя бы ведем число
//	//если файл изначально не отсортирован - виноваты вы
//	system("cls");
//	printf("entering number without loosing out sorting: ");
//	int enter, checker;
//	checker = scanf_s("%d", &enter);
//	while (!checker)
//		checker = scanf_s("%d", &enter);
//
//	//теперь наша цель пробежать по файлу, чтобы найти место для числа, и всё сдвинуть к херам =)
//	//но для начала найдём конец файла
//	fpos_t end;
//	rewind(bin);
//	fseek(bin, 0, 2);
//	fgetpos(bin, &end);
//	end -= sizeof(int);
//
//	//обрабатываем исключение
//	if (end <= 0) {
//		fwrite(&enter, sizeof(int), 1, bin);
//		return;
//	}
//	rewind(bin);
//	
//	//теперь наш основной цикл
//	fpos_t probeg; int temp;
//	fgetpos(bin, &probeg);
//	while (probeg <= end) {
//		fread(&temp, sizeof(int), 1, bin);
//		if (temp >= enter) { //когда мы нашли позицию
//			while (probeg <= end+sizeof(int)) {
//				int next;                            //нам нужно запомнить следующиее значение чтобы не потерять его
//				fsetpos(bin, &probeg);              //становимся на позицию замеы
//				fread(&next, sizeof(int), 1, bin); //запоминаем ее для дальнейшего помещения
//				fsetpos(bin, &probeg);            //возвращаемся на позицию замены
//				fwrite(&enter, sizeof(int), 1, bin);//вписывае наше значение
//				enter = next;                  //для будущей записи
//				probeg += sizeof(int);        //переходим на след позицию
//			}
//			return;
//		}
//		else {
//			probeg += sizeof(int);
//			continue;
//		}
//	}
//	//если мы дошли до сюда, значин наш элемент должен быть самым последним
//	fwrite(&enter, sizeof(int), 1, bin);
//}
//
//int main() {
//	int option, proverka;
//	FILE* bin;
//	if (fopen_s(&bin, "binary.bin", "r+"))
//		fopen_s(&bin, "binary.bin", "w+");
//	while (1) {
//		system("cls");
//		printf("1. Add to bin\n2. print bin\n3. do the task\n4. regenerate files\n>");
//		proverka = scanf_s("%d", &option);
//		while (!proverka)
//			proverka = scanf_s("%d", &option);
//		switch (option){
//		case 1: add_bin(bin); break;
//		case 2: print_bin(bin); break;
//		case 3: do_the_task(bin); puts("done!"); system("pause"); break;
//		case 4: fclose(bin); fopen_s(&bin, "binary.bin", "w+"); printf("done!\n"); system("pause"); break;
//		default: return 0;
//		}
//	}
//}

//############################################################################################################
//в этом задании есть один неочевидный вопрос : как расположены слова в файле?
//для своего удобства я сделал каждое слово на отдельное строке
struct tree {
	char* name;
	tree* left, * right;
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

void output_rekurs(tree* drevo) {
	if (!drevo) {
		printf("tree is empty!\n");
		return;
	}
	if (drevo->left != nullptr) {
		output_rekurs(drevo->left);
	}
	else if (drevo->right != nullptr) {
		printf("%s\n", drevo->name);
		output_rekurs(drevo->right);
		return;
	}
	else {
		printf("%s\n", drevo->name);
		return;
	}
	printf("%s\n", drevo->name);
	if (drevo->right != nullptr) {
		output_rekurs(drevo->right);
	}
	return;
}

void append_tree(tree** drevo, char* word) {
	//создаем блок данных
	tree* block;							
	block = (tree*)malloc(sizeof(tree));
	block->name = word;
	block->left = nullptr; block->right = nullptr;

	//обрабатываем исключение
	if (!(*drevo)) {
		(*drevo) = block; 
		printf("word: %s \nresult: correctly located!\n\n", word);
		return;
	}

	//с помощь бегунка ищем место в нашем дереве
	tree* probeg = (*drevo);
	while (1) {
		int side = strcmp(probeg->name, block->name);
		if (side == 0) {
			printf("word: %s \nresult: error, word is already in tree\n\n", word);
			return;
		}
		else if (side == -1) {
			if (probeg->right != nullptr) {
				probeg = probeg->right;
				continue;
			}
			else {
				probeg->right = block;
				printf("word: %s \nresult: correctly located!\n\n", word);
				break;
			}
		}
		else if (side == 1) {
			if (probeg->left != nullptr) {
				probeg = probeg->left;
				continue;
			}
			else {
				probeg->left = block;
				printf("word: %s \nresult: correctly located!\n\n", word);
				break;
			}
		}
	}
}

void do_the_task(tree** drevo,FILE *txt) {
	//итак, в чем замысел - мы читаем запись до \n и отправляем ее в добавлениее дерева
	//в принципе ничего сложного, только что писать муторно

	// читаем данные
	rewind(txt);
	int num = 0; char* word; word = (char*)malloc(2);
	while (!feof(txt)) {
		fscanf_s(txt, "%c", &word[num]);

		//если мы закончили ввод слова, то самое время отправить его в дерево
		if (word[num] == '\n') {
			word[num] = '\0';         //не забываем поставить нуль символ
			append_tree(drevo,word); //добавляем в дерево
			//обновляем все значения
			word = (char*)malloc(2);
			num = 0;
			continue;
		}

		num++;
		word = (char*)realloc(word,num+1);
	}
	return;
}

void add_file(FILE* txt) {
	system("cls");
	printf("enter your note: ");
	rewind(stdin);
	char temp, * word; int num = 0;
	word = (char*)malloc(sizeof(char)*2);
	while (1) {
		scanf_s("%c", &temp, 1);
		word[num] = temp;
		num++;
		word = (char*)realloc(word, (num + 1));
		if (temp == '\n')
			break;
	}

	fseek(txt, 0, 2);
	for (int i = 0; i < num; i++)
		fprintf(txt, "%c", word[i]);

	int more = 0;
	printf("\n done! do you want to add more?  1 - yes: ");
	rewind(stdin);
	checker_num(more);
	if (more == 1)
		add_file(txt);
	return;
}

int main() {
	tree* drevo = nullptr;
	FILE* txt;
	if (fopen_s(&txt, "tree.txt", "r+"))
		fopen_s(&txt, "tree.txt", "w+");
	SetConsoleCP(1251);          //change console language
	SetConsoleOutputCP(1251);   //change output language
	int option;
	while (1) {
		system("cls");
		printf("1. Do tha task\n2. Add something to file\n3. Regenerate file\n4. Output tree\n>");
		rewind(stdin);
		checker_num(option);
		switch (option) {
		case 1:  do_the_task(&drevo, txt); system("pause"); break;
		case 2:  add_file(txt); break;
		case 3:  fclose(txt); fopen_s(&txt, "tree.txt", "w+"); puts("done!"); system("pause"); break;
		case 4:  output_rekurs(drevo); system("pause"); break;
		default: return 0;
		}
	}
}

//############################################################################################################