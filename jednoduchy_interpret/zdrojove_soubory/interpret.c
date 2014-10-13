#include <stdio.h>
#include "str.h"
#include "stable.h"
#include "ilist.h"

int inter(tSymbolTable *ST, tListOfInstr *instrList)
{
  listFirst(instrList);
  tInstr *I;
  while (1)
  {
    I = listGetData(instrList);
    
    switch (I->instType)
    {
    case I_STOP:
      // instrukce konce programu
      return 0;
      break;
    
    case I_READ:
      // instrukce pro nacteni hodnoty ze std. vstupu
      scanf("%i", &(((tData*)I->addr1)->varValue));
      break;
    
    case I_WRITE:
      // instrukce pro vypis hodnoty na std. vystup
      // pro lepsi citelnost za kazdou vypsanou hodnotu je vlozen
      // znak konce radku
      printf("%i\n", ((tData*)I->addr1)->varValue);
      break;
    
    case I_SETZERO:
      // instrukce pro nastaveni promenne na nulu
      ((tData*)I->addr1)->varValue = 0;
      break;
    
    case I_INC:
      // instrukce pro inkrementaci promenne
      ((tData*)I->addr1)->varValue++;
      break;
    
    case I_DEC:
      // instrukce pro dekrementaci promenne
      ((tData*)I->addr1)->varValue--;
      break;
    
    case I_NOT:
      // instrukce pro negaci (0 -> 1, cokoliv krome 0 -> 0)
      if (((tData*)I->addr1)->varValue == 0)
         ((tData*)I->addr3)->varValue = 1;
      else
         ((tData*)I->addr3)->varValue = 0;
      break;
    
    case I_LAB:
      // instrukce navesti
      // nic se neprovadi
      break;
      
    case I_IFGOTO:
      // instrukce pro provedeni podmineneho skoku
      if (((tData*)I->addr1)->varValue) listGoto(instrList, I->addr3);
      break;
      
    case I_GOTO:
      // instrukce skoku
      listGoto(instrList, I->addr3);
      break;
    }
    
    // prejdeme na dalsi instrukci
    // POZOR! Pokud byl proveden skok, nemelo by se posouvat na dalsi instrukci!
    // Protoze ale vime, ze skok mohl byt proveden pouze na navesti a v ramci
    // navesti se nic neprovadi, muzeme si dovolit prejit na dalsi instrukci i
    // v tomto pripade, pouze tim urychlime beh programu
    listNext(instrList);

  }
}
