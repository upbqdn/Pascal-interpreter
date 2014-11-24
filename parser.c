/*
** @author Filip Ježovica xjezov01
**
*/
/* hlavickove soubory */
#include "header.h"
#include "stack.h"
#include "scanner.h"
#include "parser.h"


void extractRule()
{
	switch(myTop(&S))
	{
		
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
				     		myPushMul(&S 4, S_KLIC_READLN, S_LEVA_ZAVORKA, S_IDENTIFIKATOR, S_PRAVA_ZAVORKA);
				     		break;

				     case S_KLIC_WRITE:

				     		myPop(&S);
				     		myPushMul(&S 4, S_KLIC_WRITE, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA);
				     		break;
			    
			     default:   // co tu ? chyba alebo nie ?


				    break;

			  }		


		//--------------------------RHS----------------------//
		case  LL_RHS:
			  if (actToken.stav == S_IDENTIFIKATOR )     // !!!!!treba zmenit (FUNKCIA)
			  {

			  	myPop(&S);
			  	myPushMul(&S, 4, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA );
			  	
			
			  }
			  else 
			  {

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
		     	  else if (actToken.stav == S_REAL)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_REAL, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_STRING)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 2, S_STRING, LL_NSPLIST);

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
		     	  else if (actToken.stav == S_REAL)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_REAL, LL_NSPLIST);

		     	  }	
		     	  else if (actToken.stav == S_STRING)
		     	  {

			  	myPop(&S);
			  	myPushMul(&S, 3, S_CARKA, S_STRING, LL_NSPLIST);

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
		}
		else if (myTop(&S) >= LL_INIT && myTop(&S) <= LL_EXP )  // TERMINAL/NETERMINAL
		{
			// NETERMINAL  velke mismenka
			extractRule(); // rozvin pravidla...
		}
		else
		{
			// TERMINAL male pismenka
			if (myTop(&S) == actToken.stav)
			{
				myPop(&S);	// odstranime z vrcholu zasobnika
				actToken = get_token(); // nacitame novy token
			}
		}
	}



	return true;
}
