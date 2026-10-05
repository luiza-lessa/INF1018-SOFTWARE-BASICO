/*
int add (int a, int b, int c) {
  return a+b+c;
}
*/
/*
Dicionário
Reg     Var
edi     a       (1o argumento)
esi     b       (2o argumento)
edx     c       (3o argumento)
eax     temp
*/

# não há .data porque não há variáveis globais nem strings constantes

.text

#int add (int a, int b, int c) {
.globl add
add:
    # criar o RA da função chamada
    pushq   %rbp            # salvando o RA da função chamadora
    movq    %rsp, %rbp      # RBP aponta para a base do RA da função chamada
    # não preciso de subq: não há chamada de função nem variável na pilha

    # salvar os registradores callee-saved que serão usados
    # (nenhum)

#   return a+b+c;
#   temp = a
    movl    %edi, %eax      # eax = a
#   temp = temp + b
    addl    %esi, %eax      # eax = a + b
#   temp = temp + c
    addl    %edx, %eax      # eax = a + b + c   (valor de retorno)

#}
    # restaurar os registradores callee-saved que foram salvos
    # (nenhum)
    leave                   # destruir o RA e restaurar o RA da chamadora
    ret

    