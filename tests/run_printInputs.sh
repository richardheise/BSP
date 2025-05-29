#!/bin/bash

SCRIPT="printInput.py"
DIR="inputs/"

# Cria o diretório de saída de imagens, se não existir
mkdir -p .. /imgs

# Itera sobre os arquivos .in de forma segura
while IFS= read -r -d '' input_file; do
  echo "Generating images for: $input_file"
  python3 "$SCRIPT" "$input_file"
done < <(find "$DIR" -name "*.in" -print0)