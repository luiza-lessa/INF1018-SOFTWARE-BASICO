# Gerador de código LBS → x86-64 (INF1018, 2026.2)

Documento de apoio (não é entregue). Organizado nas partes A–H pedidas.

---

## PARTE A — Análise da especificação

**Função exigida:** `void gera_codigo(FILE *f, unsigned char code[], funcp *entry);` (protótipo de `gera_codigo.h`, que **não** pode ser alterado nem é enviado).

| Requisito | Origem no enunciado |
|---|---|
| Ler uma ou mais funções LBS de `f` (já aberto) | "O que fazer" |
| Gerar **código de máquina** x86-64 (não assembly) em `code[]` | "Implementação" |
| `*entry` = 1ª instrução da **última** função do arquivo | "O que fazer" |
| Tipo único: inteiro de 32 bits com sinal | "A Linguagem LBS" |
| Locais `v0..v4` (máx. 5), **obrigatoriamente na pilha** | idem |
| Parâmetro único `p0`; constantes `$n` com sinal opcional | idem |
| `var = varpc op varpc`, `op ∈ {+,-,*}` | gramática |
| `var = call num varpc`; função 0 é a primeira; só chama a si mesma ou anteriores | idem |
| `ret varpc`; `zret a b` retorna `b` se `a == 0`, senão segue | idem |
| Prólogo usual + espaço para locais; finalização padrão | "Implementação" |
| Convenções C/Linux: parâmetro, retorno, salvamento de registradores | idem |
| Entrada sempre correta, ≤ 50 linhas, um comando por linha | idem |
| Compilar com `gcc -Wall -Wa,--execstack` | "Testando" |
| Entregar `gera_codigo.c` (sem `main`, nomes no topo no formato `/* Nome Matricula Turma */`) e `relatorio.txt` | "Entrega" |
| Constante com as letras `CEB` + comentário indicado | última linha do enunciado |
| Trabalho que não compila recebe zero; atraso: −1 ponto/dia; possível apresentação oral | "Instruções Gerais" |

**Pontos críticos:** `%edi` é caller-saved, então `p0` se perde no primeiro `call` se ficar só no registrador; offsets relativos do `call` são calculados a partir do fim da instrução; o `zret` precisa de um desvio para frente; `%rsp` precisa estar alinhado em 16 em cada `call`.

---

## PARTE B — Arquitetura da solução

### Registro de ativação (igual para toda função LBS)

```
  8(%rbp)  endereço de retorno
  0(%rbp)  %rbp do chamador
 -4(%rbp)  v0        -16(%rbp)  v3
 -8(%rbp)  v1        -20(%rbp)  v4
-12(%rbp)  v2        -24(%rbp)  p0 (cópia de %edi)
-32..-25   preenchimento (frame de 32 bytes)
```

**Alinhamento:** na entrada da função `%rsp ≡ 8 (mod 16)` (o `call` empilhou 8 bytes). Após `push %rbp`, `%rsp ≡ 0`. Subtraindo 32 (múltiplo de 16) continua `≡ 0`, logo todo `call` gerado ocorre com a pilha alinhada. Isso foi verificado dinamicamente (ver Parte E).

**Por que copiar `p0` para a pilha:** `%edi` recebe o argumento de cada `call` que a função faz, então depois do primeiro `call` o valor original de `p0` estaria perdido. Com a cópia em `-24(%rbp)`, `p0` é sempre lido da memória. Assim o gerador **não usa nenhum registrador callee-saved** além de `%rbp`, e não há nada a salvar/restaurar além dele.

**Registradores:** `%eax` (1º operando/resultado/retorno), `%ecx` (2º operando), `%edi` (argumento do `call`). Todos caller-saved, então o valor deles nunca precisa sobreviver a uma chamada — o que precisa sobreviver está na pilha.

### Tradução de cada comando

| LBS | Assembly gerado | Bytes |
|---|---|---|
| `function` | `pushq %rbp`<br>`movq %rsp,%rbp`<br>`subq $32,%rsp`<br>`movl %edi,-24(%rbp)` | `55`<br>`48 89 e5`<br>`48 83 ec 20`<br>`89 7d e8` |
| carregar `$n` em reg | `movl $n,%reg` | `b8+r` imm32 (little-endian) |
| carregar `vi`/`p0` em reg | `movl d(%rbp),%reg` | `8b`, ModRM `01 reg 101`, disp8 |
| `vD = a + b` | a→`%eax`, b→`%ecx`; `addl %ecx,%eax`; `movl %eax,dD(%rbp)` | … `01 c8` `89 45 dD` |
| `vD = a - b` | idem com `subl %ecx,%eax` | `29 c8` |
| `vD = a * b` | idem com `imull %ecx,%eax` | `0f af c1` |
| `ret x` | x→`%eax`; `leave`; `ret` | … `c9 c3` |
| `zret a b` | a→`%eax`; `cmpl $0,%eax`; `jne L`; b→`%eax`; `leave`; `ret`; `L:` | … `83 f8 00` `75 rel8` … `c9 c3` |
| `vD = call n x` | x→`%edi`; `call f_n`; `movl %eax,dD(%rbp)` | … `e8 rel32` `89 45 dD` |
| `end` | `leave`; `ret` (proteção caso não haja `ret`) | `c9 c3` |

