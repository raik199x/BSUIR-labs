#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <windows.h>

struct office {
	char *second_name;
	char *first_name;
	char *otchestvo;
	int number_score;
	int summa;
	int last_change;
};
void checker_num(int &n) {
	int temp = scanf_s("%d", &n);
	while (temp != 1 || n <= 0) {
		printf("wrong amount, read again\n");
		rewind(stdin);
		temp = scanf_s("%d", &n);
	}
}
void checker_num_peacefull(int &n) {
	int temp = scanf_s("%d", &n);
	while (temp != 1 || n < 0) {
		printf("wrong amount, read again\n");
		rewind(stdin);
		temp = scanf_s("%d", &n);
	}
}
void entering_second_name(office *num, int j, int data_amount) {
	char temp;
	int i = 0, n = 1;
	(num + j + data_amount)->second_name = (char*)malloc(1 * sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			(num + j + data_amount)->second_name[i] = '\0';
			break;
		}
		(num + j + data_amount)->second_name[i] = temp;
		i++; n++;
		(num + j + data_amount)->second_name = (char*)realloc((num + j + data_amount)->second_name, n * sizeof(char));
	}
}
void entering_first_name(office *num, int j, int data_amount) {
	char temp;
	int i = 0, n = 1;
	(num + j + data_amount)->first_name = (char*)malloc(1 * sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			(num + j + data_amount)->first_name[i] = '\0';
			break;
		}
		(num + j + data_amount)->first_name[i] = temp;
		i++; n++;
		(num + j + data_amount)->first_name = (char*)realloc((num + j + data_amount)->first_name, n * sizeof(char));
	}
}
void entering_otchestvo(office *num, int j, int data_amount) {
	char temp;
	int i = 0, n = 1;
	(num + j + data_amount)->otchestvo = (char*)malloc(1 * sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			(num + j + data_amount)->otchestvo[i] = '\0';
			break;
		}
		(num + j + data_amount)->otchestvo[i] = temp;
		i++; n++;
		(num + j + data_amount)->otchestvo = (char*)realloc((num + j + data_amount)->otchestvo, n * sizeof(char));
	}
}
void menu(int &option) {
	printf("*************************\n");
	printf("*                       *\n");
	printf("* welcome to main menu  *\n");
	printf("*                       *\n");
	printf("*************************\n");
	printf("\n\n");
	printf("7) searching system\n6) show clients databse\n5) add client\n4) delete client\n3) sort system\n2) change information about client\n1) end program\nchoose option: ");
	checker_num(option);
	while (option < 1 || option > 7) {
		system("cls");
		printf("missing function, please check existing variants\n\n");
		printf("7) searching system\n6) show clients databse\n5) add client\n4) delete client\n3) sort system\n2) change information about client\n1) end program\nchoose option: ");
		rewind(stdin);
		scanf_s("%d", &option);
	}
}
void searching_secondName(office *num, int data_amount) {
	char *search, temp;
	printf("what you want to find?\n note: you can use '*' to ignore symbols\n");
	int zi = 0, zn = 1;
	search = (char*)malloc(zn * sizeof(char));
	rewind(stdin);
	while (1) {
		char temp;
		scanf_s("%c", &temp);
		if (temp == '\n') {
			search[zi] = '\0';
			break;
		}
		search[zi] = temp;
		zi++; zn++;
		search = (char*)realloc(search, zn * sizeof(char));
	}
	rewind(stdin);
	system("cls");
	if (search[0] == '\0') {
		system("cls");
		printf("am i joke to you?");
		printf("\npress enter to continue\n");
		scanf_s("%c", &temp);
		rewind(stdin);
		return;
	}
	for (int i = 0; search[i] != '\0'; i++) {
		if (search[i] < 97 && search[i] != '*') {
			search[i] += 32;
		}
	}
	int flag = 0, step = 0, j = 0, stop_me = 0;
	for (int i = 0; i < data_amount; i++) {
		while (stop_me == 0) {
			//���� ��������
			if (search[step] == '*') {
				flag = 1;
				step++;
			}
			//��������� (�������)
			if (search[step] == '\0' && search[step] == (num + i)->second_name[j]) {
				printf("\nclient %d\n second name: %s\n", i + 1, (num + i)->second_name);
				printf("continue searching?\n 0) yes\n any other number) no\n");
				checker_num_peacefull(stop_me);
				if (stop_me != 0) {
					system("cls");
					return;
				}
				break;
			}
			//������� ��������� ����� (���)
			if ((num + i)->second_name[j] == '\0') {
				j = 0;
				step = 0;
				break;
			}
			//��������� �������
			if (search[step] == '\0' && flag == 1) {
				j++;
				continue;
			}
			//������� ������ (���)
			if ((num + i)->second_name[j] != search[step] && (num + i)->second_name[j] + 32 != search[step] && flag == 0) {
				j = 0;
				step = 0;
				break;
			}
			if ((num + i)->second_name[j] == search[step] || (num + i)->second_name[j] + 32 == search[step]) {
				step++;
				flag = 0;
			}
			j++;
		}
		j = 0;
		step = 0;
		if (stop_me != 0 || i == data_amount - 1) {
			system("cls");
			return;
		}
	}
}
void searching_firstName(office *num, int data_amount) {
	char *search, temp;
	printf("what you want to find?\n note: you can use '*' to ignore symbols\n");
	int zi = 0, zn = 1;
	search = (char*)malloc(zn * sizeof(char));
	rewind(stdin);
	while (1) {
		char temp;
		scanf_s("%c", &temp);
		if (temp == '\n') {
			search[zi] = '\0';
			break;
		}
		search[zi] = temp;
		zi++; zn++;
		search = (char*)realloc(search, zn * sizeof(char));
	}
	rewind(stdin);
	system("cls");
	if (search[0] == '\0') {
		system("cls");
		printf("am i joke to you?");
		printf("\npress enter to continue\n");
		scanf_s("%c", &temp);
		rewind(stdin);
		return;
	}
	for (int i = 0; search[i] != '\0'; i++) {
		if (search[i] < 97 && search[i] != '*') {
			search[i] += 32;
		}
	}
	int flag = 0, step = 0, j = 0, stop_me = 0;
	for (int i = 0; i < data_amount; i++) {
		while (stop_me == 0) {
			//���� ��������
			if (search[step] == '*') {
				flag = 1;
				step++;
			}
			//��������� (�������)
			if (search[step] == '\0' && search[step] == (num + i)->first_name[j]) {
				printf("\nclient %d\n first name: %s\n", i + 1, (num + i)->first_name);
				printf("continue searching?\n 0) yes\n any other number) no\n");
				checker_num_peacefull(stop_me);
				if (stop_me != 0) {
					system("cls");
					return;
				}
				break;
			}
			//������� ��������� ����� (���)
			if ((num + i)->first_name[j] == '\0') {
				j = 0;
				step = 0;
				break;
			}
			//��������� �������
			if (search[step] == '\0' && flag == 1) {
				j++;
				continue;
			}
			//������� ������ (���)
			if ((num + i)->first_name[j] != search[step] && (num + i)->first_name[j] + 32 != search[step] && flag == 0) {
				j = 0;
				step = 0;
				break;
			}
			if ((num + i)->first_name[j] == search[step] || (num + i)->first_name[j] + 32 == search[step]) {
				step++;
				flag = 0;
			}
			j++;
		}
		j = 0;
		step = 0;
		if (stop_me != 0 || i == data_amount - 1) {
			system("cls");
			return;
		}
	}
}
void searching_otchestvo(office *num, int data_amount) {
	char *search, temp;
	printf("what you want to find?\n note: you can use '*' to ignore symbols\n");
	int zi = 0, zn = 1;
	search = (char*)malloc(zn * sizeof(char));
	rewind(stdin);
	while (1) {
		char temp;
		scanf_s("%c", &temp);
		if (temp == '\n') {
			search[zi] = '\0';
			break;
		}
		search[zi] = temp;
		zi++;
		zn++;
		search = (char*)realloc(search, zn * sizeof(char));
	}
	rewind(stdin);
	system("cls");
	if (search[0] == '\0') {
		system("cls");
		printf("am i joke to you?");
		printf("\npress enter to continue\n");
		scanf_s("%c", &temp);
		rewind(stdin);
		return;
	}
	for (int i = 0; search[i] != '\0'; i++) {
		if (search[i] < 97 && search[i] != '*') {
			search[i] += 32;
		}
	}
	int flag = 0, step = 0, j = 0, stop_me = 0;
	for (int i = 0; i < data_amount; i++) {
		while (stop_me == 0) {
			//���� ��������
			if (search[step] == '*') {
				flag = 1;
				step++;
			}
			//��������� (�������)
			if (search[step] == '\0' && search[step] == (num + i)->otchestvo[j]) {
				printf("\nclient %d\n otchestvo: %s\n", i + 1, (num + i)->otchestvo);
				printf("continue searching?\n 0) yes\n any other number) no\n");
				checker_num_peacefull(stop_me);
				if (stop_me != 0) {
					system("cls");
					return;
				}
				break;
			}
			//������� ��������� ����� (���)
			if ((num + i)->otchestvo[j] == '\0') {
				j = 0;
				step = 0;
				break;
			}
			//��������� �������
			if (search[step] == '\0' && flag == 1) {
				j++;
				continue;
			}
			//������� ������ (���)
			if ((num + i)->otchestvo[j] != search[step] && (num + i)->otchestvo[j] + 32 != search[step] && flag == 0) {
				j = 0;
				step = 0;
				break;
			}
			if ((num + i)->otchestvo[j] == search[step] || (num + i)->otchestvo[j] + 32 == search[step]) {
				step++;
				flag = 0;
			}
			j++;
		}
		j = 0;
		step = 0;
		if (stop_me != 0 || i == data_amount - 1) {
			system("cls");
			return;
		}
	}
}
void searching_money(office *num, int data_amount) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	for (int i = 0; i < data_amount; i++) {
		if ((num + i)->summa >= counter - depth && (num + i)->summa <= counter + depth) {
			printf("client: %d\nsecond name: %s\n balance: %d\n", i + 1, (num + i)->second_name, (num + i)->summa);
			printf("Do you want to continue?\n0)yes\nanything else)no\n");
			int variant;
			checker_num_peacefull(variant);
			if (variant != 0) {
				system("cls");
				return;
			}
		}
	}
	system("cls");
}
void searching_numberScore(office *num, int data_amount) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	for (int i = 0; i < data_amount; i++) {
		if ((num + i)->number_score >= counter - depth && (num + i)->number_score <= counter + depth) {
			printf("client: %d\nsecond name: %s\n number score: %d\n", i + 1, (num + i)->second_name, (num + i)->number_score);
			printf("Do you want to continue?\n0)yes\nanything else)no\n");
			int variant;
			checker_num_peacefull(variant);
			if (variant != 0) {
				system("cls");
				return;
			}
		}
	}
	system("cls");
}
void searching_lastChange(office *num, int data_amount) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	for (int i = 0; i < data_amount; i++) {
		if ((num + i)->last_change >= counter - depth && (num + i)->last_change <= counter + depth) {
			printf("client: %d\nsecond name: %s\n last change: %d\n", i + 1, (num + i)->second_name, (num + i)->last_change);
			printf("Do you want to continue?\n0)yes\nanything else)no\n");
			int variant;
			checker_num_peacefull(variant);
			if (variant != 0) {
				system("cls");
				return;
			}
		}
	}
	system("cls");
}
void searching_system(office *num, int data_amount) {
	system("cls");
	printf("what do you want to search?\n0)go back\n1)second name\n2)first name\n3)otchestvo\n4)balance\n5)number score\n6)last change\n");
	int option;
	checker_num_peacefull(option);
	switch (option) {
	case 0: system("cls"); return;
	case 1: searching_secondName(num, data_amount); return;
	case 2: searching_firstName(num, data_amount); return;
	case 3: searching_otchestvo(num, data_amount); return;
	case 4: searching_money(num, data_amount); return;
	case 5: searching_numberScore(num, data_amount); return;
	case 6: searching_lastChange(num, data_amount); return;
	}
}
void data_add(office *num, int &data_amount, int n) {
	int temp;
	for (int j = 0; j < n; j++) {
		system("CLS");
		printf("entering %d client\n", j + 1);
		printf("enter second name: ");
		entering_second_name(num, j, data_amount);
		printf("enter first name: ");
		entering_first_name(num, j, data_amount);
		printf("enter otchestvo: ");
		entering_otchestvo(num, j, data_amount);
		printf("enter number_score: ");
		checker_num(temp);
		(num + j + data_amount)->number_score = temp;
		printf("enter balance: ");
		checker_num_peacefull(temp);
		(num + j + data_amount)->summa = temp;
		printf("enter last change: ");
		checker_num(temp);
		(num + j + data_amount)->last_change = temp;
	}
	data_amount += n;
	system("cls");
}
void show_database(office *num, int data_amount) {
	rewind(stdin);
	system("cls");
	if (data_amount == 0) {
		printf("at that moment you don't have any clients =(\npress enter to conitinue");
		char press = '1';
		while (press != '\n') {
			scanf_s("%c", &press);
		}
		system("cls");
		return;
	}
	for (int j = 0; j < data_amount; j++) {
		printf("%d\n", j + 1);
		printf("%s ", (num + j)->second_name);
		printf("%s ", (num + j)->first_name);
		printf("%s ", (num + j)->otchestvo);
		printf("\nnumber score: %d", (num + j)->number_score);
		printf("\nbalance: %d", (num + j)->summa);
		printf("\ndate of last change: %d", (num + j)->last_change);
		puts("");
		puts("");
	}
	printf("press enter to conitinue");
	char press = '1';
	while (press != '\n') {
		scanf_s("%c", &press);
	}
	system("cls");
	return;
}
void info_change(office *num, int data_amount) {
	system("cls");
	printf("select client for changes\n(enter 0 if you want to go back)\n");
	for (int j = 0; j < data_amount; j++) {
		printf("%d\n", j + 1);
		printf("name: %s ", (num + j)->second_name);
		printf("\nnumber score: %d", (num + j)->number_score);
		printf("\n\n");
	}
	int client;
	checker_num_peacefull(client);
	if (client > data_amount) {
		printf("invalid client\npress enter to continue");
		rewind(stdin);
		char press = '1';
		while (press != '\n') {
			scanf_s("%c", &press);
		}
		system("cls");
		return;
	}
	if (client == 0) {
		system("cls");
		return;
	}
	client--;
	system("cls");
	printf("selected client is %d\n", client + 1);
	printf("%s ", (num + client)->second_name);
	printf("%s ", (num + client)->first_name);
	printf("%s ", (num + client)->otchestvo);
	printf("\nnumber score: %d", (num + client)->number_score);
	printf("\nbalance: %d", (num + client)->summa);
	printf("\ndate of last change: %d", (num + client)->last_change);
	printf("\nwhat do you want to change?\n");
	printf("1) second name\n2) first_name\n3) otchestvo\n4) number score\n5) balance\n6) last change\n");
	int changeable;
	checker_num(changeable);
	if (changeable > 6) {
		printf("invalid client\npress enter to conitinue");
		rewind(stdin);
		char press = '1';
		while (press != '\n') {
			scanf_s("%c", &press);
		}
		system("cls");
		return;
	}
	int temp;
	switch (changeable) {
	case 1: system("cls");
		printf("enter new second name: ");
		entering_second_name(num, client, 0);
		system("cls");
		break;
	case 2: system("cls");
		printf("enter new first name: ");
		entering_first_name(num, client, 0);
		system("cls");
		break;
	case 3: system("cls");
		printf("enter new otchestvo: ");
		entering_otchestvo(num, client, 0);
		system("cls");
		break;
	case 4: system("cls");
		printf("enter new number score: ");
		checker_num(temp);
		(num + client)->number_score = temp;
		system("cls");
		break;
	case 5: system("cls");
		printf("enter new balance: ");
		checker_num_peacefull(temp);
		(num + client)->summa = temp;
		system("cls");
		break;
	case 6: system("cls");
		printf("enter new date (yyymmdd): ");
		checker_num(temp);
		(num + client)->last_change = temp;
		system("cls");
		break;
	}
}
void swap(office *num, int first, int second) {
	char *temp;
	temp = (num + first)->second_name;
	(num + first)->second_name = (num + second)->second_name;
	(num + second)->second_name = temp;

	temp = (num + first)->first_name;
	(num + first)->first_name = (num + second)->first_name;
	(num + second)->first_name = temp;

	temp = (num + first)->otchestvo;
	(num + first)->otchestvo = (num + second)->otchestvo;
	(num + second)->otchestvo = temp;

	int temp2;
	temp2 = (num + first)->number_score;
	(num + first)->number_score = (num + second)->number_score;
	(num + second)->number_score = temp2;

	temp2 = (num + first)->summa;
	(num + first)->summa = (num + second)->summa;
	(num + second)->summa = temp2;

	temp2 = (num + first)->last_change;
	(num + first)->last_change = (num + second)->last_change;
	(num + second)->last_change = temp2;
}
void delete_client(office *num, int &data_amount) {
	system("cls");
	printf("select client for deleting\n(enter 0 if you want to go back)\n");
	for (int j = 0; j < data_amount; j++) {
		printf("%d\n", j + 1);
		printf("name: %s ", (num + j)->second_name);
		printf("\nnumber score: %d", (num + j)->number_score);
		printf("\n\n");
	}
	int client;
	checker_num_peacefull(client);
	if (client > data_amount) {
		printf("invalid client\npress enter to conitinue");
		rewind(stdin);
		char press = '1';
		while (press != '\n') {
			scanf_s("%c", &press);
		}
		system("cls");
		return;
	}
	if (client == 0) {
		system("cls");
		return;
	}
	client--;
	printf("WARNING!\nYou loose all information about this client\nAre you sure you want to continue?\n1 - yes\nany other nubmer - no\n");
	int warning;
	checker_num_peacefull(warning);
	if (warning != 1) {
		system("cls");
		return;
	}
	for (int j = client; j < data_amount - 1; j++) {
		swap(num, j, j + 1);
	}
	free((num + data_amount - 1)->second_name);
	free((num + data_amount - 1)->first_name);
	free((num + data_amount - 1)->otchestvo);
	data_amount--;
	system("cls");
}
void sort_secondName(office *num, int data_amount) {
	int position = 0;
	char temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (num + i)->second_name[0];
		if (temp < 97) {
			temp += 32;
		}
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (num + j)->second_name[0];
			if (temp2 < 97) {
				temp2 += 32;
			}
			if (temp2 < temp) {
				position = j;
				temp = temp2;
			}
			else if (temp2 == temp) {
				char temp3, temp4;
				int probeg = 1;
				while (1) {
					temp3 = (num + position)->second_name[probeg];
					temp4 = (num + j)->second_name[probeg];
					//���� ����� �����������
					if (temp3 == '\0' && temp4 != '\0') {
						position = j;
						temp = temp2;
						break;
					}
					if (temp4 == '\0') {
						break;
					}
					//������������ �����
					if (temp3 < 97) {
						temp3 += 32;
					}
					if (temp4 < 97) {
						temp4 += 32;
					}
					//���������
					if (temp3 < temp4) {
						break;
					}
					else if (temp4 < temp3) {
						position = j;
						temp = temp2;
						break;
					}
					probeg++; //������� �� ����� ����� � �����
				}
			}
		}
		swap(num, i, position);
	}
	system("cls");
}
void sort_otchestvo(office *num, int data_amount) {
	int position = 0;
	char temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (num + i)->otchestvo[0];
		if (temp < 97) {
			temp += 32;
		}
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (num + j)->otchestvo[0];
			if (temp2 < 97) {
				temp2 += 32;
			}
			if (temp2 < temp) {
				position = j;
				temp = temp2;
			}
			else if (temp2 == temp) {
				char temp3, temp4;
				int probeg = 1;
				while (1) {
					temp3 = (num + position)->otchestvo[probeg];
					temp4 = (num + j)->otchestvo[probeg];
					//���� ����� �����������
					if (temp3 == '\0' && temp4 != '\0') {
						position = j;
						temp = temp2;
						break;
					}
					if (temp4 == '\0') {
						break;
					}
					//������������ �����
					if (temp3 < 97) {
						temp3 += 32;
					}
					if (temp4 < 97) {
						temp4 += 32;
					}
					//���������
					if (temp3 < temp4) {
						break;
					}
					else if (temp4 < temp3) {
						position = j;
						temp = temp2;
						break;
					}
					probeg++; //������� �� ����� ����� � �����
				}
			}
		}
		swap(num, i, position);
	}
	system("cls");
}
void sort_firstName(office *num, int data_amount) {
	int position = 0;
	char temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (num + i)->first_name[0];
		if (temp < 97) {
			temp += 32;
		}
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (num + j)->first_name[0];
			if (temp2 < 97) {
				temp2 += 32;
			}
			if (temp2 < temp) {
				position = j;
				temp = temp2;
			}
			else if (temp2 == temp) {
				char temp3, temp4;
				int probeg = 1;
				while (1) {
					temp3 = (num + position)->first_name[probeg];
					temp4 = (num + j)->first_name[probeg];
					//���� ����� �����������
					if (temp3 == '\0' && temp4 != '\0') {
						position = j;
						temp = temp2;
						break;
					}
					if (temp4 == '\0') {
						break;
					}
					//������������ �����
					if (temp3 < 97) {
						temp3 += 32;
					}
					if (temp4 < 97) {
						temp4 += 32;
					}
					//���������
					if (temp3 < temp4) {
						break;
					}
					else if (temp4 < temp3) {
						position = j;
						temp = temp2;
						break;
					}
					probeg++; //������� �� ����� ����� � �����
				}
			}
		}
		swap(num, i, position);
	}
	system("cls");
}
void sort_summ(office *num, int data_amount) {
	int position = 0;
	int temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (num + i)->summa;
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (num + j)->summa;
			if (temp2 > temp) {
				position = j;
				temp = temp2;
			}
		}
		swap(num, i, position);
	}
	system("cls");
}
void sort_number(office *num, int data_amount) {
	int position = 0;
	int temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (num + i)->number_score;
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (num + j)->number_score;
			if (temp2 > temp) {
				position = j;
				temp = temp2;
			}
		}
		swap(num, i, position);
	}
	system("cls");
}
void sort_date(office *num, int data_amount) {
	int position = 0;
	int temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (num + i)->last_change;
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (num + j)->last_change;
			if (temp2 > temp) {
				position = j;
				temp = temp2;
			}
		}
		swap(num, i, position);
	}
	system("cls");
}
void sort_menu(office *num, int data_amount) {
	system("cls");
	if (data_amount == 0) {
		printf("nothing to sort\npress enter to continue");
		char temp = '1';
		while (temp != '\n') {
			scanf_s("%c", &temp);
		}
		system("cls");
		return;
	}
	int option;
	printf("choose how you want to sort database:\n0)go back\n1)by second name\n2)by first name\n3)by otchestvo\n4)by summ\n5)by number score\n6)by last change(data)\n");
	checker_num_peacefull(option);
	switch (option) {
	case 0: system("cls"); return;
	case 1: sort_secondName(num, data_amount); return;
	case 2: sort_firstName(num, data_amount); return;
	case 3: sort_otchestvo(num, data_amount); return;
	case 4: sort_summ(num, data_amount); return;
	case 5: sort_number(num, data_amount); return;
	case 6: sort_date(num, data_amount); return;
	}
}
int main() {
	office *num;
	num = (office*)malloc(1 * sizeof(office));
	system("cls");
	int option, data_amount = 0;
	while (1) { // menu - start
		menu(option);
		if (option == 1) {
			system("cls");
			printf("\t\t\tlooking forward to see you again\n\n\n\n\n\n\n");
			break;
		}
		if (option == 2) {
			info_change(num, data_amount);
			continue;
		}
		if (option == 3) {
			sort_menu(num, data_amount);
			continue;
		}
		if (option == 4) {
			delete_client(num, data_amount);
			num = (office*)realloc(num, data_amount * sizeof(office));
			continue;
		}
		if (option == 5) {
			system("cls");
			int n;
			printf("enter how many clients you want to add\n(enter 0 if you want to go back)\n");
			checker_num_peacefull(n);
			if (n == 0) {
				system("cls");
				continue;
			}
			num = (office*)realloc(num, (n + data_amount) * sizeof(office));
			data_add(num, data_amount, n);
			continue;
		}
		if (option == 6) {
			show_database(num, data_amount);
			continue;
		}
		if (option == 7) {
			searching_system(num, data_amount);
			continue;
		}
	}
}