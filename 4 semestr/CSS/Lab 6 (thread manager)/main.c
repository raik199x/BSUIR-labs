#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include "../../../MyLibs/C_Lib/functions.h"

unsigned int ThreadStatusLength = 0;     //Number of threads
unsigned long **ThreadStatus;           //for Threads to handle signals
sem_t *threadmanager;
int forbidden = 0; unsigned long one_allowed = 0;
/*  Thread Status manual
*   0 - no action
*   1 - output personal info
*   2 - forbidden to send data
*/

void ShowAllData(){
    for(int i = 0; i < ThreadStatusLength; i++){
        if(ThreadStatus[i][1] == 2){
            //printf("Some threads are blocked\n");
        }
        else
            ThreadStatus[i][1] = 1;
    }
    return;
}

void NewSetAlarm(){
    forbidden = 0;
    return;
}

void ForbidAllData(){
    for(int i = 0; i < ThreadStatusLength; i++){
        if(ThreadStatus[i][0] == one_allowed)
            continue;
        ThreadStatus[i][1] = 2;
    }
    return;
}

void AllowAllData(){
    for(int i = 0; i < ThreadStatusLength; i++)
        ThreadStatus[i][1] = 0;
    return;
}

void ShowStatus(){
    for(int i = 0; i < ThreadStatusLength; i++)
        printf("%d\n",ThreadStatus[i][1]);
}

int StopThread(const long ThreadId){
    for(int i = 0;i < ThreadStatusLength;i++)
        if(ThreadId == ThreadStatus[i][0]){
            ThreadStatus[i][1] = 9;
            return i;
        }
    printf("No such id\n");
    return -1;
}

int CheckExistId(const long ThreadId){
    for(int i = 0;i < ThreadStatusLength;i++)
        if(ThreadId == ThreadStatus[i][0]){
            return i;
        }
    printf("No such id\n");
    return -1;
}

void *ThreadAction(){
    long FooThread = pthread_self();
    ThreadStatus[ThreadStatusLength-1][0] = FooThread;
    printf("%ld started mining bitcoin\n",pthread_self());
    double Bitcoin = 0; unsigned short __EndThread__ = 1;
    while(__EndThread__){
        Bitcoin+=0.00001;       //simple
        system("sleep 0.2");   //task for thread
        //Status logic
        sem_wait(threadmanager);
        for(int i = 0; i < ThreadStatusLength; i++){
            if(pthread_self() == ThreadStatus[i][0]){
                switch(ThreadStatus[i][1]){
                    case 0: break;
                    case 1: printf("%ld - mined %f of bitcoin\n",pthread_self(),Bitcoin);break;
                    case 2: ThreadStatus[i][1] = 2; break;
                    case 9: __EndThread__ = 0; printf("Signal accepted\n"); break;
                    default:printf("Error case of ThreadStatus\n"); break;
                }
                ThreadStatus[i][1] = 0;
                break;
            }
        }
        sem_post(threadmanager);
    }
    pthread_exit(NULL);
}

void UpdateThreadsIncrease(pthread_t **ThreadArray, int *ThreadCounter){
    pthread_t *NewArray = (pthread_t*)malloc(sizeof(pthread_t)*((*ThreadCounter)+1)); //Preparing new array for threads
    for (int i = 0; i < (*ThreadCounter);i++)                                        // copy old data
        NewArray[i] = (*ThreadArray)[i];
    free((*ThreadArray));                                                          // clear old array                                                         
    (*ThreadArray) = NewArray;                                                  //returning new array

    //updating status array
    ThreadStatus = (unsigned long**)realloc(ThreadStatus,sizeof(unsigned long*)*(++ThreadStatusLength));
    ThreadStatus[ThreadStatusLength-1] = (unsigned long*)malloc(sizeof(unsigned long)*2);
    pthread_create(&NewArray[(*ThreadCounter)++],NULL,&ThreadAction,NULL);
    system("sleep 0.1");
    ThreadStatus[ThreadStatusLength-1][1] = 2;

}

