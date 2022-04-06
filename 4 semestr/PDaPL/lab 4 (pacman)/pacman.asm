.model small
.stack 100h
.data

.code 

ClearScreen proc
    mov ah,0h 
    mov al,03 ;setting graphig mod 3
    int 10h   ; + clearing screen
ClearScreen endp
start:
    mov ax,DGROUP
    mov ds,ax

    push 0B800h ; code of vider driver
    pop es      ; setting code

    xor di,di
    mov di,222

    call ClearScreen

    mov es:[di],word ptr ' '
    inc di
    mov es:[di],word ptr 40h
    xor dx,dx
    xor cx,cx
    mov ax,8600h
    mov cx,10
    int 15h

    call ClearScreen

    mov ax,4C00h
    int 21h
end start