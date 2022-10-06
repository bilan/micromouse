#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"

#define read_Vol		   readADC(ADC1, 9,  ADC_SampleTime_84Cycles)
#define read_LF_Sensor     readADC(ADC1, 8,  ADC_SampleTime_84Cycles)
#define read_RF_Sensor     readADC(ADC1, 4,  ADC_SampleTime_84Cycles)
#define	read_L_Sensor      readADC(ADC1, 15,  ADC_SampleTime_84Cycles)
#define read_R_Sensor      readADC(ADC1, 7,  ADC_SampleTime_84Cycles)
#define	read_OUTZ	       readADC(ADC1, 3,  ADC_SampleTime_84Cycles)
#define	read_TEMP	       readADC(ADC1, 14, ADC_SampleTime_84Cycles)

void ADC_Configuration(void);
u16 readADC(ADC_TypeDef* ADCx, u8 channel, uint8_t sampleTime);

#endif
