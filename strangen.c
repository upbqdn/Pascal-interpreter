/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 */

#include "strangen.h"
#include "garbage.h"

int GlobalI = 1;

char *stringRandGen()
{
	char *pomocna = mymalloc(sizeof(char)* PREFIX);
	strcpy(pomocna, "@#$");

	//char *c = mymalloc(sizeof(char));
	char *c;
	int GlobPom = GlobalI;
	int i = 0;
	char SUFFIX[] = "@$$#";
	do{
		if(GlobPom > ROZP5)
		{
			GlobPom = GlobPom - ROZP5;
			*c = ROZP5 + BEGINASCI;
			strcat(pomocna, c); 
		}
		else
		{

			 pomocna =  myrealloc(pomocna, (sizeof(char)* (PREFIX + i+1)));
			//*c = GlobPom + BEGINASCI;
			c = GlobPom + BEGINASCI;
			strcat(pomocna, &c);
			strcat(pomocna, SUFFIX);
			GlobPom = GlobPom - ROZP5;
		}
		i++;
	}while(GlobPom > 0);
	GlobalI++;
	//printf("AAAA %s\n", pomocna);
	return(pomocna);
}

/*
int main()
{
	for(int i = 0; i < 300; i++)
	{
		//printf("%s\n", stringRandGen());
	}
 	return 0;
}*/
