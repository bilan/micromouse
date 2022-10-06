#include "adc.h"

void ADC_Configuration(void)
{
	ADC_InitTypeDef       	ADC_InitStruct;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	GPIO_InitTypeDef 		GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/*
	 * ADC1_IN3 - GYRO OUTZ			PA3
	 * ADC1_IN4 - RF Transistor		PA4
	 * ADC1_IN7 - R Transistor		PA7
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*
	 * ADC1_IN8 - LF Transistor 	PB0
	 * ADC1_IN9 - VOL METER			PB1
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*
	 * ADC1_IN14 - GYRO TEMP		PC4
	 * ADC1_IN15 - L Transistor		PC5
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

u16 readADC(ADC_TypeDef* ADCx, u8 channel, uint8_t sampleTime)
{
	ADC_RegularChannelConfig(ADCx, channel, 1, sampleTime);
	ADC_SoftwareStartConv(ADCx);
	while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADCx);
}
