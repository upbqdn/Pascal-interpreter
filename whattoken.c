/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulán    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

/*
 *Nainkludovanie hackoveho suboru
 */
#include "header.h"
#include "whattoken.h"

 
void whattoken(tStav stav)
{
	switch (stav)
	{

//------------------------------------- KLUCOVE SLOVA - DOLE ---------------------------------------//
  		case S_KLIC_BEGIN:
        	fprintf(stderr, "< keyword: BEGIN >");
        	break;
        case S_KLIC_BOOLEAN:
        	fprintf(stderr, "< keyword: BOOLEAN >");
        	break;
         case S_KLIC_DO:
            fprintf(stderr, "< keyword: DO >");
        	break;
         case S_KLIC_ELSE:
        	fprintf(stderr, "< keyword: ELSE >");
        	break;
         case S_KLIC_END:
        	fprintf(stderr, "< keyword: END >");
        	break;
         case S_KLIC_FALSE:
        	fprintf(stderr, "< keyword: FLASE >");
        	break;
         case S_KLIC_FORWARD:
        	fprintf(stderr, "< keyword: FORWARD >");
        	break;
         case S_KLIC_FUNCTION:
        	fprintf(stderr, "< keyword: FUNCTION >");
        	break;
         case S_KLIC_IF:
        	fprintf(stderr, "< keyword: IF >");
        	break;
         case S_KLIC_INTEGER:
        	fprintf(stderr, "< keyword: INTEGER >");
        	break;
         case S_KLIC_READLN:
        	fprintf(stderr, "< keyword: READLN >");
        	break;
         case S_KLIC_REAL:
        	fprintf(stderr, "< keyword: REAL >");
        	break;
         case S_KLIC_STRING:
        	fprintf(stderr, "< keyword: STRING >");
        	break;
         case S_KLIC_THEN:
        	fprintf(stderr, "< keyword: THEN >");
        	break;
         case S_KLIC_TRUE:
        	fprintf(stderr, "< keyword: TRUE >");
        	break;
         case S_KLIC_VAR:
        	fprintf(stderr, "< keyword: VAR >");
        	break;
         case S_KLIC_WHILE:
        	fprintf(stderr, "< keyword: WHILE >");
        	break;
        case S_KLIC_WRITE:
        	fprintf(stderr, "< keyword: WRITE >");
        	break;

//------------------------------------- KLUCOVE SLOVA - HORE ---------------------------------------//
        	
        
        case S_INTEGER:
        	fprintf(stderr, "< type: INTEGER >");
        	break;
        case S_DOUBLE:
        	fprintf(stderr, "< type: DOUBLE >");
        	break;
        case S_IDENTIFIKATOR:
        	fprintf(stderr, "< type: IDENTIFIKATOR >");
            break;
        case S_RETEZEC:
            fprintf(stderr, "< type: STRING >");
        	break;
        case S_BOOLEAN:
            fprintf(stderr, "< type: BOOLEAN >");
            break;
        case S_PLUS:
        	fprintf(stderr, "< key: + >");
        	break;
        case S_MINUS:
        	fprintf(stderr, "< key:- >");
        	break;
        case S_KRAT:
        	fprintf(stderr, "< key: * >");
        	break;
        case S_DELENO:
        	fprintf(stderr, "< key: / >");
        	break;
        case S_MENSI:
        	fprintf(stderr, "< key: < >");
        	break;
        case S_MENSI_NEBO_ROVNO:
        	fprintf(stderr, "< key: <= >");
        	break;
        case S_VETSI:
        	fprintf(stderr, "< key: > >");
        	break;
        case S_VETSI_NEBO_ROVNO:
        	fprintf(stderr, "< key: >= >");
        	break;
        case S_ROVNO:
        	fprintf(stderr, "< key: = >");
        	break;
        case S_NEROVNO:
        	fprintf(stderr, "< key: <> >");
        	break;
        case S_STREDNIK:
        	fprintf(stderr, "< key: ; >");
        	break;
        case S_DVOJTECKA:
        	fprintf(stderr, "< key: : >");
        	break;
        case S_PRIRAZENI:
        	fprintf(stderr, "< key: := >");
        	break;
        case S_TECKA:
        	fprintf(stderr, "< key: . >");
        	break;
        case S_CARKA:
        	fprintf(stderr, "< key: , >");
        	break;
        case S_LEVA_ZAVORKA:
        	fprintf(stderr, "< key: ( >");
        	break;
        case S_PRAVA_ZAVORKA:
        	fprintf(stderr, "< key: ) >");
        	break;
        case S_LEVA_SLOZENA_ZAVORKA:
        	fprintf(stderr, "< key: { >");
        	break;
        case S_PRAVA_SLOZENA_ZAVORKA:
        	fprintf(stderr, "< key: } >");
        	break; 
        case S_MRIZKA:
        	fprintf(stderr, "< key: # >");
        	break;
        case S_ESCAPE_SEKVENCE:
        	fprintf(stderr, "< ESCAPE SEKVENCIU >");
        	break;
        case S_END_OF_FILE:
        	fprintf(stderr, "< EOF >");
        	break;

//------------------------------------- LL GRAMATIKA - DOLE ------------------------------------------------------//

        case LL_INIT:
        	fprintf(stderr, "< LL-gram: INIT >");
        	break;
        case LL_VLIST:
        	fprintf(stderr, "< LL-gram: VLIST (var list) >");
        	break;
        case LL_VDEC:
        	fprintf(stderr, "< LL-gram: VDEC >");
        	break;
        case LL_NVLIST:
        	fprintf(stderr, "< LL-gram: NVLIST >");
        	break;
        case LL_TYPE:
        	fprintf(stderr, "< LL-gram: TYPE (int,double,str,bool) >");
        	break;
        case LL_FLIST:
        	fprintf(stderr, "< LL-gram: FLIST >");
        	break;
        case LL_FUNC:
        	fprintf(stderr, "< LL-gram: FUNC >");
        	break;
        case LL_PLIST:
        	fprintf(stderr, "< LL-gram: PLIST (param list) >");
        	break;
        case LL_NPLIST:
        	fprintf(stderr, "< LL-gram: NPLIST >");
        	break;
        case LL_STLIST:
        	fprintf(stderr, "< LL-gram: STLIST >");
        	break;
        case LL_NSTLIST:
        	fprintf(stderr, "< LL-gram: NSTLIST >");
        	break;
        case LL_STAT:
        	fprintf(stderr, "< LL-gram: STAT (statement) >");
        	break;
        case LL_RHS:
        	fprintf(stderr, "< LL-gram: RHS >");
        	break;
        case LL_BSTAT:
        	fprintf(stderr, "< LL-gram: BSTAT >");
        	break;
        case LL_SPLIST:
        	fprintf(stderr, "< LL-gram: SPLIST >");
        	break;
        case LL_NSPLIST:
        	fprintf(stderr, "< LL-gram: NSPLIST >");
        	break;
        case LL_E:
        	fprintf(stderr, "< LL-gram: E (exp) >");
        	break;
//------------------------------------- LL GRAMATIKA - HORE ------------------------------------------------------//

        case S_CHYBA:
            fprintf(stderr, "< CHYBA >");
            break;


     default:
        	fprintf(stderr, "< ERR:NOT DEFINED, chybu hladaj u seba >");
        	break;
	}
}
