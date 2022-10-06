#include "controller.h"
#include "algo.h"
#include "stack.h"

/**************************/

extern s32 leftEncoder  = 0;
extern s32 rightEncoder = 0;

s32 leftEncoderChange 	= 0;
s32 rightEncoderChange	= 0;
s32 encoderChange		= 0;	// aktualna prêdkosæ w imp/ms

s32 leftEncoderOld		= 0;
s32 rightEncoderOld		= 0;

s32 leftEncoderCount	= 0;
s32 rightEncoderCount	= 0;
s32 encoderCount		= 0;

s32 distanceLeft		= 0;	// pozosta³y dystans do przejechania
s32 rotateDistance		= 0;	// pozosta³y dystans rotacji

/**************************/

float	accX			= 0.4;
float	decX			= 0.6;
float	accW			= 0.1;
float	decW			= 0.1;

float 		curSpeedX		= 0;
s32 		targetSpeedX	= 0;

float 		curSpeedW		= 0;
s32 		targetSpeedW	= 0;

/**************************/

s32		encoderFeedbackX	= 0;
s32		encoderFeedbackW	= 0;
s32		sensorFeedback		= 0;
s32		sensorError			= 0;
s32		posErrorX			= 0;
s32 	posErrorW			= 0;
s32 	posPwmX				= 0;
s32		posPwmW				= 0;
s32		oldPosErrorX		= 0;
s32		oldPosErrorW		= 0;
s32		leftBaseSpeed		= 0;
s32		rightBaseSpeed		= 0;

u8		kpX	= 3;
u8		kdX = 2;
u8		kpW = 3;
u8		kdW = 2;

/**************************/

u8		movePermit	 = 0;	// zewzwolenie na ruch
s32		sensorErrorCounter = 0;
u8		frontCheck  = 0;

/*
 * zmienna ta ustawia siê na 1 gdy:
 * ¿¹dana prêdkosc z regulatora wynosi 0, czyli mysz stoi	(controller.c)
 * gdy mysz doje¿d¿a do punktu w którym wyczuwa scianki i decyduje co zrobi w tej komórce (algo.c)
 *
 * zeruje siê gdy:
 * domyslnie jest wyzerowana
 * po wykonaniu polecenia z warunku if(movePermit)
 *
 */

/**************************/

s32 gyroCenter, gyroAfterTurn = 0;
u8	justRotated = 0; //inforomuje ¿e w³asnie mysz wykona³a obrot

/**************************/

void speedProfile(void)
{
	getSensorError();
	getEncoderStatus();
	updateCurrentSpeed();
	calculateMotorPwm();

	if(distanceLeft < curSpeedX*curSpeedX/(2*decX))
	{
		//trzeba hamowaæ
		targetSpeedX = 0;
	}
	else
	{
		targetSpeedX = VMAX;
	}

	if(abs(rotateDistance) < curSpeedW*curSpeedW/(2*decW))
	{
		targetSpeedW = 0;
	}
	else
	{
		if(rotateDistance > 0)
			targetSpeedW = VMAX;	// w lewo
		else
			targetSpeedW = -VMAX;	// w prawo
	}

	/* warunek pozwalaj¹cy okreliæ, ¿e mysz siê zatrzyma³a
	 * uwaga: currentspeed
	 *  */
	if(movePermit == 0 && abs(curSpeedX) < 1 && abs(curSpeedW) < 1 && targetSpeedX == 0 && targetSpeedW == 0)
	{
		unsigned char txt[15] = {0};
		//sprawdzenie czy obróci³ siê dobrze
		if(justRotated > 0)
		{
			if(justRotated == RIGHT)
				rotateDistance = (angle/10000-90)*IMP_PER_DEGREE;

			if(justRotated == LEFT)
				rotateDistance = (angle/10000+90)*IMP_PER_DEGREE;

			//sprintf((char *)txt, "angle %d \r\n", angle/10000-90);
			//USART_puts(txt);

			if(abs(rotateDistance) < IMP_PER_MM)
				movePermit = 1;

			justRotated   = 0;
		}
		else
		{
			movePermit = 1;
		}
		//USART_puts("ZC\r\n");
	}
	/* ogarniecie distanceLeft, ¿eby resztki nie zostawa³y */
//	if (abs(distanceLeft) < IMP_PER_MM && abs(rotateDistance) < IMP_PER_MM)
//	{
//		distanceLeft = 0;
//		movePermit = 1;
//	}
}

void getEncoderStatus(void)
{
	leftEncoder  = getLeftEncCount()*1.007;
	rightEncoder = getRightEncCount();

	leftEncoderChange  = leftEncoder  - leftEncoderOld;
	rightEncoderChange = rightEncoder - rightEncoderOld;
	encoderChange = (leftEncoderChange+rightEncoderChange)/2;

	leftEncoderOld  = leftEncoder;
	rightEncoderOld = rightEncoder;

	leftEncoderCount  += leftEncoderChange;
	rightEncoderCount += rightEncoderChange;
	encoderCount = (leftEncoderCount+rightEncoderCount)/2;

	distanceLeft 	-= encoderChange;
	rotateDistance 	-= (rightEncoderChange-leftEncoderChange);

	algoDistance += encoderChange;
}

