#include <stdio.h>
/* Experimento: NAO declaramos o prototipo de foo de proposito. */
int main(void) {
    double a = 3.0; float b = 1.0f;
    /* esperado (a+b)*(a-b) = (4)*(2) = 8.0 */
    printf("foo(3.0, 1.0) = %f\n", foo(a, b));
    return 0;
}