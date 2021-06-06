//bilet 15
//made by AlexanderXD
//team sessia na 8+

#include <stdio.h>
#include <Windows.h>

//############################################################################################################

//void add_txt(FILE* txt) {
//	system("cls");
//	printf("entering number\nprintf letter to stop\n");
//	fseek(txt, 0, 2);
//	int temp, number;
//	while (1) {
//		temp = scanf_s("%d", &number);
//		if (number < 10 || number > 99)
//			continue;
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
//		printf("%2d ", number);
//	}
//	puts("");
//	system("pause");
//}
//
//void do_the_task(FILE* txt) {
//	//здесь есть огромный плюс в том, что все числа двузначные, поэтому сортировать будет очень легко
//	fpos_t left = 0, right; //позиции
//
//	//ищем позицию конца
//	fseek(txt, 0, 2);   
//	fgetpos(txt,&right);
//	right -= 3;  //отнимаем т.к. мы получаем позицию feof, а нам нужна позиция числа (3 потому что два числа + пробел)
//
//	//обрабатываем исключение
//	if (left == right && left == 0) {
//		printf("file is empty!\n");
//		return;
//	}
//
//	//сама *сортировка*
//	rewind(txt);
//	int left_num, right_num;
//	while (left != right) {
//		fsetpos(txt, &left);
//		fscanf_s(txt, "%d", &left_num);
//		fsetpos(txt, &right);
//		fscanf_s(txt, "%d", &right_num);
//		fsetpos(txt, &right);
//		fprintf(txt,"%2d", left_num);
//		fsetpos(txt, &left);
//		fprintf(txt, "%2d", right_num);
//		left += 3;
//		right -= 3;
//		if (left > right) //если мы видим такую штуку, то ясное дело надо заканчивать
//			break;
//	}
//	return;
//}
//
//int main() {
//	int option, proverka;
//	FILE* txt;
//	if (fopen_s(&txt, "text.txt", "r+"))
//		fopen_s(&txt, "text.txt", "w+");
//	while (1) {
//		system("cls");
//		printf("1. Add to text\n2. Print txt\n3. Do the task\n4. Regenerate txt\n>");
//		proverka = scanf_s("%d", &option);
//		while (!proverka)
//			proverka = scanf_s("%d", &option);
//		switch (option){
//		case 1: add_txt(txt); break;
//		case 2: print_txt(txt); break;
//		case 3: do_the_task(txt); break;
//		case 4: fclose(txt); fopen_s(&txt, "text.txt", "w+"); break;
//		default: return 0;
//		}
//	}
//}

//############################################################################################################

struct tree {
	char* last_name;
	int year;
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
		printf("name: %s \nyear: %d\n", drevo->last_name, drevo->year);
		output_rekurs(drevo->right);
		return;
	}
	else {
		printf("name: %s \nyear: %d\n", drevo->last_name, drevo->year);
		return;
	}
	printf("name: %s \nyear: %d\n", drevo->last_name, drevo->year);
	if (drevo->right != nullptr) {
		output_rekurs(drevo->right);
	}
	return;
}

void append_tree(tree** probeg, tree* block){
	//та сама функция, на самом деле нечего сказать, просто цикл был заменен рекурсией, всё абсолютно тоже самое
	int side = strcmp((*probeg)->last_name, block->last_name);
	if (side == 0) {
		printf("they are equal!\n");
		system("pause");
		return;
	}
	else if (side == -1) {
		if ((*probeg)->right != nullptr) {
			(*probeg) = (*probeg)->right;
			append_tree(probeg, block);
		}
		else {
			(*probeg)->right = block;
			return;
		}
	}
	else if (side == 1) {
		if ((*probeg)->left != nullptr) {
			(*probeg) = (*probeg)->left;
			append_tree(probeg, block);
		}
		else {
			(*probeg)->left = block;
			return;
		}
	}
}

void do_the_task(tree** drevo) {
	//создаем блок информации
	tree* block;
	block = (tree*)malloc(sizeof(tree));
	block->left = nullptr; block->right = nullptr;
	int year;
	printf("enter year: ");
	checker_num(year);
	block->year = year;
	char* second_name; int num = 0;
	second_name = (char*)malloc(2);
	rewind(stdin);
	printf("\nenter last name: ");
	while (1) {
		scanf_s("%c", &second_name[num]);
		if (second_name[num] == '\n') {
			second_name[num] = '\0';
			break;
		}
		num++;
		second_name = (char*)realloc(second_name, num + 1);
	}
	block->last_name = second_name;
	//теперь рекурсивное добавление, вставлять будем по фамилии
	//обработаем исключение
	if (!(*drevo)) {
		(*drevo) = block;
		return;
	}
	tree* probeg = (*drevo);
	append_tree(&probeg, block);
}

int main() {
	tree* drevo = nullptr;
	int option;
	while (1) {
		system("cls");
		rewind(stdin);
		printf("1. do_the_task\n2. Output\n>");
		checker_num(option);
		switch (option) {
		case 1:  do_the_task(&drevo); break;
		case 2:  output_rekurs(drevo); puts(""); system("pause"); break;
		default: return 0;
		}
	}
}

//############################################################################################################