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
#include "garbage.h"

FILE *soubor;
tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA
list *GLOBFRAME; // globalna tabulka
astack FRAME;

list *MASTERTAB;

void signals(int sighandler) {
    (void)sighandler;
    trashDestroy(9);
}

int main(int argc, char *argv[])
{
	signal(SIGTERM, signals);
	signal(SIGINT, signals);
	trashInit();
  if(argc != 2)
  {
    //fprintf(stderr, "");
    exit(99);
  }
  if((soubor = fopen(argv[1], "r")) == NULL)
  {
    //fprintf(stderr, "");
    exit(99);
  }

  GLOBFRAME = hash_init();
  MASTERTAB = hash_init();

/*  uz nie je treba
  hash_insert_it(GLOBFRAME, "a", S_INTEGER);
  hash_insert_it(GLOBFRAME, "b", S_INTEGER);
  hash_insert_it(GLOBFRAME, "c", S_RETEZEC);
*/


  InitInstrList(&INSTR_PASKA);
  astack_init(&FRAME);
  myaPush(&FRAME, GLOBFRAME);

      parse();  //t pustime parser+semant+precend
	inter();

      myfree(actToken.data);

      hash_destroy(GLOBFRAME);
      hash_destroy(MASTERTAB);
      destroyaStack(&FRAME);
      DestroyInstrList(&INSTR_PASKA);

	trashDestroy(0);
  return 0;
}
