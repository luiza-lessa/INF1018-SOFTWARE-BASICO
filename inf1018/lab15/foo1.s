/* float foo1(float a, float b){ return sin(a) + b; }
   a: %xmm0 (float)  b: %xmm1 (float)   retorno: %xmm0 (float)
   b precisa sobreviver a' chamada de sin -> guardo na pilha. */
.text
.globl foo1
foo1:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movss %xmm1, -4(%rbp)     /* salva b */
    cvtss2sd %xmm0, %xmm0     /* a -> double (argumento de sin) */
    call  sin                 /* xmm0 = sin(a) (double) */
    movss -4(%rbp), %xmm1     /* recupera b */
    cvtss2sd %xmm1, %xmm1     /* b -> double */
    addsd %xmm1, %xmm0        /* sin(a) + b (double) */
    cvtsd2ss %xmm0, %xmm0     /* -> float */
    leave
    ret