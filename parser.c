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
#include "header.h"

#include "stack.h"
#include "astack.h"
#include "scanner.h"
#include "parser.h"
#include "whattoken.h"
#include "instrlist.h"
#include "ial.h"
#include "prec.h"

list *GLOBFRAME; // globalna tabulka
astack FRAME;

stack S; //zasobnik
tToken actToken; // aktualny token GLOBALNY

list_element Tab_prvok;
int priznak;

void extractRule()
{
	switch(myTop(&S))
	{
		    case LL_INIT:
			{
			    myPop(&S);
			    myPushMul(&S, 6, LL_VLIST, LL_FLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, S_TECKA);
			   
			    // INIT -> VLIST FLIST begin STLIST end.
			}
			break;


			case LL_VLIST:
			if (actToken.stav == S_KLIC_VAR)
			{
			    myPop(&S);
			    myPushMul(&S, 4, S_KLIC_VAR, LL_VDEC, S_STREDNIK, LL_NVLIST);
			    
			    // VLIST -> var VDEC ; NVLIST
			}
			else
			{
			    myPop(&S);
			    // VLIST -> eps prechod
			}
			break;


			case LL_VDEC:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
			    myPop(&S);
			    myPushMul(&S, 3, S_IDENTIFIKATOR, S_DVOJTECKA, LL_TYPE);
			    // VDEC -> id : TYPE
			    NaplnInstr(I_VAR_ZARAZKA, NULL, NULL, NULL);

			    // VDEC -> if : TYPE
			   
          
     //     sem_context->act_id = actToken.data;  //ulozenie id premennej              
	            void *spracADDR = spracuj(actToken.stav, actToken.data);
				if (spracADDR == NULL )
				{
					// chybiska
					printf("CHYBISKA.....\n");
					return 1; // tu nejaky ERR KOD
				}

				printf("GREEP generuji instrukci VAR ID... I_IDENT >>"); whattoken(actToken.stav);
				NaplnInstr( I_IDENT, NULL, spracADDR, NULL );


         // sem_context->act_id = actToken.data;
			    
			}
			break;


			case LL_NVLIST:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
			    myPop(&S);
			    myPushMul(&S, 3, LL_VDEC, S_STREDNIK, LL_NVLIST);
			  
			    //  NVLIST -> VDEC ; NVLIST
          
        //  sem_check (sem_context);  //volam analyzu novo deklarovanej premennej
			}
			else // eps prechod
			{
			    myPop(&S); //  NVLIST -> eps 

        //  sem_check (sem_context);   //volam analyzu novo deklarovanej premennej
			}
			break;



			case LL_TYPE:
				{
					
				switch(actToken.stav) //pri deklaracii
                   {
					case S_KLIC_INTEGER:
					{
					myPop(&S);
					myPush(&S, S_KLIC_INTEGER);

                    NaplnInstr(I_ALLOC_INT, NULL, NULL, NULL);

	  	//		sem_context->act_type = S_INTEGER;
					}
					break;

					case S_KLIC_REAL:
					{

					    myPop(&S);
					    myPush(&S, S_KLIC_REAL);

					NaplnInstr(I_ALLOC_DOU, NULL, NULL, NULL);
					    
            //  sem_context->act_type = S_DOUBLE;
					}
					break;

					case S_KLIC_STRING:
					{
					    myPop(&S);
					    myPush(&S, S_KLIC_STRING);

					    NaplnInstr(I_ALLOC_STR, NULL, NULL, NULL);


				//	    sem_context->act_type = S_RETEZEC;
					break;

					case S_KLIC_BOOLEAN:
					{
					    myPop(&S);
					    myPush(&S, S_KLIC_BOOLEAN);

					    NaplnInstr(I_ALLOC_BOO, NULL, NULL, NULL);


				//	    sem_context->act_type = actToken.stav;
					}
					break;

					default:
					{
					   // doplnime casom ... CHYBISKA exit(2)  CAKAL SOM TYPE...
					}
					break;
				}
				break;
			  }   
                

			case LL_FLIST:
			if (actToken.stav == S_KLIC_FUNCTION)
			{
			    myPop(&S);
			    myPushMul(&S, 9, S_KLIC_FUNCTION, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_PLIST, S_PRAVA_ZAVORKA, S_DVOJTECKA, LL_TYPE, S_STREDNIK, LL_FUNC);

        //  sem_context->context = function_dec;  //prepnutie kontextu na deklaraciu funkcie
         // Id_sign = rem_id;                     //pri spracovani sa id ulozi

			    //  FLIST -> function id ( PLIST ) : TYPE ; FUNC
			}
			else
			{
			    myPop(&S); //  FLIST -> eps prechod    
			}
			break;


			case LL_FUNC:
			if (actToken.stav == S_KLIC_FORWARD)
			{
			    myPop(&S);
			    myPushMul(&S, 3, S_KLIC_FORWARD, S_STREDNIK, LL_FLIST);
			    // FUNC -> forward ; FLIST
			}
			else
			{
			    myPop(&S);
			    myPushMul(&S, 6, LL_VLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, S_STREDNIK, LL_FLIST );
			    // FUNC -> VLIST begin STLIST end ; FLIST
			}
			break;


			case LL_PLIST:
			if (actToken.stav == S_IDENTIFIKATOR)
			{
			    myPop(&S);
			    myPushMul(&S, 4, S_IDENTIFIKATOR,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);
			    // PLIST -> id : TYPE NPLIST
			}
			else
			{
			    myPop(&S); // PLIST -> eps prechod
			}
			break;


			case LL_NPLIST:
			if (actToken.stav == S_STREDNIK)
			{
			    myPop(&S);
			    myPushMul(&S, 5, S_STREDNIK, S_IDENTIFIKATOR,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);
			    // NPLIST -> ; id : TYPE NPLIST
			}
			else
			{
			    myPop(&S); // NPLIST -> eps prechod
			}
			break;

		 
			case  LL_STLIST:
				if ((actToken.stav == S_IDENTIFIKATOR) || (actToken.stav == S_KLIC_WHILE) || (actToken.stav == S_KLIC_IF) || (actToken.stav == S_KLIC_READLN) || (actToken.stav == S_KLIC_WRITE))
				{
					myPop(&S);
					myPushMul(&S, 2, LL_STAT, LL_NSTLIST);
				}
				else 
				{
					myPop(&S);
				}
				break;

//-------------------------------NSTLIST------------------------------------------------------
			case  LL_NSTLIST:
			  if (actToken.stav == S_STREDNIK)
			  {
			  	myPop(&S);
			  	myPushMul(&S, 3, S_STREDNIK, LL_STAT, LL_NSTLIST);
			  }
			  else 
			  {
			  	myPop(&S); // eps prechod
			  }

			  break;


//-------------------------------STAT------------------------------------------------------
		case  LL_STAT:
			  {
			  	switch(actToken.stav)
			      {
				     case S_IDENTIFIKATOR:
						{
					     	myPop(&S);
				     		myPushMul(&S, 3, S_IDENTIFIKATOR, S_PRIRAZENI, LL_RHS);


				     		void *spracADDR = spracuj(actToken.stav, actToken.data);
							if (spracADDR == NULL )
							{
								// chybiska
								printf("CHYBISKA.....\n");
								return 1; // tu nejaky ERR KOD
							}
							printf("GREEP generuji instrukci vloz LL_STAT cize v KODE.. I_IDENT >>"); whattoken(actToken.stav);
							NaplnInstr( I_IDENT, NULL, spracADDR, NULL );

					     	break;
				     	}


				     case S_KLIC_WHILE:
				     	{
				     		myPop(&S);      // mozna zmena LL_E //
				     		myPushMul(&S, 4, S_KLIC_WHILE, LL_E, S_KLIC_DO, LL_BSTAT);
				     		
				     		// tuto sa musi hned vygenerovat instrukcia skoku // 

				     	break;
				     	}

				     case S_KLIC_IF:

				     		myPop(&S);      // mozna zmena LL_E //
				     		myPushMul(&S, 6, S_KLIC_IF, LL_E, S_KLIC_THEN, LL_BSTAT, S_KLIC_ELSE, LL_BSTAT);
				     		break;

				     case S_KLIC_READLN:

				     		myPop(&S);
				     		myPushMul(&S, 4, S_KLIC_READLN, S_LEVA_ZAVORKA, S_IDENTIFIKATOR, S_PRAVA_ZAVORKA);
                            priznak=read;

				     		break;

				     case S_KLIC_WRITE:

				     		myPop(&S);
				     		myPushMul(&S, 4, S_KLIC_WRITE, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA);
				     		


				     		/***********nastavim tu priznak aby som vedel ze ked pridem do***********
				     		 ***********SPLISTU mam generovat hned printf a nie nieco z RHS *********
				     		*/ 
				     		priznak=write; 
				     		
				     		break;
			    
			         default:   // co tu ? chyba alebo nie ?
			         {
			         	break;
			         }
			  	}	
			  	break;
			}


		//--------------------------RHS----------------------//


			  //.............................................................POZOOOOOOOOOOOOOOOOOOOOOOOOOOOR...........................................................
		case  LL_RHS:
			 /* if ((actToken.stav == S_IDENTIFIKATOR ))     // && (F_ID == Tab_prvok->tid)  !!!!!!!!!!!!!!!FUNKCIA!!!!!!!!!!!!!!!!!!!!
			  {         // skontrolovat ci je su v podmienke aktualne nazvy// 

			  	myPop(&S);
			  	myPushMul(&S, 4, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA );
			  //.............................................................POZOOOOOOOOOOOOOOOOOOOOOOOOOOOR...........................................................	
			
			  } */
			  //else	
			  if ((actToken.stav == S_IDENTIFIKATOR ) || (actToken.stav == S_INTEGER) || (actToken.stav == S_RETEZEC) || (actToken.stav == S_DOUBLE) || (actToken.stav == S_BOOLEAN) || (actToken.stav == S_LEVA_ZAVORKA)) // opytat sa ci je to ? ????
			  {
			  		//vieme ze nemama FUKNCIU, BUDEME PUSTAT PRECEDENCNU---->isVyraz();

					myPop(&S);

					if ((isVyraz())==0)   // ak podmienka plati vytvorime instrukciu priradenia
		            { 
		                 NaplnInstr( I_PRIRAD, NULL , NULL, NULL ); 
		            }
			  }
			  else 
			  {
			  	// chybova hlaska CHYBISKA....
			  }
			  break;


//----------------------------------BSTAT----------------------------------
		case  LL_BSTAT:
			  {
			  	myPop(&S);
			  	myPushMul(&S, 3, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END );		 
			  }
			  break;



//----------------------------------SPLIST--------------------------------
		case  LL_SPLIST:
			  	 if (actToken.stav == S_IDENTIFIKATOR )     
			      {

			  		myPop(&S);
			  		myPushMul(&S, 2, S_IDENTIFIKATOR, LL_NSPLIST);
			  		if(priznak == write)
               		 {
			     		void *spracADDR = spracuj(actToken.stav, actToken.data);
						if (spracADDR == NULL )
						{
							// chybiska
							printf("CHYBISKA.....\n");
							return 1; // tu nejaky ERR KOD
						}
						NaplnInstr( I_IDENT, NULL, spracADDR, NULL );
						NaplnInstr( I_WRITE_IDE, NULL, NULL, NULL );
                	 }
			      }
		     	  else if (actToken.stav == S_INTEGER)
		     	  {
			  		myPop(&S);
			  		myPushMul(&S, 2, S_INTEGER, LL_NSPLIST);
			  		if(priznak == write)
               		 {               		 	
               		    void *spracADDR = spracuj(actToken.stav, actToken.data);
						if (spracADDR == NULL )
						{
							// chybiska
							printf("CHYBISKA.....\n");
							return 1; // tu nejaky ERR KOD
						}

						tStav *TIPSTAV = malloc(sizeof(tStav));
						*TIPSTAV = actToken.stav;

						NaplnInstr( I_PREC, NULL, spracADDR, TIPSTAV );
						NaplnInstr( I_WRITE_INT, NULL, NULL, NULL );						
                	 }
		     	  }
		     	  else if (actToken.stav == S_DOUBLE)
		     	  {
		     	  	if(priznak == write)
               		 {
               		    void *spracADDR = spracuj(actToken.stav, actToken.data);
						if (spracADDR == NULL )
						{
							// chybiska
							printf("CHYBISKA.....\n");
							return 1; // tu nejaky ERR KOD
						}

						tStav *TIPSTAV = malloc(sizeof(tStav));
						*TIPSTAV = actToken.stav;
						
						NaplnInstr( I_PREC, NULL, spracADDR, TIPSTAV );
						NaplnInstr( I_WRITE_DOU, NULL, NULL, NULL );
                	 }

			  		myPop(&S);
			  		myPushMul(&S, 2, S_KLIC_REAL, LL_NSPLIST);
		     	  }
		     	  else if (actToken.stav == S_RETEZEC)
		     	  {
		     	    if(priznak == write)
               		 {
                  	   void *spracADDR = spracuj(actToken.stav, actToken.data);
							if (spracADDR == NULL )
							{
								// chybiska
								printf("CHYBISKA.....\n");
								return 1; // tu nejaky ERR KOD
							}

							tStav *TIPSTAV = malloc(sizeof(tStav));
							*TIPSTAV = actToken.stav;

               		 	NaplnInstr( I_PREC, NULL, spracADDR, TIPSTAV );
                  	  	NaplnInstr( I_WRITE_STR, NULL , NULL, NULL );                	  	
                	 }

			  		myPop(&S);
			  		myPushMul(&S, 2, S_RETEZEC, LL_NSPLIST);

		     	  }
		     	  else if (actToken.stav == S_KLIC_TRUE) //////////////////////////////////////////// CO ROBIT ????? BOL/TRU
		     	  {

			  		myPop(&S);
			  		myPushMul(&S, 2, S_KLIC_TRUE, LL_NSPLIST);

		     	  }
		     	  else if (actToken.stav == S_KLIC_FALSE)
		     	  {

			  		myPop(&S);
			  		myPushMul(&S, 2, S_KLIC_FALSE, LL_NSPLIST);

		     	  }
			  else
			  {
			  	myPop(&S); // ->eps prechod    // SKONTROLOVAT V PRAVIDLACH !!!!!
			  	priznak = nic;
			  }
			  break;   


//----------------------------------NSPLIST------------------------------------------
		case  LL_NSPLIST:
		{
			if (actToken.stav == S_CARKA )     
			{
			  	myPop(&S);
				myPushMul(&S, 2, S_CARKA, LL_SPLIST);
			}
			 
			else
			{
			 	myPop(&S);
	            priznak=nic; // ->eps prechod
	            
			}
			break;
        }
			   
	    case LL_E:
	   		{
	   		 	myPop(&S);
	   		 
	   			if ((isVyraz())==0)   // ak podmienka plati vytvorime instrukciu priradenia
	   		        {  
	   		        	NaplnInstr( I_PRIRAD, NULL , NULL, NULL );
	   		        }

	   		break;
	   		}

        default:
        {
        	break; // NEJAKA CHIBISKA....
        }
	}
}


