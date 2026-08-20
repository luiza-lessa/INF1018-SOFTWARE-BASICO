#include <stdio.h>

int deslocabitmenos(int x){
    //comparar com o 0x000000FF, fazendo um "e" bit a bit
    int a = 0x000000FF;
    int b = x&a;
    return b;
}

int deslocabitmais(int x){
    //comparar com o 0x000000FF, fazendo um "e" bit a bit
    int a = 0xFF000000;
    int b = x|a;
    return b;
}

int main(void) {
  unsigned int x = 0x87654321;
  unsigned int y, z;

  /* o byte menos significativo de x e os outros bits em 0 */
  y = deslocabitmenos(x);

  /* o byte mais significativo com todos os bits em '1' 
     e os outros bytes com o mesmo valor dos bytes de x */
  z = deslocabitmais(x);

  printf("%08x %08x\n", y, z);
}