/* double foo2(float a, float b){ return sin(a) + cos(b); }
   a: %xmm0 (float)  b: %xmm1 (float)   retorno: %xmm0 (double)
   Guardo b antes de sin, e o resultado de sin antes de cos. */
.text
.globl foo2
foo2:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp
    movss %xmm1, -4(%rbp)     /* salva b */
    cvtss2sd %xmm0, %xmm0     /* a -> double */
    call  sin                 /* xmm0 = sin(a) */
    movsd %xmm0, -16(%rbp)    /* salva sin(a) */
    movss -4(%rbp), %xmm0     /* b */
    cvtss2sd %xmm0, %xmm0     /* b -> double */
    call  cos                 /* xmm0 = cos(b) */
    movsd -16(%rbp), %xmm1    /* recupera sin(a) */
    addsd %xmm1, %xmm0        /* sin(a) + cos(b) */
    leave
    ret