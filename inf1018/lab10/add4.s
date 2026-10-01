/*
int f(int x);

void map2 (int* um, int * outro, int n) {
  int i;
  for (i=0; i<n; i++)               i=0   while(i<n)   
    *(outro+i) = f(*(um+i));    i++
}

Dicionario:
rdi - um
rsi - outro
edx - n
*/

.text
.globl
map2:
	#criar o RA da funcao chamada
	pushq %rbp 	#salva o RA da funcao chamadora
	movq %rsp, %rbp 	#RBP aponta p base do RA da f. chamada
	subq $???, %rsp	 #alocando espaco p RA da f. chamada (tem que ser mult 16) - é 16 mesmo?
    movl $0, %edi  #int i=0

WHILE:
    cmpl %edi, %edx    #comparando i com n
    jle FORA_WHILE     #se n for menor ou igual a i

    #salvando os reg que estou usando
    movq %rdi, -32(%rbp)
    movq %rsi, -24(%rbp)
    movl %edx, -16(%rbp)

    #*(um+i) tem que ir pro edi (primeiro arg int da f)
    #temp3=um
    movq %rdi, %r10

    #temp4=i (se i=2 e um=1000, entao i+um=1008)
    movq %edi, %r11d

    #multiplicando i por 4
    imul $4, %r11d

    #temp3=i+temp3
    addq %r11d, %r10

    #temp = *(temp3)
    movq (%r10), %edi

    #f(temp)
    call f

    #*(outro+i)
    addl %rdi, 
    movq %eax, 

    #restaurando os reg que usei
    movq -32(%rbp), %rdi
    movq -24(%rbp), %rsi
    movl -16(%rbp), %edx
    
    #i++




