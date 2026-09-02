#include <stdio.h>
#include <time.h>

long int fib_recursivo(long int n) {
    if (n <= 1) {
        return n;
    }
    return fib_recursivo(n - 1) + fib_recursivo(n - 2);
}

long int fib_iterativa(long int n) {
    if (n <= 1) {
        return n;
    }
    long int f1 = 0;
    long int f2 = 1;
    long int temp;

    for (long int i = 2; i <= n; i++) {
        temp = f1 + f2;
        f1 = f2;
        f2 = temp;
        volatile long int delay = 0;
        for (long int j = 0; j < 5000000; j++) {
            delay++;
        }
    }
    return f2;
}

int main() {
    long int valores_teste[] = {10, 20, 30, 40, 45};
    int num_testes = sizeof(valores_teste) / sizeof(valores_teste[0]);

    printf("N\tTempo Iterativo (s)\tTempo Recursivo (s)\n");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < num_testes; i++) {
        long int teste = valores_teste[i];
        clock_t inicio, fim;
        double tempo_iterativo, tempo_recursivo;

        inicio = clock();
        fib_recursivo(teste);
        fim = clock();
        tempo_recursivo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

        inicio = clock();
        fib_iterativa(teste);
        fim = clock();
        tempo_iterativo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

        printf("%ld\t%f\t\t%f\n", teste, tempo_iterativo, tempo_recursivo);
    }

    return 0;
}