/*
int foo (int x) {
  return x+1;
}
Dicionario
edi - x

*/

.text
.globl foo
foo:
    pushq %rbp
    movq %rsp, %rbp

    movl %edi, %eax
    incl %eax

    leave
    ret

    decl %eax
