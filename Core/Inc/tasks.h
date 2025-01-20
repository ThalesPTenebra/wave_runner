/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: tasks.h
 * Descrição: Cabeçalho com definições das tarefas do sistema.
 */

#ifndef TASKS_H
#define TASKS_H

#include "cmsis_os.h" // Biblioteca FreeRTOS para threads e filas
#include "config.h"   // Configurações globais
#include "debug.h"    // Debug e logs

// Definição de estruturas de dados para troca de informações entre tarefas

/**
 * @brief Estrutura para dados de sensores.
 * Contém as distâncias medidas pelos beacons e a orientação calculada pelo magnetômetro.
 */
typedef struct {
    float distances[3];    // Distâncias em metros dos beacons
    float orientation;     // Ângulo calculado pelo magnetômetro
} SensorData_t;

/**
 * @brief Estrutura para dados de atuação.
 * Contém as posições calculadas e a direção desejada para o barco.
 */
typedef struct {
    float position[2];     // Posição X e Y em metros
    float heading;         // Direção desejada em graus
} ActuationData_t;

// Prototipação das funções das tarefas
void Task_ReadSensors(void *argument);    // Tarefa de leitura dos sensores
void Task_Trilateration(void *argument);  // Tarefa de cálculo de trilateração
void Task_Actuation(void *argument);      // Tarefa de controle dos atuadores

// Prototipação para criação de tarefas
void CreateTasks(void);

#endif // TASKS_H
