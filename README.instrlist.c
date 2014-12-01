/* 	INSTRLIST.README  -- NIEJE POTREBA ODOVZDAVAT !!!!  je pre pochopenie ako pracovat s instruknou paskou
**	MOZNOST  nakopirovat do nejakeho adresara > premenovat na main.c a hura mozme testovat
*/

/* PRECITAT + nazriet do kodu ...snazil som sa to napisat dost zrozumitelne ;)
** citat i zakomentovane useky
** je to cele bez erroru aj bez warningov
**
** Naplnili sme 2 instrukcie, aktivovali pasku od prvej instrukcie, posunuli sa na dalsiu,
** Zapametali si ADRESU AKTIVNEJ POZICIE, >> tPrvokListuPtr PAMAT_POZICIA = InstrDajPoz(&listok);
** Potom koli ukazke skoku sme vykonali skok na zapametanu POZICIU  >> InstrGoto(&listok, PAMAT_POZICIA);
** cize teraz by mala byt AKTIVNA zase ta prva instrukcia {ta 42, "nejaky retazec"}
** vytiehneme si tu instrukciu >> OUT = DajInstr(&listok);
** pouzijeme nejake pomocne premenne kde si ulozime data z instrukcie {AKCIU, ADRR,...}
** dame si vipisat >>> POZOR >>> pouzivame pretypovanie podla TYPU {int,char,...}
** TYP sa bude zistovat napr z tabulky symbolov > aspon si myslim
** Nakoniec VYMAZEME celu instrukcnu pasku.
*/


/* hlavickove subory */
#include "header.h"
#include "instrlist.h"

int main()
{
    InitInstrList(&listok); // inicializacia instrukcnej pasky
    tInstrukcia *OUT;  // vytvorenie instrukcie

    /* takto alokujeme CHAR */

    // mozme priamo... to je toto zakomentovane ...
    //char **retaz = malloc(sizeof(char)*15); // v retaz je ulozena adresa
    //*retaz="nejaky retazec";

    // alebo mozme akokeby nevieme o aky typ sa jedna cize dame VOID *
    // a ked tam nieco ukladame a vieme akeho typu
    // tak pretypujeme napr ako je ukazane hned tu dole :: *(char**) retaz
    void *retaz = malloc(sizeof(char)*15);
    *(char**) retaz ="nejaky retazec";


    printf("\n\n");

    printf("takto sa vypise retazec >>>%s<<<\n\n", *(char**) retaz);

    printf("ADRESA naAlokovaneho miesta >>>%p<<<\n\n", retaz); // ak si chcem nehat printf vypisat adresu treba dat %p - inac hadze warning

    printf("ADRESA PREMENNEJ v ktorej je ulozena adresa malloku  >>>%p<<<\n\n", &retaz);


    // takto sa naplni instrukcia .. cize to :retaz: predstavuje adresu vid hore ako ziskat adresu{v tom printf}
    NaplnInstr(42, (void*) retaz, NULL, NULL); // 42 moze byt konstanta urcitej AKCIE , 3 dalsie su adresy


// vyskusame pridat instrukciu 2*****************************************************************
    void *retaz2 = malloc(sizeof(char)*15);
    *(char**) retaz2 ="DRUHY RETAZEC";
    NaplnInstr(2444, (void*) retaz2, NULL, NULL); // 2444 moze byt konstanta urcitej AKCIE , 3 dalsie su adresy


    InstrStart(&listok); // aktivacia intrukcnej pasky od PRVEJ instrukcie

    /* Takto si ulozime adresu AKTIVNEJ POZICIE V INSTRUKCNEJ PASKE
    ** Vyuzitie napr pri JUMPOCH,.. tu si zapametame aktualnu poziciu
    */
    tPrvokListuPtr PAMAT_POZICIA = InstrDajPoz(&listok); // adresa aktivnej instrukcie


    InstrDalsia(&listok);  // tymto sa posunieme na dalsie instrukciu na PASKE

    InstrGoto(&listok, PAMAT_POZICIA); // takto spravime skok  na "ZAPAMETANU poziciu na paske {JUMP}"



    OUT = DajInstr(&listok); // da ti celu struturu AKTIVNEJ instrukcie

    int AKC = OUT->AKCIA;  // akcia je cislo/konstanta  vypitas si ju takto
    void *adresa;
    adresa = OUT->ADDR_KDE; // k adrese sa pristupi takto / treba si vytvorit pomoc premennu s *


    printf("ACKIA JE >>>%d<<<\n", AKC);


    // samozrejme treba vediet co mam na tej adrese podla typu si vipisem co porebujem {int,char,double,.../ %d,%s,%c,...}
    printf("to co je ulozene na adrese >>>%s<<<\n\n", *(char**) adresa );  // pristup k tomu co je ulozene na tej adrese

    printf("toto je ADRESA >>>%p<<<\n\n", adresa); // toto je ta adresa presne taka ista ktora sa pridelila pri alokacii hore...


    DestroyInstrList(&listok); // zrusime celu pasku



    return 0;
}
