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
astack aS;
astack_init(&aS);


// ----------------alokacia pomocnych premennych roznych TIPOV------------//
int *c_integer = malloc(sizeof(int));
int *c_double = malloc(sizeof(double));
int *c_boolean = malloc(sizeof(boolean));



int inter(tabulku symbolov , instrukcie) // doplnit predavanie         //AKCIA, KDE,int *PRVA,int *DRUHA//
{
 hash_init();
 tStav TIP;


	while(1)
	{
		Instr=getinstrukciu();
      
      	switch(Instr.AKCIA) 
      	{

      		//============ak pride int,double,boolean,string...===============//
      		  case I_PREC:
               myaPUSH(&aS, Instr.ADDR_PRVA);
               TIP=Inst.ADDR_DRUHA; // na tejto adrese musi byt napr. S_INTEGER
              
            break;
           

            //============ak pride IDENTIFIKATOR===============//
            case I_PREC_ID:


                Llist TOPFRAME = myTop(&FRAME);    // fiko magic // 
        			
        			  Llist_element* prvok = Lhash_adress(TOPFRAME, Instr.ADDR_PRVA);
                if (prvok == NULL) // hladame v GLOBAL
        				{
        				prvok = Lhash_adress(GLOBFRAME, stoken->data);
        				}


        				TIP=prvok.type;
        				myaPUSH(&aS, prvok.data);
                      
            break;

            case I_PRIRAD:
               if (TIP==S_INTEGER)
               {	
               // namiesto ??? sa musi vyriesit to aby sa dalo z KEY pristupit priamo na HODNOTU
               ???= (*(int*)(myaTop(&aS)));  
               myaPop(&aS);
               }

               else if (TIP==S_DOUBLE)
               {	
               ???= (*(float*)(myaTop(&aS)));  // float ?
               myaPop(&aS);
               }

            break;



//>>>>>>>>>>>>>>>>>>>>>>>>>>--WRITE pripady--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<// 
            case I_WRITE_INT:

            break;

            case I_WRITE_DOU:

            break;

            case I_WRITE_STR:

            break; 

            case I_WRITE_BOO:

            break;


  //>>>>>>>>>>>>>>>>>>>>>>--nasleduje +,-,*,/--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<// 
            case I_PLUS:
               if (TIP==S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 c_integer = b + a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                 myaPUSH(&aS, c_integer);
                 ;

               }

               else if (TIP==S_DOUBLE)
               {
               	 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 c_double = b + a;   
                 myaPUSH(&aS, c_double);
               	
               }

               else if (TIP==S_BOOLEAN)
               {
               	
               }

               else if (TIP==S_STRING)
               {
               	
               }
        	
        	break;  

        	case I_MINUS:
               if (TIP==S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 c_integer = b - a;   
                 myaPUSH(&aS, c);
                 ;

               }

               else if (TIP==S_DOUBLE)
               {
                 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 c_double = b - a;   
                 myaPUSH(&aS, c_double);
               	
               }

               else if (TIP==S_BOOLEAN)
               {
               	
               }

               else if (TIP==S_STRING)
               {
               	
               }
        	
        	break; 

        	case I_KRAT:
               if (TIP==S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 c_integer = b * a;   
                 myaPUSH(&aS, c);
                 ;

               }

               else if (TIP==S_DOUBLE)
               {
               	 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 c_double = b * a;   
                 myaPUSH(&aS, c_double);
               	
               }

               else if (TIP==S_BOOLEAN)
               {
               	
               }

               else if (TIP==S_STRING)
               {
               	
               }
        	
        	break;   

            case I_DELENO:
               if (TIP==S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 c_integer = b / a;   
                 myaPUSH(&aS, c);
                 ;

               }

               else if (TIP==S_DOUBLE)
               {
               	 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 c_double = b / a;   
                 myaPUSH(&aS, c_double);
               	
               }

               else if (TIP==S_BOOLEAN)
               {
               	
               }

               else if (TIP==S_STRING)
               {
               	
               }
        	
        	break;  

      	}


	}

	// tuto bude dealokacia pomocnych premenych 
}
