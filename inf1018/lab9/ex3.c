/* main.c — testa a funcao foo escrita em assembly */
#include <stdio.h>

void foo(int a[], int n);   /* implementada em ex3.s */

void imprime(const char *rotulo, int a[], int n) {
    int i;
    printf("%s", rotulo);
    for (i = 0; i < n; i++)
        printf(" %d", a[i]);
    printf("\n");
}

int main(void) {
    int a[] = {3, 0, 5};
    int b[] = {1, 2, 0, 4, 0, 7};
    int c[] = {0, 0, 0};
    int d[] = {9, 8, 7};

    imprime("antes: ", a, 3);
    foo(a, 3);
    imprime("depois:", a, 3);

    imprime("antes: ", b, 6);
    foo(b, 6);
    imprime("depois:", b, 6);

    imprime("antes: ", c, 3);
    foo(c, 3);
    imprime("depois:", c, 3);

    imprime("antes: ", d, 3);
    foo(d, 3);
    imprime("depois:", d, 3);

    return 0;
}


