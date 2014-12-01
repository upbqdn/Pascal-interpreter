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

tListInstrukcii listok;  // globalne >> v .h je extern

/* INICIALIZACIA INSTRUKCNEJ PASKY .. vsetko null */
void InitInstrList (tListInstrukcii *I_List)
{
    I_List->Aktivna = NULL;
    I_List->Prva = NULL;
    I_List->Posledna = NULL;
}


/* ZMAZAT CELU INSTRUKCNU PASKU + ODALOKOVAT VSETKO ! */
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


/* Funkcia VLOZI  novu INSTRUKCIU do instrukcnej pasky, predame jej instrukciu(strukturu) */
void InstrInsert (tListInstrukcii *I_List, tInstrukcia DataOfInstr)
{
    tPrvokListuPtr PomUk;

    if ((PomUk = malloc(sizeof(struct tPrvokListu))) == NULL) // keby nastane chyba alokacie..
    {
        // chyba alokacie !!
    }
    else // alokacia sa podarila mozme vkladat data
    {
        PomUk->Instrukcia = DataOfInstr; // skopirujeme celu inštrukciu
        PomUk->dalsiPtr = NULL; // dalsi neexistuje za nim nikdy cize NULL !

        /* ak je toto prva inštrukcia na inštrukcnej paske */
        if (I_List->Prva == NULL)
        {
            // tak je prvou instrukciou na paske
            I_List->Prva = PomUk;
        }
        else // ak uz tam nieco bolo tak musime kopirovat ukazatel
        {
            /* ta co bola ako posledna musi ukazovat na novu */
            I_List->Posledna->dalsiPtr=PomUk;
        }
        /* poslednou sa stáva práve vkladaná instrukcia*/
        I_List->Posledna = PomUk;
    }
}


/* Tato funkcia naprace INSTRUKCIU do instrukcnej pasky */
/* 4 PARAMETRE
** 1 > AKCIA AKA SA MA VYKONAT {int konstanta} : JMP, +,-,*,/,aloc,....
** 2 > ADDR_KDE 	> KAM SA MA UKLADAT nieco, pri jednoadresovych instrukciach sa pouziva univerzalne ostatne budu NULL!
** 3 > ADDR_PRVA 	> ODKIAL sa cita 1 adresa
** 4 > ADDR_DRUHA	> ODKIAL sa cita 2 adresa
** ::: vsetky adresy su typu void * >> pri pouzivani pouzi PRETYPOVANIE :::
*/
void NaplnInstr(tINSTR AKCIA, void *ADDR_KDE, void *ADDR_PRVA, void *ADDR_DRUHA)
{
    tInstrukcia Ins;
    Ins.AKCIA = AKCIA;
    Ins.ADDR_KDE = ADDR_KDE;
    Ins.ADDR_PRVA = ADDR_PRVA;
    Ins.ADDR_DRUHA = ADDR_DRUHA;
    InstrInsert(&listok, Ins);

}


/* da nam prave AKTIVNU poziciu v paske */
void *InstrDajPoz(tListInstrukcii *I_List)
{
    return (void *) I_List->Aktivna;
}


/* ADRESA POSLEDNEJ POZICIE INSTRUKCIE v paske*/
void *InstrDajPosledPoz(tListInstrukcii *I_List)
{
    return (void*) I_List->Posledna;
}


/* da nam prave AKTIVNU celu instrukciu (strukturov) */
tInstrukcia *DajInstr(tListInstrukcii *I_List)
{
    if (I_List->Aktivna == NULL)
    {
        return NULL; // ziadna AKTIVNA instr
    }
    else
    {
        return &(I_List->Aktivna->Instrukcia);
    }
}


/* AKTIVUJE INSTRUKCNU PASKU od PRVEJ INSTRUKCIE */
void InstrStart(tListInstrukcii *I_List)
{
    I_List->Aktivna = I_List->Prva;
}


/* podla zvolenej adresy  v druhom operande AKTIVUJE DANU INSTRUKCIU */
void InstrGoto(tListInstrukcii *I_List, void *gotoAddr)
{
    I_List->Aktivna = (tPrvokListuPtr) gotoAddr; // mozno bez *
}


/* POSUN SA O INSTRUKCIU DALEJ */
void InstrDalsia(tListInstrukcii *I_List)
{
    if (I_List != NULL) // je to mozne iba ak máme vloženú aspon jednu inštrukciu inac by sme mali chyby
    {
        /* Nastavi NASLEDUJUCU INSTRUKICU aby bola aktivna, ak by nebola dalsia najdeme tam NULL */
        I_List->Aktivna = I_List->Aktivna->dalsiPtr;
    }
}




/* SPECIAL FUNKCION */
/* premeni data napr z tokenu na odpovedajuce typy
** pretoze mi ich dostavame ako char..
*/

void *spracuj(tStav TIPSTAV, char* data)
{
    switch(TIPSTAV)
    {

        case S_INTEGER:
        {
            void *pomocnicek = malloc(sizeof(int));
            if (pomocnicek == NULL) // chyba alokacie
            {
                return NULL;
            }
            *(int *) pomocnicek = atoi(data);
            return pomocnicek;
        }

        case S_DOUBLE:
        {
            void *pomocnicek = malloc(sizeof(float));
            if (pomocnicek == NULL) // chyba alokacie
            {
                return NULL;
            }
            *(float *) pomocnicek = atof(data);
            return pomocnicek;
        }


        /* take iste lebo identifikator vraciam ako string 
        ** koli zapichu do tabulky
        ** STRING - klasicky ako aj tie cisla hore
        */
        case S_RETEZEC:
        case S_IDENTIFIKATOR:
        {
            int dlzka = strlen(data);
            void *pomocnicek = malloc(((sizeof(char))*dlzka)+1);
            if (pomocnicek == NULL) // chyba alokacie
            {
                return NULL;
            }
            strcpy( pomocnicek, data);
            return pomocnicek;
        }

        case S_KLIC_TRUE:
        {
            void *pomocnicek = malloc(sizeof(bool));
            if (pomocnicek == NULL) // chyba alokacie
            {
                return NULL;
            }
            *(bool *) pomocnicek = true;
            return pomocnicek;
        }
        case S_KLIC_FALSE:
        {
            void *pomocnicek = malloc(sizeof(bool));
            if (pomocnicek == NULL) // chyba alokacie
            {
                return NULL;
            }
            *(bool *) pomocnicek = false;
            return pomocnicek;
        }


        default:
        return NULL; // chybiska ..
    }
    
}
