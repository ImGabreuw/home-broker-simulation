#ifndef LOG_COMMON_H
#define LOG_COMMON_H

#include <stdio.h>
#include <pthread.h>

typedef enum
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

/**
 * O uso de um mutex no log é suficiente para garantir a exclusão mútua durante a escrita no arquivo, evitando sobreposição de logs entre threads. 
 * Não é necessário usar uma variável de condição (pthread_cond_t), pois não há dependência de estados ou gerenciamento entre threads nesse contexto.
 */
static pthread_mutex_t log_mutex;

/**
 * Inicializa o sistema de log, abrindo o arquivo especificado.
 *
 * @param filename O nome do arquivo onde os logs serão gravados.
 * Se o arquivo já estiver aberto, ele será fechado antes de ser reaberto.
 */
void log_init(const char *filename);

/**
 * Registra uma mensagem de log no arquivo, incluindo o nível de log e o ID da thread.
 *
 * @param level O nível da mensagem de log (INFO, WARNING, ERROR).
 * @param format A string de formato para a mensagem de log, similar ao printf.
 * Permite uma lista variável de argumentos.
 */
void log_message(LogLevel level, const char *format, ...);

/**
 * Finaliza o sistema de log, fechando o arquivo de log, se aberto.
 */
void log_cleanup();

#endif // LOG_COMMON_H
