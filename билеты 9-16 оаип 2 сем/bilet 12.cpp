#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <Windows.h>

void add_bin(FILE* bin) {
	system("cls");
	printf("entering number\nprintf letter to stop\n");
	int temp, number[1];
	fseek(bin, 0, 2);
	rewind(stdin);
	while (1) {
		temp = scanf_s("%d", &number[0]);
		if (!temp) {
			rewind(stdin);
			break;
		}
		fwrite(number, sizeof(int), 1, bin);
	}
	printf("entreing is done\n");
	system("pause");
}

void print_bin(FILE* bin) {
	system("cls");
	int j;
	rewind(bin);
	do {
		fread(&j, sizeof(int), 1, bin);
		if (feof(bin))  break;
		printf("%d ", j);
	} while (1);
	puts("");
	system("pause");
}

void add_txt(FILE* txt) {
	system("cls");
	printf("entering number\nprintf letter to stop\n");
	fseek(txt, 0, 2);
	int temp, number;
	while (1) {
		temp = scanf_s("%d", &number);
		if (!temp) {
			rewind(stdin);
			break;
		}
		fprintf(txt, "%3d ", number);
	}
	puts("entering done!");
	system("pause");
}

void print_txt(FILE *txt) {
	rewind(txt);
	system("cls");
	int number = 1000;
	while (!feof(txt)) {
		fscanf_s(txt, "%d", &number);
		if (feof(txt)) {
			rewind(stdin);
			break;
		}
		printf("%d ", number);
	}
	puts("");
	system("pause");
}

void do_the_task(FILE *txt, FILE *bin) {
	int binary_number, * future_numbers, num = 0;
	rewind(bin); rewind(txt);
	future_numbers = (int*)malloc(2 * sizeof(int));
	//сори, переделываю билет (потому что изначальную версию проебал) поэтому комментариев будет меньше
	//т.к. у нас нет никаких запретов, то я просто возьму числа их текстового файла в массив, и потом удалю из них всё лишнее
	//и пересоздам файл, вот так
	while (!feof(txt)) {
		fscanf_s(txt, "%d", &future_numbers[num]);
		if (feof(txt))
			break;
		num++;
		future_numbers = (int*)realloc(future_numbers, (num+1) * sizeof(int));
 	}
	num--;
	while (!feof(bin)) {
		fread(&binary_number, 1, sizeof(int), bin);
		if (feof(bin))
			break;
		for (int i = 0; i < num+1; i++) {
			if (future_numbers[i] == binary_number)
				future_numbers[i] = -1;
		}
	}
	fclose(txt);
	fopen_s(&txt, "text.txt", "w+");
	rewind(txt);
	for (int i = 0; i < num+1; i++) {
		if (future_numbers[i] == -1)
			continue;
		fprintf(txt, "%3d ", future_numbers[i]);
	}
	free(future_numbers);
	return;
}

int main() {
	int option, proverka;
	FILE* bin, * txt;
	if (fopen_s(&bin, "binary.bin", "r+"))
		fopen_s(&bin, "binary.bin", "w+");
	if (fopen_s(&txt, "text.txt", "r+"))
		fopen_s(&txt, "text.txt", "w+");
	while (1) {
		system("cls");
		printf("1. Add to bin\n2. Add to txt\n3. output bin\n4. output txt\n5. do the task\n6. Regenerate files\n>");
		proverka = scanf_s("%d", &option);
		while (!proverka)
			proverka = scanf_s("%d", &option);
		switch (option){
		case 1: add_bin(bin); break;
		case 2: add_txt(txt); break;
		case 3: print_bin(bin); break;
		case 4: print_txt(txt); break;
		case 5: do_the_task(txt, bin); break;
		case 6: fclose(bin); fclose(txt); fopen_s(&bin, "binary.bin", "w+"); fopen_s(&txt, "text.txt", "w+"); printf("done!\n"); system("pause"); break;
		default: return 0;
		}
	}
}

//########################################################################################################################

