.386P


descr struc    ;дескриптор сегмента
    lim 	dw 0;предел
    base_1 	dw 0;адрес база младшая
    base_m 	db 0;адрес база средняя
    attr_1	db 0; атрибут 1(бит присутствия, уровень привилегий(2 
				;бита),идентификатор сегмента(1),тип(4 бита))
    attr_2	db 0; атрибут 2(бит дробности, бит размера операндов, 0, бит 
				;avl не используется,старшие 4 бита лимита)
    base_h 	db 0;адрес база старшая
descr ends

gate struc  ;дескриптор шлюза(таблица прерываний)
	offs_1	dw 0 	;смещения младшая часть
	sel		dw 16 	;селектор сегмента
	cntr	db 0	;счетчик 5 бит
	dtype	db 8Eh	; атрибут == атрибуту дескриптора сегмента
	offs_h	dw 0	;смещения старшая часть
gate ends  

data segment use16
	;global descriptor table - талица глобальных дескрипторов
	gdt_null 	descr <0, 0, 0, 0, 0, 0>
	gdt_data 	descr <data_size - 1, 0, 0, 92h, 0, 0>
	gdt_code 	descr <code_size - 1, 0, 0, 98h, 0, 0>
	gdt_stack 	descr <255,,, 92h,,>
	gdt_screen 	descr <3999, 8000h, 0Bh, 92h, 0, 0>
	
	gdt_size = $ - gdt_null  ;размер таблицы gdt
	;таблица дескрипторов исключений
	idt label word
	db 32 dup ( 8 dup (0))
	gate <new_08,,8Eh> ; irq0
	gate <new_09,,8Eh> ; irq1
	idt_size=$-idt     

	pdescr df 0
	sym db 1
	attr db 1Eh
	master db 0
	slave db 0
	count db 0
	time dw 0
	time_msg db 'T', 1Eh, 'i', 1Eh, 'm', 1Eh, 'e', 1Eh, ':', 1Eh, ' ', 1Eh
	time_buffer db '0', 1Eh, '0', 1Eh, '0', 1Eh, '0', 1Eh, '0', 1Eh
	time_buffer_line_size equ 11
	time_buffer_pos dw 1840
	scan_msg db ' ', 1Eh, ' ', 1Eh, ' ', 1Eh, ' ', 1Eh, ':', 1Eh, ' ', 1Eh
	scan_buffer db '0', 1Eh, '0', 1Eh
	scan_buffer_line_size equ 8
	scan_buffer_pos dw 1680
	scancode db 0A9h
	sixteen db 16
	ten dw 10
	sign dw ?
	numqwerty dw 0
	videobuffer_line_size db 160
	quit_flag db 0
	num_err_msg db 0Dh,0Ah,"Imput is incorrect. Try again",0Dh,0Ah,'$'  

	msg db "Return!",0Dh,0Ah,'$'
	data_size = $ - gdt_null

data ends    

text segment use16
    assume CS:text, DS:data
textseg label word  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; обработчик IDQ0
new_08 proc 

	add numqwerty, 1
	mov di, scan_buffer_pos + 50
	push offset time_buffer + 20
	call putnum
	inc count
	cmp count, 20
	jne end_irq0
	mov count, 0
	dec time
	cmp time, 0
	jne not_quit
	mov quit_flag, 1
not_quit:
	push offset time_buffer + 8
	call puttime
	
	mov cx, time_buffer_line_size 
	mov di, time_buffer_pos
	mov si, offset time_msg
render1:	
	lodsw
	stosw
	loop render1

end_irq0:	;EOI
	mov al, 20h
	out 20h, al
	db 66h
	iret
new_08 endp  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IRQ1
new_09 proc
	;add numqwerty, 1
	;mov di, scan_buffer_pos + 50
	;push offset time_buffer + 20
	;call putnum	
	in al, 60h
	cmp al, 01h
	jne nqu
	mov quit_flag, 1
nqu:
	mov scancode, al
	mov di, offset scan_buffer
	call putscan
	mov cx, 8
	mov di, scan_buffer_pos
	mov si, offset scan_msg
render:
	lodsw
	stosw
	loop render
	;EOI
	mov al, 20h
	out 20h, al
	db 66h
	iret
