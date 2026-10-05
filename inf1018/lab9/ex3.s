/*
void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}
*/
/*
Dicionário
Reg     Var
rdi     a       (1o argumento)
esi     n       (2o argumento)
r10d    i
r11d    s
rcx     temp (endereco de a[i])
eax     temp2 (valor de a[i])
*/

# não há .data porque não há variáveis globais nem strings constantes

.text

#void foo (int a[], int n) {
#void foo (int *a, int n) {
.globl foo
foo:
    # criar o RA da função chamada
    pushq   %rbp            # salvando o RA da função chamadora
    movq    %rsp, %rbp      # RBP aponta para a base do RA da função chamada
    # não preciso de subq: não há chamada de função nem variável na pilha

    # salvar os registradores callee-saved que serão usados
    # (nenhum: so uso rax, rcx, r10, r11 e os registradores de argumento)

#   int i;
#   int s = 0;
    movl    $0, %r11d       # s = 0

#   for (i=0; ...
    movl    $0, %r10d       # i = 0
FOR:
#   ... i<n; ...            -> sai do laco se n <= i
    cmpl    %r10d, %esi     # compara n com i
    jle     FORA_FOR

#     indexacao: endereco de a[i]
    movl    %r10d, %eax     # eax = i
    imull   $4, %eax        # eax = 4 * i   (int ocupa 4 bytes)
    movslq  %eax, %rcx      # rcx = (long) eax
    addq    %rdi, %rcx      # rcx = a + 4*i = endereco de a[i]   (rdi intacto)

#     temp2 = a[i]
    movl    (%rcx), %eax    # eax = a[i]   (leitura)

#     s += a[i];
    addl    %eax, %r11d     # s = s + a[i]

#     if (a[i] == 0)        -> pula o corpo do if se a[i] != 0
    cmpl    $0, %eax
    jne     FIM_IF

#       a[i] = s;
    movl    %r11d, (%rcx)   # a[i] = s   (escrita no mesmo endereco)

#       s = 0;
    movl    $0, %r11d

#     }
FIM_IF:

#   ... i++)
    incl    %r10d
#   }
    jmp     FOR
FORA_FOR:

#   funcao void: nao ha valor de retorno em eax

#}
    # restaurar os registradores callee-saved que foram salvos
    # (nenhum)
    leave                   # destruir o RA e restaurar o RA da chamadora
    ret

    