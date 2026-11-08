#!/usr/bin/env bash
# Testes automatizados do gerador LBS (arquivo auxiliar, nao e entregue).
# Uso:  bash testes.sh        (a partir da pasta do projeto)

cd "$(dirname "$0")" || exit 1

echo "== Compilando: gcc -Wall -Wa,--execstack -o teste main.c gera_codigo.c"
if ! gcc -Wall -Wa,--execstack -o teste main.c gera_codigo.c; then
  echo "FALHA NA COMPILACAO"
  exit 1
fi
echo

# arquivo | argumento | resultado esperado
CASOS=(
  "t01_constante.lbs            0       100"
  "t02_zero.lbs                 0       0"
  "t03_negativa.lbs             0       -42"
  "t04_p0.lbs                   7       7"
  "t04_p0.lbs                   -3      -3"
  "t04_p0.lbs                   0       0"
  "t05_soma_oficial.lbs         10      11"
  "t05_soma_oficial.lbs         -1      0"
  "t06_subtracao.lbs            3       -2"
  "t07_multiplicacao.lbs        7       -21"
  "t07_multiplicacao.lbs        -4      12"
  "t08_negativos.lbs            -4      5"
  "t08_negativos.lbs            0       13"
  "t09_combinacoes.lbs          4       15"
  "t10_cinco_vars.lbs           0       12345"
  "t10_cinco_vars.lbs           1       23456"
  "t11_zret_p0.lbs              0       1"
  "t11_zret_p0.lbs              5       2"
  "t12_zret_var.lbs             3       -7"
  "t12_zret_var.lbs             5       4"
  "t12_zret_var.lbs             0       9"
  "t13_zret_const.lbs           123     9"
  "t14_zret_const_nz.lbs        6       6"
  "t15_caminhos.lbs             0       10"
  "t15_caminhos.lbs             1       20"
  "t15_caminhos.lbs             2       2"
  "t15_caminhos.lbs             3       40"
  "t15_caminhos.lbs             -1      40"
  "t16_ret_meio.lbs             4       5"
  "t17_duas_funcoes.lbs         3       22"
  "t18_preserva_estado.lbs      1       36"
  "t18_preserva_estado.lbs      10      81"
  "t19_fatorial_oficial.lbs     0       1"
  "t19_fatorial_oficial.lbs     1       1"
  "t19_fatorial_oficial.lbs     5       120"
  "t19_fatorial_oficial.lbs     10      3628800"
  "t19_fatorial_oficial.lbs     12      479001600"
  "t20_somaquadrados_oficial.lbs 0      0"
  "t20_somaquadrados_oficial.lbs 1      1"
  "t20_somaquadrados_oficial.lbs 3      14"
  "t20_somaquadrados_oficial.lbs 10     385"
  "t20_somaquadrados_oficial.lbs 100    338350"
  "t21_soma_recursiva.lbs       1000    500500"
  "t21_soma_recursiva.lbs       10000   50005000"
  "t22_fibonacci.lbs            0       0"
  "t22_fibonacci.lbs            1       1"
  "t22_fibonacci.lbs            2       1"
  "t22_fibonacci.lbs            10      55"
  "t22_fibonacci.lbs            20      6765"
  "t23_tres_funcoes.lbs         0       0"
  "t23_tres_funcoes.lbs         5       10"
  "t24_limites.lbs              32767   2147418112"
  "t24_limites.lbs              -32768  -2147483648"
  "t25_int_max.lbs              0       2147483647"
  "t26_int_min.lbs              0       -2147483648"
  "t27_sem_newline_final.lbs    5       12"
)

pass=0; fail=0
for caso in "${CASOS[@]}"; do
  read -r arq arg esp <<< "$caso"
  obt=$(./teste "testes/$arq" "$arg" 2>&1)
  st=$?
  if [ $st -eq 0 ] && [ "$obt" = "$esp" ]; then
    printf "PASS  %-30s f(%s) = %s\n" "$arq" "$arg" "$obt"
    pass=$((pass + 1))
  else
    printf "FAIL  %-30s f(%s): esperado %s, obtido '%s' (status %d)\n" \
           "$arq" "$arg" "$esp" "$obt" "$st"
    fail=$((fail + 1))
  fi
done

# mesma funcao gerada chamada varias vezes seguidas pelo programa C
obt=$(./teste testes/t22_fibonacci.lbs 10 20 10 | tr '\n' ' ')
if [ "$obt" = "55 6765 55 " ]; then
  echo "PASS  chamadas repetidas (fib 10 20 10) = $obt"; pass=$((pass + 1))
else
  echo "FAIL  chamadas repetidas: esperado '55 6765 55 ', obtido '$obt'"; fail=$((fail + 1))
fi

echo
echo "RESUMO: $pass PASS, $fail FAIL, total $((pass + fail))"
[ $fail -eq 0 ]
