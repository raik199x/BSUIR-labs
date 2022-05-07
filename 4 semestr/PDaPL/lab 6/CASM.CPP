#include <stdio.h>
#include <dos.h>

int main(){
    double array[10], sum = 0;

    for(int i = 0; i < 10; i++){
	printf("Enter %d number: ",i+1);
	if(!scanf("%f",&array[i])){
	 printf("Error scanf, setted to 0\n");
	 rewind(stdin);
	 array[i] = 0;
	 continue;
	}
	puts("");
    }
    asm{
	finit
	lea bx,array
	fld sum
	push cx

	mov cx,10
    }
     start_loop:
    asm{
	fld dword ptr [bx]  //push into stack
	faddp
	add bx,8 //move to next slot of array
     loop start_loop
	pop cx
	fst sum
	fwait
    }
    printf("%f\n",sum);
    return 0;
}
