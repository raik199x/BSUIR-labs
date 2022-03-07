#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    printf("children will open path: %s\n", argv[1]);
    FILE *f;
    if(!(f = fopen(argv[1],"r+"))){
        printf("Children: error to open file, exit\n");
        exit(-1);
    }
    char temp; int count = 0;
    while(fscanf(f,"%c",&temp)){
        if (feof(f))
            break;
        count++;
    }
    count--;
    printf("Children: There are %d symbols in file!\n", count);
    fclose(f);
	system("sleep 8");
    //system("/usr/bin/konsole");
	//printf("Children: Opening terminal");
    //system("terminator");
}
