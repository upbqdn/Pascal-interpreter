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
 *Hlavickovy subor S enumeraciami STAVOV, ERROROV, ....
 */

/* hlavickove soubory */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

/* chyby a chybove kody */
typedef enum ERROR
{
  vsechno_ok = 0,							// 0
  chyba_v_programu_v_ramci_lexikalni_analyzy,			// 1
  chyba_v_programu_v_ramci_syntakticke_analyzy,		// 2
  semanticka_chyba_pri_deklaraci,					// 3
  semanticka_chyba_typove_kompatibility,			// 4
  ostatni_semanticke_chyby,					// 5
  behova_chyba_pri_nacitani_ciselne_hodnoty_ze_vstupu,	// 6
  behova_chyba_pri_praci_s_neinicializovanou_promennou,	// 7
  behova_chyba_deleni_nulou,					// 8
  ostatni_behove_chyby,					// 9
  interni_chyba_interpretu = 99,					// 99
} tERROR;


/* jednotlive stavy automatu + stavy do LL-gramatiky */
typedef enum
{
                            // STAV klucove slova.. MUSI TO BYT NA ZACIATKU ! NEMENIT
    S_KLIC_BEGIN = 0,
    S_KLIC_BOOLEAN,
    S_KLIC_DO,
    S_KLIC_ELSE,
    S_KLIC_END, // 4
    S_KLIC_FALSE, // 5
    S_KLIC_FIND,
    S_KLIC_FORWARD,
    S_KLIC_FUNCTION,
    S_KLIC_IF,
    S_KLIC_INTEGER,  // 10
    S_KLIC_READLN,
    S_KLIC_REAL,
    S_KLIC_SORT,
    S_KLIC_STRING,  // 14
    S_KLIC_THEN,  // 15
    S_KLIC_TRUE,
    S_KLIC_VAR, // 17
    S_KLIC_WHILE,
    S_KLIC_WRITE, // 19
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
    S_IDENTIFIKATOR,				//identifikator  // 29
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
    S_NEROVNO,					// <>  //40
    S_STREDNIK,					// ; //41
    S_DVOJTECKA,				// :
    S_PRIRAZENI,				// := 
    S_TECKA,					// . //44
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
    S_DOUBLE_POMOCDES4,   // 60

    LL_INIT,    // 61
    LL_VLIST,   //62
    LL_VDEC,    //63
    LL_NVLIST,  //64
    LL_TYPE,    //65
    LL_FLIST,   //66
    LL_FUNC,    //67
    LL_PLIST,   //68
    LL_NPLIST,  //69
    LL_STLIST,  //70
    LL_NSTLIST, //71
    LL_STAT,    //72
    LL_RHS,     //73
    LL_BSTAT,   //74
    LL_SPLIST,  //75
    LL_NSPLIST, //76
    LL_E,


    F_ID, // 78
    S_BOOLEAN, /* */
    S_DOLAR, /* pomocny stav pro precedencni analyzu */

    write,
    read,
    nic,

} tStav;

