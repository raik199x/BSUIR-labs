#include <stdio.h>
#include <malloc.h>
#include <Windows.h>

union additional_data {
	char last_win[5];
	int looses;
};

struct football_teams {
	int winner_number;
	char *team_name;
	int flag;
	additional_data odd_data;
};

void enter_continue() {
	rewind(stdin);
	printf("press enter to continue");
	char temp = '1';
	while (temp != '\n') {
		scanf_s("%c", &temp);
	}
	system("cls");
}

void entering_team_name(football_teams *teams, int j, int data_amount) {
	char temp;
	int i = 0, n = 1;
	(teams + j + data_amount)->team_name = (char*)malloc(1 * sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			(teams + j + data_amount)->team_name[i] = '\0';
			break;
		}
		(teams + j + data_amount)->team_name[i] = temp;
		i++; n++;
		(teams + j + data_amount)->team_name = (char*)realloc((teams + j + data_amount)->team_name, n * sizeof(char));
	}
}

void checker_num(int &n) {
	int temp = scanf_s("%d", &n);
	while (temp != 1 || n <= 0 || n > 9999999) {
		if (temp != 1) {
			printf("detected letters, do not use them\n");
		} else if (n == 0) {
			   printf("0 pointer\n");
		} else if (n < 0) {
			printf("detected negative number, unacceptable\n");
		} else if (n > 999999) {
			printf("we are sorry! our programm dont work with number higher than 999999\n");
		}
		rewind(stdin);
		temp = scanf_s("%d", &n);
	}
}
void entering_num_date(football_teams *teams, int data_amount, int j) {
	char temp;
	rewind(stdin);
	for (int i = 0; i < 4; i++) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			printf("not enough number, left to write - %d\n", 4-i);
			i--;
			continue;
		}else  if (temp < '0' || temp > '9') {
		     	i--;
		     	printf("letter detected, dont use them\n");
		    	continue;
		       }
		(teams + j + data_amount)->odd_data.last_win[i]= temp;
	}
	(teams + j + data_amount)->odd_data.last_win[4] = '\0';
	rewind(stdin);
}
void checker_num_peacefull(int &n) {
	int temp = scanf_s("%d", &n);
	while (temp != 1 || n < 0 || n > 9999999) {
		if (temp != 1) {
			printf("detected letters, do not use them\n");
		} else if (n < 0) {
			    printf("detected negative number, unacceptable\n");
		       }
		else if (n > 999999) {
			printf("we are sorry! our programm dont work with number higher than 999999\n");
		}
		rewind(stdin);
		temp = scanf_s("%d", &n);
	}
}

void show_database(football_teams *teams, int data_amount) {
	rewind(stdin);
	system("cls");
	if (data_amount == 0) {
		printf("at that moment you don't have any clients =(\n");
		enter_continue();
		return;
	}
	printf("  \t\twins\t\tlosses\t\tdate of last win\n");
	for (int j = 0; j < data_amount; j++) {
		printf("%d ", j + 1);
		int temp = 0;
		for (int i = 0; (teams + j)->team_name[i] != '\0'; i++) {
			if (temp == 13) {
				printf("\n  ");
				temp = 0;
			}
			temp++;
			printf("%c", (teams + j)->team_name[i]);
		}
		while (temp != 13) {
			temp++;
			printf(" ");
		}
		printf("\t");
		printf("%d\t\t", (teams + j)->winner_number);
		if ((teams + j)->flag == 1) {
			printf("%d\t\t", (teams + j)->odd_data.looses);
		}
		else {
			printf("\t\t%s", (teams + j)->odd_data.last_win);
		}
		puts("");
		puts("");
	}
	enter_continue();
	return;
}

void data_add(football_teams *teams, int &data_amount, int n) {
	int temp;
	for (int j = 0; j < n; j++) {
		system("CLS");
		printf("entering %d team\n", j + 1);
		printf("enter team name: ");
		entering_team_name(teams, j, data_amount);
		printf("enter count of wins: ");
		checker_num_peacefull(temp);
		(teams + j + data_amount)->winner_number = temp;
		printf("time to choose\n1)looses\n2)date of last win\n");
		temp = 3;
		while (temp > 2) {
			checker_num_peacefull(temp);
			if (temp > 2) {
				printf("invalid number\n");
			}
		}
		(teams + j + data_amount)->flag = temp;
		if (temp == 1) {
			rewind(stdin);
			printf("enter count of losses: ");
			checker_num_peacefull(temp);
			(teams + j + data_amount)->odd_data.looses = temp;
		}
		else if (temp == 2) {
			rewind(stdin);
			printf("enter year of last win (yyyy): ");
			entering_num_date(teams, data_amount, j);
		}
	}
	data_amount += n;
	system("cls");
}

