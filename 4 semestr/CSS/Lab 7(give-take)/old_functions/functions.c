#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

//returns pointer to a new written line
char* EnterLineReturn(){
    char *line;
    int line_size = 0, letter_num = 0; char letter;
    line = (char*)malloc(1*sizeof(char));
    while(1){
        scanf("%c", &letter);
        if(letter == '\0')
            continue;
        else if (letter == '\n'){
            line[letter_num] = '\0';
            break;
        }
        line[letter_num] = letter;
        line_size+=2; letter_num++;
        line = (char*)realloc(line, line_size);
    }
    return line;
}

//return array of ints that was parsed from line, last array member will be -1 as end point
//returns values from end, ex: line="0up d1s k100" return="100 1 0"
int* ParseInt(char *line){
    //find end of line
    int end_line, pos = 0, temp, array_pos = 0, return_int = 0; int* return_array;
    for(int i = 0; line[i] != '\0';i++)
        end_line = i;

    return_array = (int*)malloc(sizeof(int));
    return_array[0] = -1;

    int checker = -1;
    for(int i = end_line; i >= 0; i--){
        if(line[i] >= '0' && line[i] <='9'){
            temp = line[i]-48;
            for(int j = 0;j < pos; j++)
                temp*=10;
            pos++;
            return_int+=temp;
            checker = 0;
            if (i == 0)
                checker = 1;
        } else if (line[i] == '-'){
            return_int*=-1;
            checker = 1;
            if(return_int == 0)
                checker = 0;
        } else
            if(checker == 0)
                checker = 1;
        if(checker == 1){
            return_array[array_pos] = return_int;
            array_pos++;
            return_array = (int*)realloc(return_array,(array_pos+1)*sizeof(int));
            return_int = 0;
            checker = -1;
            pos = 0;
            return_array[array_pos] = -1;
        }
    }
    return return_array;
}

//set output to specified color, if no color provided set default
void SetOutputColor(char *color){
    if (strcmp(color,"RED") == 0)
        printf("\033[1;31m");
    else if (strcmp(color,"GREEN") == 0)
        printf("\033[0;32m");
    else if (strcmp(color,"BLUE") == 0)
        printf("\033[0;34m");
    else if (strcmp(color,"YELLOW") == 0)
        printf("\033[0;33m");
    else
        printf("\033[9;39;49;0m");
}

//simple confirmation message
int AreYouSure(){
    rewind(stdin);
    printf("Are you sure? [Y\\N]\n");
    char *line = EnterLineReturn();
    if(line[0] == 'Y' || line[0] == 'y'){
        free(line);
        return 1;
    }
    else{
        free(line);
        return 0;
    }
}

//simple pause message
void PressEnterContinue(){
    rewind(stdin);
    printf("Press enter to continue");
    char *line = EnterLineReturn();
    free(line);
    return;
}

// "123\0" -> 123, returns value
int CharToInt(char *line){
    int result = 0, runner = 0, neg_flag = 0;
    while(line[runner] != '\0'){
        if(line[runner] == '-'){
            neg_flag = 1;
            runner++;
            continue;
        }
        if(line[runner > '9' || line[runner] < '0']){
            printf("Char To Int error, symbols in line\n");
            return 0;
        }
        result = result * 10 + line[runner]-48;
        runner++;
    }
    if(neg_flag != 0)
        result*=-1;
    return result;
}

// 123 -> "123\0"
char* IntToChar(int num){
    char *line = (char*)malloc(sizeof(char)); int size = 0;
    int rev_x = 0;
    if(num < 0){
        line[0] = '-';
        line = (char*)realloc(line,2*sizeof(char));
        size++; rev_x++;
    }
    while(num != 0){
        char temp = num%10+48;
        num/=10;
        line[size] = temp;
        size++;
        line = (char*)realloc(line,(size+2)*sizeof(char));
    }
    line[size] = '\0';
    size--;
    for(;rev_x < size;rev_x++,size--){
        char temp = line[rev_x];
        line[rev_x] = line[size];
        line[size] = temp;
    }
    return line;
}

