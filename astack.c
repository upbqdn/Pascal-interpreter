/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

/*
 *Nainkludovanie hackoveho suboru
 */
#include "astack.h"
#include "garbage.h"

/*
 *Premenne definicie
 */
astack aS; //zasobnik

/*
 *Funkcia na inicializaciu zasobnika
 *Parameter: Zasobnik
 */
void astack_init(astack *aS)
{
	aS->First = NULL;
	aS->Second = NULL;
}

/* 
 *Funkcia ktora ulozi na zasobnik hodnotu
 *Parameter: Zasobnik a vkladana adresa
 */
void myaPush(astack *aS , void *adresa)
{
	if(aS->First == NULL) //ak sa pushuje prvy prvok
	{
		astack_element pomocna = mymalloc(sizeof(struct astack_elementS));
		pomocna->adresa = adresa;
		pomocna->ptr_next = aS->First;
		aS->First = pomocna;
		if(aS->First != NULL)
		{
			aS->Second = aS->First->ptr_next; //tuto nasmerueme este aj Second
		}
	}
	else //ak sa pushuje nty prvok
	{
		astack_element pomocna = mymalloc(sizeof(struct astack_elementS));
		pomocna->adresa = adresa;
		pomocna->ptr_next = aS->First;
		aS->First = pomocna;
		aS->Second = aS->First->ptr_next; //tuto nasmerueme este aj Second
	}
}

/*
 *Funkcia na popnutie vrcholu zasobniku
 *Parameter: Zasobnik
 */
void myaPop(astack *aS)
{
	if((aS->First != NULL) && (aS->Second != NULL)) //ak je tam aj First aj Second polozka
	{
		astack_element pomocna = aS->First;
		aS->First = aS->First->ptr_next;
		aS->Second = aS->First->ptr_next;
		myfree(pomocna);
	}
	else if((aS->First != NULL) && (aS->Second == NULL)) //iba first polozka
	{
		astack_element pomocna = aS->First;
		aS->First = aS->First->ptr_next;
		myfree(pomocna);
	}
}

/*
 *Funkcia vrati adresu, ktora sa nachadza na vrchole zasobniku
 *Parameter: Zasobnik
 *Vracia: void adresu, ktora sa nachadza na zasobniku na tope
 */
void *myaTop(astack *aS)
{
	if(aS->First != NULL)
	{
		return aS->First->adresa;
	}
	return NULL;
}

/*
 *Funkcia vrati adresu, ktora sa nachadza tesne pod vrcholom zasobniku
 *Parameter: Zasobnik
 *Vracia: void adresu, ktora sa nachadza na zasobniku na druhom mieste
 */
void *myaSecTop(astack *aS)
{
	if(aS->Second != NULL)
	{
		return aS->Second->adresa;
	}
	return NULL;
}

/*
 *Zrusi cely Zasobnik
 *Parameter: Zasobnik
 */
void destroyaStack(astack *aS)
{
	astack_element pomocna;
	while(aS->First != NULL)
	{
		pomocna = aS->First;
		aS->First = aS->First->ptr_next;
		myfree(pomocna);
	}
	aS->Second = NULL;
}

/*
int main() 
{
	int *a = malloc(sizeof(int));
	*a  = 5;
	int *b = malloc(sizeof(int));
	*b  = 10;
	int *c = malloc(sizeof(int));
	*c  = 15;
	int *d = malloc(sizeof(int));
	*d  = 20;
	astack_init(&aS);
	myaPush(&aS, a);
	printf("%d\n", (*(int*)(myaTop(&aS)))); //nejak takto ked chceme dostat hodnotu zo zasobnikovej adresy!!!!!!!
	myaPush(&aS, b);
	printf("%d\n", (*(int*)(myaSecTop(&aS))));
	myaPush(&aS, c);
	printf("%d\n", (*(int*)(myaSecTop(&aS))));
	myaPush(&aS, d);
	printf("%d\n", (*(int*)(myaSecTop(&aS))));
	myaPop(&aS);
	printf("%d\n", (*(int*)(myaTop(&aS))));
	myaPop(&aS);
	myaPop(&aS);
	myaPop(&aS);
	destroyaStack(&aS);
 	return 0;
}*/
