/*
-----------------------------------------------------
  @Author: Marek Bielik		xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan		xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica	xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška	xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár		xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------          
*/

/* instruction list */

/* hlavickove subory */
#include "header.h"
#include "instrlist.h"

void InitInstrList (tListInstrukcii *I_List) 
{
	I_List->Aktivna = NULL;
	I_List->Prva = NULL;
	I_List->Posledna = NULL;
}

void DestroyInstrList (tListInstrukcii *I_List)
{
	tPrvokListuPtr PomUk; // pomocny ukazatel
	
	while (I_List->Prva != NULL)
	{
		PomUk = I_List->Prva;

		I_List->Prva = I_List->Prva->dalsiPtr;
		free(PomUk);
	}
	I_List->Aktivna = NULL; 
}


void InsertNextInstr (tListInstrukcii *I_List, tInstrukcia DataOfInstr) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
	tPrvokListuPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tPrvokListu))) != NULL)
	{
		PomUk->Instrukcia = DataOfInstr; //nastavenie dátovej zložky
		PomUk->dalsiPtr = NULL; // predám ukazatel, ktorý ukazoval na začiatok
		if (I_List->Prva == NULL)
		{
			I_List->Prva = PomUk;
		}
		else
		{
			I_List->Posledna->dalsiPtr=PomUk;
		}
		I_List->Posledna = PomUk;
	}
	else
	{
		// chyba alokacie !
	}
}

void InstrPrva(tListInstrukcii *I_List)
{
  I_List->Aktivna = I_List->Prva;
}

void InstrDalsia(tListInstrukcii *I_List)
{
	if (I_List != NULL)
	{
		I_List->Aktivna = I_List->Aktivna->dalsiPtr;
	}
  
}

void *InstrDajPoslednyPtr(tListInstrukcii *I_List)
// vrati ukazatel na posledni instrukci
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  return (void*) I_List->Posledna;
}

void InstrGoto(tListInstrukcii *I_List, void *gotoAddr)
// nastavime aktivni instrukci podle zadaneho ukazatele
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  I_List->Aktivna = (tPrvokListuPtr*) gotoAddr; // mozno bez *
}

tInstrukcia *InstrDaj(tListInstrukcii *I_List)
// vrati aktivni instrukci
{
  if (I_List->Aktivna == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }
  else return &(I_List->Aktivna->Instrukcia);
}