# Documentação do Projeto

## Sumário

- [Introdução](#introdução)
- [Arquitetura do Sistema](#arquitetura-do-sistema)
- [Fluxo de Execução](#fluxo-de-execução)
- [Descrição das Tarefas](#descrição-das-tarefas)
- [Monitoramento em Tempo Real](#monitoramento-em-tempo-real)
- [Estratégia de Competição](#estratégia-de-competição)
- [Dependências e Configuração](#dependências-e-configuração)
- [Como Rodar o Projeto](#como-rodar-o-projeto)

---

## Introdução

Este projeto é um sistema embarcado projetado para controlar um barco autônomo em uma competição. Ele utiliza FreeRTOS para gerenciar tarefas concorrentes, sensores Bluetooth e magnetômetro para navegação, controle de leme via servo SG90, e controle de motores usando o módulo L293D para posicionamento e propulsão.

### Objetivo

O objetivo principal é atingir o destino usando trilateração baseada em sinais Bluetooth (RSSI) e orientação do magnetômetro, ajustando a trajetória em tempo real.

---

## Arquitetura do Sistema

O sistema é dividido em módulos principais:

1. **Leitura de Sensores:**

   - Coleta dados dos sensores JDY-18 (Bluetooth) e HMC5883L (Magnetômetro).

2. **Trilateração:**

   - Processa os dados dos sensores para calcular a posição atual do barco.

3. **Atuação:**

   - Controla os motores e o leme com base nos cálculos de posição e orientação para corrigir a trajetória.

4. **Monitoramento:**
   - Registra dados do sistema em memória para análise pós-competição.

### Diagrama de Arquitetura

```
+------------------+
| Task_ReadSensors |
| (Sensores JDY18 e|
|  Magnetômetro)   |
+---------|--------+
          |
          v
+------------------+
| Task_Trilateration|
| (Cálculo de Posição)|
+---------|--------+
          |
          v
+------------------+
| Task_Actuation   |
| (Controle de Motores e Leme)|
+---------|--------+
          |
          v
+------------------+
| Task_Monitoring  |
| (Registro de Dados)|
+------------------+
```

---

## Fluxo de Execução

1. **Inicialização do Sistema:**

   - Configuração de hardware (UART, I2C, GPIO, Timers).
   - Inicialização dos módulos JDY-18 e HMC5883L.

2. **Ciclo de Operação:**
   - **Task_ReadSensors:** Coleta dados dos sensores e os envia para a fila `sensorDataQueueHandle`.
   - **Task_Trilateration:** Calcula a posição atual com base nos dados da fila de sensores e envia os resultados para `actuationDataQueueHandle`.
   - **Task_Actuation:** Ajusta os motores e o leme com base na posição e no heading calculados.
   - **Task_Monitoring:** Registra os dados do sistema para análise pós-competição.

---

## Descrição das Tarefas

### Task_ReadSensors

**Responsabilidade:**

- Coleta os dados dos sensores JDY-18 e HMC5883L.
- Atualiza as filas globais com as leituras.

**Funções:**

- `JDY18_ScanDevices()` e `HMC5883LDriver_GetHeading()`.

---

### Task_Trilateration

**Responsabilidade:**

- Calcula a posição atual do barco com base na trilateração.

**Funções:**

- Processa coordenadas dos beacons e distâncias medidas.

---

### Task_Actuation

**Responsabilidade:**

- Controla o leme (servo SG90) e a propulsão (módulo L293D).

**Funções:**

- Calcula o heading desejado e ajusta motores proporcionalmente.

---

### Task_Monitoring

**Responsabilidade:**

- Registra dados de sensores e atuadores em memória para análise.

**Funções:**

- Salva logs com leituras de sensores, posição e estado dos motores.

---

## Estratégia de Competição

A estratégia adotada para vencer a competição é baseada nos seguintes pilares:

1. **Trilateração Precisa:**

   - Usar sinais RSSI dos sensores Bluetooth para calcular a posição exata em relação aos beacons. A posição é continuamente ajustada para garantir precisão.

2. **Correção Dinâmica de Trajetória:**

   - Monitorar constantemente a posição e orientação do barco em relação ao destino, ajustando o leme e a velocidade do motor de forma proporcional ao erro detectado.

3. **Controle Proporcional do Leme:**

   - Implementação de um controle proporcional (P) para o leme com base no erro angular, garantindo curvas suaves e alinhamento eficiente ao destino.

4. **Ajuste de Velocidade Dinâmico:**

   - Diminuir a velocidade do motor ao se aproximar do destino, reduzindo erros de overshooting e garantindo uma parada precisa.

5. **Redundância e Monitoramento:**

   - Registrar dados críticos do sistema durante a operação para análise e ajustes futuros.
   - Implementar tolerância a falhas, como recalibração de sensores em caso de leituras anômalas.

6. **Análise Pós-Competição:**
   - Todos os dados registrados são analisados para identificar pontos de melhoria e otimizar a performance em futuras competições.

---

## Dependências e Configuração

### Dependências

- **FreeRTOS:** Gerenciamento de tarefas.
- **HAL:** Interface com hardware.
- **JDY-18:** Sensores Bluetooth.
- **HMC5883L:** Magnetômetro.

---

## Como Rodar o Projeto

1. **Configuração de Hardware:**

   - Conecte os sensores JDY-18 via UART e o HMC5883L via I2C.
   - Certifique-se de que o SG90 e o módulo L293D estão conectados corretamente.

2. **Compilação:**

   - Use o STM32CubeIDE com o toolchain ARM GCC.

3. **Execução:**
   - Grave o firmware no microcontrolador e conecte o sistema ao barco.
   - Inicie o sistema e observe a execução.

---
