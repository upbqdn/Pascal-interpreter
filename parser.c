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
#include "garbage.h"

void* zarazka; // global

list *GLOBFRAME; // globalna tabulka
astack FRAME;

astack IFJMP;

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
            //syntakt chiba
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som neaky datovy typ.\n", actToken.radek+1);
            trashDestroy(2);
            break;
        }
        }
        break;
    }


    case LL_FLIST:
    {
        if (actToken.stav == S_KLIC_FUNCTION)
        {
            //gener-dole-------------------------------------

            // JMP na BEGIN hlavneho programu
            // podla kluca 'begin' si najde v GLOBAL->(key)start  adresu kam skocit
            void *spracADDR = spracuj(S_IDENTIFIKATOR, "begin"); // slo by to aj inac ale pre jednotnost..
            NaplnInstr(I_JMPF_KEY, NULL, spracADDR, NULL); // skoci prec

            //gener--hore--------------------------


            myPop(&S);
            myPushMul(&S, 9, S_KLIC_FUNCTION, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_PLIST, S_PRAVA_ZAVORKA, S_DVOJTECKA, LL_TYPE, S_STREDNIK, LL_FUNC);

            s_con->context = FUNCTION_DEC;  //prepnutie kontextu na deklaraciu funkcie
            Id_sign = rem_id;                     //pri spracovani sa id ulozi

            //  FLIST -> function id ( PLIST ) : TYPE ; FUNC
        }
        else
        {
            //--gener-dole-----------------------------

            // generuj adresu BEGINU... tu je begin
            void *spracADDR = spracuj(S_IDENTIFIKATOR, "begin"); // slo by to aj inac ale pre jednotnost..
            // NaplnInstr(I_JMPF_KEY_S, NULL, spracADDR, NULL); //


            //printf("INTER_uloz BEGIN! : navestie funkcie \n");
            // uloz adresu KDE sa skoci  do GLOB TAB... cize navestie funkcie


            list_element prvok;
            prvok = (list_element)(hash_adress(GLOBFRAME, spracADDR));
            void* pomad;

            //navestie na BEGIN
            NaplnInstr(I_NICNEROBA, NULL, NULL, NULL);

            pomad = InstrDajPoz(&INSTR_PASKA);
            // &(prvok)->ref

            ((prvok)->start) = pomad;

            //--gener hore----------------------------------------------------


            myPop(&S); //  FLIST -> eps prechod

            if (f_counter != 0)    //semanticka kontrola, ci vsetky funkcie boli definovane
            {
                fprintf (stderr, "v programe su nedefinovane funkcie\n");
                trashDestroy (semanticka_chyba_pri_deklaraci);
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
            //--gener-----------ESTE NEVIEME -------------------------------

            myPop(&S);
            myPushMul(&S, 3, S_KLIC_FORWARD, S_STREDNIK, LL_FLIST);


            s_con->context = FUNC_TYPE_DEC;  //kontext nastavenia navratoveho typu funkcie
            sem_check (s_con);  //ulozi sa typ funkcie

            if (hash_is_sign (GLOBFRAME, s_con->act_fun) != DEFINED)
            {
                f_counter++;  //zvysuje sa pocet forwardnutych funkcii
                //funkcia sa nastavi na forwarded, ak nebola definovana
                hash_set_sign (GLOBFRAME, s_con->act_fun, FORWARDED);
            }
            else
            {
                fprintf (stderr, "chyba pri deklaracii funkcie \'%s\' na riadku '%d'\n",
                         s_con->act_fun, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
            }

            // FUNC -> forward ; FLIST
        }
        else // nasleduje deklaracia (telo) funkcie
        {
            myPop(&S);
            myPushMul(&S, 6, LL_VLIST, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END, LL_FUNCEND, LL_FLIST );


            s_con->context = FUNC_TYPE_DEC;  //kontext nastavenia navratoveho typu funkcie
            sem_check (s_con);  //ulozi sa typ funkcie, v pripade definicie sa skontroluje

            s_con->context = L_VAR_DEC;  //nasleduje kontext deklaracie lok. premennych

            // FUNC -> VLIST begin STLIST end ; FLIST
        }

        break;
    }

    case LL_FUNCEND:
    {
        if (actToken.stav == S_STREDNIK)
        {
            myPop(&S);
            myPush(&S, S_STREDNIK);

            //----------------gener---------------------------------------------------
            NaplnInstr(I_JMP_BACK, NULL, NULL, NULL); // skoci naspet ... ZASOBNIK skokov.
            //direct JUMP MA BYT ZMENTTOOO

            //----------------gener---------------------------------------------------
        }
        else
        {
            //syntakt chiba
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som predtym ", actToken.radek+1);
            whattoken(S_STREDNIK);
            fprintf(stderr, "\n");
            trashDestroy(2);
        }

        break;
    }


    case LL_PLIST:
    {
        if (actToken.stav == S_IDENTIFIKATOR)
        {
            //gener-----------------------------------------------------
            // generujeme
            void *spracADDR = spracuj(actToken.stav, actToken.data); // mame KLUC
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL ); // ulozime identifikator 2x
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL ); // ulozime identifikator 2x

            //-----------gener hore ---------------------------------------------------

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


//--------------------------------krasny stav navyse--------------------------------------------------------

    case LL_NPLIST_NID:
    {
        if (actToken.stav == S_IDENTIFIKATOR)
        {
            myPop(&S);
            myPush(&S, S_IDENTIFIKATOR);

            //gener-----------------------------------------------------
            // generujeme
            void *spracADDR = spracuj(actToken.stav, actToken.data); // mame KLUC
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL ); // ulozime identifikator 2x
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL ); // ulozime identifikator 2x

            //-----------gener hore ---------------------------------------------------

        }
        else
        {
            //syntakt chyba
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som ", actToken.radek+1);
            whattoken(S_IDENTIFIKATOR);
            fprintf(stderr, "\n");
            trashDestroy(2);
        }

        break;
    }




