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



tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA
list *GLOBFRAME;
astack FRAME;

astack aS; // pomocny zasobnik adries pre interpreter


int inter()    //AKCIA, KDE,int *PRVA,int *DRUHA//
{
    astack_init(&aS);
    void* zarazka = malloc(sizeof(char));
    myaPush(&aS, zarazka);
    printf("....NAHADZUJEM HLAAAVNUUUUU ZARAYKU\n");


    // ----------------alokacia pomocnych premennych roznych TIPOV------------//
    void *c_integer = malloc(sizeof(int));
    void *c_double = malloc(sizeof(float));
   // void *c_boolean = malloc(sizeof(bool));
    void *c_string = malloc(sizeof(char));
    // niekedy odalokujeme


    tStav TIP;
    tInstrukcia *Instr; // lokalna instrukcia
    InstrStart(&INSTR_PASKA); // aktivovat instrukcnu pasku

    while(INSTR_PASKA.Aktivna != NULL) // dokedy neprideme na koniec INSTRUKCNEJ PASKY...
    {
        Instr = DajInstr(&INSTR_PASKA);

        switch(Instr->AKCIA)
        {

        case I_VAR_ZARAZKA:
        {
            printf("....NAHADZUJEM  ZARAYKU\n");
            myaPush(&aS, zarazka);
            break;
        }

        //============ak pride int,double,boolean,string...===============//
        case I_PREC:
        {
            //printf("NICO ZAUJIMAVE..........................................................\n");
            //printf("AZARAZKA..... %p \n", zarazka );
            //printf("ADRESA NA ZASOBNIKU pred pushom %p \n", myaTop(&aS) );
            //printf("ADRESA PRED ZASOBNIKU %p \n", Instr->ADDR_PRVA );

           // printf("SKUSKA.....>>%s<< \n", Instr->ADDR_PRVA );


            myaPush(&aS, Instr->ADDR_PRVA);
            //printf("ADRESA NA ZASOBNIKU %p \n", myaTop(&aS) );
            TIP = *(tStav *)(Instr->ADDR_DRUHA); // na tejto adrese musi byt napr. S_INTEGER
printf("????  INSTR_PREC OK\n");
            break;
        }

        //============ak pride IDENTIFIKATOR===============//
        case I_IDENT:
        {

            list *TOPFRAME;
            TOPFRAME = myaTop(&FRAME);    // fiko magic //

            list_element prvok;
            prvok = (list_element)(hash_adress(TOPFRAME, Instr->ADDR_PRVA));

            if (prvok == NULL) // hladame v GLOBAL
            {
                prvok = (list_element)(hash_adress(GLOBFRAME, Instr->ADDR_PRVA));
            }

            TIP = (*prvok).type;
            
            myaPush(&aS, &(prvok)->ref); 
            printf("POMOCNA>>>>>>>AD222>>%p\n" , myaTop(&aS)  );
            printf("POMOCNA>>>>>>>AD222>>%p\n" , (prvok)->ref  );
            
printf("????INSTR_IDENT OK\n");
            break;
        }

        case I_PRIRAD:
        {
            if (TIP == S_INTEGER)
            {
                // namiesto ??? sa musi vyriesit to aby sa dalo z KEY pristupit priamo na HODNOTU
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
                printf("spravne\n");
                void* pomAddr1 = myaTop(&aS);  // co chceme ulozit
                printf("TOP>>%s\n" , myaTop(&aS) );
                printf("TOP>>%s\n" , pomAddr1 );
                myaPop(&aS);
                void* pomAddr2 = (*(void **)(myaTop(&aS))); // kam to ulozit
                myaPop(&aS);

                printf("ADDRESA2 je >>%p\n" , pomAddr2  );
                
                printf("spravne\n");
                int dlzka = strlen(((char**)pomAddr1));
                printf("spravne %d \n", dlzka);
                printf("HMMM>>AD2>>%p\n" , pomAddr2  );
                pomAddr2 =  realloc( pomAddr2 ,( ((sizeof(char))*dlzka)+2 ) );     //..realok
                printf("spravne\n");
                if (pomAddr2 == NULL) // chyba alokacie
                {
                    printf("cele zle\n");
                    return NULL;
                }

                

                strcpy( ((char*)pomAddr2) , ((char*)pomAddr1)  );
                printf("HMMM>>AD2>>%p\n" , pomAddr2  );
                printf("AD2>>%s\n" , (char*) pomAddr2  );
                printf("POMOCNIK>>AD2>>%p\n" , pomAddr2  );

            }
            else if ( TIP == S_KLIC_TRUE || S_KLIC_FALSE )
            {
                bool pomoc1 = (*(bool*)(myaTop(&aS)));
                myaPop(&aS);

                (*(bool*)(myaTop(&aS))) = pomoc1 ;
                myaPop(&aS);
            }
printf("????  INSTR_PRIRAD OK\n");
            break;
        }

//>>>>>>>>>>>>>>>>>>>>>>>>>>--ALLOC pripady--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
        case I_ALLOC_INT:
        {
            
            while(myaTop(&aS) != zarazka)
            {
                (*(void **)(myaTop(&aS))) = malloc(sizeof(int));
                myaPop(&aS);
            }
            myaPop(&aS); // vyhodime zarazku !

            printf("????INSTR_ALLOC_INT OK\n");
            break;

        }



        case I_ALLOC_DOU:
        {
            while(myaTop(&aS) != zarazka)
            {
                (*(void **)(myaTop(&aS))) = malloc(sizeof(float));
                myaPop(&aS);
            }
            myaPop(&aS); // vyhodime zarazku !
            printf("????INSTR_ALLOC_DOU OK\n");
            break;
        }



        case I_ALLOC_BOO:
        {
            while(myaTop(&aS) != zarazka)
            {
                (*(void **)(myaTop(&aS))) = malloc(sizeof(bool));
                myaPop(&aS);
            }
            myaPop(&aS); // vyhodime zarazku !
            printf("????INSTR_ALLOC_BOOL OK\n");
            break;
        }

        case I_ALLOC_STR:
        {
            while(myaTop(&aS) != zarazka)
            {
                (*(void **)(myaTop(&aS))) = malloc(sizeof(char));
                myaPop(&aS);
            }
            myaPop(&aS); // vyhodime zarazku !

            printf("????INSTR_ALLOC_RETEZEC OK\n");
            break;
        }


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>--READ--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//

        case I_READ:
        {
            list *TOPFRAME;
            TOPFRAME = myaTop(&FRAME);    // fiko magic //

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
                		znak=getchar();
                		&(prvok)->ref=realloc( &(prvok)->ref2 , (sizeof(char)*dlzkastringu+1));
                		(prvok)->ref[dlzkastringu]=znak;
                		dlzkastringu++;
                        
                		

                      
                	}
                	(prvok)->ref[dlzkastringu]= '\0';
                	

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





//>>>>>>>>>>>>>>>>>>>>>>>>>>--WRITE pripady--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
        case I_WRITE_IDE:
        {
            // treba zistit akeho je tipu //
            
            printf("????????????????????????????????????????????\n");

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
                printf("idem pisat IDE string\n");
                printf("%s", *(char**)myaTop(&aS)   );
                break;
            }

            case S_KLIC_FALSE:
            case S_KLIC_TRUE:
            {
                printf("%d", *(bool*)myaTop(&aS)  );
                break;
            }

            default:
                break;

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
           // printf("ADD %p\n", *(void *) (Instr->ADDR_PRVA) );
            printf("%s",  myaTop(&aS) );
            myPop(&aS);
            break;
        }

        case I_WRITE_BOO:
        {
            printf("%d", myaTop(&aS) );
            myPop(&aS);
            break;
        }


        //>>>>>>>>>>>>>>>>>>>>>>--nasleduje +,-,*,/--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
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
                void* pomAddr1 = myaTop(&aS);
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS);
                myaPop(&aS);

                int dlzka = (  (strlen((*(char**)pomAddr1))) +  (strlen((*(char**)pomAddr2)))    );
                void* pomAddr3 = malloc(((sizeof(char))*dlzka)+1);
                if (pomAddr3 == NULL) // chyba alokacie
                {
                    return NULL;
                }
                strcpy(  pomAddr3, (*(char**)pomAddr2)  );

                c_string = realloc(   c_string  ,    ( ((sizeof(char))*dlzka)+1 )     )     ;    //......................................................realok
                if (c_string == NULL) // chyba alokacie
                {
                    return NULL;
                }
                strcpy(  c_string, (*(char**)pomAddr3)  );
                strcat(  c_string, (*(char**)pomAddr1)  );
                myaPush(&aS, c_string);
            }

            break;  // PLUS KONIEC
        }

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


        default:
            break;

        }

        InstrDalsia(&INSTR_PASKA);
    }

    // tuto bude dealokacia pomocnych premenych

    return 1; // VSETKO OK
}
