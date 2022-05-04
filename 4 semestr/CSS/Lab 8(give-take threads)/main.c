#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include "circle.h"
#include "../../../MyLibs/C_Lib/functions.h"

pthread_mutex_t lock;
long long mainThreadId = -1;

void Killer(int sig){
    if(mainThreadId == pthread_self()){
        printf("Nice try\n");
        return;
    }
        char *name = (char*)malloc(100);
        sprintf(name,"%lld",pthread_self());
        FILE *file = fopen(name,"w");
        fprintf(file,"0");
        fclose(file);
        free(name);  
}

void Generate(struct Circle **BufferCircle){
    //creating message
    int size = rand()%253; //26 symbols + 96
    for(int i = 0; i < size;i++){
        int letter = rand()%26;
        (*BufferCircle)->message[i] = letter+96;
    }
    (*BufferCircle)->message[size] = '\0';
    
    //creating checksum
    for(int i = 0; i < size;i++){
        int num = (*BufferCircle)->message[i];
        (*BufferCircle)->checksum = (*BufferCircle)->checksum + num*i;
    }
    (*BufferCircle)->added++;
    printf("Produser setted message\n");
}

void *Producer(void *arg){
    sem_t *SemProducer = sem_open("/semproducer", 0644,0);
    sem_t *SemTaker = sem_open("/semtaker", 0644, 0);

    struct sigaction killer;
    killer.sa_handler = Killer;
    sigaction(SIGUSR1,&killer,0);

    struct Circle *BufferCircle =  (struct Circle*)arg;
    int leave_process = 1;
    while(leave_process == 1){
        sem_wait(SemProducer);
        pthread_mutex_lock(&lock);
        if(BufferCircle->message[0] == '\0'){
            Generate(&BufferCircle);
            pthread_mutex_unlock(&lock);
            sem_post(SemTaker);
            sleep(3);
        } else{
            pthread_mutex_unlock(&lock);
        }
        sem_post(SemProducer);

        BufferCircle = BufferCircle->right;
        char *name = (char*)malloc(100);
        sprintf(name,"%lld",pthread_self());
        FILE *file = fopen(name,"r");
        if(!file)
            break;

        fscanf(file,"%d",&leave_process);
        fclose(file);
        free(name);       
        sleep(1);
    }

    sem_close(SemProducer);
    sem_close(SemTaker);

    pthread_exit(NULL);
}

void Read(struct Circle **BufferCircle){
    unsigned long long check_checksum = 0;
    for(int i = 0; (*BufferCircle)->message[i] != '\0';i++){
        int num = (*BufferCircle)->message[i];
        check_checksum = check_checksum + num*i;
    }
    if(check_checksum != (*BufferCircle)->checksum)
        printf("Checksum wasn't correct!\n");
    printf("Taker read: %s\n",(*BufferCircle)->message);
    for(int i = 0; i < 254;i++)
        (*BufferCircle)->message[i] = '\0';
    (*BufferCircle)->checksum = 0;
    (*BufferCircle)->deleted++;
}

void *Taker(void *arg){
    sem_t *SemProducer = sem_open("/semproducer", 0644,0);
    sem_t *SemTaker = sem_open("/semtaker", 0644, 0);

    struct sigaction killer;
    killer.sa_handler = Killer;
    sigaction(SIGUSR1,&killer,0);

    char *name = (char*)malloc(100);
    sprintf(name,"%lld",pthread_self());
    struct Circle *BufferCircle = (struct Circle*)arg;

    int leave_process = 1;
    while(leave_process == 1){

        pthread_mutex_lock(&lock);
        sem_wait(SemTaker);
        if(BufferCircle->message[0] != '\0'){
            Read(&BufferCircle);
            sem_post(SemProducer);
            pthread_mutex_unlock(&lock);
            sleep(3);
        } else{
            pthread_mutex_unlock(&lock);
        }
        sem_post(SemTaker);
        //going to right circle
        BufferCircle = BufferCircle->right;
        FILE *file = fopen(name,"r");
        if(!file)
            break;
        fscanf(file,"%d",&leave_process);
        fclose(file); 
        sleep(1);
    }

    sem_close(SemProducer);
    sem_close(SemTaker);
    remove(name);
    free(name);  
    pthread_exit(NULL);
}

