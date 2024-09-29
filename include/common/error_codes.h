#ifndef ERROR_CODES_COMMON_H
#define ERROR_CODES_COMMON_H

#include <stdbool.h>

#define ERR_UNCHECKED -1         // Erro não checado
#define ERR_VALIDATION -2        // Erro de validação
#define ERR_MEMORY_ALLOCATION -3 // Erro de alocação de memória
#define ERR_NOT_FOUND -4         // Arquivo não encontrado
#define SUCCESS 0                // Sucesso

const char *get_error_message(int error_code);

bool is_error(int result);

#endif // ERROR_CODES_COMMON_H