new_09 endp  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main proc
	xor eax, eax
	mov ax, data
	mov ds, ax
					; ввод времени нахождения в защищенном режиме
	push dx
	call getnum
	mov time, dx
	pop dx
					; настройка таймера
	mov al, 00110110b ; 1 канал режим 2(автозагрузка)
	out 43h, al
	mov ax, 59659 	; 20 Гц 1193180 / 59659
	out 40h, al
	shr ax, 8
	out 40h, al 

					; загрузка линейного адреса сегмента данных
	xor eax, eax
	mov ax, data  

	shl eax, 4
	mov ebp, eax
	mov bx, offset gdt_data
	mov [bx].base_1, ax
	shr eax, 16
	mov [bx].base_m, al 

					; загрузка линейного адреса сегмента кода
	xor eax, eax
	mov ax, cs
	shl eax, 4
	mov bx, offset gdt_code
	mov [bx].base_1, ax
	shr eax, 16
	mov [bx].base_m, al   

				; загрузка линейного адреса сегмента стека
	xor eax, eax
	mov ax, ss
	shl eax, 4
	mov bx, offset gdt_stack
	mov [bx].base_1, ax
	shr eax, 16
	mov [bx].base_m, al  

	mov dword ptr pdescr+2, ebp ; линейный базовый адрес gdt
	mov word ptr pdescr, gdt_size-1 ; предел gdt
	lgdt pdescr  ; загрузка gdtr

				; запрет прерываний
	cli         

				; сохранение масок прерываний
	in al, 21h
	mov master, al
	in al, 0A1h
	mov slave, al    

; инициализация ведущего контроллера прерываний
	mov al, 11h
	out 20h, al
	mov al, 20h ; адрес векторов
	out 21h, al
	mov al, 4h ; ведомый к 2
	out 21h, al
	mov al, 1h ; eoi
	out 21h, al
	mov al, 0FCh ; маска прерываний ведущего
	out 21h, al 

	mov al, 0FFh ; маска прерываний ведомого
	out 0A1h, al 

; загрузка idt
	mov word ptr pdescr, idt_size-1
	xor eax, eax
	mov ax, offset idt
	add eax, ebp
	mov dword ptr pdescr+2, eax
	lidt pdescr 
;Открываем А20
	mov al, 0D1h
	out 64h, al 
	mov al, 0DFh 
	out 60h, al
 
		; переход в защещенный режим 
			mov eax, cr0
			or eax, 1
			mov cr0, eax 

; загрузка селектора(указатель на описание сегмента, номер дескриптора)
	; в сегмент кода
	db 0EAh
	dw offset continue
	dw 16
continue:
	; загрузка селектора в сегмент данных
	mov ax, 8
	mov ds, ax
	; загрузка селектора в сегмент стека
	mov ax, 24
	mov ss, ax
	; загрузка селектора в расширенный сегмент
	mov ax, 32
	mov es, ax
; разрешение прерываний
	sti     

	mov sp, 100h

	wait:
	cmp quit_flag, 1
	jne wait
; запрет прерываний
	
; настройка теневых регистров сегментов для работы в реальном режиме
	mov gdt_data.lim, 0FFFFh
	mov gdt_code.lim, 0FFFFh
	mov gdt_stack.lim, 0FFFFh
	mov gdt_screen.lim, 0FFFFh
	push ds
	pop ds
	push ss
	pop ss
	push es
	pop es
; загрузка селектора cs
	db 0EAh
	dw offset go
	dw 16 

;Закрыть линию A20
	mov al, 0D1h 
	out 64h ,al 
	mov al, 0DDh 
	out 60h, al 
go:
; переход в реальный режим
			mov eax, cr0
			and eax, 0FFFFFFFEh
			mov cr0, eax
; востановление сегментов
	db 0EAh
	dw offset return
	dw text
return:
	mov ax, data
	mov ds, ax
	mov ax, stk
	mov ss, ax
	mov sp, 256       

	; восстановление старой таблицы векторов прерываний
	mov ax, 3FFh
	mov word ptr pdescr, ax
	mov eax, 0
	mov dword ptr pdescr+2, eax
	lidt pdescr 

	; переиницализация ведущего контроллера
	mov al, 11h
	out 20h, al
	mov al, 8
	out 21h, al
	mov al, 4
	out 21h, al
	mov al, 1
	out 21h, al
	mov al, 0fCh
	out 21h, al 

	; восстановление масок прерываний
	mov al, master
	out 21h, al
	mov al, slave
	out 0A1h, al
	sti
	mov ah, 09h
	mov dx, offset msg
	int 21h   

	; завершение
	mov ax, 4C00h
	int 21h  
main endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
putscan proc ; перевод числа в hex с занесение в память
	pusha
	xor ah, ah
	mov al, scancode
	div sixteen
	mov cx, 2
hex: cmp al, 10
	jl digit
	sub al, 10
	add al, 'A'
	jmp ins
digit:
	add al, '0'
	ins:
	mov [di], al
	add di, 2
	mov al, ah
	loop hex
	popa
	ret 
putscan endp  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getnum proc ; считывание числа
	push ax
	push bx
	push cx  

skip_spaces:
	call getchar 
	cmp al, ' '
	je skip_spaces
	cmp al, 0Dh
	je skip_spaces
	cmp al, 08h
	je backspace
	cmp al, '-'
	jne positive_num
	mov sign, -1
	call getchar
	mov dx, 0
	jmp start_getnum
positive_num:
	mov sign, 1
	mov dx, 0
start_getnum:
	cmp al, 08h
backspace:
	jne digit_check
	push dx
	mov ah,03h ; определяем текущее
	int 10h ; положение курсора
	push dx ; сохраняем эту позицию
	push ' '
	call putchar
	pop dx ; возвращаем позицию
	mov ah,02h ; возвращаем курсор9
	int 10h
	pop dx
	mov ax, dx
	mov dx, 0
	div ten
	mov dx, ax
	cmp dx, 0
	jz skip_spaces
	jmp getsymb
digit_check:
	cmp al, '0'
	jb num_error
	cmp al, '9'
	ja num_error
	push ax
	mov ax, dx
	mul ten
	mov dx, ax
	pop ax
	jo num_error
	mov ah, 0 
	sub ax, '0'
	add dx, ax
	mov ax, dx
	jo num_error
getsymb:
	call getchar 
	cmp al, ' '
	jz end_getnum
	cmp al, 0Dh
	jz end_getnum
	jmp start_getnum
num_error:
	cmp ax, -32768
	je minimal_num  

	push offset num_err_msg
	call putstr 
	jmp skip_spaces
minimal_num:
	xor ax, ax
	cmp sign, -1
	jne num_error
	mov sign, 1
	jmp getsymb
end_getnum:
	cmp sign, -1
	jne get_stack
	neg dx 

get_stack:
	pop cx
	pop bx
	pop ax
	ret
check_min:
	cmp sign, -1
	jne num_error
	mov ax, dx
	imul sign
	mov dx, ax	
	sub dx, ten
	jmp get_stack
endp        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
puttime proc 
	push bp
	mov bp, sp
		push ax
		push cx
	push dx
	push di
	mov di, [bp + 4]
	mov ax, time
start_putnum:
	mov dx, 0
	div ten
	add dx, '0'
	mov [di], dl

	sub di, 2
	cmp ax, 0
	jnz start_putnum
	mov dl, '0'
	mov [di], dl
	pop di
	pop dx
	pop cx
	pop ax
	mov sp, bp
	pop bp
	ret 2
endp  



putnum proc 
	push bp
	mov bp, sp
		push ax
		push cx
	push dx
	push di
	mov di, [bp + 4]
	mov ax, numqwerty
start_putnum11:
	mov dx, 0
	div ten
	add dx, '0'
	mov [di], dl

	sub di, 2
	cmp ax, 0
	jnz start_putnum11
	mov dl, '0'
	mov [di], dl
	pop di
	pop dx
	pop cx
	pop ax
	mov sp, bp
	pop bp
	ret 2
endp 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
putstr proc ; вывод строки
	str equ [bp + 4]
	push bp
	mov bp, sp
	push ax
	push dx
	mov ah, 09h
	mov dx, str
	int 21h
	pop dx
	pop ax
	; mov sp, bp
	pop bp
	ret 2
endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
putchar proc ; вывод символа
	symbol equ [bp + 4]
	push bp
	mov bp, sp
	push ax
	push dx
	mov ah, 06h
	mov dx, symbol
	int 21h
	pop dx
	pop ax
	; mov sp, bp
	pop bp
	ret 2
endp       
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchar proc ; ввод символа
	mov ah, 01h
	int 21h
	ret
endp 

	code_size = $ - textseg
	text ends
	stk segment stack use16
	db 256 dup ('^')
	stk ends
end main