void updateCurrentSpeed(void)
{
	if(curSpeedX < targetSpeedX)		//przyspieszanie
	{
		curSpeedX += accX;

		if(curSpeedX > targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	else if(curSpeedX > targetSpeedX)	//hamowanie
	{
		curSpeedX -= decX;

		if(curSpeedX < targetSpeedX)
			curSpeedX = targetSpeedX;
	}

	/********************************/

	if(curSpeedW < targetSpeedW)		//przyspieszanie
	{
		curSpeedW += accW;

		if(curSpeedW > targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	else if(curSpeedW > targetSpeedW)	//hamowanie
	{
		curSpeedW -= decW;

		if(curSpeedW < targetSpeedW)
			curSpeedW = targetSpeedW;
	}
}

void calculateMotorPwm(void)
{
	s32 rotationalFeedback, gyroFeedback, sensorFeedback = 0;

	encoderFeedbackX = rightEncoderChange + leftEncoderChange;
	encoderFeedbackW = rightEncoderChange - leftEncoderChange;

//	if(aSpeed/10000 > 6 || aSpeed/10000 < -6)
//		gyroFeedback = 2*72*aSpeed/10000;
//	else
//		gyroFeedback = 0;

	//gyroFeedback = -aSpeed/1000000;
	sensorFeedback = sensorError/2;

	rotationalFeedback = encoderFeedbackW + sensorFeedback;	//do ustalenia czy z enko, czy z ¿yro, czy z senso

	posErrorX += curSpeedX - encoderFeedbackX;
	posErrorW += curSpeedW - rotationalFeedback ;

	posPwmX = kpX * posErrorX + kdX * (posErrorX - oldPosErrorX);
	posPwmW = kpW * posErrorW + kdW * (posErrorW - oldPosErrorW);

	oldPosErrorX = posErrorX;
	oldPosErrorW = posErrorW;

	leftBaseSpeed  = posPwmX - posPwmW;
	rightBaseSpeed = posPwmX + posPwmW;

	setLeftPwm(leftBaseSpeed);
	setRightPwm(rightBaseSpeed);
}

void getSensorError(void)
{
	//pathafterturn?
	if(abs(curSpeedX) > 10 && (RF+LF)/2 > 80)
	{
//		if(L < L_SENSOR_MIDDLE && R > R_SENSOR_MIDDLE)
//			sensorError = L_SENSOR_MIDDLE - L;
//		else if(R < R_SENSOR_MIDDLE && L > L_SENSOR_MIDDLE)
//			sensorError =  R - R_SENSOR_MIDDLE;
//		else
//			sensorError = 0;
		LED5_OFF; LED6_OFF;

		if(R < 80 && L < 80)
		{
			sensorError = (R-L)*2;
			LED5_ON; LED6_ON;
		}
		else if (R < 80)
		{
			sensorError = (R - R_SENSOR_MIDDLE);
			LED6_ON;
		}
		else if (L < 80)
		{
			sensorError = (L_SENSOR_MIDDLE - L);
			LED5_ON;
		}
		else
			sensorError = 0;
	}
	else
	{
		sensorError = 0;
	}
		//shortBeep(50);

	//dorownywanie do przedniej scianki
	if(frontCheck)
	{
		if((RF+LF)/2 < 100)
		{
			distanceLeft = 68*IMP_PER_MM;
			frontCheck = 0;
		}
	}
}

/* jedŸ od punktu badania do centrum komórki */
void center()
{
	distanceLeft = (CELL_SIZE/2+SENSE_DIM)*IMP_PER_MM; 	// jedŸ na srodek komorki
	frontCheck = 1; //dorównywanie do przedniej scianki

	//angle = 0; // zeruje k¹t obrotu
	justRotated = 0;
	//gyroCenter = angle/1000; //k¹t przed skrêtem w formacie 00.0 stopnia
}

void turnRight()
{
	rotateDistance = -90*IMP_PER_DEGREE;
	frontCheck = 0;
	//angle = 0;
	justRotated = RIGHT;
}

void turnLeft()
{
	rotateDistance = 90*IMP_PER_DEGREE;
	frontCheck = 0;
	//angle = 0;
	justRotated = LEFT;
}

void turnBack()
{
	rotateDistance = 180*IMP_PER_DEGREE;
	frontCheck = 0;
	//angle = 0;
	justRotated = BACK;
}

void goStraight()
{
	distanceLeft = 2*CELL_SIZE*IMP_PER_MM;
	frontCheck = 0;
	justRotated = 0;
	angle = 0;
	//gyroAfterTurn = angle/1000; //k¹t w formacie 00.0 stopnia
}

