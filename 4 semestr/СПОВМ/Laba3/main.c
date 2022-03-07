#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

char* entering_line(){
    char *line;
    int line_size = 0, letter_num = 0; char letter;
    line = (char*)malloc(1*sizeof(char));
    while(1){
        scanf("%c", &letter);
        if (letter == '\n'){
            line[letter_num] = '\0';
            break;
        }
        line[letter_num] = letter;
        line_size+=2; letter_num++;
        line = (char*)realloc(line, line_size);
    }
    return line;
}

int main(){
    printf("Parent process is now running :D\n");
    int pid, status, died;
    int i = 5;
    char *send_to_child;
    send_to_child = (char**)malloc(2);
    printf("Enter file name for child: ");
    send_to_child = entering_line();
    puts("");
    switch(pid = fork()){
        case -1: printf("Parrent couldnt create child, interrupt\n"); exit(-1);
        case 0: if(execl("test.sh", send_to_child, NULL) == -1){
                    printf("Parent: Error using execve\n");
                    exit(-1);
                }
                exit(3);
        default:
                while(i != 0){
                    printf("%d\n", i);
                    i--;
                    system("sleep 1");
                } 
                printf("Parent wait for child\n");
                died = wait(&status);
    }
    printf("Parent: %d - proccess ended\n", died);
    printf("Ending parrent proccess...\n");
    return 0;
}
