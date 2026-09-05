#include <stdio.h>
void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  int i,j;
  short a[2][3];
  int b[2];

  for (i=0;i<2;i++) {
    b[i] = i;
    for (j=0;j<3;j++)
      a[i][j] = 3*i+j;
  }

  0x7ffecf11b633 - 00
0x7ffecf11b634 - 04
0x7ffecf11b635 - 00
0x7ffecf11b636 - 05
0x7ffecf11b637 - 00

  return 0;
}