#include <stdio.h>
#include <malloc.h>
#include <Windows.h>

union additional_data {
	char last_win[5];
	int looses;
};

struct football_teams {
	int flag;
	int winner_number;
	char *team_name;
	additional_data odd_data;
	football_teams *uk;
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

void checker_num(int &n) {
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

void print_menu() {
	system("cls");
	printf("What do you want to do?\n");
	printf("1. Add to stack\n");
	printf("2. Print all stack\n");
	printf("3. delete iteam from stack\n");
	printf("4. searching system\n");
	printf("5. file menu\n");
	printf("6. Exit\n");
	printf(">");
}

void checker_num_peacefull(int &n) {
	int temp = scanf_s("%d", &n);
	while (temp != 1 || n < 0 || n > 9999999) {
		if (temp != 1) {
			printf("detected letters, do not use them\n");
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

char* entering_team_name(char *bufer) {
	char temp;
	int i = 0, n = 1;
	bufer = (char*)malloc(1 * sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '~') {
			printf("exception founden '~'\n");
			continue;
		}
		if (i == 0 && temp == ' ') {
			printf("name cannot start with ' '\n");
			continue;
		}
		if (temp == '\n') {
			bufer[i] = '\0';
			break;
		}
		bufer[i] = temp;
		i++; n++;
		bufer = (char*)realloc(bufer, n * sizeof(char));
	}
	return bufer;
}

void entering_num_date(char bufer[]) {
	char temp;
	rewind(stdin);
	for (int i = 0; i < 4; i++) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			printf("not enough number, left to write - %d\n", 4 - i);
			i--;
			continue;
		}
		else  if (temp < '0' || temp > '9') {
			i--;
			printf("letter detected, dont use them\n");
			continue;
		}
		bufer[i] = temp;
	}
	bufer[4] = '\0';
	rewind(stdin);
}

void data_add(football_teams **teams) {
	football_teams *entering = *teams;
	*teams = (football_teams*)malloc(sizeof(football_teams));
	int temp;
	system("CLS");
	printf("enter team name: ");
	char *bufer;
	bufer = (char*)malloc(sizeof(char));
	(*teams)->team_name = entering_team_name(bufer);
	free(bufer);
	printf("enter count of wins: ");
	checker_num_peacefull(temp);
	(*teams)->winner_number = temp;
	printf("what you want to keep?\n1)date of last win\n2)amount of looses\n");
	while (1) {
		checker_num(temp);
		if (temp > 2 || temp < 0) {
			printf("invalid number\n");
			continue;
		}
		(*teams)->flag = temp;
		break;
	}
	if (temp == 2) {
		printf("enter count of losses: ");
		checker_num_peacefull(temp);
		(*teams)->odd_data.looses = temp;
	}
	else {
		printf("enter year of last win (yyyy): ");
		char bufer2[5];
		entering_num_date(bufer);
		(*teams)->odd_data.last_win[0] = bufer[0];
		(*teams)->odd_data.last_win[1] = bufer[1];
		(*teams)->odd_data.last_win[2] = bufer[2];
		(*teams)->odd_data.last_win[3] = bufer[3];
		(*teams)->odd_data.last_win[4] = bufer[4];
	}
	(*teams)->uk = entering;
	enter_continue();
}

void output(football_teams *teams) {
	if (!teams) {
		puts("stack is empty");
		enter_continue();
		return;
	}
	int j = 0;
	printf("num  team name\t\twins\t\tlosses\t\tdate of last win\n");
	do {
		printf("%3d  ", j + 1);
		int temp = 0;
		for (int i = 0; teams->team_name[i] != '\0'; i++) {
			if (temp == 13) {
				printf("\n    ");
				temp = 0;
			}
			temp++;
			printf("%c", teams->team_name[i]);
		}
		while (temp != 13) {
			temp++;
			printf(" ");
		}
		printf("\t");
		printf("%d\t\t", teams->winner_number);
		if (teams->flag == 2) {
			printf("%d\t\t", teams->odd_data.looses);
		}
		else {
			printf("\t\t%s", teams->odd_data.last_win);
		}
		puts("");
		puts("");
		teams = teams->uk;
		j++;
	} while (teams);
	enter_continue();
}

void stack_delete(football_teams **teams) {
	int option, j = 0;
	if (!(*teams)) {
		printf("stack empty\n");
		enter_continue();
		return;
	}
	football_teams *bufer = *teams, *probeg = *teams;
	printf("choose team to delete or enter 0 to go back\n");
	printf("num  team name\t\twins\t\tlosses\t\tdate of last win\n");
	do {
		printf("%3d ", j + 1);
		int temp = 0;
		for (int i = 0; bufer->team_name[i] != '\0'; i++) {
			if (temp == 13) {
				printf("\n    ");
				temp = 0;
			}
			temp++;
			printf("%c", bufer->team_name[i]);
		}
		while (temp != 13) {
			temp++;
			printf(" ");
		}
		printf("\t");
		printf("%d\t\t", bufer->winner_number);
		if (bufer->flag == 2) {
			printf("%d\t\t", bufer->odd_data.looses);
		}
		else {
			printf("\t\t%s", bufer->odd_data.last_win);
		}
		puts("");
		puts("");
		bufer = bufer->uk;
		j++;
	} while (bufer);
	checker_num_peacefull(option);
	if (option == 0)
		return;
	else if (option > j) {
		printf("invalid team\n");
		enter_continue();
		return;
	}
	bufer = *teams;
	for (int i = 0; i < option - 1; i++) {
		probeg = bufer;
		bufer = bufer->uk;
	}
	if ((*teams) == bufer) {
		free(bufer->team_name);
		(*teams) = (*teams)->uk;
		free(bufer);
	}
	else {
		free(bufer->team_name);
		probeg->uk = bufer->uk;
		free(bufer);
	}
}

void searching_teamName(football_teams *teams) {
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
	int flag = 0, step = 0, j = 0, stop_me = 0, table = 0;
	do {
		while (stop_me == 0) {
			//skip symbols
			if (search[step] == '*') {
				flag = 1;
				step++;
			}
			//good end
			if (search[step] == '\0' && search[step] == teams->team_name[j]) {
				if (table == 0) { //output table
					table++;
					printf("  team name\twins\t\tlosses\t\tdate of last win\n");
				}  // end output table
				int temp = 0; //start output info about team
				for (int i = 0; teams->team_name[i] != '\0'; i++) {
					if (temp == 13) {
						printf("\n  ");
						temp = 0;
					}
					temp++;
					printf("%c", teams->team_name[i]);
				}
				while (temp != 13) {
					temp++;
					printf(" ");
				}
				printf("\t");
				printf("%d\t\t", teams->winner_number);
				if (teams->flag == 2) {
					printf("%d\t\t", teams->odd_data.looses);
				}
				else {
					printf("\t\t%s", teams->odd_data.last_win);
				}
				puts("");
				puts("");//end output info
				break;
			}
			//bad end
			if (teams->team_name[j] == '\0') {
				j = 0;
				step = 0;
				break;
			}
			//speed up
			if (search[step] == '\0' && flag == 1) {
				j++;
				continue;
			}
			//bad end
			if (teams->team_name[j] != search[step] && teams->team_name[j] + 32 != search[step] && flag == 0) {
				j = 0;
				step = 0;
				break;
			}
			if (teams->team_name[j] == search[step] || teams->team_name[j] + 32 == search[step]) {
				step++;
				flag = 0;
			}
			j++;
		}
		j = 0;
		step = 0;
		teams = teams->uk;
		if (stop_me != 0 || !teams) {
			free(search);
			printf("\n\nsearching ended\n");
			enter_continue();
			system("cls");
			return;
		}
	} while (teams);
	printf("searching ended\n");
	enter_continue();
	free(search);
}

void searching_wins(football_teams *teams) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	do {
		if (teams->winner_number >= counter - depth && teams->winner_number <= counter + depth) {
			int temp = 0;
			for (int i = 0; teams->team_name[i] != '\0'; i++) {
				if (temp == 13) {
					printf("\n  ");
					temp = 0;
				}
				temp++;
				printf("%c", teams->team_name[i]);
			}
			while (temp != 13) {
				temp++;
				printf(" ");
			}
			printf("\t");
			printf("%d\t\t", teams->winner_number);
			if (teams->flag == 2) {
				printf("%d\t\t", teams->odd_data.looses);
			}
			else {
				printf("\t\t%s", teams->odd_data.last_win);
			}
			puts("");
			puts("");
		}
		teams = teams->uk;
	} while (teams);
	printf("searching ended\n");
	enter_continue();
	system("cls");
}

void searching_losses(football_teams *teams) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	do {
		if (teams->odd_data.looses >= counter - depth && teams->odd_data.looses <= counter + depth) {
			int temp = 0;
			for (int i = 0; teams->team_name[i] != '\0'; i++) {
				if (temp == 13) {
					printf("\n  ");
					temp = 0;
				}
				temp++;
				printf("%c", teams->team_name[i]);
			}
			while (temp != 13) {
				temp++;
				printf(" ");
			}
			printf("\t");
			printf("%d\t\t", teams->winner_number);
			if (teams->flag == 2) {
				printf("%d\t\t", teams->odd_data.looses);
			}
			else {
				printf("\t\t%s", teams->odd_data.last_win);
			}
			puts("");
			puts("");
		}
		teams = teams->uk;
	} while (teams);
	printf("searching ended\n");
	enter_continue();
	system("cls");
}

