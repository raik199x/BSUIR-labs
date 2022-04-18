#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//returns pointer to a new writte line
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

//returns number that located int the end of line or 0 if no number where found
int ParseIntFromEnd(char *line){
    //find end of line
    int end_line;
    for(int i = 0; line[i] != '\0';i++)
        end_line = i;
    int return_int = 0; //converting;
    int pos = 0, temp;
    for(int i = end_line; i > 0; i--){
        if((line[i] >= '0' && line[i] <='9') || line[i] == '-'){
            if(line[i] == '-'){
                return_int*=-1;
                return return_int;
            }
            temp = line[i]-48;
            for(int j = 0;j < pos; j++)
                temp*=10;
            pos++;
            return_int+=temp;
        } else
            break;
    }
    return return_int;
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