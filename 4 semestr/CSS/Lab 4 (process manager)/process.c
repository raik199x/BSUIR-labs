#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <malloc.h>
#include "functions.h"
#include <stdlib.h>
#include <sys/wait.h>

void help_output(){
    printf("+ - creat new proccess\n");
    printf("- - delete newest proccess\n");
    printf("k - kill all proccess\n");
    printf("g - allows all proccess send statistic, you also can specify one pid 'g <num>'\n");
    printf("s - forbid  procces to send statistic, you also can specify one pid 's <num>'\n");
    printf("p <num> - forbid all procces to send statisc and ask <num> to send his own statistic\n");
    printf("c - clears screen\n");
    printf("w - show all pids\n");
    printf("q - kill all proccess and end program\n");
}

//creates new array of pids that larger by one than old and returns it
int* UpdatePidsIncrease(int **old_pids, int count){
    int* new_pids;
    new_pids = (int*)malloc(count*sizeof(int));
    for(int i = 0; i < count-1; i++)
        new_pids[i] = (*old_pids)[i];
    free((*old_pids));
    return new_pids;
}

//creates new arrey of pids that lower by one than old, kills the newest pid and returns array
int* UpdatePidsDecrease(int **old_pids, int count){
    int* new_pids, temp;
    new_pids = (int*)malloc(count*sizeof(int));
    temp = kill((*old_pids)[count],SIGKILL);
    if(temp != 0)
        printf("Error killing procces! Try to exit program to prevent 'child' proccess stuck in system\n");
    else{
        printf("%d - procces was killed\n", (*old_pids)[count]);
        wait((*old_pids)[count]);       //to avoid proccess become a zombie we ask parent to close it
    }
    for(int i = 0; i < count; i++)
        new_pids[i] = (*old_pids)[i];
    free((*old_pids));
    return new_pids;
}

int* KillAll(int **pids, int count){
    int temp;
    if(count == 0)
        return (*pids);
    for(int i = 0; i < count; i++){
        temp = kill((*pids)[i],SIGKILL);
        if(temp != 0)
            printf("Error killing procces! Try to exit program to prevent 'child' proccess stuck in system\n");
        else{
            printf("%d - proccess was killed\n",(*pids)[i]);
            wait((*pids)[i]);
        }
    }
    free((*pids));
    return (*pids);
}

//whe the alarm() command implements, out program starts here and send to ALL procces SIGUSR2
void sig_MyAlarm(int sig){
    kill(0,SIGUSR2);
    return;
}

//to prvent parent proccess ending from signal SIGUSR2 we reimplement this signal
void user(int sig){
    return;
}

int main(void){
    char *line;
    int *pids, count = 0, temp;
    pids = (int*)malloc(sizeof(int));

    //reimplemanting signals
    struct sigaction mysig;
    struct sigaction NoKillUsr;
    mysig.sa_handler = sig_MyAlarm;
    NoKillUsr.sa_handler = user;
    sigaction(SIGALRM,&mysig,0);
    sigaction(SIGUSR2,&NoKillUsr,0);

    system("clear");
    while(1){
        rewind(stdin);
        printf("command> ");
        line = EnterLineReturn();
        if(line[0] == '\0')
            continue;
        switch(line[0]){
            case '+': {
                        int temp = fork();  //creating proccess
                        if(temp == -1){    //check if we actually creat him
                            printf("Error creating proccess!\n");
                            break;
                        }
                        if (temp != 0){ //for parent: updating info about pids
                            count++;
                            pids = UpdatePidsIncrease(&pids, count);
                            pids[count-1] = temp;
                            printf("%d - procces was created\n",pids[count-1]);
                        }
                        else{    //for child: starting child program
                            execl("./child","string",NULL);
                        }
                        break;
            }
            case '-':{
                        if(count == 0){
                            printf("Child proccess wasnt created!\n");
                            break;
                        }
                        count--;
                        pids = UpdatePidsDecrease(&pids,count);
                        break;
            }
            case 's':{
                        temp = ParseIntFromEnd(line);
                        if(temp == 0)
                            for(int i = 0;i < count;i++)
                                kill(pids[i],SIGUSR1);
                        else{
                            int i = -1;
                            for(int j = 0; j < count; j++)
                                  if(pids[j] == temp){
                                      i = j;
                                      break;
                                  }
                            if(i == -1){
                                printf("No such pid\n");
                                break;
                            }
                            kill(pids[i],SIGUSR1);
                        }
                        break;
            }
            case 'h': help_output(); break;
            case 'g': {
                        temp = ParseIntFromEnd(line);
                        if(temp == 0)
                            for(int i = 0;i < count;i++)
                                kill(pids[i],SIGUSR2);
                        else{
                            int i = -1;
                            for(int j = 0; j < count; j++)
                                  if(pids[j] == temp){
                                      i = j;
                                      break;
                                  }
                            if(i == -1){
                                printf("No such pid\n");
                                break;
                            }
                            kill(pids[i],SIGUSR2);
                        }
                        break;
            }
            case 'p': {
                        temp = ParseIntFromEnd(line);
                        int allow_again, j = -1;
                        for(int i = 0;i < count; i++)
                            if(pids[i] == temp){
                                j = i;
                                break;
                            }
                        if(j == -1){
                            printf("No such pid\n");
                            break;
                        }
                        printf("Enter delay> ");
                        scanf("%d",&allow_again);
                        for(int i = 0; i < count; i++){
                            if(i == j)
                                continue;
                            kill(pids[i],SIGUSR1);
                        }
                        alarm(allow_again);                  //preparing SIGALRM
                        getchar();                          //small bug, symbol flying into stdin
                        break;
            }
            case 'c': system("clear"); break;
            case 'w': {
                        for(int i = count-1; i>=0; i--){
                            kill(pids[i],SIGHUP);
                            system("sleep 0.01");
                        }
                        break;
            }
            case 'k': {
                      pids = KillAll(&pids,count);
                      count = 0;
                      pids = (int*)malloc(sizeof(int));
                      break;
            }
            case 'q':{
                        pids = KillAll(&pids,count);
                        free(line);
                        return 0;
            }
        }
        free(line);
    }
}