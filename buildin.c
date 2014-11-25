/*
 *Autors: Marek Bielik  	bieli05@stud.fit.vutbr.cz
 	  Filip Gulan 	xgulan00@stud.fit.vutbr.cz
          Filip Ježovica xjezov01@stud.fit.vutbr.cz
          Luboš Matouška xmatus29@stud.fit.vutbr.cz
          Eduard Rybár 	xrybar04@stud.fit.vutbr.cz
         
*/

/*
 *Kniznica obsahujuca funkcie na pracu so zasobnikom.
 */

/*
 *Nainkludovanie hackoveho suboru
 */
#include "buildin.h"

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

/*
 *Funkcia lenght na zistenie dlzky retazca
 *Parameter: retazec ktoreho dlzku chceme pocitat
 */
int lenght(char *str)
{
	int i = 0;
	char c = str[i];
	while(c != '\0')
	{
		i++;
		c = str[i];
	}
	return i;
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
