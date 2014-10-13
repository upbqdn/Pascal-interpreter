#include <stdio.h>
#include "str.h"
#include "stable.h"
#include "ilist.h"
#include "scaner.h"
#include "parser.h"


tSymbolTable *table;// globalni promenna uchovavajici tabulku symbolu
tListOfInstr *list; // globalni promenna uchovavajici seznam instrukci
int token;          // globalni promenna, ve ktere bude ulozen aktualni token
string attr;        // globalni promenna, ve ktere bude ulozen atribut tokenu

int counterVar = 1;

void generateVariable(string *var)
// generuje jedinecne nazvy identifikatoru
// nazev se sklada ze znaku $ nasledovanym cislem
// postupne se tu generuji prirozena cisla a do nazvu promenne se ukladaji
// v reverzovanem poradi - na funkcnost to nema vliv, ale je jednodussi implementace

{
  strClear(var);
  strAddChar(var, '$');
  int i;
  i = counterVar;
  while (i != 0)
  {
    strAddChar(var, (char)(i % 10 + '0'));
    i = i / 10;
  }
  counterVar ++;
} 

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

// ==================================================================
// jednotlive funkce odpovidajici jednotlivym nonterminalum gramatiky
// ==================================================================

int stat(); // deklarace funkce, nebot bude volana drive, nez je definovana


