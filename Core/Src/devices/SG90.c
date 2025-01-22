/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: L293D.c
 * Descrição: Driver para controle do módulo L293D.
 */

#include "devices/SG90.h"

#include "pwm_config.h"

int lastDegrees = 91;
static int16_t current_degrees = -999; // Valor inicial inválido

void SG90Driver_SetPosition(TIM_HandleTypeDef* htim, uint32_t channel, int16_t degrees) {
    // Limita os graus entre -90 e 90
    if (degrees < -90) degrees = -90;
    if (degrees > 90) degrees = 90;

    // Mapeia os graus para pulsos em ticks
    uint32_t pulse_ticks = ((degrees + 90) * (200 - 100) / 180) + 100;

    // Converte ticks para duty cycle (fração do período total)
    float duty_cycle = (float)pulse_ticks / 2000.0f; // 2000 é o período total do timer

    // Ajusta o PWM
    PwmService_SetDutyCycle(htim, channel, duty_cycle);
}
