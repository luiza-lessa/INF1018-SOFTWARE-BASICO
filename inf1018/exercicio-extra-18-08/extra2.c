#include <stdio.h>

void num2string(char *s, int num, int base) {
    int i = 0;
    int is_negative = 0;
    unsigned int unum; //usamos unsigned para não ter problemas com números negativos em bases como 16 (Hex)

    // Passo 1: O caso do Zero
    if (num == 0) {
        s[0] = '0';
        s[1] = '\0';
        return;
    }

    // Lidando com números negativos (geralmente usamos o sinal '-' apenas na base 10)
    if (num < 0 && base == 10) {
        is_negative = 1;
        unum = -num; // Torna positivo para facilitar a divisão
    } else {
        // Para bases como 16, tratamos o número de forma bruta (sem sinal)
        unum = (unsigned int)num;
    }

    // Passos 2 e 3: Extrair dígitos e converter para caractere (Tabela ASCII)
    while (unum > 0) {
        int resto = unum % base;

        if (resto < 10) {
            s[i] = resto + '0';        // Para dígitos de 0 a 9
        } else {
            s[i] = (resto - 10) + 'a'; // Para letras (ex: 'a', 'b', 'c' na base 16)
        }
        
        i++;
        unum = unum / base; // Divide pela base para ir para o próximo dígito
    }

    // Passo 4: O Sinal e o Terminador Nulo
    if (is_negative) {
        s[i] = '-';
        i++;
    }
    s[i] = '\0'; // Fecha a string corretamente

    // Passo 5: Inverter a string (pois os dígitos foram calculados de trás pra frente)
    int inicio = 0;
    int fim = i - 1;
    
    while (inicio < fim) {
        char temp = s[inicio];
        s[inicio] = s[fim];
        s[fim] = temp;
        
        inicio++;
        fim--;
    }
}

int main() {
    char buffer[100]; // Buffer com espaço de sobra

    // Teste 1: Base 10 (Decimal)
    num2string(buffer, -12345, 10);
    printf("Decimal (-12345, base 10): %s\n", buffer);

    // Teste 2: Base 16 (Hexadecimal)
    num2string(buffer, 255, 16);
    printf("Hexadecimal (255, base 16): %s\n", buffer);

    // Teste 3: Base 2 (Binário)
    num2string(buffer, 10, 2);
    printf("Binario (10, base 2): %s\n", buffer);
    
    // Teste 4: O caso do Zero
    num2string(buffer, 0, 10);
    printf("Zero (0, base 10): %s\n", buffer);

    return 0;
}

