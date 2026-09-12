#include <stdio.h>

int main(void) {
    int i; //variável local p iterar de 1 a 10

    for (i = 1; i <= 10; i++) {
        //calcula o quadrado multiplicando a variável por ela mesma e imprime
        printf("O quadrado de %d eh: %d\n", i, i * i);
    }

    return 0;
}

