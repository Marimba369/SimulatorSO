#!/bin/bash

# Parar no primeiro erro
set -e

# Pasta de saída
BIN_DIR="bin"
EXECUTAVEL="simulador"

# Variável para controlar o modo de depuração
# Defina DEBUG_MODE=1 antes de executar o script para ativar o modo de depuração.
# Ex: DEBUG_MODE=1 ./compile.sh
DEBUG_MODE=${DEBUG_MODE:-0} # Valor padrão é 0 (desativado)

# Opções de compilação
COMPILER_FLAGS="-Wall -Wextra -std=c11"
# Adicionar flag de depuração se DEBUG_MODE estiver ativado
if [ "$DEBUG_MODE" -eq 1 ]; then
    COMPILER_FLAGS="$COMPILER_FLAGS -DDEBUG -g" # -g para GDB
    echo "🐛 Modo de depuração (DEBUG) ativado."
else
    COMPILER_FLAGS="$COMPILER_FLAGS"
    echo "🚀 Modo de depuração (DEBUG) desativado."
fi

# Criar a pasta bin se não existir
echo "📁 Criando pasta de saída: $BIN_DIR"
mkdir -p "$BIN_DIR"

# Compilar os ficheiros C
echo "🔨 Compilando o projeto..."
# Importante: Alterei 'src/ExecProcess.c' para 'src/main.c', pois o 'main.c' é onde a função principal 'main' está.
# Certifique-se de que main.c, queue.c e process.c estão na pasta src/.
gcc $COMPILER_FLAGS -o "$BIN_DIR/$EXECUTAVEL" \
    src/ExecProcess.c \
    src/queue.c \
    src/process.c \
    -Iinclude

# Mensagem de sucesso
echo "✅ Compilação bem-sucedida. Executável em: $BIN_DIR/$EXECUTAVEL"
echo ""
echo "Para executar:       ./$BIN_DIR/$EXECUTAVEL"
echo "Para depurar com GDB: gdb ./$BIN_DIR/$EXECUTAVEL"
echo "Para executar com DEBUG: DEBUG_MODE=1 ./compile.sh && ./$BIN_DIR/$EXECUTAVEL"