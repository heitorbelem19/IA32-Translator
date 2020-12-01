section .text
global _start
_start:
	mov eax, dword[B]
	imul dword[H]
	jbe ResultOverflow
	cdq
	idiv dword[DOIS]
	mov dword[R], eax
sys_exit:
	mov eax, 1
	mov ebx, 0
	int 80h
ResultOverflow:
	push OVERFLOW
	call EscreverString
	add esp, 4
	jmp sys_exit

section .data
OVERFLOW dd 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w'
B dd 10
H dd 4
DOIS dd 2

R resd 5