bool parse()
{
    bool ERRO = true; 
	stack_init(&S);
	myPush(&S, EOF); // zarazka $$$$
	myPush(&S, LL_INIT);
	
	actToken = get_token();

//  tId_sign Id_sign;   //priznak zapamatania aktualneho id

 // tSem_context sem_context;
 // sem_context.context = g_var_dec;     //na zaciatku zdrojaku je kontext deklaracii glob. premennych


	while(actToken.stav != S_END_OF_FILE) // dokym som neni na konci suboru
	{
		
		if (myTop(&S) == EOF && actToken.stav != S_END_OF_FILE)
		{
			//CHYBA!!!! na zasobniku bol uz iba EOF ale my sme este nedocitali subor
			printf("CHyBA vyprazdneny zasobnik a este sme neni na konci suboru\n"); 
		}
		

	    if (myTop(&S) >= LL_INIT && myTop(&S) <= LL_NSPLIST )  // TERMINAL/NETERMINAL
		{
			// NETERMINAL  velke mismenka
			//printf("PUSTAM EXTRACT\n");
			extractRule(); // rozvin pravidla...
			//printf("HOTOVO EXTRACT\n");
		}
		else
		{
			// TERMINAL male pismenka
			if (((unsigned int) myTop(&S)) == actToken.stav)
			{
				showStack(&S);
				printf("Pustam TERMINAL  actToken je "); whattoken(actToken.stav);
				printf("PUSTAM TERMINAL  a mam na TOPE a zmazem ho "); whattoken(myTop(&S));

//-------------------------------GENER-READ---------------------------------------------------------------//				
				if ((actToken.stav==S_IDENTIFIKATOR)&&(priznak==read)) // ak ideme citat a pride nam CO ideme citat
				{

					priznak=nacitaj; // nastavi sa priznak pretoze nemozme generovat instrukciu bez pravej zatvorky
				}

				if ((actToken.stav==S_PRAVA_ZAVORKA)&&(priznak==nacitaj)) // prisla prava zatvorka mozme konecne nacitat
				{

					void *spracADDR = spracuj(actToken.stav, actToken.data);
						if (spracADDR == NULL )
						{
							// chybiska
							printf("CHYBISKA.....\n");
							return 1; // tu nejaky ERR KOD
						}
						NaplnInstr( I_READ, NULL, spracADDR, NULL );
						priznak=nic;

				}
//--------------------------------------------------------------------------------------------------------//


				myPop(&S);	// odstranime z vrcholu zasobnika
				//free(actToken.data); // free

        //if (actToken.stav == S_IDENTIFIKATOR) {   //ulozenie id funkcie pri deklaracii
	     //   if (Id_sign == rem_id) {
         //   Id_sign = for_id;                      //reset signum 
         //   sem_context->act_fun = actToken.data;   //save actual id of function
         //   check_sem (sem_context);
         // }
       // }

			actToken = get_token(); // nacitame novy token
				printf("KOEC TERMINAL GET TOKEN token je "); whattoken(actToken.stav);
				printf("KOEC TERMINAL GET TOKEN TOP  je "); whattoken(myTop(&S)) ;
			}
			else
			{
				ERRO = false;
				printf("mas to zle ja som cakal  >> "); whattoken(myTop(&S));
				printf("mas to zle NAPISAL SI    >> "); whattoken( actToken.stav);
				printf("mas to zle riadok.sltpec >> %d .. %d\n", actToken.radek + 1, actToken.sloupec);
				myPop(&S);	// odstranime z vrcholu zasobnika
				//free(actToken.data); // free
				actToken = get_token(); // nacitame novy token
			}
		}

		if (myTop(&S) != EOF && actToken.stav == S_END_OF_FILE)
		{
			ERRO = false;
			printf("ChyBA na zasobniku nieco zostalo a my sme na konci suboru\n"); 
		}
	}
    if(ERRO)
		return true;
	else
		return false;
	//free(actToken.data); // free
	
}
/*
void sem_check (tSem_context* sem_context) 
{
  switch (sem_context->context)     
    
    case G_VAR_DEC:              //kontext deklaracii glob. premennych

      if ( hash_search (GLOBFRAME, sem_context->act_id) == CONTAINS ) { //error if var exists
        sem_context->err = semanticka_chyba_pri_deklaraci;
        return;
      }

      hash_insert_it (GLOBFRAME,sem_context->act_id, sem_context->act_type );  //save var to GTS
      break;
  */
/*
    case FUNCTION_DEC: {
    
      if ( hash_search (GLOBFRAME, sem_context->act_fun) == CONTAINS &&
           check_forward (sem_context->act_fun) != DEFINED )
        {
          
        }
      break;*/
//}
