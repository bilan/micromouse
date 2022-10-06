#include "stack.h"

//liczba elementów na stosie
s16 stackElements = 0;

u8* pull(stos* a)
{
	u8 pos[2]; // pos[0] to X, a pos[1] to Y

	pos[0] = a->posX;
	pos[1] = a->posY;

    return pos;
}

stos*  remove(stos *a)
{
    stos *w = a;	// zapamietujemy to co usuwamy
    a = a->next;	// przeskakujemy usuwany element
    free(w); 		// zwalniamy pamiec przeskoczonego elementu
    stackElements--; //zmniejszamy liczbê elementów na stosie
    return a; 		// tak spreparowana liste zwracamy
}

stos* push(stos *a, u8 nposX, u8 nposY)
{
    stos *newElement = (stos*) malloc(sizeof(stos));//nowy element w pamiec

    newElement->posX 	= nposX;
    newElement->posY 	= nposY;
    newElement->next 	= a;		// wskaznik przypisany do reszty listy

    stackElements++; //swiêkszamy liczbê elementów na stosie

    a = newElement;// podmiana
    return a;// zwracamy nowa liste
}

/*---------------------------------*/
/* obs³uga kolejki ruchów */
//
//void movePush(int move)
//{
//	struct moveQueue *tmp = NULL;
//
//    tmp = (struct moveQueue*)malloc(sizeof(struct moveQueue));
//    tmp->move = move;
//
//    if(pointer == NULL) first = tmp;
//    else (*pointer).ref = tmp; //b¹dŸ pointer->ref
//    tmp -> ref = NULL; //przyk³adowy
//    pointer = tmp;
//}
