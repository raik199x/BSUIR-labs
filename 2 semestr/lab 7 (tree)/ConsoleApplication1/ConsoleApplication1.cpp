#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <Windows.h>

struct tree {
	char eng[21];
	char rus[21];
	tree* left, * right;
};

struct stack {
	tree* current;
	stack* next;
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
//are you even asking?
void menu() {
	system("cls");
	printf("What do you want to do?\n");
	printf("1. Create\\Add to tree\n");
	printf("2. Output as:\n");
	printf("3. Delete point\n");
	printf("4. Cut the tree\n");
	printf("5. Blance\n");
	printf("another. Exit program\n");
	printf(">");
}
//works correctly (didnt teste on bugs)
int enter(char word[]) {
	char temp; int num = 0;
	while (1) {
		scanf_s("%c", &temp, 1);
		if (num == 21) {
			printf("limit reached, returning\n");
			system("pause");
			return 1;
		}
		if (temp == '\n') {
			word[num] = '\0';
			num = 0;
			break;
		}
		word[num] = temp;
		num++;
	}
	return 0;
}
//works correctly
void append_tree(tree** drevo) {
	system("cls");
	tree *block;
	block = (tree*)malloc(sizeof(tree));
	rewind(stdin);
	printf("enter english word (up to 20 symbols, if you want to go back type \"exit\")\n> ");
	int num = 0; char temp;
	while (1) {
		scanf_s("%c", &temp,1);
		if (temp == '\n') {
			block->eng[num] = '\0';
			if (strcmp(block->eng, "exit") == 0)
				return;
			num = 0;
			break;
		}
		if (num == 21) {
			printf("limit reached, returning\n");
			system("pause");
			return;
		}
		block->eng[num] = temp;
		num++;
	}
	num = 0;
	printf("\nenter translate (rus)\n> ");
	while (1) {
		scanf_s("%c", &temp, 1);
		if (temp == '\n') {
			block->rus[num] = '\0';
			num = 0;
			break;
		}
		if (num == 21) {
			printf("limit reached, returning\n");
			system("pause");
			return;
		}
		block->rus[num] = temp;
		num++;
	}
	block->left = nullptr; block->right = nullptr;
	if (!(*drevo)) {
		(*drevo) = block;
		printf("\ndone!\n");
		system("pause");
		return;
	}
	tree* probeg = (*drevo);
	while (1) {
		int side = strcmp(probeg->eng, block->eng);
		if (side == 0) {
			printf("they are equal!\n");
			system("pause");
			return;
		}
		else if (side == -1) {
			if (probeg->right != nullptr) {
				probeg = probeg->right;
				continue;
			}
			else {
				probeg->right = block;
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
				break;
			}
		}
	}
	printf("\ndone!\n");
	system("pause");
}
//works correctly
void output_rekurs(tree* drevo) {
	if (!drevo) {
		printf("tree is empty!\n");
		return;
	}
	if (drevo->left != nullptr) {
		output_rekurs(drevo->left);
	}
	else if(drevo->right != nullptr) {
		printf("%20s  ---  %s\n", drevo->eng, drevo->rus);
		output_rekurs(drevo->right);
		return;
	} else {
		printf("%20s  ---  %s\n", drevo->eng,drevo->rus);
		return;
	}
	printf("%20s  ---  %s\n", drevo->eng, drevo->rus);
	if (drevo->right != nullptr) {
		output_rekurs(drevo->right);
	}
	return;
}

void push(stack** collect, tree* current) {
	if ((*collect)->current == nullptr) {
		(*collect)->current = current;
		(*collect)->next = nullptr;
		return;
	}
	stack* block;
	block = (stack*)malloc(sizeof(stack));
	block->current = current;
	block->next = (*collect);
	(*collect) = block;
}

void pop(stack** collect) {
	printf("%21s --- %s\n", (*collect)->current->eng,(*collect)->current->rus);
	stack* block = (*collect)->next;
	free((*collect));
	(*collect) = block;
}

void output_not_rekurs(tree *drevo) {
	if (!drevo) {
		printf("empty!");
		return;
	}
	bool let_me_leave = false;
	system("cls");
	tree* current = drevo;                                //set the top
	stack* collect;                                      //create stack to work with
	collect = (stack*)malloc(sizeof(stack));            //memory for stack
	collect->current = nullptr;                        //actually i could place it on top alredy (because it MUST exist) but let it be
	while (collect || let_me_leave == false) {       //working with tree

		if (!collect && let_me_leave == false) {
			collect = (stack*)malloc(sizeof(stack));
			collect->current = nullptr;
			let_me_leave = true;
		}

		while(1) {
			push(&collect, current);          //place leave in a stack
			if (current->left != nullptr)
				current = current->left;
			else
				break;
		}
		while (collect->current->right == nullptr || collect) {
			if (collect->current->right != nullptr) {
				current = collect->current->right;
				pop(&collect);
				let_me_leave = false;
				break;
			}
			pop(&collect);
			if (!collect) {
				let_me_leave = true;
				break;
			}
			let_me_leave = true;
		}
	}
	return; 
}
//works correclty
void find_removeable(tree *drevo,tree** past, tree** removeable, char deleter[]) {
	int way;
	while ((way = strcmp(drevo->eng, deleter)) != 0) {
		(*past) = drevo;
		if (way == 1) {
			if (drevo->left != nullptr) {
				drevo = drevo->left;
				continue;
			}
			return;
		}
		if (way == -1) {
			if (drevo->right != nullptr) {
				drevo = drevo->right;
				continue;
			}
			return;
		}
	}
	*(removeable) = drevo;
	return;
}

void delete_smth(tree** drevo) {
	if (!*(drevo))
		return;
	system("cls");
	printf("enter word to delete: ");
	rewind(stdin);
	char to_delete[21];
	int option = enter(to_delete);
	if (option == 1)
		return;
	tree* past = nullptr, * removeable = nullptr, * future = nullptr;
	find_removeable(*(drevo), &past, &removeable, to_delete);
	if (removeable == nullptr) {
		printf("no such word!\n");
		return;
	}
	//0. if removeable item is top of tree
	tree* new_top; int replace = 0; new_top = (tree*)malloc(sizeof(tree)); new_top->left = nullptr; new_top->right = nullptr; //only if we neede;
	if (past == nullptr && removeable->left == nullptr && removeable->right == nullptr) {
		free(removeable);
		(*drevo) = nullptr;
		printf("done!\n");
		return;
	}
	else if (past == nullptr){
		new_top->left = removeable;
		new_top->right = nullptr;
		past = new_top;
		replace = 1;
	}
	//1. if removeable item dont have any connections
	if (removeable->left == nullptr && removeable->right == nullptr) {
		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
			past->left = nullptr;
		else
			past->right = nullptr;
	} // 2.1 if removeable item has 1 connection and its on right side
	else if (removeable->right != nullptr && removeable->left == nullptr) {
		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
			past->left = removeable->right;
		else
			past->right = removeable->right;
	} //2.2 if removeable item has 1 connection and its on left side
	else if (removeable->right == nullptr && removeable->left != nullptr) {
		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
			past->left = removeable->left;
		else
			past->right = removeable->left;
	} //3. 2 connections
	else {
		future = removeable->left;
		tree* do_not_loose = future;
		while (future->right != nullptr) {
			do_not_loose = future;
			future = future->right;
		}
		if (future->left != nullptr && do_not_loose != future) {
			do_not_loose->right = future->left;
		}
		else if (future->left != nullptr && do_not_loose == future) {
			future->right = removeable->right;
			if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
				past->left = future;
			else
				past->right = future;
			if (replace == 1)
				(*drevo) = new_top->left;
			free(new_top);
			free(removeable);
			printf("done!\n");
			return;
		}
		else if (do_not_loose == future) {
			future->right = removeable->right;
			if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
				past->left = future;
			else
				past->right = future;
			if (replace == 1)
				(*drevo) = new_top->left;
			free(new_top);
			free(removeable);
			printf("done!\n");
			return;
		}
		else{
			do_not_loose->right = nullptr;
		}
		future->left = removeable->left;
		future->right = removeable->right;
		if (past->left != nullptr && strcmp(removeable->eng, past->left->eng) == 0)
			past->left = future;
		else
			past->right = future;
	}
	if (replace == 1)
		(*drevo) = new_top->left;
	free(new_top);
	free(removeable);
	printf("done!\n");
	return;
}

int cut_tree(tree** drevo) {
	if (!(*drevo)) {
		return 0;
	}
	if ((*drevo)->left != nullptr) {
		cut_tree(&((*drevo)->left));
	}
	else if ((*drevo)->right != nullptr) {
		cut_tree(&((*drevo)->right));
		free((*drevo));
		return 1;
	}
	else {
		free((*drevo));
		return 1;
	}
	if ((*drevo)->right != nullptr) {
		cut_tree(&((*drevo)->right));
	}
	free((*drevo));
	return 1;
}
//start test

void remember_depth(int& depth, int& max_depth) {
	if (depth > max_depth)
		max_depth = depth;
	depth--;
}

int catch_depth(tree* drevo, int forget) {
	static int depth = 0, max_depth;
	if (forget == 1) {
		max_depth = 0;
		depth = 0;
		return 0;
	}
	else if (!drevo) {
		return 0;
	}
	depth++;
	if (drevo->left != nullptr) {
		catch_depth(drevo->left, 0);
	}
	else if (drevo->right != nullptr) {
		catch_depth(drevo->right, 0);
		remember_depth(depth, max_depth);
		return max_depth;
	}
	else {
		remember_depth(depth, max_depth);
		return max_depth;
	}
	if (drevo->right != nullptr) {
		catch_depth(drevo->right, 0);
	}
	remember_depth(depth, max_depth);
	return max_depth;
}

void leave_add(tree* drevo, char*** catch_tree, int depth) {
	char temp = '1'; int counter = 0;
	char **backup = *(catch_tree);
	while (temp != '\0') {
		temp = backup[depth][counter];
		counter++;
	}
	counter--;
	backup[depth] = (char*)realloc(backup[depth], (counter + 2) * sizeof(char));
	backup[depth][counter] = ' ';
	if(counter != 0)
		counter++;
	temp = '1';
	int i = 0;
	while (temp != '\0') {
		temp = drevo->eng[i];
		i++;
		backup[depth][counter] = temp;
		counter++;
		backup[depth] = (char*)realloc(backup[depth], (counter + 2) * sizeof(char));
	}
	*(catch_tree) = backup;
}

void catch_leaves(tree* drevo, char*** catch_tree) {
	static int depth = 0;
	depth++;
	if (drevo->left != nullptr) {
		catch_leaves(drevo->left, catch_tree);
	}
	else if (drevo->right != nullptr) {
		catch_leaves(drevo->right, catch_tree);
		leave_add(drevo, catch_tree, depth);
		depth--;
		return;
	}
	else {
		leave_add(drevo, catch_tree, depth);
		depth--;
		return;
	}
	leave_add(drevo, catch_tree, depth);
	if (drevo->right != nullptr) {
		catch_leaves(drevo->right, catch_tree);
	}
	depth--;
	return;
}

void output_as_tree(tree *drevo) { //works correctly
	system("cls");
	if (!drevo) {
		printf("tree is empty!\n");
		return;
	}
	int depth = catch_depth(drevo, 0);
	depth++;
	char** catch_tree;
	catch_tree = (char**)malloc(depth * sizeof(char*));
	for (int i = 0; i < depth; i++) {
		catch_tree[i] = (char*)malloc(2);
		catch_tree[i][0] = '\0';
	}
	catch_leaves(drevo, &catch_tree);
	for (int i = 0; i < depth; i++)
		printf("%s\n", catch_tree[i]);
	for (int i = 0; i < depth; i++)
		free(catch_tree[i]);
	free(catch_tree);
	catch_depth(drevo, 1);
}
//end test

void spin_left(tree** drevo) {
	tree* replace1 = (*drevo), * replace2 = (*drevo)->right;
	replace1->right = replace2->left;
	replace2->left = replace1;
	(*drevo) = replace2;
}

void spin_right(tree** drevo) {
	tree* replace1 = (*drevo), * replace2 = (*drevo)->left;
	replace1->left = replace2->right;
	replace2->right = replace1;
	(*drevo) = replace2;
}

void Balance(tree** drevo) {
	if (!(*drevo)) {
		printf("tree is empty!\n");
		return;
	}
	while (1) {
		int left = catch_depth((*drevo)->left, 0);
		catch_depth((*drevo), 1);
		int right = catch_depth((*drevo)->right, 0);
		catch_depth((*drevo), 1);
		int left2, right2;
		if (left - right > 1) {
			//ворочаем в право
			left2 = catch_depth((*drevo)->left->right, 0);
			catch_depth((*drevo), 1);
			right2 = catch_depth((*drevo)->left->left, 0);
			catch_depth((*drevo), 1);
			if (left2 > right2) {
				spin_left(&(*drevo)->left);
				spin_right(drevo);
			}
			else {
				spin_right(drevo);
			}
		}
		else if (left - right < -1) {
			//ворочаем в лево
			left2 = catch_depth((*drevo)->right->left, 0);
			catch_depth((*drevo), 1);
			right2 = catch_depth((*drevo)->right->right, 0);
			catch_depth((*drevo), 1);
			if (left2 > right2) {
				spin_right(&(*drevo)->right);
				spin_left(drevo);
			}
			else {
				spin_left(drevo);
			}
		}
		else{
			printf("balanced!\n");
			return;
		}
	}
}

int main() {
	tree *drevo;
	SetConsoleCP(1251);          //change console language
	SetConsoleOutputCP(1251);   //change output language
	drevo = nullptr;
	int option;
	while (1) {
		rewind(stdin);
		menu();
		checker_num(option);
		switch (option){
		case 1:  append_tree(&drevo); break;
		case 2:  system("cls");
			     printf("1.Rekursiv\n2.Stack\n3.Tree levels\n> ");
				 checker_num(option);
				 switch (option) {
				 case 1: system("cls"); output_rekurs(drevo); system("pause"); break;
				 case 2: output_not_rekurs(drevo); system("pause"); break;
				 case 3: output_as_tree(drevo); system("pause");
				 default:break;
				 } break;
		case 3:  delete_smth(&drevo); system("pause"); break;
		case 4:  option = cut_tree(&drevo);
			     if (option == 0) { printf("tree is empty!\n"); }
				 if (option == 1) { printf("dont tell green about it...\n"); drevo = nullptr;}
				 system("pause");
				 break;
		case 5:  Balance(&drevo); system("pause"); break;
		default: cut_tree(&drevo); return 0;
		}
	}
}