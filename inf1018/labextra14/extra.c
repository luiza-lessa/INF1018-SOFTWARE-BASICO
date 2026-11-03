#include <stdio.h>

void foo (int a[], double b[], int n) {
  int *pint;
  double *pdouble;

  for (pint = a, pdouble = b; n-- ; pint++, pdouble++) {
    *pdouble = (double)*pint * 2.0;
  }
}

double boo (double v) {
  int a1[4] = {1,2,3,4};
  double a2[4];
  int i;

  foo(a1, a2, 4);
  for (i = 0; i < 4; i++)
    v += a2[i];
  return v;
}

int main() {
  printf("%f\n",boo(1.0));
  return 0;
}