//struct tree {
//	char eng[21];
//	tree* left, * right;
//};
//
//void checker_num(int& n) {  //checks correcting written number
//	int temp = scanf_s("%d", &n);
//	while (temp != 1 || n <= 0 || n > 9999999) {
//		if (temp != 1) {
//			printf("detected letters, do not use them\n");
//		}
//		else if (n == 0) {
//			printf("0 pointer\n");
//		}
//		else if (n < 0) {
//			printf("detected negative number, unacceptable\n");
//		}
//		else if (n > 999999) {
//			printf("we are sorry! our programm dont work with number higher than 999999\n");
//		}
//		rewind(stdin);
//		temp = scanf_s("%d", &n);
//	}
//}
////are you even asking?
//void menu() {
//	system("cls");
//	printf("What do you want to do?\n");
//	printf("1. Create\\Add to tree\n");
//	printf("2. Output\n");
//	printf("another. Exit program\n");
//	printf(">");
//}
////works correctly (didnt teste on bugs)
//int enter(char word[]) {
//	char temp; int num = 0;
//	while (1) {
//		scanf_s("%c", &temp, 1);
//		if (num == 21) {
//			printf("limit reached, returning\n");
//			system("pause");
//			return 1;
//		}
//		if (temp == '\n') {
//			word[num] = '\0';
//			num = 0;
//			break;
//		}
//		word[num] = temp;
//		num++;
//	}
//	return 0;
//}
////works correctly
//void append_tree(tree** drevo) {
//	system("cls");
//	tree* block;
//	block = (tree*)malloc(sizeof(tree));
//	rewind(stdin);
//	printf("enter english word (up to 20 symbols, if you want to go back type \"exit\")\n> ");
//	int num = 0; char temp;
//	while (1) {
//		scanf_s("%c", &temp, 1);
//		if (temp == '\n') {
//			block->eng[num] = '\0';
//			if (strcmp(block->eng, "exit") == 0)
//				return;
//			num = 0;
//			break;
//		}
//		if (num == 21) {
//			printf("limit reached, returning\n");
//			system("pause");
//			return;
//		}
//		block->eng[num] = temp;
//		num++;
//	}
//	num = 0;
//	tree* probeg = (*drevo);
//	block->left = nullptr; block->right = nullptr;
//	if (probeg == nullptr) {
//		(*drevo) = block;
//		return;
//	}
//	while (1) {
//		int side = strcmp(probeg->eng, block->eng);
//		if (side == 0) {
//			printf("they are equal!\n");
//			system("pause");
//			return;
//		}
//		else if (side == -1) {
//			if (probeg->right != nullptr) {
//				probeg = probeg->right;
//				continue;
//			}
//			else {
//				probeg->right = block;
//				break;
//			}
//		}
//		else if (side == 1) {
//			if (probeg->left != nullptr) {
//				probeg = probeg->left;
//				continue;
//			}
//			else {
//				probeg->left = block;
//				break;
//			}
//		}
//	}
//	printf("\ndone!\n");
//	system("pause");
//}
////works correctly
//void output_rekurs(tree* drevo) {
//	if (!drevo) {
//		printf("tree is empty!\n");
//		return;
//	}
//	if (drevo->left != nullptr) {
//		output_rekurs(drevo->left);
//	}
//	else if (drevo->right != nullptr) {
//		printf("%20s\n", drevo->eng);
//		output_rekurs(drevo->right);
//		return;
//	}
//	else {
//		printf("%20s\n", drevo->eng);
//		return;
//	}
//	printf("%20s\n", drevo->eng);
//	if (drevo->right != nullptr) {
//		output_rekurs(drevo->right);
//	}
//	return;
//}
//
//int main() {
//	tree* drevo;
//	SetConsoleCP(1251);          //change console language
//	SetConsoleOutputCP(1251);   //change output language
//	drevo = nullptr;
//	int option;
//	while (1) {
//		rewind(stdin);
//		menu();
//		checker_num(option);
//		switch (option) {
//		case 1:  append_tree(&drevo); break;
//		case 2:  output_rekurs(drevo); system("pause"); break;
//		default: return 0;
//		}
//	}
//}