//--------------------------------krasny stav navyse--------------------------------------------------------


    case LL_NPLIST:
    {
        if (actToken.stav == S_STREDNIK)
        {
            myPop(&S);
            myPushMul(&S, 5, S_STREDNIK, LL_NPLIST_NID,  S_DVOJTECKA, LL_TYPE, LL_NPLIST);

            sem_check (s_con);   //kontrola parametra

            Id_sign = rem_pid;  // id dalsieho parametra sa zapamata

            // NPLIST -> ; id : TYPE NPLIST



        }
        else
        {
            myPop(&S); // NPLIST -> eps prechod

            sem_check (s_con);  // kontrola parametra


        }

        //->act_type;
        //printf("AKTUAL TYP :::%d:::\n", s_con->act_type); whattoken(s_con->act_type);
        //--gener-------------------------------------------------------------------
        // toto sa deje vzdy
        // ber co mas na POMOCNOM zasobniku + pop pomocny
        // prirad
        tStav *TIPSTAV = mymalloc(sizeof(tStav));;
        *TIPSTAV = s_con->act_type;

        NaplnInstr( I_FUN_PRIRAD_PARAM, NULL, NULL, TIPSTAV );
        //--gener-------------------------------------------------------------------



        break;
    }


    case  LL_STLIST:
    {
        if ( (actToken.stav == S_KLIC_BEGIN) || (actToken.stav == S_IDENTIFIKATOR) || (actToken.stav == S_KLIC_WHILE) || (actToken.stav == S_KLIC_IF) || (actToken.stav == S_KLIC_READLN) || (actToken.stav == S_KLIC_WRITE))
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

        case S_KLIC_BEGIN:
        {
            // begin begin begin end end end
            myPop(&S);
            myPush(&S, LL_BSTAT); //sem bstat klasik

            break;
        }


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
            myPushMul(&S, 4, S_KLIC_WHILE, LL_E, S_KLIC_DO, LL_BSTAT_WHILE);

            // koli instrukciam
            void *whileaddr1 = mymalloc(sizeof(struct tPrvokListu)) ;
            myaPush(&IFJMP, whileaddr1);

            // vytvorime si navestie pomocou NIC-NEROBY :D
            NaplnInstr(I_NICNEROBA, NULL,NULL,NULL);

            void* pomad;
            pomad = InstrDajPoz(&INSTR_PASKA);

            *(void**) (myaTop(&IFJMP)) = pomad; // HVIEZDICKI



            void *whileaddr2 = mymalloc(sizeof(struct tPrvokListu)) ;
            myaPush(&IFJMP, whileaddr2);

            break;
        }

        case S_KLIC_IF:
        {
            myPop(&S);
            myPushMul(&S, 6, S_KLIC_IF, LL_E, S_KLIC_THEN, LL_BSTAT, LL_IF_END, LL_BSTAT_ELSE_VETVA);

            // koli instrukciam
            void *ifaddr1 = mymalloc(sizeof(struct tPrvokListu)) ;
            myaPush(&IFJMP, ifaddr1);


            void *ifaddr2 = mymalloc(sizeof(struct tPrvokListu)) ;
            myaPush(&IFJMP, ifaddr2);


            break;
        }



        case S_KLIC_READLN:
        {
            myPop(&S);
            myPushMul(&S, 4, S_KLIC_READLN, S_LEVA_ZAVORKA, S_IDENTIFIKATOR, S_PRAVA_ZAVORKA);
            priznak=read;


            Id_sign = readln;  //priznak zapamatania id pre readln

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

            
            s_con->write_sgn = WRITE;  //obmedzenie typovej kontroly pri argumentoch
            
            break;
        }

        default:
        {
            if (actToken.stav == S_STREDNIK)
            {
                //syntakt chyba
                fprintf(stderr, "2: Syntakticka chyba na riadku '%d'.  Za poslednym prikazom nesmie byt strednik ! \n", actToken.radek+1);
                trashDestroy(2);
            }
            else
            {
               //syntakt chyba
                fprintf(stderr, "2: Syntakticka chyba na riadku '%d'.\n", actToken.radek+1);
                trashDestroy(2); 
            }
            
            break;
        }
        }
        break;
    }


//----------------------------------BSTAT----dole------------------------------

    // BSTAT klasik - begin begin begin end end end
    case  LL_BSTAT:
    {
        myPop(&S);
        myPushMul(&S, 3, S_KLIC_BEGIN, LL_STLIST, S_KLIC_END );

        break;
    }




//----toto je WHILE----dole---------------------------------

    case LL_BSTAT_WHILE:
    {
        myPop(&S);
        myPushMul(&S, 3, S_KLIC_BEGIN, LL_STLIST, LL_BSTAT_WHILE_END );

        // podmienka vyhodnotena

        break;
    }


    case LL_BSTAT_WHILE_END:
    {
        if (actToken.stav == S_KLIC_END)
        {
            myPop(&S);
            myPush(&S, S_KLIC_END );

            NaplnInstr(I_JMP, myaSecTop(&IFJMP), NULL, NULL);

            NaplnInstr(I_NICNEROBA, NULL, NULL, NULL); // pomocna instrukcia

            void* pomad;
            pomad = InstrDajPoz(&INSTR_PASKA);

            *(void**) (myaTop(&IFJMP)) = pomad; // HVIEZDICKI

            myaPop(&IFJMP);
            myaPop(&IFJMP);



        }
        else
        {
            //syntakt chyba
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som ", actToken.radek+1);
            whattoken(S_KLIC_END);
            fprintf(stderr, "\n");
            trashDestroy(2);
        }
        break;
    }
//----toto je WHILE----hore---------------------------------



//----toto je IF-------dole---------------------------------

case LL_IF_END:
    {
        if (actToken.stav == S_KLIC_ELSE)
        {
            myPop(&S);
            myPush(&S, S_KLIC_ELSE);

            // treba zapisat do instrukcie adresu kam sa bude skakat.
            // lenze to miestecko mame az ako druhe v zasobniku
            // DIRECT JMP musi skocit

            NaplnInstr(I_JMP, myaSecTop(&IFJMP), NULL, NULL);

            NaplnInstr(I_NICNEROBA, NULL,NULL,NULL);

            // tu nam zacina vetva ELSE.. sem sme mohli skocit po vyhodnoteni vyrazu >> FALSE
            void* pomad;
            pomad = InstrDajPoz(&INSTR_PASKA);

            *(void**) (myaTop(&IFJMP)) = pomad; // HVIEZDICKI

            myaPop(&IFJMP);


        }
        else
        {
            //syntakt chyba
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som ", actToken.radek+1);
            whattoken(S_KLIC_ELSE);
            fprintf(stderr, "\n");
            trashDestroy(2);
        }

        break;
    }


//----toto je ELSE VETVA   IF-u ----dole---------------------------------
    case  LL_BSTAT_ELSE_VETVA:
    {
        myPop(&S);
        myPushMul(&S, 3, S_KLIC_BEGIN, LL_STLIST, LL_BSTAT_END );

        break;
    }

    case LL_BSTAT_END:
    {
        if (actToken.stav == S_KLIC_END)
        {
            myPop(&S);
            myPush(&S, S_KLIC_END );
            // PRE IF kocien

            // tu nam konci vetva ELSE.. sem sme mohli skocit po vyhodnoteni vyrazu >> FALSE
            NaplnInstr(I_NICNEROBA, NULL, NULL, NULL); // pomocna instrukcia


            void* pomad;
            pomad = InstrDajPoz(&INSTR_PASKA);

            *(void**) (myaTop(&IFJMP)) = pomad; // HVIEZDICKI

            myaPop(&IFJMP);


        }
        else
        {
            //syntakt chyba
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som ", actToken.radek+1);
            whattoken(S_KLIC_END);
            fprintf(stderr, "\n");
            trashDestroy(2);
        }
        break;
    }

//----toto je ELSE VETVA   IF-u -----hore--------------------------------


//----------------------------------BSTAT----hore------------------------------

    




