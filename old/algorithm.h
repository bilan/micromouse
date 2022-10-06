#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "stm32f4xx.h"
#include "regulator.h"
#include "stack.h"
#include "main.h"

#define NORTH 	 0b00010000
#define EAST	 0b00000100
#define WEST	 0b01000000
#define SOUTH	 0b00000001

#define NORTH_WALL 	 0b00000010
#define EAST_WALL	 0b10000000
#define WEST_WALL	 0b00001000
#define SOUTH_WALL	 0b00100000

#define MAZE_X 4
#define MAZE_Y 4

#define DEBUG_STOP while(1){ shortBeep(20); setRightPwm(0); setLeftPwm(0); }

#define GO_STRAIGHT		1
#define TURN_RIGHT  	2
#define TURN_LEFT   	3
#define TURN_BACK   	4
#define CENTER  		5	//dojedŸ do centrum komórki
#define SET_CENTER_VARS 6

extern s32 aencR_old;
extern u8 map_flag;

extern u8 ROAD[MAZE_X][MAZE_Y];
extern u8 WALL[MAZE_X][MAZE_Y];
extern u8 orientation;

extern s8 X, Y;

extern u8 firstMove, secondMove, thirdMove;

extern stos Stos;
extern struct Stos *wsk;

extern u8 searchingFinished;
extern u8 speedMode;

extern u8 numberSpeedMode;

extern u8 checkNextCell;

void mazeMapper(void);
u8 orienationConv(u8 data);

#endif
