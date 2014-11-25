#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "stack.h"

#define MAX_PT 18
stack s;
tToken token;
int ahoj_jsem_pomoc = 999;
int chyba = 0;
int left = 111;
const S_E = 18; 

int magicFunction(int a)
{
switch (a)
{
	case S_KRAT: 					{return 0; break; }
	case S_DELENO:					{return 1; break; }
	case S_PLUS: 					{return 2; break; }
	case S_MINUS:					{return 3; break; }
	case S_MENSI: 					{return 4; break; }
	case S_VETSI:					{return 5; break; }
	case S_MENSI_NEBO_ROVNO: 		{return 6; break; }
	case S_VETSI_NEBO_ROVNO:		{return 7; break; }
	case S_ROVNO: 					{return 8; break; }
	case S_NEROVNO:					{return 9; break; }
	case S_LEVA_ZAVORKA: 			{return 10; break; }
	case S_PRAVA_ZAVORKA:			{return 11; break; }
	case S_IDENTIFIKATOR: 			{return 12; break; }
	case S_DOLAR:					{return 13; break; }
	case S_INTEGER:					{return 14; break; }
	case S_DOUBLE: 					{return 15; break; }
	case S_RETEZEC:					{return 16; break; }
	case S_BOOLEAN:					{return 17; break; }
	case S_E:						{return 18; break; }
	default: 						{return 13; break; } 
	 	
}
}




int isVyraz()
{


const char precedent_table[MAX_PT][MAX_PT] = {
">>>>>>>>>><><><<<<", // *
">>>>>>>>>><><><<<<", // /
"<<>>>>>>>><><><<<<", // +
"<<>>>>>>>><><><<<<", // -
"<<<<>>>>>><><><<<<", // <
"<<<<>>>>>><><><<<<", // >
"<<<<>>>>>><><><<<<", // <=
"<<<<>>>>>><><><<<<", // >=
"<<<<>>>>>><><><<<<", // =
"<<<<>>>>>><><><<<<", // <>
"<<<<<<<<<<<=<E<<<<", // (
">>>>>>>>>>E>E>EEEE", // )
">>>>>>>>>>E>E<EEEE", // id
"<<<<<<<<<<<E<E<<<<", // $
">>>>>>>>>>E>E<EEEE", // integer
">>>>>>>>>>E>E<EEEE", // double
">>>>>>>>>>E>E<EEEE", // string
">>>>>>>>>>E>E<EEEE", // boolean
};


myPush(&s, 13);
do
{
	int pricti = 0;
	int a = magicFunction(myTop(&s));
	int b = magicFunction(token.stav);
	switch(precedent_table[a][b])
	{
		case '=': 
		{
			myPush(&s, b);
			get_token();
			break;
		}
		case '<':
		{
			if (magicFunction(myTop(&s)) == S_E) {myPop(&s); pricti = 1;}
			ahoj_jsem_pomoc = magicFunction(myTop(&s));
			myPop(&s);
			myPush(&s, left);
			myPush(&s, ahoj_jsem_pomoc);
			myPush(&s, b);
			if (pricti == 1) { myPush(&s, S_E); }
			get_token();
			break;
		}
		case '>':
		{
			switch (myTop(&s))
			{
				case S_BOOLEAN: /* pravidlo c. 16 */
				{
						myPop(&s);
						myPush(&s, magicFunction(S_E));
						break;
				}
				case S_INTEGER: /* 13 */
				{
						myPop(&s);
						myPush(&s, magicFunction(S_E));
						break;
				}
				case S_DOUBLE: /* 14 */
				{
						myPop(&s);
						myPush(&s, magicFunction(S_E));
						break;
				}
				case S_RETEZEC: /* 15 */
				{
						myPop(&s);
						myPush(&s, magicFunction(S_E));
						break;
				}
				case S_IDENTIFIKATOR: /* 12 */
				{
						myPop(&s);
						myPush(&s, magicFunction(S_E));
						break;
				}
				case S_PRAVA_ZAVORKA:
				{
						myPop(&s);
						if (magicFunction(myTop(&s)) == magicFunction(S_E)) 
						{
							myPop(&s);
							if (magicFunction(myTop(&s)) == magicFunction(S_LEVA_ZAVORKA)) /* 11 */
							{
								myPop(&s);
								myPush(&s, S_E);
							}
							else 
							{
								chyba = 1;
							}
						}
						else
						{
							chyba = 1;
						}
						break;
				}
				case S_E:
				{		
						myPop(&s);
						switch (myTop(&s))
						{
							case S_KRAT:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 1 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_DELENO:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 2 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_PLUS:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 3 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_MINUS:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 4 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_MENSI:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 5 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_VETSI:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 6 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_MENSI_NEBO_ROVNO:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 7 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_VETSI_NEBO_ROVNO:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 8 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_ROVNO:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 9 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_NEROVNO:
							{
								myPop(&s);
								if (magicFunction(myTop(&s)) == magicFunction(S_E)) /* 10 */
								{
									myPop(&s);
									myPush(&s, S_E);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
						} /* operator */
				} /* S_E */
			
			}
		
	}
	case 'E': 
	{
		chyba = 1;
		break;
	}
	
	}
}
while (!((magicFunction(myTop(&s) == S_DOLAR ) && ( magicFunction(token.stav) == S_DOLAR))));

return chyba; /* if chyba == 0, tak je vyraz v poradku */
}
