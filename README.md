# Documentação do Projeto

## Sumário
- [Introdução](#introdução)
- [Arquitetura do Sistema](#arquitetura-do-sistema)
- [Fluxo de Execução](#fluxo-de-execução)
- [Descrição das Tarefas](#descrição-das-tarefas)
- [Dependências e Configuração](#dependências-e-configuração)
- [Como Rodar o Projeto](#como-rodar-o-projeto)

---

## Introdução
Este projeto é um sistema embarcado projetado para controlar um barco autônomo em uma competição. Ele utiliza FreeRTOS para gerenciar tarefas concorrentes, sensores Bluetooth e magnetômetro para navegação, e controle preciso de motores para posicionamento.

### Objetivo
O objetivo principal é atingir o destino usando trilateração baseada em sinais Bluetooth (RSSI) e orientação do magnetômetro.

---

## Arquitetura do Sistema
O sistema é dividido em três módulos principais:

1. **Leitura de Sensores:**
   - Coleta dados dos sensores JDY-18 (Bluetooth) e HMC5883L (Magnetômetro).

2. **Trilateração:**
   - Processa os dados dos sensores para calcular a posição atual do barco.

3. **Atuação:**
   - Controla os motores com base nos cálculos de posição e orientação para corrigir a trajetória.

### Diagrama de Arquitetura
```
+------------------+
|   Task_ReadSensors   |
| (Sensores JDY18 e    |
|  Magnetômetro)       |
+---------|----------+
          |
          v
+------------------+
| Task_Trilateration |
| (Cálculo de Posição)|
+---------|----------+
          |
          v
+------------------+
| Task_Actuation    |
| (Controle de Motores)|
+------------------+
```

---

## Fluxo de Execução
1. **Inicialização do Sistema:**
   - Configuração de hardware (UART, I2C, GPIO, Timers).
   - Inicialização dos módulos JDY-18 e HMC5883L.

2. **Ciclo de Operação:**
   - **Task_ReadSensors:**
     - Lê os valores dos sensores.
     - Envia dados para a fila `sensorDataQueueHandle`.
   - **Task_Trilateration:**
     - Recebe os dados da fila de sensores.
     - Realiza cálculos de trilateração para determinar a posição atual do barco.
     - Envia os resultados para a fila `actuationDataQueueHandle`.
   - **Task_Actuation:**
     - Recebe os dados da posição atual.
     - Calcula o erro em relação ao destino.
     - Ajusta os motores para corrigir a trajetória.

---

## Descrição das Tarefas

### Task_ReadSensors
**Responsabilidade:**
- Coleta os dados dos sensores JDY-18 e HMC5883L.
- Atualiza as filas globais com as leituras.

**Principais Funções:**
- `JDY18_ScanDevices()`: Lê distâncias dos beacons Bluetooth.
- `HMC5883LDriver_GetHeading()`: Obtém o ângulo de orientação do magnetômetro.

---

### Task_Trilateration
**Responsabilidade:**
- Calcula a posição atual do barco com base nos sinais Bluetooth e na trilateração.

**Principais Funções:**
- Realiza cálculos baseados em coordenadas dos beacons e distâncias medidas.
- Atualiza a fila `actuationDataQueueHandle` com a posição calculada.

**Exemplo de Cálculo:**
```c
float A = -2 * BEACON_X1 + 2 * BEACON_X2;
float B = -2 * BEACON_Y1 + 2 * BEACON_Y2;
float C = pow(distances[0], 2) - pow(distances[1], 2) - pow(BEACON_X1, 2) + pow(BEACON_X2, 2) - pow(BEACON_Y1, 2) + pow(BEACON_Y2, 2);
```

---

### Task_Actuation
**Responsabilidade:**
- Ajusta os motores para corrigir a trajetória do barco com base na posição atual e no destino.

**Principais Funções:**
- Calcula o erro entre o heading atual e o heading desejado.
- Controla os motores para corrigir a direção.

**Exemplo de Cálculo:**
```c
float targetHeading = atan2f(errorY, errorX) * 180.0f / M_PI;
float headingError = targetHeading - actuationData.heading;
```

---

## Dependências e Configuração

### Dependências
- **FreeRTOS**: Gerenciamento de tarefas.
- **HAL (Hardware Abstraction Layer)**: Comunicação com dispositivos embarcados.

### Configuração
1. **Configuração de Hardware:**
   - JDY-18 conectado via UART.
   - HMC5883L conectado via I2C.

2. **FreeRTOS:**
   - Fila para dados dos sensores: `sensorDataQueueHandle`.
   - Fila para dados de trilateração: `actuationDataQueueHandle`.

---