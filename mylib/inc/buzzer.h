#ifndef BUZZER_H
#define BUZZER_H

#include "stm32f4xx.h"

#define BEEP_ON  TIM3->CCR1 = 5 //set the duty cycle as of the loudness for each frequency
#define BEEP_OFF TIM3->CCR1 = 0  //set the duty cycle

void buzzer_Configuration(void);
void shortBeep(int32_t duration);

int16_t buzzerTime;

#endif
