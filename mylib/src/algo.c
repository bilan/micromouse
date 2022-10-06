#include "algo.h"

/* deklaracja stosu */
stos Stos;
struct Stos *stackWsk;
/* ---------------- */

/* aktualne po³o¿enie myszy */
struct mouseState mState;

/* odgleg³oci od celu */
u8 ROAD[MAZE_X][MAZE_Y] =
{
//	{ 6, 5, 4, 3 },  /* x=0 */
//	{ 5, 4, 3, 2 },  /* x=1 */
//	{ 4, 3, 2, 1 },  /* x=2 */
//	{ 3, 2, 1, 0 }   /* x=3 */
		{ 14, 13, 12, 11, 10, 9, 8, 7 },  /* x=0 */
		{ 13, 12, 11, 10, 9, 8, 7, 6 },  /* x=1 */
		{ 12, 11, 10, 9, 8, 7, 6, 5 },  /* x=2 */
		{ 11, 10, 9, 8, 7, 6, 5, 4 },  /* x=3 */
		{ 10, 9, 8, 7, 6, 5, 4, 3 },  /* x=4 */
		{ 9, 8, 7, 6, 5, 4, 3, 2 },  /* x=5 */
		{ 8, 7, 6, 5, 4, 3, 2, 1 },  /* x=6 */
		{ 7, 6, 5, 4, 3, 2, 1, 0 }  /* x=7 */
};

/* mapa labiryntu */
u8 WALL[MAZE_X][MAZE_Y] =// {0};
{
		{184, 152, 152, 152, 152, 152, 152, 26 },
		{184, 24, 152, 152, 24, 154, 56, 14, },
		{56, 2, 58, 56, 2, 56, 146, 58 },
		{34, 32, 2, 178, 176, 146, 56, 2 },
		{34, 34, 176, 152, 24, 152, 2, 34 },
		{34, 32, 26, 56, 0, 24, 146, 34 },
		{34, 34, 34, 178, 34, 34, 58, 34 },
		{160, 130, 162, 184, 130, 160, 130, 162}
};
/* przechowuje info czy wybrano s¹siada */
u8 neighbourSelected = 0;

/* aktualnie przetwarzana komórka przez algorytm */
u8 aX, aY = 0;

/* wspó³rzêdne s¹siada w floodfill() */
u8 nX, nY = 0;

/* zmienne do wyznaczania nastêpnego ruchu */
u8 addX, addY, gX, gY = 0;

/* kierunek myszy */
u8 dir = NORTH_WALL;

/* wartosci sasiadów, w [0] przechowuje tego wybranego */
u8 neighbourVal[5] = { 250 };

/* zmienna licznikowa */
u8 i, n = 0;

/* kolejka ruchów */
u8 moveQueue[3] = {0};

/* licznik dystansu */
s32 algoDistance = 0;

unsigned char txt[50] = {0};

