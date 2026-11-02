/* double foo3(double *a, int n){ int i; double r=0;
     for(i=0;i<n;i++){ r += sin(*a); a++; } return r; }
   a: %rdi  n: %esi   retorno: %xmm0 (double)
   a, n, i e r precisam sobreviver a' chamada de sin -> tudo na pilha. */
.text
.globl foo3
foo3:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $32, %rsp
    movq  %rdi, -8(%rbp)      /* a */
    movl  %esi, -12(%rbp)     /* n */
    movl  $0,   -16(%rbp)     /* i = 0 */
    pxor  %xmm0, %xmm0
    movsd %xmm0, -24(%rbp)    /* r = 0.0 */
FOR:
    movl  -16(%rbp), %eax     /* i */
    cmpl  -12(%rbp), %eax     /* i < n ? */
    jge   FIM
    movq  -8(%rbp), %rax      /* a */
    movsd (%rax), %xmm0       /* *a (double) -> argumento de sin */
    call  sin                 /* xmm0 = sin(*a) */
    movsd -24(%rbp), %xmm1    /* r */
    addsd %xmm1, %xmm0        /* r + sin(*a) */
    movsd %xmm0, -24(%rbp)    /* r = ... */
    movq  -8(%rbp), %rax
    addq  $8, %rax            /* a++  (double = 8 bytes) */
    movq  %rax, -8(%rbp)
    movl  -16(%rbp), %eax
    addl  $1, %eax            /* i++ */
    movl  %eax, -16(%rbp)
    jmp   FOR
FIM:
    movsd -24(%rbp), %xmm0    /* return r */
    leave
    ret
