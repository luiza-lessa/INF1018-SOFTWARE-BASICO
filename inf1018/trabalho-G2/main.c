/*
 * Programa AUXILIAR de teste (nao faz parte da entrega).
 *
 * Uso:   ./teste arquivo.lbs arg1 [arg2 ...]
 *        ./teste arquivo.lbs -d          (gera code.bin para o objdump)
 *
 * Para cada argumento, chama a ultima funcao LBS do arquivo e imprime o
 * valor retornado (um numero por linha, no stdout).
 *
 * O vetor code[] e uma variavel LOCAL, portanto fica na pilha. A pilha so
 * e executavel porque o programa e compilado com -Wa,--execstack, como
 * pede o enunciado. Um vetor global/static ficaria em .bss/.data, que nao
 * sao executaveis, e a chamada daria Segmentation fault.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

#define TAM_CODE 500   /* mesmo tamanho do esqueleto do enunciado */

int main (int argc, char *argv[]) {
  FILE *fp;
  unsigned char code[TAM_CODE];
  funcp funcLBS = NULL;
  int i;

  if (argc < 3) {
    fprintf(stderr, "uso: %s arquivo.lbs arg1 [arg2 ...] | -d\n", argv[0]);
    return 1;
  }

  /* Abre o arquivo para leitura */
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("nao conseguiu abrir arquivo");
    return 1;
  }

  /* Zera o vetor: como a ultima instrucao gerada e sempre um 'ret' (0xC3),
     o ultimo byte diferente de zero marca o fim do codigo gerado. */
  memset(code, 0, sizeof(code));

  /* Gera o codigo */
  gera_codigo(fp, code, &funcLBS);
  fclose(fp);
  if (funcLBS == NULL) {
    printf("Erro na geracao\n");
    return 1;
  }

  if (strcmp(argv[2], "-d") == 0) {
    int tam = TAM_CODE;
    FILE *out;
    while (tam > 0 && code[tam - 1] == 0) tam--;
    out = fopen("code.bin", "wb");
    if (out == NULL) { perror("code.bin"); return 1; }
    fwrite(code, 1, tam, out);
    fclose(out);
    fprintf(stderr, "%d bytes gravados em code.bin; entry = code + %ld\n",
            tam, (long) ((unsigned char *) funcLBS - code));
    return 0;
  }

  /* Chama a funcao gerada (varias vezes, se houver varios argumentos) */
  for (i = 2; i < argc; i++) {
    int res = (*funcLBS)(atoi(argv[i]));
    printf("%d\n", res);
  }
  return 0;
}
