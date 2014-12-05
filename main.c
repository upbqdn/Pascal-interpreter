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

FILE *soubor;
tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA
list *GLOBFRAME; // globalna tabulka
astack FRAME;




int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    //fprintf(stderr, "");
    return 99;
  }
  if((soubor = fopen(argv[1], "r")) == NULL)
  {
    //fprintf(stderr, "");
    return 99;
  }

  GLOBFRAME = hash_init();

/*  uz nie je treba
  hash_insert_it(GLOBFRAME, "a", S_INTEGER);
  hash_insert_it(GLOBFRAME, "b", S_INTEGER);
  hash_insert_it(GLOBFRAME, "c", S_RETEZEC);
*/


  printf("PREDTYM SOM GLOB SU:::>>>%p<<<<:::\n" , GLOBFRAME);
  InitInstrList(&INSTR_PASKA);
  astack_init(&FRAME);
  myaPush(&FRAME, GLOBFRAME);

      bool parsEXT;
      parsEXT = parse();  //t pustime parser+semant+precend

      printf("GREAT WORK %d\n", parsEXT);

      if (parsEXT)
      {
        inter();
      }
      else
        printf("CHYBICKA PARSERU \n");

      myfree(actToken.data);

      hash_destroy(GLOBFRAME);
      destroyaStack(&FRAME);
      DestroyInstrList(&INSTR_PASKA);

	trashDestroy();
  return 0;
}
