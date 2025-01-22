/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: tasks.h
 * Descrição: Header das tarefas do sistema.
 */

#ifndef TASKS_H
#define TASKS_H

#include "cmsis_os2.h" // RTOS
#include "devices/L293D.h"     // Para controle do motor L293D

/* Handles das filas globais */
extern osMessageQueueId_t sensorDataQueueHandle;
extern osMessageQueueId_t actuationDataQueueHandle;

/* Estruturas de dados */
typedef struct {
    float distances[3];
    float orientation;
} SensorData_t;

typedef struct {
    float position[2];
    float heading;
} ActuationData_t;

/* Prototipação das funções */
void CreateTasks(L293D_HandleTypeDef* motorHandle);
void Task_ReadSensors(void *argument);
void Task_Trilateration(void *argument);
void Task_Actuation(void *argument);

#endif // TASKS_H
