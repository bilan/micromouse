#include "main.h"

/* obs³uga startu */
uint8_t start = 1;

void systick(void)
{
	speedProfile();
}

int main(void)
{
	unsigned char txt[50] = {0};
	u32 curt, timer2 = 0;

	s8 p = 0;

	Systick_Configuration();
	LED_Configuration();
	button_Configuration();
	buzzer_Configuration();
	usart1_Configuration(1382400);
	TIM4_Configuration();
	Encoder_Configration();
	ADC_Configuration();

	/* procedura startowa */
	USART_puts("\r\nD E N D R Y T\r\n");
	shortBeep(100);
	delay_ms(150);
	shortBeep(100);

	LED8_ON;

	while(start); //czekaj na wciniêcie przycisku
	start = 1; //spowrotem ustaw na 1
	shortBeep(50);
	USART_puts("READY...\r\n");
	delay_ms(4000);

	resetRightEncCount();
	resetLeftEncCount();

	//setRightPwm(1000);
	//setLeftPwm(1000);

	/* inicjalizacja algorytmu */
	mazeInit();

	//algoRight();

	//distanceLeft = 1349*IMP_PER_MM;
	timer2 = micros();
    while(1)
    {
    	curt = micros();
		readGyro();
		readSensor();

		algo();

    	if(curt-timer2 > 10000000)
    	{
    		//sprintf((char *)txt, "R %d L %d FRONT %d angle %d \r\n",  R, L, (RF+LF)/2, angle/10000);
    		//USART_puts(txt);
    		//printMaze();

    		//sprintf((char *)txt, "angle %d \r\n", angle/10000);
    		//USART_puts(txt);

    		//sprintf((char *)txt, "posErrorX %d posErrorW %d encoderFeedbackX %d curSpeedX %f curSpeedW %f leftBaseSpeed %d \r\n",  posErrorX, posErrorW, encoderFeedbackX, curSpeedX, curSpeedW, leftBaseSpeed);
    		//USART_puts(txt);

    		//sprintf((char *)txt, "%f %f %d %d %d\r\n", curSpeedX, curSpeedW, targetSpeedX, targetSpeedW, distanceLeft);
    		//sprintf((char *)txt, "aSpeed %f posErrorW %d posErrorX %d\r\n",  aSpeed/10000, posErrorW, posErrorX);
    	    //USART_puts(txt);

    		timer2 = micros();
    	}


    	elapseMicros(1000, curt);
    }


}

void button_Interrupt()
{
	start = 0;
	//shortBeep(30);
}


