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
	football_teams *left, *right;
};

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

void print_menu() {
	system("cls");
	printf("What do you want to do?\n");
	printf("1. Add to circle\n");
	printf("2. Print all circle\n");
	printf("3. delete iteam from circle\n");
	printf("4. searching system\n");
	printf("5. file menu\n");
	printf("6. Exit\n");
	printf(">");
}

char* entering_team_name() {
	char temp, *bufer;
	int i = 0, n = 1;
	bufer = (char*)malloc(1 * sizeof(char));
	rewind(stdin);                                                      //free console bufer
	while (1) {
		scanf_s("%c", &temp,1);
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

void enter_date(football_teams **new_block) {
	rewind(stdin);
	char temp;
	for (int i = 0; i < 5; i++) {
		scanf_s("%c", &temp,1);
		if ((temp < '0' || temp > '9') && temp != '\n') {
			printf("letter detected, reset\n");
			i = -1;
			rewind(stdin);
			continue;
		}
		if (temp == '\n' && i != 4) {
			printf("number consist 4 numbers, reset\n");
			i = -1;
			rewind(stdin);
			continue;
		}
		else if (temp =='\n' && i == 4){
			(*new_block)->odd_data.last_win[i] = '\0';
			return;
		}
		if (i == 4) {
			printf("number consist 4 numbers, reset\n");
			i = -1;
			rewind(stdin);
			continue;
		}
		(*new_block)->odd_data.last_win[i] = temp;
	}
}

void make_circle(football_teams **circle) {
	system("cls");
	int numbers;
	football_teams *new_block;
	new_block = (football_teams*)malloc(sizeof(football_teams));
	//start to add infromation**************************************************
	printf("enter team name: ");
	new_block->team_name = entering_team_name();
	puts("");
	printf("enter amount of wins: ");
	checker_num_peacefull(numbers);
	new_block->winner_number = numbers;
	puts("");
	printf("what you want to keep?\n1)date of last win\n2)amount of looses\n");
	while (1) {                                                         //choosing
		checker_num(numbers);
		if (numbers > 2 || numbers < 0) {
			printf("invalid number\n");
			continue;
		}
		new_block->flag = numbers;
		break;
	}                                                             //end of choosing
	if (numbers == 2) {                                          //entering looses
		printf("enter count of losses: ");
		checker_num_peacefull(numbers);
		new_block->odd_data.looses = numbers;
	}
	else {
		printf("enter date of last win: ");
		enter_date(&new_block);
	}
	new_block->right = nullptr;
	new_block->left = nullptr;
	//end to fill up information block********************************************

	if ((*circle) == nullptr) {
		printf("cricle succesfully created!\n");
		(*circle) = new_block;
		(*circle)->right = (*circle); (*circle)->left = (*circle);
		system("pause");
		return;
	}
	int side = 4;
	printf("choose your side!\n1)add block on top\n2)add block on bottom\n>");
	while (side <= 0 || side > 2) {
		checker_num(side);
		if (side > 2)
			printf("invalid option\n");
	}
	if (side == 1) {
		new_block->right = (*circle);
		new_block->left = (*circle)->left;
		(*circle)->left->right = new_block;
		(*circle)->left = new_block;
		(*circle) = new_block;
		printf("cricle succesfully created!\n");
		system("pause");
	}
	else {
		new_block->right = (*circle);
		new_block->left = (*circle)->left;
		(*circle)->left->right = new_block;
		(*circle)->left = new_block;
		printf("cricle succesfully created!\n");
		system("pause");
	}
}

int output(football_teams *circle) {
	if (!circle) {
		puts("circle is empty");
		return 0;
	}
	int side = 4;
	printf("how you want to show circle?\n1)as queu\n2)as stack\n>");
	while (side <= 0 || side > 2) {
		checker_num(side);
		if (side > 2)
			printf("invalid option\n");
	}
	if (side == 2)
		circle = circle->left;
	football_teams *end = circle;
	int j = 0;
	printf(" _________________________________________________________________________\n");
	printf("|                                                                         |\n");
	printf("|num  team name\t\twins\t\tlosses\t\tdate of last win  |\n");
	printf("|_________________________________________________________________________|\n");
	do {
		printf("|%3d  ", j + 1);
		int temp = 0;
		for (int i = 0; circle->team_name[i] != '\0'; i++) {
			if (temp == 13) {
				printf("\t\t\t\t\t\t\t  |\n|     ");
				temp = 0;
			}
			temp++;
			printf("%c", circle->team_name[i]);
		}
		while (temp != 13) {
			temp++;
			printf(" ");
		}
		printf("\t");
		printf("%d\t\t", circle->winner_number);
		if (circle->flag == 2) {
			printf("%d\t\t\t\t  ", circle->odd_data.looses);
		}
		else {
			printf("\t\t%s\t\t  ", circle->odd_data.last_win);
		}
		puts("|\t\t\t");
		printf("|_________________________________________________________________________|\n");
		if (side == 1) {
			circle = circle->right;
			if (circle == end)
				break;
		}
		else {
			circle = circle->left;
			if (circle == end)
				break;
		}
		j++;
	} while (1);
	return side;
}

void delete_object(football_teams **circle) {
	system("cls");
	if (!(*circle)) {
		printf("maybe you want to try to add something?\n");
		system("pause");
		return;
	}
	printf("Prepearing for delete\n");
	football_teams *bufer = (*circle);
	int side = output(bufer), kill;
	printf("choose object to delete: ");
	checker_num(kill);
	if (side == 2)
		bufer = bufer->left;
	football_teams *end = bufer;
	for (int i = 1; i < kill; i++) {
		if (side == 1) {
			bufer = bufer->right;
			if (bufer == end) {
				printf("invalid object\n");
				system("pause");
				return;
			}
		}
		else {
			bufer = bufer->left;
			if (bufer == end) {
				printf("invalid object\n");
				system("pause");
				return;
			}
		}
	}
	if ((*circle)->right == (*circle)) {
		free((*circle)->team_name);
		free((*circle));
		(*circle) = nullptr;
		return;
	}
	else if ((*circle) == bufer) {
		(*circle) = (*circle)->right;
	}
	free(bufer->team_name);
	bufer->right->left = bufer->left;
	bufer->left->right = bufer->right;
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
		scanf_s("%c", &temp,1);
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
		system("pause");
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
	football_teams *end = teams;
	do {
		while (1) {
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
		teams = teams->right;
		if (stop_me != 0 || !teams) {
			free(search);
			printf("\n\nsearching ended\n");
			system("pause");
			system("cls");
			return;
		}
	} while (teams != end);
	printf("searching ended\n");
	system("pause");
	free(search);
}

void searching_wins(football_teams *teams) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	football_teams *end = teams;
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
		teams = teams->right;
	} while (teams != end);
	printf("searching ended\n");
	system("pause");
	system("cls");
}

void searching_losses(football_teams *teams) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	football_teams *end = teams;
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
		teams = teams->right;
	} while (teams != end);
	printf("searching ended\n");
	system("pause");
	system("cls");
}

