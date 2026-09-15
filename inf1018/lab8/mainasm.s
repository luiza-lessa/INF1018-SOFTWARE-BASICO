  .data
  nums: .int 3, -5, 7, 8, -2
  s1:   .string "%d\n"

  .text
  .globl main
  main:
  /* prologo */
     pushq %rbp
     movq  %rsp, %rbp
     subq  $16, %rsp
     movq  %rbx, -8(%rbp)
     movq  %r12, -16(%rbp)

  /* coloque seu codigo aqui */
  movl $0, %ebx
  movq $nums, %r12

  loop_inicio:
   cmpl $5, %ebx #compara i com 5
   jge loop_fim  #se i>=5 vai pra fora do loop_inicio
   movl (%r12), %edi      # 1º argumento (EDI): valor apontado por p (*p)
   movl $1, %esi          # 2º argumento (ESI): valor do LIM (que é 1)
   call filtro            # retorno da função ficará em EAX

   movl %eax, %esi        # 2º argumento
   movq $s1, %rdi         # 1º argumento
   movl $0, %eax
   call printf

   add $4, %r12
   incl %ebx
   jmp loop_inicio

  loop_fim:
   movl $0, %eax

  /* finalizacao */
     movq -8(%rbp), %rbx
     movq -16(%rbp), %r12
     leave
     ret