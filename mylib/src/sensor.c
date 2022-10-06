#include "sensor.h"

#define e 			2.7183
#define GYRO_SENS 	0.6059*10000 //czu³osæ w ADC/stopieñ/sekunde
#define gyroZero 	2040.67	 //poziom gdzie predkosc k¹towa wynosi 0

int32_t volMeter 	= 0;
int32_t voltage 	= 0;

int32_t LFSensor 	= 0;
int32_t RFSensor 	= 0;
int32_t LSensor 	= 0;
int32_t RSensor 	= 0;

u16 LF 	= 0;
u16 RF 	= 0;
u16 L 	= 0;
u16 R 	= 0;

int32_t gyroTime	= 0;

int32_t Outz 		= 0;
float aSpeed 		= 0;	//angular velocity
int32_t aSpeed_RAW	= 0;
int32_t angle 		= 0;

int32_t angularVelocity = 0;

void readSensor(void)
{
	u32 curt;

	RFSensor 	= read_RF_Sensor;
	LFSensor 	= read_LF_Sensor;
	LSensor 	= read_L_Sensor;
	RSensor 	= read_R_Sensor;

	curt = micros();

	/**************************************************/
	LF_EM_ON;
	elapseMicros(60,curt);
	LFSensor = read_LF_Sensor - LFSensor;
	LF_EM_OFF;

	if(LFSensor < 0)//error check
		LFSensor = 0;

	LF = 10*(22.5*powf(e, -0.0065*LFSensor)+11.24*powf(e, -0.0007*LFSensor));

	elapseMicros(140,curt);
	/**************************************************/
	RF_EM_ON;
	elapseMicros(200,curt);
	RFSensor = read_RF_Sensor - RFSensor;
	RF_EM_OFF;

	if(RFSensor < 0)
		RFSensor = 0;

	RF = 10*(23.59*powf(e, -0.0064*RFSensor)+11.46*powf(e, -0.0007*RFSensor));

	elapseMicros(280,curt);
	/**************************************************/
	SIDE_EM_H_ON;
	elapseMicros(340,curt);
	LSensor = read_L_Sensor - LSensor;
	RSensor = read_R_Sensor - RSensor;
	SIDE_EM_H_OFF;

	if(LSensor < 0)
		LSensor = 0;
	if(RSensor < 0)
		RSensor = 0;

	L = 10*(21.52*powf(e, -0.0066*LSensor)+11.2*powf(e, -0.0008*LSensor));
	R = 10*(21.52*powf (e, -0.0066*RSensor)+11.2*powf(e, -0.0008*RSensor));
	/**************************************************/

	readVol();
	lowBatCheck();
}

void readGyro(void)
{
	aSpeed = (read_OUTZ - gyroZero)*GYRO_SENS;
	//angularVelocity = aSpeed*GYRO_SENS; //predkosc w stopniach/s*10000

	/* usuwanie próbek o ma³ych wartociach - do dupy */
//	if(aSpeed < 2 && aSpeed > -2)
//		aSpeed = 0;

	angle = angle + aSpeed/1000; //kat wychodzi po podzieleniu przez 10000

	/* korekcja zera y = -33.947x + 93933
	 * y - to ile trzeba odj¹æ [ADC]
	 * x - czas [ms]
	 */

	//angle = angle - (-33.947*gyroTime+93933);

	//gyroTime++;
}

void readVol(void)
{
	volMeter = read_Vol;//raw value
	voltage  = volMeter*1100/4550; // *100 aby nie bawiæ siê w floaty
}

void lowBatCheck(void)
{
	unsigned char txt[20] = {0};

	if(voltage < 680)
	{
		sprintf((char *)txt, "VOL. ALARM %d \r\n",  voltage);
		USART_puts(txt);

		while(1)
		{
			targetSpeedX = 0;
			targetSpeedW = 0;
			setRightPwm(0);
			setLeftPwm(0);

			BEEP_OFF;
			ALL_LED_OFF;
			delay_ms(200);

			BEEP_ON;
			ALL_LED_ON;
			delay_ms(200);
		}
	}
}
