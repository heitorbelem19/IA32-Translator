section .text
global _start
_start:
	mov eax, dword[B]
	imul dword[H]
	jmp L1
	cdq
	idiv dword[DOIS]
	mov dword[R], eax
L1:
	mov eax, 1
	mov ebx, 0
	int 80h

section .data
OVERFLOW dd 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w'
B dd 10
H dd 4
DOIS dd 2

section .bss
R resd 5