void UpdateThreadsDecrease(pthread_t **ThreadArray, int *ThreadCounter,const long ThreadId){
    //delete from ThreadArray
    for(int i = 0; i < (*ThreadCounter);i++)
        if(ThreadStatus[i][0] == ThreadId){
            for(int j = i; j < (*ThreadCounter)-1;j++){
                (*ThreadArray)[j] = (*ThreadArray)[j+1];
                ThreadStatus[j][0] = ThreadStatus[j+1][0];
                ThreadStatus[j][1] = ThreadStatus[j+1][1];
            }
            break;
        }
    (*ThreadArray) = (pthread_t*)realloc((*ThreadArray),sizeof(pthread_t)*(--(*ThreadCounter)));
    ThreadStatus = (unsigned long**)realloc(ThreadStatus,sizeof(unsigned long*)*(*ThreadCounter));
    ThreadStatusLength--;
}

int main(){
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++DATA SEG
    pthread_t *ThreadArray; int ThreadCounter = 0;
    ThreadArray = (pthread_t*)malloc(sizeof(pthread_t));
    char *line; int __EndProgram__ = 1;
    ThreadStatus = (unsigned long**)malloc(sizeof(unsigned long*));
    ThreadStatus[0] = (unsigned long*)malloc(sizeof(long)*2);
    int result = 0;

    sem_unlink("/threadmanager");
    threadmanager = sem_open("threadmanager",O_CREAT,0644,1);

    struct sigaction MyAlarm;
    MyAlarm.sa_handler = NewSetAlarm;
    sigaction(SIGALRM,&MyAlarm,0);
    //-----------------------------------------------------END DATA SEG

    while(__EndProgram__){
        rewind(stdin);
        if(forbidden == 1){
            sem_wait(threadmanager);
            ForbidAllData();
            sem_post(threadmanager);
        }
        line = EnterLineReturn();

        switch(line[0]){
            case 'h': printf("+ - create new thread\n"
                             "- <id>- delete thread\n"
                             "k - end all threads\n"
                             "g - allows threads send information\n"
                             "s - forbid threads send information\n"
                             "p - forbids thread send information for some time except if specified\n"
                             "h - show help menu\n"
                             "c - clear screen\n"
                             "w - show all thread + output information\n"
                             "q - exit program\n");
                             break;
            case 'c': system("clear"); break;
            case '+': sem_wait(threadmanager); UpdateThreadsIncrease(&ThreadArray, &ThreadCounter); sem_post(threadmanager); break;
            case 'g': sem_wait(threadmanager); AllowAllData(); forbidden = 0; sem_post(threadmanager); break;
            case 's': forbidden = 1; break;
            case '-': free(line);
                      printf("Enter id: ");
                      line = EnterLineReturn();
                      result = StopThread(atol(line));
                      if(result == -1)
                        break;
                      system("sleep 1");
                      sem_wait(threadmanager);
                      UpdateThreadsDecrease(&ThreadArray, &ThreadCounter,atol(line));
                      sem_post(threadmanager);
                      break;
            case 'k': sem_wait(threadmanager);
                      for(int i = ThreadStatusLength-1; i >= 0;i--){
                        pthread_cancel(ThreadArray[i]);
                        UpdateThreadsDecrease(&ThreadArray, &ThreadCounter,ThreadStatus[i][0]);
                      }
                      sem_post(threadmanager);
                      break;
            case 'w': sem_wait(threadmanager); ShowAllData(); sem_post(threadmanager); break;
            case 'p': free(line);
                      printf("Enter id: ");
                      line = EnterLineReturn();
                      result = CheckExistId(atol(line));
                      if(result == -1){
                          printf("No such id\n");
                          break;
                      }
                      one_allowed = atol(line);
                      forbidden = 1; alarm(5); break;
            case 'z': ShowStatus(); break;
            case 'q': __EndProgram__ = 0; break;
            default: break;
        }
    }
    sem_close(threadmanager);
    sem_unlink("/threadmanager");
    return 0;
}