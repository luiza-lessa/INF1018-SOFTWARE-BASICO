/* main.c — testa a funcao fat escrita em assembly */
#include <stdio.h>

int fat(int n);   /* implementada em ex2.s */

int main(void) {
    int i;
    for (i = 0; i <= 12; i++)
        printf("fat(%2d) = %d\n", i, fat(i));
    return 0;
}