void searching_lastWin(football_teams *teams) {
	int counter, depth;
	system("cls");
	printf("enter number\n");
	checker_num_peacefull(counter);
	printf("enter depth (+-)\n");
	checker_num_peacefull(depth);
	football_teams *end = teams;
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
		teams = teams->right;
	} while (teams != end);
	printf("searching ended\n");
	system("pause");
	system("cls");
}

void searching_system(football_teams *circle) {
	system("cls");
	if (!circle) {
		printf("maybe you should try to add something?\n");
		system("pause");
		return;
	}
	printf("what do you want to search?\n0)go back\n1)team name\n2)wins\n3)losses\n4)date of last win\n");
	int option;
	checker_num_peacefull(option);
	switch (option) {
	case 0: system("cls"); return;
	case 1: searching_teamName(circle); return;
	case 2: searching_wins(circle); return;
	case 3: searching_losses(circle); return;
	case 4: searching_lastWin(circle); return;
	}
}

void save_data_bin(football_teams *teams, FILE *file) {
	char temp[1];
	int i = 0;
	football_teams *end = teams;
	do {
		while (1) {
			temp[0] = teams->team_name[i];
			if (temp[0] == '\0')
				break;
			fwrite(temp, sizeof(char), 1, file);
			i++;
		}
		temp[0] = '~';
		fwrite(temp, sizeof(char), 1, file);

		int number[1];
		number[0] = teams->winner_number;
		fwrite(number, sizeof(int), 1, file);
		fwrite(temp, sizeof(char), 1, file);
		number[0] = teams->flag;
		fwrite(number, sizeof(int), 1, file);
		fwrite(temp, sizeof(char), 1, file);
		if (number[0] == 2) {
			number[0] = teams->odd_data.looses;
			fwrite(number, sizeof(int), 1, file);
			fwrite(temp, sizeof(char), 1, file);
		}
		else {
			int j = 0;
			while (1) {
				temp[0] = teams->odd_data.last_win[j];
				if (temp[0] == '\0')
					break;
				fwrite(temp, sizeof(char), 1, file);
				j++;
			}
			temp[0] = '~';
			fwrite(temp, sizeof(char), 1, file);
		}
		fwrite(temp, sizeof(char), 1, file);
		teams = teams->right;
		i = 0;
	} while (teams != end);
}

