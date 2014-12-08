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

tId_sign Id_sign = for_all;   //priznak zapamatania aktualneho id
int arg_num = 0;         //pocet argumentov funkcie inicializovany na 0
unsigned f_counter = 0;             //pocitadlo forwarded funkcii

void extractRule(tSem_context* s_con)
{
    switch(myTop(&S))
    {
    case LL_INIT:
    {
        myPop(&S);
        myPushMul(&S, 6, LL_VLIST, LL_FLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, S_TECKA);

        // INIT -> VLIST FLIST begin STLIST end.

        break;
    }



    case LL_VLIST:
    {
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
    }


    case LL_VDEC:
    {
        if (actToken.stav == S_IDENTIFIKATOR)
        {
            //VDEC -> id : TYPE
            
            myPop(&S);
            myPushMul(&S, 3, S_IDENTIFIKATOR, S_DVOJTECKA, LL_TYPE);
            

            void *spracADDR = spracuj(actToken.stav, actToken.data);

            //printf("GREEP generuji instrukci VAR ID... I_IDENT >>");
            //whattoken(actToken.stav);
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL );


            s_con->act_id = actToken.data;  //ulozenie id premennej
        }
        break;
    }



    case LL_NVLIST:
    {
        if (actToken.stav == S_IDENTIFIKATOR)
        {
            myPop(&S);
            myPushMul(&S, 3, LL_VDEC, S_STREDNIK, LL_NVLIST);

            //  NVLIST -> VDEC ; NVLIST

             sem_check (s_con);  //volam analyzu novo deklarovanej premennej
        }
        else // eps prechod
        {
            myPop(&S); //  NVLIST -> eps

            sem_check (s_con);   //volam analyzu novo deklarovanej premennej
        }

        break;
    }



    case LL_TYPE:
    {

        switch(actToken.stav) //pri deklaracii
        {
        case S_KLIC_INTEGER:
        {
            myPop(&S);
            myPush(&S, S_KLIC_INTEGER);

            NaplnInstr(I_ALLOC_INT, NULL, NULL, NULL);

        		s_con->act_type = S_INTEGER;
            break;
        }

        case S_KLIC_REAL:
        {

            myPop(&S);
            myPush(&S, S_KLIC_REAL);

            NaplnInstr(I_ALLOC_DOU, NULL, NULL, NULL);

            s_con->act_type = S_DOUBLE;
            break;
        }

        case S_KLIC_STRING:
        {
            myPop(&S);
            myPush(&S, S_KLIC_STRING);

            NaplnInstr(I_ALLOC_STR, NULL, NULL, NULL);

            s_con->act_type = S_RETEZEC;

            break;
        }

        case S_KLIC_BOOLEAN:
        {
            myPop(&S);
            myPush(&S, S_KLIC_BOOLEAN);

            NaplnInstr(I_ALLOC_BOO, NULL, NULL, NULL);

            s_con->act_type = actToken.stav;
            break;
        }


        default:
        {
            // doplnime casom ... CHYBISKA exit(2)  CAKAL SOM TYPE...
            break;
        }
        }
        break;
    }


    case LL_FLIST:
    {
        if (actToken.stav == S_KLIC_FUNCTION)
        {
            myPop(&S);
            myPushMul(&S, 9, S_KLIC_FUNCTION, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_PLIST, S_PRAVA_ZAVORKA, S_DVOJTECKA, LL_TYPE, S_STREDNIK, LL_FUNC);

            s_con->context = FUNCTION_DEC;  //prepnutie kontextu na deklaraciu funkcie
            Id_sign = rem_id;                     //pri spracovani sa id ulozi

            //  FLIST -> function id ( PLIST ) : TYPE ; FUNC
        }
        else
        {
            myPop(&S); //  FLIST -> eps prechod
            
            if (f_counter != 0) {  //semanticka kontrola, ci vsetky funkcie boli definovane
              fprintf (stderr, "v programe su nedefinovane funkcie\n");
              exit (semanticka_chyba_pri_deklaraci);
            }

            s_con->scope = GLOBAL;  // prepnutie na globalny scope
        }
        break;
    }


    case LL_FUNC:
    {
        // s_con->c_fun // instrukcie pre Spustenie funkcie


        if (actToken.stav == S_KLIC_FORWARD)
        {
            myPop(&S);
            myPushMul(&S, 3, S_KLIC_FORWARD, S_STREDNIK, LL_FLIST);
         

            s_con->context = FUNC_TYPE_DEC;  //kontext nastavenia navratoveho typu funkcie
            sem_check (s_con);  //ulozi sa typ funkcie
            
            if (hash_is_sign (GLOBFRAME, s_con->act_fun) != DEFINED) {
              f_counter++;  //zvysuje sa pocet forwardnutych funkcii
              //funkcia sa nastavi na forwarded, ak nebola definovana
              hash_set_sign (GLOBFRAME, s_con->act_fun, FORWARDED);
              }
            else {
              fprintf (stderr, "semanticka chyba pri deklaraci funkcie \'%s\', volam exit(3), dealokuje OS\n", s_con->act_fun);
              exit (semanticka_chyba_pri_deklaraci);
            }

            // FUNC -> forward ; FLIST
        }
        else
        {
            myPop(&S);
            myPushMul(&S, 6, LL_VLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, S_STREDNIK, LL_FLIST );
            
            
            s_con->context = FUNC_TYPE_DEC;  //kontext nastavenia navratoveho typu funkcie
            sem_check (s_con);  //ulozi sa typ funkcie, v pripade definicie sa skontroluje 

            s_con->context = L_VAR_DEC;  //nasleduje kontext deklaracie lok. premennych

            // FUNC -> VLIST begin STLIST end ; FLIST
        }

        break;
    }


    case LL_PLIST:
    {
        if (actToken.stav == S_IDENTIFIKATOR)
        {
            myPop(&S);
            myPushMul(&S, 4, S_IDENTIFIKATOR,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);
            
            s_con->act_id = actToken.data;      //ulozenie id parametra
            s_con->context = FUNC_ARG_DEC;      //kontext deklaracii argumentov funkcie

            // PLIST -> id : TYPE NPLIST

        }
        else
        {
            myPop(&S); // PLIST -> eps prechod
        }

        break;
    }


    case LL_NPLIST:
    {
        if (actToken.stav == S_STREDNIK)
        {
            myPop(&S);
            myPushMul(&S, 5, S_STREDNIK, S_IDENTIFIKATOR,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);
            
           sem_check (s_con);   //kontrola parametra

           Id_sign = rem_pid;  // id dalsieho parametra sa zapamata

            // NPLIST -> ; id : TYPE NPLIST
        }
        else
        {
            myPop(&S); // NPLIST -> eps prechod

            sem_check (s_con);  // kontrola parametra
        }
        break;
    }


    case  LL_STLIST:
    {
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
    }

//-------------------------------NSTLIST------------------------------------------------------
    case  LL_NSTLIST:
    {
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
    }


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

           // printf("GREEP generuji instrukci vloz LL_STAT cize v KODE.. I_IDENT >>");
            //whattoken(actToken.stav);
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL );
            
            s_con->l_id = actToken.data;  //ulozenie id pre LHS

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
        {
            myPop(&S);      // mozna zmena LL_E //
            myPushMul(&S, 6, S_KLIC_IF, LL_E, S_KLIC_THEN, LL_BSTAT, S_KLIC_ELSE, LL_BSTAT);

            break;
        }

        case S_KLIC_READLN:
        {
            myPop(&S);
            myPushMul(&S, 4, S_KLIC_READLN, S_LEVA_ZAVORKA, S_IDENTIFIKATOR, S_PRAVA_ZAVORKA);
            priznak=read;

            break;
        }

        case S_KLIC_WRITE:
        {
            myPop(&S);
            myPushMul(&S, 4, S_KLIC_WRITE, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA);
            /***********nastavim tu priznak aby som vedel ze ked pridem do***********
            ***********SPLISTU mam generovat hned printf a nie nieco z RHS *********
            */
            priznak=write;

            break;
        }

        default:   // co tu ? chyba alebo nie ?
        {
            whattoken(actToken.stav);
            fprintf(stderr, "2: Syntakticka chyba. Za poslednym prikazom nesmie byt strednik !\n");
            exit(2);
            break;
        }
        }
        break;
    }

    //--------------------------RHS----------------------//

    case  LL_RHS:
    {
       if ( actToken.stav == S_IDENTIFIKATOR && 
            hash_return_type (GLOBFRAME, actToken.data) == F_ID)  //semantika overuje typ id
        	{         // skontrolovat ci je su v podmienke aktualne nazvy//

    		  	myPop (&S);
      	  	myPushMul (&S, 4, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA );

            s_con->c_fun = actToken.data;   //ulozi sa id volanej funkcie
            s_con->context = RET_VAL_CHECK;  //typova kontrola priradenia navr. hod. funkcie
            sem_check (s_con);              
        	 }

       else if
         ((actToken.stav == S_IDENTIFIKATOR ) || (actToken.stav == S_INTEGER) || 
          (actToken.stav == S_RETEZEC) || (actToken.stav == S_DOUBLE) ||
          (actToken.stav == S_BOOLEAN) || (actToken.stav == S_LEVA_ZAVORKA)) // opytat sa ci je to ?
        {
            //vieme ze nemama FUKNCIU, BUDEME PUSTAT PRECEDENCNU---->isVyraz();

            myPop(&S);

            if ((isVyraz()) == 0)   // ak podmienka plati vytvorime instrukciu priradenia
            {
                NaplnInstr( I_PRIRAD, NULL , NULL, NULL );
            }
        }

        else
        {
            // chybova hlaska CHYBISKA....
        }
        break;
    }


