.text
.globl add2
add2:
	#criar o RA da funcao chamada
	pushq %rbp 	#salva o RA da funcao chamadora
	movq %rsp, %rbp 	#RBP aponta p base do RA da f. chamada
    subq $16, %rsp	 #alocando espaco p RA da f. chamada (tem que ser mult 16)

	cmpq $0, %rdi #pq sao 8 bytes
	jne ELSE
    #return 0
    movl $0, %eax
    jmp FIM

ELSE:
	#temp = x->next
	movq 8(%rdi), %r11 #q é por causa do tamanho de 8 bytes de next e p é pq preciso andar 8 casas da struct p pegar o next
    movq %rdi, -8(%rbp) #salvando os reguistradores
    movq %r11, %rdi #movendo o primeiro param p rdi
    call add2
    movq -8(%rbp), %rdi #restaurando os regs salvos
    addl (%rdi), %eax

FIM:
    leave
    ret
