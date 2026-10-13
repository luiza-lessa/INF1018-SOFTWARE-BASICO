/*
int addl (int a[], int n);

int bemboba (int num) {
  int local[4];
  int *a;
  int i;

  for (i=0, a=local; i<4; i++) {
    *a = num;
    a++;
  }
  return addl (local, 4);
}
*/
/*
Dicionário
Reg         Var
edi         num     (1o argumento)
r10d        i
r11         a
-16(%rbp)   local[4]    (vetor local: 4 ints = 16 bytes, NAO cabe em registrador)

local[0] -> -16(%rbp)
local[1] -> -12(%rbp)
local[2] ->  -8(%rbp)
local[3] ->  -4(%rbp)
*/

.text
.globl bemboba
bemboba:
    # criar o RA da função chamada
    pushq   %rbp            # salvando o RA da função chamadora
    movq    %rsp, %rbp      # RBP aponta para a base do RA da função chamada
    subq    $16, %rsp       # espaço para o vetor local[4] (4 ints = 16 bytes)
    #for (i=0, a=local; ...
    movl    $0, %r10d       # i = 0
    leaq    -16(%rbp), %r11 # a = local   (leaq: calcula o endereço, não lê memória)
FOR:
    cmpl    $4, %r10d       # compara i com 4
    jge     FORA_FOR

    #*a = num;
    movl    %edi, (%r11)    # escreve num no endereço apontado por a

    #a++
    addq    $4, %r11        # a avança 1 int = 4 bytes

    #i++
    incl    %r10d
    jmp     FOR

FORA_FOR:
    leaq    -16(%rbp), %rdi # 1o parametro: endereço de local
    movl    $4, %esi        # 2o parametro: 4
    call    addl            # eax = addl(local, 4)
    # o valor de retorno de addl já está em eax: nada a fazer
    leave
    ret




    