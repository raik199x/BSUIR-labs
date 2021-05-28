#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <malloc.h>
struct files {
	char file_name[11];
	char type[5];
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

void entering_file_name(files *file, int number_file) {
	char temp, *bufer;
	FILE *test;
	int i = 0, problem = 0;
	bufer = (char*)malloc(1 * sizeof(char));
	rewind(stdin);
	while (1) {
		if (problem == 1) {
			rewind(stdin);
			i = 0;
			printf("reseting ended\nwrite name of file\n");
			problem = 0;
		}
		if (i == 11) {
			printf("to many letters (acceptable only <=10)\n");
			printf("reseting\n");
			bufer = (char*)realloc(bufer, 1 * sizeof(char));
			bufer[0] = '\0';
			problem = 1;
			continue;
		}
		scanf_s("%c", &temp);
		if (temp == '\n') {
			if (i == 0) {
				printf("file without name cannot exist\n");
				problem = 1;
				continue;
			}
			bufer[i] = '\0';
			for (int i = 0; i < number_file; i++) {
				if (strcmp(bufer, (file + i)->file_name) == 0) {
					printf("this file is already exist, write another name\n");
					problem = 1;
					break;
				}
			}
			if (!fopen_s(&test, bufer, "w+")) {
				if (problem == 0) {
					break;
				}
			}
			else {
				printf("unexpected error\n");
				problem = 1;
			}
		}
		if (problem == 1)
			continue;
		bufer[i] = temp;
		i++;
		bufer = (char*)realloc(bufer, (i+1) * sizeof(char));
	}
	for (int j = 0; j < i+1; j++) {
		(file + number_file - 1)->file_name[j] = bufer[j];
	}
	fclose(test);
	remove((file + number_file - 1)->file_name);
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

void menu() {
	printf("WW                 WW     EEEEEEEEEEEEEEEEEE     LLL                        CCCCCCCCCCCCC        OOOOOOOOOOOOO           MMM       MMM        EEEEEEEEEEEEEEEEEE\n");
	printf("WW       WWW       WW     EEEEEEEEEEEEEEEEEE     LLL                      CCCCCCCCCCCCCCC      OOOOOOOOOOOOOOOOO         MMM       MMM        EEEEEEEEEEEEEEEEEE\n");
	printf("WW       WWW       WW     EEE                    LLL                     CCCCC                OOOOO         OOOOO       MM MM     MM MM       EEE\n");
	printf(" WW     WW WW     WW      EEE                    LLL                    CCCCC                OOOOO           OOOOO      MM MM     MM MM       EEE\n");
	printf(" WW     WW WW     WW      EEE                    LLL                    CCCC                 OOOO             OOOO     MM   MM   MM   MM      EEE\n");
	printf("  WW   WW   WW   WW       EEEEEEEEEEEEEEE        LLL                    CCCC                 OOOO             OOOO     MM   MM   MM   MM      EEEEEEEEEEEEEEE\n");
	printf("  WW   WW   WW   WW       EEE                    LLL                    CCCC                 OOOO             OOOO    MM     MM MM     MM     EEE\n");
	printf("   WW WW     WW WW        EEE                    LLL                    CCCCC                OOOOO           OOOOO    MM     MM MM     MM     EEE\n");
	printf("   WW WW     WW WW        EEE                    LLL                     CCCCC                OOOOO         OOOOO    MM       MMM       MM    EEE\n");
	printf("    WWW       WWW         EEEEEEEEEEEEEEEEEE     LLLLLLLLLLLLLLLLLL       CCCCCCCCCCCCCCCC     OOOOOOOOOOOOOOOOO     MM       MMM       MM    EEEEEEEEEEEEEEEEEE\n");
	printf("    WWW       WWW         EEEEEEEEEEEEEEEEEE     LLLLLLLLLLLLLLLLLL         CCCCCCCCCCCCCC       OOOOOOOOOOOOO       MM                 MM    EEEEEEEEEEEEEEEEEE\n\n\n");
	printf("1)create file\n2)delete file\n3)show existing files\n4)open file\n5)close programm\n");
}

void create_file(files *file, int decide, int number_file) {
	if (decide == 1) {
		(file + number_file - 1)->type[0] = '.';
		(file + number_file - 1)->type[1] = 't';
		(file + number_file - 1)->type[2] = 'x';
		(file + number_file - 1)->type[3] = 't';
		(file + number_file - 1)->type[4] = '\0';
	}
	else {
		(file + number_file - 1)->type[0] = '.';
		(file + number_file - 1)->type[1] = 'b';
		(file + number_file - 1)->type[2] = 'i';
		(file + number_file - 1)->type[3] = 'n';
		(file + number_file - 1)->type[4] = '\0';
	}
	printf("enter file name (<=10 letters)\n");
	entering_file_name(file, number_file);
	FILE *testing;
	if (decide == 1) {
		if (!fopen_s(&testing, (file + number_file - 1)->file_name, "w+")) {
			printf("successful created!\n");
			enter_continue();
			system("cls");
		}
		else {
			printf("bullshit");
		}
	}
	else {
		if (!fopen_s(&testing, (file + number_file - 1)->file_name, "w+b")) {
			printf("successful created!\n");
			enter_continue();
			system("cls");
		}
		else {
			printf("bullshit");
		}
	}
	fclose(testing);
}

void show_existing(files *file, int number_file) {
	system("cls");
	printf("at that moment you have %d files, here they are:\n", number_file);
	for (int i = 0; i < number_file; i++) {
		printf("%d)%s%s\n", i + 1, (file + i)->file_name, (file + i)->type);
	}
	enter_continue();
	system("cls");
}

void swap(files *file, int first, int second) {
	files *temp;
	temp = (files*)malloc(1 * sizeof(files));
	*temp = *(file + first);
	*(file + first) = *(file + second);
	*(file + second) = *temp;
	free(temp);
}

void delete_file(files *file, int &number_file) {
	printf("choose to delete\n0)go back\n");
	for (int i = 0; i < number_file; i++) {
		printf("%d)%s%s\n", i + 1, (file + i)->file_name, (file + i)->type);
	}
	int option;
	checker_num_peacefull(option);
	if (option == 0)
		return;
	if (option > number_file) {
		printf("invalid number\nexiting\n");
		enter_continue();
		system("cls");
		return;
	}
	rewind(stdin);
	option--;
	for (int i = option; i < number_file-1; i++) {
		swap(file, i, i + 1);
	}
	if (-1 == remove((file + number_file-1)->file_name))
		printf("error\n");
	else
		printf("done!\n");
	number_file--;
	enter_continue();
	system("cls");
}

void txt_add(FILE *txt) {
	system("cls");
	printf("enter:\n\n");
	char bufer;
	rewind(stdin);
	fseek(txt, 0, 2);
	while (1) {
		scanf_s("%c", &bufer);
		fprintf(txt, "%c", bufer);
		if (bufer == '\n') {
			break;
		}
	}
	printf("ended\n");
	enter_continue();
}

void txt_print(FILE *txt) {
	system("cls");
	rewind(txt);
	char bufer;
	while (1) {
		fscanf_s(txt, "%c", &bufer);
		if (feof(txt)) break;
		printf("%c",bufer);
	}
	puts("");
	enter_continue();
}

void txt_output(FILE *txt) {
	system("cls");
	printf("second word is: ");
	int flag = 0;
	char bufer;
	fpos_t l1, l2;
	rewind(txt);
	while (1) {
		fscanf_s(txt, "%c", &bufer);
		if (feof(txt)) break;
		if (bufer != ' ' && flag == 0) {
			flag = 1;
			continue;
		}
		if (flag == 1 && bufer == ' ') {
			flag = 2;
			continue;
		}
		if (flag == 2 && bufer != ' ')
			printf("%c", bufer);
		if (flag == 2 && (bufer == ' ' || bufer == '\n'))
			break;
	}
	puts("");
	rewind(txt); fgetpos(txt, &l2); fseek(txt, -3, 2);
	printf("the word before last: ");
	flag = 0;
	char *reverse;
	int i = 0;
	reverse = (char*)malloc(1 * sizeof(char));
	while (1) {
		fscanf_s(txt, "%c", &bufer);
		if (bufer != ' ' && flag == 0 && bufer!='\n') {
			flag = 1;
			fseek(txt, -2, 1);
			continue;
		}
		if (flag == 1 && bufer == ' ') {
			flag = 2;
			fseek(txt, -2, 1);
			continue;
		}
		if (flag == 2 && bufer != ' ') {
			reverse[i] = bufer;
			i++;
			reverse = (char*)realloc(reverse,(i + 1) * sizeof(char));
		}
		if (flag == 2 && (bufer == ' ' || bufer == '\n'))
			break;
		if (flag != 2 && bufer == '\n')
			break;
		fseek(txt, -2, 1);
		fgetpos(txt, &l1);
		if (l2 == l1) {
			if (flag == 2) {
				fscanf_s(txt, "%c", &bufer);
				reverse[i] = bufer;
				i++;
			}
			break;
		}
	}
	i--;
	for (i; i > -1; i--)
		printf("%c", reverse[i]);
	puts("");
	free(reverse);
	enter_continue();
}

void reverse(FILE *txt, fpos_t pervious, fpos_t second) {
	int dlina = second - pervious;
	char left, right;
	if (dlina % 2 != 0)
		dlina++;
	for (int i = 0; i < dlina / 2; i++) {
		fsetpos(txt, &pervious);   //set first pos
		fscanf_s(txt, "%c", &left); // get first letter
		fsetpos(txt, &second);   //set second pos
		fscanf_s(txt, "%c", &right);  // get second letter
		fseek(txt, -1, 1);     // went were we stay
		fprintf(txt, "%c", left); //change letter
		fsetpos(txt, &pervious); // went on left side (nedeeed pose)
		fprintf(txt, "%c", right); //change letter
		second--;
		pervious++;
	}
}

void txt_change(FILE *txt) {
	fpos_t l_start, l_end, s_start, s_end, temp;
	rewind(txt);
	int dlina = 0, max = 0, min = 0, flag = 0;
	char current;
	while (!feof(txt)) {
		fscanf_s(txt, "%c", &current);
		if (feof(txt) || current == '\n')
			break;
		if (current != ' ') {
			dlina++;
			if (flag == 0) {
				fseek(txt, -1, 1);
				fgetpos(txt, &temp);
				fseek(txt, 1, 1);
				flag = 1;
			}
			continue;
		}
		else {
			if (dlina > max) {
				fseek(txt, -2, 1);
				fgetpos(txt, &l_end);
				fseek(txt, 2, 1);
				l_start = temp;
				max = dlina;
			}
			if (dlina < min || min == 0) {
				fseek(txt, -2, 1);
				fgetpos(txt, &s_end);
				fseek(txt, 2, 1);
				min = dlina;
				s_start = temp;
			}
			dlina = 0;
			flag = 0;
		}
	}
	if (dlina != 0) {
		if (dlina > max) {
			fseek(txt, -3, 1);
			fgetpos(txt, &l_end);
			fseek(txt, 3, 1);
			l_start = temp;
			max = dlina;
		}
		if (dlina < min || min == 0) {
			fseek(txt, -3, 1);
			fgetpos(txt, &s_end);
			fseek(txt, 3, 1);
			min = dlina;
			s_start = temp;
		}
		dlina = 0;
		flag = 0;
	}
	if (max == min) {
		printf("words length is equal, exiting\n");
		enter_continue();
		return;
	}
	fpos_t start, end;
	if (l_end < s_start) {
		start = l_start;
		end = s_end;
		reverse(txt, start, end);
	}
	else {
		start = s_start;
		end = l_end;
		reverse(txt, start, end);
	}
	fsetpos(txt, &start);
	fpos_t current_temp, start_temp = start;
	for (current_temp = start; current_temp <= end+1; current_temp++) {
		fsetpos(txt, &current_temp);
		fscanf_s(txt, "%c", &current);
		if (current == '\n' || current == ' ') {
			temp = current_temp - 1;
			reverse(txt, start_temp, temp);
			flag = 0;
		}
		if (current != ' ' && flag == 0) {
			fgetpos(txt, &start_temp);
			start_temp--;
			flag = 1;
		}
	}
	enter_continue();
}

void work_with_txt(files *file, int option) {
	system("cls");
	FILE *txt;
	if (!fopen_s(&txt, (file + option)->file_name, "r+")) {
		//printf("opened\n");
	}
	else { printf("bullshit happens\n"); return; }
	int temp;
	while (1) {
		printf("TTTTTTTT X     X TTTTTTTT\n   TT     X   X     TT  \n   TT      X X      TT  \n   TT       X       TT  \n   TT       X       TT  \n   TT      X X      TT  \n   TT     X   X     TT  \n   TT    X     X    TT  \n");
		printf("choose option:\n1)add to file\n2)print file\n3)tools\n4)close file\n");
		checker_num(temp);
		if (temp >= 4)
			break;
		if (temp == 1) {
			txt_add(txt);
			continue;
		}
		if (temp == 2) {
			txt_print(txt);
			continue;
		}
		if (temp == 3) {
			system("cls");
			printf("choose option:\n1)output second and before last word\n2)change position of longest and shortest word\n3)go back\n");
			checker_num(temp);
			if (temp >= 3) continue;
			if (temp == 1) txt_output(txt);
			if (temp == 2) txt_change(txt);
		}
	}
	fclose(txt);
}

void add_bin(FILE *bin) {
	system("cls");
	printf("entering number\nprintf letter to stop\n");
	int temp, number[1];
	fseek(bin, 0, 2);
	while (1) {
		temp = scanf_s("%d", &number[0]);
		if (!temp) {
			rewind(stdin);
			break;
		}
		fwrite(number, sizeof(int), 1, bin);
	}
	printf("entreing is done\n");
	enter_continue();
}

void printf_bin(FILE *bin) {
	system("cls");
	int j;
	rewind(bin);
	do{
	    fread(&j, sizeof(int), 1, bin);
		if(feof(bin))  break;
		printf("%d ", j);
	} while (1);
	puts("");
	enter_continue();
}

void zero_br(FILE *bin) {
	fpos_t l1,l2;
	fseek(bin, 0, 2);
	fgetpos(bin, &l2);
	rewind(bin);
	int j = 0, buffer[1];
	buffer[0] = 0;
	while (1) {
		fgetpos(bin, &l1);
		if (l1 == l2)
			break;
		if (j % 2 == 0) {
			fwrite(buffer, sizeof(int), 1, bin);
			j++;
		}
		else {
			fseek(bin, sizeof(int), 1);
			j++;
		}
	}
	printf("operation done\n");
	enter_continue();
}

void query(FILE *bin) {
	int current_num,current_size=1,last, max = 1;
	rewind(bin);
	int flag = 0;
	while (1) {
		fread(&current_num, sizeof(int), 1, bin);
		if (feof(bin)) {
			break;
		}
		if (flag == 0) {
			last = current_num;
			current_size--;
			flag++;
		}
		if (current_num != last) {
			if (max < current_size)
				max = current_size;
			current_size = 1;
			last = current_num;
		}
		else {
			current_size++;
		}
	}
		if (max < current_size)
			max = current_size;
	printf("longest is: %d\n", max);
	enter_continue();
}

void work_with_bin(files *file, int option) {
	system("cls");
	FILE *bin;
	if (!fopen_s(&bin,(file+option)->file_name,"r+b")) {
		//printf("opened\n");
	}
	else { printf("bullshit happens\n"); return; }
	int temp;
	while (1) {
		printf("BBBB  IIIIII N    N\nB   B   II   NN   N\nB   B   II   N N  N\nBBBB    II   N N  N\nBBBB    II   N  N N\nB   B   II   N  N N\nB   B   II   N   NN\nBBBB  IIIIII N    N\n");
		printf("current opened file: %s%s\n", (file + option)->file_name, (file + option)->type);
		printf("1)add in file\n2)tools\n3)print file\n4)close file\n");
		checker_num(temp);
		if (temp == 1) {
			add_bin(bin);
			continue;
		}
		if (temp == 2) {
			system("cls");
			printf("choose option:\n1)0 makes go brrrrr\n2)Find the longest sequence of equal elements\n3)go back\n");
			checker_num(temp);
			if (temp >= 3) continue;
			if (temp == 1) zero_br(bin);
			if (temp == 2) query(bin);
		}
		if (temp == 3) {
			printf_bin(bin);
			continue;
		}
		if (temp >= 4)
			break;
	}
	fclose(bin);
}

int main() {
	files *file;
	FILE *remember_files;
	int option, number_file=0, test;
    test = fopen_s(&remember_files, "remember_files.txt", "r+");
	if (test != 0) {
		fopen_s(&remember_files, "remember_files.txt", "w+");
		fprintf(remember_files, "%d\n", number_file);
		fclose(remember_files);
	}
	else {
		fclose(remember_files);
	}
	fopen_s(&remember_files, "remember_files.txt", "r+");
	fscanf_s(remember_files, "%d\n", &number_file);
	if (number_file == 0) {
		file = (files*)malloc(1 * sizeof(files));
	}
	else {
		char temp = '-';
		file = (files*)malloc(number_file * sizeof(files));
		for (int i = 0; i < number_file; i++) {
			int j = 0;
			while (1) {
				fscanf_s(remember_files, "%c", &temp);
				if (temp == '\n') { (file + i)->file_name[j] = '\0';  break; }
			    (file + i)->file_name[j] = temp;
				j++;
			}
			j = 0;
			while (1) {
				fscanf_s(remember_files, "%c", &temp);
				if (temp == '\n') { (file + i)->type[j] = '\0'; break; }
				(file + i)->type[j] = temp;
				j++;
			}
			j = 0;
		}
	}
	fclose(remember_files);
	while (1) {
		system("cls");
		menu();
		checker_num(option);
		if (option >= 5)
			break;
		if (option == 1) {
			system("cls");
			int decide;
			printf("1)txt\n2)bin\n3)go back\n");
			checker_num(decide);
			if (decide >= 3)
				continue;
			number_file++;
			file = (files*)realloc(file, number_file * sizeof(files));
			create_file(file,decide,number_file);
		}
		if (option == 2) {
			delete_file(file, number_file);
			file = (files*)realloc(file, number_file * sizeof(files));
		}
		if (option == 3)
			show_existing(file,number_file);
		if (option == 4) {
			printf("choose file:\n0)go back\n", number_file);
			for (int i = 0; i < number_file; i++) {
				printf("%d)%s%s\n", i + 1, (file + i)->file_name, (file + i)->type);
			}
			checker_num_peacefull(option);
			if (option == 0)
				continue;
			if (option > number_file) {
				printf("invalid number\nexiting\n");
				continue;
			}
			option--;
			if ((file + option)->type[1] == 't') {
				work_with_txt(file,option);
			}
			else {
				work_with_bin(file,option);
			}
		}
	}
	if (number_file > 0) {
		fopen_s(&remember_files, "remember_files.txt", "r+");
		fprintf(remember_files, "%d\n", number_file);
		for (int i = 0; i < number_file; i++) {
			fprintf(remember_files, "%s\n%s\n", (file + i)->file_name, (file + i)->type);
		}
		fclose(remember_files);
	}
	else {
		fopen_s(&remember_files, "remember_files.txt", "w+");
		fprintf(remember_files, "%d\n", number_file);
		fclose(remember_files);
	}
}