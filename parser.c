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
#include "stack.h"
#include "scanner.h"
#include "parser.h"
#include "whattoken.h"
#include "ial.h"


list_element Tab_prvok;

void extractRule()
{
	switch(myTop(&S))
	{
		    case LL_INIT:
			{
			    myPop(&S);
			    myPushMul(&S, 6, LL_VLIST, LL_FLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, S_TECKA);
			   
			    // INIT -> VLIST FLIST begin STLIST end.
			}
			break;

			case LL_VLIST:
			if (actToken.stav == S_KLIC_VAR)
			{
			    myPop(&S);
			    
			    myPushMul(&S, 4, S_KLIC_VAR, LL_VDEC, S_STREDNIK, LL_NVLIST);
			    
			    // VLIST -> var VDEC ; NVLIST
			}
			else // eps prechod
			{
			
			    myPop(&S);
			    // VLIST -> eps
			}
			break;


			case LL_VDEC:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
			    myPop(&S);
			    myPushMul(&S, 3, S_IDENTIFIKATOR, S_DVOJTECKA, LL_TYPE);
			   

			    // VDEC -> id : TYPE
			}
			break;


			case LL_NVLIST:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
			    myPop(&S);
			    myPushMul(&S, 3, LL_VDEC, S_STREDNIK, LL_NVLIST);
			  
			    //  NVLIST -> VDEC ; NVLIST
			}
			else // eps prechod
			{
			    myPop(&S);
		
			    //  NVLIST -> eps
			}
			break;



			case LL_TYPE:
				{
					
				switch(actToken.stav) //pri deklaracii
                   {
					case S_KLIC_INTEGER:
					{
					myPop(&S);
					myPush(&S, S_KLIC_INTEGER);
					
					}
					break;

					case S_KLIC_REAL:
					{
					    myPop(&S);
					    myPush(&S, S_KLIC_REAL);
					}
					break;

					case S_KLIC_STRING:
					{
					    myPop(&S);
					    myPush(&S, S_KLIC_STRING);
					  
					}
					break;

					case S_KLIC_BOOLEAN:
					{
					    myPop(&S);
					    myPush(&S, S_KLIC_BOOLEAN);
					  
					}
					break;

					default:
					{
					   // doplnime casom
					}
					break;
				}
			  }
				break;   
                

			case LL_FLIST:
			if (actToken.stav == S_KLIC_FUNCTION)
			{
			    myPop(&S);
			    myPushMul(&S, 9, S_KLIC_FUNCTION, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_PLIST, S_PRAVA_ZAVORKA, S_DVOJTECKA, LL_TYPE, S_STREDNIK, LL_FUNC);
			    //  FLIST -> function id ( PLIST ) : TYPE ; FUNC
			}
			else // eps prechod
			{
			    myPop(&S);
			    
			    //  FLIST -> eps
			}
			break;

			case LL_FUNC:
			if (actToken.stav == S_KLIC_FORWARD)
			{
			    myPop(&S);
			    myPushMul(&S, 3, S_KLIC_FORWARD, S_STREDNIK, LL_FLIST);
			    // FUNC -> forward ; FLIST
			}
			else
			{
			    myPop(&S);
			    myPushMul(&S, 6, LL_VLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, S_STREDNIK, LL_FLIST );
			    // FUNC -> VLIST begin STLIST end ; FLIST
			}
			break;


			case LL_PLIST:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
			    myPop(&S);
			    myPushMul(&S, 4, S_IDENTIFIKATOR,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);
			    // PLIST -> id : TYPE NPLIST
			}
			else // eps prechod
			{
			    myPop(&S);
			    // PLIST -> eps
			}
			break;


			case LL_NPLIST:
			if (actToken.stav == S_STREDNIK)
			{
			    myPop(&S);
			    myPushMul(&S, 5, S_STREDNIK, S_IDENTIFIKATOR,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);
			    // NPLIST -> ; id : TYPE NPLIST
			}
			else // eps prechod
			{
			    myPop(&S);
			    // NPLIST -> eps
			}
			break;

		
		//------------------STLIST------------------------// 
		case  LL_STLIST:
			  if ((actToken.stav == S_IDENTIFIKATOR) || (actToken.stav == S_KLIC_WHILE) || (actToken.stav == S_KLIC_IF) || (actToken.stav == S_KLIC_READLN) || (actToken.stav == S_KLIC_WRITE))
			  {

			  	myPop(&S);
			  	myPushMul(&S, 2, LL_STAT, LL_NSTLIST);
			
			  }
			  else 
			  {
              
			  	myPop(&S);

			  }

			  break;


		//--------------------NSTLIST---------------------//
		case  LL_NSTLIST:
			  if ((actToken.stav == S_IDENTIFIKATOR) || (actToken.stav == S_KLIC_WHILE) || (actToken.stav == S_KLIC_IF) || (actToken.stav == S_KLIC_READLN) || (actToken.stav == S_KLIC_WRITE))
			  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_STREDNIK, LL_STAT, LL_NSTLIST);
			
			  }
			  else 
			  {

			  	myPop(&S);

			  }

			  break;


		//------------------------STAT--------------------//
		case  LL_STAT:
			  switch(actToken.stav)
			      {
				     case S_IDENTIFIKATOR:

				     		myPop(&S);
			     		myPushMul(&S, 3, S_IDENTIFIKATOR, S_PRIRAZENI, LL_RHS);
				     		break;

				     case S_KLIC_WHILE:

				     		myPop(&S);      // mozna zmena LL_E //
				     		myPushMul(&S, 4, S_KLIC_WHILE, LL_E, S_KLIC_DO, LL_BSTAT);
			     		break;

				     case S_KLIC_IF:

				     		myPop(&S);      // mozna zmena LL_E //
				     		myPushMul(&S, 6, S_KLIC_IF, LL_E, S_KLIC_THEN, LL_BSTAT, S_KLIC_ELSE, LL_BSTAT);
				     		break;

				     case S_KLIC_READLN:

				     		myPop(&S);
				     		myPushMul(&S, 4, S_KLIC_READLN, S_LEVA_ZAVORKA, S_IDENTIFIKATOR, S_PRAVA_ZAVORKA);
				     		break;

				     case S_KLIC_WRITE:

				     		myPop(&S);
				     		myPushMul(&S, 4, S_KLIC_WRITE, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA);
				     		break;
			    
			         default:   // co tu ? chyba alebo nie ?


				    break;

			  }		
			  break;


		//--------------------------RHS----------------------//
		case  LL_RHS:
			  if ((actToken.stav == S_IDENTIFIKATOR ) && (F_ID == Tab_prvok->tid))     // zavolat funkciu co pozre do tabulky symbolov a pozre sa na FID porovname
			  {

			  	myPop(&S);
			  	myPushMul(&S, 4, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA );
			  	
			
			  }
			  else 
			  {
//************************************** PRECEDENCNI ANALYZA ZACATEK ***************************************** 
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
if (chyba == 1) {printf("POSRALO SE TO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");} /* if chyba == 0, tak je vyraz v poradku */
//**************************** PRECEDENCNI ANALYZA KONEC ************************************************************************************************************************
                // tuto ma byt chybove hlasenie  aj tento pop ma ist prec // 
			  	myPop(&S);

			  }
			  break;


		//---------------BSTAT---------------//
		case  LL_BSTAT:
			  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END );
						 
			  }
			  break;



		//--------------SPLIST------------------//
		case  LL_SPLIST:                                              // rozne stavy mozu byt dplnit else if
			  if (actToken.stav == S_IDENTIFIKATOR )     
			  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_IDENTIFIKATOR, LL_NSPLIST);
			
			  }
		     	  else if (actToken.stav == S_INTEGER)
		     	  {
			  	myPop(&S);
			  	myPushMul(&S, 2, S_INTEGER, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_REAL)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_KLIC_REAL, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_STRING)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_KLIC_STRING, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_TRUE)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_KLIC_TRUE, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_FALSE)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_KLIC_FALSE, LL_NSPLIST);

		     	  }	
			  else
			  {

			  	myPop(&S);

			  }
			  break;   


		//  -----------NSPLIST-------------------//
		case  LL_NSPLIST:
			  if (actToken.stav == S_IDENTIFIKATOR )     
			  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_IDENTIFIKATOR, LL_NSPLIST);
			
			  }
			  else if (actToken.stav == S_INTEGER)
		     	  {
			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_INTEGER, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_REAL)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_KLIC_REAL, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_STRING)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_KLIC_STRING, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_TRUE)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_KLIC_TRUE, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_KLIC_FALSE)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_KLIC_FALSE, LL_NSPLIST);

		     	  }	
			  else 
			  {

			  	myPop(&S);

			  }

			  break;

			   
	    case LL_E:

              // tuto sa pusti funkcia E 

        
          break;
		  
			}
}


