#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

#define LED1_ON    GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define LED1_OFF   GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define LED2_ON    GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define LED2_OFF   GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define LED3_ON    GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define LED3_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_6)

#define LED4_ON    GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define LED4_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define LED5_ON    GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define LED5_OFF   GPIO_ResetBits(GPIOB, GPIO_Pin_13)

#define LED6_ON    GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define LED6_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_2)

#define LED7_ON    GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define LED7_OFF   GPIO_ResetBits(GPIOC, GPIO_Pin_0)

#define LED8_ON    GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define LED8_OFF   GPIO_ResetBits(GPIOC, GPIO_Pin_1)

#define LED9_ON    GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define LED9_OFF   GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define RF_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define RF_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_14)

#define LF_EM_ON   GPIO_SetBits(GPIOH, GPIO_Pin_1)
#define LF_EM_OFF  GPIO_ResetBits(GPIOH, GPIO_Pin_1)

#define SIDE_EM_H_ON   GPIO_SetBits(GPIOH, GPIO_Pin_0); GPIO_SetBits(GPIOC, GPIO_Pin_15)
#define SIDE_EM_H_OFF  GPIO_ResetBits(GPIOH, GPIO_Pin_0); GPIO_ResetBits(GPIOC, GPIO_Pin_15)

#define SIDE_EM_L_ON   GPIO_SetBits(GPIOH, GPIO_Pin_0);
#define SIDE_EM_L_OFF  GPIO_ResetBits(GPIOH, GPIO_Pin_0);

#define ALL_LED_OFF LED1_OFF; \
                    LED2_OFF; \
                    LED3_OFF; \
										LED4_OFF; \
										LED5_OFF; \
										LED6_OFF; \
                    LED7_OFF; \
                    LED8_OFF; \
										LED9_OFF


#define ALL_LED_ON  LED1_ON; \
                    LED2_ON; \
										LED3_ON; \
										LED4_ON; \
										LED5_ON; \
										LED6_ON; \
                    LED7_ON; \
                    LED8_ON; \
										LED9_ON

void LED_Configuration(void);

#endif /* LED_H */
