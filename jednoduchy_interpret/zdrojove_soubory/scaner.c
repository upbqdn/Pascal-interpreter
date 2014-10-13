// lexikalni analyzator
#include <stdio.h>
#include <ctype.h>
#include "str.h"
#include "scaner.h"

// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
  source = f;
}

int getNextToken(string *attr)
// hlavni funkce lexikalniho analyzatoru
{
   int state = 0;
   int c;
   // vymazeme obsah atributu a v pripade identifikatoru
   // budeme postupne do nej vkladat jeho nazev
   strClear(attr);
   while (1)
   {     
     // nacteni dalsiho znaku
     c = getc(source);
           
     switch (state)
     {
       case 0:
       // zakladni stav automatu 
         if (isspace(c))
         // bila mista - ignorovat
	    state = 0;
	 else
	 if (c == '<')
	 // komentar
	    state = 1;
	 else
	 if (isalpha(c))
	 // identifikator nebo klicove slovo
	 {    
	    strAddChar(attr, c);
	    state = 2;
	 }
	 else
	 if (c == '+')
	 // operator ++
	    state = 3;
	 else
	 if (c == '-')
	 // operator --
	    state = 4;
	 else
	 if (c == '{') return LEFT_VINCULUM;
	 else
	 if (c == '}') return RIGHT_VINCULUM;
	 else
	 if (c == ';') return SEMICOLON;
	 else
	 if (c == EOF) return END_OF_FILE;
	 else
	 return LEX_ERROR;
       break;
	 

       case 1:
       // komentar
         if (c == '>') state = 0; 
         else 
         // komentar pokracuje libovolnymi znaky, zustan ve stavu 1,
         // ale kontroluj, zda neprijde EOF (neukonceny komentar)
         if (c == EOF) return LEX_ERROR;
       break;
       
       
       case 2:
       // identifikator nebo klicove slovo
         if (isalnum(c))
	 // identifikator pokracuje
	    strAddChar(attr, c);
	 else
	 // konec identifikatoru
	 {
       	    ungetc(c, source); // POZOR! Je potreba vratit posledni nacteny znak
	    
	    // kontrola, zda se nejedna o klicove slovo
	    if (strCmpConstStr(attr, "setzero") == 0) return SETZERO;
	    else 
	    if (strCmpConstStr(attr, "read") == 0) return READ;
	    else 
	    if (strCmpConstStr(attr, "write") == 0) return WRITE;
	    else 
	    if (strCmpConstStr(attr, "while") == 0) return WHILE;
	    else 
	    // jednalo se skutecne o identifikator
	       return ID;
          }
       break;   
       

       case 3:
       // pokracovani operatoru ++
         if (c == '+') return INC;
         else
         return LEX_ERROR;
       break;
       
       
       case 4:
       // pokracovani operatoru --
         if (c == '-') return DEC;
         else
         return LEX_ERROR;
       break;
       
    }
  }
}
