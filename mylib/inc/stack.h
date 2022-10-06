#ifndef STACK_H
#define STACK_H

#include "stm32f4xx.h"
#include <stdlib.h>

typedef struct Stos
{
	u8 posX;
	u8 posY;

    struct Stos *next; 	//wskaznik na element poni¿ej w stosie
} stos;

extern s16 stackElements;

u8* 	pull(stos *); //odczyt 1 elementu
stos* 	remove(stos *); // usuwanie 1 elementu
stos* 	push(stos *, u8 nposX, u8 nposY); // wstawianie na wierzch


/***************************/
//
//struct moveQueue
//{
//    int move;
//    moveQueue *next;
//};
//
//struct moveQueue *pointer = NULL;
//struct moveQueue *first = NULL;

#endif


//example

/*
 *
stos stos;
struct Stos *wsk;

wsk = &stos;
wsk->next = NULL;
wsk->distance = 5;

wsk = push(wsk,10);

sprintf((char *)txt, "element 1: %d \r", pull(wsk));
USART_puts(txt);

wsk = remove(wsk);

sprintf((char *)txt, "element 2: %d \r", pull(wsk));
USART_puts(txt);

*/