//nie dziala powrót;
void algo()
{
	/*
	 * sprawdŸ czy nale¿y wyczuæ scianki, powinno zostaæ to
	 * zrobione SENSE_DIM mm przed komórk¹ lub na starcie
	 */
	if((algoDistance/IMP_PER_MM) > CELL_SIZE+1 || (mState.posX == 0 && mState.posY == 0))
	{
		shortBeep(50);
		printMaze();

				#ifdef DEBUG_MODE
				USART_puts("---\r\n");
				sprintf((char *)txt, "Wjezdzam %d %d\r\n",  mState.posX, mState.posY);
				USART_puts(txt);
				#endif

		/* dla pozycji startowej nie badaj scianek */
		//if(mState.posX != 0 && mState.posY != 0)
		//{
					#ifdef DEBUG_MODE
					USART_puts("check WALL\r\n");
					#endif

			/* usuwam smieci z komórki WALL'a */
			//WALL[mState.posX][mState.posY] = 0;

			/* sens walls */
			if((RF+LF)/2 < FRONT_WALL_TRESH)
			{
				//WALL[mState.posX][mState.posY] |= NORTH_WALL;
				LED4_ON;

					#ifdef DEBUG_MODE
					USART_puts("FR WALL\r\n");
					#endif
			}
			else
				LED4_OFF;

			if(R < SIDE_WALL_TRESH)
			{
				//WALL[mState.posX][mState.posY] |= EAST_WALL;
				LED1_ON;

					#ifdef DEBUG_MODE
					USART_puts("R WALL\r\n");
					#endif
			}
			else
				LED1_OFF;

			if(L < SIDE_WALL_TRESH)
			{
				//WALL[mState.posX][mState.posY] |= WEST_WALL;
				LED9_ON;

					#ifdef DEBUG_MODE
					USART_puts("L WALL\r\n");
					#endif
			}
			else
				LED9_OFF;

			/* zapis orientacji */
			//WALL[mState.posX][mState.posY] |= mState.ori;

				#ifdef DEBUG_MODE
				sprintf((char *)txt, "Przed k %d\r\n", WALL[mState.posX][mState.posY]);
				USART_puts(txt);
				#endif

			/* korekacja oreintacji na NORTH */
			//WALL[mState.posX][mState.posY] = orienationConv(WALL[mState.posX][mState.posY]);

				#ifdef DEBUG_MODE
				sprintf((char *)txt, "Po k %d\r\n",  WALL[mState.posX][mState.posY]);
				USART_puts(txt);
				#endif

				#ifdef DEBUG_MODE
				printMaze();
				#endif
		//}

		/* odbicie scianek */
		mazeReflect();

		/* zerowanie licznika dystansu */
		if (mState.posX == 0 && mState.posY == 0)
		{
			algoDistance = (38+SENSE_DIM)*IMP_PER_MM; //80
			//shortBeep(300);
		}
		else
		{
			//shortBeep(100);
			algoDistance = 0;
		}

		/* ------ */
		floodFill();
		/* ------ */

		/* zezwolenie na ruch */
		movePermit = 1;

			#ifdef DEBUG_MODE
			USART_puts("ZA\r\n");
			#endif
	}

	/* obs³uga kolejki ruchów */
	if(movePermit)	//je¿eli jest zezwolenie na ruch
	{
			#ifdef DEBUG_MODE
			sprintf((char *)txt, "mQ[%d] %d \r\n", n, moveQueue[n]);
			USART_puts(txt);
			#endif

		LED3_OFF;

		//sprintf((char *)txt, "MOVEQ %d\r\n", moveQueue[n]);//sprintf((char *)txt, "aSpeed %f posErrorW %d posErrorX %d\r\n",  aSpeed/10000, posErrorW, posErrorX);
		//USART_puts(txt);

		if(moveQueue[n] == CENTER)
		{
			center();

			/* uaktualnianie licznika dystansu */
			//algoDistance = CELL_SIZE - SENSE_DIM;
		}
		else if(moveQueue[n] == RIGHT)
		{
			turnRight();
		}
		else if(moveQueue[n] == LEFT)
		{
			turnLeft();
		}
		else if(moveQueue[n] == BACK)
		{
			turnBack();
		}
		else if(moveQueue[n] == AHEAD)
		{
			goStraight();
		}



		moveQueue[n] = 0;
		n++;

		if(n > 2)
			n = 0;

		movePermit = 0;
	}
}

