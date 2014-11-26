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
#include "ial.c"          
#include "header.h"
#include "interpreter.h"

list list_array[HASH_ARRAY_SIZE];

int inter(tabulku symbolov , instrukcie) // doplnit predavanie         //AKCIA, KDE,int *PRVA,int *DRUHA//
{
 hash_init();
	while(1)
	{
		Instr=getinstrukciu()
      
      	switch(Instr.AKCIA) // doplnit
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

            case I_PLUS:
               {
                  if (Instr.KDE==NULL)  //------ ak nemame urcene kam---------------------------
	                   {
	                      if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                        ???  =Instr.PRVA+Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                   else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	??? =Instr.PRVA+ ??? ;

	                     }  

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	??? = ??? + Instr.DRUHA;

	                     }   

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	??? = ??? + ??? ;          // otocit treba

	                     }    
	                    
	                   }

                 else   // ---ak mame urcene kam--------------------------------------------//
	                   {
	                    if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                    Instr.KDE=Instr.PRVA+Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                    else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	Instr.KDE=Instr.PRVA+ ??? ;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	Instr.KDE=??? + Instr.DRUHA;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	Instr.KDE= ??? + ??? ;         

	                     }  

	                   }
               } 

             
                                    
            break;
            
            case I_MINUS:
               {
                  if (Instr.KDE==NULL)  //------ ak nemame urcene kam---------------------------
	                   {
	                      if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                        ???  =Instr.PRVA-Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                   else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	??? =Instr.PRVA- ??? ;

	                     }  

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	??? = ??? - Instr.DRUHA;

	                     }   

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	??? = ??? - ??? ;          // otocit treba

	                     }    
	                    
	                   }

                 else   // ---ak mame urcene kam--------------------------------------------//
	                   {
	                    if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                    Instr.KDE=Instr.PRVA-Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                    else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	Instr.KDE=Instr.PRVA- ??? ;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	Instr.KDE=??? - Instr.DRUHA;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	Instr.KDE= ??? - ??? ;          // otocit treba

	                     }  

	                   }
               }                     
            break;

            case I_KRAT:
               {
                  if (Instr.KDE==NULL)  //------ ak nemame urcene kam---------------------------
	                   {
	                      if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                        ???  =Instr.PRVA*Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                   else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	??? =Instr.PRVA* ??? ;

	                     }  

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	??? = ??? * Instr.DRUHA;

	                     }   

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	??? = ??? * ??? ;          // otocit treba

	                     }    
	                    
	                   }

                 else   // ---ak mame urcene kam--------------------------------------------//
	                   {
	                    if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                    Instr.KDE=Instr.PRVA*Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                    else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	Instr.KDE=Instr.PRVA* ??? ;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	Instr.KDE=??? * Instr.DRUHA;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	Instr.KDE= ??? * ??? ;         

	                     }  

	                   }
               } 
             
                                    
            break;

            case I_DELENO:
               {
                  if (Instr.KDE==NULL)  //------ ak nemame urcene kam---------------------------
	                   {
	                      if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                        ???  =Instr.PRVA/Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                   else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	??? =Instr.PRVA/ ??? ;

	                     }  

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	??? = ??? / Instr.DRUHA;

	                     }   

	                   else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	??? = ??? / ??? ;          // otocit treba

	                     }    
	                    
	                   }

                 else   // ---ak mame urcene kam--------------------------------------------//
	                   {
	                    if((Instr.PRVA!=NULL)&&(Instr.DRUHA!=NULL))   // ak su obe zadane
	                    {	
	                    Instr.KDE=Instr.PRVA/Instr.DRUHA;
	                      // treba doplnit ulozenie

	                    } 

	                    else if (((Instr.PRVA!=NULL)&&(Instr.DRUHA==NULL))) // ak je prva zadana
	                     {

	                     	Instr.KDE=Instr.PRVA/ ??? ;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA!=NULL))) // ak je druha zadana
	                     {

	                     	Instr.KDE=??? / Instr.DRUHA;

	                     } 

	                    else if (((Instr.PRVA==NULL)&&(Instr.DRUHA==NULL))) // ak nie su zadane
	                     {

	                     	Instr.KDE= ??? / ??? ;          // otocit treba

	                     }  

	                   }
               } 
                                    
            break;
            
            case I_SKOK:
             
                                    
            break;



        
        
        	default:  
                  // tuto bude asi chybicka nejaka 
        	break  


      	}


	}
}
