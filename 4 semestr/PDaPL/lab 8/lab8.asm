.model small
.stack 100h
.data
  old_int dd 0
  testLine db "interrupt was used",10,13,'$'
  cmd_line db 127 dup('$')
  CmdError db "No arguments passed", 10,13,'$'
  temp db ?

  Error_way db "Such path wasn't found", 10,13,'$'
  Error_files db "Too many opened files",10,13,'$'
  Error_access db "Forbidden",10,13,'$'
  Error_file404 db "Such file not found",10,13,'$'
  Error_wrongAccess db "Wrong method connection",10,13,'$'
.code
start:
jmp installing_new_handler

;psp 126 symbols max
CheckCommandLine proc
    ;getting psp block (nor really necessary, but shit can happen)
    mov ah, 62h
    int 21h     ;get block
    push es
    mov es,bx   ;and send it to es
    mov bx,80h  ; [80h] - amount of symbols in args (like argc in C\C++)
    mov cl,es:bx

    mov ah,1
    xor ch,ch
    cmp cl,0            ;if there is no symbols
    je end_fun_check    ;leave the function

    inc bx
    clear_freacking_spaces: ;*there is a thing, that arguments also parse all enters
    mov al,es:bx            ;*so a just clear all starting spaces
    cmp al,32               ;*others maybe necessary (not really, because path would be broken)
    jne cleared_freacking_spaces
    dec cl                  ;dont forget to decrease amount of symbols to be readen
    inc bx
    jmp clear_freacking_spaces
    cleared_freacking_spaces:

    copy_cmd_into_line:
    xor ch,ch
    xor si,si

    copy_letter:
    mov al,es:bx        ;moving from argument line
    mov cmd_line[si],al ;and saving for our file name
    inc si
    inc bx
    loop copy_letter
    mov cmd_line[si],0h ;file name should be ended with 0, so we put 0h
    mov ah,0

    end_fun_check:
    pop es
    ret
    CheckCommandLine endp

int_handler proc far
  ;preparations
  pushf
  ;call cs:old_int
  push ds
  push es
  push ax
  push si
  ;main part (do not use dos interruptions)
  xor si,si

  output_message:
  mov al,testLine[si]
  cmp al, '$'
  je end_output_message
  mov ah, 0Eh
  int 10h
  inc si
  jmp output_message
  end_output_message:
  
  mov ah,00h
  int 16h
  ;recovering
  pop si
  pop ax
  pop es
  pop ds
  popf
  iret
  int_handler endp

installing_new_handler:
  ;saving old interrupt
  mov ax, 3523h
  int 21h
  mov word ptr old_int, bx
  mov word ptr old_int+2, es

  ;placing new interrupt
  mov ax,2523h
  push cs
  pop ds
  lea dx, int_handler
  int 21h

  mov ax, DGROUP
  mov ds,ax
  call CheckCommandLine
  cmp ah,1
  je BadParseCMD

  mov ah,3Dh
  mov al,00h
  lea dx,cmd_line
  int 21h
  jc __ErrorHandling_Files__
  mov bx,ax

  reading_data: ;main part of prog
  mov cx,1
  mov ah,3fh
  lea dx,temp
  int 21h
  jc __ErrorHandling_Files__
  mov cx,ax
  jcxz end_reading_file
  mov ah,02h
  mov dl,temp
  int 21h

  ;slowing program
  mov cx,0010h
  mov dx,0000h
  mov ah,86h
  int 15h
  jmp reading_data

  end_reading_file:
  mov ah,3Eh
  int 21h

  jmp __END__
;error handling
BadParseCMD:
  mov ah,9
  lea dx,CmdError
  int 21h
  jmp __END__
__ErrorHandling_Files__:
    cmp ax,04h
    jne Error_forbid
    mov ah,9
    lea dx,Error_files
    int 21h
    jmp __END__
Error_forbid:
    cmp ax,05h
    jne Error_no_file
    mov ah,9
    lea dx,Error_forbid
    int 21h
    jmp __END__
Error_no_file:
    cmp ax,02h
    jne Error_wrong_method_connection
    mov ah,9
    lea dx,Error_file404
    int 21h
    lea dx,cmd_line
    int 21h
Error_wrong_method_connection:
    cmp ax,0Ch
    jne __END__
    mov ah,9
    lea dx,Error_wrongAccess
    int 21h
    jmp __END__
__END__:
  ;recover interrupt
  mov bx, word ptr old_int
  mov es, word ptr old_int+2
  mov ax, 2523h
  int 21h
  ;leave program
  mov ax, 4C00h
  int 21h

  end start
