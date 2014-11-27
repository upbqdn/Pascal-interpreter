/*
-----------------------------------------------------
  @Author: Marek Bielik		xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan		xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica	xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matouška	xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár		xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------          
*/

/* hlavickove soubory */
#include "header.h"
#include "scanner.h"
#include "stack.h"
#include "prec.h"

#define MAX_PT 18
int chyba = 0;
int left = 111;

const char precedent_table[MAX_PT][MAX_PT] = {
">>>>>>>>>><><><<<<", // * 0
">>>>>>>>>><><><<<<", // / 1
"<<>>>>>>>><><><<<<", // + 2
"<<>>>>>>>><><><<<<", // - 3 
"<<<<>>>>>><><><<<<", // < 4
"<<<<>>>>>><><><<<<", // > 5
"<<<<>>>>>><><><<<<", // <= 6
"<<<<>>>>>><><><<<<", // >= 7
"<<<<>>>>>><><><<<<", // = 8
"<<<<>>>>>><><><<<<", // <> 9
"<<<<<<<<<<<=<E<<<<", // ( 10
">>>>>>>>>>E>E>EEEE", // ) 11
">>>>>>>>>>E>E>EEEE", // id 12
"<<<<<<<<<<<E<E<<<<", // $ 13
">>>>>>>>>>E>E>EEEE", // integer 14
">>>>>>>>>>E>E>EEEE", // double 15
">>>>>>>>>>E>E>EEEE", // string 16
">>>>>>>>>>E>E>EEEE" // boolean 17
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
	printf("Spoustim precedencni analyzu \n");
	
	actPrecToken = get_token();
	myPush(&S, 13);
	int a = 0;
	int b = 0;
do
{
		if (myTop(&S) != 42) {int a = (myTop(&S));}
	else { myPop(&S); a = myTop(&S); myPush(&S, 42); }
	 
	/****************overenie dole ********/
	//printf("actToken je %d \n", magicFunction(actPrecToken.stav));
	printf("\naktualni token : "); whattoken(actPrecToken.stav); 
	printf("jeho hodnota je: %d \n", magicFunction(actPrecToken.stav)); 
	printf("na vrcholu zasobniku je : %d \n", a);
	//printf("pouzivam pravidlo %c \n", precedent_table[myTop(&S)][magicFunction(actPrecToken.stav)]);

	/*if (actPrecToken.stav == S_END_OF_FILE || actPrecToken.stav == S_STREDNIK)
	{
		printf("KOKOKOKOKOKOT\n");
		break;
	}*/
	
	/****************overenie potialto ********/
	
	int b = magicFunction(actPrecToken.stav);
	
	
	switch(precedent_table[a][b])
	{
		case '=': 
	   showStack(&S);
	   printf(" pouzivam pravidlo = \n");
			myPush(&S, b);
			printf("vkladam na zasobnik : %d \n", b);
			showStack(&S);
			actPrecToken = get_token();
			printf("ZAVOLAL JSEM SI DALSI TOKEN \n");
			printf("\naktualni token : "); whattoken(actPrecToken.stav); 
			printf("jeho hodnota je: %d \n", magicFunction(actPrecToken.stav));
			printf(" ukoncuji pravidlo = \n ******************** \n");
		//	printf(" brejkuji = \n ");
		//	break;
		case '<':
	   showStack(&S);
	   printf(" pouzivam pravidlo < \n");
			if ((myTop(&S)) == 42)
			{
				printf(" popuji %d \n", myTop(&S));
				myPop(&S);
				showStack(&S);
				printf("vkladam na zasobnik : 111 \n");
				myPush(&S, left);
				showStack(&S);
				printf("vkladam na zasobnik : E = 42 \n");
				myPush(&S, 42);
				showStack(&S); 
				printf("vkladam na zasobnik : %d \n", b);
				myPush(&S, b);
				showStack(&S);
				actPrecToken = get_token();
				printf("ZAVOLAL JSEM SI DALSI TOKEN\n");
				printf("\naktualni token : "); whattoken(actPrecToken.stav); 
				printf("jeho hodnota je: %d \n", magicFunction(actPrecToken.stav)); 
				printf(" ukoncuji pravidlo < \n ******************** \n");
			}
			else
			{   printf("vkladam na zasobnik : 111 \n");
				myPush(&S, left);
				showStack(&S);
				printf("vkladam na zasobnik : %d \n", b);
				myPush(&S, b);
				showStack(&S);
				actPrecToken = get_token();
				printf("ZAVOLAL JSEM SI DALSI TOKEN\n");
				printf("\naktualni token : "); whattoken(actPrecToken.stav); 
				printf("jeho hodnota je: %d \n", magicFunction(actPrecToken.stav)); 
				printf(" ukoncuji pravidlo < \n ******************** \n");
			}
		case '>':
			showStack(&S);
			printf(" pouzivam pravidlo > \n");
			//switch(myTop(&S))
			//{			
			//case 12: /* identifikator E -> id */	
			//case 14: /* integer E -> int */	
			//case 15: /* double E -> double */	
			//case 16: /* retezec E -> string */	
			//case 17: /* boolean E -> boolean */	
			//case 2: /* plus E -> E + E */
			//case 0: /* krat E -> E * E */
			//case 1: /* deleno E -> E / E */
			//case 3: /* minus E -> E - E */
				showStack(&S);
				do 
				{ 
					printf(" popuji %d \n", myTop(&S));
					myPop(&S);
				}
				while (myTop(&S) != 111);
				printf(" popuji %d \n", myTop(&S));
				myPop(&S);
				printf("vkladam na zasobnik : E = 42 \n");
				myPush(&S, 42);
				showStack(&S);
				printf(" ukoncuji pravidlo > \n ******************** \n");
			//}
		//	printf(" brejkuji > \n");
		//	break;
			
	//	case 'E': 
	//	showStack(&S);
	//    printf(" pouzivam pravidlo E \n");
	//    printf(" ukoncuji pravidlo E \n ******************** \n");
	//    printf(" popuji %d \n", myTop(&S));
	//    myPop(&S);
	//    showStack(&S);
	//	chyba = 1;
	//	printf(" brejkuji E \n");
	//	exit(42);
	//	break;
	}
}
while ((a != 13 ) && ( b != 13));
if (chyba == 1) {printf("NE, tohle neni vyraz! \n");}
if (chyba == 0) {printf("OK \n");
printf("Odstranuji umele vytvorenou zarazku a vracim zasobnik do puvodniho stavu\n");
myPop(&S);
myPop(&S);
showStack(&S);
}
return chyba;	
}
