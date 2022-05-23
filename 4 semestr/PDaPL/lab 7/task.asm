.model small
.stack 100h
.386
.data
    programName db "HW.EXE",0
	cmd_line db 127 dup ('$')
    
    ErrorMessage db "an error was occured",10,13,'$'
    ErrorBlockdestroyed db "memory control block destroyed",10,13,'$'
    ErrorNotEnoughMemory db "not enought memory", 10, 13,'$'
    ErrorEsWrong db "ES contains invalid adress",10,13,'$'
    ErrorFileNot db "file was not found",10,13,'$'
    ErrorNoPremission db "access to file is restricted",10,13,'$'
    ErrorEnviroment db "wrong enviroment was setted",10,13,'$'
    ErrorFormat db "wrong format was setted",10,13,'$'
    ErrorZero db "0 is not an option",10,13,'$'
    ErrorMore db "more than 255 is not acceptable",10,13,'$'
    ErrorCmd db "empty command line",10,13,'$'
	ErrorBadConvert db "symbols in command line",10,13,'$'
    UnErrorCmd db "unexpected error in cmd",10,13,'$'
    
    EPB dw 0000 ;dos-environment
        dw offset commandline,0
        dw 005Ch,006Ch
    commandline db 125
                db " /?"
    commandtext db 122 dup(?)
.code

    jmp start

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
    mov cmd_line[si],'$' ;file name should be ended with 0, so we put 0h
    mov ah,0
    
    end_fun_check:
    pop es
    ret
    CheckCommandLine endp
    
	ConvertToNum proc
		push si
		push bx
		
		xor ax,ax
		xor cx,cx
		xor si,si
		mov bx,10
		converting:
		mov al,cmd_line[si]
		cmp al,'$'
		je end_convert
		sub al,30h
		cmp al,9
		ja bad_convert
		cmp al,0
		jb bad_convert
		xchg ax,cx
		mul bx
		xchg ax,cx
		add cx,ax
        inc si
		jmp converting
		end_convert:
		cmp cx,255
		ja bad_convert
		cmp cx,0
		jb bad_convert
		je bad_convert
		
		pop bx
		pop si
		ret
		bad_convert:
		pop bx
		pop si
		mov ah,1
		ret
		ConvertToNum endp
start:
    mov ax,DGROUP
    mov ds,ax

	call CheckCommandLine
	cmp ah,1
	je EmptyCmd
	call ConvertToNum
	cmp ah,1
	je ErrorCmdx
	
    cmp cx,0
    jne continue_check
    mov ah,9
    lea dx,ErrorZero
    int 21h
    jmp __END__
    continue_check:
    cmp cx,255
    jng loop_start
    mov ah,9
    lea dx,ErrorMore
    int 21h
    jmp __END__
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
	jmp __END__
EmptyCmd:
	mov ah,9
	lea dx,ErrorCmd
	int 21h
    jmp __END__
ErrorCmdx:
  mov ah,9
  lea dx,UnErrorCmd
  int 21h
__END__:
    mov ax,4C00H
    int 21h

    program_length equ $-start
    end start
