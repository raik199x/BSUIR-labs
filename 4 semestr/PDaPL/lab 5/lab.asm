.model small
.386
.stack 100h
.data
    EnteringAcceotSymbolsMessage db 10,13,"enter symbols to accept: ",'$'
    SymbolWriteError db "Symbol wasnt written",10,13,'$'

    OutputFileName db "output.txt",0
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
    mov ah,9
    lea dx,EnteringAcceotSymbolsMessage
    int 21h
    Enter_cont:
    mov ah,01h
    int 21h
    cmp al,0Dh
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

    ;now we need to find place where our line is starting
    mov dx,FileLinesPassed
    cmp dx,0                ;if there is no need to run again through file
    je writting_data        ;just jump into writting
    running:
    mov cx,1        ;reading 1 symbol form file
    mov ah,3fh      ;mode for reading
    push dx         ;remember data in dx
    lea dx,buffer
    int 21h
    pop  dx         ;retreaving data for dx
    mov al,buffer[0] ;taking readed symbol
    cmp al,0Ah      ;if FOR SOME REASON 0dh is missing, we should check 0ah
    jne is_it_0dh
    dec dx
    jmp this_is_not_end_of_line
    is_it_0dh:
    cmp al,0Dh      ;another variation of enter
    jne this_is_not_end_of_line
    mov cx,1        ;*
    mov ah,3fh      ;*
    push dx         ;*
    lea dx,buffer   ;*
    int 21h         ;*skipping 0Ah symbol
    pop dx
    dec dx          ;(if enter) one lees line to run
    this_is_not_end_of_line:
    cmp dx,0        ;check if we already runned all our line
    jne running     ;if not, continue running
    ;at that moment we must write our needed lien, for that
    writting_data:
    lea dx,buffer   ;we will alwasy save symbol in buffer[0]
    mov bx,FileDescriptorInput  ;placing working descriptor input file
    mov cx,1
    mov ah,3fh                  ;setting mode for reading
    int 21h

    cmp cx,ax
    jne endfile_input

    mov bx,FileDescriptorOutput ;placing working descriptor output file
    mov ah,40h                  ;setting write mode
    mov cx,1                    ;write one symbol
    int 21h

    mov al,buffer[0]            ;now we need to check what exactly we wrote
    cmp al,0Ah                  ;another variation of enter
    je end_writting             ;another leave
    jmp writting_data           ;else continue

    end_writting:
    jmp full_end_fun
    endfile_input:
    mov bx,FileDescriptorOutput
    mov ah,40h
    mov cx,1
    mov al,0Dh
    mov buffer[0],al
    lea dx,buffer
    xor al,al
    int 21h
    full_end_fun:
    pop dx
    pop cx
    pop bx
    pop ax
    mov bx,FileDescriptorInput
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
    cmp ah,1
    je not_set_false_return
    set_false_return:
    mov al,1
    jmp end_checking_accept_moments
    not_set_false_return:
    inc si
    jmp checking_accept_moments
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

    call CheckCommandLine       ;parsing arguments of command line
    cmp ah,1                    ;(result) if it failed (==1)
    jne ArgumentsParsed         ;(if not failed) skip to main program
    mov ah,9                    ;set mode output
    lea dx,cmd_Error_NoArgument ;set message output
    int 21h
    jmp __END__                 ;(if failed) end program

    ArgumentsParsed:
    call EnterAcceptSymbols     ;entering symbols that we need to accept

    ;creating+open output file
    mov ah,3ch
    mov cx,7
    lea dx,OutputFileName
    int 21h
    jc __Error_open_file__
    mov ah,3Dh
    mov al,01h
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

    reading_Data:   ;Start reading symbols
    mov cx,1        ;read one symbol
    mov ah,3fh      ;mode for reading
    lea dx,buffer   ;symbol will be stored in buffer[0]
    int 21h
    jc Error_files_message
    ;
    mov cx,ax
    jcxz check_end_of_file ;if we reached limit of file

    xor ax,ax
    mov ah,buffer[0]            ;checking symbol
    cmp ah,0Ah                  ;if FOR SOME REASON 0dh is mssing, we should firstly check 0ah
    jne is_it_0dh_main
    call EndOfLine
    cmp al,0
    jne line_is_not_accepted
    call InputThisLine          ;(if all letters where used) We run function to write line
    call RestoreLetters         ;set to 0 written symbols
    mov ax,FileLinesPassed      ;*also increasing
    inc ax                      ;*number
    mov FileLinesPassed,ax      ;*of passed lines
    jmp reading_Data            ;start again
    is_it_0dh_main:
    cmp ah,0Dh                  ;enter symbol
    jne Calling_checker_letter
    ;after 0Dh goes symbol 0Ah, so we need to skip him
    mov cx,1        ;*
    mov ah,3fh      ;*
    lea dx,buffer   ;*
    int 21h         ;*skipped
    call EndOfLine              ;(if enter) we check if all letter were used
    cmp al,0                    ;(result from function EndOfLine)
    jne line_is_not_accepted    ;if its not eqaul to 0, means not all letters where used
    ;if we need this line
    call InputThisLine          ;(if all letters where used) We run function to write line
    call RestoreLetters         ;set to 0 written symbols
    mov ax,FileLinesPassed      ;*also increasing
    inc ax                      ;*number
    mov FileLinesPassed,ax      ;*of passed lines
    jmp reading_Data            ;start again
    line_is_not_accepted:
    call RestoreLetters     ;we clear accept bits
    mov ax,FileLinesPassed   ;*increasing
    inc ax                  ;*number
    mov FileLinesPassed,ax    ;*of passed lines
    jmp reading_Data        ;and continue to read data (ofc if can)
    Calling_checker_letter:
    call CheckLetters

    jmp reading_Data

check_end_of_file:
    call EndOfLine              ;(if enter) we check if all letter were used
    cmp al,0                    ;(result from function EndOfLine)
    jne close_file
    call InputThisLine
close_file:
    mov ah,3Eh
    int 21h
    mov bx,FileDescriptorOutput
    mov ah,3Eh
    int 21h
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