#ifndef POSITION_H
#define POSITION_H

#include <stdio.h>

#define MAX_ASSET_CODE_LENGTH 6

typedef struct {
    char asset_code[MAX_ASSET_CODE_LENGTH];
    int shares;
} Position;

/**
 * @brief Cria uma nova posição com o código do ativo e a quantidade de ações.
 * 
 * @param position Ponteiro para a estrutura Position a ser inicializada.
 * @param asset_code Código do ativo (deve ter no máximo 5 caracteres + o caractere nulo).
 * @param shares Quantidade de ações na posição.
 * 
 * @return Retorna 0 se a posição foi criada com sucesso, caso contrário retorna um código de erro.
 */
int create_position(Position *position, const char *asset_code, int shares);

#endif // POSITION_H
