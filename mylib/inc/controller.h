#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stm32f4xx.h"
#include "sensor.h"

#define IMP_PER_MM 		120//122.63//122.63 // 120 -niby dzia³a
#define IMP_PER_DEGREE 	162
#define VMAX			50


//VMAX 50 i ACC 0.3 daje nam hamowanie na 33 mm

/**************************/

extern s32 leftEncoder;
extern s32 rightEncoder;
extern s32 encoderCount;
extern s32 leftEncoderChange;
extern s32 rightEncoderChange;

/**************************/

extern s32 	targetSpeedX;
extern s32 	targetSpeedW;

extern float 	curSpeedX;
extern float 	curSpeedW;

/**************************/

extern s32  posErrorX;
extern s32  posErrorW;

extern s32  encoderFeedbackX;
extern s32  gyroFeedback;
extern s32 leftBaseSpeed;
extern s32 rightBaseSpeed;

extern s32 distanceLeft;
extern s32 rotateDistance;

/**************************/

extern u8 	movePermit;
extern s32	gyroAfterTurn;

void 	calculateMotorPwm(void);
void 	updateCurrentSpeed(void);
void 	getEncoderStatus(void);
void 	speedProfile(void);


#endif
