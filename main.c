/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

/* hlavickove soubory */

/* hlavickove subory */
#include "header.h"
#include "stack.h"
#include "astack.h"
#include "ial.h"
#include "instrlist.h"
#include "htable.h"
#include "scanner.h"
#include "parser.h"
#include "whattoken.h"
#include "prec.h"
#include "interpreter.h"

extern FILE *soubor;
tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA
list GLOBFRAME; // globalna tabulka
astack FRAME;

int main()
{
	soubor = fopen("subor.ifj", "r");

  GLOBFRAME = hash_init();
  InitInstrList(&INSTR_PASKA);
  astack_init(&FRAME);
  myaPUSH(&FRAME, GLOBFRAME);

  		bool parsEXT;
  		parsEXT = parse();  //t pustime parser+semant+precend

      printf("GREAT WORK %d\n", parsEXT);

      if (parsEXT)
      {
        inter();
      }
      else
        printf("CHYBICKA PARSERU \n");

  	  free(actToken.data);

      fclose(soubor);

	return 0;
}