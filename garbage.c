/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

#include "garbage.h"
#include "header.h"

trash TrashX;

void trashInit()
{
	
 	TrashX.Act = NULL;
 	TrashX.First = NULL;
 	TrashX.Last = NULL;
}

void addToBin(void *adresa)
{
 	trash_element pomocny = malloc(sizeof(struct elementSTrash));
	if(pomocny == NULL) //chyba alokacie
	{
		trashDestroy(99);
	}
	else //alokacia prebehla spravne
	{
		pomocny->adresa = adresa; //nahrame data
		pomocny->lptr = NULL; //prvok nalavo od pomocneho bude ukazovat na null
		pomocny->rptr = TrashX.First; //prvok napravo od pomocneho bude ukazovat na prvy prvok
		if(TrashX.Last == NULL) //ak to je prve vlozenie
		{
			TrashX.Last = pomocny; //posledny je nas vlozeny prvok
		}
		else //nieje prve vlozenie
		{
			TrashX.First->lptr = pomocny; //prvok nalavo prveho bude ukazovat na pomocny
		}
		TrashX.First = pomocny; //prvy je nas vlozeny prvok
	}
}

void *mymalloc(long long int size)
{
	void *adresa = malloc(size);
	if(adresa == NULL)
	{
		trashDestroy(99);
	}
	trash_element pomocny = malloc(sizeof(struct elementSTrash));
	if(pomocny == NULL) //chyba alokacie
	{
		trashDestroy(99);
	}
	else //alokacia prebehla spravne
	{
		pomocny->adresa = adresa; //nahrame data
		pomocny->lptr = NULL; //prvok nalavo od pomocneho bude ukazovat na null
		pomocny->rptr = TrashX.First; //prvok napravo od pomocneho bude ukazovat na prvy prvok
		if(TrashX.Last == NULL) //ak to je prve vlozenie
		{
			TrashX.Last = pomocny; //posledny je nas vlozeny prvok
		}
		else //nieje prve vlozenie
		{
			TrashX.First->lptr = pomocny; //prvok nalavo prveho bude ukazovat na pomocny
		}
		TrashX.First = pomocny; //prvy je nas vlozeny prvok
	}
	return adresa;
}

void *myrealloc(void *adresa,long long int size)
{
	emptyMem(adresa);
	adresa = realloc(adresa, size);
	if(adresa == NULL)
	{
		trashDestroy(99);
	}
	addToBin(adresa);
	return adresa;
}

void myfree(void *adresa)
{
	TrashX.Act = TrashX.First;
	while(TrashX.Act != NULL)
	{
		if(TrashX.Act->adresa == adresa)
		{
			if(TrashX.Act == TrashX.First) //zoznam je aktivny a aktivny neni posledny
			{
				trash_element pomocny = TrashX.First; //dame si prvy do pomocnej nech ho mozme uvolnit neskor
				if(TrashX.First == TrashX.Last) //ak bol prvy zaroven posledny tak nastavime aktivitu a posledny tiez na nul
				{
					TrashX.First = NULL;
					TrashX.Last = NULL;
					TrashX.Act = NULL;
				}
				else
				{
					if(TrashX.First == TrashX.Act) //ak bol prvy aktivny, zrusime aktivitu tiez
					{
						TrashX.Act = NULL;
					}
					TrashX.First = pomocny->rptr; //prvym sa stava prvok napravo od prveho
					TrashX.First->lptr = NULL; //novy prvy prvok bude ukzovat nalavo na null
				}
				free(pomocny->adresa);
				free(pomocny);
				return;
			}
			if(TrashX.Act == TrashX.Last) //zoznam je aktivny a aktivny neni posledny
			{
				trash_element pomocny = TrashX.Last;
				if(TrashX.First == TrashX.Last) //ak bol prvy zaroven posledny tak nastavime aktivitu a posledny tiez na nul
				{
					TrashX.First = NULL;
					TrashX.Last = NULL;
					TrashX.Act = NULL;
				}
				else
				{
					if(TrashX.Last == TrashX.Act) //ak bol posledny aktivny, zrusime aktivitu tiez
					{
						TrashX.Act = NULL;
					}
					TrashX.Last = pomocny->lptr; //posledny sa stava prvok nalavo od posleneho 
					TrashX.Last->rptr = NULL; //novy posledny bude napravo ukazovat na null
				}
				free(pomocny->adresa);
				free(pomocny);
				return;
			}
			else
			{
				trash_element pomocny = TrashX.Act;
				TrashX.Act->lptr->rptr = TrashX.Act->rptr;
				TrashX.Act->rptr->lptr = TrashX.Act->lptr;
				free(pomocny->adresa);
				free(pomocny);
				return;
			}
		}
		TrashX.Act = TrashX.Act->rptr;
	}
}

