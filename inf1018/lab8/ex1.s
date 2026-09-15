  
  
  .data
  s1: .string "ls -ls"

  .text
  .globl main
  main:
  /* prologo */
    pushq %rbp
    movq  %rsp, %rbp

  /* coloque seu codigo aqui */
  movq  $s1, %rdi  # carrega o endereço do s1 em rdi (1º arg)
  movl  $0, %eax   # padrao do printf - tem que vir antes do call
  call  system     # chama a função system da biblioteca padrão (libc)

  /* finalizacao */
    leave
    ret

