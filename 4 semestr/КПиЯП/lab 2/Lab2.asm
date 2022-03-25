.model small
.stack 100h

.data
    FirstMessage db 10, 13, "Enter text: ", '$'
    SecondMessage db 10, 13, "Enter what to find: ",'$'
    ThirdMessage db 10, 13, "Enter with what replace: ", '$'
    MainLine db 400 dup('$')
    FindTo db 200 dup('$')
    ReplaceTo db 200 dup('$')
    size_mainline dw 0
    size_find_to dw 0
    size_replace_to dw 0
    start_word_pos dw -1
    ErrorMessage_overflow db "Task cannot be completed - the final result > 400 db", 10, 13, '$'
    ResultLine db 50 dup('-'), 10, 13, '$'
    TaskLine db 10, 13, "Task: ", '$'
    WordResult db 10, 13, "Result: ", '$'
.code
; ODh - enter
; 32 - space

New_line_output proc
    mov dl,10
    mov ah,2
    int 21h
    mov dl,13
    int 21h
    ret
New_line_output endp

inc_data proc
    inc di
    inc cx
    cmp start_word_pos, -1
    jne Ret_in_data
    mov start_word_pos, si
Ret_in_data:
    inc si
    ret
inc_data endp

ReplaceWord_equal proc
    mov si,start_word_pos
    xor di,di
Start_placing_equal:
    cmp ReplaceTo[di], '$'
    je End_placing_equal
    mov ah, ReplaceTo[di]
    mov MainLine[si], ah
    inc di
    inc si
    jmp Start_placing_equal
End_placing_equal:
    ret
ReplaceWord_equal endp

start:
    mov ax,DGROUP
    mov ds,ax
    ; output First message
    mov dx,offset FirstMessage
    mov ah,9
    int 21h
;++++++++++++++++++++++++Writting data++++++++++++++++++++++++++++
    xor si,si
    mov cx,1
    enter_line_text:
        mov ah,1
        int 21h
        cmp al,0Dh
        je out_loop_line_text
        mov MainLine[si],al
        inc si
        cmp si,199
        je out_loop_line_text
        inc cx
        inc size_mainline
    loop enter_line_text
out_loop_line_text:

    mov ah,9
    mov dx,offset SecondMessage
    int 21h

    mov cx,1
    xor si,si

    mov size_find_to, 0
    enter_line_FindTo:
        mov ah,1
        int 21h
        cmp al, 0Dh
        je out_loop_line_FindTo
        cmp al, 32
        je enter_line_FindTo
        mov FindTo[si],al
        inc si
        inc size_find_to
        cmp si,199
        je out_loop_line_FindTo

        inc cx
    loop enter_line_FindTo
out_loop_line_FindTo:

    mov ah,9
    mov dx,offset ThirdMessage
    int 21h


    mov cx,1
    xor si,si

    enter_line_ReplaceTo:
        mov ah,1
        int 21h
        cmp al, 0Dh
        je out_loop_line_ReplaceTo
        cmp al,32
        je enter_line_ReplaceTo
        mov ReplaceTo[si],al
        inc si
        inc size_replace_to
        cmp si,199
        je out_loop_line_ReplaceTo
        inc cx
    loop enter_line_ReplaceTo
out_loop_line_ReplaceTo:
;-------------------------------------Writting data---------------------------------
;si bx di -- could be used as indexes

;++++++++++++++++++++++++++Finding index++++++++++++++++++++++++++++++++++++++++++
xor si,si
xor di,di
mov cx,1
    index_find_MainLine:
    mov ax,size_find_to               ;Check if word was found
    cmp ax,di
    jne Not_Prepare_to_exit  ;If try to leave
Prepare_to_exit:
    mov ah,MainLine[si]
    cmp ah, 32
    je Out_index_find_MainLine ; if we found space or $ we could leave
    cmp ah, '$'
    je Out_index_find_MainLine
    inc si
    mov ah,MainLine[si]
    cmp ah, '$'
    je Out_index_find_MainLine
    dec si
    jmp cmp_not_true_index_find_MainLine ;fail to leave
Not_Prepare_to_exit:
    cmp MainLine[si], '$'       ;Check if this is a end of line
    je  Out_index_find_MainLine

    mov ah, FindTo[di]
    cmp MainLine[si], ah           ;check letter
    jne cmp_not_true_index_find_MainLine    
    call inc_data                           ;if true inc "points"
    loop index_find_MainLine 
    cmp_not_true_index_find_MainLine:       ;if not true
        xor di,di                           ;start again FindTo
        inc si                              ;next letter MainLine
        inc cx                              ;do not stop cycle
        mov start_word_pos, -1
        jmp index_find_MainLine             ;again in to the cycle
Out_index_find_MainLine:
;----------------------------Finding index----------------------------

    cmp start_word_pos, -1  ;if such word dosent exitst
    jne SimpleContProg
    jmp Last_words           ;we simply leave probgram
SimpleContProg:

;++++++++++++++++Replacing+++++++++++++++++++++++++++++++
    ;if words is equal we can simply overwrite it
    mov ax,size_find_to
    mov bx,size_replace_to
    cmp ax,bx
    jne Words_not_equal
    call ReplaceWord_equal
    jmp Last_words
Words_not_equal:
    ;if ReplaceTo is smaller than existing word
    jl ReplaceTo_is_bigger
    call ReplaceWord_equal
    ;we alse overwrite sentese and move line left
    mov di, start_word_pos
    add di, size_find_to
replacing_smaller:
    cmp MainLine[di], '$'
    mov ah,MainLine[di]
    mov MainLine[si],ah
    je Last_words
    inc si
    inc di
    jmp replacing_smaller
ReplaceTo_is_bigger:
    xor ax,ax
    mov ax,size_mainline
    add ax,size_replace_to
    sub ax,size_find_to
    cmp ax,400
    jl SimpleContReplacing
    jmp _END_OVERFLOW_
SimpleContReplacing:
    ;for that moment we will move our lie 'x' times so we can put our word using our function
    mov di,0
    mov cx,size_replace_to
    sub cx,size_find_to
    mov ax, size_replace_to
    move_mainline:
        mov si, start_word_pos
        add si, size_find_to
        add si, di
        mov ah,MainLine[si]
        mov MainLine[si], 32
        inc si
        swapper_start:
            mov bh,MainLine[si]
            mov MainLine[si],ah
            cmp ah,'$'
            je swapper_end
            mov ah,bh
            inc si
            jmp swapper_start
        swapper_end:
        inc di
    loop move_mainline
    call ReplaceWord_equal
;----------------Replacing-------------------------------

;+++++++++++++++Output+++++++++++++++++++++++++++++++++++
Last_words:
    call New_line_output
    mov ah,9
    lea dx, ResultLine
    int 21h

    mov ah,9
    lea dx,TaskLine
    int 21h
    lea dx,FindTo
    int 21h
    mov ah,2
    mov dx,32
    int 21h
    mov dx,'-'
    int 21h
    mov dx, '>'
    int 21h
    mov dx,32
    int 21h
    mov ah,9
    lea dx,ReplaceTo
    int 21h
    lea dx, WordResult
    int 21h
    lea dx,MainLine
    int 21h
    call New_line_output
_END_:
    mov ax,4C00h
    int 21h
_END_OVERFLOW_:
    mov ah,9
    lea dx, ErrorMessage_overflow
    int 21h
    mov ax,4C00h
    int 21h
;-------------Output-----------------------------------
end start