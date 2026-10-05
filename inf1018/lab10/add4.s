/*
int f(int x);

void map2 (int *um, int *outro, int n) {
  int i;
  for (i = 0; i < n; i++)
    *(outro+i) = f(*(um+i));
}
*/
/*
Dicionário
Reg     Var
rdi     um      (1o argumento)
rsi     outro   (2o argumento)
edx     n       (3o argumento)
r10d    i
eax     temp
rcx     temp2 (endereco de outro+i)
*/

# não há .data porque não há variáveis globais nem strings constantes

.text

#void map2 (int *um, int *outro, int n) {
.globl map2
map2:
    # criar o RA da função chamada
    pushq   %rbp            # salvando o RA da função chamadora
    movq    %rsp, %rbp      # RBP aponta para a base do RA da função chamada
    subq    $32, %rsp       # 8+8+4+4 = 24 bytes, arredondado para 32

    # salvar os registradores callee-saved que serão usados
    # (nenhum: so uso rax, rcx, r10 e os registradores de argumento)

#   int i;  i = 0
    movl    $0, %r10d       # i = 0

#   while (i < n)           -> sai se n <= i
WHILE:
    cmpl    %r10d, %edx     # compara n com i
    jle     FORA_WHILE

#     temp = *(um + i)
    movl    %r10d, %eax     # eax = i
    imull   $4, %eax        # eax = 4 * i   (int ocupa 4 bytes)
    movslq  %eax, %rax      # rax = (long) eax
    addq    %rdi, %rax      # rax = endereco de um+i   (rdi intacto)
    movl    (%rax), %eax    # eax = *(um+i)

#     temp = f(temp)
    # salvar os registradores não callee-saved que estão em uso
    movq    %rdi,   -8(%rbp)    # salva um
    movq    %rsi,  -16(%rbp)    # salva outro
    movl    %edx,  -20(%rbp)    # salva n
    movl    %r10d, -24(%rbp)    # salva i

    # preparar os argumentos da função f
    movl    %eax, %edi      # 1o parametro: *(um+i)
    call    f               # eax = f(*(um+i))

    # restaurar os registradores não callee-saved salvos
    movq     -8(%rbp), %rdi     # recupera um
    movq    -16(%rbp), %rsi     # recupera outro
    movl    -20(%rbp), %edx     # recupera n
    movl    -24(%rbp), %r10d    # recupera i

#     *(outro + i) = temp
    movl    %r10d, %ecx     # ecx = i
    imull   $4, %ecx        # ecx = 4 * i
    movslq  %ecx, %rcx      # rcx = (long) ecx
    addq    %rsi, %rcx      # rcx = endereco de outro+i   (rsi intacto)
    movl    %eax, (%rcx)    # ESCRITA: *(outro+i) = f(*(um+i))

#   i++
    incl    %r10d
#   }
    jmp     WHILE
FORA_WHILE:

#   funcao void: nao ha valor de retorno em eax

#}
    # restaurar os registradores callee-saved que foram salvos
    # (nenhum)
    leave                   # destruir o RA e restaurar o RA da chamadora
    ret



    