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
#include "htable.h"
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
  myaPUSH(&aS, zarazka);


  // ----------------alokacia pomocnych premennych roznych TIPOV------------//
  void *c_integer = malloc(sizeof(int));
  void *c_double = malloc(sizeof(float));
  void *c_boolean = malloc(sizeof(bool));
  void *c_string = malloc(sizeof(char));
  // niekedy odalokujeme


 tStav TIP;
 tInstrukcia Instr; // lokalna instrukcia
 InstrStart(&INSTR_PASKA); // aktivovat instrukcnu pasku

  while(INSTR_PASKA->Aktivna != NULL) // dokedy neprideme na koniec INSTRUKCNEJ PASKY...
  {
    Instr = DajInstr(&INSTR_PASKA);
      
        switch(Instr.AKCIA) 
        {

          case I_VAR_ZARAZKA:
          {
            myaPUSH(&aS, zarazka);
            break;
          }

          //============ak pride int,double,boolean,string...===============//
          case I_PREC:
               myaPUSH(&aS, Instr.ADDR_PRVA);
               TIP = Instr.ADDR_DRUHA; // na tejto adrese musi byt napr. S_INTEGER
              
            break;
           

            //============ak pride IDENTIFIKATOR===============//
            case I_IDENT:


                list TOPFRAME = myaTop(&FRAME);    // fiko magic // 
              
                list_element* prvok = hash_adress(TOPFRAME, Instr.ADDR_PRVA);
                if (prvok == NULL) // hladame v GLOBAL
                {
                prvok = hash_adress(GLOBFRAME, Instr.ADDR_PRVA);
                }


                TIP = prvok.type;
                myaPUSH(&aS, prvok.data);
                      
            break;

            case I_PRIRAD:
               if (TIP == S_INTEGER)
               {  
               // namiesto ??? sa musi vyriesit to aby sa dalo z KEY pristupit priamo na HODNOTU
               int pomoc1 = (*(int*)(myaTop(&aS)));
               myaPop(&aS);

               (*(int*)(myaTop(&aS)) = pomoc1 ;
               myaPop(&aS);
               }

               else if (TIP == S_DOUBLE)
               {  
               float pomoc1 = (*(float*)(myaTop(&aS)));
               myaPop(&aS);

               (*(float*)(myaTop(&aS)) = pomoc1 ;
               myaPop(&aS);
               }

               else if (TIP == S_STRING)
               {
                void* pomAddr1 = myaTop(&aS);  // co chceme ulozit
                myaPop(&aS);
                void* pomAddr2 = myaTop(&aS); // kam to ulozit
                myaPop(&aS);

                int dlzka = strlen((*(char**)pomAddr1);
                pomAddr2 = realloc(((sizeof(char))*dlzka)+1);    //......................................................realok
                if (pomAddr2 == NULL) // chyba alokacie
                {
                    return NULL;
                }
                strcpy( (*(char**)pomAddr2 , (*(char**)pomAddr1  );
                
                }

               else if ( TIP == S_KLIC_TRUE || S_KLIC_FALSE )
               {  
               bool pomoc1 = (*(bool*)(myaTop(&aS)));
               myaPop(&aS);

               *(bool*)(myaTop(&aS)) = pomoc1 ;
               myaPop(&aS);
               }

            break;

//>>>>>>>>>>>>>>>>>>>>>>>>>>--ALLOC pripady--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<// 
            case I_ALLOC_INT:
            {
              while(myaTop(&aS) != zarazka)
              {
                 *(void *)(myaTop(&aS)) = malloc(sizeof(int));
                 myaPop(&aS);
              }
              myaPop(&aS); // vyhodime zarazku !
              break;
            }
                  
            

            case I_ALLOC_DOU:
            {
              while(myaTop(&aS) != zarazka)
              {
                 *(void *)(myaTop(&aS)) = malloc(sizeof(float));
                 myaPop(&aS);
              }
              myaPop(&aS); // vyhodime zarazku !
              break;
            }

            

            case I_ALLOC_BOO:
            {
              while(myaTop(&aS) != zarazka)
              {
                 *(void *)(myaTop(&aS)) = malloc(sizeof(bool));
                 myaPop(&aS);
              }
              myaPop(&aS); // vyhodime zarazku !
              break;
            }

            case I_ALLOC_STR:
            {
              while(myaTop(&aS) != zarazka)
              {
                 *(void *)(myaTop(&aS)) = malloc(sizeof(char));
                 myaPop(&aS);
              }
              myaPop(&aS); // vyhodime zarazku !
              break;
            } 

            

         


//>>>>>>>>>>>>>>>>>>>>>>>>>>--WRITE pripady--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<// 
            case I_WRITE_IDE:
      {
                   // treba zistit akeho je tipu //
              list TOPFRAME = myaTop(&FRAME);    // fiko magic // 
              
            list_element* prvok = hash_adress(TOPFRAME, Instr.ADDR_PRVA);
                if (prvok == NULL) // hladame v GLOBAL
                {
                  prvok = hash_adress(GLOBFRAME, Instr.ADDR_PRVA);
                }


            TIP = prvok.type;

            switch(TIP)
            {
              case S_INTEGER:
                {
                  printf("%d", *(int*)prvok.ref );
                  break;
                }

                case S_DOUBLE:
                 {
                  printf("%f", *(float*)prvok.ref );
                  break;
                }

                case S_STRING:
                 {
                  printf("%s", *(char**)prvok.ref );
                  break;
                }

                case S_KLIC_FALSE:
                case S_KLIC_TRUE:
                 {
                  printf("%d", *(bool*)prvok.ref );
                  break;
                }

            }

                    
              break;
          }


            case I_WRITE_INT:
            {
              printf("%d", *(int*)Instr.ADDR_PRVA );
              break;
            }

            case I_WRITE_DOU:
             {
              printf("%f", *(float*)Instr.ADDR_PRVA );
              break;
            }

            case I_WRITE_STR:
             {
              printf("%s", *(char**)Instr.ADDR_PRVA );
              break;
            }

            case I_WRITE_BOO:
             {
              printf("%d", *(bool*)Instr.ADDR_PRVA );
              break;
            }


  //>>>>>>>>>>>>>>>>>>>>>>--nasleduje +,-,*,/--<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<// 
            case I_PLUS:
               if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 *(int*)c_integer = b + a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                 myaPUSH(&aS, c_integer);
                 

               }

               else if (TIP == S_DOUBLE)
               {
                 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 *(float*)c_double = b + a;   
                 myaPUSH(&aS, c_double);
                
               }


               else if (TIP == S_STRING)
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

                c_string = realloc(((sizeof(char))*dlzka)+1);    //......................................................realok
                if (c_string == NULL) // chyba alokacie
                {
                    return NULL;
                }
                strcpy(  c_string, (*(char**)pomAddr3)  );
                strcat(  c_string, (*(char**)pomAddr1)  );
                myaPUSH(&aS, c_string);
               }
          
          break;  // PLUS KONIEC


          case I_MINUS:
               if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 *(int*)c_integer = b - a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                 myaPUSH(&aS, c_integer);
                 

               }

               else if (TIP == S_DOUBLE)
               {
                 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 *(float*)c_double = b - a;   
                 myaPUSH(&aS, c_double);
                
               }

               // mozno bollen / string uvidime ...
          
          break;  // KONIEC MINUS



          case I_KRAT:
               if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 *(int*)c_integer = b * a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                 myaPUSH(&aS, c_integer);
                 

               }

               else if (TIP == S_DOUBLE)
               {
                 float a = *(float *)(myaTop(&aS) ;  
                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 *(float*)c_double = b * a;   
                 myaPUSH(&aS, c_double);
                
               }

          
          break;   


            case I_DELENO:
               if (TIP == S_INTEGER)  // adresa vs cislo toto treba opravit
               {

                 int a = *(int *)(myaTop(&aS) ;
                 if (a == 0)
                   {
                    //DELENIE NULOV
                    return 0; // zle zle zle fuj fuj
                   }  

                 myaPop(&aS);
                 int b = *(int *)(myaTop(&aS) ; 
                 myaPop(&aS);
                 
                 *(int*)c_integer = b / a;   // spocitaju sa hodnoty a priradia sa do medzi vysledku
                 myaPUSH(&aS, c_integer);
                 

               }

               else if (TIP == S_DOUBLE)
               {
                 float a = *(float *)(myaTop(&aS) ;  
                 if (a == 0)
                   {
                    //DELENIE NULOV
                    return 0; // zle zle zle fuj fuj
                   }  

                 myaPop(&aS);
                 float b = *(float *)(myaTop(&aS) ; 
                 myaPop(&aS);   
                 *(float*)c_double = b / a;   
                 myaPUSH(&aS, c_double);
                
               }
          
          break;  

        }

        InstrDalsia(&INSTR_PASKA);
  }

  // tuto bude dealokacia pomocnych premenych 
}