//--------------------------RHS-----------------------------------------------------------------//

    case  LL_RHS:
    {
        if ( actToken.stav == S_IDENTIFIKATOR &&
                hash_return_type (GLOBFRAME, actToken.data) == F_ID)  //semantika overuje typ id
        {
            // skontrolovat ci je su v podmienke aktualne nazvy//
            // toto je FUNKCIA
/*
            //----------------------------gener-------------------------------------------------------
            // generujeme zaciatocne veci pri spusteni FUNKCIE-kopirovanie tabulki, nahodenie na FRAMESTACK
            // + dalej generujeme pomocnu zarazku na zasobnik -- vsetko v 1 instrukcii

            void *spracADDR = spracuj(actToken.stav, actToken.data);

            NaplnInstr( I_RUNFUN_COPY, NULL , spracADDR, NULL ); // podla kluca

            //-----gener--------------------------------------------------------------------------------
*/
            
            NaplnInstr(I_ZARAZKA,NULL,NULL,NULL);

            myPop (&S);
            myPushMul (&S, 4, S_IDENTIFIKATOR, S_LEVA_ZAVORKA, LL_SPLIST, S_PRAVA_ZAVORKA );


            s_con->c_fun = actToken.data;   //ulozi sa id volanej funkcie
            s_con->context = RET_VAL_CHECK;  //typova kontrola priradenia navr. hod. funkcie
            sem_check (s_con);
        }

        else if
        ((actToken.stav == S_IDENTIFIKATOR ) || (actToken.stav == S_INTEGER) ||
                (actToken.stav == S_RETEZEC) || (actToken.stav == S_DOUBLE)   ||
                (actToken.stav == S_BOOLEAN) || (actToken.stav == S_KLIC_FALSE)  ||
                (actToken.stav == S_KLIC_TRUE) || (actToken.stav == S_LEVA_ZAVORKA)) // opytat sa ci je to ?
        {
            //vieme ze nemama FUKNCIU, BUDEME PUSTAT PRECEDENCNU---->isVyraz();

            myPop(&S);


            //overenie lavej strany priradenia LHS
            s_con->context = ID_DEC_CHECK;      //kontrola deklaracie
            s_con->act_id = s_con->l_id;        //bude sa kontrolovat l_id
            sem_check (s_con);
            s_con->l_type = s_con->act_type;    //ulozi sa typ l_id


            if ((isVyraz(s_con)) == 0)   // ak podmienka plati vytvorime instrukciu priradenia
            {

                NaplnInstr( I_PRIRAD, NULL , NULL, NULL );
            }

            
            s_con->context = EXP_RET_CHECK;  //kontrola typov LHS a RHS
            sem_check (s_con);  
        }

        else
        {
            fprintf(stderr, "2: Syntakticka chyba. Ocakaval som < FUNKCIU / VYRAZ >\n" );
            trashDestroy(2);
        }
        break;
    }