bool parse()
{
	stack_init(&S);
	myPush(&S, EOF); // zarazka $$$$
	myPush(&S, LL_INIT);
	
	actToken = get_token();

	while(actToken.stav != S_END_OF_FILE) // dokym som neni na konci suboru
	{
		
		if (myTop(&S) == EOF && actToken.stav != S_END_OF_FILE)
		{
			//CHYBA!!!! na zasobniku bol uz iba EOF ale my sme este nedocitali subor
			printf("CHyBA vyprazdneny zasobnik a este sme neni na konci suboru\n"); 
		}
		

	    if (myTop(&S) >= LL_INIT && myTop(&S) <= LL_NSPLIST )  // TERMINAL/NETERMINAL
		{
			// NETERMINAL  velke mismenka
			printf("PUSTAM EXTRACT\n");
			extractRule(); // rozvin pravidla...
			printf("HOTOVO EXTRACT\n");
		}
		else
		{
			// TERMINAL male pismenka
			if (myTop(&S) == actToken.stav)
			{
				printf("Pustam TERMINAL  actToken je "); whattoken( actToken.stav);
				printf("PUSTAM TERMINAL  a mam na TOPE %d a zmazem ho "); whattoken(myTop(&S)) ;
				myPop(&S);	// odstranime z vrcholu zasobnika
				//free(actToken.data); // free
				actToken = get_token(); // nacitame novy token
				printf("KOEC TERMINAL GET TOKEN token je "); whattoken( actToken.stav);
				printf("KOEC TERMINAL GET TOKEN TOP  je "); whattoken(myTop(&S)) ;
			}
			else
			{
				printf("mas to napicu ja som cakal  >> "); whattoken(myTop(&S));
				printf("mas to napicu NAPISAL SI    >> "); whattoken( actToken.stav);
				printf("mas to napicu riadok.sltpec >> %d .. %d\n", actToken.radek + 1, actToken.sloupec);
				myPop(&S);	// odstranime z vrcholu zasobnika
				//free(actToken.data); // free
				actToken = get_token(); // nacitame novy token
			}
		}

		if (myTop(&S) != EOF && actToken.stav == S_END_OF_FILE)
		{
			printf("ChyBA na zasobniku nieco zostalo a my sme na konci suboru\n"); 
		}
	}
   

	//free(actToken.data); // free
	return true;
}