void emptyMem(void *adresa)
{
	TrashX.Act = TrashX.First;
	while(TrashX.Act != NULL)
	{
		if(TrashX.Act->adresa == adresa)
		{
			if(TrashX.Act == TrashX.First) //zoznam je aktivny a aktivny neni posledny
			{
				trash_element pomocny = TrashX.First; //dame si prvy do pomocnej nech ho mozme uvolnit neskor
				if(TrashX.First == TrashX.Last) //ak bol prvy zaroven posledny tak nastavime aktivitu a posledny tiez na nul
				{
					TrashX.First = NULL;
					TrashX.Last = NULL;
					TrashX.Act = NULL;
				}
				else
				{
					if(TrashX.First == TrashX.Act) //ak bol prvy aktivny, zrusime aktivitu tiez
					{
						TrashX.Act = NULL;
					}
					TrashX.First = pomocny->rptr; //prvym sa stava prvok napravo od prveho
					TrashX.First->lptr = NULL; //novy prvy prvok bude ukzovat nalavo na null
				}
				free(pomocny);
				return;
			}
			if(TrashX.Act == TrashX.Last) //zoznam je aktivny a aktivny neni posledny
			{
				trash_element pomocny = TrashX.Last;
				if(TrashX.First == TrashX.Last) //ak bol prvy zaroven posledny tak nastavime aktivitu a posledny tiez na nul
				{
					TrashX.First = NULL;
					TrashX.Last = NULL;
					TrashX.Act = NULL;
				}
				else
				{
					if(TrashX.Last == TrashX.Act) //ak bol posledny aktivny, zrusime aktivitu tiez
					{
						TrashX.Act = NULL;
					}
					TrashX.Last = pomocny->lptr; //posledny sa stava prvok nalavo od posleneho 
					TrashX.Last->rptr = NULL; //novy posledny bude napravo ukazovat na null
				}
				free(pomocny);
				return;
			}
			else
			{
				trash_element pomocny = TrashX.Act;
				TrashX.Act->lptr->rptr = TrashX.Act->rptr;
				TrashX.Act->rptr->lptr = TrashX.Act->lptr;
				free(pomocny);
				return;
			}
		}
		TrashX.Act = TrashX.Act->rptr;
	}
}

void trashDestroy(int error)
{
 	while(TrashX.First != NULL) //prejde vsetky prvky zoznamu
	{
		if(TrashX.First == TrashX.Act) //ak je ruseny prvok aktivny, tak zrusi aktivitu 
		{
			TrashX.Act = NULL;
		}
		if(TrashX.First == TrashX.Last) //ak je prvy zaroven poslednym tak zrusi ukazovanie na posledny
		{
			TrashX.Last = NULL;
		}
		trash_element pomocny = TrashX.First; //dame prvy do pomocnej aby sme ho mohli uvolnit
		TrashX.First = TrashX.First->rptr; //prvok napravo od prveho sa stava prvym
		free(pomocny->adresa);
		free(pomocny);
	}
	fclose(soubor);
	exit(error);
}

/*
int main()
{
	trashInit();
	char *a = mymalloc(1);
	char *x = "ahojXEEEEEE";
	strcpy(a, x);
	a = myrealloc(a, sizeof(char) * 6);
	int *b = malloc(sizeof(int));
	addToBin(b);
	int *c = malloc(sizeof(int));
	addToBin(c);
	myfree(b);
	//myfree(a);
	//long long int d = sizeof(char);
	printf("%s", a);
	trashDestroy(0);
 	return 0;
}*/