void save_data(football_teams *teams, FILE *file) {
	char temp;
	int i = 0;
	football_teams *end = teams;
	do {
		i = 0;
		while (1) {
			temp = teams->team_name[i];
			if (temp == '\0')
				break;
			fprintf(file, "%c", temp);
			i++;
		}
		fprintf(file, "~");
		int number = teams->winner_number;
		fprintf(file, "%d", number);
		fprintf(file, "~");
		number = teams->flag;
		fprintf(file, "%d", number);
		fprintf(file, "~");
		if (number == 2) {
			number = teams->odd_data.looses;
			fprintf(file, "%d", number);
			fprintf(file, "~");
		}
		else {
			int j = 0;
			while (1) {
				temp = teams->odd_data.last_win[j];
				if (temp == '\0')
					break;
				fprintf(file, "%c", temp);
				j++;
			}
			fprintf(file, "~");
		}
		fprintf(file, "\n");
		teams = teams->right;
	} while (teams != end);
}

void save_file(football_teams *teams) {
	if (!teams) {
		printf("your circle is empty!\n");
		system("pause");
		return;
	}
	FILE *file;
	int option = 0;
	char *name, temp;
	printf("enter file name (up to 20 symbols, without format): ");
	int n = 0;
	name = (char*)malloc(sizeof(char));
	rewind(stdin);
	while (1) {
		scanf_s("%c", &temp,1);
		if (temp == '\n') {
			name[n] = '\0';
			if (fopen_s(&file, name, "w+")) {
				printf("error to create file\n");
				n = 22;
				system("pause");
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

void exporting_txt(football_teams **teams, FILE *file) {
	char temp = ' ';
	fscanf_s(file, "%c", &temp,1);
	if (temp == ' ' || temp == '\n') {
		printf("seems file is empty\n");
		system("pause");
		return;
	}
	fseek(file, -1, 1);

	football_teams *entering;
	entering = (football_teams*)malloc(sizeof(football_teams));
	int i = 0;
	entering->team_name = (char*)malloc(1 * sizeof(char));
	while (1) {
		fscanf_s(file, "%c", &temp,1);
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

	fscanf_s(file, "%c", &temp,1);
	fscanf_s(file, "%d", &i);
	entering->flag = i;
	fscanf_s(file, "%c", &temp,1);

	if (i == 2) {
		fscanf_s(file, "%d", &i);
		entering->odd_data.looses = i;
	}
	else {
		i = 0;
		fscanf_s(file, "%c", &temp,1);
		entering->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp,1);
		entering->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp,1);
		entering->odd_data.last_win[i] = temp;
		i++;
		fscanf_s(file, "%c", &temp,1);
		entering->odd_data.last_win[i] = temp;
		i++;
		entering->odd_data.last_win[i] = '\0';
	}
	fscanf_s(file, "%c", &temp,1);
	if ((*teams) == nullptr) {
		(*teams) = entering;
		(*teams)->right = (*teams); (*teams)->left = (*teams);
		return;
	}
	else {
		entering->right = (*teams);
		entering->left = (*teams)->left;
		(*teams)->left->right = entering;
		(*teams)->left = entering;
		return;
	}
}

void exporting_bin(football_teams **teams, FILE *file) {
	char temp[1];
	temp[0] = ' ';
	fread(temp, sizeof(char), 1, file);
	if (temp[0] == ' ' || temp[0] == '\n') {
		printf("seems file is empty\n");
		system("pause");
		return;
	}
	fseek(file, -1, 1);

	football_teams *entering;
	entering = (football_teams*)malloc(sizeof(football_teams));
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
	if ((*teams) == nullptr) {
		(*teams) = entering;
		(*teams)->right = (*teams); (*teams)->left = (*teams);
		return;
	}
	else {
		entering->right = (*teams);
		entering->left = (*teams)->left;
		(*teams)->left->right = entering;
		(*teams)->left = entering;
		return;
	}
}

football_teams *file_system(football_teams *circle) {
	int option = 5;
	system("cls");
	printf("1)save circle in file\n2)export circle from file\n3)go back\n");
	while (option > 3 || option < 1)
		checker_num(option);
	if (option == 1)
		save_file(circle);
	if (option == 2) { // preparation for export
		system("cls");
		printf("write file name: ");
		char name[23];  //word cannot be longer than 23 symbols
		rewind(stdin);
		int i = 0, internal_error = 0;
		while (1) { //start writting file name
			if (i == 23) { // bad end
				printf("this file cannot be readen");
				system("pause");
				internal_error = 1;
				break;
			}
			scanf_s("%c", &name[i],1);
			if (name[i] == '\n') {
				name[i] = '\0';
				break;
			}
			i++;
		}    //end writting file name
		if (internal_error == 1)
			return circle;
		i -= 4;
		if (name[i] != '.') {  //bad end
			printf("cannot find format file\n");
			system("pause");
			return circle;
		}
		i += 3;
		if (name[i] != 't' && name[i] != 'n') {  //bad end
			printf("unknown format\n");
			system("pause");
			return circle;
		}
		FILE *file; //start work with file
		if (fopen_s(&file, name, "r+")) {
			printf("cannot open file\n");
			system("pause");
			return circle;
		}
		if (name[i] == 't') {
			char stop_me = '\n';
			while (1) {
				exporting_txt(&circle, file);
				fscanf_s(file, "%c", &stop_me,1); //try to find more information
				fscanf_s(file, "%c", &stop_me,1);//still trying
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
				exporting_bin(&circle, file);
				fread(stop_me, sizeof(char), 1, file); //try to find more information
				fread(stop_me, sizeof(char), 1, file);//still trying
				if (stop_me[0] == '~') //means end
					break;
				else
					fseek(file, -1, 1); //go back and start reading again
			}
		}
		fclose(file);
		return circle;
	}
	return circle;
}

int main() {
	football_teams *circle;
	circle = nullptr;
	while (1) {
		system("cls");
		rewind(stdin);
		int option;
		print_menu();
		checker_num(option);
		switch (option) {
		case 1: make_circle(&circle); break;
		case 2: output(circle); system("pause"); break;
		case 3: delete_object(&circle); break;
		case 4: searching_system(circle); break;
		case 5: circle = file_system(circle); break;
		default: return 0;
		}
	}
}