void searching_lastWin(football_teams *teams) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	do {
		int temp_number;
		temp_number = (teams->odd_data.last_win[0] - 48) * 1000 + (teams->odd_data.last_win[1] - 48) * 100 + (teams->odd_data.last_win[2] - 48) * 10 + (teams->odd_data.last_win[3] - 48);
		if (temp_number >= counter - depth && temp_number <= counter + depth) {
			int temp = 0;
			for (int i = 0; teams->team_name[i] != '\0'; i++) {
				if (temp == 13) {
					printf("\n  ");
					temp = 0;
				}
				temp++;
				printf("%c", teams->team_name[i]);
			}
			while (temp != 13) {
				temp++;
				printf(" ");
			}
			printf("\t");
			printf("%d\t\t", teams->winner_number);
			if (teams->flag == 2) {
				printf("%d\t\t", teams->odd_data.looses);
			}
			else {
				printf("\t\t%s", teams->odd_data.last_win);
			}
			puts("");
			puts("");
		}
		teams = teams->uk;
	} while (teams);
	printf("searching ended\n");
	enter_continue();
	system("cls");
}

void searching_system(football_teams *teams) {
	system("cls");
	if (!teams) {
		printf("maybe you should try to add something?\n");
		enter_continue();
		return;
	}
	printf("what do you want to search?\n0)go back\n1)team name\n2)wins\n3)losses\n4)date of last win\n");
	int option;
	checker_num_peacefull(option);
	switch (option) {
	case 0: system("cls"); return;
	case 1: searching_teamName(teams); return;
	case 2: searching_wins(teams); return;
	case 3: searching_losses(teams); return;
	case 4: searching_lastWin(teams); return;
	}
}

