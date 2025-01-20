#include "devices/JDY18.h"

/* Handles globais */
TaskHandle_t JDY18_TaskHandle = NULL;  // Handle da tarefa JDY-18
uint8_t rxBuffer[RX_BUFFER_SIZE];      // Buffer de recepção (declarado uma vez aqui)

//#define BEACON_NAME_1 "PSE2022_B1"
#define BEACON_NAME_1 "D.E.T.E"

#define BEACON_NAME_2 "PSE2022_B2"
#define BEACON_NAME_3 "PSE2022_B3"
#define DEBUG_MODE 1 // Defina como 1 para habilitar debug

/* Variáveis globais */
float JDY18_RSSI[3] = {0.0f, 0.0f, 0.0f}; // Distâncias calculadas para cada beacon
int JDY18_BeaconCount = 0;                // Número de beacons detectados

/* ============================ Funções do JDY-18 ============================ */

/**
 * @brief Inicializa o módulo JDY-18 com as configurações necessárias para a competição.
 */
void JDY18_Init(void) {
    JDY18_SendCommand("AT");             // Entra no modo AT
    JDY18_SendCommand("AT+RESET");       // Reset para estado inicial
    JDY18_SendCommand("AT+NAMEWAVE_RUNNER"); // Configura nome do módulo
    JDY18_SendCommand("AT+ROLE1");       // Define como central (master)
    JDY18_SendCommand("AT+INQM=0,10,24");// Configura modo de inquiry
    JDY18_SendCommand("AT+BAUD4");       // Define baud rate 9600bps
    JDY18_SendCommand("AT+PARITY0");     // Sem paridade
    JDY18_SendCommand("AT+RESET");       // Reinicia para aplicar configurações
}

/**
 * @brief Envia um comando AT para o JDY-18.
 * @param cmd Comando a ser enviado.
 */
void JDY18_SendCommand(const char* cmd) {
    HAL_UART_Transmit(&huart3, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

/**
 * @brief Inicia a recepção de dados do JDY-18.
 */
void JDY18_ReceiveResponse(void) {
    memset(rxBuffer, 0, sizeof(rxBuffer));  // Limpa o buffer
    HAL_UART_Receive(&huart3, rxBuffer, sizeof(rxBuffer), 500); // Timeout ajustável
    if (DEBUG_MODE) {
        printf("Received: %s\n", rxBuffer);
    }
}

/**
 * @brief Realiza a varredura de dispositivos Bluetooth.
 */
void JDY18_ScanDevices(void) {
    JDY18_BeaconCount = 0; // Reseta contadores

    JDY18_SendCommand("AT+INQ");
    JDY18_ReceiveResponse();

    char* start = (char*)rxBuffer;
    while ((start = strstr(start, "+DEV:")) != NULL) {
        int rssi = ExtractRSSI(start);

        // Verifica se o dispositivo corresponde a um dos beacons
        if (strstr(start, BEACON_NAME_1) != NULL) {
            JDY18_RSSI[0] = calculateDistance(rssi);
            #if DEBUG_MODE
            printf("BEACON_NAME_1 -> RSSI: %d, Distance: %.2f meters\n", rssi, JDY18_RSSI[0]);
            #endif
            JDY18_BeaconCount++;
        } else if (strstr(start, BEACON_NAME_2) != NULL) {
            JDY18_RSSI[1] = calculateDistance(rssi);
            #if DEBUG_MODE
            printf("BEACON_NAME_2 -> RSSI: %d, Distance: %.2f meters\n", rssi, JDY18_RSSI[1]);
            #endif
            JDY18_BeaconCount++;
        } else if (strstr(start, BEACON_NAME_3) != NULL) {
            JDY18_RSSI[2] = calculateDistance(rssi);
            #if DEBUG_MODE
            printf("BEACON_NAME_3 -> RSSI: %d, Distance: %.2f meters\n", rssi, JDY18_RSSI[2]);
            #endif
            JDY18_BeaconCount++;
        }

        // Avança para o próximo token
        start = strstr(start, "\r\n");
        if (start != NULL) start += 2;
    }

    #if DEBUG_MODE
    printf("Total Relevant Devices Found: %d\n", JDY18_BeaconCount);
    #endif
}


/**
 * @brief Extrai o valor de RSSI de um token recebido.
 * @param token String do dispositivo detectado.
 * @return RSSI como um número inteiro.
 */
int ExtractRSSI(char* token) {
    // Localiza a primeira vírgula
    char* firstComma = strchr(token, ',');
    if (firstComma != NULL) {
        // Avança para o próximo caractere após a primeira vírgula
        char* rssiStart = firstComma + 1;

        // Verifica se o próximo caractere é um número ou sinal de RSSI (-)
        if (*rssiStart == '-' || (*rssiStart >= '0' && *rssiStart <= '9')) {
            return atoi(rssiStart); // Converte o valor para inteiro
        }
    }
    return -100; // Retorna -100 em caso de falha
}



/**
 * @brief Calcula a distância baseada no RSSI.
 * @param rssi Valor do RSSI recebido.
 * @return Distância estimada em metros.
 */
float calculateDistance(int rssi) {
    static const int rssi_at_one_meter = -59; // RSSI típico a 1 metro
    return pow(10.0, (rssi_at_one_meter - rssi) / (10.0 * 2.0));
}

/* ========================== Callback do UART =============================== */

/**
 * @brief Callback de recepção UART via DMA.
 * @param huart Handle do UART que chamou o callback.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart3) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        /* Notifica a tarefa responsável pelo processamento */
        if (JDY18_TaskHandle != NULL) {
            xTaskNotifyFromISR(JDY18_TaskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
        }

        /* Garante que a tarefa notificada será executada imediatamente */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
