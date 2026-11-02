#include <stdio.h>
#include <math.h>
float foo1(float a, float b);
int main(void){
    printf("foo1(0, 1)      = %.4f  (sin0 + 1   = 1)\n",      foo1(0.0f, 1.0f));
    printf("foo1(PI/2, 2)   = %.4f  (sin(PI/2)+2 = 3)\n",     foo1((float)M_PI_2, 2.0f));
    printf("foo1(PI, 5)     = %.4f  (sin(PI) + 5 = 5)\n",     foo1((float)M_PI, 5.0f));
    return 0;
}