void save_data_bin(football_teams *teams, FILE *file) {
	football_teams *bufer = teams;
	if (!bufer)
		return;
	else
		save_data_bin(teams->uk, file);
	char temp[1];
	int i = 0;
	while (1) {
		temp[0] = bufer->team_name[i];
		if (temp[0] == '\0')
			break;
		fwrite(temp, sizeof(char), 1, file);
		i++;
	}
	temp[0] = '~';
	fwrite(temp, sizeof(char), 1, file);

	int number[1];
	number[0] = bufer->winner_number;
	fwrite(number, sizeof(int), 1, file);
	fwrite(temp, sizeof(char), 1, file);
	number[0] = bufer->flag;
	fwrite(number, sizeof(int), 1, file);
	fwrite(temp, sizeof(char), 1, file);
	if (number[0] == 2) {
		number[0] = bufer->odd_data.looses;
		fwrite(number, sizeof(int), 1, file);
		fwrite(temp, sizeof(char), 1, file);
	}
	else {
		int j = 0;
		while (1) {
			temp[0] = bufer->odd_data.last_win[j];
			if (temp[0] == '\0')
				break;
			fwrite(temp, sizeof(char), 1, file);
			j++;
		}
		temp[0] = '~';
		fwrite(temp, sizeof(char), 1, file);
	}
	fwrite(temp, sizeof(char), 1, file);
}

