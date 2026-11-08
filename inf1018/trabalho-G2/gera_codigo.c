/* Luiza Souza Chaves Lessa Campos - 2512204 - 3WB */

#include <stdio.h>
#include <stdlib.h>
#include "gera_codigo.h"

/*
Registro de ativacao de toda funcao LBS (offsets relativos a %rbp):
8(%rbp)  endereco de retorno
0(%rbp)  %rbp do chamador
-4(%rbp)  v0
-8(%rbp)  v1
-12(%rbp)  v2
-16(%rbp)  v3
-20(%rbp)  v4
-24(%rbp)  p0 (copia de %edi feita no prologo)
-32(%rbp)  (preenchimento: o frame tem 32 bytes para manter %rsp
alinhado em 16 antes de cada call)
Registradores usados: %eax, %ecx e %edi, todos caller-saved na ABI
System V AMD64; nenhum callee-saved alem de %rbp (salvo no prologo).
*/

#define MAX_VARS 5
#define MAX_FUNCS 50        /* o arquivo tem no maximo 50 linhas */
#define TAM_FRAME 32
#define DESLOC_P0 (-24)

/* codigos de registradores x86-64 (campo reg do ModRM / opcode+r) */
#define REG_EAX 0
#define REG_ECX 1
#define REG_EDI 7

typedef struct {
  unsigned char *code; /* vetor de saida */
  int pos;             /* proxima posicao livre em code[] */
} Gerador;

typedef struct {
  char tipo; /* 'v' (local), 'p' (parametro) ou '$' (constante) */
  int valor; /* indice da variavel ou valor da constante */
} Operando;

/* ----- Emissao de bytes ----- */

static void emite_byte (Gerador *g, unsigned char b) {
  g->code[g->pos++] = b;
}

/* inteiro de 32 bits em little-endian */
static void emite_int32 (Gerador *g, int v) {
  unsigned int u = (unsigned int) v;
  int i;
  for (i = 0; i < 4; i++)
    emite_byte(g, (unsigned char) ((u >> (8 * i)) & 0xFF));
}

/* escreve um int32 little-endian numa posicao ja emitida (correcao) */
static void corrige_int32 (Gerador *g, int pos, int v) {
  unsigned int u = (unsigned int) v;
  int i;
  for (i = 0; i < 4; i++)
    g->code[pos + i] = (unsigned char) ((u >> (8 * i)) & 0xFF);
}

/* ----- traducao ----- */

static int desloc_var (int idx) {
  return -4 * (idx + 1); /* v0-> -4, ..., v4-> -20 */
}

static int desloc_operando (Operando op) {
  return (op.tipo == 'v') ? desloc_var(op.valor) : DESLOC_P0;
}

/* pushq %rbp; movq %rsp,%rbp; subq $32,%rsp; movl %edi,-24(%rbp) */
static void emite_prologo (Gerador *g) {
  emite_byte(g, 0x55);
  emite_byte(g, 0x48); emite_byte(g, 0x89); emite_byte(g, 0xE5);
  emite_byte(g, 0x48); emite_byte(g, 0x83); emite_byte(g, 0xEC);
  emite_byte(g, TAM_FRAME);
  emite_byte(g, 0x89); emite_byte(g, 0x7D);
  emite_byte(g, (unsigned char) DESLOC_P0);
}

/* leave ret */
static void emite_epilogo (Gerador *g) {
  emite_byte(g, 0xC9);
  emite_byte(g, 0xC3);
}

/* coloca o valor de um operando (32 bits) no reg */
static void carrega_operando (Gerador *g, Operando op, int reg) {
  if (op.tipo == '$') {
    /* movl $imm32, %reg -> B8+r imm32 */
    emite_byte(g, (unsigned char) (0xB8 + reg));
    emite_int32(g, op.valor);
  }
  else {
    /* movl disp8(%rbp), %reg -> 8B /r, ModRM = 01 reg 101 */
    emite_byte(g, 0x8B);
    emite_byte(g, (unsigned char) (0x45 | (reg << 3)));
    emite_byte(g, (unsigned char) desloc_operando(op));
  }
}

/* movl %eax, disp8(%rbp) -> 89 45 disp8 */
static void armazena_eax (Gerador *g, int idx_var) {
  emite_byte(g, 0x89);
  emite_byte(g, 0x45);
  emite_byte(g, (unsigned char) desloc_var(idx_var));
}

/* vD = a op b */
static void traduz_aritmetica (Gerador *g, int dest, Operando a, char op,
                               Operando b) {
  carrega_operando(g, a, REG_EAX);
  carrega_operando(g, b, REG_ECX);
  switch (op) {
    case '+':                     /* addl %ecx, %eax */
      emite_byte(g, 0x01); emite_byte(g, 0xC8);
      break;
    case '-':                     /* subl %ecx, %eax */
      emite_byte(g, 0x29); emite_byte(g, 0xC8);
      break;
    case '*':                     /* imull %ecx, %eax */
      emite_byte(g, 0x0F); emite_byte(g, 0xAF); emite_byte(g, 0xC1);
      break;
  }
  armazena_eax(g, dest);
}

