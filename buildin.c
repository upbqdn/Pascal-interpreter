/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu so zasobnikom.
 */

/*
 *Nainkludovanie hackoveho suboru
 */
#include "stack.h"

/*
 *Premenne definicie
 */
 //zasobnik

 //------------------funkcia COPY----------------------//

void copy(char *s, int i, int n, char *podretazec )
  {

  	 
    for (int j = 0; j < n; j++)
    {   
    
    	podretazec[j]=s[i-1+j];

      //printf("%c\n", podretazec[j]);
    }

  
  }



/*int main()
{
	stack_init(&S);
	myPushMul(&S ,8, 1,2,3,4,5,80,75,87);
	myPush(&S, 505);
	myPush(&S, 7805);
	myPush(&S, 55);
	showStack(&S);
	destroyStack(&S);
 	return 0;
}*/