section .text
global _start
_start:
	mov eax, dword[B]
	imul dword[H]
	jbe ResultOverflow
	cdq
	idiv dword[DOIS]
	mov dword[R], eax
	push dword[R]
	call EscreverInteiro
	add esp, 4
sys_exit:
	mov eax, 1
	mov ebx, 0
	int 80h
EscreverChar:
	enter 0, 0
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, [EBP+8]
	mov edx, 1
	int 80h
	popa
	leave
	ret
EscreverString:
	enter 0, 0
	push eax
	mov eax, [EBP+8]
escrita_string:
	push eax
	call EscreverChar
	add esp, 4
	add eax, 4
	cmp dword[eax], 0
	jnz escrita_string
	pop eax
	add esp, 4
	leave
	ret
EscreverInteiro:
	enter 0, 0
	pusha
	mov ebx, esp
	mov eax, [EBP+8]
	mov edx, 0
	push edx
	mov ecx, 10
escrita_inteiro:
	mov edx, 0
	div ecx
	add edx, 0x30
	push edx
	cmp eax, 0
	jnz escrita_inteiro
	push esp
	call EscreverString
	add esp, 4
	mov esp, ebx
	popa
	leave
	ret
ResultOverflow:
	push OVERFLOW
	call EscreverString
	add esp, 4
	jmp sys_exit

section .data
OVERFLOW dd 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w'
DOIS dd 2

B resd 1
H resd 1
R resd 1
