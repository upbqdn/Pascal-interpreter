/*
 *Autors: Marek Bielik    bieli05@stud.fit.vutbr.cz
 	  Filip Gulan 	  xgulan00@stud.fit.vutbr.cz
          Filip Ježovica  xjezov01@stud.fit.vutbr.cz
          Luboš Matouška  xmatus29@stud.fit.vutbr.cz
          Eduard Rybár 	  xrybar04@stud.fit.vutbr.cz
         
*/




#include <stdio.h>
#include <stdlib.h>
#include "ial.h"
#include "header.h"


int inter() // doplnit predavanie
{
	while(1)
	{
      
      	switch() // doplnit
      	{
      		case I_STOP:
             return; 
            break;

            case I_READ:
             scanf( );  // doplnit 
            break;

        	case I_WRITE:
             printf("%s\n", );   // doplnit

        	break;

        	case I_SORT:
             
                   sort()                  // doplnit sortovanie parametre

            break;

            case I_FIND:
             
                  find()                  // doplnit find parametre

            break;

            case I_LENGTH:
             
                                    // doplnit length
            break;

            case I_COPY:
             
                    podretazec=malloc(sizeof (char)*dlzka);            // zatial len na konkretny pripad
                    copy(retazec, i, n, *podretazec);

            break;

        
        
        	default:  
                  // tuto bude asi chybicka nejaka 
        	break  


      	}


	}
}
