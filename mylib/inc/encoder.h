#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx.h"

void Encoder_Configration(void);

int32_t getLeftEncCount(void);
int32_t getRightEncCount(void);
void resetRightEncCount(void);
void resetLeftEncCount(void);

#endif
