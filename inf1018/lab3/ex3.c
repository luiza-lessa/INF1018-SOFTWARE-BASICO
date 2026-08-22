#include <stdio.h>

unsigned char switch_byte(unsigned char x){
    return (x >> 4) | (x << 4);
}

int main() {
    unsigned char teste = 0xAB;
    unsigned char resultado = switch_byte(teste);
    
    printf("Original: 0x%X\n", teste);
    printf("Trocado:  0x%X\n", resultado);
    
    return 0;
}