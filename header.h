/*
 *Hlavickovy subor S enumeraciami STAVOV, ERROROV, ....
 */

/* hlavickove soubory */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* chyby a chybove kody */
typedef enum ERROR
{
  vsechno_ok,							// 0
  chyba_v_programu_v_ramci_lexikalni_analyzy,			// 1
  chyba_v_programu_v_ramci_syntakticke_analyzy,		// 2
  semanticka_chyba_v_programu,					// 3
  semanticka_chyba_typove_kompatibility,			// 4
  ostatni_semanticke_chyby,					// 5
  behova_chyba_pri_nacitani_ciselne_hodnoty_ze_vstupu,	// 6
  behova_chyba_pri_praci_s_neinicializovanou_promennou,	// 7
  behova_chyba_deleni_nulou,					// 8
  ostatni_behove_chyby,					// 9
  interni_chyba_interpretu,					// 99
} tERROR;


/* jednotlive stavy automatu + stavy do LL-gramatiky */
typedef enum
{
                            // STAV klucove slova.. MUSI TO BYT NA ZACIATKU ! NEMENIT
    S_KLIC_BEGIN = 0,
    S_KLIC_BOOLEAN,
    S_KLIC_DO,
    S_KLIC_ELSE,
    S_KLIC_END,
    S_KLIC_FALSE,
    S_KLIC_FIND,
    S_KLIC_FORWARD,
    S_KLIC_FUNCTION,
    S_KLIC_IF,
    S_KLIC_INTEGER,
    S_KLIC_READLN,
    S_KLIC_REAL,
    S_KLIC_SORT,
    S_KLIC_STRING,
    S_KLIC_THEN,
    S_KLIC_TRUE,
    S_KLIC_VAR,
    S_KLIC_WHILE,
    S_KLIC_WRITE,
                          //klucove slova potialto ! posledne :: 19 :: 

    S_START,					//pocatecni stav :: 20 :: 
    S_END,					//koncovy stav
    S_CHYBA,					//chybovy stav
    S_KOMENTAR,					//komentar
    S_INTEGER,					//cislo typu integer
    S_DOUBLE,					//cislo typu double
  //  S_DOUBLE_CELA_A_DESETINNA_CAST,		//cislo typu double
    S_DOUBLE_CELA_CAST_A_EXPONENT,		//cislo typu double
    S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT,//cislo typu double
    S_DOUBLE_POMOC,				//pomocny double 
    S_IDENTIFIKATOR,				//identifikator
    S_PLUS,					// +
    S_MINUS,					// -
    S_KRAT,					// *
    S_DELENO,					// / 
    S_UKAZATEL,					// ^
    S_MENSI,					// <
    S_MENSI_NEBO_ROVNO,				// <=
    S_VETSI,					// >
    S_VETSI_NEBO_ROVNO,				// >= 
    S_ROVNO,					// =
    S_NEROVNO,					// <>
    S_STREDNIK,					// ;
    S_DVOJTECKA,				// :
    S_PRIRAZENI,				// := 
    S_TECKA,					// .
    S_DVE_TECKY,				// ..
    S_CARKA,					// ,
    S_LEVA_ZAVORKA,				// (
    S_PRAVA_ZAVORKA,				// ) 
    S_LEVA_HRANATA_ZAVORKA,			// [
    S_PRAVA_HRANATA_ZAVORKA,			// ]
    S_LEVA_SLOZENA_ZAVORKA,			// {
    S_PRAVA_SLOZENA_ZAVORKA,			// }
    S_RETEZEC,					// string 
    S_MRIZKA,					// #
    S_ESCAPE_SEKVENCE,				// #0-255
    S_END_OF_FILE,				// EOF
    S_DOUBLE_POMOCDES1,			
    S_DOUBLE_POMOCDES2,
    S_DOUBLE_POMOCDES3,
    S_DOUBLE_POMOCDES4,

    LL_INIT, // DOPLNAM PRE SYNTAKTICKU ANALIZU NETERMINALY
    LL_STLIST,
    LL_STAT,
    LL_EXP,

} tStav;