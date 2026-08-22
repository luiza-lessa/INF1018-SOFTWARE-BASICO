#include <stdio.h>

int odd_ones(unsigned int x) {
    int n = 0; //qtd de 1's
    int i;
    
    //um int tem 32 bits, então vamos de 0 até 31
    for (i = 0; i < 32; i++) {
        //desloca o bit 'i' para a primeira posição e checa se ele vale 1
        if ((x >> i) & 1) {
            n += 1;
        }
    }
    
    //verifica o último bit do contador 'n' (substitui o n % 2 != 0)
    if (n & 1) {
        return 1; //é ímpar
    } else {
        return 0; //é par
    }
}

int main() {
    printf("%x tem numero %s de bits\n", 0x01010101, odd_ones(0x01010101) ? "impar" : "par");
    printf("%x tem numero %s de bits\n", 0x01030101, odd_ones(0x01030101) ? "impar" : "par");
    return 0;
}