#include <stdio.h>
#include <math.h>
double foo2(float a, float b);
int main(void){
    printf("foo2(PI/2, 0)   = %.4f  (sin(PI/2)+cos0   = 2)\n", foo2((float)M_PI_2, 0.0f));
    printf("foo2(0, PI)     = %.4f  (sin0 + cos(PI)   = -1)\n", foo2(0.0f, (float)M_PI));
    printf("foo2(PI/2, PI)  = %.4f  (1 + (-1)         = 0)\n", foo2((float)M_PI_2, (float)M_PI));
    return 0;
}