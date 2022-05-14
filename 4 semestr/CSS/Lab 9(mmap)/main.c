#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>

#include "../../../MyLibs/C_Lib/functions.h"

struct threadData{
    long bytes;  //how much to proccess
    unsigned long long offset; //from where to start
    char *nameOfFile;
};

void *ProccessData(void *arg){
    struct threadData *data = (struct threadData *)arg;

    //preparations
    //printf("Thread starts\n");
    int fd =  -1;
    fd = open(data->nameOfFile, O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1){
        printf("Thread faced an error with file, leaving thread\n");
        pthread_exit(NULL);
    }
    char *fileInMemory = mmap(NULL, data->bytes, PROT_READ | PROT_WRITE, MAP_SHARED,fd,data->offset*sysconf(_SC_PAGESIZE));
    if(fileInMemory == MAP_FAILED){
        printf("Thread faced an error with mmap, leaving thread\n");
        pthread_exit(NULL);
    }
    for(int i = 0; i < data->bytes;i++)
        if(i % 2 == 0)
            fileInMemory[i] = toupper(fileInMemory[i]);
        
    munmap(fileInMemory,data->bytes);
    close(fd);
    //printf("Thread finished\n");
    pthread_exit(NULL);
}

int main(){
    FILE *fp; char* path;
    struct stat statistic;
    pthread_t *processingFiles = (pthread_t*)malloc(sizeof(pthread_t)*10);
    printf("page size: %ld\n",sysconf(_SC_PAGESIZE));
    printf("Enter path to file: ");
    path = EnterLineReturn();
    if ((fp=fopen(path, "rb")) == NULL) {
        printf("Cannot open file.\n");
        return -1;
    }

    fstat (fileno (fp), &statistic);
    printf("File was found, size: %ld\n", statistic.st_size);
    long temp = statistic.st_size;
    struct threadData *sendData = (struct threadData*)malloc(sizeof(struct threadData)*10); int currentTask = 0; unsigned long offset = 0;
    //telling all threads about file
    for(int i = 0; i < 10;i++)
        sendData[i].nameOfFile = path;
    //decide how many blocks of memory we've got
    while(temp > 0){
        if(temp < sysconf(_SC_PAGESIZE)){
            pthread_join(processingFiles[currentTask],NULL);
            sendData[currentTask].bytes = temp;
            sendData[currentTask].offset = offset;
            pthread_create(&processingFiles[currentTask],NULL,&ProccessData,&sendData[currentTask]);
            break;
        }
        pthread_join(processingFiles[currentTask],NULL);
        sendData[currentTask].bytes = sysconf(_SC_PAGESIZE);
        sendData[currentTask].offset = offset;
        temp-= sysconf(_SC_PAGESIZE);
        pthread_create(&processingFiles[currentTask],NULL,&ProccessData,&sendData[currentTask]);
        currentTask++; offset++;
        if(currentTask > 9)
            currentTask = 0;
    }
    //waiting for threads
    for(int i = 0;i < 10;i++)
        pthread_join(processingFiles[i],NULL);

    free(path);
    fclose(fp);
    return 0;
}