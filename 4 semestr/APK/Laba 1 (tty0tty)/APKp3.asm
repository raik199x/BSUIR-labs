.model small
.stack 100h
.data

.code
start:


mov dx,3F8h
mov al,'r'
out dx,al

mov al,'a'

mov dx,2F8h

in al,dx
mov ah,2
mov dl,al
int 21h

mov ax,4C00h
int 21h
end start

