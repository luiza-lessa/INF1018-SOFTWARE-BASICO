.text
.globl add
add:
	#criar o RA da funcao chamada
	pushq %rbp 	#salva o RA da funcao chamadora
	movq %rsp, %rbp 	#RBP aponta p base do RA da f. chamada
	#salvar os reg callee-saved que serão usados
	movl $0, %r10d 	#int a=0

WHILE:
	cmpq $0, %rdi #pq sao 8 bytes
	je FORA_WHILE
	#temp=x->val
	#a+=temp
	movl (%rdi), %r11d
	addl %r11d, %r10d

	#x = x->next
	movq 8(%rdi), %rdi #q é por causa do tamanho de 8 bytes de next e p * é pq preciso andar 8 casas da struct p pegar o next
	jmp WHILE

FORA_WHILE:
	#return a
	movl %r10d, %eax

leave
ret
