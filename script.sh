#!/bin/bash

# Parar no primeiro erro
set -e

# Pasta de saída
BIN_DIR="bin"
EXECUTAVEL="simulador"

# Criar a pasta bin se não existir
mkdir -p $BIN_DIR

# Compilar os ficheiros C
echo "🔨 Compilando o projeto..."
gcc -Wall -Wextra -std=c11 -o "$BIN_DIR/$EXECUTAVEL" \
    src/ExecProcess.c \
    src/queue.c \
    src/process.c \
    -Iinclude

# Mensagem de sucesso
echo "✅ Compilação bem-sucedida."
