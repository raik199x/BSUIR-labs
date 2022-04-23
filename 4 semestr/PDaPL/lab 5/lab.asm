.model small
.stack 100h
.data
    OutputFileName db "output.txt",0
    ;CreatedFile db "File was created",10,13,'$'

    buffer db 2 dup('$')
    AcceptedSymbols db 200 dup('$')
    IsAcceptedSymbols db 200 dup('$')
    FileLinesPassed dw 0
    FileDescriptorInput dw ?
    FileDescriptorOutput dw ?

    cmd_line   db 127 dup('$')
    cmd_Error_NoArgument db "No arguments provided",10,13,'$'


    Errors db "Error open file",10,13,'$'
    Error_way db "Such path wasn't found", 10,13,'$'
    Error_files db "Too many opened files",10,13,'$'
    Error_access db "Forbidden",10,13,'$'
    Error_file404 db "Such file not found",10,13,'$'
    Error_wrongAccess db "Wrong method connection",10,13,'$'
.code

jmp start

;psp 126 symbols max
CheckCommandLine proc

    mov ah, 62h
    int 21h
    push es
    mov es,bx
    mov bx,80h
    mov cl,es:bx

    mov ah,1
    xor ch,ch
    cmp cl,0
    je end_fun_check

    inc bx
    clear_freacking_spaces:
    mov al,es:bx
    cmp al,32
    jne cleared_freacking_spaces
    dec cl
    inc bx
    jmp clear_freacking_spaces
    cleared_freacking_spaces:

    copy_cmd_into_line:
    xor ch,ch
    xor si,si

    copy_letter:
    mov al,es:bx
    mov cmd_line[si],al
    inc si
    inc bx
    loop copy_letter
    mov cmd_line[si],0h
    mov ah,0

    end_fun_check:
    pop es
    ret
    CheckCommandLine endp

EnterAcceptSymbols proc
    xor si,si
    Enter_cont:
    mov ah,02h
    int 21h
    cmp ah,0Dh
    je Enter_exit
    mov AcceptedSymbols[si],al
    mov IsAcceptedSymbols[si],0
    inc si
    cmp si,199
    je Enter_exit
    jmp Enter_cont
    Enter_exit:
    ret
    EnterAcceptSymbols endp

InputThisLine proc
    push ax
    push bx
    push cx
    push dx

    xor cx,cx
    xor dx,dx
    ;moving to start of file
    mov bx,FileDescriptorInput
    mov ah,42h
    mov al,0
    int 21h

    ;now we need to find place where our line is strting
    mov dx,FileLinePassed
    running:
    mov cx,1        ;reading 1 symbol form file
    mov ah,3fh      ;mode for reading
    lea dx,buffer
    int 21h
    mov al,buffer[0] ;taking readed symbol
    cmp al,0Dh      ; if it is an enter
    jne this_is_not_end_of_line
    dec dx          ;(if enter) one lees line to run
    this_is_not_end_of_line:
    cmp dx,0        ;check if we already runned all our line
    jne running     ;if not, continue running

    lea dx,buffer   ;we will alwasy save symbol in buffer[0]
    ;at that moment we must write our needed lien, for that
    writting_data:
    mov bx,FileDescriptorInput  ;placing working descriptor input file
    mov ah,3fh                  ;setting mode for reading
    int 21h

    mov bx,FileDescriptorOutput ;placing working descriptor output file
    mov ah,40h                  ;setting write mode
    mov cx,1                    ;write one symbol
    int 21h

    mov al,buffer[0]            ;now we need to check what exactly we wrote
    cmp al,0Dh                  ;if its enter
    je end_writting             ;we leave funtion
    jmp writting_data           ;else continue

    end_writting:
    pop dx
    pop cx
    pop bx
    pop ax
    ret
    InputThisLine endp

RestoreLetters proc
    push si
    push ax
    xor si,si

    Restoring:
    mov al,IsAcceptedSymbols[si]
    cmp al,'$'
    je end_restoring
    mov IsAcceptedSymbols[si],0
    inc si
    jmp Restoring
    end_restoring:
    pop ax
    pop si
    ret
    RestoreLetters endp

EndOfLine proc
    push si
    xor ax,ax
    xor si,si
    mov al,0
    ;1 Check if we get all accept symbols
    checking_accept_moments:
    mov ah,IsAcceptedSymbols[si]
    cmp ah,'$'
    je end_checking_accept_moments
    cmp ah,0
    inc si
    jne checking_accept_moments
    mov al,1
    jmp end_checking_accept_moments
    end_checking_accept_moments:
    pop si
    ret
    EndOfLine endp

CheckLetters proc
    push si
    push ax
    push bx

    xor si,si
    xor ax,ax
    xor bx,bx

    mov bh,buffer[0]
    CheckingLettersAccept:
    mov bl,AcceptedSymbols[si]
    cmp bl,'$'
    je end_checking_letters
    cmp bh,bl
    jne Letters_are_not_equal
    mov IsAcceptedSymbols[si],1
    Letters_are_not_equal:
    inc si
    jmp CheckingLettersAccept

    end_checking_letters:
    pop bx
    pop ax
    pop si
    ret
    CheckLetters endp
start:
    mov ax,DGROUP
    mov ds,ax

    call CheckCommandLine
    cmp ah,1
    jne ArgumentsParsed
    mov ah,9
    lea dx,cmd_Error_NoArgument
    int 21h
    jmp __END__

    ArgumentsParsed:
    call EnterAcceptSymbols

    ;creating+open output file
    mov ah,5Bh
    mov cx,7
    lea dx,OutputFileName
    int 21h
    jc __Error_open_file__
    mov FileDescriptorOutput,ax

    ;opening existing file
    mov ah,3Dh
    mov al,00h
    lea dx,cmd_line
    int 21h
    jc __Error_open_file__
    mov FileDescriptorInput,ax

    mov bx,ax ;setting input file as active
    mov di,01

    reading_Data:
    mov cx,1
    mov ah,3fh      ;mode for reading
    lea dx,buffer
    int 21h
    jc Error_files_message
    ;
    mov cx,ax
    jcxz close_file ;to change in future

    xor ax,ax
    mov ah,buffer[0]
    cmp ah,0Dh
    jne Calling_checker_letter
    call EndOfLine
    cmp ah,0
    jne line_is_not_accepted
    ;if we need this line
    call InputThisLine
    call RestoreLetters
    jmp reading_Data
    line_is_not_accepted:
    call RestoreLetters ;we clear accept bits
    mov ax,FileLinePassed   ;*increasing
    inc ax                  ;*number
    mov FileLinePasse,ax    ;*of passe line
    jmp reading_Data        ;and continue to read data (ofc if can)
    Calling_checker_letter:
    call CheckLetter

    mov ah,40h
    xchg bx,di
    int 21h
    xchg di,bx
    jc close_file
    jmp reading_Data

close_file:
    mov ah,3Eh
    int 21h

    ;checkers
    jmp __END__

__Error_open_file__:
    push ax
    mov ah,9
    lea dx,Errors
    int 21h
    pop ax
    cmp ax,03h
    jne Error_files_message
    mov ah,9
    lea dx,Error_way
    int 21h
    lea dx,cmd_line
    int 21h
    jmp __END__
Error_files_message:
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

__END__:
    mov ax, 4C00h
    int 21h
    end start