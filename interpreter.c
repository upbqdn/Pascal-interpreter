/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

#include "header.h"
#include "astack.h"
#include "ial.h"
#include "instrlist.h"
#include "interpreter.h"



typedef enum MOZNOSTI
{
    IDENT_IDENT = 0,
    IDENT_PREC,
    PREC_IDENT,
    PREC_PREC,

} tMoznosti;



tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA
list *GLOBFRAME;
astack FRAME;

astack aS; // pomocny zasobnik adries pre interpreter

tMoznosti EXTRA = IDENT_IDENT;


int inter()    //AKCIA, KDE,int *PRVA,int *DRUHA//
{
    astack_init(&aS);

    void* zarazka = malloc(sizeof(char));
    if (zarazka == NULL)
    {
    	// chyba alokacie
			trashDestroy();
			exit(99);
    }
   

    myaPush(&aS, zarazka);
    //printf("....NAHADZUJEM HLAAAVNUUUUU ZARAZKU\n");


    // ----------------alokacia pomocnych premennych roznych TIPOV------------//
    void *c_integer = malloc(sizeof(int));
     if (c_integer == NULL)
    {
    	// chyba alokacie
			trashDestroy();
			exit(99);
    }
    
    

    void *c_double = malloc(sizeof(float));
    if (c_double == NULL)
    {
    	// chyba alokacie
			trashDestroy();
			exit(99);
    }
    
    	
    

    void *c_bool = malloc(sizeof(bool));
    if (c_bool == NULL)
    {
    	// chyba alokacie
			trashDestroy();
			exit(99);
    }
    

    void *c_string = malloc(sizeof(char));
    if (c_string == NULL)
    {
    	// chyba alokacie
			trashDestroy();
			exit(99);
    }
    
    	

    
    // niekedy odalokujeme


    tStav TIP;
    tInstrukcia *Instr; // lokalna instrukcia
    InstrStart(&INSTR_PASKA); // aktivovat instrukcnu pasku

    while(INSTR_PASKA.Aktivna != NULL) // dokedy neprideme na koniec INSTRUKCNEJ PASKY...
    {
        Instr = DajInstr(&INSTR_PASKA);

        switch(Instr->AKCIA)
        {

//-----------------------------ak pride int,double,boolean,string..-----------------------------------//
        case I_PREC:
        {
            // aby sme vedeli ako je to adresovane .. ci to ide z tabulky symbolov alebo priamo
            if (EXTRA == IDENT_PREC)
            {
                EXTRA = PREC_PREC;
            }
            else
            {
                EXTRA = IDENT_PREC;
            }


            myaPush(&aS, Instr->ADDR_PRVA);

            TIP = *(tStav *)(Instr->ADDR_DRUHA); // na tejto adrese musi byt napr. S_INTEGER

            break;
        }

//------------------------------------ak pride IDENTIFIKATOR------------------------------------------//
        case I_IDENT:
        {
            // aby sme vedeli ako je to adresovane .. ci to ide z tabulky symbolov alebo priamo
            if (EXTRA == IDENT_PREC)
            {
                EXTRA = PREC_IDENT;
            }


            list *TOPFRAME;
            TOPFRAME = myaTop(&FRAME);    // pozrieme sa na najvrchnejsi FRAME //

            list_element prvok;
            prvok = (list_element)(hash_adress(TOPFRAME, Instr->ADDR_PRVA));

            if (prvok == NULL) // hladame v GLOBAL
            {
                prvok = (list_element)(hash_adress(GLOBFRAME, Instr->ADDR_PRVA));
            }

            TIP = (*prvok).type;

            myaPush(&aS, &(prvok)->ref); // vlozime na zasobnik adresu chlieviku v ktorom je adresa na danu polozku dat

            break;
        }


//----------------------------PRIRADENIE----------------------------------------------------------------//
        case I_PRIRAD:
        {
            if (TIP == S_INTEGER)
            {
                int pomoc1 = (*(int*)(myaTop(&aS)));
                myaPop(&aS);

                (*(int*)(myaTop(&aS))) = pomoc1 ;
                myaPop(&aS);
            }
            else if (TIP == S_DOUBLE)
            {
                float pomoc1 = (*(float*)(myaTop(&aS)));
                myaPop(&aS);

                (*(float*)(myaTop(&aS))) = pomoc1 ;
                myaPop(&aS);
            }
            else if (TIP == S_RETEZEC)
            {


                void* pomAddr1; // to co vytiahneme zo zasobnika ako NAJVRCHNEJSIE
                // ci bolo ADRESOVANE z tabulky symbolov alebo priamo
                if( (EXTRA == IDENT_PREC) || (EXTRA == PREC_PREC) )
                {
                    pomAddr1 = myaTop(&aS);  // PREC (PRIAMO)

                }
                else
                {
                    pomAddr1 = (*(void **)(myaTop(&aS)));  // IDENTIFIKATOR (CEZ TABULKU SYMBOLOV)

                }


                myaPop(&aS); // mozme zahodit z zasobniku

                // berieme zo zasobniku dalsiu ADRESU - KDE  to ulozit (adresacia cez TABULKU SYMBOLOV)
                void* pomAddr2 = (*(void **)(myaTop(&aS)));
                int dlzka = strlen(((char**)pomAddr1));


                void *pom = realloc( pomAddr2 ,( ((sizeof(char))*dlzka)+2 ) );     //..realok
                void **kk = (myaTop(&aS));
                *kk = pom; // ked sa nahodu zmeni adresa po alokacii tak ulozime...

                strcpy( ((char*) pom) , ((char*)pomAddr1)  );

                myaPop(&aS); // pop 2 adresa

                // ci bolo ADRESOVANE z tabulky symbolov alebo priamo
                EXTRA = IDENT_IDENT;

            }
            else if ( TIP == S_KLIC_TRUE || S_KLIC_FALSE )
            {
                bool pomoc1 = (*(bool*)(myaTop(&aS)));
                myaPop(&aS);

                (*(bool*)(myaTop(&aS))) = pomoc1 ;
                myaPop(&aS);
            }

            break;
        }



//-----------------------------------------------ALLOC pripady-----------------------------------------//
        case I_ALLOC_INT:
        {
            if(  ((*(void **)(myaTop(&aS))) = malloc(sizeof(int))) == NULL)
            {
            	// chyba alokacie
        		trashDestroy();
       			exit(99);
            }


            myaPop(&aS);

            break;
        }


        case I_ALLOC_DOU:
        {

            if(  ((*(void **)(myaTop(&aS))) = malloc(sizeof(float))) == NULL)
            {
            	// chyba alokacie
        		trashDestroy();
       			exit(99);
            }

            myaPop(&aS);

            break;
        }



        case I_ALLOC_BOO:
        {

           if( ((*(void **)(myaTop(&aS))) = malloc(sizeof(bool))) == NULL)
           {
           		// chyba alokacie
        		trashDestroy();
       			exit(99);
           }

            myaPop(&aS);

            break;
        }

        case I_ALLOC_STR:
        {

            if( ((*(void **)(myaTop(&aS))) = malloc(sizeof(char))) == NULL)
            {
            	// chyba alokacie
        		trashDestroy();
       			exit(99);
            }

            myaPop(&aS);

            break;
        }


//-----------------------------------------------READ-----------------------------------------------//

        case I_READ:
        {
            list *TOPFRAME;
            TOPFRAME = myaTop(&FRAME);

            list_element prvok;
            prvok = (list_element)(hash_adress(TOPFRAME, Instr->ADDR_PRVA));

            if (prvok == NULL) // hladame v GLOBAL
            {
                prvok = (list_element)(hash_adress(GLOBFRAME, Instr->ADDR_PRVA));
            }

            switch ((*prvok).type)
            {
            case S_INTEGER:
            {
                scanf("%d", &(prvok)->ref );

                break;
            }

            case S_DOUBLE:
            {
                scanf("%f", &(prvok)->ref );

                break;
            }

            case S_RETEZEC:
            {
                char znak;
                int dlzkastringu = 0;
                while((znak=getchar())!= '\n' )
                {
                    (prvok)->ref = realloc( (prvok)->ref , (sizeof(char)*dlzkastringu+2));

                    //  OK  *(char *) &((prvok)->ref)[dlzkastringu] = znak;  magia takto to ide :D
                    *(char *) &((prvok)->ref)[dlzkastringu] = znak;
                    dlzkastringu++;

                }

                *(char *) &((prvok)->ref)[dlzkastringu+1] = '\0';  // pridame znak ukoncenia retazca

                break;
            }
            default:
            {
                //chybka
                break;
            }

            }

            break;
        }





//------------------------------------------------WRITE pripady-------------------------------------------//
        case I_WRITE_IDE:
        {
            // treba zistit akeho je tipu //
            switch(TIP)
            {
            case S_INTEGER:
            {
                printf("%d", *(int*)myaTop(&aS) );
                break;
            }

            case S_DOUBLE:
            {
                printf("%f", *(float*)myaTop(&aS)  );
                break;
            }

            case S_RETEZEC:
            {

                void* pomADKA = (*(void **)(myaTop(&aS)));
                printf("%s",  (char*) pomADKA );

                break;
            }

            case S_KLIC_FALSE:
            case S_KLIC_TRUE:
            {
                printf("%d", *(bool*)myaTop(&aS)  );
                break;
            }

            default:
            {
                break;
            }

            }

            break;
        }



        case I_WRITE_INT:
        {
            printf("%d", myaTop(&aS) );
            myPop(&aS);

            break;
        }

        case I_WRITE_DOU:
        {
            printf("%f", myaTop(&aS) );
            myPop(&aS);

            break;
        }

        case I_WRITE_STR:
        {
            printf("%s", (char*) myaTop(&aS) );
            myPop(&aS);

            break;
        }

        case I_WRITE_BOO:
        {
            printf("%d", myaTop(&aS) );
            myPop(&aS);

            break;
        }


//-------------------------------------nasleduje +,-,*,--------------------------------------------//
        case I_PLUS:
        {
            if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(int*)c_integer = b + a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                myaPush(&aS, c_integer);


            }
            else if (TIP == S_DOUBLE)
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(float*)c_double = b + a;
                myaPush(&aS, c_double);

            }
            else if (TIP == S_RETEZEC)
            {
                void* pomAddr1;
                void* pomAddr2;

                if(EXTRA == PREC_PREC )  // 2x PRIAMO adresacia  PRECENDECNA
                {
                    pomAddr1 = myaTop(&aS);
                    myaPop(&aS);
                    pomAddr2 = myaTop(&aS);
                    myaPop(&aS);
                }
                else if (EXTRA == IDENT_IDENT) // 2x IDENTIFIKATOR
                {
                    pomAddr1 = (*(void **)(myaTop(&aS))) ;
                    myaPop(&aS);
                    pomAddr2 = (*(void **)(myaTop(&aS))) ;
                    myaPop(&aS);
                }
                else if (EXTRA == PREC_IDENT)  // PRECENDECNA +  IDENTIFIKATOR
                {
                    pomAddr1 = (*(void **)(myaTop(&aS))) ;
                    myaPop(&aS);
                    pomAddr2 = myaTop(&aS);
                    myaPop(&aS);
                }
                else if (EXTRA == IDENT_PREC) // IDENTIFIKATOR + PRECENDECKA
                {
                    pomAddr1 = myaTop(&aS);
                    myaPop(&aS);
                    pomAddr2 = (*(void **)(myaTop(&aS)));
                    myaPop(&aS);
                }

                //int dlzka = (  (strlen((*(char**)pomAddr1))) +  (strlen((*(char**)pomAddr2)))    );
                int dlzka = strlen( pomAddr2 ) + strlen( pomAddr1 );

                void* pomAddr3 = malloc(((sizeof(char))*dlzka)+1);
                if (pomAddr3 == NULL) // chyba alokacie
                {
                    // chyba alokacie
        			trashDestroy();
       				exit(99);
                }

                strcpy(  pomAddr3, ((char**)pomAddr2)  );

                c_string = realloc(   c_string  , ( ((sizeof(char))*dlzka)+2 ) );
                if (c_string == NULL) // chyba alokacie
                {
                    // chyba alokacie
        			trashDestroy();
       				exit(99);
                }
                strcpy(  c_string, ((char**)pomAddr3)  );
                strcat(  c_string, ((char**)pomAddr1)  );
                myaPush(&aS, c_string);
            }

            EXTRA = IDENT_PREC;

            break;  // PLUS KONIEC
        }


//-------------------------------------------------MINUS-----------------------------------------------//
        case I_MINUS:
        {
            if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(int*)c_integer = b - a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                myaPush(&aS, c_integer);
            }
            else if (TIP == S_DOUBLE)
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(float*)c_double = b - a;
                myaPush(&aS, c_double);
            }

            break;  // KONIEC MINUS
        }


