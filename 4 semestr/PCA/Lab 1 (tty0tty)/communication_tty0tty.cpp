#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <malloc.h>

int entering_line(char **line){
    int line_size = 0, letter_num = 0; char letter;
    (*line) = (char*)malloc(1*sizeof(char));
    while(1){
        scanf("%c", &letter);
        if (letter == '\n'){
            (*line)[letter_num] = '\0';
            break;
        }
        (*line)[letter_num] = letter;
        line_size+=2; letter_num++;
        (*line) = (char*)realloc(*line, line_size);
    }
    return line_size;
}

int main(){

    int fd0 = open("/dev/tnt0", O_RDWR | O_NOCTTY | O_NDELAY);
    int fd1 = open("/dev/tnt1", O_RDWR | O_NOCTTY | O_NDELAY);
    int bytes;
    if (fd0 == -1){
        perror("open_port: Unable to open");
        return fd0;
    }
    else{
        fcntl(fd0, F_SETFL, 0);
        printf("Opened successfully!\n");
    }

    char *send_message;
    bytes = entering_line(&send_message);


    int n;
    n = write(fd0, send_message, bytes);
    if (n == -1){
        fputs("write() of 1 bytes failed!\n", stderr);
        free(send_message);
        return n;
    } else {
        printf("Success write!\n");
    }

    char accept_letter = 'a';
    while (accept_letter != '\0'){
        n = read(fd1, &accept_letter, 1);
        if(n == -1){
            printf("Error read\n");
            free(send_message);
            return -1;
        }
        printf("%c", accept_letter);
    }
    puts("");
    close(fd0);
    close(fd1);
    free(send_message);
    return 0;
}
