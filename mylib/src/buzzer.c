#include "buzzer.h"

void buzzer_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef  TIM_OCInitStructure;

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	  /* BUZZER PB4 | GPIOB Configuration:  TIM3 CH1 (PB4)  */
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);

	  TIM_TimeBaseStructure.TIM_Period = 100-1;//150  TIM3->AAR
	  TIM_TimeBaseStructure.TIM_Prescaler = 420-1;//140
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//TIM3 clock(84MHz here) divide by 1(0+1) to be the clock to be the base clock of TIM3 PWM
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	  /* PWM1 Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM mode 1, most commond mode
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 0;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//if polarity is high, when you set PWM at 25% duty cycle, the 25% will be high and 75% will be low

	  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//enable the preload for PWM channel 1

	  TIM_ARRPreloadConfig(TIM3, ENABLE);              //enable the proload for TIM3-ARR register
	  TIM_Cmd(TIM3, ENABLE);	// enable TIM3
}

//this is the kinda of beep that you can use when the mouse is running.
//for example, if you want to mouse to beep for 100ms, we turn on the buzzer and set the buzzerTime as the beeping duration
//we also call buzzerTime--; inside of SysTick_Handler inside of stm32f4xx_it.c . so the buzzerTime will reduce by 1 every milliseoncd
//since SysTick_Handler runs every 1 ms.  We also have to include inside of SysTick_Handler something like
//         if(buzzerTime < 0)
//		       beep_off;
//in order to turn off the buzzer after the designated time elaspes. By this way, the MCU doesn't have to wait and't can't do anything else until the buzzer is off.
//this can be use to tell some information such as when the wall-to-no-wall or post-to-no-post transition is detected when mouse is running.
//this is a smarter way than the funtion void beep(int times). However, you can still use void beep(int times) when the mouse stops before anything else going to happen.
void shortBeep(int32_t duration)
{
	BEEP_ON;
	//TIM3->CCR1 = (TIM3->ARR+1)/4;
	//setBuzzerFrequency(freq);
	buzzerTime = duration;
}

void beepON()
{
	TIM3->CCR1 = 20;
	buzzerTime = 300;
}