void floodFill()
{
	/* wsadŸ komórkê w której jest robot na stos */
	stackWsk = push(stackWsk, mState.posX, mState.posY);

		#ifdef DEBUG_MODE
		sprintf((char *)txt, "Wkladam %d %d\r\n",  mState.posX,  mState.posY);
		USART_puts(txt);
		#endif

	/* obs³uga dojechania do mety i kolejnych przejazdów */
	if (ROAD[mState.posX][mState.posY] == 0)
	{
		ROAD[0][0] = 0;

		stackWsk = push(stackWsk, 0, 0);
		ROAD[7][7] = 200;
	}

	/* etap rozpoznania scianek i ich ODBICIA */
	// WALL[mState.posX][mState.posY] = . . .

	/* je¿eli stos nie jest pusty */
	while(stackElements > 0)
	{
		/* wyjmujê ze stosu */
		aX = stackWsk->posX;
		aY = stackWsk->posY;
		stackWsk = remove(stackWsk);

		#ifdef DEBUG_MODE
		sprintf((char *)txt, "Pull %d %d\r\n",  aX, aY);
		USART_puts(txt);
		#endif

		/*
		 * sprawdazm czy kom-1 = wartoæ któregos z DOSTÊPNYCH s¹siadów
		 * najpierw sprawdzam czy jest dostêpny, a potem czy ma ROAD o 1 miejsze
		 */

		neighbourSelected = 0;

		if ((WALL[aX][aY] & NORTH_WALL) != NORTH_WALL)
		{
			//wspó³rzêdne rozpatrywanego s¹siada, do którego mo¿na pojechaæ
			nX = aX;
			nY = aY + 1;

			if ((ROAD[aX][aY] - 1 == ROAD[nX][nY]))
				neighbourSelected = 1;
		}
		if (!neighbourSelected && (WALL[aX][aY] & EAST_WALL) != EAST_WALL)
		{
			nX = aX + 1;
			nY = aY;

			if ((ROAD[aX][aY] - 1 == ROAD[nX][nY]))
				neighbourSelected = 1;
		}
		if (!neighbourSelected && (WALL[aX][aY] & WEST_WALL) != WEST_WALL)
		{
			nX = aX - 1;
			nY = aY;

			if ((ROAD[aX][aY] - 1 == ROAD[nX][nY]))
				neighbourSelected = 1;
		}
		if (!neighbourSelected && (WALL[aX][aY] & SOUTH_WALL) != SOUTH_WALL)
		{
			nX = aX;
			nY = aY - 1;

			if ((ROAD[aX][aY] - 1 == ROAD[nX][nY]))
				neighbourSelected = 1;
		}


		if (neighbourSelected)
		{
				#ifdef DEBUG_MODE
				sprintf((char *)txt, "Wybr. sas. %d %d\r\n", nX, nY);
				USART_puts(txt);
				#endif

			if(stackElements < 1)
			{
				if (mState.ori == NORTH)
				{
					addX = 0;
					addY = 1;
					dir = NORTH_WALL;
				}
				else if (mState.ori == EAST)
				{
					addX = 1;
					addY = 0;
					dir = EAST_WALL;
				}
				else if (mState.ori == WEST)
				{
					addX = -1;
					addY = 0;
					dir = WEST_WALL;
				}
				else if (mState.ori == SOUTH)
				{
					addX = 0;
					addY = -1;
					dir = SOUTH_WALL;
				}

				//najpierw sprawdŸ czy nie mogê jechaæ w tym samym kierunku co poprzednio
				if ((ROAD[mState.posX + addX][mState.posY + addY] == ROAD[mState.posX][mState.posY] - 1)
					&& (WALL[mState.posX][mState.posY] & dir) != dir)
				{
					gX = mState.posX + addX;
					gY = mState.posY + addY;
				}
				else if (ROAD[mState.posX][mState.posY + 1] == ROAD[mState.posX][mState.posY] - 1
					&& (WALL[mState.posX][mState.posY] & NORTH_WALL) != NORTH_WALL)
				{
					gX = mState.posX;
					gY = mState.posY + 1;
				}
				else if (ROAD[mState.posX + 1][mState.posY] == ROAD[mState.posX][mState.posY] - 1
					&& (WALL[mState.posX][mState.posY] & EAST_WALL) != EAST_WALL)
				{
					gX = mState.posX + 1;
					gY = mState.posY;
				}
				else if (ROAD[mState.posX - 1][mState.posY] == ROAD[mState.posX][mState.posY] - 1
					&& (WALL[mState.posX][mState.posY] & WEST_WALL) != WEST_WALL)
				{
					gX = mState.posX - 1;
					gY = mState.posY;
				}
				else if (ROAD[mState.posX][mState.posY - 1] == ROAD[mState.posX][mState.posY] - 1
					&& (WALL[mState.posX][mState.posY] & SOUTH_WALL) != SOUTH_WALL)
				{
					gX = mState.posX;
					gY = mState.posY - 1;
				}
				else
				{
					/* b³¹d krytyczny - dorobiæ obs³ugê */
				}

				/*
				 *
				 * jedŸ do gX, gY
				 *
				 */

				/* ZMIEN MOUSESTATE! */

					#ifdef DEBUG_MODE
					sprintf((char *)txt, "Jedz %d %d\r\n",  gX, gY);
					USART_puts(txt);
					#endif

				mazeRun();
			}
			else
			{
				//stos nie jest pusty
				#ifdef DEBUG_MODE
				sprintf((char *)txt, "Stos nie pusty\r\n");
				USART_puts(txt);
				#endif
			}
		}
		else
		{
			/* brak dostêpnych s¹siadów */

			for (i = 0; i < 5; i++)
				neighbourVal[i] = 250;

			/* zmieñ wartoæ przetwarzanej komórki na s¹siad + 1 */
			if ((WALL[aX][aY] & EAST_WALL) != EAST_WALL)
			{
				nX = aX + 1;
				nY = aY;

				neighbourVal[1] = ROAD[nX][nY];
			}
			if ((WALL[aX][aY] & WEST_WALL) != WEST_WALL)
			{
				nX = aX - 1;
				nY = aY;

				neighbourVal[2] = ROAD[nX][nY];
			}
			if ((WALL[aX][aY] & SOUTH_WALL) != SOUTH_WALL)
			{
				nX = aX;
				nY = aY - 1;

				neighbourVal[3] = ROAD[nX][nY];
			}
			if ((WALL[aX][aY] & NORTH_WALL) != NORTH_WALL)
			{
				nX = aX;
				nY = aY + 1;

				neighbourVal[4] = ROAD[nX][nY];

			}

			/* wybieram najmniejsz¹ wartoœæ s¹siada */
			for (i = 1; i < 5; i++)
			{
				if (neighbourVal[i] < neighbourVal[0])
					neighbourVal[0] = neighbourVal[i];
			}


			/* jesli przetwarzana komórka nie jest celem */
			if (ROAD[aX][aY] != 0)
			{
				/* zmieniam na wartosc s¹siada +1 */
				ROAD[aX][aY] = neighbourVal[0] + 1;

					#ifdef DEBUG_MODE
					sprintf((char *)txt, "Brak sasiad, %d %d na %d\r\n",  aX, aY, ROAD[aX][aY]);
					USART_puts(txt);
					#endif

				/* zabezpieczenie przed zapêtlaniem na zamkniêtych obszarach */
				if (ROAD[aX][aY] < 210)
				{
					/* wrzucam WSZYSTKICH s¹siadów na stos */
					if (aY + 1 < MAZE_Y)
					{
						stackWsk = push(stackWsk, aX, aY + 1);

							#ifdef DEBUG_MODE
							sprintf((char *)txt, "Stos %d %d\r\n",  aX, aY+1);
							USART_puts(txt);
							#endif
					}
					if (aY - 1 >= 0)
					{
						stackWsk = push(stackWsk, aX, aY - 1);

							#ifdef DEBUG_MODE
							sprintf((char *)txt, "Stos %d %d\r\n",  aX, aY-1);
							USART_puts(txt);
							#endif
					}
					if (aX - 1 >= 0)
					{
						stackWsk = push(stackWsk, aX - 1, aY);

							#ifdef DEBUG_MODE
							sprintf((char *)txt, "Stos %d %d\r\n",  aX-1, aY);
							USART_puts(txt);
							#endif
					}
					if (aX + 1 < MAZE_X)
					{
						stackWsk = push(stackWsk, aX + 1, aY);

							#ifdef DEBUG_MODE
							sprintf((char *)txt, "Stos %d %d\r\n",  aX+1, aY);
							USART_puts(txt);
							#endif
					}
				}
			}
		}
	}
}

