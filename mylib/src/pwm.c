#include "pwm.h"

/*
 * TIM4 CH1 (PB6) LEFT
 * TIM4 CH2 (PB7) LEFT
 * TIM4 CH3 (PB8) RIGHT
 * TIM4 CH4 (PB9) RIGHT
 */

/* PWM Duty */
u16 CCR1_Val = 0;
u16 CCR2_Val = 0;
u16 CCR3_Val = 0;
u16 CCR4_Val = 0;

void TIM4_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
}

void TIM4_Configuration()
{
	TIM4_GPIO_Config();

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* 21 kHz */
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 3;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* 1,2,3,4 Channels */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* CH 1 */
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* CH 2 */
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* CH 3 */
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* CH 4 */
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
}

void setLeftPwm(int32_t speed)
{
	if(speed > MAX_PWM)//parameter check
		speed = MAX_PWM;

	if(speed < -MAX_PWM)
		speed = -MAX_PWM;

	if(speed >= 0)//forward
	{
	    L_PWM_R = speed;
	    L_PWM_F	= 0;
	}
    else//rewind
	{
	    L_PWM_R = 0;
	    L_PWM_F	= -speed;
	}
}

void setRightPwm(int32_t speed)
{
	if(speed > MAX_PWM)
		speed = MAX_PWM;

	if(speed < -MAX_PWM)
		speed = -MAX_PWM;

    if(speed >= 0)//forward
    {
        R_PWM_F = speed;
        R_PWM_R	= 0;
    }
    else//rewind
    {
        R_PWM_F = 0;
        R_PWM_R	= -speed;
    }
}


