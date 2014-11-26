	printf("Spoustim precedencni analyzu");
#define MAX_PT 18
int chyba = 0;
int left = 111;

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
	//case S_E:						{return 18; break; } /* 42 */
	default: 						{return 13; break; } 
	 	
}
}
int isVyraz()
{
myPush(&S, 13);
do
{
	int a = magicFunction(myTop(&S));
	int b = magicFunction(actToken.stav);
	switch(precedent_table[a][b])
	{
		case '=': 
		{
			myPush(&S, b);
			actToken = get_token();
			break;
		}
		case '<':
		{
			if ((myTop(&S)) == 42) {myPop(&S); myPush(&S, left); myPush(&S, 42); myPush(&S, b);}
			else{
				myPush(&S, left);
				myPush(&S, b);
			}
			actToken = get_token();
			break;
		}
		case '>':
		{
			switch (myTop(&S))
			{
				case S_BOOLEAN: /* pravidlo c. 16 */
				{
						myPop(&S);
						if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
						myPush(&S, 42);
						break;
				}
				case S_INTEGER: /* 13 */
				{
						myPop(&S);
						if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
						myPush(&S, 42);
						break;
				}
				case S_DOUBLE: /* 14 */
				{
						myPop(&S);
						if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
						myPush(&S, 42);
						break;
				}
				case S_RETEZEC: /* 15 */
				{
						myPop(&S);
						if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
						myPush(&S, 42);
						break;
				}
				case S_IDENTIFIKATOR: /* 12 */
				{
						myPop(&S);
						if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
						myPush(&S, 42);
						break;
				}
				case S_PRAVA_ZAVORKA:
				{
						myPop(&S);
						if ((myTop(&S)) == 42) 
						{
							myPop(&S);
							if ((myTop(&S)) == magicFunction(S_LEVA_ZAVORKA)) /* 11 */
							{
								myPop(&S);
								if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
								myPush(&S, 42);
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
				case 42:
				{		
						myPop(&S);
						switch (myTop(&S))
						{
							case S_KRAT:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 1 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_DELENO:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 2 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_PLUS:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 3 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_MINUS:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 4 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_MENSI:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 5 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_VETSI:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 6 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_MENSI_NEBO_ROVNO:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 7 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_VETSI_NEBO_ROVNO:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 8 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_ROVNO:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 9 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
								}
								else 
								{
									chyba = 1;
								}
								break;
							}
							case S_NEROVNO:
							{
								myPop(&S);
								if ((myTop(&S)) == 42) /* 10 */
								{
									myPop(&S);
									if (myTop(&S) == left) { myPop(&S); } else {chyba = 1; break ;}
									myPush(&S, 42);
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
while (!((magicFunction(myTop(&S) == S_DOLAR ) && ( magicFunction(actToken.stav) == S_DOLAR))));
if (chyba == 1) {printf("POSRALO SE TO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"); exit(42);}
return chyba;	
}
