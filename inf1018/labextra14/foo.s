/*
Dicionario
a: %rdi (pint)   
b: %rsi (pdouble)   
n: %edx
*/

.text
.globl foo
foo:
    pushq %rbp
    movq  %rsp, %rbp
    movl  $2, %ecx
    cvtsi2sd %ecx, %xmm1

WHILE:
    cmpl $0, %edx
    je FIM
    subl  $1, %edx
    movl  (%rdi), %eax        /* *pint  (int) */
    cvtsi2sd %eax, %xmm0      /* (double)*pint */
    mulsd %xmm1, %xmm0
    movsd %xmm0, (%rsi)       /* *pdouble = ... */
    addq  $4, %rdi            /* pint++ */
    addq  $8, %rsi            /* pdouble++ */
    jmp   WHILE

FIM:
    ret
