/*
int fat (int n) {
  if (n==0) return 1;
  else return n*fat(n-1);
}

DESMEMBRANDO ESSA FUNÇÃO PRA FACILITAR A TRADUÇÃO:
int fat(int n){
    if(n==0){
        temp=1;
    }
    else{
        temp=n;
        temp-=1;
        temp=fat(temp);
        temp=temp*n;
    }
    return temp;
}

Reg   Var
EDI   n
*/

#nao ha variaveis globais e nem strings constantes --> nao preciso do .data

.text
.globl fat
fat:
    pushq %rbp  #salva a base do RA da funcao chamadora e coloca o endereço do topo da pilha multiplo de 16!!!
    movq %rsp, %rbp  #cria a base do RA da funcao chamada
    subq $16, %rsp  #abre espaço no RA da função chamada
    #TODO: salvar os registradores callee-saved que vamos usar

    #if(n==0)
    cmpl $0, %edi
    jne ELSE

    #temp=1
    movl $1, %eax
    jmp FORA_IF


ELSE:
    #else
    #salvar todos os registradores nao callee-saved ainda em uso
    movl %edi, -4(%rbp)

    #temp = n
    movl %edi, %edi

    #temp-=1
    decl %edi

    #temp=fat(temp)
    call fat  #1o parametro EDI (antigo temp), valor de retorno EAX (novo temp)

    #restaurar os registradores não callee-saved salvos
    movl -4(%rbp), %edi

    #temp=temp*n
    imull %edi, %eax


FORA_IF:
    #return temp
    #nada a fazer pq o valor ja esta no EAX

    leave
    ret