void save_data(football_teams *teams, FILE *file) {
	football_teams *bufer = teams;
	if (!bufer)
		return;
	else
		save_data(teams->uk, file);
	char temp;
	int i = 0;
	while (1) {
		temp = bufer->team_name[i];
		if (temp == '\0')
			break;
		fprintf(file, "%c", temp);
		i++;
	}
	fprintf(file, "~");
	int number = bufer->winner_number;
	fprintf(file, "%d", number);
	fprintf(file, "~");
	number = bufer->flag;
	fprintf(file, "%d", number);
	fprintf(file, "~");
	if (number == 2) {
		number = bufer->odd_data.looses;
		fprintf(file, "%d", number);
		fprintf(file, "~");
	}
	else {
		int j = 0;
		while (1) {
			temp = bufer->odd_data.last_win[j];
			if (temp == '\0')
				break;
			fprintf(file, "%c", temp);
			j++;
		}
		fprintf(file, "~");
	}
	fprintf(file, "\n");
}

void save_file(football_teams *teams) {
	if (!teams) {
		printf("your stack is empty!\n");
		enter_continue();
		return;
	}
	FILE *file;
	int option = 0;
	char *name, temp;
	printf("enter file name (up to 20 symbols): ");
	int n = 0;
	name = (char*)malloc(sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '\n') {
			name[n] = '\0';
			if (fopen_s(&file, name, "w+")) {
				printf("error to create file\n");
				n = 22;
				enter_continue();
			}
			else {
				printf("1)txt\n2)bin\n");
				while (option > 2 || option < 1)
					checker_num(option);
				fclose(file);
				remove(name);
				if (option == 1) {
					name[n] = '.';
					name[n + 1] = 't';
					name[n + 2] = 'x';
					name[n + 3] = 't';
					name[n + 4] = '\0';
					fopen_s(&file, name, "w+");
				}
				else {
					name[n] = '.';
					name[n + 1] = 'b';
					name[n + 2] = 'i';
					name[n + 3] = 'n';
					name[n + 4] = '\0';
					fopen_s(&file, name, "w+b");
				}
			}
			break;
		}
		if (n == 20) {
			printf("to many symbols\nresterted\enter file name (up to 20 symbols): ");
			rewind(stdin);
			n = 0;
			name = (char*)realloc(name, (n + 1) * sizeof(char));
			continue;
		}
		name = (char*)realloc(name, (n + 1) * sizeof(char));
		name[n] = temp;
		n++;
	}
	if (n == 22)
		return;
	if (option == 1)
		save_data(teams, file);
	else
		save_data_bin(teams, file);
	fclose(file);
}

void exporting_bin(football_teams **teams, FILE *file) {
	char temp[1];
	temp[0] = ' ';
	fread(temp, sizeof(char), 1, file);
	if (temp[0] == ' ' || temp[0] == '\n') {
		printf("seems file is empty\n");
		enter_continue();
		return;
	}
	fseek(file, -1, 1);

	football_teams *entering = *teams;
	*teams = (football_teams*)malloc(sizeof(football_teams));
	(*teams)->uk = nullptr;
	int i = 0;
	(*teams)->team_name = (char*)malloc(sizeof(char));
	while (1) {
		fread(temp, sizeof(char), 1, file);
		if (temp[0] == '~') {
			(*teams)->team_name[i] = '\0';
			break;
		}
		(*teams)->team_name = (char*)realloc((*teams)->team_name, (i + 2) * sizeof(char));
		(*teams)->team_name[i] = temp[0];
		i++;
	}
	int mas[1];
	fread(mas, sizeof(int), 1, file);
	(*teams)->winner_number = mas[0];

	fread(temp, sizeof(char), 1, file);
	fread(mas, sizeof(int), 1, file);
	(*teams)->flag = mas[0];
	fread(temp, sizeof(char), 1, file);

	if (i == 2) {
		fread(mas, sizeof(int), 1, file);
		(*teams)->odd_data.looses = mas[0];
	}
	else {
		i = 0;
		fread(temp, sizeof(char), 1, file);
		(*teams)->odd_data.last_win[i] = temp[0];
		i++;
		fread(temp, sizeof(char), 1, file);
		(*teams)->odd_data.last_win[i] = temp[0];
		i++;
		fread(temp, sizeof(char), 1, file);
		(*teams)->odd_data.last_win[i] = temp[0];
		i++;
		fread(temp, sizeof(char), 1, file);
		(*teams)->odd_data.last_win[i] = temp[0];
		i++;
		(*teams)->odd_data.last_win[i] = '\0';
	}
	fread(temp, sizeof(char), 1, file);
	(*teams)->uk = entering;
}

