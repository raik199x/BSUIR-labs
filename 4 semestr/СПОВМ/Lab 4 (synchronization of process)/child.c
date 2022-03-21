#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"

struct statistic{
    int time_alive;
    int is_allowed_stat;
    int stat[2];
};

struct statistic ChildStat;

void resrict_output(int sig){
    ChildStat.is_allowed_stat = 0;
    return;
}

void allow_output(int sig){
    ChildStat.is_allowed_stat = 1;
    return;
}

void showing_stat(int sig){
    printf("pid - %d; ",getpid());
    if(ChildStat.is_allowed_stat == 1){
        printf("Time alive - %d; ",ChildStat.time_alive);
        printf("stat: %d",ChildStat.stat[0]);
        printf(" %d\n",ChildStat.stat[1]);
    }
    else
        printf("Sending stat restricted\n");
    return;
}

int main(){
    //Preparing all signal
    struct sigaction first;
    struct sigaction second;
    struct sigaction SendStat;
    first.sa_handler = resrict_output;
    second.sa_handler = allow_output;
    SendStat.sa_handler = showing_stat;
    sigaction(SIGUSR1,&first,0);
    sigaction(SIGUSR2,&second,0);
    sigaction(SIGHUP,&SendStat,0);
    ChildStat.time_alive = 0;
    ChildStat.stat[0] = 0;
    ChildStat.stat[1] = 0;
    ChildStat.is_allowed_stat = 1;
    
    int a = 0;
    while(1){
        if (a == 0)
            ChildStat.stat[0] = 1;
        else if (a == 1)
            ChildStat.stat[1] = 1;
        else if (a == 2)
            ChildStat.stat[0] = 0;
        else{
            ChildStat.stat[1] = 0;
            a = 0;
            system("sleep 1");
            continue;
        }
        a++;
        ChildStat.time_alive++;
        system("sleep 1");
    }
    return 0;
}