void mazeRun()
{
	//gX gY mState.posX mState.posY
	u8 newOri = NORTH;

	if(gX > mState.posX)		// jedŸ w prawo wzglêdem NORTH
	{
		if(mState.ori == NORTH)
		{
			// obkrêæ siê w prawo o 90st
			algoRight();
		}
		else if (mState.ori == EAST)
		{
			// jedŸ prosto
			algoStraight();
		}
		else if (mState.ori == WEST)
		{
			// obkrêæ siê o 180st
			algoBack();
		}
		else if (mState.ori == SOUTH)
		{
			// obkrêæ siê w lewo o 90st
			algoLeft();
		}
		newOri = EAST;
	}
	else if (gX < mState.posX) 	// jedŸ w lewo
	{
		if(mState.ori == NORTH)
		{
			// obkrêæ siê w lewo o 90st
			algoLeft();
		}
		else if (mState.ori == EAST)
		{
			// obkrêæ siê o 180st
			algoBack();
		}
		else if (mState.ori == WEST)
		{
			// jedŸ prosto
			algoStraight();
		}
		else if (mState.ori == SOUTH)
		{
			// obkrêæ siê w prawo o 90st
			algoRight();
		}
		newOri = WEST;
	}
	else if (gY > mState.posY)	// jedŸ w górê
	{
		if(mState.ori == NORTH)
		{
			// jedŸ prosto
			algoStraight();
		}
		else if (mState.ori == EAST)
		{
			// obkrêæ siê w lewo o 90st
			algoLeft();
		}
		else if (mState.ori == WEST)
		{
			// obkrêæ siê w prawo o 90st
			algoRight();
		}
		else if (mState.ori == SOUTH)
		{
			// obkrêæ siê o 180st
			algoBack();
		}
		newOri = NORTH;
	}
	else if (gY < mState.posY)	// jedŸ na dó³
	{
		if(mState.ori == NORTH)
		{
			// obkrêæ siê o 180st
			algoBack();
		}
		else if (mState.ori == EAST)
		{
			// obkrêæ siê w prawo o 90st
			algoRight();
		}
		else if (mState.ori == WEST)
		{
			// obkrêæ siê w lewo o 90st
			algoLeft();
		}
		else if (mState.ori == SOUTH)
		{
			// jedŸ prosto
			algoStraight();
		}
		newOri = SOUTH;
	}
	else
	{
		// critical error
	}

	mState.posX = gX;
	mState.posY = gY;
	mState.ori	= newOri;

	n = 0;
}

