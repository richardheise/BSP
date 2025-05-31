#!/bin/bash

# Caminho para os diretórios de testes
TEST_DIR="tests/inputs"
OUTPUT_DIR="tests/outputs"
ANSWER_DIR="tests/answers"
VERBOSE=""
PRINT_STDOUT=false

# Verifica as flags passadas
while [[ $# -gt 0 ]]; do
  case "$1" in
    -v)
      VERBOSE="-v"
      shift
      ;;
    -o)
      PRINT_STDOUT=true
      shift
      ;;
    *)
      echo "Uso: $0 [-v] [-o]"
      exit 1
      ;;
  esac
done

# Verifica se o executável existe e é executável
if [[ ! -x ./bsp ]]; then
  echo "Erro: o executável ./bsp não foi encontrado ou não tem permissão de execução."
  exit 1
fi

# Cria a pasta de saída se necessário
mkdir -p "$OUTPUT_DIR"

# Loop por todos os arquivos .in na pasta de testes
for test_file in "$TEST_DIR"/*.in; do
  test_name=$(basename "$test_file" .in)
  output_file="$OUTPUT_DIR/${test_name}.out"
  answer_file="$ANSWER_DIR/${test_name}.out"

  echo "Executando teste: $test_file"

  if $PRINT_STDOUT; then
    ./bsp $VERBOSE < "$test_file"
  else
    ./bsp $VERBOSE < "$test_file" > "$output_file"
    echo "Saída escrita em: $output_file"
  fi

  # Se o arquivo de resposta existe, compara usando diff
  if [[ -f "$answer_file" ]]; then
    diff_output=$(diff -u "$answer_file" "$output_file")
    if [[ $? -eq 0 ]]; then
      echo "✔ Teste $test_name: saída correta."
    else
      echo "✘ Teste $test_name: saída diferente da esperada:"
      echo "$diff_output"
    fi
  else
    echo "⚠ Arquivo de resposta $answer_file não encontrado. Pulei a comparação."
  fi

  echo "-----------------------------"
done
