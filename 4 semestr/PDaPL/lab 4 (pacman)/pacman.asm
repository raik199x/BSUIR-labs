.model small
.stack 100h
.data
    __map__ db 29 dup('#')                                                                                              ;1
            db '#', 27 dup(32), '#'                                                                                     ;2
            db '#',32,'#','#',32,32,'#',32,32,32,'#','#',32,3 dup('#'),32,'#','#',32,'#','#',32,32, 3 dup('#'),32,'#'   ;3
            db '#',32,32,'#',32,32,'#',32,32,32,'#',32,32,32,'#',32,32,'#',32,'#',32,'#',32,32,'#',32,32,32,'#'         ;4
            db '#',32,32,'#',32,32,'#',32,32,32,'#','#',32,32,'#',32,32,'#',32,32,32,'#',32,32,'#','#','#',32,'#'       ;5
            db '#','#',4 dup(32),'#',32,32,32,'#',32,32,32,'#',32,32,'#',32,32,32,'#',32,32,'#',32,32,32,'#'            ;6
            db '#','#',4 dup(32),3 dup('#'),32,'#','#',32,32,'#',32,32,'#',32,32,32,'#',32,32,'#','#','#',32,'#'        ;7
            db '#',9 dup(32),'#',32,32,32,'#',32,32,'#',10 dup(32),'#'                                                  ;8
            db '#',9 dup(32),'#',32,32,32,'#',32,32,32,'#',5 dup(32),'#','#',32,32,'#'                                  ;9
            db '#',32,32,32,'#','#',4 dup(32),'#',32,32,32,'#',6 dup(32),'#',32,32,'#',32,32,32,'#'                     ;10
            db '#',5 dup(32),'#',32,32,32,'#',32,32,32,'#',6 dup(32),'#',32,32,'#',32,32,32,'#'                         ;11
            db '#',32,32,'#',32,'#',13 dup(32),'#','#','#',32,32,'#','#','#',32,'#'                                     ;12
            db '#',32,'#',32,'#',32,32,32,'#',7 dup(32),'#',4 dup(32),'#',6 dup(32),'#'                                 ;13
            db '#',32,'#',6 dup(32),7 dup('#'),9 dup(32),'#',32,32,'#'                                                  ;14
            db '#',32,32,'#',8 dup(32),4 dup('#'),32,32,32,5 dup('#'),32,'#','#',32,'#'                                 ;15
            db '#',32,'#',10 dup(32),4 dup('#'),6 dup(32),'#',32,'#',32,32,'#'                                          ;16
            db '#',13 dup(32),'#','#',32,32,'#',32,32,'#',32,'#',32,'#',32,32,'#'                                       ;17
            db '#',32,'#','#',32,32,3 dup('#'),32,3 dup('#'),6 dup(32),'#',32,'#',32,'#',32,'#','#',32,'#'              ;18
            db '#',32,'#','#','#',32,32,'#',32,32,'#',4 dup(32),'#','#',6 dup(32),'#',32,32,'#',32,'#'                  ;19
            db '#',32,'#','#','#',32,32,'#',32,32,3 dup('#'),32,32,3 dup('#'),3 dup(32),4 dup('#'),32,'#',32,'#'       ;20
            db '#',32,'#','#','#',32,32,'#',32,32,'#',4 dup(32),'#','#',9 dup(32),'#',32,'#'                            ;21
            db '#',32,'#','#',32,32,3 dup('#'),32,'#','#','#',5 dup(32),'#',32,32,'#',32,'#','#',32,32,32,'#'           ;22
            db '#','C',26 dup(32),'#'                                                                                   ;23
            db 29 dup('#')                                                                                              ;24
    pacman_pos dw ?
    apples db ?
    score  db 0
    logo db "Made by raik199x" ;length == 16
    word_score db "Score: " ;length == 7

.code 
; 696 whole map
; whole screen 80x25
; 42 - apples
; heart == apple
jmp start

    ClearScreen macro
        mov ax,0003h ;; setting graphic mode
        int 10h      ;; + clearing screen
    endm

New_line_output proc
    mov dl,10
    mov ah,2
    int 21h
    mov dl,13
    int 21h
    ret
    New_line_output endp

PrintMap proc

    push 0B800h ; code of vider driver
    pop es      ; setting codes

    xor si,si
    xor di,di
    xor ax,ax
    mov cx,696

    start_line:
        mov bl,__map__[si]
        mov es:di,bx
        inc di

        ;decide which color to set
        cmp bx,'*'
        jne check_if_space
        mov es:di,word ptr 32
        check_if_space:
        cmp bx,32
        je decided_color
        cmp bx,'#'
        jne is_pacman
        mov es:di,word ptr 1Fh
            is_pacman:
        cmp bx,'C'
        jne is_pacman_2
        mov es:di,word ptr 0Eh
        push si
        pop pacman_pos
            is_pacman_2:
        cmp bx,'O'
        jne decided_color
        push si
        pop pacman_pos
        mov es:di,word ptr 0EH
        decided_color: ;decided =)
        inc si
        inc di
        inc ax
        cmp ax,29
        jne continue_output_map
        xor ax,ax
        add di,102
        continue_output_map:
    loop start_line
    ret
    PrintMap endp

