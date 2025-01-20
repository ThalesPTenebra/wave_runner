
#ifndef JDY18_H
#define JDY18_H

#include "stm32f4xx_hal.h" // Certifique-se de que é a biblioteca correta para sua família STM32
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* Configurações */
#define RX_BUFFER_SIZE 254  // Tamanho do buffer de recepção

/* Handles globais */
extern TaskHandle_t JDY18_TaskHandle;        // Handle para a tarefa responsável pelo JDY-18
extern UART_HandleTypeDef huart3;           // Handle UART para comunicação com o JDY-18
extern uint8_t rxBuffer[RX_BUFFER_SIZE];    // Buffer global para recepção

/* Array global para armazenar RSSI */
extern float JDY18_RSSI[3]; // Valores de RSSI/distance dos três beacons
extern int JDY18_BeaconCount; // Número de beacons detectados na última varredura

/* Prototypes das funções */
void JDY18_Init(void);                      // Inicializa o JDY-18
void JDY18_SendCommand(const char* cmd);    // Envia comando AT ao JDY-18
void JDY18_ReceiveResponse(void);           // Recebe resposta do JDY-18
void JDY18_ScanDevices(void);               // Realiza a varredura de dispositivos
float calculateDistance(int rssi);          // Calcula distância com base no RSSI
int ExtractRSSI(char* token);

#endif /* JDY18_H */
