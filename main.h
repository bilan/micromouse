#ifndef MAIN_H
#define MAIN_H

//#define DEBUG_MODE

#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "adc.h"
#include "usart.h"
#include "sensor.h"
#include "pwm.h"
#include "encoder.h"
#include "button.h"

#include "controller.h"
#include "stack.h"
#include "algo.h"

#include <math.h>

void button_Interrupt(void);

#endif