Códigos de registrador: `%eax`=0, `%ecx`=1, `%edi`=7 → `movl $n,%edi` = `bf`, `movl d(%rbp),%ecx` = `8b 4d d`.
Todas as codificações foram conferidas com `as` + `objdump` (Parte G, etapa 7).

### Resolução de endereços
- `inicio[k]` guarda a posição em `code[]` onde começa a função `k`. Como uma função só chama a si mesma ou anteriores, o alvo de todo `call` já é conhecido quando o `call` é emitido — **não há necessidade de segunda passada**.
- `call`: `rel32 = inicio[n] − (pos_do_e8 + 5)` (endereço da instrução seguinte). Ex.: no fatorial, o `call` está em `0x37`, a próxima instrução em `0x3c`, alvo `0x00` → `rel32 = −0x3c` = `c4 ff ff ff`.
- `zret`: emite `75 00`, gera o bloco de retorno e depois corrige o byte com a distância até o fim do bloco (5 ou 7 bytes; cabe em `rel8`).
- `*entry = (funcp)(code + inicio[última])`.

### Interpretação da entrada
Mesma estrutura de `tratamentoentrada.c` (despacho pelo primeiro caractere + `fscanf` com os mesmos formatos), com estas adaptações: roda dentro de `gera_codigo` usando `f`; operandos viram uma struct `{tipo, valor}`; `%d` já lê o sinal de `$-10`; em erro, imprime a linha e deixa `*entry = NULL` (em vez de `exit`), compatível com o teste `funcLBS == NULL` do esqueleto do enunciado; `end` aceito mesmo sem `\n` final.

---

## PARTE C — `gera_codigo.c`
Arquivo `gera_codigo.c` (entregável). Sem `main`, só caracteres ASCII (o enunciado está em ISO-8859-1; evitar acentos elimina problemas de codificação), compila sem nenhum aviso com `-Wall` e também com `-Wall -Wextra -std=c99`.

## PARTE D — `main.c`
Arquivo `main.c` (auxiliar, **não** entregue). Uso:

```
./teste arquivo.lbs arg1 [arg2 ...]   # imprime f(arg) de cada argumento
./teste arquivo.lbs -d                # grava code.bin para o objdump
```

- `code[500]` é **local** (pilha), como no esqueleto. A pilha só é executável por causa de `-Wa,--execstack`; um vetor global/`static` ficaria em `.bss`, que não é executável → Segmentation fault.
- O `ld` mostra `warning: ... requires executable stack` — é a consequência esperada da flag, não é erro.
- Modo `-d`: o vetor é zerado antes e o último byte gerado é sempre `c3`, então o último byte não-zero marca exatamente o fim do código.

---

## PARTE E — Testes

Todos executados neste ambiente (Linux x86-64, gcc 13.3.0, Ubuntu 24.04): **57/57 PASS**. Os esperados foram calculados à mão e conferidos com um interpretador LBS independente escrito em Python.

Comando de cada teste: `./teste testes/<arquivo> <arg>` (ou `bash testes.sh` para todos).

