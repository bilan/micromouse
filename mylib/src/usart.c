#include "usart.h"

volatile char buforRx[MAX_STRLEN] = {0};
uint8_t dataReceived 	= 0; //syngalizacja odebrania danych przez USART
uint8_t bufRxIndex 		= 0;

void usart1_Configuration(u32 baudrate) //1024000
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // PA9 TX | PA10 RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	USART_OverSampling8Cmd(USART1, ENABLE);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

void USART_puts(volatile char *s)
{
	while(*s)
	{
		// wait until data register is empty
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
		//while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USART1, *s);
		*s++;
	}
}

void USART1_IRQHandler(void)
{
	/* sprawdzam czy zdarzenie pochodzi od otrzymania danej (USART1_IRQ.. moze miec rozne zrodla */
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		char t = USART1->DR;

		if(t != '.' && (bufRxIndex < MAX_STRLEN) )
		{
			buforRx[bufRxIndex] = t;
			bufRxIndex++;
		}
		else
		{
			buforRx[bufRxIndex]='\0';
			bufRxIndex++;

			/* usuniecie smieci z bufora */
			while(bufRxIndex < MAX_STRLEN)
			{
				buforRx[bufRxIndex]=0;
				bufRxIndex++;
			}
			dataReceived = 1;
			bufRxIndex = 0;
		}
	}
}
