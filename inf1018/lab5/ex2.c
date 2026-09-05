#include <stdio.h>
void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

struct X {
  int a;
  short b;
  int c;
} x;

int main(void) {
  // Inicializando com valores em hexadecimal para facilitar o reconhecimento
  x.a = 0xa1a2a3a4;
  x.b = 0xb1b2;
  x.c = 0xc1c2c3c4;

  printf("Tamanho da struct X: %lu bytes\n", sizeof(x));
  
  printf("Mapa de memoria da struct:\n");
  dump(&x, sizeof(x));

  return 0;
}