//----------------------------------SPLIST--------------------------------
    case  LL_SPLIST:
    {
        if (actToken.stav == S_IDENTIFIKATOR )
        {
            myPop(&S);
            myPushMul(&S, 2, S_IDENTIFIKATOR, LL_NSPLIST);


            void *spracADDR = spracuj(actToken.stav, actToken.data); // kluc
            NaplnInstr( I_IDENT, NULL, spracADDR, NULL );


            if(priznak == write)
            {
                NaplnInstr( I_WRITE_IDE, NULL, NULL, NULL );
            }
            
            s_con->act_id = actToken.data;   //ulozenie id argumentu
            s_con->context = ID_ARG_CHECK;   //kontext kontroly id parametra pri volani funkcie 
            sem_check (s_con);
        }
        else if (actToken.stav == S_INTEGER)
        {
            myPop(&S);
            myPushMul(&S, 2, S_INTEGER, LL_NSPLIST);



            void *spracADDR = spracuj(actToken.stav, actToken.data); //
            void *spracID = spracuj(S_IDENTIFIKATOR, actToken.data);


            tStav *TIPSTAV = mymalloc(sizeof(tStav));
            *TIPSTAV = actToken.stav;

            NaplnInstr( I_PREC, spracID, spracADDR, TIPSTAV );

            if(priznak == write)
            {
                NaplnInstr( I_WRITE_INT, NULL, NULL, NULL );
            }


            s_con->act_type = S_INTEGER;   //kontrola hodnoty typu integer
            s_con->context = DV_ARG_CHECK;
            sem_check (s_con);
        }
        else if (actToken.stav == S_DOUBLE)
        {
            myPop(&S);
            myPushMul(&S, 2, S_DOUBLE, LL_NSPLIST);

            void *spracADDR = spracuj(actToken.stav, actToken.data);
            void *spracID = spracuj(S_IDENTIFIKATOR, actToken.data);

            tStav *TIPSTAV = mymalloc(sizeof(tStav));
            *TIPSTAV = actToken.stav;

            NaplnInstr( I_PREC, spracID, spracADDR, TIPSTAV );

            if(priznak == write)
            {
                NaplnInstr( I_WRITE_DOU, NULL, NULL, NULL );
            }

        
            s_con->act_type = S_DOUBLE;   //kontrola hodnoty typu double
            s_con->context = DV_ARG_CHECK;
            sem_check (s_con);
        }
        else if (actToken.stav == S_RETEZEC)
        {
            myPop(&S);
            myPushMul(&S, 2, S_RETEZEC, LL_NSPLIST);

            void *spracADDR = spracuj(actToken.stav, actToken.data);

            tStav *TIPSTAV = mymalloc(sizeof(tStav));
            *TIPSTAV = actToken.stav;

            NaplnInstr( I_PREC, spracADDR, spracADDR, TIPSTAV );

            if(priznak == write)
            {
                NaplnInstr( I_WRITE_STR, NULL , NULL, NULL );
            }


            s_con->act_type = S_RETEZEC;   //kontrola hodnoty typu string
            s_con->context = DV_ARG_CHECK;
            sem_check (s_con);
        }
        else if (actToken.stav == S_KLIC_TRUE) //////////////////////////////////////////// CO ROBIT ????? BOL/TRU
        {

            myPop(&S);
            myPushMul(&S, 2, S_KLIC_TRUE, LL_NSPLIST);

            void *spracADDR = spracuj(actToken.stav, actToken.data);

            tStav *TIPSTAV = mymalloc(sizeof(tStav));
            *TIPSTAV = actToken.stav;

            NaplnInstr( I_PREC, spracADDR, spracADDR, TIPSTAV );

            if(priznak == write)
            {
                NaplnInstr( I_WRITE_BOO, NULL , NULL, NULL );
            }


            s_con->act_type = S_KLIC_BOOLEAN;   //kontrola hodnoty typu boolean
            s_con->context = DV_ARG_CHECK;
            sem_check (s_con);
        }
        else if (actToken.stav == S_KLIC_FALSE)
        {

            myPop(&S);
            myPushMul(&S, 2, S_KLIC_FALSE, LL_NSPLIST);

            void *spracADDR = spracuj(actToken.stav, actToken.data);

            tStav *TIPSTAV = mymalloc(sizeof(tStav));
            *TIPSTAV = actToken.stav;

            NaplnInstr( I_PREC, spracADDR, spracADDR, TIPSTAV );
            
            if(priznak == write)
            {
                NaplnInstr( I_WRITE_BOO, NULL , NULL, NULL );
            }


            s_con->act_type = S_KLIC_BOOLEAN;   //kontrola hodnoty typu boolean
            s_con->context = DV_ARG_CHECK;
            sem_check (s_con);
        }
        else if(actToken.stav == S_PRAVA_ZAVORKA)
        {
            // generuj instrukcie spust funkciu
            //s_con->c_fun // tu mam ulozeny identifikator ff ***********************************OVERIT

            //----generrr--------------------------------------------------------------------------------

            // tu sa ukoncuje SPLIST ... zatim by mala nasledovat ')'
            // daj mi to na POMOCNY zasobnik az po zarazku // koli spravnemu poradiu param funkcie
            // generujem instrukciu DAJ NA POMOCNY ZASOBNIK az po zarazku ak sa spusta funkcia
            void *spracADDR = spracuj(S_IDENTIFIKATOR, s_con->c_fun);



            if (priznak != write) // !!IBA!! ak mam UZIVATELSKU FUNKCIU !!
            {

                NaplnInstr( I_RUNFUN_PARAM, NULL, NULL, NULL );

                           //----------------------------gener-------------------------------------------------------
            // generujeme zaciatocne veci pri spusteni FUNKCIE-kopirovanie tabulki, nahodenie na FRAMESTACK
            // + dalej generujeme pomocnu zarazku na zasobnik -- vsetko v 1 instrukcii

            NaplnInstr( I_RUNFUN_COPY, NULL , spracADDR, NULL ); // podla kluca

            //-----gener--------------------------------------------------------------------------------


                

                // ULOZ ADRESU FUNKCIE SAVE na zasobnik skokov
                NaplnInstr( I_JMP_S, NULL , NULL, NULL );

                // spusti FUNKCIU.........................................................................................SPUSTI FUNKCIU!!

                NaplnInstr( I_JMPF_KEY, NULL , spracADDR, NULL ); // KLUC zo zapametania

                // I PRIRAD
                NaplnInstr( I_PRIRAD, NULL , NULL, NULL ); // priradime

                // stack del FRAME
                NaplnInstr( I_FRAME_DEL, NULL , NULL, NULL );

            }


            //-----geber---------------------------------------------------------------------------------

            myPop(&S); // ->eps prechod ked namam ziadany parameter
            priznak = nic;


            s_con->context = ARG_NUM_CHECK;  ////kontrola spravneho poctu zadanych argumentov
            sem_check (s_con);
        }
        else
        {
            fprintf(stderr, "2: Syntakticka chyba. Ocakaval som pravu zatvorku.\n");
            trashDestroy(2);
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


                void *spracADDR = spracuj(actToken.stav, actToken.data); // kluc
                NaplnInstr( I_IDENT, NULL, spracADDR, NULL );


                if(priznak == write)
                {
                    NaplnInstr( I_WRITE_IDE, NULL, NULL, NULL );
                }

                s_con->act_id = actToken.data;   //ulozenie id argumentu
                s_con->context = ID_ARG_CHECK;  //kontext kontroly id parametra pri volani funkcie 
                sem_check (s_con);
            }
            else if (actToken.stav == S_INTEGER)
            {
                myPop(&S);
                myPushMul(&S, 2, S_INTEGER, LL_NSPLIST);



                void *spracADDR = spracuj(actToken.stav, actToken.data); //
                void *spracID = spracuj(S_IDENTIFIKATOR, actToken.data);


                tStav *TIPSTAV = mymalloc(sizeof(tStav));
                *TIPSTAV = actToken.stav;

                NaplnInstr( I_PREC, spracID, spracADDR, TIPSTAV );

                if(priznak == write)
                {
                    NaplnInstr( I_WRITE_INT, NULL, NULL, NULL );
                }


                s_con->act_type = S_INTEGER;   //kontrola hodnoty typu integer
                s_con->context = DV_ARG_CHECK;
                sem_check (s_con);
            }
            else if (actToken.stav == S_DOUBLE)
            {
                myPop(&S);
                myPushMul(&S, 2, S_DOUBLE, LL_NSPLIST);

                void *spracADDR = spracuj(actToken.stav, actToken.data);
                void *spracID = spracuj(S_IDENTIFIKATOR, actToken.data);

                tStav *TIPSTAV = mymalloc(sizeof(tStav));
                *TIPSTAV = actToken.stav;

                NaplnInstr( I_PREC, spracID, spracADDR, TIPSTAV );

                if(priznak == write)
                {
                    NaplnInstr( I_WRITE_DOU, NULL, NULL, NULL );
                }


                s_con->act_type = S_DOUBLE;   //kontrola hodnoty typu double
                s_con->context = DV_ARG_CHECK;
                sem_check (s_con);
            }
            else if (actToken.stav == S_RETEZEC)
            {
                myPop(&S);
                myPushMul(&S, 2, S_RETEZEC, LL_NSPLIST);

                void *spracADDR = spracuj(actToken.stav, actToken.data);

                tStav *TIPSTAV = mymalloc(sizeof(tStav));
                *TIPSTAV = actToken.stav;

                NaplnInstr( I_PREC, spracADDR, spracADDR, TIPSTAV );

                if(priznak == write)
                {
                    NaplnInstr( I_WRITE_STR, NULL , NULL, NULL );
                }


                s_con->act_type = S_RETEZEC;  //kontrola hodnoty typu string
                s_con->context = DV_ARG_CHECK;
                sem_check (s_con);
            }
            else if (actToken.stav == S_KLIC_TRUE) //////////////////////////////////////////// CO ROBIT ????? BOL/TRU
            {

                myPop(&S);
                myPushMul(&S, 2, S_KLIC_TRUE, LL_NSPLIST);


                s_con->act_type = S_KLIC_BOOLEAN;   //kontrola hodnoty typu boolean
                s_con->context = DV_ARG_CHECK;
                sem_check (s_con);
            }
            else if (actToken.stav == S_KLIC_FALSE)
            {

                myPop(&S);
                myPushMul(&S, 2, S_KLIC_FALSE, LL_NSPLIST);
      
                
                s_con->act_type = S_KLIC_BOOLEAN;   //kontrola hodnoty typu boolean
                s_con->context = DV_ARG_CHECK;
                sem_check (s_con);
            }

            else
            {
                fprintf(stderr, "2: Syntakticka chyba. Ocakaval som dalsi parameter. \n");
                trashDestroy(2);
            }


        }
        else if(actToken.stav == S_PRAVA_ZAVORKA)
        {
            // generuj instrukcie spust funkciu
            //s_con->c_fun // tu mam ulozeny identifikator ff ***********************************OVERIT

            //----generrr--------------------------------------------------------------------------------

            // tu sa ukoncuje SPLIST ... zatim by mala nasledovat ')'
            // daj mi to na POMOCNY zasobnik az po zarazku // koli spravnemu poradiu param funkcie
            // generujem instrukciu DAJ NA POMOCNY ZASOBNIK az po zarazku ak sa spusta funkcia
            void *spracADDR = spracuj(S_IDENTIFIKATOR, s_con->c_fun);



            if (priznak != write) // !!IBA!! ak mam UZIVATELSKU FUNKCIU !!
            {

                NaplnInstr( I_RUNFUN_PARAM, NULL, NULL, NULL );


                       //----------------------------gener-------------------------------------------------------
            // generujeme zaciatocne veci pri spusteni FUNKCIE-kopirovanie tabulki, nahodenie na FRAMESTACK
            // + dalej generujeme pomocnu zarazku na zasobnik -- vsetko v 1 instrukcii

            NaplnInstr( I_RUNFUN_COPY, NULL , spracADDR, NULL ); // podla kluca

            //-----gener--------------------------------------------------------------------------------


                

         


                // ULOZ ADRESU FUNKCIE SAVE na zasobnik skokov
                NaplnInstr( I_JMP_S, NULL , NULL, NULL );

                // spusti FUNKCIU.........................................................................................SPUSTI FUNKCIU!!

                NaplnInstr( I_JMPF_KEY, NULL , spracADDR, NULL ); // KLUC zo zapametania

                // I PRIRAD
                NaplnInstr( I_PRIRAD, NULL , NULL, NULL ); // priradime

                // stack del FRAME
                NaplnInstr( I_FRAME_DEL, NULL , NULL, NULL );

            }


            //-----geber---------------------------------------------------------------------------------

            myPop(&S); // ->eps prechod:: ked pride prave jeden parameter
            priznak = nic;

            
            s_con->context = ARG_NUM_CHECK;  ////kontrola spravneho poctu zadanych argumentov
            sem_check (s_con);
        }


        else
        {
            fprintf(stderr, "2: Syntakticka chyba. Ocakaval som pravu zatvorku.\n");
            trashDestroy(2);
        }
        break;
    }

    case LL_E:
    {
        myPop(&S); 

        //-----------------------------------------------------  
        switch (actToken.stav)
		{
			case S_KRAT: 					{ break; }
			case S_DELENO:					{ break; }
			case S_PLUS: 					{ break; }
			case S_MINUS:					{ break; }
			case S_MENSI: 					{ break; }
			case S_VETSI:					{ break; }
			case S_MENSI_NEBO_ROVNO: 		{ break; }
			case S_VETSI_NEBO_ROVNO:		{ break; }
			case S_ROVNO: 					{ break; }
			case S_NEROVNO:					{ break; }
			case S_LEVA_ZAVORKA: 			{ break; }
			case S_PRAVA_ZAVORKA:			{ break; }
			case S_IDENTIFIKATOR: 			{ break; }
			case S_DOLAR:					{ break; }
			case S_INTEGER:					{ break; }
			case S_DOUBLE: 					{ break; }
			case S_RETEZEC:					{ break; }
			case S_KLIC_TRUE:				{ break; }
			case S_KLIC_FALSE:				{ break; }
			default: 						
			{
				fprintf(stderr, "2: Syntakticka chyba. Ocakaval som PODMIENKU!\n" );
				trashDestroy(2);
				break; 
			} 
			 	
		}
		//-----------------------------------------------------     


        if ((isVyraz(s_con)) == 0)   // ak podmienka plati vytvorime instrukciu
        {
            NaplnInstr(I_PODM_JMP, myaTop(&IFJMP), NULL, NULL);
        }
        else
        {
            fprintf(stderr, "2: Syntakticka chyba. Neplatny vyraz / podmienka.\n");
            trashDestroy(2);
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
    NaplnInstr(I_NICNEROBA, NULL,NULL,NULL);
    InstrStart(&INSTR_PASKA);

    zarazka  = mymalloc(sizeof(char)); // zarazka adresova

    hash_insert_it(GLOBFRAME, "begin", 444); // navestie mainu

    bool ERRO = true;
    stack_init(&S);

    astack_init(&IFJMP);

    myPush(&S, EOF); // zarazka $$$$
    myPush(&S, LL_INIT);

    actToken = get_token();

    fun_init ();       //inicializacia vstavanych funkcii

    tSem_context s_con;           //instancia struktury nesie semanticky kontext a jeho atributy
    s_con.scope = LOCAL;        //na zaciatku je lokalny kontext
    s_con.context = G_VAR_DEC;    //na zaciatku zdrojaku je kontext deklaracii glob. premennych


    while(actToken.stav != S_END_OF_FILE) // dokym som neni na konci suboru
    {
        if (myTop(&S) == EOF && actToken.stav != S_END_OF_FILE)
        {
            //CHYBA!!!! na zasobniku bol uz iba EOF ale my sme este nedocitali subor
            fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Uz som neocakaval nic, ale nieco prislo. ", actToken.radek+1);
            trashDestroy(2); // syntakticka chyba
        }


        if (myTop(&S) >= LL_INIT && myTop(&S) <= LL_FUNCEND )  // TERMINAL/NETERMINAL
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

                    NaplnInstr( I_READ, NULL, spracADDR, NULL );
                    priznak=nic;
                }
//--------------------------------------------------------------------------------------------------------//


                myPop(&S);	// odstranime z vrcholu zasobnika
                //free(actToken.data); // free

                
                if (actToken.stav == S_KLIC_DO || actToken.stav == S_KLIC_THEN)
                {
                    s_con.context = BOOL_CHECK;
                    sem_check (&s_con);
                }



                if ( actToken.stav == S_IDENTIFIKATOR )
                {
                    //ulozenie id funkcie pri deklaracii
                    if (Id_sign == rem_id)
                    {
                        Id_sign = for_id;                      //reset signum
                        s_con.act_fun = actToken.data;   //save actual id of function
                        sem_check (&s_con);


                        // *********** TOTO JE INSTRUKCIA***ZACIATOK************************************
                        // posielam ID funkcie koli ADRESE ZACIATKU FUN NA instrukc paske
                        //--gener-dole-----------------------------

                        // generuj kluc na zaciatok funkcie TU zacina funkcia
                        void *spracADDR = spracuj(S_IDENTIFIKATOR, actToken.data); // slo by to aj inac ale pre jednotnost..
                        // NaplnInstr(I_JMPF_KEY_S, NULL, spracADDR, NULL); //

                        // uloz adresu KDE sa skoci  do GLOB TAB... cize navestie funkcie

                        // vytvorime si navestie pomocou NIC-NEROBY :D
                        NaplnInstr(I_NICNEROBA, NULL,NULL,NULL);


                        list_element prvok;
                        prvok = (list_element)(hash_adress(GLOBFRAME, spracADDR));
                        void* pomad;
                        pomad = InstrDajPoz(&INSTR_PASKA);


                        ((prvok)->start) = pomad;


                        //--gener hore---------------------------------------------------

                        //budeme alokovat miesto pre navratovu hodnotu podla kluca  toto je ALOKACIA NAVRATOVEJ HODNOTY!!
                        NaplnInstr( I_IDENT, NULL, spracADDR, NULL );
                        NaplnInstr( I_IDENT, NULL, spracADDR, NULL );

                        // *********** TOTO JE INSTRUKCIA***KONIEC************************************



                    }
                    
                    //zapamata sa id 
                    if (Id_sign == rem_pid)
                    {
                        Id_sign = for_pid;
                        s_con.act_id = actToken.data;
                    }

                    //overi sa deklaracia id premennej zadanej pre readln
                    if (Id_sign == readln)
                    {
                        Id_sign = noreadln;
                        s_con.act_id = actToken.data;
                        s_con.context = ID_READ_CHECK;
                        sem_check (&s_con);
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
                //syntakt chyba
                fprintf(stderr, "2: Syntakticka chyba na riadku '%d'. Ocakaval som ", actToken.radek+1);
                whattoken(myTop(&S));
                fprintf(stderr, "\n");
                trashDestroy(2);

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
            //syntakt chyba
            fprintf(stderr, "2: Syntakticka chyba. Koniec suboru, malo nasledovat  ");
            whattoken(myTop(&S));
            fprintf(stderr, "\n");
            trashDestroy(2);

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


list* get_local (char* id)    //vrati referenciu na lokalnu tabulku podla zadaneho id funkcie
{
    return ((list_element) hash_adress(GLOBFRAME, id))->ref;
}

void sem_check (tSem_context* s_con)
{
    unsigned arg_type;  //typ parametra, ktory sa ziska podla pozicie

    switch (s_con->context)
    {
    case G_VAR_DEC:              //kontext deklaracii glob. premennych
        if ( hash_search (GLOBFRAME, s_con->act_id) == CONTAINS )   //error if var exists
        {
            fprintf (stderr, "chyba pri deklaraci globalnej premennej \'%s\' na riadku '%d'\n",
                     s_con->act_id, actToken.radek+1);
            trashDestroy (semanticka_chyba_pri_deklaraci);
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
        if (hash_is_sign (GLOBFRAME, s_con->act_fun) == FORWARDED)
        {
            hash_set_sign (GLOBFRAME, s_con->act_fun, DEFINED);
            f_counter--;  //pocet forwardnutych funkcii sa znizi
            break;
        }

        //nastala chyba pri definicii alebo deklaracii funkcie
        fprintf (stderr, "chyba pri deklaraci funkcie \'%s\' na riadku '%d'\n",
                 s_con->act_fun, actToken.radek+1);
        trashDestroy (semanticka_chyba_pri_deklaraci);


    case FUNC_ARG_DEC:        //kontext deklaracie a overovania argumentov funkcie
        //definicia funkcie, overuju sa argumenty
        if (hash_is_sign (GLOBFRAME, s_con->act_fun) == DEFINED)
        {
            //chyba ak argument bebol deklarovany alebo nesedi typ alebo pozicia
            if ( hash_search (get_local (s_con->act_fun), s_con->act_id) == NOCONTAINS ||
                 hash_return_type (get_local (s_con->act_fun), s_con->act_id) != s_con->act_type ||
                 get_arg_num (get_local (s_con->act_fun), s_con->act_id) != ++arg_num )
            {
                fprintf (stderr, "chyba argumentu \'%s\' funkcie \'%s\' na riadku '%d'\n",
                         s_con->act_id, s_con->act_fun, actToken.radek+1);
                trashDestroy(semanticka_chyba_pri_deklaraci);
            }
        }

        //funkcia nebola deklarovana, ulozia sa argumenty
        else
        {
            //chyba ak parameter uz existuje
            if (hash_search (get_local (s_con->act_fun), s_con->act_id) == CONTAINS)
            {
                fprintf (stderr, "chyba pri deklaraci argumentu \'%s\' funkcie \'%s\'"
                         " na riadku '%d'\n", s_con->act_id, s_con->act_fun, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
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
        if (hash_is_sign (GLOBFRAME, s_con->act_fun) == DEFINED)
        {
            //ak sa typ pri deklaracii nezhoduje s typom pri definicii
            if (hash_return_type (get_local (s_con->act_fun), s_con->act_fun) != s_con->act_type)
            {
                fprintf (stderr, "chyba pri defenici typu navratovej hodnoty funkcie \'%s\'"
                         " na riadku '%d'\n", s_con->act_fun, actToken.radek+1);
                trashDestroy(semanticka_chyba_pri_deklaraci);
            }

            //funkcia bola deklarovana a pri definicii bol zadany nespravny pocet argumentov
            if (get_arg_num (GLOBFRAME, s_con->act_fun) != arg_num)
            {
                fprintf (stderr, "nespravny pocet argumentov pri defenici funkcie '%s'"
                         " na riadku '%d'\n", s_con->act_fun, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
            }
        }
        else { //funkcia nebola deklarovana, uklada sa typ
          hash_insert_it (get_local(s_con->act_fun), s_con->act_fun, s_con->act_type);
          //arg_num = 0 ===> navratova hodnota sa netyka argumentov
          set_arg_num (get_local (s_con->act_fun), s_con->act_fun, 0);
        }

        arg_num = 0; //reset pocitadla argumentov, suvisi s FUNC_ARG_DEC
        break;


    case L_VAR_DEC:      //kontext deklaracie lokalnych premennych
        //chyba, ak premenna alebo argument alebo navratova hodnota funkcie existuje
        if ( hash_search (get_local (s_con->act_fun), s_con->act_id) == CONTAINS)
        {
            fprintf (stderr, "chyba pri deklaraci lokalnej premennej \'%s\' funkcie \'%s\'"
                     " na riadku '%d'\n", s_con->act_id, s_con->act_fun, actToken.radek+1);
            trashDestroy (semanticka_chyba_pri_deklaraci);
        }

        //ulozenie premennej do LTS
        hash_insert_it (get_local (s_con->act_fun), s_con->act_id, s_con->act_type);
        //arg_num = 0 ===> lokalna premenna sa netyka argumentov
        set_arg_num (get_local (s_con->act_fun), s_con->act_id, 0);
        break;


    case RET_VAL_CHECK:            //kontrola priradenia id := funkcia()
        //lokalny rozsah premennych
        if (s_con->scope == LOCAL)
        {
            //premenna nie je deklarovana lokalne
            if ( hash_search (get_local (s_con->act_fun), s_con->l_id) != CONTAINS )
            {
                //premenna nie je deklarovana ani globalne - chyba
                if ( hash_search (GLOBFRAME, s_con->l_id) != CONTAINS )
                {
                    fprintf (stderr, "premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                             s_con->l_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_pri_deklaraci);
                }
                else   //premenna je deklarovana globalne
                {
                    //overenie typov premennej a navratovej hodnoty funkcie
                    if ( hash_return_type (GLOBFRAME, s_con->l_id) !=
                            hash_return_type (get_local (s_con->c_fun), s_con->c_fun) )
                    {
                        fprintf (stderr, "typova nekompatibilita medzi funkciou \'%s\'"
                                 " a premennou\'%s\' na riadku '%d'\n",
                                 s_con->c_fun, s_con->l_id, actToken.radek+1);
                        trashDestroy (semanticka_chyba_typove_kompatibility);
                    }
                }
            }
            else    //premenna je deklarovana lokalne
            {
                //overenie typov premennej a navratovej hodnoty funkcie
                if ( hash_return_type (get_local (s_con->act_fun), s_con->l_id) !=
                        hash_return_type (get_local (s_con->c_fun), s_con->c_fun) )
                {
                    fprintf (stderr, "typova nekompatibilita medzi funkciou \'%s\'"
                             " a premennou \'%s\'\n na riadku '%d'",
                             s_con->c_fun, s_con->l_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_typove_kompatibility);
                }
            }
        }

        //globalny rozsah premennych
        if (s_con->scope == GLOBAL)
        {
            //globalna premenna nebola deklarovana
            if (hash_search (GLOBFRAME, s_con->l_id) != CONTAINS)
            {
                fprintf (stderr, "globalna premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                         s_con->l_id, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
            }
            //premenna bola deklarovana, overenie typu premennej a navratovej hodnoty funkcie
            if ( hash_return_type (GLOBFRAME, s_con->l_id) !=
                    hash_return_type (get_local (s_con->c_fun), s_con->c_fun) )
            {
                fprintf (stderr, "typova nekompatibilita medzi funkciou \'%s\' a premennou \'%s\'"
                         " na riadku '%d'\n",
                         s_con->c_fun, s_con->l_id, actToken.radek+1);
                trashDestroy (semanticka_chyba_typove_kompatibility);
            }
        }
        break;


    case ID_ARG_CHECK:            //kontrola id parametra pri volani funkcie
        
        arg_num++;  //zvysenie pocitadla pozicie parametrov
       
        //ziskanie typu parametra podla jeho pozicie pri definicii ak nejde o write
        if (s_con->write_sgn != WRITE)
           arg_type = arg_numSearch (get_local (s_con->c_fun), arg_num);
        
        //ak sa typ parametra v LTS nenajde a ak nemame write
        if ( ! arg_type && s_con->write_sgn != WRITE ) {
          fprintf (stderr, "prekroceny pocet argumentov pri volani funkcie '%s'"
                   " na riadku '%d'\n",
                   s_con->c_fun, actToken.radek+1);
          trashDestroy (semanticka_chyba_typove_kompatibility);
        }
          
        //lokalny rozsah premennych
        if (s_con->scope == LOCAL)
        {
            //premenna nie je deklarovana lokalne
            if ( hash_search (get_local (s_con->act_fun), s_con->act_id) != CONTAINS )
            {
                //premenna nie je deklarovana ani globalne - chyba
                if ( hash_search (GLOBFRAME, s_con->act_id) != CONTAINS )
                {
                    fprintf (stderr, "premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                             s_con->act_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_pri_deklaraci);
                }
                else   //premenna je deklarovana globalne
                {
                    //overenie typov premennej a argumentu funkcie na spravnej pozicii
                    if ( (hash_return_type (GLOBFRAME, s_con->act_id) != arg_type &&
                         s_con->write_sgn != WRITE) ||
                         hash_return_type (GLOBFRAME, s_con->act_id) == F_ID)
                    {
                        fprintf (stderr, "typova nekompatibilita v argumente \'%s\'"
                                 " na riadku '%d'\n",
                                 s_con->act_id, actToken.radek+1);
                        trashDestroy (semanticka_chyba_typove_kompatibility);
                    }
                }
            }
            else    //premenna je deklarovana lokalne
            {
                //overenie typov premennej a argumentu funkcie na spravnej pozicii
                if ( hash_return_type (get_local (s_con->act_fun), s_con->act_id) != arg_type &&
                     s_con->write_sgn != WRITE)
                {
                    fprintf (stderr, "typova nekompatibilita pri volani funkcie \'%s\'"
                             " v argumente \'%s\' na riadku '%d'\n",
                             s_con->c_fun, s_con->act_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_typove_kompatibility);
                }
            }
        }

        //globalny rozsah premennych
        if (s_con->scope == GLOBAL)
        {
            //globalna premenna nebola deklarovana
            if (hash_search (GLOBFRAME, s_con->act_id) != CONTAINS)
            {
                fprintf (stderr, "globalna premenna \'%s\' na riadku '%d' nebola deklarovana\n", 
                         s_con->act_id, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
            }

            //premenna bola deklarovana, overenie typu premennej a argumentu na spravnej pozicii
            if ( (hash_return_type (GLOBFRAME, s_con->act_id) != arg_type &&
                 s_con->write_sgn != WRITE) ||
                 hash_return_type (GLOBFRAME, s_con->act_id) == F_ID )
            {
                fprintf (stderr, "typova nekompatibilita v argumente \'%s\' na riadku '%d'\n",
                         s_con->act_id, actToken.radek+1);
                trashDestroy (semanticka_chyba_typove_kompatibility);
            }
        }
    break;

    case DV_ARG_CHECK:    //kontrola argumentu zadaneho priamou hodnotou
      
        arg_num++;  //zvysenie pocitadla pozicie parametrov
       
        if (s_con->write_sgn == WRITE)  //v pripade write sa argumenty neoveruju
          break;

        //ziskanie typu parametra podla jeho pozicie 
        arg_type = arg_numSearch (get_local (s_con->c_fun), arg_num);
        
        //ak sa typ parametra v LTS nenajde a ak nemame write
        if ( ! arg_type ) {
          fprintf (stderr, "prekroceny pocet argumentov pri volani funkcie '%s' na riadku '%d'\n",
                   s_con->c_fun, actToken.radek+1);
          trashDestroy (semanticka_chyba_typove_kompatibility);
        }

        //overi sa typ hodnoty s typom argumentu na spravnej pozicii
        if (s_con->act_type != arg_type) {
          fprintf (stderr, "zadany zly typ hodnoty pre argument na pozicii %d"
                   " vo funkcii '%s' na riadku '%d'\n",
                   arg_num, s_con->c_fun, actToken.radek+1);
          trashDestroy (semanticka_chyba_typove_kompatibility);
        }

    break;


    case ARG_NUM_CHECK: //kontrola spravneho poctu zadanych argumentov pri volani funkcie
      
      //nedostatocny pocet argumentov
      if ( s_con->write_sgn != WRITE && 
           get_arg_num (GLOBFRAME, s_con->c_fun) != arg_num ) {
        fprintf (stderr, "nedostatocny pocet argumentov pri volani funkcie '%s' na riadku '%d'\n",
                 s_con->c_fun, actToken.radek+1);
        trashDestroy (semanticka_chyba_typove_kompatibility);
      }

      if ( s_con->write_sgn == WRITE ) {
        s_con->write_sgn = NOWRITE;     //reset priznaku pre write
      }

      arg_num = 0; //reset pocitadla argumentov
    
    break;


    case ID_READ_CHECK:   // kontrola premennej id zadanej do readln()

        //lokalny rozsah premennych
        if (s_con->scope == LOCAL)
        {
            //premenna nie je deklarovana lokalne
            if ( hash_search (get_local (s_con->act_fun), s_con->act_id) != CONTAINS )
            {
                //premenna nie je deklarovana ani globalne - chyba
                if ( hash_search (GLOBFRAME, s_con->act_id) != CONTAINS )
                {
                    fprintf (stderr, "premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                             s_con->act_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_pri_deklaraci);
                }
                //premenna je deklarovana globalne
                else
                {
                    //overnie typu premennej na typ boolean
                    if (hash_return_type (GLOBFRAME, s_con->act_id) == S_KLIC_BOOLEAN ||
                        hash_return_type (GLOBFRAME, s_con->act_id) == F_ID)
                    {
                      fprintf (stderr, "argument '%s' prikazu readln () na riadku '%d'"
                               " je nespravneho typu\n",
                               s_con->act_id, actToken.radek+1);
                      trashDestroy (semanticka_chyba_typove_kompatibility);
                    }
                }
            }
            //premenna je deklarovana lokalne
            else
            {
                 //overnie typu premennej na typ boolean
                 if (hash_return_type (get_local(s_con->act_fun), s_con->act_id) == S_KLIC_BOOLEAN)
                 {
                    fprintf (stderr, "argument '%s' prikazu readln () na riadku '%d'"
                             " nesmie byt typu boolean\n",
                             s_con->act_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_typove_kompatibility);
                 }
            }
        }

        //globalny rozsah premennych
        if (s_con->scope == GLOBAL)
        {
            //globalna premenna nebola deklarovana
            if (hash_search (GLOBFRAME, s_con->act_id) != CONTAINS)
            {
                fprintf (stderr, "globalna premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                         s_con->act_id, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
            }
            //overnie typu premennej na typ boolean
            if (hash_return_type (GLOBFRAME, s_con->act_id) == S_KLIC_BOOLEAN ||
                hash_return_type (GLOBFRAME, s_con->act_id) == F_ID) 
            {
              fprintf (stderr, "argument '%s' prikazu readln () na riadku '%d'"
                       " je nespravneho typu\n",
                       s_con->act_id, actToken.radek+1);
              trashDestroy (semanticka_chyba_typove_kompatibility);
            }
        }

    break;


    case ID_DEC_CHECK:

        //lokalny rozsah premennych
        if (s_con->scope == LOCAL)
        {
            //premenna nie je deklarovana lokalne
            if ( hash_search (get_local (s_con->act_fun), s_con->act_id) != CONTAINS )
            {
                //premenna nie je deklarovana ani globalne - chyba
                if ( hash_search (GLOBFRAME, s_con->act_id) != CONTAINS )
                {
                    fprintf (stderr, "premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                             s_con->act_id, actToken.radek+1);
                    trashDestroy (semanticka_chyba_pri_deklaraci);
                }
                //premenna je deklarovana globalne
                else
                {
                  //ulozenie typu 
                  s_con->act_type = hash_return_type (GLOBFRAME, s_con->act_id);
                }
            }
            //premenna je deklarovana lokalne
            else
            {
                 //ulozenie typu
                 s_con->act_type = hash_return_type (get_local (s_con->act_fun), s_con->act_id);
            }
        }

        //globalny rozsah premennych
        if (s_con->scope == GLOBAL)
        {
            //globalna premenna nebola deklarovana
            if (hash_search (GLOBFRAME, s_con->act_id) != CONTAINS)
            {
                fprintf (stderr, "globalna premenna \'%s\' na riadku '%d' nebola deklarovana\n",
                         s_con->act_id, actToken.radek+1);
                trashDestroy (semanticka_chyba_pri_deklaraci);
            }
            //ulozenie typu
            s_con->act_type = hash_return_type (GLOBFRAME, s_con->act_id);
        }

    break;


    case EXP_RET_CHECK:  //kontrola typov LHS a RHS pri vyrazoch
      
      if (s_con->l_type != s_con->act_type) {  //ak LHS nema rovnaky typ ako RHS
        fprintf (stderr, "premenna '%s' nie je typovo komatibilna s vyrazom na riadku '%d'\n",
                 s_con->l_id, actToken.radek+1);
        trashDestroy (semanticka_chyba_typove_kompatibility);
      }

    break;


    case BOOL_CHECK:   //kontrola, ci je vyraz typu boolean

      if (s_con->act_type != S_KLIC_BOOLEAN) {  //ak vyraz nie je typu boolean
        fprintf (stderr, "vyraz na riadku '%d' musi byt typu boolean\n", actToken.radek+1);
        trashDestroy (semanticka_chyba_typove_kompatibility);
      }

    break;
    }
}

void fun_init() {
  
//funkcia length:
  hash_insert_it (GLOBFRAME, "length", F_ID);  //nastavenie id funkcie jeho typu
  hash_insert_func (GLOBFRAME, "length");  //vytvorenie LTS funkcie
  set_arg_num (GLOBFRAME, "length", 1); //pocet parametrov funkcie
  hash_insert_it (get_local("length"), "length", S_INTEGER); //navratova hodnota
  set_arg_num (get_local ("length"), "length", 0); //navr. hodn. sa netyka argumentov

  //argumenty funkcie:
  hash_insert_it (get_local("length"), "s", S_RETEZEC); //vytvorenie parametra
  set_arg_num (get_local ("length"), "s", 1);  //1. parameter


//funkcia copy:
  hash_insert_it (GLOBFRAME, "copy", F_ID);  //nastavenie id funkcie jeho typu
  hash_insert_func (GLOBFRAME, "copy");  //vytvorenie LTS funkcie
  set_arg_num (GLOBFRAME, "copy", 3); //pocet parametrov funkcie
  hash_insert_it (get_local("copy"), "copy", S_RETEZEC); //navratova hodnota
  set_arg_num (get_local ("copy"), "copy", 0); //navr. hodn. sa netyka argumentov

  //argumenty funkcie:
  hash_insert_it (get_local("copy"), "s", S_RETEZEC); //vytvorenie parametra
  set_arg_num (get_local ("copy"), "s", 1);  //1. parameter
  
  hash_insert_it (get_local("copy"), "i", S_INTEGER); //vytvorenie parametra
  set_arg_num (get_local ("copy"), "i", 2);  //2. parameter

  hash_insert_it (get_local("copy"), "n", S_INTEGER); //vytvorenie parametra
  set_arg_num (get_local ("copy"), "n", 3);  //3. parameter
  
    
//funkcia find:
  hash_insert_it (GLOBFRAME, "find", F_ID);  //nastavenie id funkcie jeho typu
  hash_insert_func (GLOBFRAME, "find");  //vytvorenie LTS funkcie
  set_arg_num (GLOBFRAME, "find", 2); //pocet parametrov funkcie
  hash_insert_it (get_local("find"), "find", S_INTEGER); //navratova hodnota
  set_arg_num (get_local ("find"), "find", 0); //navr. hodn. sa netyka argumentov

  //argumenty funkcie:
  hash_insert_it (get_local("find"), "s", S_RETEZEC); //vytvorenie parametra
  set_arg_num (get_local ("find"), "s", 1);  //1. parameter
  
  hash_insert_it (get_local("find"), "search", S_RETEZEC); //vytvorenie parametra
  set_arg_num (get_local ("find"), "search", 2);  //2. parameter

 
//funkcia sort:
  hash_insert_it (GLOBFRAME, "sort", F_ID);  //nastavenie id funkcie jeho typu
  hash_insert_func (GLOBFRAME, "sort");  //vytvorenie LTS funkcie
  set_arg_num (GLOBFRAME, "sort", 1); //pocet parametrov funkcie
  hash_insert_it (get_local("sort"), "sort", S_RETEZEC); //navratova hodnota
  set_arg_num (get_local ("sort"), "sort", 0); //navr. hodn. sa netyka argumentov

  //argumenty funkcie:
  hash_insert_it (get_local("sort"), "s", S_RETEZEC); //vytvorenie parametra
  set_arg_num (get_local ("sort"), "s", 1);  //1. parameter 
}
