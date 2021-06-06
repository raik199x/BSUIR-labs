//bilet 16
//made by AlexanderXD
//team sessia na 8+

#include <stdio.h>
#include <Windows.h>

//############################################################################################################
//В 1 бит мы можем записать 1 или 0,
//и это все возможные варианты остатков при деление на 2
//а в два бита мы можем записать 0 1 2 3 и это уже все возможные варианты остатка от деления на 4
struct pole {
	unsigned int four : 2;
	unsigned int two : 1;
	int num;
};

int main() {
	printf("%d", sizeof(pole));
	pole *something;
	something = (pole*)malloc(sizeof(pole));
	scanf_s("%d", &something->num);
	//присваиваются только первые биты
	something->four = something->num;
	something->two = something->num;
	printf("four: %d\ntwo: %d\n", something->four, something->two);
}

//############################################################################################################
////для воссоздания ситуация я просто спиздил свою же лабу, конечно укоротив ее
//#include <stdio.h>
//#include <malloc.h>
//#include <string.h>
//#include <Windows.h>
//
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
//	printf("3. Delete point\n");
//	printf("4. Do the task\n");
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
//	block->left = nullptr; block->right = nullptr;
//	if (!(*drevo)) {
//		(*drevo) = block;
//		printf("\ndone!\n");
//		system("pause");
//		return;
//	}
//	tree* probeg = (*drevo);
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
//		printf("%s\n", drevo->eng);
//		output_rekurs(drevo->right);
//		return;
//	}
//	else {
//		printf("%s\n", drevo->eng);
//		return;
//	}
//	printf("%s\n", drevo->eng);
//	if (drevo->right != nullptr) {
//		output_rekurs(drevo->right);
//	}
//	return;
//}
//
//
////works correclty
//void find_removeable(tree* drevo, tree** past, tree** removeable, char deleter[]) {
//	int way;
//	while ((way = strcmp(drevo->eng, deleter)) != 0) {
//		(*past) = drevo;
//		if (way == 1) {
//			if (drevo->left != nullptr) {
//				drevo = drevo->left;
//				continue;
//			}
//			return;
//		}
//		if (way == -1) {
//			if (drevo->right != nullptr) {
//				drevo = drevo->right;
//				continue;
//			}
//			return;
//		}
//	}
//	*(removeable) = drevo;
//	return;
//}
//
//void delete_smth(tree** drevo) {
//	if (!*(drevo))
//		return;
//	system("cls");
//	printf("enter word to delete: ");
//	rewind(stdin);
//	char to_delete[21];
//	int option = enter(to_delete);
//	if (option == 1)
//		return;
//	tree* past = nullptr, * removeable = nullptr, * future = nullptr;
//	find_removeable(*(drevo), &past, &removeable, to_delete);
//	if (removeable == nullptr) {
//		printf("no such word!\n");
//		return;
//	}
//	//0. if removeable item is top of tree
//	tree* new_top; int replace = 0; new_top = (tree*)malloc(sizeof(tree)); new_top->left = nullptr; new_top->right = nullptr; //only if we neede;
//	if (past == nullptr && removeable->left == nullptr && removeable->right == nullptr) {
//		free(removeable);
//		(*drevo) = nullptr;
//		printf("done!\n");
//		return;
//	}
//	else if (past == nullptr) {
//		new_top->left = removeable;
//		new_top->right = nullptr;
//		past = new_top;
//		replace = 1;
//	}
//	//1. if removeable item dont have any connections
//	if (removeable->left == nullptr && removeable->right == nullptr) {
//		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
//			past->left = nullptr;
//		else
//			past->right = nullptr;
//	} // 2.1 if removeable item has 1 connection and its on right side
//	else if (removeable->right != nullptr && removeable->left == nullptr) {
//		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
//			past->left = removeable->right;
//		else
//			past->right = removeable->right;
//	} //2.2 if removeable item has 1 connection and its on left side
//	else if (removeable->right == nullptr && removeable->left != nullptr) {
//		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
//			past->left = removeable->left;
//		else
//			past->right = removeable->left;
//	} //3. 2 connections
//	else {
//		future = removeable->left;
//		tree* do_not_loose = future;
//		while (future->right != nullptr) {
//			do_not_loose = future;
//			future = future->right;
//		}
//		if (future->left != nullptr && do_not_loose != future) {
//			do_not_loose->right = future->left;
//		}
//		else if (future->left != nullptr && do_not_loose == future) {
//			future->right = removeable->right;
//			if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
//				past->left = future;
//			else
//				past->right = future;
//			if (replace == 1)
//				(*drevo) = new_top->left;
//			free(new_top);
//			free(removeable);
//			printf("done!\n");
//			return;
//		}
//		else if (do_not_loose == future) {
//			future->right = removeable->right;
//			if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
//				past->left = future;
//			else
//				past->right = future;
//			if (replace == 1)
//				(*drevo) = new_top->left;
//			free(new_top);
//			free(removeable);
//			printf("done!\n");
//			return;
//		}
//		else {
//			do_not_loose->right = nullptr;
//		}
//		future->left = removeable->left;
//		future->right = removeable->right;
//		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
//			past->left = future;
//		else
//			past->right = future;
//	}
//	if (replace == 1)
//		(*drevo) = new_top->left;
//	free(new_top);
//	free(removeable);
//	printf("done!\n");
//	return;
//}
//
//void putting_file(tree* drevo, FILE* txt) {
//	int num = 0;
//	while (drevo->eng[num] != '\0') {
//		fprintf(txt, "%c", drevo->eng[num]);
//		num++;
//	}
//	fprintf(txt, "\n");
//}
//
//void write_file(tree* drevo, FILE* txt) {
//	if (!drevo) {
//		printf("tree is empty!\n");
//		return;
//	}
//	if (drevo->left != nullptr) {
//		write_file(drevo->left, txt);
//	}
//	else if (drevo->right != nullptr) {
//		putting_file(drevo, txt);
//		write_file(drevo->right, txt);
//		return;
//	}
//	else {
//		putting_file(drevo, txt);
//		return;
//	}
//	putting_file(drevo, txt);
//	if (drevo->right != nullptr) {
//		write_file(drevo->right, txt);
//	}
//	return;
//}
//
//void do_the_task(tree* drevo) {
//	if (!drevo)
//		return;
//	//сама такска, какая сложность? 
//   // да пиздец сложность - надо ввести свое название файла (ну допустим похуй)
//  // надо записать дерево (хуевые приколы рили) но да ландно, решим проблему с файлом
//	FILE* txt;
//	char* file_name; int num = 0;
//	file_name = (char*)malloc(2);
//	rewind(stdin);
//	printf("enter file name: ");
//	while (1) {
//		scanf_s("%c", &file_name[num]);
//		if (file_name[num] == '\n') {
//			file_name[num] = '\0';
//			break;
//		}
//		num++;
//		file_name = (char*)realloc(file_name, num + 1);
//	}
//	fopen_s(&txt, file_name, "w+"); //опа а это наш файл UPD имеет смысл запилить проверку
//	//казалось бы, половина дела сделана, ну в принципе так и есть, мы берем функцию вывода, и все принты заменям на запись в файле
//	rewind(txt);
//	write_file(drevo, txt);
//}
//
//int main() {
//	tree* drevo = nullptr;
//	int option;
//	while (1) {
//		rewind(stdin);
//		menu();
//		checker_num(option);
//		switch (option) {
//		case 1:  append_tree(&drevo); break;
//		case 2:  output_rekurs(drevo); system("pause"); break;
//		case 3:  delete_smth(&drevo); system("pause"); break;
//		case 4:  do_the_task(drevo); break;
//		default: return 0;
//		}
//	}
//}

//############################################################################################################