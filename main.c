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
#include "scanner.h"
#include "parser.h"
#include "whattoken.h"
#include "prec.h"
#include "interpreter.h"

extern FILE *soubor;
tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA
list *GLOBFRAME; // globalna tabulka
astack FRAME;

int main()
{
	soubor = fopen("subor.ifj", "r");

  GLOBFRAME = hash_init();
  
  // hash_insert_it(GLOBFRAME, "a", S_INTEGER);
  // hash_insert_it(GLOBFRAME, "b", S_INTEGER);
  // hash_insert_it(GLOBFRAME, "c", S_RETEZEC);
  
  
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



	//vecicky co treba nakonci spravit
  	  free(actToken.data);
  	  hash_destroy(GLOBFRAME);
  	  destroyaStack(&FRAME);
  	  DestroyInstrList(&INSTR_PASKA);
  	  fclose(soubor);

	return 0;
}
