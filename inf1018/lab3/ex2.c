#include <stdio.h>

int odd_ones(unsigned int x) {
  int n=0; //qtd de 1's
  for (i<33; i++){
    if x[i] == 1{
        n+=1;
    }
  }
  
}



int main() {
  printf("%x tem numero %s de bits\n",0x01010101,odd_ones(0x01010101) ? "impar":"par");
  printf("%x tem numero %s de bits\n",0x01030101,odd_ones(0x01030101) ? "impar":"par");
  return 0;
}