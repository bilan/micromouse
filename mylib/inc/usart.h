#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

/* rozmiar bufora Rx dla USART */
#define MAX_STRLEN 20

extern uint8_t dataReceived;
extern volatile char buforRx[MAX_STRLEN];

void usart1_Configuration(u32 baudrate);
void USART_puts(volatile char *s);
void USART1_IRQHandler(void);

#endif
