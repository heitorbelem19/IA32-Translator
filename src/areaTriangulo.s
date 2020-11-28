section .text
global _start
_start:
	push NUM
	call LerString
	add esp, 4
	mov eax, dword[BASE]
	imul dword[ALTURA]
	cdq
	idiv dword[DOIS]
	mov dword[RES], eax
sys_exit:
	mov eax, 1
	mov ebx, 0
	int 80h
LerChar:
	enter 0, 0
	pusha
	mov eax, 3
	mov ebx, 0
	mov ecx, [EBP+8]
	mov edx, 1
	int 80h
	popa
	leave
	ret
LerString: 
	enter 0, 0
	pusha
	mov eax, 0
leitura:
	mov ecx, [EBP+8]
	mov ebx, eax
	shl ebx, 2
	add ecx, ebx
	push ecx
	call LerChar
	add esp, 4
	inc eax
	cmp dword[ecx], 0xa
	jne leitura
	mov dword[ecx], 0
	popa
	add esp, 4
	leave
	ret

section .data
BASE dd 3
ALTURA dd 4
DOIS dd 2

section .bss
RES resd 1
NUM resd 1
