#!/bin/bash

SRC_DIR="./src"
INCLUDE_DIR="./include"
TEST_DIR="./test"

OUTPUT="home_broker"

CFLAGS="-Wall -Werror -I$INCLUDE_DIR/common -I$INCLUDE_DIR/models -I$INCLUDE_DIR/scheduler -pthread"

SRC_FILES=$(find $SRC_DIR -name "*.c" ! -path "$TEST_DIR/*")

echo "Compiling project..."

gcc $CFLAGS -o $OUTPUT $SRC_FILES

if [ $? -eq 0 ]; then
    echo "Compilation successful! Output binary: $OUTPUT"
else
    echo "Compilation failed!"
fi
