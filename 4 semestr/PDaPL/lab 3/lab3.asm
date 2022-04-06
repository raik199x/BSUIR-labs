.model small
.stack 100h
.data
    BorderFrom dw ?
    BorderTo dw ?
    Massiv dw 30 dup(0)
    WelcomeMessage db 10,13, "Enter num ", '$'
    FirstBorderMessage db 10,13, "From: ", '$'
    SecondBorderMessage db 10,13, "To: ", '$'
    OverflowMessage db "  Overflow detected, number changed to 0", 10,13,'$'
    ResultMessage db 10,13, "Result: ", '$'

.code
    jmp start
    
    ;notificates user that he got bufer overflow
    BufOverflow proc
        mov ah,9
        lea dx,OverflowMessage
        int 21h
        ret
    BufOverflow endp

    ;function to write nums
    WriteNum proc
        xor di,di
        xor ax,ax   ;
        xor bx,bx   ;   clearing
        xor cx,cx   ;
        mov bx,10   ;use for dividing by 10
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
        call BufOverflow
        mov cx,0
        ret
    WriteNum endp

    ;output num located in cx
    OutputNum proc
        cmp cx,0        ;for 0 in cx we have alternative way to output    
        je OutputZero
        xor bx,bx   ;
        mov ax,cx   ;   clearing mess
        xor cx,cx   ;
        StartDivOutput:
        cmp ax,0    ;if FOR SOME REASON in ax will be 0, we should leave
        je OutFunOutput
        xor dx,dx   ;we also FOR SOME F*****G reason should clear dx befor div
        mov bx,10   ;divide by 10
        div bx
        add dx,30h  ;make it become a letter
        push dx     ;remember what num we've got
        inc cx      ;increasing number of nums that we should get from stack
        jmp StartDivOutput
    ;simple output nums
    OutFunOutput:
        mov ah,2
        pop dx
        int 21h
    loop OutFunOutput
        ret
    OutputZero:
        mov ah,2
        mov dx,cx
        add dx,30h
        int 21h
        ret
    OutputNum endp

start:
    mov ax,DGROUP
    mov ds,ax
    mov si,0
    mov di,1
    ;++++++++++++++++++++++Enter array++++++++++++++++++++++++
    WrittingData:
    mov ah,9
    lea dx,WelcomeMessage
    int 21h
    mov cx,di       ;
    call OutputNum  ;
    mov dx,':'
    int 21h
    mov dx, 32
    int 21h
    push di
    call WriteNum
    pop di
    mov Massiv[si],cx
    inc si
    inc si
    inc di
    cmp di,30
    jle WrittingData
    ;----------------------Enter array------------------------
    ;++++++++++++++++++++++++Enter from/to++++++++++++++++++++
    mov ah,9
    lea dx,FirstBorderMessage
    int 21h
    call WriteNum
    mov ah,9
    mov BorderFrom,cx
    lea dx,SecondBorderMessage
    int 21h
    call WriteNum
    mov BorderTo,cx
    ;-----------------------Enter from/to---------------------

    ;+++++++++++++++++++++++Logic+++++++++++++++++++++++++++++
    xor si,si
    xor di,di
    xor cx,cx
    mov bx,0
    StartSearching:
    cmp di,30
    je Good_end
    mov cx, Massiv[si]
    inc si
    inc si
    inc di
    mov ax,BorderFrom
    cmp cx, ax
    jl StartSearching
    mov ax, BorderTo
    cmp cx, ax
    jg StartSearching
    inc bx
    jmp StartSearching
    ;-----------------------Logic-----------------------------
Good_end:
    mov ah,9
    lea dx,ResultMessage
    int 21h
    mov cx,bx
    call OutputNum
    mov ax,4C00h
    int 21h
end start