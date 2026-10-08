/*
int novonum(void) {
  int minhalocal;
  printf("numero: ");
  scanf("%d",&minhalocal);
  return minhalocal;
}

Dicionario:
-4(%rbp) - minhalocal
*/

.data
Sf: .string "%d"
Sp: .string "numero: "
.text
.globl novonum
novonum:
    pushq %rbp
    movq %rsp,%rbp
    subq $16, %rsp

    #printf (???)
    movq    $Sp, %rdi # 1o parametro: endereco da string
    movl    $0, %eax  # printf
    call    printf

    movq $Sf, %rdi
    leaq -4(%rbp), %rsi
    call scanf 

    movl -4(%rbp) , %eax
    leave 
    ret

