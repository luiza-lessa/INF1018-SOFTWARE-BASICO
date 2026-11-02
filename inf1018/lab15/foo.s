/* float foo(double a, float b) { return (a+b)*(a-b); }
   a: %xmm0 (double)   b: %xmm1 (float)   retorno: %xmm0 (float)
   Sem chamadas -> nao preciso de pilha. 
*/

.text
.globl foo
foo:
    cvtss2sd %xmm1, %xmm1    /* b -> double */
    movsd    %xmm0, %xmm2    /* xmm2 = a */
    addsd    %xmm1, %xmm0    /* xmm0 = a + b */
    subsd    %xmm1, %xmm2    /* xmm2 = a - b */
    mulsd    %xmm2, %xmm0    /* xmm0 = (a+b)*(a-b)  (double) */
    cvtsd2ss %xmm0, %xmm0    /* -> float */
    ret