int declrList()
{
  switch (token)
  {
    case ID:
      // pravidlo <declrList> -> "ID" ";" <declrList>

      // resime ID, ulozime do tabulky symbolu
      // pokud tento ID jiz byl deklarovan, jedna se o semantickou chybu
      if (tableInsert(table, &attr, TYPE_INT) == 1) return SEM_ERROR;
      
      // pozadame o dalsi token, kterym musi byt ";"
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      if (token != SEMICOLON) return SYNTAX_ERROR;
      
      // pozadame o dalsi token a rekurzivne zavolame funkci declrList
      // cele pravidlo probehne v poradku, pokud nam tato funkce vrati uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      return declrList();    
    break;
      

    case LEFT_VINCULUM:
      // pravidlo <declrList> -> "{"  
 
      // pouze pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;	    
      return SYNTAX_OK;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int statList()
{
  int result;
  switch (token)
  {
    case RIGHT_VINCULUM:
      // pravidlo <statList> -> "}"

      // pouze pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      return SYNTAX_OK;
    break;    

    case WHILE:
    case READ:
    case WRITE:
    case SETZERO:
    case INC:
    case DEC:
      // pravidlo <statList> -> <stat> <statList>
      
      // nejprve zavolame funkci stat
      result = stat();
      // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
      if (result != SYNTAX_OK) return result;
      // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce statList
      return statList();
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}


int stat()
{
  tData *variableInfo;

  int result;
  int firstTermInRule;

  switch (token)
  {
    case READ:
    case WRITE:
    case SETZERO:
    case INC:
    case DEC:
      // tato pravidla maji stejny kontext, budeme je zpracovavat zaroven,
      // akorat vygenerujeme prislusnou instrukci
    
      // pravidlo <stat> -> "READ" "ID" ";"
      // pravidlo <stat> -> "WRITE" "ID" ";"
      // pravidlo <stat> -> "SETZERO" "ID" ";"
      // pravidlo <stat> -> "INC" "ID" ";"
      // pravidlo <stat> -> "DEC" "ID" ";"
      
      firstTermInRule = token;  // musime si ulozit tento token, abychom na zaver vedeli, jakou instrukci generovat
      
      // pozadame o dalsi token, kterym musi byt ID
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      // pokud token neni ID, jedna se o syntaktickou chybu
      if (token != ID) return SYNTAX_ERROR;
      // musime zkontrolovat, zda je jiz identifikator v tabulce symbolu
      // pokud ne, jedna se o semantickou chybu
      if ((variableInfo = tableSearch(table, &attr)) == NULL) return SEM_ERROR;      
      // nagenerujeme patricnou instrukci
      switch (firstTermInRule)
      {
        case READ:    generateInstruction(I_READ,    (void*) variableInfo, NULL, NULL); break;
        case WRITE:   generateInstruction(I_WRITE,   (void*) variableInfo, NULL, NULL); break;
        case SETZERO: generateInstruction(I_SETZERO, (void*) variableInfo, NULL, NULL); break;
        case INC:     generateInstruction(I_INC,     (void*) variableInfo, NULL, NULL); break;
        case DEC:     generateInstruction(I_DEC,     (void*) variableInfo, NULL, NULL); break;
      }
      
      // pozadame o dalsi token, kterym musi byt strednik
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      if (token != SEMICOLON) return SYNTAX_ERROR;
      
      // pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      return SYNTAX_OK;
    break;	 
        
    case WHILE:
      // pravidlo <stat> -> "WHILE" "ID" "{" <statList>
      
      // nagenerujeme instrukci noveho navesti a uchovame jeji ukazatel
      generateInstruction(I_LAB, NULL, NULL, NULL);
      void *addrOfLab1;
      addrOfLab1 = listGetPointerLast(list);
      
      // pozadame o dalsi token, kterym musi byt ID
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      // pokud token neni ID, jedna se o syntaktickou chybu
      if (token != ID) return SYNTAX_ERROR;
      // musime zkontrolovat, zda je jiz identifikator v tabulce symbolu
      // pokud ne, jedna se o semantickou chybu
      if ((variableInfo = tableSearch(table, &attr)) == NULL) return SEM_ERROR;      
      
      // nagenerujeme pomocnou promennou, kterou ulozime do tabulky symbolu
      // testovat jeji existenci nemusime, nebot jeji nazev je jedinecny
      string newVar;
      strInit(&newVar);
      generateVariable(&newVar);
      tableInsert(table, &newVar, TYPE_INT);
      tData *newVariableInfo;
      newVariableInfo = tableSearch(table, &newVar);
      strFree(&newVar);
      
      // nagenerujeme instrukci, ktera provede negaci drive ziskaneho identifikatoru
      // a vysledek ulozi do nove promenne      
      generateInstruction(I_NOT, (void*) variableInfo, NULL, (void*) newVariableInfo);
      
      // nagenerujeme instrukci podmineneho skoku a ulozime jeji adresu,
      // protoze pozdeji bude potreba doplnit adresu skoku
      void *addrOfIfGoto;
      generateInstruction(I_IFGOTO, (void*) newVariableInfo, NULL, NULL); 
      addrOfIfGoto = listGetPointerLast(list);
      
      // zazadame o dalsi token, kterym musi byt slozena zavorka
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      if (token != LEFT_VINCULUM) return SYNTAX_ERROR;
      
      // zazadame o dalsi token a rekurzivne zavolame funkci stat
      if ((token = getNextToken(&attr)) == LEX_ERROR) return LEX_ERROR;
      result = statList();
      if (result != SYNTAX_OK) return result;
      
      // nagenerujeme instrukci skoku
      generateInstruction(I_GOTO, NULL, NULL, (void*) addrOfLab1);

      // nagenerujeme instrukci druheho navesti
      generateInstruction(I_LAB, NULL, NULL, NULL);
      void *addrOfLab2;
      addrOfLab2 = listGetPointerLast(list);

      // jiz zname adresu druheho navesti, muzeme tedy nastavit adresu
      // do drive generovane instrukce podmineneho skoku
      listGoto(list, addrOfIfGoto);
      tInstr *data;
      data = listGetData(list);
      data->addr3 = addrOfLab2;
      
      return SYNTAX_OK;
    break;
    
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;      
}    
    
    
int program()
{
  int result;
  switch (token)
  {
    case ID:
    case LEFT_VINCULUM:    
      // pravidlo <program> -> <declrList> <statList> <END_OF_FILE>
      result = declrList();
      if (result != SYNTAX_OK) return result;
      result = statList();
      if (result != SYNTAX_OK) return result;
      
      // POZOR! Nezapomenout testovat, zda nasleduje konec souboru.
      // Pri oponenuti teto veci by zde mohly pokracovat nejake nesmysly, ktere by se
      // v ramci syntakticke analyzy jiz nezpracovavaly a program by se tvaril, ze je OK
      if (token != END_OF_FILE) return SYNTAX_ERROR;
      
      // nagenerujeme instrukci konce programu
      generateInstruction(I_STOP, NULL, NULL, NULL);
      
      return SYNTAX_OK;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}


int parse(tSymbolTable *ST, tListOfInstr *instrList)
{
  int result;
  table = ST;
  list = instrList;
  strInit(&attr);
  if ((token = getNextToken(&attr)) == LEX_ERROR)
     // nastala chyba jiz pri nacteni prvniho lexemu
     result = LEX_ERROR;
  else
     result = program();
  strFree(&attr);
  return result;
}