| Arquivo | Código LBS (uma linha por comando) | arg → esperado | Justificativa / o que testa |
|---|---|---|---|
| t01_constante | `ret $100` | 0 → 100 | constante positiva |
| t02_zero | `ret $0` | 0 → 0 | constante zero |
| t03_negativa | `ret $-42` | 0 → −42 | constante negativa (imm32 `d6 ff ff ff`) |
| t04_p0 | `ret p0` | 7→7, −3→−3, 0→0 | parâmetro |
| t05_soma_oficial | `v0 = p0 + $1` / `ret v0` | 10→11, −1→0 | exemplo do enunciado |
| t06_subtracao | `v0 = p0 - $5` / `ret v0` | 3 → −2 | subtração com resultado negativo |
| t07_multiplicacao | `v0 = p0 * $-3` / `ret v0` | 7→−21, −4→12 | `imul` com sinal |
| t08_negativos | `v0 = $-10 - p0` / `v1 = v0 * $-2` / `v2 = v1 + $-7` / `ret v2` | −4→5, 0→13 | p=−4: −6, 12, 5 |
| t09_combinacoes | `v0 = $2 + $3` / `v1 = p0 * v0` / `v2 = v1 - p0` / `v3 = v2 * $-1` / `v4 = v3 + v0` / `v4 = p0 - v4` / `ret v4` | 4 → 15 | const+const, p0*var, var−p0, var*const, var+var, p0−var: 5,20,16,−16,−11,15 |
| t10_cinco_vars | `vi = p0 + $(i+1)` para i=0..4, depois `v0 = v0*$10 + vi` encadeado / `ret v0` | 0→12345, 1→23456 | cada variável tem seu próprio slot |
| t11_zret_p0 | `zret p0 $1` / `ret $2` | 0→1, 5→2 | zret verdadeiro e falso |
| t12_zret_var | `v0 = p0 - $3` / `zret v0 $-7` / `v1 = v0 * v0` / `ret v1` | 3→−7, 5→4, 0→9 | zret com variável e retorno negativo |
| t13_zret_const | `zret $0 $9` / `ret $1` | 123 → 9 | 1º operando constante zero |
| t14_zret_const_nz | `zret $1 $9` / `ret p0` | 6 → 6 | 1º operando constante ≠ 0 |
| t15_caminhos | `zret p0 $10` / `v0 = p0 - $1` / `zret v0 $20` / `v0 = v0 - $1` / `zret v0 p0` / `ret $40` | 0→10, 1→20, 2→2, 3→40, −1→40 | quatro saídas diferentes |
| t16_ret_meio | `v0 = p0 + $1` / `ret v0` / `v0 = $99 + $0` / `ret v0` | 4 → 5 | `ret` encerra a função de imediato |
| t17_duas_funcoes | f0: `v0 = p0 * $2`/`ret v0`; f1: `v0 = call 0 p0` / `v1 = call 0 v0` / `v2 = call 0 $5` / `v3 = v1 + v2` / `ret v3` | 3 → 22 | 6, 12, 10 → 22; call com p0, var e constante |
| t18_preserva_estado | f0 escreve em todas as suas v0..v4; f1 preenche v0..v3, chama f0 e depois soma v0..v4 e **p0** | 1→36, 10→81 | locais e p0 sobrevivem ao call (p=1: 2+3+4+5+21+1) |
| t19_fatorial_oficial | exemplo do enunciado | 0→1, 1→1, 5→120, 10→3628800, 12→479001600 | recursão |
| t20_somaquadrados_oficial | exemplo do enunciado | 0→0, 1→1, 3→14, 10→385, 100→338350 | duas funções + recursão |
| t21_soma_recursiva | `zret p0 $0` / `v0 = p0 - $1` / `v1 = call 0 v0` / `v1 = v1 + p0` / `ret v1` | 1000→500500, 10000→50005000 | recursão profunda (10000 frames) |
| t22_fibonacci | `zret p0 $0` / `v0 = p0 - $1` / `zret v0 $1` / `v1 = call 0 v0` / `v2 = v0 - $1` / `v3 = call 0 v2` / `v4 = v1 + v3` / `ret v4` | 0→0, 1→1, 2→1, 10→55, 20→6765 | recursão dupla; v1 tem que sobreviver ao 2º call |
| t23_tres_funcoes | f0: x+1; f1: f0(f0(x)); f2: `zret p0 $0`, f2(n−1), f1(·), f0(−1) | 0→0, 5→10 | f2(n)=2n; chama a si mesma e duas anteriores |
| t24_limites | `v0 = p0 * $65536` / `ret v0` | 32767→2147418112, −32768→−2147483648 | produtos no limite de 32 bits, sem overflow |
| t25_int_max | `ret $2147483647` | 0 → 2147483647 | maior constante |
| t26_int_min | `ret $-2147483648` | 0 → −2147483648 | menor constante |
| t27_sem_newline_final | `v0 = p0 + $7` / `ret v0` / `end` (sem `\n`) | 5 → 12 | robustez da leitura |
| (repetição) | t22 | `./teste testes/t22_fibonacci.lbs 10 20 10` → `55 6765 55` | mesmo código chamado várias vezes pelo C |