void mazeInit()
{
	/* mysz startuje z pkt 0,0 z dziubkiem na pó³noc */
	mState.posX = 0;
	mState.posY = 0;
	mState.ori	= NORTH;

	/* rysowanie obwiedni w orientacji NORTH */

	//dolna czêœæ
	for (i = 0; i < MAZE_X; i++)
		WALL[i][0] |= SOUTH_WALL | NORTH;

	//gorna czesc
	for (i = 0; i < MAZE_X; i++)
		WALL[i][MAZE_Y - 1] |= NORTH_WALL | NORTH;

	//prawa czesc
	for (i = 0; i < MAZE_Y; i++)
		WALL[MAZE_X - 1][i] |= EAST_WALL | NORTH;

	//lewa czesc
	for (i = 0; i < MAZE_Y; i++)
		WALL[0][i] |= WEST_WALL | NORTH;
}

/* odbija scianki pomiedzy komorkami, labirynt powinien byc zapisany w WALL w orientacji NORTH */
void mazeReflect()
{
	u8 x, y = 0;

	//interacja po wszystkich komórkach
	for(y=0; y<MAZE_Y; y++)
	{
		for(x=0; x<MAZE_X; x++)
		{
			if((WALL[x][y] & NORTH_WALL) == NORTH_WALL) //je¿eli jest na pó³nocy sciana
			{
				//sprawdzenie czy nie wykraczam poza labirynt
				if(y+1<MAZE_Y)
				//to komórka wy¿ej ma j¹ na po³udniu
					WALL[x][y+1] |= SOUTH_WALL;
			}
			if((WALL[x][y] & EAST_WALL) == EAST_WALL)
			{
				if(x+1<MAZE_X)
					WALL[x+1][y] |= WEST_WALL;
			}
			if((WALL[x][y] & WEST_WALL) == WEST_WALL)
			{
				if(x-1>=0)
					WALL[x-1][y] |= EAST_WALL;
			}
			if((WALL[x][y] & SOUTH_WALL) == SOUTH_WALL)
			{
				if(y-1>=0)
					WALL[x][y-1] |= NORTH_WALL;
			}
		}
	}

	/* scianka z prawej strony startowej komórki */
	WALL[0][0] = 0b10111000;
}