void swap(football_teams *teams, int first, int second) {
	football_teams *temp;
	temp = (football_teams*)malloc(1 * sizeof(football_teams));
	*temp = *(teams + first);
	*(teams + first) = *(teams + second);
	*(teams + second) = *temp;
}

void delete_team(football_teams *teams, int &data_amount) {
	system("cls");
	if (data_amount == 0) {
		printf("nothing to delete\n");
		enter_continue();
		return;
	}
	printf("select team for deleting\n(enter 0 if you want to go back)\n\n");
	printf("\t team name\n");
	for (int j = 0; j < data_amount; j++) {
		printf("%d\n", j + 1);
		printf("name: %s ", (teams + j)->team_name);
		printf("\n\n");
	}
	int client;
	checker_num_peacefull(client);
	if (client > data_amount) {
		printf("invalid client\n");
		rewind(stdin);
		enter_continue();
		return;
	}
	if (client == 0) {
		system("cls");
		return;
	}
	client--;
	printf("WARNING!\nYou loose all information about this team\nAre you sure you want to continue?\n1 - yes\nany other nubmer - no\n");
	int warning;
	checker_num_peacefull(warning);
	if (warning != 1) {
		system("cls");
		return;
	}
	for (int j = client; j < data_amount - 1; j++) {
		swap(teams, j, j + 1);
	}
	free((teams + data_amount - 1)->team_name);
	data_amount--;
	teams = (football_teams*)realloc(teams, data_amount * sizeof(football_teams));
	system("cls");
}

void menu(int &option) {
	printf("*************************\n");
	printf("*                       *\n");
	printf("* welcome to main menu  *\n");
	printf("*                       *\n");
	printf("*************************\n");
	printf("\n\n");
	printf("1)exit programm\n2)add teams\n3)show database\n4)delete team\n5)searching system\n6)sorting system\n7)tools\n");
	checker_num(option);
	while (option > 7) {
		system("cls");
		printf("missing function, please check existing variants\n\n");
		printf("1)exit programm\n2)add teams\n3)show database\n4)delete team\n5)searching system\n6)sorting system\n7)tools\n");
		rewind(stdin);
		checker_num(option);
	}
}

void searching_teamName(football_teams *teams, int data_amount) {
	char *search, temp;
	system("cls");
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
		enter_continue();
		rewind(stdin);
		free(search);
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
			//skip symbols
			if (search[step] == '*') {
				flag = 1;
				step++;
			}
			//good end
			if (search[step] == '\0' && search[step] == (teams + i)->team_name[j]) {
				printf("\nteam position: %d\nteam name: %s\n", i + 1, (teams + i)->team_name);
				printf("continue searching?\n 0) yes\n 1) no\n");
				checker_num_peacefull(stop_me);
				if (stop_me != 0) {
					system("cls");
					free(search);
					return;
				}
				break;
			}
			//bad end
			if ((teams + i)->team_name[j] == '\0') {
				j = 0;
				step = 0;
				printf("noting found\n");
			    enter_continue();
				break;
			}
			//speed up
			if (search[step] == '\0' && flag == 1) {
				j++;
				continue;
			}
			//bad end
			if ((teams + i)->team_name[j] != search[step] && (teams + i)->team_name[j] + 32 != search[step] && flag == 0) {
				j = 0;
				step = 0;
				printf("nothing found\n");
				enter_continue();
				break;
			}
			if ((teams + i)->team_name[j] == search[step] || (teams + i)->team_name[j] + 32 == search[step]) {
				step++;
				flag = 0;
			}
			j++;
		}
		j = 0;
		step = 0;
		if (stop_me != 0 || i == data_amount - 1) {
			free(search);
			printf("searching ended\n");
			enter_continue();
			system("cls");
			return;
		}
	}
	printf("searching ended\n");
	enter_continue();
	free(search);
}

void searching_wins(football_teams *teams, int data_amount) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	for (int i = 0; i < data_amount; i++) {
		if ((teams + i)->winner_number >= counter - depth && (teams + i)->winner_number <= counter + depth) {
			printf("team: %d\nteam name: %s\n wins: %d\n", i + 1, (teams + i)->team_name, (teams + i)->winner_number);
			printf("Do you want to continue?\n0)yes\n1)no\n");
			int variant;
			checker_num_peacefull(variant);
			if (variant != 0) {
				system("cls");
				return;
			}
		}
	}
	printf("searching ended\n");
	enter_continue();
	system("cls");
}

