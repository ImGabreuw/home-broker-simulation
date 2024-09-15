#!/bin/bash

SRC_DIR="./src"
INCLUDE_DIR="./include"

OUTPUT="home_broker"

# Flags do compilador
CFLAGS="-Wall -Werror -I$INCLUDE_DIR/common -I$INCLUDE_DIR/models -I$INCLUDE_DIR/services -pthread"

# Lista de arquivos fonte
SRC_FILES=$(find . -name "*.c")

echo "Compiling project..."

gcc $CFLAGS -o $OUTPUT $SRC_FILES

# Verificação de sucesso ou falha
if [ $? -eq 0 ]; then
    echo "Compilation successful! Output binary: $OUTPUT"
else
    echo "Compilation failed!"
fi
