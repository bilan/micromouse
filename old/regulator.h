#ifndef REGULATOR_H
#define REGULATOR_H

#include "stm32f4xx.h"
#include "sensor.h"
#include "algorithm.h"

#define V_MAX  	40
#define ACC		0.2
#define IMP_PER_MM		121.095//121.7364
#define IMP_PER_DEGREE 	74			//75.2778
//extern u16 acceleration 	= 0;

#define GYRO_ERROR_MAX	5000 //dopuszczalny b³¹d obrotu +/- 0.5 stopnia, ¿eby nie zastyga³ bo za ma³y b³¹d.

extern int32_t actualV_Left;
extern int32_t actualV_Right;

extern	int32_t actualV_Left_old;
extern	int32_t actualV_Right_old;

extern int32_t regRight;
extern int32_t regLeft;

extern int32_t errorRight;
extern int32_t errorLeft;

extern int32_t errorRightOld;
extern int32_t errorLeftOld;

extern int32_t gyroError;
extern int32_t gyroRightError;
extern int32_t gyroLeftError;

extern int32_t  path;

extern int32_t  pathRDef;
extern int32_t  pathLDef;

extern int32_t  pathLeftRight;
extern int32_t  pathLeftLeft;

extern int32_t 	i;

extern u8 flaga;
extern u8 frontCheck;
extern u8 frontCheck2;

extern u8 turnFlag;

extern float  demandV_Left;			//zadana prêdkosæ
extern float  demandV_Right;

extern int32_t errorRightSensor;
extern int32_t errorLeftSensor;

extern u8 isLeftWall;
extern u8 isRightWall;
extern u8 isFrontWall;

extern u8 VMax;
extern float accSpeedRun;

extern s32 pathAfterTurn; //droga przejechana po wykonaniu skrêtu
extern s32 pathAfterTurn_old;

extern u8 K4;

void profiler(void);
void regulator(void);

void turnLeft(int32_t degree);
void turnRight(int32_t degree);



void wallDetector();

#endif

