.model small
.stack 100h

.data

Error_Write db "Write error!",0Dh,0Ah,'$'
Error_Read db "Read error!",0Dh,0Ah,'$'
           
.code

start:
; connect data
mov ax,DGROUP
mov ds,ax
; send byte
mov dx,0
mov ah,1
mov al,'r'
int 14h
mov al,'a'

;check if cool
cmp ah,7
je W_Error
jmp W_Cont
W_Error:
   mov dx,offset Error_Write
   mov ah,9
   int 21h
   mov ax,4C00h
   int 21h
   ret
W_Cont:
; read
xor dx,dx
mov dx,1
mov ah,02h
int 14h
;check if read
cmp ah,0
jne R_Error
jmp R_Continue
R_Error:
   mov dx,offset Error_Read
   mov ah,9
   int 21h
   mov ax,4C00h
   int 21h
   ret
R_Continue:
   mov ah,2
   mov dl,al
   int 21h
   mov ax,4C00h
   int 21h

end start