Verificações extras feitas aqui (não estão no `testes.sh`):
- **Alinhamento de pilha:** substituí o início da função 0 de t17 por um salto para uma função C que verifica `%rbp % 16 == 0`; resultado: 3 chamadas, 0 desalinhadas.
- **CRLF:** t20 com quebras de linha Windows → 385 (correto).
- **Entrada inválida** (`call 1` dentro da função 0, `v7`): mensagem com a linha e `Erro na geracao`.
- **Tamanho:** maior teste gera 230 bytes (cabe em `code[500]`).

`testes.sh` compila com as flags do enunciado, roda todos os casos acima, mostra PASS/FAIL e um resumo, e retorna código de saída 0 só se tudo passar.

---

## PARTE F — `relatorio.txt`
Arquivo `relatorio.txt` (entregável). Complete sobrenome, turma e o segundo integrante (ou apague a linha se fizer sozinha), e confirme os resultados rodando `bash testes.sh` no Codespaces.

---

## PARTE G — Tutorial no GitHub Codespaces

### Etapa 1 — Organização
```
trabalho/
├── gera_codigo.c        ← ENTREGAR
├── relatorio.txt        ← ENTREGAR
├── gera_codigo.h        (oficial, não modificar, não entregar)
├── main.c               (auxiliar)
├── testes.sh            (auxiliar)
├── LEIAME.md            (este arquivo)
└── testes/              (27 arquivos .lbs)
```

### Etapa 2 — Preparação
Arraste o `trabalho.zip` para o Explorer do VS Code (painel esquerdo) e, no terminal (`Ctrl+'`):
```bash
unzip trabalho.zip && cd trabalho
ls -1 && ls testes | wc -l          # deve mostrar 27
md5sum gera_codigo.h                 # compare com o .h baixado do site, se quiser
```
Para conferir que o `.h` é o oficial: `curl -s https://www.inf.puc-rio.br/~inf1018/2026.2/trabs/gera_codigo.h | diff - gera_codigo.h && echo IGUAL`

### Etapa 3 — Ambiente
```bash
gcc --version | head -1      # qualquer gcc recente
uname -m                     # tem que ser x86_64
which objdump as             # binutils
```
Se `uname -m` mostrar `aarch64`, o código gerado **não roda** nessa máquina (é x86-64); use outro tipo de Codespace. O código só executa se a pilha for executável — é isso que `-Wa,--execstack` garante.

### Etapa 4 — Compilação
```bash
gcc -Wall -Wa,--execstack -o teste main.c gera_codigo.c
```
- `-Wall`: todos os avisos comuns; o esperado é **nenhum** aviso do compilador.
- `-Wa,--execstack`: repassa `--execstack` ao montador, marcando a pilha como executável (o `code[]` do `main` está na pilha).
- Esperado: só `/usr/bin/ld: warning: ... requires executable stack ...` (normal). Nenhuma flag adicional é necessária.

Checagem extra opcional: `gcc -Wall -Wextra -std=c99 -c gera_codigo.c` (também sem avisos).

### Etapa 5 — Execução
```bash
./teste testes/t01_constante.lbs 0               # 100
./teste testes/t05_soma_oficial.lbs 10           # 11
./teste testes/t19_fatorial_oficial.lbs 5        # 120
./teste testes/t20_somaquadrados_oficial.lbs 10  # 385
./teste testes/t22_fibonacci.lbs 10 20 10        # 55, 6765, 55 (uma por linha)
bash testes.sh                                    # termina com: RESUMO: 57 PASS, 0 FAIL, total 57
```

### Etapa 6 — Diagnóstico
- **Erro de compilação / warning:** a mensagem traz arquivo:linha. Se aparecer `unknown type name 'FILE'`, o `#include <stdio.h>` precisa vir antes de `gera_codigo.h`.
- **Segmentation fault logo na chamada:** faltou `-Wa,--execstack`, ou `code[]` não está na pilha.
- **Resultado aritmético errado:** gere o binário e desmonte (etapa 7) e confira os offsets (`-0x4` = v0 … `-0x18` = p0).
- **Falha em recursão:** confira o `rel32` do `call` (deve apontar para o `push %rbp` da função alvo) e que o resultado é guardado com `mov %eax,…` logo após o `call`.
- **Pilha desalinhada:** todo prólogo deve ter `sub $0x20,%rsp`; confira com `objdump` e com o raciocínio da Parte B (ou repita a sonda descrita na Parte E).
- **Desvio errado no `zret`:** o `jne` tem que cair na instrução logo depois do `ret` do bloco.
- Depurar passo a passo com gdb (compile com `-g` só para depurar):
  ```bash
  gcc -g -Wall -Wa,--execstack -o teste main.c gera_codigo.c
  gdb -q --args ./teste testes/t19_fatorial_oficial.lbs 3
  (gdb) break main.c:67        # linha do res = (*funcLBS)(...)
  (gdb) run
  (gdb) break *funcLBS         # para na 1a instrução do código gerado
  (gdb) continue
  (gdb) x/12i $pc              # mostra as próximas instruções geradas
  (gdb) info registers rdi rsp # rdi = 3; rsp termina em 8 (≡ 8 mod 16) na entrada
  (gdb) stepi                  # executa uma instrução gerada por vez
  ```

