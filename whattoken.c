/*
-----------------------------------------------------
 *@Author: Marek Bielik		xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan		xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica	xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matouška	xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár		xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------          
*/

/*
 *Nainkludovanie hackoveho suboru
 */
#include "whattoken.h"
#include "header.h"


/*
 *Premenne definicie
 */
 
void whattoken(tStav stav)
{
	switch (stav)
	{
  		case S_KLIC_BEGIN:
        	printf("S_KLIC_BEGIN");
        	printf("\n");
        	break;
        case S_KLIC_BOOLEAN:
        	printf("S_KLIC_BOOLEAN");
        	printf("\n");
        	break;
         case S_KLIC_DO:
        	printf("S_KLIC_DO");
        	printf("\n");
        	break;
         case S_KLIC_ELSE:
        	printf("S_KLIC_ELSE");
        	printf("\n");
        	break;
         case S_KLIC_END:
        	printf("S_KLIC_END");
        	printf("\n");
        	break;
         case S_KLIC_FALSE:
        	printf("S_KLIC_FALSE");
        	printf("\n");
        	break;
         case S_KLIC_FIND:
        	printf("S_KLIC_FIND");
        	printf("\n");
        	break;
         case S_KLIC_FORWARD:
        	printf("S_KLIC_FORWARD");
        	printf("\n");
        	break;
         case S_KLIC_FUNCTION:
        	printf("S_KLIC_FUNCTION");
        	printf("\n");
        	break;
         case S_KLIC_IF:
        	printf("S_KLIC_IF");
        	printf("\n");
        	break;
         case S_KLIC_INTEGER:
        	printf("S_KLIC_INTEGER");
        	printf("\n");
        	break;
         case S_KLIC_READLN:
        	printf("S_KLIC_READLN");
        	printf("\n");
        	break;
         case S_KLIC_REAL:
        	printf("S_KLIC_REAL");
        	printf("\n");
        	break;
         case S_KLIC_SORT:
        	printf("S_KLIC_SORT");
        	printf("\n");
        	break;
         case S_KLIC_STRING:
        	printf("S_KLIC_STRING");
        	printf("\n");
        	break;
         case S_KLIC_THEN:
        	printf("S_KLIC_THENS_KLIC_TRUE");
        	printf("\n");
        	break;
         case S_KLIC_TRUE:
        	printf("S_KLIC_TRUE");
        	printf("\n");
        	break;
         case S_KLIC_VAR:
        	printf("S_KLIC_VAR");
        	printf("\n");
        	break;
         case S_KLIC_WHILE:
        	printf("S_KLIC_WHILE");
        	printf("\n");
        	break;
        case S_KLIC_WRITE:
        	printf("S_KLIC_WRITE");
        	printf("\n");
        	break;
        case S_START:
        	printf("S_START");
        	printf("\n");
        	break;
        case S_END:
        	printf("S_END");
        	printf("\n");
        	break;
        case S_CHYBA:
        	printf("S_CHYBA");
        	printf("\n");
        	break;
        case S_KOMENTAR:
        	printf("S_KOMENTAR");
        	printf("\n");
        	break;
        case S_INTEGER:
        	printf("S_INTEGER");
        	printf("\n");
        	break;
        case S_DOUBLE:
        	printf("S_DOUBLE");
        	printf("\n");
        	break;
        case S_DOUBLE_CELA_CAST_A_EXPONENT:
        	printf("S_DOUBLE_CELA_CAST_A_EXPONENT");
        	printf("\n");
        	break;
        case S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT:
        	printf("S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT");
        	printf("\n");
        	break;
        case S_DOUBLE_POMOC:
        	printf("S_DOUBLE_POMOC");
        	printf("\n");
        	break;
        case S_IDENTIFIKATOR:
        	printf("S_IDENTIFIKATOR");
        	printf("\n");
        	break;
        case S_PLUS:
        	printf("S_PLUSS_MINUS");
        	printf("\n");
        	break;
        case S_MINUS:
        	printf("S_MINUS");
        	printf("\n");
        	break;
        case S_KRAT:
        	printf("S_KRAT");
        	printf("\n");
        	break;
        case S_DELENO:
        	printf("S_DELENO");
        	printf("\n");
        	break;
        case S_UKAZATEL:
        	printf("S_UKAZATEL");
        	printf("\n");
        	break;
        case S_MENSI:
        	printf("S_MENSI");
        	printf("\n");
        	break;
        case S_MENSI_NEBO_ROVNO:
        	printf("S_MENSI_NEBO_ROVNO");
        	printf("\n");
        	break;
        case S_VETSI:
        	printf("S_VETSI");
        	printf("\n");
        	break;
        case S_VETSI_NEBO_ROVNO:
        	printf("S_VETSI_NEBO_ROVNO");
        	printf("\n");
        	break;
        case S_ROVNO:
        	printf("S_ROVNO");
        	printf("\n");
        	break;
        case S_NEROVNO:
        	printf("S_NEROVNO");
        	printf("\n");
        	break;
        case S_STREDNIK:
        	printf("S_STREDNIK");
        	printf("\n");
        	break;
        case S_DVOJTECKA:
        	printf("S_DVOJTECKA");
        	printf("\n");
        	break;
        case S_PRIRAZENI:
        	printf("S_PRIRAZENI");
        	printf("\n");
        	break;
        case S_TECKA:
        	printf("S_TECKA");
        	printf("\n");
        	break;
        case S_DVE_TECKY:
        	printf("S_DVE_TECKY");
        	printf("\n");
        	break;
        case S_CARKA:
        	printf("S_CARKA");
        	printf("\n");
        	break;
        case S_LEVA_ZAVORKA:
        	printf("S_LEVA_ZAVORKA");
        	printf("\n");
        	break;
        case S_PRAVA_ZAVORKA:
        	printf("S_PRAVA_ZAVORKA");
        	printf("\n");
        	break;
        case S_LEVA_HRANATA_ZAVORKA:
        	printf("S_LEVA_HRANATA_ZAVORKA");
        	printf("\n");
        	break;
        case S_PRAVA_HRANATA_ZAVORKA:
        	printf("S_PRAVA_HRANATA_ZAVORKA");
        	printf("\n");
        	break;
        case S_LEVA_SLOZENA_ZAVORKA:
        	printf("S_LEVA_SLOZENA_ZAVORKA");
        	printf("\n");
        	break;
        case S_PRAVA_SLOZENA_ZAVORKA:
        	printf("S_PRAVA_SLOZENA_ZAVORKA");
        	printf("\n");
        	break;
        case S_RETEZEC:
        	printf("S_RETEZEC");
        	printf("\n");
        	break;
        case S_MRIZKA:
        	printf("S_MRIZKA");
        	printf("\n");
        	break;
        case S_ESCAPE_SEKVENCE:
        	printf("S_ESCAPE_SEKVENCE");
        	printf("\n");
        	break;
        case S_END_OF_FILE:
        	printf("S_END_OF_FILE");
        	printf("\n");
        	break;
        case S_DOUBLE_POMOCDES1:
        	printf("S_DOUBLE_POMOCDES1");
        	printf("\n");
        	break;
        case S_DOUBLE_POMOCDES2:
        	printf("S_DOUBLE_POMOCDES2");
        	printf("\n");
        	break;
        case S_DOUBLE_POMOCDES3:
        	printf("S_DOUBLE_POMOCDES3");
        	printf("\n");
        	break;
        case S_DOUBLE_POMOCDES4:
        	printf("S_DOUBLE_POMOCDES4");
        	printf("\n");
        	break;
        case LL_INIT:
        	printf("LL_INIT");
        	printf("\n");
        	break;
        case LL_VLIST:
        	printf("LL_VLIST");
        	printf("\n");
        	break;
        case LL_VDEC:
        	printf("LL_VDEC");
        	printf("\n");
        	break;
        case LL_NVLIST:
        	printf("LL_NVLIST");
        	printf("\n");
        	break;
        case LL_TYPE:
        	printf("LL_TYPE");
        	printf("\n");
        	break;
        case LL_FLIST:
        	printf("LL_FLIST");
        	printf("\n");
        	break;
        case LL_FUNC:
        	printf("LL_FUNC");
        	printf("\n");
        	break;
        case LL_PLIST:
        	printf("LL_PLIST");
        	printf("\n");
        	break;
        case LL_NPLIST:
        	printf("LL_NPLIST");
        	printf("\n");
        	break;
        case LL_STLIST:
        	printf("LL_STLIST");
        	printf("\n");
        	break;
        case LL_NSTLIST:
        	printf("LL_NSTLIST");
        	printf("\n");
        	break;
        case LL_STAT:
        	printf("LL_STAT");
        	printf("\n");
        	break;
        case LL_RHS:
        	printf("LL_RHS");
        	printf("\n");
        	break;
        case LL_BSTAT:
        	printf("LL_BSTAT");
        	printf("\n");
        	break;
        case LL_SPLIST:
        	printf("LL_SPLIST");
        	printf("\n");
        	break;
        case LL_NSPLIST:
        	printf("LL_NSPLIST");
        	printf("\n");
        	break;
        case LL_E:
        	printf("LL_E");
        	printf("\n");
        	break;
 	 default:
        	printf("Nedefinovaneeeeeee!!!!!");
        	printf("\n");
        	break;
	}
}

/*
int main()
{
	for(int i = 0; i < 78; i++)
		{
			whattoken(i);
		}
 	return 0;
} */