void algoRight()
{
	movePermit = 1;
	moveQueue[0] = CENTER;
	moveQueue[1] = RIGHT;
	moveQueue[2] = AHEAD;

				#ifdef DEBUG_MODE
				USART_puts("Prawo\r\n");
				#endif
}

void algoLeft()
{
	movePermit = 1;
	moveQueue[0] = CENTER;
	moveQueue[1] = LEFT;
	moveQueue[2] = AHEAD;

				#ifdef DEBUG_MODE
				USART_puts("Lewo\r\n");
				#endif
}

void algoBack()
{
	movePermit = 1;
	moveQueue[0] = CENTER;
	moveQueue[1] = BACK;
	moveQueue[2] = AHEAD;

				#ifdef DEBUG_MODE
				USART_puts("Zawroc\r\n");
				#endif
}

void algoStraight()
{
	movePermit = 1;
	moveQueue[0] = AHEAD;

				#ifdef DEBUG_MODE
				USART_puts("Prosto\r\n");
				#endif
}


u8 orienationConv(u8 data)
{
	u8 ori; 		//orientacja danych
	u8 bitMemory;	// do zapamietywania bitów które tracimy, spieszmy siê kochaæ bity, tak szybko odchodz¹

	ori = (data & 0b01010101);

	if(ori == EAST)
	{
		bitMemory = (data & 0b00000011);
		bitMemory <<= 6;

		data = ((data >> 2) | bitMemory);
	}
	else if(ori == WEST)
	{
		bitMemory = (data & 0b11000000);
		bitMemory >>= 6;

		data = ((data << 2) | bitMemory);
	}
	else if(ori == SOUTH)
	{
		bitMemory = (data & 0b00001111);
		bitMemory <<= 4;

		data = ((data >> 4) | bitMemory);
	}
	else
	{
		//ori to NORTH albo jaki error - nieznana ori
	}

	//korekcja orientacji
	data &= 0b10111010;

	return data;
}

void printMaze()
{
	unsigned char txt[20] = {0};
	//zmienne licznikowe
	u8 r, c = 0;

	USART_puts("Maze:\r\n");
	for(c = 0; c<MAZE_Y; c++)
	{
		for(r=0; r<MAZE_X; r++)
		{
			sprintf((char *)txt, "%d|%d|%d|%d\r\n", WALL[r][c], r, c, ROAD[r][c]);
			USART_puts(txt);
		}
	}
}
