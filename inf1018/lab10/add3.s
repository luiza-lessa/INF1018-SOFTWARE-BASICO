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

	subq $???, %rsp	 #alocando espaco p RA da f. chamada (tem que ser mult 16) - é 16 mesmo?
    
WHILE:
    cmpl $0, %esi  #esi pq int n tem 4 bytes e é segundo argumento
    je FIM
    decl %esi      #n-- (tem q ser depois do je pq muda a flag)

    #salvando os registradores que estou usando (rdi, esi, edx)
    movq %rdi, %rbp
    movl %esi, %rbp
    pushl %edx

	#temp=px->val (?)

    #temp=f(px->val1, val) (?)


    call f

	#px++
    addq $8, %rbx
    jmp WHILE

FIM:
    leave
    ret