void exporting_txt(football_teams **teams, FILE *file) {
	char temp = ' ';
	fscanf_s(file, "%c", &temp);
	if (temp == ' ' || temp == '\n') {
		printf("seems file is empty\n");
		enter_continue();
		return;
	}
	fseek(file, -1, 1);

	football_teams *entering = *teams;
	*teams = (football_teams*)malloc(sizeof(football_teams));
	(*teams)->uk = nullptr;
	int i = 0;
	(*teams)->team_name = (char*)malloc(1 * sizeof(char));
	while (1) {
		fscanf_s(file, "%c", &temp);
		if (temp == '~') {
			(*teams)->team_name[i] = '\0';
			break;
		}
		(*teams)->team_name = (char*)realloc((*teams)->team_name, (i + 2) * sizeof(char));
		(*teams)->team_name[i] = temp;
		i++;
	}
	fscanf_s(file, "%d", &i);
	(*teams)->winner_number = i;

	fscanf_s(file, "%c", &temp);
	fscanf_s(file, "%d", &i);
	(*teams)->flag = i;
	fscanf_s(file, "%c", &temp);

	if (i == 2) {
		fscanf_s(file, "%d", &i);
		(*teams)->odd_data.looses = i;
	}
	else {
		i = 0;
		fscanf_s(file, "%c", &temp);
		(*teams)->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp);
		(*teams)->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp);
		(*teams)->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp);
		(*teams)->odd_data.last_win[i] = temp;
		i++;
		(*teams)->odd_data.last_win[i] = '\0';
	}
	fscanf_s(file, "%c", &temp);
	(*teams)->uk = entering;
}

int main() {
	football_teams *teams = nullptr;
	int option = 20;
	while (option != 6) {
		print_menu();
		checker_num(option);
		switch (option) {
		case 1: data_add(&teams); break;
		case 2: output(teams); break;
		case 3: stack_delete(&teams); break;
		case 4: searching_system(teams); break;
		}
		if (option == 5) {
			system("cls");
			printf("1)save stack in file\n2)export stack from file\n3)go back\n");
			while (option > 3 || option < 1)
				checker_num(option);
			if (option == 1)
				save_file(teams);
			if (option == 2) { //////////////////////////
				system("cls");
				printf("write file name: ");
				char name[23];
				rewind(stdin);
				int i = 0, internal_error = 0;
				while (1) {
					if (i == 23) {
						printf("this file cannot be readen");
						enter_continue();
						internal_error = 1;
						break;
					}
					scanf_s("%c", &name[i]);
					if (name[i] == '\n') {
						name[i] = '\0';
						break;
					}
					i++;
				}
				if (internal_error == 1)
					continue;
				i -= 4;
				if (name[i] != '.') {
					printf("cannot find format file\n");
					enter_continue();
					continue;
				}
				i += 3;
				if (name[i] != 't' && name[i] != 'n') {
					printf("unknown format\n");
					enter_continue();
					continue;
				}
				FILE *file;
				if (fopen_s(&file, name, "r+")) {
					printf("cannot open file\n");
					enter_continue();
					continue;
				}
				if (name[i] == 't') {
					char stop_me = '\n';
					while (1) {
						exporting_txt(&teams, file);
						fscanf_s(file, "%c", &stop_me);
						fscanf_s(file, "%c", &stop_me);
						if (stop_me == '\n')
							break;
						else
							fseek(file, -1, 1);
					}
				}
				else {
					char stop_me[1];
					stop_me[0] = '~';
					while (1) {
						exporting_bin(&teams, file);
						fread(stop_me, sizeof(char), 1, file);
						fread(stop_me, sizeof(char), 1, file);
						if (stop_me[0] == '~')
							break;
						else
							fseek(file, -1, 1);
					}
				}
				fclose(file);
			} ///////////////////////////////////////////////////
		}
	}
}