### Etapa 7 — Inspeção das instruções
```bash
# código gerado de verdade
./teste testes/t19_fatorial_oficial.lbs -d
objdump -D -b binary -m i386:x86-64 code.bin

# descobrir a codificação de uma instrução
printf 'movl %%eax, %%ecx\nimull %%ecx, %%eax\ncall .\n' > meuteste.s
gcc -c meuteste.s && objdump -d meuteste.o
```
Esperado para o fatorial: 82 bytes, `call` em `0x37` com bytes `e8 c4 ff ff ff` → `call 0x0`.

### Etapa 8 — Sequência final antes de entregar
```bash
gcc -Wall -Wa,--execstack -o teste main.c gera_codigo.c
bash testes.sh; echo "status=$?"                # status=0
grep -c "main" gera_codigo.c                     # 0
grep -n "CEB" gera_codigo.c                      # constante + comentário
head -2 gera_codigo.c                            # nomes preenchidos?
head -6 relatorio.txt                            # nomes preenchidos?
```

### Etapa 9 — Entrega no Moodle
- **Obrigatórios:** `gera_codigo.c` e `relatorio.txt` (+ nomes dos integrantes na área de texto da tarefa).
- **Não enviar:** `gera_codigo.h` (o corretor usa o original), `main.c`, `testes.sh`, `testes/`, `LEIAME.md`, `teste`, `code.bin`.

---

## PARTE H — Checklist de conformidade

| # | Requisito | Onde está | Teste |
|---|---|---|---|
| 1 | Protótipo oficial, `.h` intacto | `gera_codigo.c` inclui o `.h` original | compilação |
| 2 | Código de máquina x86-64 real | funções `emite_*`, `traduz_*` | todos; `objdump` |
| 3 | `*entry` = início da última função | fim de `gera_codigo` | t17, t20, t23 |
| 4 | Constantes com sinal | `carrega_operando` (`b8+r imm32`) | t01–t03, t25, t26 |
| 5 | `p0` | cópia em `-24(%rbp)` no prólogo | t04, t18 |
| 6 | 5 locais na pilha | `desloc_var`: −4…−20 | t10 |
| 7 | `+ - *` com qualquer operando | `traduz_aritmetica` | t05–t09, t24 |
| 8 | `ret` | `traduz_ret` | t01, t16 |
| 9 | `zret` verdadeiro/falso | `traduz_zret` (`cmp`, `jne rel8`) | t11–t15 |
| 10 | Várias funções, chamar anteriores | `inicio[]`, `traduz_call` | t17, t20, t23 |
| 11 | Recursão | `call rel32` para `inicio[atual]` | t19, t21, t22 |
| 12 | Convenção C (edi/eax, alinhamento, callee-saved) | prólogo/epílogo; só usa eax/ecx/edi | sonda de alinhamento, t18 |
| 13 | Prólogo/epílogo padrão | `emite_prologo`, `emite_epilogo` (`leave; ret`) | todos |
| 14 | Entrada ≤ 50 linhas | `MAX_FUNCS 50` | — |
| 15 | Compila com `-Wall -Wa,--execstack` sem avisos | — | `testes.sh` |
| 16 | `gera_codigo.c` sem `main` | — | etapa 8 |
| 17 | Nomes no formato `/* Nome Matricula Turma */` | linhas 1–2 | completar |
| 18 | Constante com `CEB` + comentário | linhas 8–9 | etapa 8 |
| 19 | `relatorio.txt`: funciona / não funciona / exemplos / nomes | `relatorio.txt` | — |

**Limitação conhecida:** `gera_codigo` não recebe o tamanho de `code[]`. Cada comando ocupa 7–17 bytes (prólogo 11, `end` 2). Com `code[500]` cabem com folga os testes usuais (maior aqui: 230 bytes); um arquivo de 50 linhas só de operações poderia passar de 500 bytes. Se quiser reduzir, as formas compactas seriam usar o 2º operando direto da memória/imediato (`03 45 d` = `addl d(%rbp),%eax`; `05 imm32` = `addl $imm,%eax`; `69 c0 imm32` = `imull $imm,%eax,%eax`), mas isso aumenta o número de codificações a justificar numa apresentação.
