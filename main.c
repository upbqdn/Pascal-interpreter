/* hlavickove soubory */

/* hlavickove subory */
#include "header.h"
#include "stack.h"
#include "scanner.h"
#include "parser.h"

extern FILE *soubor;

int main()
{
	soubor = fopen("subor.ifj", "r");

	/*tToken tokenik;

	  for (int i = 0; i < 20; ++i)
 		 {
    			tokenik  = get_token();
    			printf("%s   >>> ", tokenik.data );
     			printf("%d\n", tokenik.stav );
     			printf("errrrrrrrrrrr > %d\n",error );	    		
  		}
	
	*/

  		// test for parserito.c

  		bool parsEXT;
  		parsEXT = parse();

  		printf("%d hovno \n",parsEXT );

	return 0;
}