void searching_losses(football_teams *teams, int data_amount) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	for (int i = 0; i < data_amount; i++) {
		if ((teams + i)->odd_data.looses >= counter - depth && (teams + i)->odd_data.looses <= counter + depth) {
			printf("team: %d\nteam name: %s\n losses: %d\n", i + 1, (teams + i)->team_name, (teams + i)->odd_data.looses);
			printf("Do you want to continue?\n0)yes\n1)no\n");
			int variant;
			checker_num_peacefull(variant);
			if (variant != 0) {
				system("cls");
				return;
			}
		}
	}
	printf("searching ended\n");
	enter_continue();
	system("cls");
}

void searching_lastWin(football_teams *teams, int data_amount) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	for (int i = 0; i < data_amount; i++) {
		int temp_number;
		temp_number = ((teams + i)->odd_data.last_win[0] - 48) * 1000 + ((teams + i)->odd_data.last_win[1] - 48) * 100 + ((teams + i)->odd_data.last_win[2] - 48) * 10 + ((teams + i)->odd_data.last_win[3]-48);
		if (temp_number >= counter - depth && temp_number <= counter + depth) {
			printf("team: %d\nteam name: %s\n last win: %s\n", i + 1, (teams + i)->team_name, (teams + i)->odd_data.last_win);
			printf("Do you want to continue?\n0)yes\n1)no\n");
			int variant;
			checker_num_peacefull(variant);
			if (variant != 0) {
				system("cls");
				return;
			}
		}
	}
	printf("searching ended\n");
	enter_continue();
	system("cls");
}

void searching_system(football_teams *teams, int data_amount) {
	system("cls");
	printf("what do you want to search?\n0)go back\n1)team name\n2)wins\n3)losses\n4)date of last win\n");
	int option;
	checker_num_peacefull(option);
	switch (option) {
	case 0: system("cls"); return;
	case 1: searching_teamName(teams, data_amount); return;
    case 2: searching_wins(teams, data_amount); return;
    case 3: searching_losses(teams, data_amount); return;
	case 4: searching_lastWin(teams, data_amount); return;
	}
}

void sort_teamName(football_teams *teams, int data_amount) {
	int position = 0;
	char temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (teams + i)->team_name[0];
		if (temp < 97) {
			temp += 32;
		}
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (teams + j)->team_name[0];
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
					temp3 = (teams + position)->team_name[probeg];
					temp4 = (teams + j)->team_name[probeg];
					//if it end
					if (temp3 == '\0' && temp4 != '\0') {
						position = j;
						temp = temp2;
						break;
					}
					if (temp4 == '\0') {
						break;
					}
					//register
					if (temp3 < 97) {
						temp3 += 32;
					}
					if (temp4 < 97) {
						temp4 += 32;
					}
					if (temp3 < temp4) {
						break;
					}
					else if (temp4 < temp3) {
						position = j;
						temp = temp2;
						break;
					}
					probeg++;
				}
			}
		}
		swap(teams, i, position);
	}
	system("cls");
}

void sort_wins(football_teams *teams, int data_amount) {
	printf("sort by\n1)from highest\n2)to highest\n3)back to menu\n");
	int method_sort;
	checker_num(method_sort);
	if (method_sort >= 3) {
		system("cls");
		return;
	}
	int position = 0;
	int temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		temp = (teams + i)->winner_number;
		for (int j = i + 1; j < data_amount; j++) {
			temp2 = (teams + j)->winner_number;
			if (temp2 > temp) {
				position = j;
				temp = temp2;
			}
		}
		swap(teams, i, position);
	}
	system("cls");
	if (method_sort == 2) {
		int reverse = data_amount / 2;
		for (int i = 0; i < reverse; i++) {
			swap(teams, i, data_amount - i - 1);
		}
	}
}

void sort_losses(football_teams *teams, int data_amount) {
	printf("sort by\n1)from highest\n2)to highest\n3)back to menu\n");
	int method_sort;
	checker_num(method_sort);
	if (method_sort >= 3) {
		system("cls");
		return;
	}
	int position = 0;
	int temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		if ((teams + i)->flag == 2) {
			temp = 0;
		}
		else {
			temp = (teams + i)->odd_data.looses;
		}
		for (int j = i + 1; j < data_amount; j++) {
			if ((teams + j)->flag == 2) {
				temp2 = 0;
			}
			else {
				temp2 = (teams + j)->odd_data.looses;
			}
			if (temp2 > temp) {
				position = j;
				temp = temp2;
			}
		}
		swap(teams, i, position);
	}
	system("cls");
	if (method_sort == 2) {
		int reverse = data_amount / 2;
		for (int i = 0; i < reverse; i++) {
			swap(teams, i, data_amount - i - 1);
		}
	}
}

