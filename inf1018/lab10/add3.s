/*
struct X {
  int val1;
  int val2;
};

int f(int i, int v);

void boo (struct X *px, int n, int val) {
  while (n--) {
    px->val2 = f(px->val1, val);
    px++;
  }
}
Dicionario
rdi -- px
esi -- n
edx -- val
*/

.text
.globl boo
boo:
	#criar o RA da funcao chamada
	pushq %rbp 	#salva o RA da funcao chamadora
	movq %rsp, %rbp 	#RBP aponta p base do RA da f. chamada

	#salvar os registradores callee-saved que serão usados (caso sejam usados)

	subq $16, %rsp	 #alocando espaco p RA da f. chamada (tem que ser mult 16) - é 16 mesmo?
    
WHILE:
    cmpl $0, %esi  #esi pq int n tem 4 bytes e é segundo argumento
    je FIM
    decl %esi      #n-- (tem q ser depois do je pq muda a flag)

    #salvando os registradores que estou usando (rdi, esi, edx)
    movq %rdi, -16(%rbp)
    movl %esi, -8(%rbp)
    movl %edx, -4(%rbp)

    #temp=f(px->val1, val) (-> indica que eu quero oq ta dentro do end)
    movl (%rdi), %edi  #rdi guarda o end da struct e eu qujero oq ta dentro do end que ele guarda
    movl %edx, %esi
    call f
    #restaurar os reg que usei
    movq -16(%rbp), %rdi
    movl -8(%rbp), %esi
    movl -4(%rbp), %edx
    # pegar o valor de retorno de f e colocar no px->val2
    movl %eax, 4(%rdi)  

    addq $8, %rdi  #8 pq é o tam da struct
    jmp WHILE

FIM:
    leave
    ret