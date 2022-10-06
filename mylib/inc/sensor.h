#ifndef SENSOR_H
#define SENSOR_H

#include "stm32f4xx.h"

#include "adc.h"
#include "delay.h"
#include "buzzer.h"
#include "led.h"
#include "controller.h"

#define hasLeftFrontWall	100
#define hasRightFrontWall	100

#define hasLeftWall			120
#define hasRightWall		120

#define leftMiddleValue		50
#define rightMiddleValue	50

#define L_SENSOR_MIDDLE		65
#define R_SENSOR_MIDDLE		60//60

extern int32_t volMeter;
extern int32_t voltage;

extern int32_t LFSensor;
extern int32_t RFSensor;
extern int32_t LSensor;
extern int32_t RSensor;

extern u16 LF;
extern u16 RF;
extern u16 L;
extern u16 R;

extern int32_t Outz;
extern float aSpeed;
extern int32_t angle;

extern int32_t angularVelocity;

void readSensor(void);
void readGyro(void);
void readVol(void);
void lowBatCheck(void);

#endif
