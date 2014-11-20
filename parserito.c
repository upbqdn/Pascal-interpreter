/*
** @author Filip Ježovica xjezov01
**
*/
/* hlavickove soubory */
#include "header.h"
#include "stack.h"
#include "scanner.h"
#include "parserito.h"


void extractRule()
{
	switch(myTop(&S))
	{
		case LL_INIT:
			if (actToken.stav == S_KLIC_BEGIN)
			{
				myPop(&S);
				//myPush(&S, S_TECKA); //este to nemame s texku na konci
				myPush(&S, S_KLIC_END);
				myPush(&S, LL_STLIST);
				myPush(&S, S_KLIC_BEGIN);
			}
			else
			{
				//CHYBA !!
			}
			break;

		case LL_STLIST:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
				myPop(&S);
				myPush(&S, LL_STLIST);
				myPush(&S, S_STREDNIK);
				myPush(&S, LL_STAT);
			}
			else if(actToken.stav == S_KLIC_END)
			{
				// eps prechod
				myPop(&S);
			}
			break;

		case LL_STAT:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
				myPop(&S);
				myPush(&S, LL_EXP);
				myPush(&S, S_PRIRAZENI);
				myPush(&S, S_IDENTIFIKATOR);
			}
			break;

		case LL_EXP:
			if (actToken.stav == S_INTEGER)
			{
				myPop(&S);
				myPush(&S, S_INTEGER);
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