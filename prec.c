/*
-----------------------------------------------------
  @Author: Marek Bielik		xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan		xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica	xjezov01@stud.fit.vutbr.cz
  @Author: Lubomir Matuška	xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár		xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------          
*/

/* hlavickove soubory */
#include "header.h"
#include "scanner.h"
#include "stack.h"
#include "prec.h"
#include "instrlist.h"

#define MAX_PT 18
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
"<<<<<<<<<<<E<O<<<<", // $ 13
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
	default: 						{return 13; break; } 
	 	
}
}
int isVyraz()
{
	printf("Spoustim precedencni analyzu \n");
	int chyba = 0;
	actPrecToken = get_token(); /* pri implementaci do parseru, bude treba odstranit tento radek, protoze precedencni analyza prevezme token od parseru */
	myPush(&S, 13);
	int a = 0;
	int b = 0;
do
{
	// POZNAMKA: "E" = 42, "<" = 111, "$" = 13
	if ((actPrecToken.stav == S_IDENTIFIKATOR) || (actPrecToken.stav == S_INTEGER) || (actPrecToken.stav == S_DOUBLE) || (actPrecToken.stav == S_RETEZEC) || (actPrecToken.stav == S_BOOLEAN))/* + int , double ... */
	{
		printf("GREEP generuji instrukci vloz na zasobnik %d \n", actPrecToken.stav );
		NaplnInstr( I_PREC, NULL, &actPrecToken, NULL );
	}
	
	
	if (myTop(&S) != 42) {a = (myTop(&S));}
	else { myPop(&S); a = myTop(&S); myPush(&S, 42); }
	 
	 int b = magicFunction(actPrecToken.stav);
	
	printf("\naktualni token : "); whattoken(actPrecToken.stav); 
	printf("jeho hodnota je: %d \n", magicFunction(actPrecToken.stav)); 
	printf("na vrcholu zasobniku je : %d \n", a);
	printf("[%d][%d] = %c", a , b, precedent_table[a][b] );
	
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
			printf(" ukoncuji pravidlo = \n ******************** \n");
			break;
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
				printf(" ukoncuji pravidlo < \n ******************** \n");
				break;
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
				printf(" ukoncuji pravidlo < \n ******************** \n");
				break;
			}
		case '>':
			showStack(&S);
			printf(" pouzivam pravidlo > \n");
			if ((myTop(&S) == 12) || (myTop(&S) == 14) || (myTop(&S) == 15) || (myTop(&S) == 16) || (myTop(&S) == 17))
			{
				do 
				{ 
					printf(" popuji %d \n", myTop(&S)); 
					myPop(&S);
				}
				while (myTop(&S) != 111);
				printf(" popuji %d \n", myTop(&S)); 
				myPop(&S);
			}
			else
			{	
				do 
				{ 
					if (myTop(&S) == 0) /* * */
					{ 
						printf("GREEP ********** g e n e r u j i  i n st r u k c i * \n");
						NaplnInstr( I_KRAT, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 1) /* / */ 
					{
						 printf("GREEP ********** g e n e r u j i  i n st r u k c i / \n");
						 NaplnInstr( I_DELENO, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 2) /* + */
					{
						 printf("GREEP ********** g e n e r u j i  i n st r u k c i + \n");
						 NaplnInstr( I_PLUS, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 3) /* - */
					{
						 printf("GREEP ********** g e n e r u j i  i n st r u k c i - \n");
						 NaplnInstr( I_MINUS, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 4) /* < */
					{
						printf("GREEP ********** g e n e r u j i  i n st r u k c i < \n");
						NaplnInstr( I_MENSI, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 5) /* > */
					{
						printf("GREEP ********** g e n e r u j i  i n st r u k c i > \n");
						NaplnInstr( I_VETSI, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 6) /* <= */
					{
						printf("GREEP ********** g e n e r u j i  i n st r u k c i <= \n");
						NaplnInstr( I_MENSIROVNO, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 7) /* >= */
					{
						printf("GREEP ********** g e n e r u j i  i n st r u k c i >= \n");
						NaplnInstr( I_VETSIROVNO, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 8) /* = */
					{
						printf("GREEP ********** g e n e r u j i  i n st r u k c i = \n");
						NaplnInstr( I_ROVNO, NULL, NULL, NULL );
					}
					else if (myTop(&S) == 9) /* <> */
					{
						printf("GREEP ********** g e n e r u j i  i n st r u k c i <> \n");
						NaplnInstr( I_NEROVNO, NULL, NULL, NULL );
					}
					printf(" popuji %d \n", myTop(&S)); 
					myPop(&S);
				}
				while (myTop(&S) != 111);
				printf(" popuji %d \n", myTop(&S));
				myPop(&S);
			}
			printf("vkladam na zasobnik : E = 42 \n");
			myPush(&S, 42);
			showStack(&S);
			printf(" ukoncuji pravidlo > \n ******************** \n");
			break;
			
		case 'O': /* VYRAZ JE OK */ /* O [o, nikoli nula] je specialni stav, kdy na zasobniku je dolar a narazi se na konec vyrazu */
			showStack(&S);
			printf(" pouzivam pravidlo O \n");
			printf(" ukoncuji pravidlo O \n ******************** \n");
			myPop(&S); /* posledni E hazu pryc */
			goto adios ;
	    
	    case 'E': /* VYRAZ JE SPATNE */
			showStack(&S);
			chyba = 1;
			printf("pouzivam pravidlo E \n");
			printf("vyraz je spatne, uklizim zasobnik!! \n ");
			while (myTop(&S) != 13)
			{
				myPop(&S); /* nechavam jen zarazku */
			}
			showStack(&S);
			printf("ukoncuji pravidlo E \n ********************* \n");
			goto adios;
	}
}
while (!((a == 13 ) && (b == 13))); /* naprosto nefugujici podminka jen pro formalitu, obesel jsem ji pomoci skoku */
adios:
if (chyba == 1) {printf("NE, tohle neni vyraz! \n");}
if (chyba == 0) {printf("OK, tohle je vyraz \n");}
printf("Odstranuji umele vytvorenou zarazku a vracim zasobnik do puvodniho stavu\n");
//myPop(&S);
myPop(&S);
showStack(&S);
return chyba;	
}
