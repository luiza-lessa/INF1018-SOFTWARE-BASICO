/* double boo(double v)   v: %xmm0   retorno: %xmm0
   Pilha:  a2 -> -32(%rbp)   a1 -> -48(%rbp)   v -> -56(%rbp)   i -> -60(%rbp) */
.text
.globl boo
boo:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $64, %rsp
    movsd %xmm0, -56(%rbp)     /* salva v (sobrevive a' chamada de foo) */

    /* a1[4] = {1,2,3,4} */
    movl  $1, -48(%rbp)
    movl  $2, -44(%rbp)
    movl  $3, -40(%rbp)
    movl  $4, -36(%rbp)

    /* foo(a1, a2, 4) */
    leaq  -48(%rbp), %rdi      /* &a1[0]  (1o parametro) */
    leaq  -32(%rbp), %rsi      /* &a2[0]  (2o parametro) */
    movl  $4, %edx             /* 4       (3o parametro) */
    call  foo

    /* for(i=0;i<4;i++) v += a2[i]; */
    movl  $0, -60(%rbp)        /* i = 0 */
    movsd -56(%rbp), %xmm0     /* recupera v; acumulo em xmm0 */
FOR2:
    cmpl  $4, -60(%rbp)        /* i < 4 ? */
    jge   FIM2
    movl  -60(%rbp), %eax      /* i */
    imull $8, %eax             /* 8*i  (double = 8 bytes) */
    movslq %eax, %rax          /* (long) */
    leaq  -32(%rbp), %rdx      /* &a2[0] */
    addq  %rax, %rdx           /* &a2[i] */
    movsd (%rdx), %xmm1        /* a2[i] */
    addsd %xmm1, %xmm0         /* v += a2[i] */
    addl  $1, -60(%rbp)        /* i++ */
    jmp   FOR2
FIM2:
    leave                      /* xmm0 ja tem o resultado */
    ret