int main(){
    sem_unlink("/semproducer");
    sem_unlink("/semtaker");
    pthread_mutex_init(&lock, NULL);

    sem_t *SemProducer = sem_open("/semproducer", O_CREAT, 0644,1);
    sem_t *SemTaker = sem_open("/semtaker", O_CREAT, 0644, 1);

    if(SemProducer == SEM_FAILED || SemTaker == SEM_FAILED){
        perror("open failed");
        return -1;
    }

    //DATA SEG
    pthread_t *producerThreads = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_t *takerThreads = (pthread_t*)malloc(sizeof(pthread_t));
    char *get_line; int leave = 1;
    struct Circle *BufferCircle = NULL;
    mainThreadId = pthread_self();
    int amountProducerThreads = 0, amountTakerThreads = 0;
    //END DATA SEG

    while(leave){
        get_line = EnterLineReturn();
        switch(get_line[0]){
            case 'h': printf("p - create one more producer\n"
                             "t - create one more taker\n"
                             "-(p/t) - delete producer/taker\n"
                             "q - quit program\n"
                             "c - create part of ring\n"
                             "z - show statistic of all elements of ring\n"); break;
            case 'p': {
                        if(BufferCircle == NULL){
                            SetOutputColor("RED");
                            printf("Cant create producer/taker without having any slots of memory\n");
                            SetOutputColor("def");
                            break;
                        }
                        producerThreads = (pthread_t*)realloc(producerThreads,(++amountProducerThreads)*sizeof(pthread_t));
                        pthread_create(&producerThreads[amountProducerThreads-1], NULL, &Producer, BufferCircle);
                        SetOutputColor("GREEN");
                        printf("producer thread was created\n");
                        SetOutputColor("def");

                        char *name = (char*)malloc(100);
                        sprintf(name,"%lld",producerThreads[amountProducerThreads-1]);
                        FILE *file = fopen(name,"w"); fprintf(file,"1"); fclose(file);
                        free(name);
                        break;
            }
            case 't': {
                        if(BufferCircle == NULL){
                            SetOutputColor("RED");
                            printf("Cant create producer/taker without having any slots of memory\n");
                            SetOutputColor("def");
                            break;
                        }
                        amountTakerThreads++;
                        takerThreads = (pthread_t*)realloc(takerThreads,amountTakerThreads*sizeof(pthread_t));
                        pthread_create(&takerThreads[amountTakerThreads-1],NULL, &Taker, BufferCircle);
                        SetOutputColor("GREEN");
                        printf("taker thread was created\n");
                        SetOutputColor("def");

                        char *name = (char*)malloc(100);
                        sprintf(name,"%lld",takerThreads[amountTakerThreads-1]);
                        FILE *file = fopen(name,"w");
                        fprintf(file,"1");
                        fclose(file);
                        free(name);
                        break;
            }
            case '-':{
                        if(get_line[1] == 'p'){
                            if (amountProducerThreads == 0){
                                SetOutputColor("RED");
                                printf("There is no producers");
                                SetOutputColor("def");
                                break;
                            }
                            char *name = (char*)malloc(100);
                            sprintf(name,"%lld",producerThreads[amountProducerThreads-1]);

                            pthread_kill(producerThreads[--amountProducerThreads],10);
                            pthread_join(producerThreads[amountProducerThreads],NULL);
                            printf("Producer thread was killed\n");
                            producerThreads = (pthread_t*)realloc(producerThreads,sizeof(pthread_t)*amountProducerThreads);

                            remove(name);
                            free(name);
                        } else if (get_line[1] == 't'){
                            if (amountTakerThreads == 0){
                                SetOutputColor("RED");
                                printf("There is no takers");
                                SetOutputColor("def");
                                break;
                            }
                            char *name = (char*)malloc(100);
                            sprintf(name,"%lld",takerThreads[amountTakerThreads-1]);                           

                            pthread_kill(takerThreads[(--amountTakerThreads)],10);
                            pthread_join(takerThreads[amountTakerThreads],NULL);
                            printf("Taker thread was killed\n");
                            takerThreads = (pthread_t*)realloc(takerThreads,sizeof(pthread_t)*amountTakerThreads);

                            remove(name);
                            free(name);
                        }
                        break;
            }
            case 'c':   pthread_mutex_lock(&lock);
                        AppendCircle(&BufferCircle); sem_post(SemProducer); sem_post(SemTaker);
                        pthread_mutex_unlock(&lock);
                        printf("Circle added\n"); break;
            case 'q': leave = 0; break;
            case 'z': if(BufferCircle == NULL)
                        break;
                      printf("You now in a queue, pls wait\n");
                      pthread_mutex_lock(&lock);

                      struct Circle *runner = BufferCircle;
                      do{
                        printf("------------------------------------------------------\n");
                        printf("Was Added: %d \nWas Removed: %d\nCurrent cheksum: %d\n",runner->added,runner->deleted,runner->checksum);
                        runner = runner->right;
                      }while(runner != BufferCircle);
                        printf("------------------------------------------------------\n");
                      pthread_mutex_unlock(&lock);
                      break;
            default: break;
        }
        free(get_line);
    }

    //END SECTION
        //killing threads
    while(amountProducerThreads != 0){
        char *name = (char*)malloc(100);
        sprintf(name,"%lld",producerThreads[amountProducerThreads-1]);

        pthread_kill(producerThreads[--amountProducerThreads],10);
        pthread_join(producerThreads[amountProducerThreads],NULL);
        printf("Producer was killed\n");

        remove(name);
        free(name);
    }
    while(amountTakerThreads != 0){
        char *name = (char*)malloc(100);
        sprintf(name,"%lld",takerThreads[amountTakerThreads-1]);

        pthread_kill(takerThreads[--amountTakerThreads],10);
        pthread_join(takerThreads[amountTakerThreads],NULL);
        printf("Taker was killed\n");

        remove(name);
        free(name);
    }
        //CLOSING SEMAPHORS
    sem_close(SemProducer);
    sem_close(SemTaker);
    sem_unlink("/semproducer");
    sem_unlink("/semtaker");
    pthread_mutex_destroy(&lock);
    return 0;
}
