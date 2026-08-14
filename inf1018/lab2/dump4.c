#include <stdio.h>
#include <ctype.h> //necessário para poder usar a função isdigit()

int string2num (char *s, int base) {
  int a = 0;
  int valor_digito = 0;

  for (; *s; s++) {
    //verifica se o caractere atual é um número (0 a 9)
    if (isdigit(*s)) {
      valor_digito = *s - '0';
    } 
    //se não for número, é uma letra minúscula (a a z)
    else {
      //subtrair 'a' zera a letra (ex: 'c' - 'a' = 2). 
      //somamos 10 porque, a partir da base 11, 'a' vale 10, 'b' vale 11, etc.
      valor_digito = (*s - 'a') + 10;
    }
    
    //multiplica o acumulado pela base informada e soma o novo dígito
    a = a * base + valor_digito;
  }
  
  return a;
}

int main (void) {
  //chamadas de teste exigidas pelo laboratório
  printf("==> %d\n", string2num("1a", 16));   //esperado: 26
  printf("==> %d\n", string2num("a09b", 16)); //esperado: 41115
  printf("==> %d\n", string2num("z09b", 36)); //esperado: 1633307
  
  return 0;
}

