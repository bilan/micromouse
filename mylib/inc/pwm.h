#ifndef PWM_H
#define PWM_H

#include "stm32f4xx.h"

#define L_PWM_R TIM4->CCR1
#define L_PWM_F TIM4->CCR2
#define R_PWM_R TIM4->CCR3
#define R_PWM_F TIM4->CCR4

#define MAX_PWM 300 //999 to maks

void TIM4_GPIO_Config(void);
void TIM4_Configuration(void);

void setLeftPwm(int32_t speed);
void setRightPwm(int32_t speed);

#endif
