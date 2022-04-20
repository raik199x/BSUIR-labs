#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include "circle.h"
#include "old_functions/functions.h"


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
    temp = kill((*old_pids)[count],SIGUSR1);
    if(temp != 0)
        printf("Error killing procces! Try to exit program to prevent proccess stuck in system\n");
    else{
        SetOutputColor("GREEN");
        printf("%d - procces was killed\n", (*old_pids)[count]);
        SetOutputColor("def");
        wait((*old_pids)[count]);       //to avoid proccess become a zombie we ask parent to close it
    }
    for(int i = 0; i < count; i++)
        new_pids[i] = (*old_pids)[i];
    free((*old_pids));
    return new_pids;
}

int main(){
    struct Circle *BufferCircle = NULL;

    sem_unlink("/semproducer");
    sem_unlink("/semtaker");
    sem_unlink("/semmutex");

    sem_t *SemProducer = sem_open("/semproducer", O_CREAT, 0644,0);
    sem_t *SemTaker = sem_open("/semtaker", O_CREAT, 0644, 0);
    sem_t *SemMutex = sem_open("/semmutex", O_CREAT, 0644, 1);

    if(SemProducer == SEM_FAILED || SemTaker == SEM_FAILED || SemMutex == SEM_FAILED){
        perror("open failed");
        return -1;
    }

    //DATA SEG
    int *ProducerPids, CountProducer=0, CreatedCircles = 0;
    int *TakerPids, CountTaker=0;
    ProducerPids = (int*)malloc(sizeof(int));
    TakerPids = (int*)malloc(sizeof(int));
    char *get_line; int leave = 1;

    //END DATA SEG

    while(leave){
        get_line = EnterLineReturn();
        switch(get_line[0]){
            case 'h': printf("p - create one more producer\n"
                             "t - create one more taker\n"
                             "-(p/t) - delete producer/taker\n"
                             "q - quit programm\n"
                             "c - create shared memory\n"
                             "z - show statistic of all circles\n"); break;
            case 'p': {
                        if(BufferCircle == NULL){
                            SetOutputColor("RED");
                            printf("Cant create producer/taker without having any slots of memory\n");
                            SetOutputColor("def");
                            break;
                        }
                        int temp = fork();  //creating proccess
                        if(temp == -1){    //check if we actually creat him
                            printf("Error creating proccess!\n");
                            break;
                        }
                        if (temp != 0){ //for parent: updating info about pids
                            CountProducer++;
                            ProducerPids = UpdatePidsIncrease(&ProducerPids, CountProducer);
                            ProducerPids[CountProducer-1] = temp;
                            SetOutputColor("GREEN");
                            printf("%d - Producer procces was created\n",ProducerPids[CountProducer-1]);
                            SetOutputColor("def");
                            sem_post(SemProducer);
                        }
                        else{
                            char *send_shmid = IntToChar(BufferCircle->current_shmid);
                            execl("./producer",send_shmid, NULL);
                            printf("Critical\n");
                        }
                        break;
            }
            case 't': {
                        if(BufferCircle == NULL){
                            SetOutputColor("RED");
                            printf("Cant create producer/taker without having any slots of memory\n");
                            SetOutputColor("def");
                            break;
                        }
                        int temp = fork();  //creating proccess
                        if(temp == -1){    //check if we actually creat him
                            printf("Error creating proccess!\n");
                            break;
                        }
                        if (temp != 0){ //for parent: updating info about pids
                            CountTaker++;
                            TakerPids = UpdatePidsIncrease(&TakerPids, CountTaker);
                            TakerPids[CountTaker-1] = temp;
                            SetOutputColor("GREEN");
                            printf("%d - Taker procces was created\n",TakerPids[CountTaker-1]);
                            SetOutputColor("def");
                            sem_post(SemTaker);
                        }
                        else{
                            char *send_shmid = IntToChar(BufferCircle->current_shmid);
                            execl("./taker",send_shmid, NULL);
                            printf("Critical\n");
                        }
                        break;
            }
            case '-':{
                        if(get_line[1] == 'p'){
                            if (CountProducer == 0){
                                SetOutputColor("RED");
                                printf("There is no producers");
                                SetOutputColor("def");
                                break;
                            }
                            CountProducer--;
                            ProducerPids = UpdatePidsDecrease(&ProducerPids,CountProducer);
                            sem_wait(SemProducer);
                        } else if (get_line[1] == 't'){
                            if (CountTaker == 0){
                                SetOutputColor("RED");
                                printf("There is no producers");
                                SetOutputColor("def");
                                break;
                            }
                            CountTaker--;
                            TakerPids = UpdatePidsDecrease(&TakerPids,CountTaker);
                            sem_wait(SemTaker);
                        }
                        break;
            }
            case 'c': sem_wait(SemMutex); AppendCircle(&BufferCircle); CreatedCircles++; sem_post(SemMutex); printf("Circle added\n"); break;
            case 'q': leave = 0; break;
            case 'z': if(CreatedCircles == 0)
                        break;
                      printf("You now in a queue, pls wait\n");
                      sem_wait(SemMutex);

                      int remember_shmid = BufferCircle->current_shmid; //this will be our flag for end
                      int next_shmid = BufferCircle->right_shmid;   //setting current pos
                      shmdt(BufferCircle);                          //detachins block of memory
                      BufferCircle = shmat(next_shmid, NULL,0);     //setting next

                      while(remember_shmid != BufferCircle->current_shmid){
                        printf("------------------------------------------------------\n");
                        printf("SHMID: %d \nWas Added: %d \nWas Removed: %d\nCurrent cheksum: %d\n",BufferCircle->current_shmid,BufferCircle->added,BufferCircle->deleted,BufferCircle->checksum);
                        next_shmid = BufferCircle->right_shmid;

                        shmdt(BufferCircle);
                        BufferCircle = shmat(next_shmid,NULL,0);
                      }
                        printf("------------------------------------------------------\n");
                        printf("SHMID: %d \nWas Added: %d \nWas Removed: %d\nCurrent cheksum: %d\n",BufferCircle->current_shmid,BufferCircle->added,BufferCircle->deleted,BufferCircle->checksum);
                        printf("------------------------------------------------------\n");
                      sem_post(SemMutex);
                      break;
            default: break;
        }
        free(get_line);
    }

    //END SECTION
        //killing proccess
    while(CountProducer != 0){
        CountProducer--;
        ProducerPids = UpdatePidsDecrease(&ProducerPids,CountProducer);
    }
    while(CountTaker != 0){
        CountTaker--;
        TakerPids = UpdatePidsDecrease(&TakerPids,CountTaker);
    }
        //CLOSING SEMAPHORS
    sem_close(SemMutex);
    sem_close(SemProducer);
    sem_close(SemTaker);
    sem_unlink("/semproducer");
    sem_unlink("/semtaker");
    sem_unlink("/semmutex");
        //CLOSING SHARED MEMORY (if needed)
    if(BufferCircle == NULL)
        return 0;
    int result_end = 1;
        //find size of circle
    int next_shmid = BufferCircle->current_shmid;
    while(result_end >= 0 && CreatedCircles != 0){
        int shmid = next_shmid;
        if(CreatedCircles > 1)
            next_shmid = BufferCircle->right_shmid;
        result_end = shmdt(BufferCircle);
        if(result_end == -1){
            printf("shmdt error\n");
        }
        result_end = shmctl(shmid, IPC_RMID,NULL);
        if(result_end == -1){
            printf("shmctl error\n");
        }
        CreatedCircles--;
        if(CreatedCircles > 0)
            BufferCircle = shmat(next_shmid,NULL,0);
    }

    return 0;
}
