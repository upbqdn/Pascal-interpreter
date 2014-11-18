#include <stdio.h>
#include "ial.h"
#include "stack.h"
//#include "scaner.h"

FILE *f;

stack S; //zasobnik

int main(int argc, char **argv)
{
	if(argc == 0)
	{
		fprintf(stderr, "Prilis malo arguentov!\n");
	}
	else if(argc == 1)
	{
		if((f = fopen(argv[1], "r")) == NULL)
		{
			fprintf(stderr, "Subor sa nepodarilo otvorit!\n");
		}
	}
	else
	{
		fprintf(stderr, "Prilis vela argumentov!\n");
	}
	return 0;
	myPush(&S, 5);
	int a = myTop(&S);
	printf("%d\n", a);
}