#include <stdio.h>
#include "str.h"
#include "stable.h"
#include "ilist.h"
#include "scaner.h"
#include "parser.h"
#include "interpret.h"

#define FILE_ERROR 5

int main(int argc, char** argv)
{
   FILE *f;
   if (argc == 1)
   {
      printf("Neni zadan vstupni soubor\n");
      return FILE_ERROR;
   }
   if ((f = fopen(argv[1], "r")) == NULL)
   {
      printf("Soubor se nepodarilo otevrit\n");
      return FILE_ERROR;
   }   
   setSourceFile(f);
   
   tSymbolTable ST; 
   tableInit(&ST); // inicializace tabulky symbolu
  
   tListOfInstr instrList;
   listInit(&instrList); // inicializace seznamu instrukci
  
   int result;
   result = parse(&ST, &instrList); // provedeme syntaktickou analyzu
   
   switch (result)
   {
     case LEX_ERROR:
     case SYNTAX_ERROR:
     case SEM_ERROR:
       // nastala chyba v prubehu prekladu
       tableFree(&ST);
       listFree(&instrList);
       fclose(f);
       return -result;
     break;
     // jinak probehlo vse v poradku, muzeme provadet kod
   } 
   
   // provedeme interpretaci
   inter(&ST, &instrList);

   tableFree(&ST);
   listFree(&instrList);
   fclose(f);
   return 0;
}
