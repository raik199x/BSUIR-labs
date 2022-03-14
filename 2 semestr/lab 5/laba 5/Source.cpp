#define CRT_SECURE_NO_WARNINGS

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

void enter_continue() { //to pause program until pressing enter
	rewind(stdin);
	printf("press enter to continue");
	char temp = '1';
	while (temp != '\n') {
		scanf_s("%c", &temp);
	}
	system("cls");
}

void checker_num(int &n) {  //checks correcting written number
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
	printf("1. Add to queu\n");
	printf("2. Print all queu\n");
	printf("3. delete iteam from queu\n");
	printf("4. searching system\n");
	printf("5. file menu\n");
	printf("6. Exit\n");
	printf(">");
}

void checker_num_peacefull(int &n) { //checking written number (includes 0)
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
	rewind(stdin);                                                      //free console bufer
	while (1) {
		scanf_s("%c", &temp);
		if (temp == '~') { 
			printf("exception founden '~'\n");
			continue;
		}
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

void data_add(football_teams **teams, football_teams **head, football_teams **tail) {
	football_teams *entering;                                              //create block of memory
	entering = (football_teams*)malloc(sizeof(football_teams));           //increase memory
	//filling up entering block
	int temp;
	system("CLS");
	printf("enter team name: ");
	char *bufer;
	bufer = (char*)malloc(sizeof(char));                                  //space for name
	entering->team_name = entering_team_name(bufer);                     //taking name
	free(bufer);                                                        // free because we dont need it anymore
	printf("enter count of wins: ");
	checker_num_peacefull(temp);                                       //accept number of wins
	entering->winner_number = temp;                                   //wrote number of wins
	printf("what you want to keep?\n1)date of last win\n2)amount of looses\n");
	while (1) {                                                      //choosing
		checker_num(temp);
		if (temp > 2 || temp < 0) {
			printf("invalid number\n");
			continue;
		}
		entering->flag = temp;
		break;
	}                                                             //end of choosing
	if (temp == 2) {                                             //entering looses
		printf("enter count of losses: ");
		checker_num_peacefull(temp);
		entering->odd_data.looses = temp;
	}
	else {                                                    //entering year of last win
		printf("enter year of last win (yyyy): ");
		char bufer2[5];
		entering_num_date(bufer);
		entering->odd_data.last_win[0] = bufer[0];
		entering->odd_data.last_win[1] = bufer[1];
		entering->odd_data.last_win[2] = bufer[2];
		entering->odd_data.last_win[3] = bufer[3];
		entering->odd_data.last_win[4] = bufer[4];
	}
	entering->uk = nullptr;                              // end enetring block
	if (!(*tail)) {                                     //if we dont have any queu
		*teams = entering;
		*head = *teams;
		*tail = *teams;
	}
	else {                                             //add to end of queu
		(*tail)->uk = entering;
		(*tail) = (*tail)->uk;
	}
	enter_continue();
}

void output(football_teams *teams, football_teams *head, football_teams *tail) {
	if (!teams) {
		puts("queu is empty");
		return;
	}
	int j = 0;
	printf(" _________________________________________________________________________\n");
	printf("|                                                                         |\n");
	printf("|num  team name\t\twins\t\tlosses\t\tdate of last win  |\n");
	printf("|_________________________________________________________________________|\n");
	do {
		printf("|%3d  ", j + 1);
		int temp = 0;
		for (int i = 0; teams->team_name[i] != '\0'; i++) {
			if (temp == 13) {
				printf("\t\t\t\t\t\t\t  |\n|     ");
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
			printf("%d\t\t\t\t  ", teams->odd_data.looses);
		}
		else {
			printf("\t\t%s\t\t  ", teams->odd_data.last_win);
		}
		puts("|\t\t\t");
		printf("|_________________________________________________________________________|\n");
		teams = teams->uk;
		j++;
	} while (teams);
}

void queu_delete(football_teams **teams, football_teams **head, football_teams **tail) {
	int option, j = 0;
	if (!(*teams)) {
		printf("queu empty\n");
		enter_continue();
		return;
	}
	football_teams  *probeg = *teams;
	printf("choose team to delete or enter 0 to go back\n");
	output((*teams), (*head), (*tail));
	checker_num_peacefull(option);
	if (option == 0)
		return;
	(*teams) = (*head);
	for (int i = 0; i < option - 1; i++) {
		probeg = (*teams);
		(*teams) = (*teams)->uk;
		if (!(*teams)) {
			printf("invalid teams\n");
			enter_continue();
			return;
		}
	}
	if ((*head) == (*teams)) {
		int flag = 0;
		if ((*tail) == (*head))
			flag++;
		free((*teams)->team_name);
		(*teams) = (*teams)->uk;
		free((*head));
		(*head) = (*teams);
		if (flag != 0)
			(*tail) = (*head);
	}
	else {
		free((*teams)->team_name);
		if ((*teams) == (*tail))
			(*tail) = probeg;
		probeg->uk = (*teams)->uk;
		free((*teams));
		(*teams) = (*head);
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

void save_data_bin(football_teams *teams, FILE *file, football_teams *head, football_teams *tail) {
	char temp[1];
	int i = 0;
	do {
		while (1) {
			temp[0] = head->team_name[i];
			if (temp[0] == '\0')
				break;
			fwrite(temp, sizeof(char), 1, file);
			i++;
		}
		temp[0] = '~';
		fwrite(temp, sizeof(char), 1, file);

		int number[1];
		number[0] = head->winner_number;
		fwrite(number, sizeof(int), 1, file);
		fwrite(temp, sizeof(char), 1, file);
		number[0] = head->flag;
		fwrite(number, sizeof(int), 1, file);
		fwrite(temp, sizeof(char), 1, file);
		if (number[0] == 2) {
			number[0] = head->odd_data.looses;
			fwrite(number, sizeof(int), 1, file);
			fwrite(temp, sizeof(char), 1, file);
		}
		else {
			int j = 0;
			while (1) {
				temp[0] = head->odd_data.last_win[j];
				if (temp[0] == '\0')
					break;
				fwrite(temp, sizeof(char), 1, file);
				j++;
			}
			temp[0] = '~';
			fwrite(temp, sizeof(char), 1, file);
		}
		fwrite(temp, sizeof(char), 1, file);
		head = head->uk;
	} while (head);
}

void save_data(football_teams *teams, FILE *file, football_teams *head, football_teams *tail) {
	char temp;
	int i = 0;
	do {
		i = 0;
		while (1) {
			temp = head->team_name[i];
			if (temp == '\0')
				break;
			fprintf(file, "%c", temp);
			i++;
		}
		fprintf(file, "~");
		int number = head->winner_number;
		fprintf(file, "%d", number);
		fprintf(file, "~");
		number = head->flag;
		fprintf(file, "%d", number);
		fprintf(file, "~");
		if (number == 2) {
			number = head->odd_data.looses;
			fprintf(file, "%d", number);
			fprintf(file, "~");
		}
		else {
			int j = 0;
			while (1) {
				temp = head->odd_data.last_win[j];
				if (temp == '\0')
					break;
				fprintf(file, "%c", temp);
				j++;
			}
			fprintf(file, "~");
		}
		fprintf(file, "\n");
		head = head->uk;
	} while (head);
}

void save_file(football_teams *teams, football_teams *head, football_teams *tail) {
	if (!teams) {
		printf("your queu is empty!\n");
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
		save_data(teams, file, head, tail);
	else
		save_data_bin(teams, file, head, tail);
	fclose(file);
}

void exporting_bin(football_teams **teams, FILE *file, football_teams **head, football_teams **tail) {
	char temp[1];
	temp[0] = ' ';
	fread(temp, sizeof(char), 1, file);
	if (temp[0] == ' ' || temp[0] == '\n') {
		printf("seems file is empty\n");
		enter_continue();
		return;
	}
	fseek(file, -1, 1);

	football_teams *entering;
	entering = (football_teams*)malloc(sizeof(football_teams));
	entering->uk = nullptr;
	int i = 0;
	entering->team_name = (char*)malloc(sizeof(char));
	while (1) {
		fread(temp, sizeof(char), 1, file);
		if (temp[0] == '~') {
			entering->team_name[i] = '\0';
			break;
		}
		entering->team_name = (char*)realloc(entering->team_name, (i + 2) * sizeof(char));
		entering->team_name[i] = temp[0];
		i++;
	}
	int mas[1];
	fread(mas, sizeof(int), 1, file);
	entering->winner_number = mas[0];

	fread(temp, sizeof(char), 1, file);
	fread(mas, sizeof(int), 1, file);
	entering->flag = mas[0];
	fread(temp, sizeof(char), 1, file);

	if (i == 2) {
		fread(mas, sizeof(int), 1, file);
		entering->odd_data.looses = mas[0];
	}
	else {
		i = 0;
		fread(temp, sizeof(char), 1, file);
		entering->odd_data.last_win[i] = temp[0];
		i++;
		fread(temp, sizeof(char), 1, file);
		entering->odd_data.last_win[i] = temp[0];
		i++;
		fread(temp, sizeof(char), 1, file);
		entering->odd_data.last_win[i] = temp[0];
		i++;
		fread(temp, sizeof(char), 1, file);
		entering->odd_data.last_win[i] = temp[0];
		i++;
		entering->odd_data.last_win[i] = '\0';
	}
	fread(temp, sizeof(char), 1, file);
	if (!(*teams)) {
		(*teams) = entering;
		(*head) = (*teams);
		(*tail) = (*teams);
	}
	else {
		(*tail)->uk = entering;
		(*tail) = (*tail)->uk;
	}
}

void exporting_txt(football_teams **teams, FILE *file, football_teams **head, football_teams **tail) {
	char temp = ' ';
	fscanf_s(file, "%c", &temp);
	if (temp == ' ' || temp == '\n') {
		printf("seems file is empty\n");
		enter_continue();
		return;
	}
	fseek(file, -1, 1);

	football_teams *entering;
	entering = (football_teams*)malloc(sizeof(football_teams));
	entering->uk = nullptr;
	int i = 0;
	entering->team_name = (char*)malloc(1 * sizeof(char));
	while (1) {
		fscanf_s(file, "%c", &temp);
		if (temp == '~') {
			entering->team_name[i] = '\0';
			break;
		}
		entering->team_name = (char*)realloc(entering->team_name, (i + 2) * sizeof(char));
		entering->team_name[i] = temp;
		i++;
	}
	fscanf_s(file, "%d", &i);
	entering->winner_number = i;

	fscanf_s(file, "%c", &temp);
	fscanf_s(file, "%d", &i);
	entering->flag = i;
	fscanf_s(file, "%c", &temp);

	if (i == 2) {
		fscanf_s(file, "%d", &i);
		entering->odd_data.looses = i;
	}
	else {
		i = 0;
		fscanf_s(file, "%c", &temp);
		entering->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp);
		entering->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp);
		entering->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp);
		entering->odd_data.last_win[i] = temp;
		i++;
		entering->odd_data.last_win[i] = '\0';
	}
	fscanf_s(file, "%c", &temp);
	if (!(*teams)) {
		(*teams) = entering;
		(*head) = (*teams);
		(*tail) = (*teams);
	}
	else {
		(*tail)->uk = entering;
		(*tail) = (*tail)->uk;
	}
}

int main() {
	football_teams *teams = nullptr, *head = nullptr, *tail = nullptr;
	int option = 20;
	while (option != 6) {
		print_menu();
		checker_num(option);
		switch (option) {
		case 1: data_add(&teams, &head, &tail); break;
		case 2: output(teams,head,tail); enter_continue(); break;
		case 3: queu_delete(&teams, &head, &tail); break;
		case 4: searching_system(teams); break;
		}
		if (option == 5) {
			system("cls");
			printf("1)save queue in file\n2)export queue from file\n3)go back\n");
			while (option > 3 || option < 1)
				checker_num(option);
			if (option == 1)
				save_file(teams,head,tail);
			if (option == 2) { // preparation for export
				system("cls");
				printf("write file name: ");
				char name[23];  //word cannot be longer than 23 symbols
				rewind(stdin);
				int i = 0, internal_error = 0;
				while (1) { //start writting file name
					if (i == 23) { // bad end
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
				}    //end writting file name
				if (internal_error == 1)
					continue;
				i -= 4;
				if (name[i] != '.') {  //bad end
					printf("cannot find format file\n");
					enter_continue();
					continue;
				}
				i += 3;
				if (name[i] != 't' && name[i] != 'n') {  //bad end
					printf("unknown format\n");
					enter_continue();
					continue;
				}
				FILE *file; //start work with file
				if (fopen_s(&file, name, "r+")) {
					printf("cannot open file\n");
					enter_continue();
					continue;
				}
				if (name[i] == 't') {
					char stop_me = '\n';
					while (1) {
						exporting_txt(&teams, file, &head, &tail);
						fscanf_s(file, "%c", &stop_me); //try to find more information
						fscanf_s(file, "%c", &stop_me);//still trying
						if (stop_me == '\n') //means end
							break;
						else
							fseek(file, -1, 1); //go back and start reading again
					}
				}
				else {
					char stop_me[1];
					stop_me[0] = '~';
					while (1) {
						exporting_bin(&teams, file, &head, &tail);
						fread(stop_me, sizeof(char), 1, file); //try to find more information
						fread(stop_me, sizeof(char), 1, file);//still trying
						if (stop_me[0] == '~') //means end
							break;
						else
							fseek(file, -1, 1); //go back and start reading again
					}
				}
				fclose(file);
			} ///////////////////////////////////////////////////
		}
	}
}