//---------------------------------------------KRAT-------------------------------------------------------//
        case I_KRAT:
        {
            if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(int*)c_integer = b * a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                myaPush(&aS, c_integer);
            }
            else if (TIP == S_DOUBLE)
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(float*)c_double = b * a;
                myaPush(&aS, c_double);

            }

            break;
        }


//---------------------------------------------------DELENO----------------------------------------------//
        case I_DELENO:
        {
            if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
            {
                int a = *(int *)(myaTop(&aS));

                if (a == 0)
                {
                    //DELENIE NULOV
                    return 0; // zle zle zle fuj fuj
                }

                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(int*)c_integer = b / a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                myaPush(&aS, c_integer);
            }
            else if (TIP == S_DOUBLE)
            {
                float a = *(float *)(myaTop(&aS)) ;

                if (a == 0)
                {
                    //DELENIE NULOV
                    return 0; // zle zle zle fuj fuj
                }

                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                *(float*)c_double = b / a;
                myaPush(&aS, c_double);

            }

            break;
        }



        //----------------------------------DOPLNIL MATUSKA - TREBA SKONTROLOVAT----------------------------//
// NUTNO ZKONTROLOVAT
        case I_ROVNO:
        {
            if (TIP == S_INTEGER) /*int = int*/
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a == b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_DOUBLE) /* real = real */
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a == b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_RETEZEC) /* string = string */
            {
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);
                if (strcmp (pomAddr1, pomAddr2))
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }// bude nasledovat porovnani dvou hodnot typu boolean
            break;
        }

        case I_NEROVNO:
        {
            if (TIP == S_INTEGER) /*int <> int*/
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a != b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_DOUBLE) /* real <> real */
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a != b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_RETEZEC) /* string <> string */
            {
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);
                if (!(strcmp (pomAddr1, pomAddr2)))
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }// bude nasledovat porovnani dvou hodnot typu boolean
            break;
        }

        case I_VETSI:
        {
            if (TIP == S_INTEGER) /*int > int*/
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a > b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_DOUBLE) /* real > real */
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a > b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_RETEZEC) /* string > string */
            {
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);
                if ((strcmp (pomAddr1, pomAddr2)) > 0)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }// bude nasledovat porovnani dvou hodnot typu boolean
            break;
        }

        case I_MENSI:
        {
            if (TIP == S_INTEGER) /*int < int*/
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a < b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_DOUBLE) /* real < real */
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a < b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_RETEZEC) /* string < string */
            {
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);
                if ((strcmp (pomAddr1, pomAddr2)) < 0)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }// bude nasledovat porovnani dvou hodnot typu boolean
            break;
        }

        case I_VETSIROVNO:
        {
            if (TIP == S_INTEGER) /*int >= int*/
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a >= b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_DOUBLE) /* real >= real */
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a >= b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_RETEZEC) /* string >= string */
            {
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);
                if ((strcmp (pomAddr1, pomAddr2)) >= 0)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }// bude nasledovat porovnani dvou hodnot typu boolean
            break;
        }

        case I_MENSIROVNO:
        {
            if (TIP == S_INTEGER) /*int <= int*/
            {
                int a = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                int b = *(int *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a <= b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_DOUBLE) /* real <= real */
            {
                float a = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                float b = *(float *)(myaTop(&aS)) ;
                myaPop(&aS);
                if (a <= b)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }
            if (TIP == S_RETEZEC) /* string <= string */
            {
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);
                if ((strcmp (pomAddr1, pomAddr2)) <= 0)
                {
                    *(bool*)c_bool = true;
                }
                else
                {
                    *(bool*)c_bool = false;
                }
                myaPush(&aS, c_bool);
            }// bude nasledovat porovnani dvou hodnot typu boolean
            break;
        }

//-----------------------------------------DOPLNIL MATUSKA----------------------------------------



        default:
        {
            break;
        }


        }

        InstrDalsia(&INSTR_PASKA);
    }

    // tuto bude dealokacia pomocnych premenych

    return 1; // VSETKO OK
}