/* vD = call n arg */
static void traduz_call (Gerador *g, int dest, int alvo, Operando arg) {
  int pos_call;
  carrega_operando(g, arg, REG_EDI);           /* argumento em %edi */
  pos_call = g->pos;
  emite_byte(g, 0xE8);                         /* call rel32 */
  emite_int32(g, 0);
  /* deslocamento relativo ao endereco da instrucao seguinte ao call */
  corrige_int32(g, pos_call + 1, alvo - (pos_call + 5));
  armazena_eax(g, dest);                       /* retorno em %eax */
}

/* ret x */
static void traduz_ret (Gerador *g, Operando x) {
  carrega_operando(g, x, REG_EAX);
  emite_epilogo(g);
}

/* zret a b : se a == 0, retorna b */
static void traduz_zret (Gerador *g, Operando a, Operando b) {
  int pos_desloc;
  carrega_operando(g, a, REG_EAX);
  emite_byte(g, 0x83); emite_byte(g, 0xF8); emite_byte(g, 0x00); /* cmpl $0,%eax */
  emite_byte(g, 0x75);                         /* jne rel8 */
  pos_desloc = g->pos;
  emite_byte(g, 0x00);                         /* corrigido abaixo */
  carrega_operando(g, b, REG_EAX);
  emite_epilogo(g);
  /* salto ate logo depois do epilogo (no maximo 7 bytes: cabe em rel8) */
  g->code[pos_desloc] = (unsigned char) (g->pos - (pos_desloc + 1));
}

/* ----- Interpretacao do arquivo LBS ----- */

static void erro (const char *msg, int line, funcp *entry) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  *entry = NULL;
}

static int operando_valido (Operando op) {
  if (op.tipo == 'v') return op.valor >= 0 && op.valor < MAX_VARS;
  if (op.tipo == 'p') return op.valor == 0;
  return op.tipo == '$';
}

void gera_codigo (FILE *f, unsigned char code[], funcp *entry) {
  Gerador g;
  int inicio[MAX_FUNCS]; /* posicao em code[] do inicio de cada funcao */
  int nfuncs = 0; /* funcoes ja abertas; a corrente e nfuncs-1 */
  int line = 1;
  int c;

  g.code = code;
  g.pos = 0;
  *entry = NULL;

  fscanf(f, " ");
  while ((c = fgetc(f)) != EOF) {
    switch (c) {
      case 'f': { /* function */
        char c0;
        if (fscanf(f, "unction%c", &c0) != 1 || nfuncs >= MAX_FUNCS) {
          erro("comando invalido", line, entry);
          return;
        }
        inicio[nfuncs++] = g.pos;
        emite_prologo(&g);
        break;
      }
      case 'e': { /* end */
        char c0;
        if (fscanf(f, "nd%c", &c0) == 0 || nfuncs == 0) {
          erro("comando invalido", line, entry);
          return;
        }
        /* se o fluxo chegar ao fim da funcao sem 'ret', a funcao retorna em vez de cair no codigo da proxima */
        emite_epilogo(&g);
        break;
      }
      case 'r': { /* retorno incondicional */
        Operando x;
        if (fscanf(f, "et %c%d", &x.tipo, &x.valor) != 2 ||
            !operando_valido(x) || nfuncs == 0) {
          erro("comando invalido", line, entry);
          return;
        }
        traduz_ret(&g, x);
        break;
      }
      case 'z': { /* retorno condicional */
        Operando a, b;
        if (fscanf(f, "ret %c%d %c%d", &a.tipo, &a.valor,
                   &b.tipo, &b.valor) != 4 ||
            !operando_valido(a) || !operando_valido(b) || nfuncs == 0) {
          erro("comando invalido", line, entry);
          return;
        }
        traduz_zret(&g, a, b);
        break;
      }
      case 'v': { /* atribuicao */
        int dest;
        char c0;
        if (fscanf(f, "%d = %c", &dest, &c0) != 2 ||
            dest < 0 || dest >= MAX_VARS || nfuncs == 0) {
          erro("comando invalido", line, entry);
          return;
        }
        if (c0 == 'c') { /* call */
          int alvo;
          Operando arg;
          if (fscanf(f, "all %d %c%d", &alvo, &arg.tipo, &arg.valor) != 3 ||
              !operando_valido(arg) || alvo < 0 || alvo >= nfuncs) {
            /* so pode chamar a si mesma ou funcoes anteriores */
            erro("comando invalido", line, entry);
            return;
          }
          traduz_call(&g, dest, inicio[alvo], arg);
        }
        else { /* operacao aritmetica */
          Operando a, b;
          char op;
          a.tipo = c0;
          if (fscanf(f, "%d %c %c%d", &a.valor, &op, &b.tipo, &b.valor) != 4 ||
              !operando_valido(a) || !operando_valido(b) ||
              (op != '+' && op != '-' && op != '*')) {
            erro("comando invalido", line, entry);
            return;
          }
          traduz_aritmetica(&g, dest, a, op, b);
        }
        break;
      }
      default:
        erro("comando desconhecido", line, entry);
        return;
    }
    line++;
    fscanf(f, " ");
  }

  if (nfuncs > 0)
    *entry = (funcp) (code + inicio[nfuncs - 1]);
}