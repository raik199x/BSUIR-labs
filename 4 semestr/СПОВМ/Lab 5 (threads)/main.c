#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "../../../MyLibs/C_Lib/functions.h"

//data for all threads
int width;
int height;
int stride;
int padding;
unsigned char **FileData;

void *ActionThreadOne(){
	for(int i = 0; i < height/2; i++)
		for(int j = 0; j < width*3;j+=3){
			FileData[i][j]*=0.3;
			FileData[i][j+1]*=0.58;
			FileData[i][j+2]*=0.11;
		}
}

void *ActionThreadTwo(){
	for(int i = height/2; i < height; i++)
		for(int j = 0; j < width*3;j+=3){
			FileData[i][j]*=0.3;
			FileData[i][j+1]*=0.58;
			FileData[i][j+2]*=0.11;
		}
}

int main(){
	
	//validation file section
	printf("Enter file name: ");
	char *FileName = EnterLineReturn();
	unsigned int counter = strlen(FileName);
	if(FileName[counter-1] != 'p' || FileName[counter-2] != 'm' || FileName[counter-3] != 'b' || FileName[counter -4] != '.'){
		free(FileName);
		printf("Error: type of file is not bmp\n");
		return -1;
	}
	//end validation
	
	FILE *fIn = fopen(FileName, "rb");
	FILE *fOutNoThread = fopen("ImageWithoutThread.bmp", "wb");
	if (!fIn || !fOutNoThread){
		perror("File error\n");
		return -1;
	}
	
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, fIn);
	fwrite(header, sizeof(unsigned char), 54, fOutNoThread);
	
	width = *(int*)&header[18];
	height = abs(*(int*)&header[22]);
	stride = (width * 3 + 3) & ~3;
	padding = stride - width * 3;
	
	clock_t start,end;
	start = clock(); //set time
	//Main Logic
	unsigned char pixel[3];
	for (int y = 0; y < height; ++y){
		for (int x = 0; x < width; ++x){
			fread(pixel, 3, 1, fIn);
			unsigned char gray = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
			memset(pixel, gray, sizeof(pixel));
			fwrite(&pixel, 3, 1, fOutNoThread);
		}
		fread(pixel, padding, 1, fIn);
		fwrite(pixel, padding, 1, fOutNoThread);
	}
	fclose(fIn);
	fclose(fOutNoThread);
	end = clock();
	printf("program without threads: %f\n",difftime(end, start));
	
	//with threads
	// The main problem is thas i can't open file with two links
	//So now i need to store information in buffer
	pthread_t ThreadOne, ThreadTwo;
	start = clock();
	
	//file manipulation
	fIn = fopen(FileName, "rb");
	FILE *fOutWithThread = fopen("ImageWithThread.bmp", "wb");
	fread(header, sizeof(unsigned char), 54, fIn);
	fwrite(header, sizeof(unsigned char), 54, fOutWithThread);
	FileData = (unsigned char**)malloc(height*sizeof(unsigned char*));
	for(int i = 0; i < height; i++)
		FileData[i] = (char*)malloc(width*3);
	
	for (int y = 0; y < height; ++y){
        int myX = 0;
		for (int x = 0; x < width; ++x){
			fread(pixel, 3, 1, fIn);
            FileData[y][myX++] = pixel[0];
            FileData[y][myX++] = pixel[1];
            FileData[y][myX++] = pixel[2];
		}
		fread(pixel, padding, 1, fIn);
	}
	
	pthread_create(&ThreadOne,NULL,&ActionThreadOne, NULL);
	pthread_create(&ThreadOne,NULL,&ActionThreadTwo, NULL);
	
	pthread_join(ThreadOne, NULL);
	pthread_join(ThreadTwo, NULL);
	
    unsigned char postpix[3];
	for(int i = 0; i < height;i++){
		for(int j = 0; j < width*3;j+=3){
			unsigned char gray = FileData[i][j] + FileData[i][j+1] + FileData[i][j+2];
			memset(postpix, gray, sizeof(postpix));
			fwrite(&postpix, 3, 1, fOutWithThread);
		}
		fwrite(pixel, padding, 1, fOutNoThread);
	}
	//END SECTION
	end = clock();
	printf("program with threads: %f\n",difftime(end, start));
	free(FileName);
	for(int i = 0; i < height; i++)
		free(FileData[i]);
	free(FileData);
	fclose(fIn);
	fclose(fOutWithThread);
	return 0;
}