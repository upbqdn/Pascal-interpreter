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

int main()
{
	soubor = fopen("subor.ifj", "r");

  		bool parsEXT;
  		parsEXT = parse();

      printf("GREAT WORK %d\n", parsEXT);

  	  free(actToken.data);

      fclose(soubor);

	return 0;
}