Check_access_point proc
    xor dx,dx
    cmp bl,'#'
    jne bl_not_equal_border
    add dx,1
    bl_not_equal_border:
    cmp bl,'*'
    jne __out_function_check_access__
    mov apples,0
    mov bl,score
    inc bl
    mov score,bl
    __out_function_check_access__:
    ret
    Check_access_point endp

PlaceNumScore proc
    xor cx,cx
    mov di,256
    push ax
    mov al,score
    check_if_can_output_num_score:
    xor bx,bx
    mov bl,10
    cmp al,9
    jl  pre_output_num_score
    div bl
    mov bl,ah
    push bx
    inc cx
    jmp check_if_can_output_num_score
    pre_output_num_score:
    push ax
    inc cx
    output_num_score:
    pop ax
    add ax,30h
    mov es:di,ax
    inc di
    mov es:di,word ptr 0Fh
    inc di
    loop output_num_score

    pop ax
    ret
    PlaceNumScore endp

Random proc
    push cx
    push dx
    push ax
    start_random:
    xor ax,ax
    mov ah,2Ch
    int 21h

    xor ax,ax
    mov ax,cx
    add ax,dx
    mov bx,2
    divide_by_two_random:
    xor dx,dx
    div bx
    cmp ax,666
    jg divide_by_two_random
    cmp ax,31
    jg set_random_point
    mul bx

    set_random_point:
    mov si,ax
    mov bl,__map__[si]
    cmp bl,'#'
    je start_random
    cmp bl,'C'
    je start_random

    ;happy end
    mov bl,'*'
    mov __map__[si],bl

    pop ax
    pop dx
    pop cx
    ret
    Random endp

start:
    mov ax,DGROUP
    mov ds,ax
    ClearScreen
    call PrintMap

    ;+++preparations++++++++
        ;drawing logo + score
    mov cx,16
    mov si,0
    mov di,86
    drawing_logo:
        mov bl,logo[si]
        mov es:di,bx
        inc di
        mov es:di,word ptr 5h
        inc di
        inc si
    loop drawing_logo
    mov cx,7
    xor si,si
    mov di,240
    drawing_score:
        mov bl,word_score[si]
        inc si
        mov es:di,bx
        inc di
        mov es:di,word ptr 0Fh
        inc di
    loop drawing_score
    ;---end preparations-----

    mov apples,0
    ;GAME STARTED
    RE_MAP:
    call PlaceNumScore
    ;mov cx,0
    ;mov dx,40000
    ;mov ah,86h
    ;int 15h
    ;++++++++place apple++++++++
    mov bl,apples
    cmp bl,0
    jne apples_are_set
    inc bl
    mov apples,bl
    call Random
    apples_are_set:
    ;--------place apple---------

    ;mov ah,1     ;функция проверки клавиатуры (ожидание нажатие на любую клавишу)
    ;int 16h
    ;jnz key_was_pressed
    ;jmp Print_Map
    ;key_was_pressed:
    xor al,al 
    mov ah,00h  ;enter symbol
    int 16h

    cmp ah,1h   ;if esc == leave program
    jne cont_esc_not_pressed
    jmp __END__
    cont_esc_not_pressed:
    xor si,si
    ;+++++++++++++++++++++++CHECKING MOVEMENT+++++++++++++++++++++++++
    ;Move up
    cmp al,'w'
    jne enterned_down
    mov si,pacman_pos
    sub si,29
    mov bl,__map__[si]
    call Check_access_point
    cmp dx,1
    jge enterned_down
    ;++++Move up true
    mov __map__[si],'C'
    mov si,pacman_pos
    mov __map__[si],32
    jmp Print_Map
    ;----Move up true

    enterned_down:
    cmp al,'s'
    jne enterned_left
    mov si,pacman_pos
    add si,29
    mov bl,__map__[si]
    call Check_access_point
    cmp dx,1
    jge enterned_left
    ;++++Move down true
    mov __map__[si],'C'
    mov si,pacman_pos
    mov __map__[si],32
    jmp Print_Map
    ;----Move down true

    enterned_left:
    cmp al,'a'
    jne enterned_right
        mov si,pacman_pos
    sub si,1
    mov bl,__map__[si]
    call Check_access_point
    cmp dx,1
    jge enterned_right
    ;++++Move left
    mov __map__[si],'C'
    mov si,pacman_pos
    mov __map__[si],32
    jmp Print_Map
    ;----Move left

    enterned_right:
    cmp al,'d'
    jne Print_Map
    mov si,pacman_pos
    add si,1
    mov bl,__map__[si]
    call Check_access_point
    cmp dx,1
    jge Print_Map
    ;+++++Move right
    mov __map__[si],'C'
    mov si,pacman_pos
    mov __map__[si],32
    jmp Print_Map
    ;-----Move right
    ;-----------------------------CHECKING MOVEMENT-------------------------------
    Print_Map:
    call PrintMap
    jmp RE_MAP

    __END__:
    ClearScreen
    mov ax,4C00h
    int 21h
end start