void sort_winData(football_teams *teams, int data_amount) {
	printf("sort by\n1)from highest\n2)to highest\n3)back to menu\n");
	int method_sort;
	checker_num(method_sort);
	if (method_sort >= 3) {
		system("cls");
		return;
	}
	int position = 0;
	int temp, temp2;
	for (int i = 0; i < data_amount - 1; i++) {
		position = i;
		if ((teams + i)->flag == 1) {
			temp = 0;
		}
		else {
			temp = ((teams + i)->odd_data.last_win[0] - 48) * 1000 + ((teams + i)->odd_data.last_win[1] - 48) * 100 + ((teams + i)->odd_data.last_win[2] - 48) * 10 + ((teams + i)->odd_data.last_win[3] - 48);
		}
		for (int j = i + 1; j < data_amount; j++) {
			if ((teams + j)->flag == 1) {
				temp = 0;
			}
			else {
				temp2 = ((teams + j)->odd_data.last_win[0] - 48) * 1000 + ((teams + j)->odd_data.last_win[1] - 48) * 100 + ((teams + j)->odd_data.last_win[2] - 48) * 10 + ((teams + j)->odd_data.last_win[3] - 48);
			}
			if (temp2 > temp) {
				position = j;
				temp = temp2;
			}
		}
		swap(teams, i, position);
	}
	system("cls");
	if (method_sort == 2) {
		int reverse = data_amount / 2;
		for (int i = 0; i < reverse; i++) {
			swap(teams, i, data_amount - i - 1);
		}
	}
}

void sort_menu(football_teams *teams, int data_amount) {
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
	printf("choose how you want to sort database:\n0)go back\n1)by team name\n2)by wins\n3)by losses\n4)by last win date\n\n");
	checker_num_peacefull(option);
	switch (option) {
	case 0: system("cls"); return;
	case 1: sort_teamName(teams, data_amount); return;
	case 2: sort_wins(teams, data_amount); return;
	case 3: sort_losses(teams, data_amount); return;
	case 4: sort_winData(teams, data_amount); return;
	}
}

void tools(football_teams *teams, int &data_amount) {
	system("cls");
	printf("beta test\nHere you can see something that we want to add in future update\nyou can try them\n");
	printf("1)go back\n2)deleting teams if the wount win after *** year\n");
	int option;
	checker_num(option);
	if (option == 1 || option > 2) {
		system("cls");
		return;
	}
	printf("enter date\n");
	int date;
	while (1) {
		int temp;
		temp = scanf_s("%d", &date);
		if (!temp || date < 1000 || date > 9999) {
			rewind(stdin);
			printf("error\n");
			continue;
		}
		break;
	}
	for (int i = 0; i < data_amount; i++) {
		if ((teams + i)->flag == 1) {
			continue;
		}
		int temp2 = ((teams + i)->odd_data.last_win[0] - 48) * 1000 + ((teams + i)->odd_data.last_win[1] - 48) * 100 + ((teams + i)->odd_data.last_win[2] - 48) * 10 + ((teams + i)->odd_data.last_win[3] - 48);
		if (temp2 < date) {
			for (int j = i; j < data_amount - 1; j++) {
				swap(teams, j, j + 1);
			}
			free((teams + data_amount - 1)->team_name);
			data_amount--;
			i--;
		}
	}
	system("cls");
}

int main() {
	football_teams *teams;
	teams = (football_teams*)malloc(1 * sizeof(football_teams));
	if (!teams) {
		printf("impossible error, calling fbi");
		return 0;
	}
	system("cls");
	int option, data_amount = 0;
	while (1) { // menu - start
		menu(option);
		switch (option) {
		case 1: system("cls");
			    printf("\t\t\tlooking forward to see you again\n\n\n\n\n\n\n");
			    return 0;
		case 2: system("cls");
			    int n;
			    printf("enter how many teams you want to add\n(enter 0 if you want to go back)\n");
			    checker_num_peacefull(n);
			    if (n == 0) {
				  system("cls");
				  break;
			    }
			    teams = (football_teams*)realloc(teams, (n + data_amount) * sizeof(football_teams));
			    data_add(teams, data_amount, n);
				break;
		case 3: show_database(teams, data_amount);
			    break;
		case 4: delete_team(teams, data_amount);
			    teams = (football_teams*)realloc(teams, data_amount * sizeof(football_teams));
			    break;
		case 5: searching_system(teams, data_amount);
			    break;
		case 6: sort_menu(teams, data_amount);
			    break;
		case 7: tools(teams, data_amount);
			    break;
		}
	}
}