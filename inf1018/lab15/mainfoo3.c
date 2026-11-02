#include <stdio.h>
#include <math.h>
double foo3(double *a, int n);
int main(void){
    double v[] = { 0.0, M_PI_2, M_PI, M_PI_2 };   /* sin: 0, 1, 0, 1 */
    printf("foo3(v, 4) = %.4f  (0 + 1 + 0 + 1 = 2)\n", foo3(v, 4));
    printf("foo3(v, 2) = %.4f  (0 + 1         = 1)\n", foo3(v, 2));
    printf("foo3(v, 0) = %.4f  (vazio         = 0)\n", foo3(v, 0));
    return 0;
}