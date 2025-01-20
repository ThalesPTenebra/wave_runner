/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: tasks.c
 * Descrição: Implementação das tarefas do sistema.
 */

#include "tasks.h"
#include "debug.h"
#include "config.h"
#include "devices/JDY18.h"
#include "devices/HMC5883L.h"
#include "queues.h" // Para acessar as filas globais


/* Handles das tarefas */
osThreadId_t taskReadSensorsHandle;
osThreadId_t taskTrilaterationHandle;
osThreadId_t taskActuationHandle;
/**
 * @brief Tarefa responsável por realizar a leitura dos sensores.
 */
void Task_ReadSensors(void *argument) {
    SensorData_t sensorData;

    printf("Task_ReadSensors inicializada.\n");

    while (1) {
        // Leitura das distâncias dos beacons
        JDY18_ScanDevices(); // Atualiza JDY18_RSSI diretamente para distâncias em metros

        // Copia as distâncias dos beacons para o struct SensorData_t
        for (int i = 0; i < 3; i++) {
            sensorData.distances[i] = JDY18_RSSI[i];
        }

        // Leitura do ângulo do magnetômetro
        sensorData.orientation = HMC5883LDriver_GetHeading();

        // Log para depuração
        printf("Leitura dos sensores: Dist1=%.2f, Dist2=%.2f, Dist3=%.2f, Orient=%.2f\n",
               sensorData.distances[0], sensorData.distances[1],
               sensorData.distances[2], sensorData.orientation);

        // Envia os dados para a fila
        if (osMessageQueuePut(sensorDataQueueHandle, &sensorData, 0, osWaitForever) == osOK) {
            printf("Dados de sensores enviados para a fila.\n");
        } else {
            printf("Erro ao enviar dados de sensores para a fila.\n");
        }

        osDelay(100); // Executa a cada 100ms
    }
}


/**
 * @brief Tarefa responsável por calcular a trilateração com base nas leituras dos sensores.
 */
void Task_Trilateration(void *argument) {
    DEBUG_PRINT("Task_Trilateration iniciada.\n");
    SensorData_t sensorData;
    ActuationData_t actuationData;

    while (1) {
        if (osMessageQueueGet(sensorDataQueueHandle, &sensorData, NULL, osWaitForever) == osOK) {
            // Trilateração com as distâncias dos beacons
            float A = -2 * BEACON_X1 + 2 * BEACON_X2;
            float B = -2 * BEACON_Y1 + 2 * BEACON_Y2;
            float C = pow(sensorData.distances[0], 2) - pow(sensorData.distances[1], 2) - pow(BEACON_X1, 2) + pow(BEACON_X2, 2) - pow(BEACON_Y1, 2) + pow(BEACON_Y2, 2);
            float D = -2 * BEACON_X2 + 2 * BEACON_X3;
            float E = -2 * BEACON_Y2 + 2 * BEACON_Y3;
            float F = pow(sensorData.distances[1], 2) - pow(sensorData.distances[2], 2) - pow(BEACON_X2, 2) + pow(BEACON_X3, 2) - pow(BEACON_Y2, 2) + pow(BEACON_Y3, 2);

            actuationData.position[0] = (C * E - F * B) / (E * A - B * D);
            actuationData.position[1] = (C * D - A * F) / (B * D - A * E);
            actuationData.heading = sensorData.orientation;

            DEBUG_PRINT("Position calculada: X=%.2f, Y=%.2f, Heading=%.2f°\n",
                        actuationData.position[0],
                        actuationData.position[1],
                        actuationData.heading);

            osMessageQueuePut(actuationDataQueueHandle, &actuationData, 0, osWaitForever);
        }

        osDelay(100); // Intervalo de execução
    }
}

/**
 * @brief Tarefa responsável por controlar os atuadores do sistema.
 */
void Task_Actuation(void *argument) {
    DEBUG_PRINT("Task_Actuation iniciada.\n");
    ActuationData_t actuationData;

    while (1) {
        if (osMessageQueueGet(actuationDataQueueHandle, &actuationData, NULL, osWaitForever) == osOK) {
            float targetX = BEACON_X1;
            float targetY = BEACON_Y1;

            float errorX = targetX - actuationData.position[0];
            float errorY = targetY - actuationData.position[1];
            float targetHeading = atan2f(errorY, errorX) * 180.0f / M_PI;

            float headingError = targetHeading - actuationData.heading;

            DEBUG_PRINT("Heading Error calculado: %.2f\n", headingError);

            // Controle de atuadores fictício
        }

        osDelay(50); // Intervalo de execução
    }
}

void CreateTasks(void) {
    const osThreadAttr_t taskReadSensorsAttr = {
        .name = "Task_ReadSensors",
        .stack_size = 512 * 4,
        .priority = osPriorityNormal
    };
    taskReadSensorsHandle = osThreadNew(Task_ReadSensors, NULL, &taskReadSensorsAttr);

    const osThreadAttr_t taskTrilaterationAttr = {
        .name = "Task_Trilateration",
        .stack_size = 512 * 4,
        .priority = osPriorityBelowNormal
    };
    taskTrilaterationHandle = osThreadNew(Task_Trilateration, NULL, &taskTrilaterationAttr);

    const osThreadAttr_t taskActuationAttr = {
        .name = "Task_Actuation",
        .stack_size = 512 * 4,
        .priority = osPriorityAboveNormal
    };
    taskActuationHandle = osThreadNew(Task_Actuation, NULL, &taskActuationAttr);

    DEBUG_PRINT("Tarefas criadas com sucesso.\n");
}
