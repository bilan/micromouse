#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h"

#define systemFrequency 168   //MHz

void Systick_Configuration(void);//initialize systick
void delay_ms(u32 nTime);
void delay_us(u32 nTime);
void elapseMicros(u32 targetTime, u32 oldt);
void elapseMillis(u32 targetTime, u32 oldt);

u32 micros(void);
u32 millis(void);
extern volatile u32 Millis;
extern volatile u32 Micros;

#endif


