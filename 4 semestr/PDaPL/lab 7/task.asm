.model small
.stack 100h
.data
    programName db "HW.EXE",0
    Times db 10,13,"Times to run program: ",'$'

    ErrorMessage db "an error was occured",10,13
    ErrorBlockdestroyed db "memory control block destroyed",10,13
    ErrorNotEnoughMemory db "not enought memory", 10, 13
    ErrorEsWrong db "ES contains invalid adress",10,13
    ErrorFileNot db "file was not found",10,13
    ErrorNoPremission db "access to file is restricted",10,13
    ErrorEnviroment db "wrong enviroment was setted",10,13
    ErrorFormat db "wrong format was setted",10,13

    EPB dw 0000 ;dos-environment
        dw offset commandline,0
        dw 005Ch,006Ch
    commandline db 125
                db " /?"
    commandtext db 122 dup(?)
.code

    jmp start

    ;function to write nums (from last labs)
    WriteNum proc
        xor di,di
        xor ax,ax   ;
        xor bx,bx   ;   clearing
        xor cx,cx   ;
        mov bx,10   ;use for dividing by 10

        mov ah,9
        lea dx, Times
        int 21h

        StartLoopEnter:
            mov ah,1
            int 21h
            xor ah,ah
            ;checker 'enter' and letters+
            cmp al,0Dh
            je OutFunWrite
            cmp al,'-'
            je MinusTrue
            cmp al, '0'
            jl OutFunWrite
            cmp al,'9'
            jg OutFunWrite
            jmp ContLoopEnter
            MinusTrue:
            cmp dx,1    ;if minus was already written we leave function as letter error
            je RetFunWrite
            mov dx,1    ;otherwise we set flag to make number negative in future
            jmp StartLoopEnter
            ContLoopEnter:
            ;checker 'enter' and letters-
                ;additional check of overflow
                cmp di,4
                jne ContinueAddingNum
                sub al,30h
                cmp dx,1
                je NegativeChecker
                cmp al,7
                jg CallBuffOverflow
                add al,30h
                jmp ContinueAddingNum
                NegativeChecker:
                cmp al,8
                jg CallBuffOverflow
                add al,30h
                jmp ContinueAddingNum
            ContinueAddingNum:
            xchg ax,cx  ;because mul command multiply number in ax, we swap two atributes
            push dx     ;remember number in dx (negative flag)
            imul bx     ;multiply by 10 (actually there is no need in imul, we could use just mul)
            pop dx      ;returning negative flag
            jo  CallBuffOverflow
            xchg ax,cx  ;returning normal number in cx
            sub al,30h  
            add cx,ax   ;adding number to cx
            inc di
            jmp StartLoopEnter
    OutFunWrite:
        cmp dx,1        ;if negative flag was set
        jne RetFunWrite
        xor dx,dx
        neg cx          ;we make negative num
    RetFunWrite:
        ret
    CallBuffOverflow:
        mov cx,0
        ret
    WriteNum endp
start:
    mov ax,DGROUP
    mov ds,ax

    call WriteNum
    loop_start:
    mov ah,4Ah
    shift = program_length + 300h
    mov sp, program_length + 300h
    mov bx,shift shr 4 + 1
    int 21h
    jc ErrorHandling

    ;prepare EPB
    mov ax,cs
    mov word ptr EPB+4, ax
    mov word ptr EPB+8, ax
    mov word ptr EPB+0Ch, ax


    mov ax,4B00h
    lea dx, programName
    lea bx, EPB
    int 21h
    jc ErrorHandling
    loop loop_start
    jmp __END__

ErrorHandling:
    push ax
    mov ah,9
    lea dx,ErrorMessage
    int 21h
    pop ax
    ;first possible message
    cmp ax,07
    jne Error4ah_2
    mov ah,9
    lea dx,ErrorBlockdestroyed
    int 21h
    jmp __END__
Error4ah_2:
    cmp ax,08
    jne Error4ah_3
    mov ah,9
    lea dx,ErrorNotEnoughMemory
    int 21h
    jmp __END__
Error4ah_3:
    cmp ax,09
    jne Error4bh_1
    mov ah,9
    lea dx,ErrorEsWrong
    int 21h
    jmp __END__
Error4bh_1:
    cmp ax,02h
    jne Error4bh_2
    mov ah,9
    lea dx,ErrorFileNot
    int 21h
    jmp __END__
Error4bh_2:
    cmp ax,05h
    jne Error4bh_3
    mov ah,9
    lea dx,ErrorNoPremission
    int 21h
    jmp __END__
Error4bh_3:
    cmp ax,08h
    jne Error4bh_4
    mov ah,9
    lea dx,ErrorNotEnoughMemory
    int 21h
    jmp __END__
Error4bh_4:
    cmp ax,0Ah
    jne Error4bh_5
    mov ah,9
    lea dx,ErrorEnviroment
    int 21h
    jmp __END__
Error4bh_5:
    cmp ax,0Bh
    jne __END__
    mov ah,9
    lea dx,ErrorFormat
    int 21h
__END__:
    mov ax,4C00H
    int 21h

    program_length equ $-start
    end start