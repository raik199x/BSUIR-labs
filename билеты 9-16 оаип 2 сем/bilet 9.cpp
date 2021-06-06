// Bilet 9
// Made by AlexanderXD
// team sessia na 8+

#include <stdio.h>
#include <malloc.h>
#include <Windows.h>

//####################################################################################################################

////эта функция вам нахер не сдалась, объяснять не буду
//void file_add(FILE* bin) {
//	system("cls");
//	printf("entering number\nprintf letter to stop\n");
//	int temp, number[1];
//	fseek(bin, 0, 2);
//	rewind(stdin);
//	while (1) {
//		temp = scanf_s("%d", &number[0]);
//		if (!temp) {
//			rewind(stdin);
//			break;
//		}
//		fwrite(number, sizeof(int), 1, bin);
//	}
//	printf("entreing is done\n");
//	system("pause");
//}
////эта функция нахер не сдалась
//void file_print(FILE* bin) {
//	system("cls");
//	int j;
//	rewind(bin);
//	do {
//		fread(&j, sizeof(int), 1, bin);
//		if (feof(bin))  break;
//		printf("%d ", j);
//	} while (1);
//	puts("");
//	system("pause");
//}
//// нужная функция
//void bullshit_happens(FILE* bin) {
//	//единственная проблема, которая может возникнуть - это опеределение последнего числа (ну надеюсь вы поняли)
//	int swap1[1], swap2[1]; //почему инты массивы? а блять бинарка с обычными не хочет работать
//	fpos_t pos1 = 0;
//	rewind(bin); //ну а кто знает в каком месте файла мы находимся
//	do {
//		//собираем значения
//		fgetpos(bin, &pos1);   // вы наверное спросите, нахуя я взял и поставил текущее положение?
//		fsetpos(bin, &pos1);  // ответ прост, бля я сам в ахуе, просто без этой хуйни бинарка отказывается работать
//		                     // но какое логическое объяснение я придумал
//		                    // фпоз_т хранит числовое значения, а бинарка передвигается по адресам
//		                   // этими двумя командами мы синхронизируем их позиции и чтение из файла не ломается
//		                  // можете пробежаться дебагерром или просто стереть fsetpos всё полетит к хуям
//		//тут всё просто, взял число 1 и 2 (если я не возьму хоть одно число значит я в конце файла и пока пока)
//		fread(&swap1[0], sizeof(int), 1, bin);
//		if (feof(bin))  break;
//		fread(&swap2[0], sizeof(int), 1, bin);
//		if (feof(bin))  break;
//
//		//ставим значения (просто переписываю значения)
//		fsetpos(bin, &pos1);  //возвращаюсь откуда начал читать
//		fwrite(&swap2[0], sizeof(int), 1, bin);
//		fwrite(&swap1[0], sizeof(int), 1, bin);
//	} while (1);
//	printf("done!\n");
//	system("pause");
//}
//
//int main() {
//	FILE* bin; // мой указатель на файлик
//	//конечно мы могли сделать ввод от руки и всё такое, но нам же не это важно, а сама реализация, так что
//	if (fopen_s(&bin, "my_bin.bin", "r+"))  //проверяем создан ли файлик
//		fopen_s(&bin, "my_bin.bin", "w+"); //если нет, то создаем
//	int option = 0, proverka;
//	while (1) {   //меню, на экзе оно нахер будет нужно, но у нас оно будет
//		system("cls");
//		rewind(stdin);
//		printf("1. Add to file\n2. Reset file\n3. Do the task\n4. output\n>");
//		// ставлю деньги, что по условию в файле уже будут какие-то данные
//	   //но мне не лень, добавлю функцию ввода
//		proverka = scanf_s("%d", &option);  // конечно же не забываем мои любимые проверочки
//		if (!proverka)
//			return 0;
//		switch (option){
//		case 1:  file_add(bin); break;
//		case 2:  fclose(bin); fopen_s(&bin, "my_bin.bin", "w+"); printf("reseted!\n"); system("pause"); break; //ну типо лол и так всё понятно
//		case 3:  bullshit_happens(bin); break; //та самая функция
//		case 4:  file_print(bin); break;
//		default: return 0;
//		}
//	}
//}

//####################################################################################################################
union razmer {
	int radius;
	int perimetr;
};

struct figura {
	char* name;
	int flag;
	razmer sm;
};
  //со структурами\объединениями вроде как всё понятно, есть только один вопрос
 // мы должны в названии просить только треугольник или сфера или просить указывать что именно хранить (радиус или периметр)?
// в этом решеении мы будем спрашивать чем является фигура, для этого в структуру добавили поле flag

char* entering_name() {
	char temp, *bufer;
	int i = 0, n = 1;
	bufer = (char*)malloc(1 * sizeof(char));
	rewind(stdin);                                                      //free console bufer
	while (1) {
		scanf_s("%c", &temp);
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

void ploshad_sphere(figura *something, int i) {
	 //единственное что я хз надо или не надо было делать, это выводить площадь в флоате (ибо там есть п)
	// но я пошел другим, хитрым путем
	printf("name of figure: %s\n square: %d*3.14\n", (something + i)->name, (something + i)->sm.radius * (something + i)->sm.radius);
	return;
}

void ploshad_rectangular(figura *something,int i) {
	 //тут прикола я не понял, или я тупой, или как из периметра ПРЯМОУГОЛЬНИКА вывести площадь я хз
	//представим что у нас не прямоугольник, а квадрат, тогда
	printf("name of figure: %s\n square: %d\n", (something + i)->name, (something + i)->sm.perimetr / 4 * (something + i)->sm.perimetr);
	return;
}

int main() {
	figura *something; //берем десять ибо нам по-любому надо ввести их 10 штук
	something = (figura*)malloc(10 * sizeof(figura));
	//функция ввода
	for (int i = 0; i < 10; i++) {
		printf("enter name of figure: ");
		(something+i)->name = entering_name(); //забираем название, я думаю на экзе можно будет заменить на fgets
		printf("\nIs it sphere or rectangular?\n1. sphere\n2. rectangular\n");
		int flag, temp = scanf_s("%d", &flag);
		while (!temp || temp < 1 || temp > 2) { //берешь и обмазываешься проверочками
			printf("invalid number!\n");
			temp = scanf_s("%d", &flag);
		}
		(something + i)->flag = flag;
		if (flag == 1) {
			printf("enter radius: ");
            temp = scanf_s("%d", &flag); 
			while (!temp) { //просто. брать. и. обмазываться.
				printf("error corrupted!\n");
				temp = scanf_s("%d", &flag);
			}
			(something + i)->sm.radius = flag;
		}
		else {
			printf("enter perimetr: ");
			temp = scanf_s("%d", &flag);
			while (!temp) { //просто. брать. и. обмазываться.
				printf("error corrupted!\n");
				temp = scanf_s("%d", &flag);
			}
			(something + i)->sm.perimetr = flag;
		}
	}
	//теперь определяем площади
	for (int i = 0; i < 10; i++) {
		if ((something + i)->flag == 1)
			ploshad_sphere(something,i);
		else
			ploshad_rectangular(something,i);
	}
}

//####################################################################################################################