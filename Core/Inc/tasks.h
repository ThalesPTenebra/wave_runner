/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: tasks.h
 * Descrição: Header das tarefas do sistema.
 */

#ifndef TASKS_H
#define TASKS_H

#include "cmsis_os2.h"     // RTOS
#include "devices/L293D.h" // Para controle do motor L293D

/* Handles das filas globais */
extern osMessageQueueId_t sensorDataQueueHandle;
extern osMessageQueueId_t actuationDataQueueHandle;

/* Estruturas de dados */
typedef struct
{
    float distances[3];
    float orientation;
} SensorData_t;

typedef struct
{
    float position[2];
    float heading;
} ActuationData_t;

// Estrutura para passar argumentos para a Task_Actuation
typedef struct
{
    TIM_HandleTypeDef *motorTimer;   // Handle do timer para o motor
    TIM_HandleTypeDef *servoTimer;   // Handle do timer para o servo
    L293D_HandleTypeDef motorHandle; // Handle do driver do motor
} TaskActuationArgs_t;

// Prototipação das funções
void CreateTasks(TaskActuationArgs_t *actuationArgs);
void Task_ReadSensors(void *argument);
void Task_Trilateration(void *argument);
void Task_Actuation(void *argument);

#endif // TASKS_H