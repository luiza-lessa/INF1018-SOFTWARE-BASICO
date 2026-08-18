
#include <stdio.h>

int is_little() {
    int x = 1; 
    //pega o endereço do inteiro (&x) para tratá-lo como um ptr de caractere
    char *primeiro_byte = (char *)&x;
    //se o primeiro byte for igual a 1, o menor byte entrou primeiro (LE)
    if (*primeiro_byte == 1) {
        return 1;
    } else {
        return 0; //se fosse BE, o primeiro byte seria 0
    }
}

int main (void){
    int n = is_little();
    if (n==0){
        printf("Não é Little Endian!\n");
    }
    else{
        printf("É Little Endian!\n");
    }
    return n;
}