//----------------------------------BSTAT----------------------------------
    case  LL_BSTAT:
    {
        myPop(&S);
        myPushMul(&S, 3, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END );

        break;
    }


//----------------------------------SPLIST--------------------------------
    case  LL_SPLIST:
    {
        if (actToken.stav == S_IDENTIFIKATOR )
        {

            myPop(&S);
            myPushMul(&S, 2, S_IDENTIFIKATOR, LL_NSPLIST);
            if(priznak == write)
            {
                void *spracADDR = spracuj(actToken.stav, actToken.data);

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
                void *spracADDR = spracuj(actToken.stav, actToken.data); // 
                void *spracID = spracuj(S_IDENTIFIKATOR, actToken.data);


                tStav *TIPSTAV = malloc(sizeof(tStav));
                *TIPSTAV = actToken.stav;

                NaplnInstr( I_PREC, spracID, spracADDR, TIPSTAV );
                NaplnInstr( I_WRITE_INT, NULL, NULL, NULL );
            }
        }
        else if (actToken.stav == S_DOUBLE)
        {
            if(priznak == write)
            {
                void *spracADDR = spracuj(actToken.stav, actToken.data);

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
        else if(actToken.stav == S_PRAVA_ZAVORKA)
        {
            // generuj instrukcie spust funkciu
            //s_con->c_fun // tu mam ulozeny identifikator ff ***********************************OVERIT

            myPop(&S); // ->eps prechod ked namam ziadany parameter
            priznak = nic;
        }
        else
        {
            fprintf(stderr, "2: Syntakticka chyba. Ocakaval som pravu zatvorku.\n");
            exit(2);
        }
        break;
    }


//----------------------------------NSPLIST------------------------------------------
    case  LL_NSPLIST:
    {
        if (actToken.stav == S_CARKA )
        {
            actToken=get_token();


                        if (actToken.stav == S_IDENTIFIKATOR )
                    {

                        myPop(&S);
                        myPushMul(&S, 2, S_IDENTIFIKATOR, LL_NSPLIST);
                        if(priznak == write)
                        {
                            void *spracADDR = spracuj(actToken.stav, actToken.data);

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
                            void *spracADDR = spracuj(actToken.stav, actToken.data); // 
                            void *spracID = spracuj(S_IDENTIFIKATOR, actToken.data);


                            tStav *TIPSTAV = malloc(sizeof(tStav));
                            *TIPSTAV = actToken.stav;

                            NaplnInstr( I_PREC, spracID, spracADDR, TIPSTAV );
                            NaplnInstr( I_WRITE_INT, NULL, NULL, NULL );
                        }
                    }
                    else if (actToken.stav == S_DOUBLE)
                    {
                        if(priznak == write)
                        {
                            void *spracADDR = spracuj(actToken.stav, actToken.data);

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
                        fprintf(stderr, "2: Syntakticka chyba. Ocakaval som dalsi parameter. \n");
                        exit(2);
                    }


        }
        else if(actToken.stav == S_PRAVA_ZAVORKA)
        {
            // generuj instrukcie spust funkciu
            //s_con->c_fun // tu mam ulozeny identifikator ff ***********************************OVERIT

            myPop(&S); // ->eps prechod:: ked pride prave jeden parameter
            priznak = nic;
        }


        else
        {
            fprintf(stderr, "2: Syntakticka chyba. Ocakaval som pravu zatvorku.\n");
            exit(2);
        }
        break;
    }

    case LL_E:
    {
        myPop(&S);

        if ((isVyraz()) == 0)   // ak podmienka plati vytvorime instrukciu priradenia
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


    tSem_context s_con;           //instancia struktury nesie semanticky kontext a jeho atributy
    s_con.scope = LOCAL;        //na zaciatku je lokalny kontext
    s_con.context = G_VAR_DEC;    //na zaciatku zdrojaku je kontext deklaracii glob. premennych


    while(actToken.stav != S_END_OF_FILE) // dokym som neni na konci suboru
    {

        if (myTop(&S) == EOF && actToken.stav != S_END_OF_FILE)
        {
            //CHYBA!!!! na zasobniku bol uz iba EOF ale my sme este nedocitali subor
            fprintf(stderr, "CHyBA vyprazdneny zasobnik a este sme neni na konci suboru\n");
        	exit(2); // syntakticka chyba
        }


        if (myTop(&S) >= LL_INIT && myTop(&S) <= LL_NSPLIST )  // TERMINAL/NETERMINAL
        {
            // NETERMINAL  velke mismenka
            //printf("PUSTAM EXTRACT\n");
            extractRule (&s_con); // rozvin pravidla...
            //printf("HOTOVO EXTRACT\n");
        }
        else
        {
            // TERMINAL male pismenka
            if (((unsigned int) myTop(&S)) == actToken.stav)
            {
                //showStack(&S);
               // printf("Pustam TERMINAL  actToken je ");
                //whattoken(actToken.stav);
                //printf("PUSTAM TERMINAL  a mam na TOPE a zmazem ho ");
                //whattoken(myTop(&S));

//-------------------------------GENER-READ---------------------------------------------------------------//
                if ((actToken.stav == S_IDENTIFIKATOR) && (priznak == read)) // ak ideme citat a pride nam CO ideme citat
                {

                    void *spracADDR = spracuj(actToken.stav, actToken.data);
                    if (spracADDR == NULL )

                    NaplnInstr( I_READ, NULL, spracADDR, NULL );
                    priznak=nic;
                }
//--------------------------------------------------------------------------------------------------------//


                myPop(&S);	// odstranime z vrcholu zasobnika
                //free(actToken.data); // free

                if ( actToken.stav == S_IDENTIFIKATOR ) 
                {                                       //ulozenie id funkcie pri deklaracii
                  if (Id_sign == rem_id) 
                  {/*
                    // *********** TOTO JE INSTRUKCIA***ZACIATOK************************************
                    // posielam ID funkcie koli ADRESE ZACIATKU FUN NA instrukc paske
                    void *spracADDR = spracuj(actToken.stav, actToken.data); // slo by to aj inac ale pre jednotnost..
                    NaplnInstr(I_JMPF_KEY_S, spracADDR, NULL, NULL); // ulozi aktualnu instrukciu 

                    //budeme alokovat miesto pre navratovu hodnotu podla kluca  toto je ALOKACIA NAVRATOVEJ HODNOTY!!
                    NaplnInstr( I_IDENT, NULL, spracADDR, NULL );
                    // *********** TOTO JE INSTRUKCIA***KONIEC************************************
					*/

                    Id_sign = for_id;                      //reset signum
                    s_con.act_fun = actToken.data;   //save actual id of function
                    sem_check (&s_con);
                  }

                  if (Id_sign == rem_pid)
                  {
                    Id_sign = for_pid;
                    s_con.act_id = actToken.data;
                  }
                }

                
                actToken = get_token(); // nacitame novy token
               // printf("KOEC TERMINAL GET TOKEN token je ");
                //whattoken(actToken.stav);
                //printf("KOEC TERMINAL GET TOKEN TOP  je ");
               // whattoken(myTop(&S)) ;
            }
            else
            {
                fprintf(stderr, "2: Syntakticka chyba. Ocakaval som >> ");
                whattoken(myTop(&S));
                fprintf(stderr, "\n" );
            	exit(2); // syntakticka chyba
                ERRO = false;
               // printf("mas to zle ja som cakal  >> ");
               // whattoken(myTop(&S));
                //printf("mas to zle NAPISAL SI    >> ");
               // whattoken( actToken.stav);
               // printf("mas to zle riadok.sltpec >> %d .. %d\n", actToken.radek + 1, actToken.sloupec);
                myPop(&S);	// odstranime z vrcholu zasobnika
                //free(actToken.data); // free
                actToken = get_token(); // nacitame novy token
            }
        }

        if (myTop(&S) != EOF && actToken.stav == S_END_OF_FILE)
        {
        	exit(2); // syntakticka chyba
            ERRO = false;
            //printf("ChyBA na zasobniku nieco zostalo a my sme na konci suboru\n");
        }
    }
    if(ERRO)
        return true;
    else
        return false;
    //free(actToken.data); // free
}


list* get_local (char* id) {  //vrati referenciu na lokalnu tabulku podla zadaneho id funkcie
  return ((list_element) hash_adress(GLOBFRAME, id))->ref;
}

void sem_check (tSem_context* s_con)
{
  switch (s_con->context) 
  {
    case G_VAR_DEC:              //kontext deklaracii glob. premennych
      if ( hash_search (GLOBFRAME, s_con->act_id) == CONTAINS ) { //error if var exists
        fprintf (stderr, "semanticka chyba pri deklaraci globalnej premennej \'%s\', volam exit(3), dealokuje OS\n", s_con->act_id);
        exit (semanticka_chyba_pri_deklaraci);
      }

      hash_insert_it (GLOBFRAME,s_con->act_id, s_con->act_type );  //save var to GTS
    break;


    case FUNCTION_DEC:          //kontext deklaracii funkcii
      //funkcia nebola deklarovana, vytvori sa nova LTS
      if ( hash_search (GLOBFRAME, s_con->act_fun) == NOCONTAINS )
        { 
          hash_insert_i (GLOBFRAME, s_con->act_fun);     //vlozenie id funkcie
          hash_insert_func (GLOBFRAME, s_con->act_fun);  //vytvorenie LTS funkcie
          hash_insert_it (GLOBFRAME, s_con->act_fun, F_ID);  //id funkcie je typu F_ID
          set_arg_num (GLOBFRAME, s_con->act_fun, arg_num);  //inicializacia poctu argumentov
          break;
        }
    
      //funkcia bola deklarovana, ocakava sa definicia, nevytvara sa nova LTS
      if (hash_is_sign (GLOBFRAME, s_con->act_fun) == FORWARDED) { 
        hash_set_sign (GLOBFRAME, s_con->act_fun, DEFINED);
        f_counter--;  //pocet forwardnutych funkcii sa znizi
        break;
      }

      //nastala chyba pri definicii alebo deklaracii funkcie
      fprintf (stderr, "semanticka chyba pri deklaraci funkcie \'%s\', volam exit(3), dealokuje OS\n", s_con->act_fun);
      exit (semanticka_chyba_pri_deklaraci);
  

    case FUNC_ARG_DEC:        //kontext deklaracie a overovania argumentov funkcie
      //definicia funkcie, overuju sa argumenty
      if (hash_is_sign (GLOBFRAME, s_con->act_fun) == DEFINED) {
       //chyba ak argument bebol deklarovany alebo nesedi typ alebo pozicia
        if ( hash_search (get_local (s_con->act_fun), s_con->act_id) == NOCONTAINS ||
             hash_return_type (get_local (s_con->act_fun), s_con->act_id) != s_con->act_type ||
             get_arg_num (get_local (s_con->act_fun), s_con->act_id) != ++arg_num ) 
          {
            fprintf (stderr, "semanticka chyba argumentu \'%s\' funkcie \'%s\', volam exit(3), dealokuje OS\n", s_con->act_id, s_con->act_fun);
            exit(semanticka_chyba_pri_deklaraci);
          }
      }

      //funkcia nebola deklarovana, ulozia sa argumenty
      else {
        //chyba ak parameter uz existuje
        if (hash_search (get_local (s_con->act_fun), s_con->act_id) == CONTAINS) {
          fprintf (stderr, "semanticka chyba pri deklaraci argumentu \'%s\' funkcie \'%s\', volam exit(3), dealokuje OS\n", s_con->act_id, s_con->act_fun);
          exit (semanticka_chyba_pri_deklaraci);
        }

        //ulozenie id a jeho typu do LTS funkcie
        hash_insert_it (get_local (s_con->act_fun), s_con->act_id, s_con->act_type);
        //ulozi sa pozicia argumentu        
        set_arg_num (get_local (s_con->act_fun), s_con->act_id, ++arg_num);
        set_arg_num (GLOBFRAME, s_con->act_fun, arg_num); //zvysi sa pocet argumentov funkcie
      }
    break;
      

    case FUNC_TYPE_DEC:     //kontext deklaracie typu navratovej hodnoty funkcie
      //funkcia bola deklarovana, musi sa overit typ
      if (hash_is_sign (GLOBFRAME, s_con->act_fun) == DEFINED) {
        //ak sa typ pri deklaracii nezhoduje s typom pri definicii
        if (hash_return_type (get_local (s_con->act_fun), s_con->act_fun) != s_con->act_type) {
          fprintf (stderr, "semanticka chyba pri defenici typu navratovej hodnoty funkcie \'%s\', volam exit(3), dealokuje OS\n", s_con->act_fun);
          exit(semanticka_chyba_pri_deklaraci);
        }
        
        //funkcia bola deklarovana a pri definicii bol zadany nespravny pocet argumentov
        if (get_arg_num (GLOBFRAME, s_con->act_fun) != arg_num) {
          fprintf (stderr, "nespravny pocet argumentov pri defenici funkcie '%s'\n", s_con->act_fun);
          exit (semanticka_chyba_pri_deklaraci);
        }
      }
      else //funkcia nebola deklarovana, uklada sa typ
        hash_insert_it (get_local(s_con->act_fun), s_con->act_fun, s_con->act_type);
   
      arg_num = 0; //reset pocitadla argumentov, suvisi s FUNC_ARG_DEC
   break;
     

    case L_VAR_DEC:      //kontext deklaracie lokalnych premennych
      //chyba, ak premenna alebo argument alebo navratova hodnota funkcie existuje
      if ( hash_search (get_local (s_con->act_fun), s_con->act_id) == CONTAINS) {
        fprintf (stderr, "semanticka chyba pri deklaraci lokalnej premennej \'%s\' funkcie \'%s\', volam exit(3), dealokuje OS\n", s_con->act_id, s_con->act_fun );
        exit (semanticka_chyba_pri_deklaraci);
      }
      
      //ulozenie premennej do LTS
      hash_insert_it (get_local (s_con->act_fun), s_con->act_id, s_con->act_type);
    break;


    case RET_VAL_CHECK:            //kontrola priradenia id := funkcia()
      //lokalny rozsah premennych
      if (s_con->scope == LOCAL) {
        //premenna nie je deklarovana lokalne
        if ( hash_search (get_local (s_con->act_fun), s_con->l_id) != CONTAINS ) {      
          
          //premenna nie je deklarovana ani globalne - chyba
          if ( hash_search (GLOBFRAME, s_con->l_id) != CONTAINS ) {
            fprintf (stderr, "premenna \'%s\' nebola deklarovana\n", s_con->l_id);
            exit (semanticka_chyba_pri_deklaraci); 
          }
          else { //premenna je deklarovana globalne
            //overenie typov premennej a navratovej hodnoty funkcie
            if ( hash_return_type (GLOBFRAME, s_con->l_id) != 
                 hash_return_type (get_local (s_con->c_fun), s_con->c_fun) )
              {
                fprintf (stderr, "typova nekompatibilita medzi funkciou \'%s\' a premennou \'%s\'\n", s_con->c_fun, s_con->l_id);
                exit (semanticka_chyba_typove_kompatibility); 
              }
          }
        }
        else {  //premenna je deklarovana lokalne
          //overenie typov premennej a navratovej hodnoty funkcie
          if ( hash_return_type (get_local (s_con->act_fun), s_con->l_id) != 
               hash_return_type (get_local (s_con->c_fun), s_con->c_fun) )
            {
              fprintf (stderr, "typova nekompatibilita medzi funkciou \'%s\' a premennou \'%s\'\n", s_con->c_fun, s_con->l_id);
              exit (semanticka_chyba_typove_kompatibility); 
            }
        }
      }
        
      //globalny rozsah premennych
      if (s_con->scope == GLOBAL) {
        //globalna premenna nebola deklarovana
        if (hash_search (GLOBFRAME, s_con->l_id) != CONTAINS) {
          fprintf (stderr, "globalna premenna \'%s\' nebola deklarovana\n", s_con->l_id);
          exit (semanticka_chyba_pri_deklaraci); 
        }
        //premenna bola deklarovana, overenie typu premennej a navratovej hodnoty funkcie
        if ( hash_return_type (GLOBFRAME, s_con->l_id) !=
             hash_return_type (get_local (s_con->c_fun), s_con->c_fun) )
          {
            fprintf (stderr, "typova nekompatibilita medzi funkciou \'%s\' a premennou \'%s\'\n"    , s_con->c_fun, s_con->l_id);
            exit (semanticka_chyba_typove_kompatibility);
          }
      } 
    break;
  }
}
