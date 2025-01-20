/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: queues.h
 * Descrição: Declaração de filas globais para comunicação entre tarefas.
 */

#ifndef QUEUES_H
#define QUEUES_H

#include "cmsis_os.h" // Biblioteca para manipulação de filas

// Declaração externa das filas globais
extern osMessageQueueId_t sensorDataQueueHandle;    // Fila para dados de sensores
extern osMessageQueueId_t actuationDataQueueHandle; // Fila para dados de atuação

#endif // QUEUES_H
