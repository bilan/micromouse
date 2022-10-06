#ifndef ALGO_H
#define ALGO_H

#include "stm32f4xx.h"
#include "controller.h"
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

/* definicje ruchów dla kolejki ruchów */
#define CENTER	1
#define RIGHT	2
#define LEFT	3
#define BACK	4
#define AHEAD	5

#define MAZE_X 		8
#define MAZE_Y 		8

#define CELL_SIZE	180			// d³ugosc scianki w mm

#define SENSE_DIM	20 //40			// odleg³osæ od komórki w jakiej wykonywany jest pomiar obecnosci scianek

// progi wykrycia scianek
#define FRONT_WALL_TRESH	250
#define SIDE_WALL_TRESH		120

extern u8 ROAD[MAZE_X][MAZE_Y];
extern u8 WALL[MAZE_X][MAZE_Y];

extern stos Stos;
extern struct Stos *stackWsk;

extern s32 algoDistance;

/* przechowuje aktualne wspó³rzêdne myszy */
extern struct mouseState
{
	u8 posX;
	u8 posY;
	u8 ori;		//orientacja
};

/* inicjalizacja algorytmu floodfill */
void mazeInit();

/* g³ówna funkcja algorytmu */
void algo();

void floodFill();

void mazeReflect();
u8 orienationConv(u8 data);

#endif
